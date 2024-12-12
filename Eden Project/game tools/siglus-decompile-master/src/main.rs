// TODO: handle reference types properly (0x0d)
//	since they can actually be used as pointers in system functions
//	maybe even user defined commands

// TODO: attach opcode addresses to instructions in basic blocks
// 	check if a target address is already inside a basic block
//	and split it if so


// TODO: cleaning
// Stack frame into one struct
// Move to different modules
// TODO: actually look for the TODO's to fix
// 	need to actually fix them

// TODO: handle procedures

extern crate getopts;
extern crate byteorder;
extern crate itertools;
extern crate petgraph;


#[macro_use]
extern crate log;
extern crate fern;

use std::fs::File;
use std::io::{Read, Seek, SeekFrom, Cursor, Write};
use byteorder::{LittleEndian, ReadBytesExt};

use getopts::Options;

type Result<T> = std::result::Result<T, Box<std::error::Error>>;

mod decrypt;
mod control_flow;
mod expression;
mod stack;
use control_flow::{ControlFlowGraph, Instruction, Statement};
use expression::{Expression, FunctionType};
use decrypt::read_scene_pack_header;
use stack::ProgStack;


#[derive(Copy, Clone, Debug, PartialEq)]
pub enum VariableType {
	Void,           //0x00
	Int,            //0x0a
	IntList(usize), //0x0b
	IntRef,         //0x0d
	IntListRef,     // 0x0e
	Str,            //0x14
	StrList(usize), //0x15
	StrRef,         //0x17
	StrListRef,     //0x18
	Obj,            //0x51e
	ObjList(usize),
	StageElem,      //0x514
	Error,
	Unknown,
}

#[derive(Debug, Clone, PartialEq)]
pub struct Variable {
	name: String,
	var_type: VariableType,
}

// TODO: turn the expression type into a wrapper around this one
impl Variable {
	fn to_expression(self) -> Expression {
		Expression::Variable(self.clone())
	}
}

impl std::fmt::Display for Variable {
	fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
		write!(f, "{} {}", self.var_type, self.name)
	}
}

// TODO: fold this into function expressions (as a command or something)
#[derive(Debug, Clone)]
pub struct Function {
	name: String,
	file_index: Option<usize>,
	offset: usize,	// in bytes
}

impl Function {
	fn to_expression(&self) -> Expression {
		Expression::Function(FunctionType::Named(self.name.clone()))
	}
}

impl VariableType {
	// TODO: check this is good
	fn from_pair((var_type, length): (u32, usize)) -> Result<VariableType> {
		match var_type {
			0x0a => if length == 0 { Ok(VariableType::Int) } else { Err("Int variable cannot have a length.".into()) },
			0x0b => Ok(VariableType::IntList(length)),
			0x14 => if length == 0 { Ok(VariableType::Str) } else { Err("Str variable cannot have a length.".into()) },
			0x15 => Ok(VariableType::StrList(length)),
			_ => Err(format!("Unexpected var type {:#x}", var_type).into())
		}
	}

	fn from_u32(var_type: u32) -> VariableType {
		match var_type {
			0x00 => VariableType::Void,
			0x0a => VariableType::Int,
			0x0b => VariableType::IntList(0),
			0x0d => VariableType::IntRef,
			0x0e => VariableType::IntListRef,
			0x14 => VariableType::Str,
			0x15 => VariableType::StrList(0),
			0x17 => VariableType::StrRef,
			0x18 => VariableType::StrListRef,
			0x51e => VariableType::Obj,
			0x514 => VariableType::StageElem,
			_ => {
				warn!("Unexpected var type {:#x}", var_type);
				VariableType::Unknown
			}
		}
	}

	fn is_list(&self) -> bool {
		match *self {
			VariableType::IntList(_) | 
			VariableType::StrList(_) | 
			VariableType::ObjList(_) => true,
			_ => false,
		}
	}

	fn set_length(&mut self, length: usize) {
		match self {
			VariableType::IntList(ref mut len) | 
			VariableType::StrList(ref mut len) | 
			VariableType::ObjList(ref mut len) => *len = length,
			_ => warn!("Type {:?} has no length.", self),
		}
	} 
}

