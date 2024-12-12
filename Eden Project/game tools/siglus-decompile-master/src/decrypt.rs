use std::io::{Seek, SeekFrom};
use std::fs::File;

use byteorder::{LittleEndian, ReadBytesExt};

use ::Result;
use ::Script;
use ::{Variable, VariableType, Function};
use ::zip;

#[derive(Debug)]
pub struct HeaderPair {
	offset: u32,
	count: u32
}

#[derive(Debug)]
pub struct ScenePackHeader {
	header_size: u32,

	global_var_types: HeaderPair,
	global_var_name_table: HeaderPair,
	global_var_name_data: HeaderPair,

	global_func_location: HeaderPair,
	global_func_name_table: HeaderPair,
	global_func_name_data: HeaderPair,

	scene_name_table: HeaderPair,
	scene_name_data: HeaderPair,

	scene_data_table: HeaderPair,
	scene_data: HeaderPair,

	pub extra_key_use: bool,
	source_header_length: u32,	// length of the first unknown block
}

impl ScenePackHeader {
	// Beginning of reader is beginning of file
	pub fn read_global_vars<R>(&self, scene_pack: &mut R) -> Result<Vec<Variable>> 
		where R: ReadBytesExt + Seek
	{
		let mut var_types: Vec<(u32, usize)> = Vec::with_capacity(self.global_var_types.count as usize);
		scene_pack.seek(SeekFrom::Start(self.global_var_types.offset as u64)).expect("Could not seek.");
		for _ in 0..self.global_var_types.count {
			let var_type = scene_pack.read_u32::<LittleEndian>()?;
			let var_length = scene_pack.read_u32::<LittleEndian>()? as usize;
			var_types.push((var_type, var_length));
		}

		let var_names = read_strings(scene_pack, &self.global_var_name_table, &self.global_var_name_data)?;

		if var_names.len() != var_types.len() {
			return Err("Global variables count mismatch.".into());
		}

		Ok(zip(var_names, var_types).map(|(name, var_type)| {
			Variable {
				name,
				var_type: VariableType::from_pair(var_type).expect("Could not extract type")
			}
		}).collect())
	}

	pub fn read_global_funcs<R>(&self, scene_pack: &mut R) -> Result<Vec<Function>> 
		where R: ReadBytesExt + Seek 
	{
		let mut func_locations: Vec<(usize, usize)> = Vec::with_capacity(self.global_func_location.count as usize);
		scene_pack.seek(SeekFrom::Start(self.global_func_location.offset as u64)).expect("Could not seek.");
		for _ in 0..self.global_func_location.count {
			let file_index = scene_pack.read_u32::<LittleEndian>()? as usize;
			let offset = scene_pack.read_u32::<LittleEndian>()? as usize;
			func_locations.push((file_index, offset));
		}
		let func_names = read_strings(scene_pack, &self.global_func_name_table, &self.global_func_name_data)?;

		if func_names.len() != func_locations.len() {
			return Err("Global functions count mismatch.".into());
		}

		Ok(zip(func_names, func_locations).map(|(name, (file_index, offset))| {
			Function {
				name,
				file_index: Some(file_index),
				offset
			}
		}).collect())
	}

	// Returns a vector of scene names and a vector of the offset and (compressed) size in the file
	pub fn read_scene_info<R>(&self, mut reader: R) -> Result<(Vec<String>, Vec<HeaderPair>)>
		where R: ReadBytesExt + Seek
	{
		let scene_names: Vec<String> = read_strings(&mut reader, &self.scene_name_table, &self.scene_name_data)?;
		let mut scene_data_table: Vec<HeaderPair> = Vec::with_capacity(self.scene_data_table.count as usize);
		reader.seek(SeekFrom::Start(self.scene_data_table.offset as u64))?;
		for _ in 0..self.scene_data_table.count {
			scene_data_table.push(read_header_pair(&mut reader).unwrap());
		}

		if scene_names.len() != scene_data_table.len() {
			Err("Scene names and data length mismatch".into())
		} else {
			Ok((scene_names, scene_data_table))
		}
	}

