#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <direct.h>
#include <windows.h>
#include <vector>
#include <string>
#include <set>



typedef uint8_t byte;

typedef struct parity_node_s
{
	uint32_t parity_name;		//hash code
	uint32_t parity_length;
}parity_node_t;

typedef struct magic_header_s
{
	char magic[8];
	parity_node_t top;		//parity name
	parity_node_t table;	//file table
}magic_header_t;

typedef struct stream_reader_s
{
	parity_node_t node;
	uint32_t offset_of_file;
}stream_reader_t;


std::vector <stream_reader_t> reader_list;

std::vector <std::string> script_name_list;


__declspec(naked)void decrypt_value()
{
	__asm
	{
		push    ebp;
		mov     ebp, esp;
		mov     eax, dword ptr[ebp + 0x8];
		mov     edx, dword ptr[ebp + 0xC];
		xor eax, edx;
		add     eax, 0x5D588B65;
		xor eax, edx;
		add     eax, 0xCA62C1D6;
		mov     ecx, eax;
		xor eax, edx;
		add     eax, 0x8F1BBCDC;
		shr     ecx, 0x1B;
		xor eax, edx;
		add     eax, 0x6C078965;
		rol     eax, cl;
		pop     ebp;
		retn;
	}
}

__declspec(naked)void calc_decrypt_xor_value()
{
	__asm
	{
		push    ebp;
		mov     ebp, esp;
		push    edi;
		mov     edi, dword ptr[ebp + 0x8];
		mov     eax, dword ptr[ebp + 0xC];
		mov     edx, eax;
		add     eax, 0x5D588B65;
		xor eax, edx;
		add     edx, 0xCA62C1D6;
		xor edx, eax;
		add     eax, 0x8F1BBCDC;
		xor eax, edx;
		add     edx, 0x6C078965;
		xor edx, eax;
		mov     dword ptr[edi], eax;
		mov     dword ptr[edi + 0x4], edx;
		pop     edi;
		pop     ebp;
		retn;
	}
}
byte hash_table[] =
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x38,0x2F,0x33,0x3C,0x40,0x3B,0x2A,0x2E,0x31,0x30,0x26,0x44,0x35,0x28,0x3E,0x12,
	0x02,0x22,0x06,0x20,0x1A,0x1C,0x0F,0x11,0x18,0x17,0x42,0x2B,0x3A,0x37,0x34,0x0C,
	0x41,0x08,0x1D,0x07,0x15,0x21,0x05,0x1E,0x0A,0x14,0x0E,0x10,0x09,0x27,0x1F,0x0B,
	0x23,0x16,0x0D,0x01,0x25,0x04,0x1B,0x03,0x13,0x24,0x19,0x2D,0x12,0x29,0x32,0x3F,
	0x3D,0x08,0x1D,0x07,0x15,0x21,0x05,0x1E,0x0A,0x14,0x0E,0x10,0x09,0x27,0x1F,0x0B,
	0x23,0x16,0x0D,0x01,0x25,0x04,0x1B,0x03,0x13,0x24,0x19,0x2C,0x39,0x43,0x36,0x00,
	0x4B,0xA9,0xA7,0xAF,0x50,0x52,0x91,0x9F,0x47,0x6B,0x96,0xAB,0x87,0xB5,0x9B,0xBB,
	0x99,0xA4,0xBF,0x5C,0xC6,0x9C,0xC2,0xC4,0xB6,0x4F,0xB8,0xC1,0x85,0xA8,0x51,0x7E,
	0x5F,0x82,0x73,0xC7,0x90,0x4E,0x45,0xA5,0x7A,0x63,0x70,0xB3,0x79,0x83,0x60,0x55,
	0x5B,0x5E,0x68,0xBA,0x53,0xA1,0x67,0x97,0xAC,0x71,0x81,0x59,0x64,0x7C,0x9D,0xBD,
	0x9D,0xBD,0x95,0xA0,0xB2,0xC0,0x6F,0x6A,0x54,0xB9,0x6D,0x88,0x77,0x48,0x5D,0x72,
	0x49,0x93,0x57,0x65,0xBE,0x4A,0x80,0xA2,0x5A,0x98,0xA6,0x62,0x7F,0x84,0x75,0xBC,
	0xAD,0xB1,0x6E,0x76,0x8B,0x9E,0x8C,0x61,0x69,0x8D,0xB4,0x78,0xAA,0xAE,0x8F,0xC3,
	0x58,0xC5,0x74,0xB7,0x8E,0x7D,0x89,0x8A,0x56,0x4D,0x86,0x94,0x9A,0x4C,0x92,0xB0
};
__declspec(naked)void hash_string()
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		add     esp, -0x24
		push    esi
		push    edi
		mov     esi, dword ptr[ebp + 0xC]
		mov     edi, offset hash_table
		mov     eax, dword ptr[ebp + 0x8]
		jmp     label2
		label1 :
		inc     esi
			xor edx, edx
			mov     dl, byte ptr[ecx + edi]
			xor eax, edx
			mov     edx, eax
			shl     edx, 0x4
			add     eax, edx
			shl     edx, 0x2
			add     eax, edx
			label2 :
		xor ecx, ecx
			mov     cl, byte ptr[esi]
			cmp     cl, 0x0
			jnz     label1
			pop     edi
			pop     esi
			mov     esp, ebp
			pop     ebp
			retn
	}
}