impl std::fmt::Display for VariableType {
	fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
		match *self {
			VariableType::Void => write!(f, "void"),
			VariableType::Int => write!(f, "int"),
			VariableType::IntList(len) => write!(f, "int[{}]", len),
			VariableType::IntRef => write!(f, "&int"),
			VariableType::IntListRef => write!(f, "&int[]"),
			VariableType::Str => write!(f, "str"),
			VariableType::StrList(len) => write!(f, "str[{}]", len),
			VariableType::StrRef => write!(f, "&str"),
			VariableType::StrListRef => write!(f, "&str[]"),
			VariableType::Obj => write!(f, "obj"),
			VariableType::ObjList(len) => write!(f, "obj[{}]", len),
			VariableType::StageElem => write!(f, "stage_elem"),
			VariableType::Error => write!(f, "ERROR"),
			VariableType::Unknown => write!(f, "???"),
		}
	}
}

// TODO: add lists (0xFFFFFFFF)
fn read_args<R>(reader: &mut R, stack: &mut ProgStack) -> Result<Vec<Expression>> where R: ReadBytesExt {
	let num_args = reader.read_u32::<LittleEndian>()?;

	let mut args = Vec::new();
	for _ in 0..num_args {
		let type_num = reader.read_u32::<LittleEndian>()?;
		if type_num == 0xFFFFFFFF {
			let list = read_args(reader, stack)?;
			args.push(Expression::List(list));
		} else {
			let arg_type = VariableType::from_u32(type_num);

			args.push(stack.get_value(arg_type));
		}
	}

	Ok(args)
}

fn print_usage(program: &str, opts: Options) {
	let brief = format!("Usage: {} [-k xorkey] [Scene.pck]", program);
	println!("{}", opts.usage(&brief));
}

fn setup_logger() -> Result<()> {
	fern::Dispatch::new()
		.format(|out, message, record| {
			out.finish(format_args!(
				"[{}][{}] {}",
				record.target(),
				record.level(),
				message
			))
		})
		.chain(
			fern::Dispatch::new()
				.level(log::LevelFilter::Debug)
				.chain(std::io::stdout())
		)
		.chain(
			fern::Dispatch::new()
				.level(log::LevelFilter::Trace)
				//.level_for("siglus_decompile", log::LevelFilter::Trace)
				.chain(std::fs::OpenOptions::new()
					.write(true)
					.create(true)
					.truncate(true)
					.open("output.log")?
				)
		)
		.apply()?;
	Ok(())
}

fn main() {
	let mut args = std::env::args();
	let prog_name = args.next().unwrap();

	let mut opts = Options::new();
	opts.optopt("k", "key", "key file to be used for decryption", "xorkey");
	opts.optopt("s", "scene", "decompile specific scene", "scenename");
	opts.optopt("m", "macro", "file containing macro rules for replacement", "macro_rules");
	opts.optflag("h", "help", "print this help menu");
	let matches = opts.parse(args).unwrap();
	if matches.opt_present("h") {
		print_usage(&prog_name, opts);
		return;
	}
	if matches.free.len() > 1 {
		print_usage(&prog_name, opts);
		return;
	}

	// Read decryption key if provided
	let decrypt_key: Option<Vec<u8>> = matches.opt_str("k").map(|keyfile| {
		let mut keyfile = File::open(keyfile).expect("Could not open key!");
		let mut key:Vec<u8> = vec![0; 16];
		keyfile.read_exact(key.as_mut_slice()).expect("Could not read key!");

		key
	});

	let target_scene: Option<String> = matches.opt_str("s");

	// Set up logging to stdout and to an output log
	setup_logger().unwrap();

	let filename: &str = matches.free.first().map(String::as_str).unwrap_or("Scene.pck");

	let mut scene_pack = match File::open(filename) {
		Ok(v) => v,
		Err(e) => {
			eprintln!("Could not open scene pack! Error: {}", e);
			return;
		}
	};

	let scene_pack_header = read_scene_pack_header(&mut scene_pack).expect("Error reading scene pack header.");

	if decrypt_key.is_some() != scene_pack_header.extra_key_use {
		warn!("Key (not) expected");
	}

	// Read global variables
	let global_vars: Vec<Variable> = scene_pack_header.read_global_vars(&mut scene_pack).unwrap(); 
	// Read global functions
	let global_functions: Vec<Function> = scene_pack_header.read_global_funcs(&mut scene_pack).unwrap();
	// Read scenes
	let (scene_names, scene_data_table) = scene_pack_header.read_scene_info(&mut scene_pack).unwrap();

	if let Some(ref name) = target_scene {
		if !scene_names.contains(name) {
			error!("Target scene {} does not exist!", name);
			return;
		}
	}

	std::fs::create_dir_all("Scene").expect("Could not create output directory!");

	info!("Parsing {} scenes.", scene_names.len());
	debug!("{:?}", scene_names);

	// TODO: remove the take
	let scene_table = {
		zip(scene_names, scene_data_table).filter(|&(ref scene_name, _)|
			if let Some(ref name) = target_scene { name == scene_name }
			else { true }
		).take(1)
	};

	for (scene_name, index) in scene_table {
		let scene = scene_pack_header.decode_scene(&mut scene_pack, index, &decrypt_key).unwrap();
		
		let script = Script::new(Cursor::new(scene), &scene_name).expect("Could not parse script");

		let graph_path = format!("Scene/{}", &scene_name);
		std::fs::create_dir_all(&graph_path).expect("Could not create graph output directory!");

		// Construct file structure
		let source_file = script.decompile(&global_vars, &global_functions, graph_path).expect("Error parsing bytecode.");

		let mut out = File::create(format!("Scene/{}.ss", &scene_name)).expect("Could not create output file for writing");
		source_file.write(&mut out).expect("Error writing file.");	
	}
}