	pub fn decode_scene<R>(&self, mut reader: R, HeaderPair { offset, count: length }: HeaderPair, decrypt_key: &Option<Vec<u8>>) -> Result<Vec<u8>> 
		where R: ReadBytesExt + Seek
	{
		reader.seek(SeekFrom::Start((self.scene_data.offset + offset) as u64))?;

		let mut buffer: Vec<u8> = vec![0; length as usize];
		reader.read_exact(buffer.as_mut_slice())?;

		if let Some(ref key) = decrypt_key {
			for (value, key) in buffer.iter_mut().zip(key.iter().cycle()) {
				*value ^= *key;
			}
		}

		decrypt_scene(buffer.as_mut_slice());

		let (compressed_size, decompressed_size) = {
			let mut buffer = buffer.as_slice();
			let comp = buffer.read_u32::<LittleEndian>()?;
			let decomp = buffer.read_u32::<LittleEndian>()? as usize;
			(comp, decomp)
		};

		if compressed_size != length {
			return Err(format!("Expected {} bytes, got {}.", length, compressed_size).into());
		}

		let decompressed = decompress_lzss_8(&buffer[8..], decompressed_size).map_err(|_| "Error in decompression".into());

		decompressed
	}
}

// TODO: figure out what the unknown blocks are
// struct ScenePackData

pub fn read_scene_pack_header(scene_pack: &mut File) -> Result<ScenePackHeader> {
	// Read header
	let header_size = scene_pack.read_u32::<LittleEndian>()?;
	if header_size != 0x5C {
		return Err(format!("Expected header size 0x5C, got {:x}", header_size).into());
	}
	let global_var_types = read_header_pair(scene_pack)?;
	let global_var_name_table = read_header_pair(scene_pack)?;
	let global_var_name_data = read_header_pair(scene_pack)?;

	let global_func_location = read_header_pair(scene_pack)?;
	let global_func_name_table = read_header_pair(scene_pack)?;
	let global_func_name_data = read_header_pair(scene_pack)?;

	let scene_name_table = read_header_pair(scene_pack)?;
	let scene_name_data = read_header_pair(scene_pack)?;

	let scene_data_table = read_header_pair(scene_pack)?;
	let scene_data = read_header_pair(scene_pack)?;

	let extra_key_use = scene_pack.read_u32::<LittleEndian>()? != 0;
	let source_header_length = scene_pack.read_u32::<LittleEndian>()?;

	Ok(ScenePackHeader {
		header_size,

		global_var_types,
		global_var_name_table,
		global_var_name_data,
		global_func_location,
		global_func_name_table,
		global_func_name_data,
		scene_name_table,
		scene_name_data,
		scene_data_table,
		scene_data,

		extra_key_use,
		source_header_length,
	})
}

fn read_header_pair<R>(reader: &mut R) -> Result<HeaderPair> 
	where R: ReadBytesExt 
{
	let offset = reader.read_u32::<LittleEndian>()?;
	let count = reader.read_u32::<LittleEndian>()?;
	Ok(HeaderPair {
		offset,
		count
	})
}

fn read_strings<R>(reader: &mut R, index: &HeaderPair, data: &HeaderPair) -> Result<Vec<String>> 
	where R: ReadBytesExt + Seek 
{
	if index.count != data.count {
		return Err("Index and string data length mismatch.".into());
	}

	let mut index_table: Vec<HeaderPair> = Vec::with_capacity(index.count as usize);
	reader.seek(SeekFrom::Start(index.offset as u64))?;
	for _ in 0..index.count {
		index_table.push(read_header_pair(reader)?);
	}

	let mut strings = Vec::with_capacity(data.count as usize);
	for i in 0..data.count as usize {
		let HeaderPair { offset, count } = index_table[i];
		reader.seek(SeekFrom::Start((data.offset + 2 * offset) as u64))?;

		let mut raw_utf16: Vec<u16> = vec![0; count as usize];
		reader.read_u16_into::<LittleEndian>(raw_utf16.as_mut_slice())?;
		strings.push(String::from_utf16(&raw_utf16)?);
	}

	Ok(strings)
}