void decrypt(int count, unsigned char* data, uint16_t key[4])
{
	uint16_t ctx_key[4];

	for (int i = 0; i < 4; i++) {
		ctx_key[i] = key[i];
	}

	for (int i = 0; i < count; i++)
	{
		uint16_t* curr = (uint16_t*)&data[i * sizeof(parity_node_t)];

		for (int j = 0; j < 4; j++) {
			curr[j] ^= ctx_key[j];
			ctx_key[j] += curr[j];
		}
	}
}

void encrypt(int count, unsigned char* data, uint16_t key[4])
{
	uint16_t ctx_key[4];

	for (int i = 0; i < 4; i++) {
		ctx_key[i] = key[i];
	}

	for (int i = 0; i < count; i++)
	{
		uint16_t* curr = (uint16_t*)&data[i * sizeof(parity_node_t)];

		for (int j = 0; j < 4; j++) {
			uint16_t temp = curr[j];
			curr[j] ^= ctx_key[j];
			ctx_key[j] += temp;
		}
	}
}

//void decrypt_buffer(uint32_t decrypt_max_count,uint64_t xor_value,byte* decrypt_data,byte* undecrypt_data)
//{
//	for(uint32_t i=0;i<decrypt_max_count;i++)
//	{
//		((uint64_t*)decrypt_data)[i] = ((uint64_t*)undecrypt_data)[i] ^ xor_value;
//		uint16_t* xor_ptr = (uint16_t*)&xor_value;
//		uint16_t* dec_ptr = (uint16_t*)&((uint64_t*)decrypt_data)[i];
//		parity_node_t* node = (parity_node_t*)dec_ptr;
//
//		xor_ptr[0] += dec_ptr[0];
//		xor_ptr[1] += dec_ptr[1];
//		xor_ptr[2] += dec_ptr[2];
//		xor_ptr[3] += dec_ptr[3];
//	}
//}

typedef uint32_t(*parity_check_func_t)(uint32_t parity_value, uint32_t size);
typedef uint32_t(*parity_get_decrypt_xor_func_t)(uint16_t* parity_xor_value, uint32_t key);
typedef uint32_t(*hash_string_func_t)(uint32_t top_name, const char* hash_name);

parity_check_func_t parity_check_func = (parity_check_func_t)&decrypt_value;
parity_get_decrypt_xor_func_t patry_get_decrypt_xor_func = (parity_get_decrypt_xor_func_t)&calc_decrypt_xor_value;
hash_string_func_t hash_string_func = (hash_string_func_t)&hash_string;


char res_name[32];