//==============================================================================
struct FunctionPrototype {
	name: String,
	// list of parameters (type, name)
	parameters: Vec<Variable>,
	// return type
	// return: Option<VariableType>
}

impl std::fmt::Display for FunctionPrototype {
	fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
		write!(f, "{}({})", self.name, format_list(self.parameters.iter()))
	}
}

struct SourceFile {
	main: Vec<Statement>,

	functions: Vec<(FunctionPrototype, Vec<Statement>)>,
}

impl SourceFile {
	fn write<F: Write>(&self, out: &mut F) -> Result<()> {
		for statement in self.main.iter() {
			statement.write(out, 0)?;
		}
		for &(ref f, ref block) in self.functions.iter() {
			write!(out, "\nfunction {} {{ \n", f)?;
			for statement in block.iter() {
				statement.write(out, 1)?;
			}
			write!(out, "}} \n")?;
		}
		Ok(())
	}
}

// ===================================================
#[derive(Debug)]
struct Script {
	name: String,

	bytecode: Vec<u8>,

	labels: Vec<usize>,
	entrypoints: Vec<usize>,
	// Pairs of (Function index, file offset)
	function_index: Vec<(usize, usize)>,

	strings: Vec<String>,

	static_vars: Vec<Variable>,
	static_funcs: Vec<Function>,

	local_var_names: Vec<String>,
}

impl Script {
	fn decompile(&self, global_vars: &Vec<Variable>, global_funcs: &Vec<Function>, graph_path: String) -> Result<SourceFile> {
		// Create function index from global functions + static functions
		let mut function_table = global_funcs.to_vec();
		function_table.extend_from_slice(&self.static_funcs);

		let mut global_var_table = global_vars.to_vec();
		global_var_table.extend_from_slice(&self.static_vars);

		// Parse main entrypoints
		let main = {
			// z_level, address
			let entrypoints: Vec<(usize, usize)> = self.entrypoints.iter().cloned().enumerate().filter(|&(_, address)| address != 0).collect();

			let mut graph = ControlFlowGraph::main(&entrypoints);

			// Vec<(address, stack)>
			let mut block_list = entrypoints.into_iter().rev().map(|(_, address)| (address, ProgStack::new())).collect();

			// Parse the bytecode to construct the control flow graph
			self.parse_bytecode(block_list, &mut graph)?;

			graph.replace_ref(&global_var_table, &function_table, &[]);

			let mut graph_out = File::create(format!("{}/{}.gv", &graph_path, "main")).expect("Could not create output file for writing");
			graph.write_graph(&mut graph_out).unwrap_or_else(|e| error!("{}", e));

			// Structure loops and two ways and everything else
			graph.structure_statements()
		};

		// Parse functions
		let mut functions = Vec::new();
		for &(index, address) in self.function_index.iter() {
			let fn_name = &function_table[index].name;
			trace!("Decompiling function: {}", fn_name);


			let mut graph = ControlFlowGraph::function(address);

			let mut block_list = vec![(address, ProgStack::new())];

			let (num_params, mut local_vars) = self.parse_bytecode(block_list, &mut graph)?.unwrap();

			graph.replace_ref(&global_var_table, &function_table, &local_vars);

			let mut graph_out = File::create(format!("{}/{}.gv", &graph_path, fn_name)).expect("Could not create output file for writing");
			graph.write_graph(&mut graph_out).unwrap_or_else(|e| error!("{}", e));


			let block = graph.structure_statements();

			local_vars.truncate(num_params);

			let prototype = FunctionPrototype {
				name: fn_name.clone(),
				parameters: local_vars
			};

			functions.push((prototype, block));
		}


		let source = SourceFile {
			main,
			functions,
		};

		Ok(source)
	}