fn read_strings_with_key<R>(reader: &mut R, index: &HeaderPair, data: &HeaderPair, key: u16) -> Result<Vec<String>> 
	where R: ReadBytesExt + Seek
{
	if index.count != data.count {
		return Err("Index and string data length mismatch.".into());
	}

	let mut index_table: Vec<HeaderPair> = Vec::with_capacity(index.count as usize);
	reader.seek(SeekFrom::Start(index.offset as u64))?;
	for _ in 0..index.count {
		index_table.push(read_header_pair(reader)?);
	}

	let mut strings = Vec::with_capacity(data.count as usize);
	for i in 0..data.count as usize {
		let HeaderPair { offset, count } = index_table[i];
		reader.seek(SeekFrom::Start((data.offset + 2 * offset) as u64))?;

		let mut raw_utf16: Vec<u16> = vec![0; count as usize];
		reader.read_u16_into::<LittleEndian>(raw_utf16.as_mut_slice())?;
		for ch in raw_utf16.as_mut_slice() {
			*ch ^= (i as u16).wrapping_mul(key);
		}
		strings.push(String::from_utf16(&raw_utf16)?);
	}

	Ok(strings)
}

fn read_labels<R>(mut reader: R, index: &HeaderPair) -> Result<Vec<usize>> where R: ReadBytesExt + Seek {
	let mut v: Vec<usize> = Vec::with_capacity(index.count as usize);

	reader.seek(SeekFrom::Start(index.offset as u64))?;
	for _ in 0..index.count {
		v.push(reader.read_u32::<LittleEndian>()? as usize);
	}
	
	Ok(v)
}

static XOR_KEY: [u8; 256] = [
	0x70, 0xF8, 0xA6, 0xB0, 0xA1, 0xA5, 0x28, 0x4F, 0xB5, 0x2F, 0x48, 0xFA, 0xE1, 0xE9, 0x4B, 0xDE,
	0xB7, 0x4F, 0x62, 0x95, 0x8B, 0xE0, 0x03, 0x80, 0xE7, 0xCF, 0x0F, 0x6B, 0x92, 0x01, 0xEB, 0xF8,
	0xA2, 0x88, 0xCE, 0x63, 0x04, 0x38, 0xD2, 0x6D, 0x8C, 0xD2, 0x88, 0x76, 0xA7, 0x92, 0x71, 0x8F,
	0x4E, 0xB6, 0x8D, 0x01, 0x79, 0x88, 0x83, 0x0A, 0xF9, 0xE9, 0x2C, 0xDB, 0x67, 0xDB, 0x91, 0x14,
	0xD5, 0x9A, 0x4E, 0x79, 0x17, 0x23, 0x08, 0x96, 0x0E, 0x1D, 0x15, 0xF9, 0xA5, 0xA0, 0x6F, 0x58,
	0x17, 0xC8, 0xA9, 0x46, 0xDA, 0x22, 0xFF, 0xFD, 0x87, 0x12, 0x42, 0xFB, 0xA9, 0xB8, 0x67, 0x6C,
	0x91, 0x67, 0x64, 0xF9, 0xD1, 0x1E, 0xE4, 0x50, 0x64, 0x6F, 0xF2, 0x0B, 0xDE, 0x40, 0xE7, 0x47,
	0xF1, 0x03, 0xCC, 0x2A, 0xAD, 0x7F, 0x34, 0x21, 0xA0, 0x64, 0x26, 0x98, 0x6C, 0xED, 0x69, 0xF4,
	0xB5, 0x23, 0x08, 0x6E, 0x7D, 0x92, 0xF6, 0xEB, 0x93, 0xF0, 0x7A, 0x89, 0x5E, 0xF9, 0xF8, 0x7A,
	0xAF, 0xE8, 0xA9, 0x48, 0xC2, 0xAC, 0x11, 0x6B, 0x2B, 0x33, 0xA7, 0x40, 0x0D, 0xDC, 0x7D, 0xA7,
	0x5B, 0xCF, 0xC8, 0x31, 0xD1, 0x77, 0x52, 0x8D, 0x82, 0xAC, 0x41, 0xB8, 0x73, 0xA5, 0x4F, 0x26,
	0x7C, 0x0F, 0x39, 0xDA, 0x5B, 0x37, 0x4A, 0xDE, 0xA4, 0x49, 0x0B, 0x7C, 0x17, 0xA3, 0x43, 0xAE,
	0x77, 0x06, 0x64, 0x73, 0xC0, 0x43, 0xA3, 0x18, 0x5A, 0x0F, 0x9F, 0x02, 0x4C, 0x7E, 0x8B, 0x01,
	0x9F, 0x2D, 0xAE, 0x72, 0x54, 0x13, 0xFF, 0x96, 0xAE, 0x0B, 0x34, 0x58, 0xCF, 0xE3, 0x00, 0x78,
	0xBE, 0xE3, 0xF5, 0x61, 0xE4, 0x87, 0x7C, 0xFC, 0x80, 0xAF, 0xC4, 0x8D, 0x46, 0x3A, 0x5D, 0xD0,
	0x36, 0xBC, 0xE5, 0x60, 0x77, 0x68, 0x08, 0x4F, 0xBB, 0xAB, 0xE2, 0x78, 0x07, 0xE8, 0x73, 0xBF
];