char* get_buffer_file_type(byte* raw)
{

	if (!strncmp((char*)raw, "OggS", 4))
	{
		return ".ogg";
	}
	else if (!strncmp((char*)raw, "\x89PNG", 4))
	{
		return ".png";
	}
	else
		if (!strncmp((char*)raw, "\x8aMNG", 4))
		{
			return ".mng";
		}
		else if (!strncmp(res_name, "script", 6))
		{
			return ".txt";
		}
		else if (!strncmp(res_name, "system", 6))
		{
			return "";
		}

	return 0;
}

char* hash_get_system_name(uint32_t Name, uint32_t DataName)
{
	char* name_array[] = {
	"gui.nut","macro.nut","system.nut","config.nut","saveload.nut","shortcut.nut","select.nut","message.nut","dialog.nut","x_rooms.nut","staff_roll.nut",
	"voicemap.txt","imagemap.txt","actor_map.txt","shortcut.mng","msgbox.mng","x_music.txt","x_cg_page1.txt","x_cg_page2.txt","x_cg_page3.txt","x_cg_page4.txt",
	"x_cg_page6.txt","x_cg_page7.txt","x_cg_page8.txt","x_scene.txt","x_actor.txt","x_actor_bg.txt","title.mng","load.mng","config.mng","config_items.mng",
	"config_shortcut.mng","dialog.mng","log.mng","chapter.mng","date.mng","staff_roll_text.mng","staff_roll.mng","x_music.mng","x_cg.mng",
	"x_cg_icon.mng","x_scene.mng","x_scene_icon.mng","x_actor.mng","save.mng","select.mng"
	};

	for (int i = 0; i < ARRAYSIZE(name_array); i++)
	{
		uint32_t hashName = hash_string_func(Name, name_array[i]);
		if (hashName == DataName)
		{
			return name_array[i];
		}
	}
	return 0;
}

