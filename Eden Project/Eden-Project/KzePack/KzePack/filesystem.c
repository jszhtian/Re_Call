#include "filesystem.h"
FILE* fd;
file_header_t f_header;
file_node_t *f_node;
file_writer_node_t* f_wrtnode;

char* HeadSig = (char*)"FileSystemVer1.00";

FILE* fd;
file_header_t f_header;
file_node_t* f_node;
file_writer_node_t* f_wrtnode;

unsigned long long MurmurHash64B(const void* key, int len, unsigned int seed)
{
	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	unsigned int h1 = seed ^ len;
	unsigned int h2 = 0;

	const unsigned int* data = (const unsigned int*)key;

	while (len >= 8)
	{
		unsigned int k1 = *data++;
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		len -= 4;

		unsigned int k2 = *data++;
		k2 *= m; k2 ^= k2 >> r; k2 *= m;
		h2 *= m; h2 ^= k2;
		len -= 4;
	}

	if (len >= 4)
	{
		unsigned int k1 = *data++;
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		len -= 4;
	}

	switch (len)
	{
	case 3: h2 ^= ((unsigned char*)data)[2] << 16;
	case 2: h2 ^= ((unsigned char*)data)[1] << 8;
	case 1: h2 ^= ((unsigned char*)data)[0];
		h2 *= m;
	};

	h1 ^= h2 >> 18; h1 *= m;
	h2 ^= h1 >> 22; h2 *= m;
	h1 ^= h2 >> 17; h1 *= m;
	h2 ^= h1 >> 19; h2 *= m;

	unsigned long long h = h1;

	h = (h << 32) | h2;

	return h;
}

void Func1(BYTE* src, DWORD len)
{
	__asm {
		mov		eax, src;
		mov		ebx, len;
	Tag:
		mov     dx, word ptr[eax];
		movzx   esi, dx;
		and dx, 0x5555;
		and esi, 0xAAAAAAAA;
		shr     esi, 1;
		add     edx, edx;
		or si, dx;
		mov     edx, esi;
		mov     word ptr[eax], dx;
		inc     ecx;
		add     eax, 0x2;
		cmp     ebx, ecx;
		ja      Tag;
	End:
	}
}

BYTE rol(int val, int n)
{
	n %= 8;
	return (val << n) | (val >> (8 - n));
}

BYTE ror(int val, int n)
{
	n %= 8;
	return (val >> n) | (val << (8 - n));
}

void cryption(BYTE* buff, DWORD size, int dec, unsigned long long cala)
{
	if (dec == 0)
	{
		DWORD count = (size - 0x10) / 4;
		BYTE* p = buff + 0x10;

		DWORD key1 = 114514;
		DWORD key2 = 0x3AD023;
		DWORD c = 0x114;

		for (DWORD i = 0; i < count; i++)
		{
			DWORD* dp = (DWORD*)p;
			*dp ^= key2;
			*dp -= key1 ^ key2;
			*dp ^= key1;
			p = (BYTE*)dp;
			*p = ror(*p, c++);
			c &= 0xFF;
			p += 4;
		}

		Func1(buff, size);
		for (int i = 0; i < size; i++)
		{
			buff[i] ^= 0x01010101;
			buff[i] += 0x12;
			buff[i] ^= KEY_TBL[i % 41][i % 1024];
			buff[i] += cala;
			cala += KEY_TBL[i % 41][i % 1024];
		}
	}
	else if (dec == 1)
	{
		DWORD count = (size - 0x10) / 4;
		BYTE* p = buff + 0x10;

		DWORD key1 = 114514;
		DWORD key2 = 0x3AD023;
		DWORD c = 0x114;
		for (int i = 0; i < size; i++)
		{
			buff[i] -= cala;
			cala += KEY_TBL[i % 41][i % 1024];
			buff[i] ^= KEY_TBL[i % 41][i % 1024];
			buff[i] -= 0x12;
			buff[i] ^= 0x01010101;
		}
		Func1(buff, size);
		for (DWORD i = 0; i < count; i++)
		{
			*p = rol(*p, c++);
			//*p = CPP_ROL(*p, c++);
			c &= 0xFF;
			DWORD* dp = (DWORD*)p;
			*dp ^= key1;
			*dp += key1 ^ key2;
			*dp ^= key2;
			p = (BYTE*)dp + 4;

		}
	}
	else if (dec == 2)
	{
		int seed = strlen(HeadSig);
		int key = (((seed << 5) ^ 0xA5) * (seed + 0x6F349)) ^ 0x34A9B129;
		unsigned int* p = (unsigned int*)(buff + 0x10);
		for (int i = 0; i < (seed - 13) / 4; i++)
		{
			p[i] ^= key;
		}
		for (int i = 0; i < size; i++)
		{
			buff[i] ^= 0xFFFFFFFF;
		}

	}
	else if (dec == 3)
	{
		for (int i = 0; i < size; i++)
		{
			buff[i] ^= 0xFFFFFFFF;
		}
		int seed = strlen(HeadSig);
		int key = (((seed << 5) ^ 0xA5) * (seed + 0x6F349)) ^ 0x34A9B129;
		unsigned int* p = (unsigned int*)(buff + 0x10);
		for (int i = 0; i < (seed - 13) / 4; i++)
		{
			p[i] ^= key;
		}
	}
}