fn decrypt_scene(buffer: &mut [u8]) {
	for (i, value) in buffer.iter_mut().enumerate() {
		*value ^= XOR_KEY[i % 256];
	}
}

fn decompress_lzss_8(mut compressed: &[u8], size: usize) -> Result<Vec<u8>> {
	let mut decompressed = Vec::with_capacity(size);

	'decomp: loop {
		if compressed.is_empty() {
			break 'decomp;
		}

		let marker = compressed.read_u8()?;
		for i in 0..8 {
			if compressed.is_empty() {
				break 'decomp;
			}
			if marker & (1 << i) == 0 {
				let counter = compressed.read_u16::<LittleEndian>()? as usize;
				let (counter, offset) = ((counter & 0xF) + 2, counter >> 4);

				// Note: this is not equivalent to a memmove
				for _ in 0..counter {
					let value = decompressed[decompressed.len() - offset];
					decompressed.push(value);
				}
			} else {
				decompressed.push(compressed.read_u8()?);
			}
		}
	}

	Ok(decompressed)
}

//=====================================================================================================
#[derive(Debug)]
struct ScriptHeader {
	header_size: u32,

	bytecode_index: HeaderPair,

	string_table: HeaderPair,
	string_data: HeaderPair,

	labels: HeaderPair,
	entrypoints: HeaderPair,
	function_locations: HeaderPair,

	static_var_types: HeaderPair,
	static_var_name_table: HeaderPair,
	static_var_name_data: HeaderPair,

	static_func_location: HeaderPair,
	static_func_name_table: HeaderPair,
	static_func_name_data: HeaderPair,

	local_var_name_table: HeaderPair,
	local_var_name_data: HeaderPair,

	_unknown6: HeaderPair,
	_unknown7: HeaderPair,
}

impl ScriptHeader {
	fn read_static_vars<R>(&self, script: &mut R) -> Result<Vec<Variable>> where R: ReadBytesExt + Seek {
		let mut var_types: Vec<(u32, usize)> = Vec::with_capacity(self.static_var_types.count as usize);
		script.seek(SeekFrom::Start(self.static_var_types.offset as u64)).expect("Could not seek.");
		for _ in 0..self.static_var_types.count {
			let var_type = script.read_u32::<LittleEndian>()?;
			let var_length = script.read_u32::<LittleEndian>()? as usize;
			var_types.push((var_type, var_length));
		}

		let var_names = read_strings(script, &self.static_var_name_table, &self.static_var_name_data)?;

		if var_names.len() != var_types.len() {
			return Err("Static variables count mismatch.".into());
		}

		Ok(zip(var_names, var_types).map(|(name, var_type)| {
			Variable {
				name,
				var_type: VariableType::from_pair(var_type).expect("Could not extract type")
			}
		}).collect())
	}