char* hash_get_script_name(uint32_t Name, uint32_t DataName)
{
	static char script_name_return[256];
	char* name_array[] = { "story.txt","start.txt","titlelogo.txt","title.txt","story.txt","@test.txt" ,
"title.txt",
"titlelogo.txt",
"title.txt",
"sinarioTORM1.txt",
"sinarioTORM2.txt",
"sinarioTORM3.txt",
"sinarioTORM4.txt",
"sinarioTORM5.txt",
"sinarioTORM6.txt",
"sinarioTORM7.txt",
"sinarioTORM8.txt",
"sinarioTORM9.txt",
"sinarioTORM10.txt",
"sinarioTORM11.txt",
"sinarioTORM12.txt",
"sinarioTORM13.txt",
"sinarioTORM14.txt",
"sinarioTORM15.txt",
"sinarioTORM16.txt",
"sinarioTORM17.txt",
"sinarioTORM18.txt",
"sinarioTORM19.txt",
"sinarioTORM20.txt",
"sinarioTORM21.txt",
"sinarioTORM22.txt",
"sinarioTORM23.txt",
"sinarioTORM24.txt",
"sinarioTORM25.txt",
"sinarioTORM26.txt",
"sinarioTORM27.txt",
"sinarioTORK1.txt",
"sinarioTORK2.txt",
"sinarioTORK3.txt",
"sinarioTORK4.txt",
"sinarioTORK5.txt",
"sinarioTORK6.txt",
"sinarioTORK7.txt",
"sinarioTORK8.txt",
"sinarioTORK9.txt",
"sinarioTORK10.txt",
"sinarioTORK11.txt",
"sinarioTORK12.txt",
"sinarioTORK13.txt",
"sinarioTORK14.txt",
"sinarioTORK15.txt",
"sinarioTORK16.txt",
"sinarioTORK17.txt",
"sinarioTORK18.txt",
"sinarioTORK19.txt",
"sinarioTORK20.txt",
"sinarioTORK21.txt",
"sinarioTORK22.txt",
"sinarioTORK23.txt",
"sinarioTORK24.txt",
"sinarioTORK25.txt",
"sinarioTORK26.txt",
"sinarioTORK27.txt",
"sinarioTORK28.txt",
"sinarioTORK29.txt",
"sinarioTORK30.txt",
"sinarioTORK31.txt",
"sinarioTORK32.txt",
"sinarioTORK33.txt",
"sinarioTORK34.txt",
"sinarioTORK35.txt",
"sinarioTORK36.txt",
"sinarioTORK37.txt",
"sinarioTORK38.txt",
"sinarioTORK39.txt",
"sinarioTORK40.txt",
"sinarioTORK41.txt",
"sinarioTORK42.txt",
"sinarioTORT1.txt",
"sinarioTORT2.txt",
"sinarioTORT3.txt",
"sinarioTORT4.txt",
"sinarioTORT5.txt",
"sinarioTORT6.txt",
"sinarioTORT7.txt",
"sinarioTORT8.txt",
"sinarioTORT30.txt",
"sinarioTORT31.txt",
"sinarioTORT9.txt",
"sinarioTORT10.txt",
"sinarioTORT33.txt",
"sinarioTORT11.txt",
"sinarioTORT12.txt",
"sinarioTORT14.txt",
"sinarioTORT15.txt",
"sinarioTORT16.txt",
"sinarioTORT17.txt",
"sinarioTORT19.txt",
"sinarioTORT20.txt",
"sinarioTORT21.txt",
"sinarioTORT22.txt",
"sinarioTORT23.txt",
"sinarioTORT24.txt",
"sinarioTORT25.txt",
"sinarioTORT26.txt",
"sinarioTORT27.txt",
"sinarioTORH1.txt",
"sinarioTORH2.txt",
"sinarioTORH3.txt",
"sinarioTORH4.txt",
"sinarioTORH5.txt",
"sinarioTORH6.txt",
"sinarioTORH7.txt",
"sinarioTORH8.txt",
"sinarioTORH9.txt",
"sinarioTORH10.txt",
"sinarioTORH11.txt",
"sinarioTORH12.txt",
"sinarioTORH13.txt",
"sinarioTORH14.txt",
"sinarioTORH15.txt",
"sinarioTORH16.txt",
"sinarioTORH17.txt",
"sinarioTORH18.txt",
"sinarioTORH19.txt",
"sinarioTORH20.txt",
"sinarioTORH21.txt",
"sinarioTORH22.txt",
"sinarioTORH23.txt",
"sinarioTORH24.txt",
"sinarioTORH25.txt",
"sinarioTORH26.txt",
"sinarioTORH27.txt",
"sinarioTORO1.txt",
"sinarioTORO2.txt",
"sinarioTORO3.txt",
"sinarioTORO4.txt",
"sinarioTORO5.txt",
"sinarioTORO6.txt",
"sinarioTORO7.txt",
"sinarioTORO8.txt",
"sinarioTORO9.txt",
"sinarioTORO10.txt",
"sinarioTORO11.txt",
"sinarioTORO12.txt",
"sinarioTORO13.txt",
"sinarioTORO14.txt",
"sinarioTORO15.txt",
"sinarioTORO16.txt",
"sinarioTORO17.txt",
"sinarioTORO18.txt",
"sinarioTORO19.txt",
"sinarioTORO20.txt",
"sinarioTORO21.txt",
"sinarioTORO22.txt",
"sinarioTORO23.txt",
"sinarioTORO24.txt",
"sinarioTORO25.txt",
"sinarioTORO26.txt",
"sinarioTORO27.txt",
"sinarioTORO28.txt",
"sinarioTORO29.txt",
"sinarioTORO30.txt",
"sinarioTORF1.txt",
"sinarioTORF2.txt",
"sinarioTORF3.txt",
"sinarioTORF4.txt",
"sinarioTORF5.txt",
"sinarioTORF6.txt",
"sinarioTORF7.txt",
"sinarioTORF8.txt",
"sinarioTORF9.txt",
"sinarioTORF10.txt",
"sinarioTORF11.txt",
"sinarioTORF12.txt",
"sinarioTORF13.txt",
"sinarioTORF14.txt",
"sinarioTORF15.txt",
"sinarioTORF16.txt",
"sinarioTORF17.txt",
"sinarioTORF18.txt",
"sinarioTORF19.txt"};

	for (int i = 0; i < ARRAYSIZE(name_array); i++)
	{
		uint32_t hashName = hash_string_func(Name, name_array[i]);
		if (hashName == DataName)
		{
			return name_array[i];
		}
	}


	for (uint32_t i = 0; i < script_name_list.size(); i++)
	{
		std::string script_name = script_name_list[i];
		script_name.append(".txt");

		uint32_t hashName = hash_string_func(Name, script_name.c_str());

		if (hashName == DataName)
		{
			strcpy(script_name_return, script_name.c_str());
			return script_name_return;
		}
	}



	return 0;
}