	// If parsing a function, returns Some((num_params, local var list))
	fn parse_bytecode(&self, mut block_list: Vec<(usize, ProgStack)>, graph: &mut ControlFlowGraph) -> Result<Option<(usize, Vec<Variable>)>> {
		let mut bytecode = Cursor::new(&self.bytecode);

		let mut params_done = false;
		let mut num_params = 0;

		// List of parameters + local variables
		// Kept in a single array for ease of access
		let mut local_vars = Vec::new();

		while let Some((block_address, mut stack)) = block_list.pop() {
			let block_id = graph.get_block_id_at(block_address);

			if graph.traversed(block_id) {
				continue;
			} else {
				graph.set_traversed(block_id);
			}

			bytecode.seek(SeekFrom::Start(block_address as u64))?;
			//debug!("\tParsing block {} at {:#x}", block_id, block_address);

			'block_loop: loop {
				let address = bytecode.position() as usize;
				let opcode = bytecode.read_u8()?;
				//debug!("\t\tParsing opcode: {:#02x}", opcode);
				match opcode {
					// Line number - debugging purposes
					0x01 => graph.add_inst(block_id, Instruction::Line(bytecode.read_u32::<LittleEndian>()?)),
					// Push
					0x02 => {
						let value_type = VariableType::from_u32(bytecode.read_u32::<LittleEndian>()?);
						let value = bytecode.read_u32::<LittleEndian>()?;
						stack.push(match value_type {
							VariableType::Int => Expression::RawInt(value as i32),
							VariableType::Str => Expression::RawString(self.strings[value as usize].clone()),
							_ => panic!("Unexpected value type pushed at address {:#x}: {:?}", address, value_type),
						});
					},
					// Pop
					0x03 => {
						let value_type = VariableType::from_u32(bytecode.read_u32::<LittleEndian>()?);
						match value_type {
							VariableType::Void => {},
							VariableType::Int | VariableType::Str => {
								let value = stack.pop(value_type);
								if value.has_side_effect() {
									graph.add_inst(block_id, value.to_inst());
								}
							},
							_ => panic!("Unexpected value type popped at address {:#x}: {:?}", address, value_type),
						}
					},
					// Duplicate
					0x04 => {
						let value_type = VariableType::from_u32(bytecode.read_u32::<LittleEndian>()?);
						match value_type {
							VariableType::Int | VariableType::Str | VariableType::Unknown => {
								let value = stack.pop(value_type);
								// Move the value into a variable if it has side effects
								if value.has_side_effect() {
									let (new_var, inst) = create_variable(value);
									graph.add_inst(block_id, inst);

									stack.push(new_var.clone());
									stack.push(new_var);
								} else {
									stack.push(value.clone());
									stack.push(value);
								}
							},
							_ => panic!("Unexpected value type duplicated at address {:#x}: {:?}", address, value_type),
						}
					},
					// Close frame
					0x05 => {
						let value = stack.handle_frame().unwrap();
						stack.push(value);
					},
					// Duplicate stack frame
					0x06 => stack.dup_frame(),
					// Declare parameter/local variable
					0x07 => {
						let mut var_type = VariableType::from_u32(bytecode.read_u32::<LittleEndian>()?);
						let name_index = bytecode.read_u32::<LittleEndian>()? as usize;
						// Length for lists is on top of the stack
						if var_type.is_list() {
							let length = stack.pop(VariableType::Int);
							if let Expression::RawInt(len) = length {
								var_type.set_length(len as usize);
							} else {
								warn!("Declaring variable of type {:?} - nonconst length ({}) not supported.", var_type, length);
							}
						}

						let var_name = self.local_var_names[name_index].to_string();
						let var = Variable {
							name: var_name,
							var_type
						};
						if params_done {
							graph.add_inst(block_id, Instruction::Expression(var.clone().to_expression()));
						}
						local_vars.push(var);
					},
					// Open frame
					0x08 => stack.open_frame(),
					// Finish declaring parameters
					0x09 => {
						params_done = true;
						num_params = local_vars.len();
					},
					// Jump
					0x10 =>  {
						let jmp_label = bytecode.read_u32::<LittleEndian>()? as usize;
						let jump_address = self.labels[jmp_label];

						let next_block_id = graph.get_block_id_at(jump_address);

						// TODO: check if not traversed/on list already to avoid unnecessary clone
						block_list.push((jump_address, stack.clone()));

						graph.add_successor(block_id, next_block_id);

						break 'block_loop;
					},
					// Jump if (not) zero
					0x11 | 0x12 => {
						let jmp_label = bytecode.read_u32::<LittleEndian>()? as usize;
						let jump_address = self.labels[jmp_label];
						let next_address = bytecode.position() as usize;

						let jump_block_id = graph.get_block_id_at(jump_address);
						let next_block_id = graph.get_block_id_at(next_address);
						
						let value = stack.pop(VariableType::Int);

						let condition = if opcode == 0x12 {
							value.negate()
						} else {
							value
						};

						graph.add_branch(block_id, condition, jump_block_id, next_block_id);

						// Add blocks to list
						block_list.push((jump_address, stack.clone()));
						block_list.push((next_address, stack.clone()));

						break 'block_loop;
					},
					// Short call
					0x13 | 0x14 => {
						let call_label = bytecode.read_u32::<LittleEndian>()? as usize;
						let call_address = self.labels[call_label];

						let call_block = graph.get_block_id_at(call_address);
						// TODO: call_block.is_function = true

						let mut args = read_args(&mut bytecode, &mut stack)?;
						args.reverse();

						// TODO: visit this block
						//block_list.push((call_address, stack.clone()));

						let call = Expression::procedure_call(call_label, args);
						stack.push(call);
					},
					// Return
					0x15 => {
						let mut args = read_args(&mut bytecode, &mut stack)?;
						args.reverse();

						graph.add_inst(block_id, Instruction::Return(args));

						break 'block_loop;
					},
					// End of script
					0x16 => {
						break 'block_loop;
					},
					// Assignment
					0x20 => {
						let _unknown1 = bytecode.read_u32::<LittleEndian>()?;
						let var_type = VariableType::from_u32(bytecode.read_u32::<LittleEndian>()?);
						let _unknown2 = bytecode.read_u32::<LittleEndian>()?;

						let value = stack.pop(var_type);

						let variable = stack.handle_frame().unwrap();

						graph.add_inst(block_id, Instruction::Binding { lhs: variable, rhs: value });
					},
					// Calc1
					0x21 => {
						let var_type = VariableType::from_u32(bytecode.read_u32::<LittleEndian>()?);
						let op = bytecode.read_u8()?;

						if var_type != VariableType::Int {
							panic!("Unknown operation: calc1 {:?} {:#02x}", var_type, op);
						}

						let var = stack.pop(var_type);

						stack.push(Expression::calc1(var, op));
					},
					// Calc2
					0x22 => {
						let type1 = VariableType::from_u32(bytecode.read_u32::<LittleEndian>()?);
						let type2 = VariableType::from_u32(bytecode.read_u32::<LittleEndian>()?);
						let op = bytecode.read_u8()?;

						match (type1, type2) {
							(VariableType::Int, VariableType::Int) =>  {
								let rhs = stack.pop(VariableType::Int);
								let lhs = stack.pop(VariableType::Int);
								stack.push(Expression::calc2_int(lhs, rhs, op));
							},
							(VariableType::Str, VariableType::Str) => {
								let rhs = stack.pop(VariableType::Str);
								let lhs = stack.pop(VariableType::Str);
								stack.push(Expression::calc2_str(lhs, rhs, op));
							},
							(_, _) => panic!("Unknown operation: {:?} {:#02x} {:?}", type1, op, type2),
						}
					},
					// Function call
					0x30 => {
						let option = bytecode.read_u32::<LittleEndian>()?;

						// arguments popped off stack in reverse order
						let mut args = read_args(&mut bytecode, &mut stack)?;
						args.reverse();

						// number of extra parameters
						let num_extra = bytecode.read_u32::<LittleEndian>()?;
						let mut extra_params: Vec<u32> = Vec::new();
						for _ in 0..num_extra {
							extra_params.push(bytecode.read_u32::<LittleEndian>()?);
						}
						extra_params.reverse();

						let return_type = VariableType::from_u32(bytecode.read_u32::<LittleEndian>()?);

						let function = Box::new(stack.handle_frame().unwrap());

						let extra = if check_function_extra(&function) {
							Some(bytecode.read_u32::<LittleEndian>()?)
						} else { None };

						let call = Expression::FunctionCall{function, option, args, extra_params, return_type, extra};
						if return_type == VariableType::Void {
							graph.add_inst(block_id, Instruction::Expression(call));
						} else {
							stack.push(call);
						}
					},
					// Add text
					0x31 => {
						let text = stack.pop(VariableType::Str);
						let id = bytecode.read_u32::<LittleEndian>()?;
						graph.add_inst(block_id, Instruction::AddText(text, id));
					},
					// Set name
					0x32 => {
						let name = stack.pop(VariableType::Str);
						graph.add_inst(block_id, Instruction::SetName(name));
					},
					_ => {
						warn!("Unexpected opcode at address {:#x}: {:#02x}", address, opcode);
						graph.add_inst(block_id, Instruction::Nop(opcode));
					}
				}

				let new_address = bytecode.position() as usize;
				// Create a new block if next address is labelled
				if self.is_labelled(new_address) {
					block_list.push((new_address, stack));
					let succ =  graph.get_block_id_at(new_address);
					graph.add_successor(block_id, succ);
					break 'block_loop;
				}
			}
		}