	fn read_static_funcs<R>(&self, script: &mut R) -> Result<Vec<Function>> where R: ReadBytesExt + Seek {
		let mut func_locations: Vec<usize> = Vec::with_capacity(self.static_func_location.count as usize);
		script.seek(SeekFrom::Start(self.static_func_location.offset as u64)).expect("Could not seek.");
		for _ in 0..self.static_func_location.count {
			let offset = script.read_u32::<LittleEndian>()? as usize;
			func_locations.push(offset);
		}
		let func_names = read_strings(script, &self.static_func_name_table, &self.static_func_name_data)?;

		if func_names.len() != func_locations.len() {
			return Err("Static functions count mismatch.".into());
		}

		Ok(zip(func_names, func_locations).map(|(name, offset)| {
			Function {
				name,
				file_index: None,
				offset,
			}
		}).collect())
	}
}

fn read_script_header<R>(script: &mut R) -> Result<ScriptHeader> where R: ReadBytesExt {
	// Read header
	let header_size = script.read_u32::<LittleEndian>()?;
	if header_size != 0x84 {
		return Err(format!("Expected header size 0x84, got {:x}", header_size).into());
	}
	let bytecode_index = read_header_pair(script)?;
	
	let string_table = read_header_pair(script)?;
	let string_data = read_header_pair(script)?;

	let labels = read_header_pair(script)?;
	let entrypoints = read_header_pair(script)?;	// "z levels"
	let function_locations = read_header_pair(script)?;	// offsets of all functions defined in this file

	let static_var_types = read_header_pair(script)?;
	let static_var_name_table = read_header_pair(script)?;
	let static_var_name_data = read_header_pair(script)?;

	let static_func_location = read_header_pair(script)?;
	let static_func_name_table = read_header_pair(script)?;
	let static_func_name_data = read_header_pair(script)?;

	let local_var_name_table = read_header_pair(script)?;
	let local_var_name_data = read_header_pair(script)?;

	let _unknown6 = read_header_pair(script)?;
	let _unknown7 = read_header_pair(script)?;

	Ok(ScriptHeader {
		header_size,

		bytecode_index,

		string_table,
		string_data,

		labels,
		entrypoints,
		function_locations,

		static_var_types,
		static_var_name_table,
		static_var_name_data,

		static_func_location,
		static_func_name_table,
		static_func_name_data,

		local_var_name_table,
		local_var_name_data,

		_unknown6,
		_unknown7,
	})
}

impl Script {
	pub fn new<R>(mut reader: R, script_name: &str) -> Result<Self> 
		where R: ReadBytesExt + Seek
	{
		debug!("Parsing script: {}", script_name);

		let script_header = read_script_header(&mut reader)?;

		let strings = read_strings_with_key(&mut reader, &script_header.string_table, &script_header.string_data, 0x7087)?;

		let labels = read_labels(&mut reader, &script_header.labels)?;
		let entrypoints = read_labels(&mut reader, &script_header.entrypoints)?;
		

		let mut function_index = Vec::with_capacity(script_header.function_locations.count as usize);
		reader.seek(SeekFrom::Start(script_header.function_locations.offset as u64))?;
		for _ in 0..script_header.function_locations.count {
			let function_id = reader.read_u32::<LittleEndian>()? as usize;
			let offset = reader.read_u32::<LittleEndian>()? as usize;

			function_index.push((function_id, offset));
			trace!("Function {:#x} at {:#x}", function_id, offset);
		}
		

		let static_vars = script_header.read_static_vars(&mut reader)?;
		debug!("\tRead {} static variables.", static_vars.len());

		let static_funcs = script_header.read_static_funcs(&mut reader)?;
		debug!("\tRead {} static functions.", static_funcs.len());

		let local_var_names = read_strings(&mut reader, &script_header.local_var_name_table, &script_header.local_var_name_data)?;
		debug!("\tRead {} local variable names.", local_var_names.len());

		// Read bytecode
		let mut bytecode: Vec<u8> = vec![0; script_header.bytecode_index.count as usize];
		reader.seek(SeekFrom::Start(script_header.bytecode_index.offset as u64))?;
		reader.read_exact(bytecode.as_mut_slice())?;

		Ok(Script {
			name: script_name.to_string(),

			bytecode,

			strings,
			labels,
			entrypoints,
			function_index,
			static_vars,
			static_funcs,
			local_var_names,
		})
	}
}