void load_named_set(byte* buffer, uint32_t length)
{
	std::set <std::string> erase_set;
	char* text = new char[length + 1];

	memset(text, 0, length + 1);
	memcpy(text, buffer, length);

	char* pos = text;
	while ((pos = (char*)strstr(pos, ".call ")) != NULL)
	{
		pos += (sizeof(".call ") - 1);
		char* next_pos = strstr(pos, "\r\n");
		if (!next_pos) break;
		*next_pos = 0;

		std::string script_name = pos;

		if (erase_set.find(script_name) == erase_set.end())
		{
			erase_set.insert(script_name);
			script_name_list.push_back(script_name);
		}

		pos = next_pos + (sizeof("\r\n") - 1);
	}
}
void export_file_reader(FILE* f, magic_header_t* header, char* export_path)
{
	char write_name[260];

	//get script name set
	if (strcmp(res_name, "script") == 0)
	{
		uint32_t hashName = hash_string_func(header->top.parity_name, "story.txt");
		for (uint32_t i = 0; i < reader_list.size(); i++)
		{
			parity_node_t node_info;
			if (reader_list[i].node.parity_name == hashName)
			{
				fseek(f, reader_list[i].offset_of_file, SEEK_SET);
				uint32_t allocate_length = ((reader_list[i].node.parity_length + 0x3F) & 0xFFFFFFC0);

				byte* undecrypt_data = new byte[allocate_length];
				byte* decrypt_data = new byte[allocate_length];

				fread(&node_info, sizeof(node_info), 1, f);
				fread(undecrypt_data, reader_list[i].node.parity_length, 1, f);

				uint32_t parity_value = parity_check_func(header->top.parity_name, reader_list[i].node.parity_length);

				//if(parity_value != node_info.parity_name)
				//{
				//	continue;
				//}

				uint16_t key[4];
				uint32_t decrypt_max_count = allocate_length / sizeof(uint64_t);

				patry_get_decrypt_xor_func(key, node_info.parity_name);

				memcpy(decrypt_data, undecrypt_data, allocate_length);
				decrypt(decrypt_max_count, decrypt_data, key);

				//decrypt_buffer(decrypt_max_count,xor_value,decrypt_data,undecrypt_data);

				load_named_set(decrypt_data, reader_list[i].node.parity_length);

				delete[] undecrypt_data;
				delete[] decrypt_data;
				break;
			}
		}
	}


	for (uint32_t i = 0; i < reader_list.size(); i++)
	{
		parity_node_t node_info;
		fseek(f, reader_list[i].offset_of_file, SEEK_SET);

		uint32_t allocate_length = ((reader_list[i].node.parity_length + 0x3F) & 0xFFFFFFC0);

		byte* undecrypt_data = new byte[allocate_length];
		byte* decrypt_data = new byte[allocate_length];

		fread(&node_info, sizeof(node_info), 1, f);
		fread(undecrypt_data, reader_list[i].node.parity_length, 1, f);

		uint32_t parity_value = parity_check_func(header->top.parity_name, reader_list[i].node.parity_length);

		if (parity_value != node_info.parity_name)
		{
			continue;
		}

		uint16_t key[4];

		uint32_t decrypt_max_count = allocate_length / sizeof(uint64_t);

		patry_get_decrypt_xor_func(key, node_info.parity_name);
		memcpy(decrypt_data, undecrypt_data, allocate_length);
		decrypt(decrypt_max_count, decrypt_data, key);
		//decrypt_buffer(decrypt_max_count,xor_value,decrypt_data,undecrypt_data);

		char fileName[32];

		char* file_ext = 0;

		if (strcmp(res_name, "system") == 0)
			file_ext = hash_get_system_name(header->top.parity_name, reader_list[i].node.parity_name);
		if (strcmp(res_name, "script") == 0)
			file_ext = hash_get_script_name(header->top.parity_name, reader_list[i].node.parity_name);

		if (!file_ext)
		{
			file_ext = get_buffer_file_type(decrypt_data);
			if (file_ext)
			{
				sprintf(fileName, "%X%s", reader_list[i].node.parity_name, file_ext);

				sprintf(write_name, "%s%s", export_path, fileName);

				FILE* export_f = fopen(write_name, "wb+");
				fwrite(decrypt_data, node_info.parity_length, 1, export_f);
				fclose(export_f);
			}
		}
		else
		{
			sprintf(fileName, "%s", file_ext);

			sprintf(write_name, "%s%s", export_path, fileName);

			FILE* export_f = fopen(write_name, "wb+");
			fwrite(decrypt_data, node_info.parity_length, 1, export_f);
			fclose(export_f);
		}


		delete[] undecrypt_data;
		delete[] decrypt_data;
	}
}
//temp test
#define DECRYPT_NAME "script"