int fs_open(const char* path) {
	int node_size;

	fd = fopen(path, "rb");

	if (!fd) {
		return FS_FILE_NOTFOUND;
	}

	if (fread(&f_header, sizeof(f_header), 1, fd) != 1) {
		fclose(fd);
		fd = NULL;
		return FS_HEADER_FAILED;
	}

	if (strcmp(f_header.header, HeadSig) != 0) {
		fclose(fd);
		fd = NULL;
		return FS_HEADER_FAILED;
	}

	node_size = sizeof(file_node_t) * f_header.f_count;
	f_node = (file_node_t*)malloc(node_size);
	if (fread(f_node, 1, node_size, fd) != node_size) {
		fclose(fd);
		fd = NULL;
		return FS_HEADER_FAILED;
	}

	return FS_SUCCESS;
}

int fs_create(const char* path) {
	fd = fopen(path, "wb+");
	if (!fd) {
		return FS_CREATE_FAILED;
	}
	strcpy(f_header.header, HeadSig);
	f_header.f_count = 0;
	f_wrtnode = (file_writer_node_t*)malloc(0);

	return FS_SUCCESS;
}

unsigned long fs_disk_flength(const char* path) {
	FILE* nfd;
	unsigned long f_length;


	nfd = fopen(path, "rb");
	if (!nfd) {
		return 0;
	}

	fseek(nfd, 0, SEEK_END);
	f_length = ftell(nfd);
	fclose(nfd);

	return f_length;
}
void fs_putfile(const char* path, const char* f_name) {
	unsigned long f_length;


	if (f_wrtnode) {
		f_length = fs_disk_flength(path);
		if (f_length > 0) {
			f_wrtnode = (file_writer_node_t*)realloc(f_wrtnode, sizeof(file_writer_node_t) * (f_header.f_count + 1));

			strcpy(f_wrtnode[f_header.f_count].f_diskpath, path);
			strcpy(f_wrtnode[f_header.f_count].f_name, f_name);
			f_wrtnode[f_header.f_count].source_size = f_length;

			f_header.f_count++;
		}
	}
}

