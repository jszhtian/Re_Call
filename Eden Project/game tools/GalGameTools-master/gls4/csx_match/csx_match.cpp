#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(1)



struct csx_header_s
{
	char magic[5];
	unsigned char fill[3]; //# 1A # 00 # 00 
	int unk;	//-1
	int zero;	// 0

	char next_magic[40];
	unsigned __int64 context_size;
};

struct csx_section_s
{
	char name[8];
	unsigned long section_size;
	unsigned int unknow;
};
#pragma pack()

struct section_memory_s
{
	char name[32];
	long section_size;
	unsigned char* section_buffer;
};
unsigned int section_count;
struct section_memory_s** section_array;



void trim( char *str )
{
	char *copied, *tail = NULL;

	if ( str == NULL )
		return;

	for( copied = str; *str; str++ )
	{
		if ( *str != ' ' && *str != '\t' )
		{
			*copied++ = *str;
			tail = copied;
		}
		else
		{
			if ( tail )
				*copied++ = *str;
		}
	}

	if ( tail )
		*tail = 0;
	else
		*copied = 0;

	return;
}

void dump_section(struct section_memory_s* section)
{
	FILE* fp;
	char section_name[32];
	strcpy(section_name,section->name);

	trim(section_name);

	fp = fopen(section_name,"wb");

	fwrite(section->section_buffer,section->section_size,1,fp);

	fclose(fp);
}
void put_section(struct section_memory_s* section)
{
	section_array = (struct section_memory_s**)realloc(section_array,sizeof(struct section_memory_s**) * (section_count + 1));
	section_array[section_count] = section;
	section_count++;
}

void read_section(struct csx_header_s * header,unsigned char* f_buf)
{
	struct section_memory_s* section;

	unsigned __int64 current_size = 0;

	struct csx_section_s * f_section = (struct csx_section_s *)&f_buf[sizeof(struct csx_header_s)];

	do 
	{
		section = (struct section_memory_s*)malloc(sizeof(struct section_memory_s));
		memset(section,0,sizeof(struct section_memory_s));
		strncpy(section->name,f_section->name,sizeof(f_section->name));
		section->section_size = f_section->section_size;

		section->section_buffer = (unsigned char*)malloc(section->section_size);

		current_size += f_section->section_size + sizeof(struct csx_section_s);

		f_section++;

		memcpy(section->section_buffer,f_section,(size_t)section->section_size);

		f_section = (struct csx_section_s*)(((unsigned char*)f_section) + section->section_size);
		
		printf("now section:%s  section_size:%08x\n",section->name,section->section_size);

		put_section(section);
		dump_section(section);

	} while (current_size != header->context_size);
}
int main(int argc,char** argv)
{
	long f_size;
	unsigned char* f_buf;

	struct csx_header_s * csx_header;

	FILE* fp = fopen("script.csx","rb");
	if(!fp)
	{
		printf("can't not open file\n");
		system("pause");
		exit(0);
	}
	fseek(fp,0,SEEK_END);
	f_size = ftell(fp);
	fseek(fp,0,SEEK_SET);

	f_buf = new unsigned char[f_size];

	fread(f_buf,f_size,1,fp);

	fclose(fp);
	section_array = (struct section_memory_s**)malloc(0);


	if(strncmp((const char*)f_buf,"Entis",5)!=0)
	{
		printf("not Entis csx file\n");
		system("pause");
		exit(0);
	}


	csx_header = (struct csx_header_s *)f_buf;

	if(strcmp(csx_header->next_magic,"Cotopha Image file")!=0)
	{
		printf("not find Cotopha Image file\n");
		system("pause");
		exit(0);
	}


	read_section(csx_header,f_buf);

	getchar();

	



	return 0;
}