int main(int argc, char* argv[])
{
	FILE* f;

	magic_header_t header;

	char export_file_path[260];

	f = fopen("script.dat", "rb");

	strcpy(res_name, DECRYPT_NAME);

	fread((void*)&header, sizeof(header), 1, f);

	if (memcmp(header.magic, "NEKOPACK", 8) != 0)
	{
		printf("invalid dat file!\n");
		exit(0);
	}

	uint32_t parity_value = parity_check_func(header.top.parity_name, header.table.parity_length);

	if (parity_value != header.table.parity_name)
	{
		printf("parity check error!\n");
		exit(0);
	}

	uint16_t key[4];

	uint32_t allocate_length = ((header.table.parity_length + 0x3F) & 0xFFFFFFC0);
	byte* undecrypt_data = new byte[allocate_length];
	byte* decrypt_data = new byte[allocate_length];

	patry_get_decrypt_xor_func(key, header.table.parity_name);

	if (header.table.parity_length % sizeof(uint64_t) != 0)
	{
		printf("parity section error\n");
		exit(0);
	}

	memset(undecrypt_data, 0, header.table.parity_length);
	memset(decrypt_data, 0, header.table.parity_length);

	uint32_t decrypt_max_count = header.table.parity_length / sizeof(uint64_t);

	fread(undecrypt_data, header.table.parity_length, 1, f);
	uint32_t reader_offset = 0;

	reader_offset += sizeof(magic_header_t);
	reader_offset += header.top.parity_length;
	reader_offset += header.table.parity_length;

	memcpy(decrypt_data, undecrypt_data, allocate_length);
	decrypt(decrypt_max_count, decrypt_data, key);
	encrypt(decrypt_max_count, decrypt_data, key);
	if (memcmp(decrypt_data, undecrypt_data, allocate_length)) {
		printf("fail\n");
		return 0;

	}
	decrypt(decrypt_max_count, decrypt_data, key);
	//decrypt(decrypt_max_count, decrypt_data, key);


	//decrypt_buffer(decrypt_max_count, xor_value, decrypt_data, undecrypt_data);
	//decrypt_buffer(decrypt_max_count, xor_value, decrypt_data, undecrypt_data);
	//decrypt_buffer(decrypt_max_count, xor_value, decrypt_data, undecrypt_data);


	for (uint32_t i = 1; i < decrypt_max_count; i++)
	{
		parity_node_t* node = (parity_node_t*)&((uint64_t*)decrypt_data)[i];

		stream_reader_t reader;
		reader.node = *node;

		reader.offset_of_file = reader_offset;

		reader_list.push_back(reader);

		reader_offset += node->parity_length + sizeof(parity_node_t);
	}

	_mkdir(DECRYPT_NAME);
	getcwd(export_file_path, sizeof(export_file_path));
	strcat(export_file_path, "\\");
	strcat(export_file_path, DECRYPT_NAME);
	strcat(export_file_path, "\\");

	export_file_reader(f, &header, export_file_path);

	delete[] undecrypt_data;
	delete[] decrypt_data;


	return 0;
}