		// Return list of parameters if we processed a function
		if params_done {
			Ok(Some((num_params, local_vars)))
		} else {
			Ok(None)
		}
	}

	fn is_labelled(&self, address: usize) -> bool {
		self.labels.contains(&address)
	}
}

//
fn create_variable(value: Expression) -> (Expression, Instruction) {
	static mut NUM_VARS: usize = 0;

	let var_name = format!("var{}", unsafe {
		NUM_VARS += 1;
		NUM_VARS
	});

	let var = Variable {
		name: var_name,
		var_type: value.get_type()
	}.to_expression();

	let bind_inst = Instruction::Binding {
		lhs: var.clone(),
		rhs: value,
	};

	(var, bind_inst)
}

// TODO: this might not actually belong here - might be non system agnostic

// Checks if a function has an extra annoying (counter?) thing that makes the size
// of the instructions not known without looking at the stack
fn check_function_extra(function: &Expression) -> bool {
	if let &Expression::System(f) = function {
		if f == 0xc || f == 0x12 || f == 0x13 || f == 0x4c {
			true
		} else {
			false
		}
	} else {
		false
	}
}

////////////////////////////////////////////////////////////////////////////////

fn zip<A, B>(a: A, b: B) -> std::iter::Zip<<A as IntoIterator>::IntoIter, <B as IntoIterator>::IntoIter>
	where A: IntoIterator, B: IntoIterator
{
	a.into_iter().zip(b)
}

fn format_list<T: std::fmt::Display>(list: impl Iterator<Item = T>) -> String {
	let mut it = list.into_iter();
	if let Some(elem) = it.next() {
		let mut comma_separated = elem.to_string();

		for elem in it {
			comma_separated.push_str(", ");
			comma_separated.push_str(&elem.to_string());
		}
		comma_separated
	} else {
		String::new()
	}
}