void fs_compr_readdisk(const char* path, void** f_comprbuf, unsigned long* f_comprlength) {
	FILE* nfd;
	unsigned char* compr_buf;
	unsigned char* source_buf;
	unsigned long source_length;
	unsigned long compr_length;
	nfd = fopen(path, "rb");
	if (nfd) {
		fseek(nfd, 0, SEEK_END);
		source_length = ftell(nfd);
		fseek(nfd, 0, SEEK_SET);

		source_buf = (unsigned char*)malloc(source_length);
		compr_length = compressBound(source_length);
		compr_buf = (unsigned char*)malloc(compr_length);

		fread(source_buf, source_length, 1, nfd);
		if (compress(compr_buf, &compr_length, source_buf, source_length) == Z_OK) {
			*f_comprbuf = compr_buf;
			*f_comprlength = compr_length;
			free(source_buf);
		}
		else {
			free(compr_buf);
			free(source_buf);
		}

		fclose(nfd);
	}
}
void fs_save() {
	unsigned long f_startpos;
	int i;
	file_node_t* f_builder;
	void* f_comprbuf = NULL;
	unsigned long f_comprlength;

	if (f_wrtnode) {
		f_builder = (file_node_t*)malloc(sizeof(file_node_t) * f_header.f_count);
		memset(f_builder, 0, sizeof(file_node_t) * f_header.f_count);

		f_startpos = sizeof(file_header_t) + (sizeof(file_node_t) * f_header.f_count);
		fseek(fd, 0, SEEK_SET);

		fwrite(&f_header, sizeof(f_header), 1, fd);

		for (i = 0; i < f_header.f_count; i++) {
			fs_compr_readdisk(f_wrtnode[i].f_diskpath, &f_comprbuf, &f_comprlength);

			if (f_comprbuf && f_comprlength) {
				fseek(fd, f_startpos, SEEK_SET);
				printf("%s \n", f_wrtnode[i].f_name);
				//strcpy(f_builder[i].f_name, f_wrtnode[i].f_name);
				unsigned long long HashName = MurmurHash64B(f_wrtnode[i].f_name,strlen(f_wrtnode[i].f_name), 0xEE6B27EB);
				f_builder[i].f_name = HashName;
				f_builder[i].position = f_startpos;
				f_builder[i].source_size = f_wrtnode[i].source_size;
				f_builder[i].compr_size = f_comprlength;
				fwrite(f_comprbuf, f_comprlength, 1, fd);
				f_startpos += f_comprlength;
			}

			if (f_comprbuf) {
				free(f_comprbuf);
				f_comprbuf = NULL;
			}
		}


		fseek(fd, sizeof(file_header_t), SEEK_SET);
		fwrite(f_builder, (sizeof(file_node_t) * f_header.f_count), 1, fd);
		fflush(fd);
	}
}
void fs_close() {
	if (fd) {
		fclose(fd);
		fd = NULL;
	}

	if (f_node) {
		free(f_node);
		f_node = NULL;
	}

	if (f_wrtnode) {
		free(f_wrtnode);
		f_wrtnode = NULL;
	}
}

int fs_havefile(const char* f_name) {
	unsigned int i;
	if (!f_node) return 0;

	for (i = 0; i < f_header.f_count; i++) {
		if (_stricmp(f_node[i].f_name, f_name) == 0) {
			return 1;
		}
	}
	return 0;
}

unsigned int fs_filesize(const char* f_name) {
	unsigned int i;
	if (!f_node) return 0;

	for (i = 0; i < f_header.f_count; i++) {
		if (_stricmp(f_node[i].f_name, f_name) == 0) {
			return f_node[i].source_size;
		}
	}
	return 0;
}

void fs_readbuffer(file_node_t* node, void** data, unsigned long* f_size, unsigned long long key) {
	unsigned char* compr_buf;
	unsigned char* uncompr_buf;
	unsigned long uncompr_length;

	fseek(fd, node->position, SEEK_SET);

	compr_buf = (unsigned char*)malloc(node->compr_size);
	uncompr_buf = (unsigned char*)malloc(node->source_size);
	uncompr_length = node->source_size;
	fread(compr_buf, node->compr_size, 1, fd);
	if (uncompress(uncompr_buf, &uncompr_length, compr_buf, node->compr_size) == Z_OK) {
		*data = uncompr_buf;
		*f_size = uncompr_length;
		free(compr_buf);
	}
	else {
		free(compr_buf);
		free(uncompr_buf);

		*data = NULL;
		*f_size = 0;
	}
}

void fs_readfile(const char* f_name, void** data, unsigned long* f_size) {
	unsigned int i;
	if (!f_node) return;
	unsigned long long HashName = MurmurHash64B(f_name, strlen(f_name), 0xEE6B27EB);
	for (i = 0; i < f_header.f_count; i++) {
		//if (_stricmp(f_node[i].f_name, f_name) == 0) {
		if (f_node[i].f_name == HashName) {
			fs_readbuffer(&f_node[i], data, f_size, MurmurHash64B(f_name, strlen(f_name), 0xEE6B27EB)); return;
		}
	
	}
}