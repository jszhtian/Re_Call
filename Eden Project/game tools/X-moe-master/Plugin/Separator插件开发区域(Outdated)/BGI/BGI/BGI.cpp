// BGI.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "BGI.h"
#include <cstdio>
#include <string>

using namespace std;

struct bits
{
	unsigned long curbits;
	unsigned long curbyte;
	unsigned char cache;
	unsigned char *stream;
	unsigned long stream_length;
};


static wstring path;
static string  pathA;


int CovtUniToASCII(char* ASCIIBuf , const wchar_t* UniStr, int nSize) 
{
    WideCharToMultiByte(936, 0, UniStr, -1, ASCIIBuf, nSize, NULL, 0); 
    return 0; 
}

BOOL CreatePath(const wchar_t *str = NULL)
{
	if(!str)
		return FALSE;

	SECURITY_ATTRIBUTES attribute;
    attribute.nLength = sizeof(attribute);
    attribute.lpSecurityDescriptor = NULL;
    attribute.bInheritHandle = FALSE;
    //创建
    if(CreateDirectoryW(str, &attribute) == 0)
	{
		return FALSE;
	}
	return TRUE;
}


wstring GetPackageName(wstring& fileName)
{
	wstring temp(fileName);
	wstring::size_type pos = temp.find_last_of(L"\\");

	if (pos != wstring::npos)
	{
    	temp = temp.substr(pos+1,temp.length());
  	}

	wstring temp2(temp);
	wstring::size_type pos2 = temp2.find_last_of(L"\\");
	if (pos2 != wstring::npos)
	{
    	temp2 = temp2.substr(pos+1,temp2.length());
  	}
  	return temp2;
}

void bits_init(struct bits *bits, unsigned char *stream, unsigned long stream_length)
{
	memset(bits, 0, sizeof(*bits));
	bits->stream = stream;
	bits->stream_length = stream_length;
}

#if 1

#if 1
int bits_get_high(struct bits *bits, unsigned int req_bits, unsigned int *retval)
{
	unsigned int bits_value = 0;

	while (req_bits > 0) {
		unsigned int req;

		if (!bits->curbits) {
			if (bits->curbyte >= bits->stream_length)
				return -1;
			bits->cache = bits->stream[bits->curbyte++];
			bits->curbits = 8;
		}

		if (bits->curbits < req_bits)
			req = bits->curbits;
		else
			req = req_bits;

		bits_value <<= req;
		bits_value |= bits->cache >> (bits->curbits - req);
		bits->cache &= (1 << (bits->curbits - req)) - 1;
		req_bits -= req;
		bits->curbits -= req;
	}
	*retval = bits_value;
	return 0;
}
#else
static BYTE table0[8] = {
	0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
};

static BYTE table1[8] = {
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

int bits_get_high(struct bits *bits, unsigned int req_bits, unsigned int *retval)
{
	unsigned int bits_value = 0;

	for (int i = req_bits - 1; i >= 0; i--) {
		if (!bits->curbits) {
			if (bits->curbyte >= bits->stream_length)
				return -1;

			bits->cache = bits->stream[bits->curbyte++];
		}

		if (bits->cache & table0[bits->curbits++])
			bits_value |= table1[i & 7];

		bits->curbits &= 7;
		if (!(i & 7) && i)
			bits_value <<= 8;
	}
	*retval = bits_value;

	return 0;
}
#endif

int bit_get_high(struct bits *bits, void *retval)
{
	return bits_get_high(bits, 1, (unsigned int *)retval);
}	

#else

/* 把距离最高位最近的地方的值返回给retval */
int bit_get_high(struct bits *bits, void *retval)
{
	if (!bits->curbits) {
		if (bits->curbyte >= bits->stream_length)
			return -1;

		bits->cache = bits->stream[bits->curbyte++];
		bits->curbits = 8;
	}
	bits->curbits--;
	*(unsigned char *)retval = bits->cache >> bits->curbits;
	bits->cache &= (1 << bits->curbits) - 1;
	return 0;
}

/* FIXME：实现错误（应该是把距离最高位最近的地方的值依次返回给retval的从高到低字节） */
#if 1
int bits_get_high(struct bits *bits, unsigned int req_bits, unsigned int *retval)
{
	for (unsigned int i = 0; i < req_bits; i++) {
		unsigned char bitval;
		
		if (bit_get_high(bits, &bitval))
			return -1;
		
		*retval <<= 1;
		*retval |= bitval & 1;
	}
	*retval &= (1 << req_bits) - 1;
	
	return 0;
}
#else
int bits_get_high(struct bits *bits, unsigned int req_bits, unsigned int *retval)
{
	unsigned char byteval = 0;
	
	for (unsigned int i = 0; i < req_bits; i++) {
		unsigned char bitval;
		
		if (bit_get_high(bits, &bitval))
			return -1;
		
		byteval <<= 1;
		byteval |= bitval & 1;

		if (!((i + 1) & 7)) {
			((unsigned char *)retval)[i / 8] = byteval;
			byteval = 0;			
		}		
	}
	if (i & 7)
		((unsigned char *)retval)[(i - 1) / 8] = byteval;
	
	return 0;
}
#endif

#endif

/* 把setval的最低位设置到离最高位最近的地方开始 */
int bit_put_high(struct bits *bits, unsigned char setval)
{
	bits->curbits++;
	bits->cache |= (setval & 1) << (8 - bits->curbits);
	if (bits->curbits == 8) {		
		if (bits->curbyte >= bits->stream_length)
			return -1;
		bits->stream[bits->curbyte++] = bits->cache;
		bits->curbits = 0;
		bits->cache = 0;
	}
	return 0;
}

/* 按照从高字节到低字节的顺序把setval中的值设置到离最高位最近的地方开始 */
int bits_put_high(struct bits *bits, unsigned int req_bits, void *setval)
{
	unsigned int this_bits;
	unsigned int this_byte;
	unsigned int i;
	
	this_byte = req_bits / 8;	
	this_bits = req_bits & 7;
	for (int k = (int)this_bits - 1; k >= 0; k--) {
		unsigned char bitval;
		
		bitval = !!(((unsigned char *)setval)[this_byte] & (1 << k));		
		if (bit_put_high(bits, bitval))
			return -1;		
	}	
	this_bits = req_bits & ~7;
	this_byte--;
	for (i = 0; i < this_bits; i++) {
		unsigned char bitval;
		
		bitval = !!(((unsigned char *)setval)[this_byte - i / 8] & (1 << (7 - (i & 7))));		
		if (bit_put_high(bits, bitval))
			return -1;		
	}

	return 0;	
}

void bits_flush(struct bits *bits)
{
	bits->stream[bits->curbyte] = bits->cache;	
}

//DC
static u8 update_key(u32 *key, u8 *magic)
{
	u32 v0, v1;

	v0 = (*key & 0xffff) * 20021;
	v1 = (magic[1] << 24) | (magic[0] << 16) | (*key >> 16);
	v1 = v1 * 20021 + *key * 346;
	v1 = (v1 + (v0 >> 16)) & 0xffff;
	*key = (v1 << 16) + (v0 & 0xffff) + 1;
	return v1 & 0x7fff;
}

typedef struct {
	unsigned int valid;				/* 是否有效的标记 */
	unsigned int weight;			/* 权值 */
	unsigned int is_parent;			/* 是否是父节点 */
	unsigned int parent_index;		/* 父节点索引 */
	unsigned int left_child_index;	/* 左子节点索引 */
	unsigned int right_child_index;	/* 右子节点索引 */		
} bg_huffman_node;

static unsigned int bg_huffman_decompress(bg_huffman_node *huffman_nodes, 
										  unsigned int root_node_index,
										  unsigned char *uncompr, unsigned int uncomprlen, 
										  unsigned char *compr, unsigned int comprlen)
{
	struct bits bits;
		
	bits_init(&bits, compr, comprlen);
	unsigned int act_uncomprlen;
	for (act_uncomprlen = 0; act_uncomprlen < uncomprlen; act_uncomprlen++) {
		unsigned char child;
		unsigned int node_index;
		
		node_index = root_node_index;
		do {
			if (bit_get_high(&bits, &child))
				goto out;

			if (!child)
				node_index = huffman_nodes[node_index].left_child_index;
			else
				node_index = huffman_nodes[node_index].right_child_index;
		} while (huffman_nodes[node_index].is_parent);

		uncompr[act_uncomprlen] = node_index;
	}
out:
	return act_uncomprlen;
}

static unsigned int	bg_create_huffman_tree(bg_huffman_node *nodes, u32 *leaf_nodes_weight)
{
	unsigned int parent_node_index = 256;	/* 父节点从nodes[]的256处开始 */
	bg_huffman_node *parent_node = &nodes[parent_node_index];
	unsigned int root_node_weight = 0;	/* 根节点权值 */
	unsigned int i;

	/* 初始化叶节点 */
	for (i = 0; i < 256; i++) {
		nodes[i].valid = !!leaf_nodes_weight[i];
		nodes[i].weight = leaf_nodes_weight[i];
		nodes[i].is_parent = 0;
		root_node_weight += nodes[i].weight;
	}

	while (1) {		
		unsigned int child_node_index[2];
		
		/* 创建左右子节点 */
		for (i = 0; i < 2; i++) {
			unsigned int min_weight;
			
			min_weight = -1;
			child_node_index[i] = -1;
			/* 遍历nodes[], 找到weight最小的2个节点作为子节点 */
			for (unsigned int n = 0; n < parent_node_index; n++) {
				if (nodes[n].valid) {
					if (nodes[n].weight < min_weight) {
						min_weight = nodes[n].weight;
						child_node_index[i] = n;
					}
				}
			}
			/* 被找到的子节点标记为无效，以便不参与接下来的查找 */			
			nodes[child_node_index[i]].valid = 0;
			nodes[child_node_index[i]].parent_index = parent_node_index;
		}
		/* 创建父节点 */		
		parent_node->valid = 1;
		parent_node->is_parent = 1;
		parent_node->left_child_index = child_node_index[0];
		parent_node->right_child_index = child_node_index[1];
		parent_node->weight = nodes[parent_node->left_child_index].weight
			+ nodes[parent_node->right_child_index].weight;	
		if (parent_node->weight == root_node_weight)
			break;
		parent_node = &nodes[++parent_node_index];
	}

	return parent_node_index;
}

static unsigned int zero_decompress(unsigned char *uncompr, unsigned int uncomprlen,
		unsigned char *compr, unsigned int comprlen)
{
	unsigned int act_uncomprlen = 0;
	int dec_zero = 0;
	unsigned int curbyte = 0;

	while (1) {
		unsigned int bits = 0;
		unsigned int copy_bytes = 0;
		unsigned char code;
		
		do {
			if (curbyte >= comprlen)
				goto out;

			code = compr[curbyte++];
			copy_bytes |= (code & 0x7f) << bits;
			bits += 7;		
		} while (code & 0x80);
		
		if (act_uncomprlen + copy_bytes > uncomprlen)
			break;
		if (!dec_zero && (curbyte + copy_bytes > comprlen))	
			break;
		
		if (!dec_zero) {
			memcpy(&uncompr[act_uncomprlen], &compr[curbyte], copy_bytes);
			curbyte += copy_bytes;
			dec_zero = 1;
		} else {
			memset(&uncompr[act_uncomprlen], 0, copy_bytes);
			dec_zero = 0;
		}
		act_uncomprlen += copy_bytes;
	}
out:	
	return act_uncomprlen;
}

static void bg_average_defilting(unsigned char *dib_buf, unsigned int width, 
								 unsigned int height, unsigned int bpp)
{
	unsigned int line_len = width * bpp;
	
	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			for (unsigned int p = 0; p < bpp; p++) {
				unsigned int a, b;
				unsigned int avg;

				b = y ? dib_buf[(y - 1) * line_len + x * bpp + p] : -1;
				a = x ? dib_buf[y * line_len + (x - 1) * bpp + p] : -1;
				avg = 0;
				
				if (a != -1)
					avg += a;
				if (b != -1)
					avg += b;
				if (a != -1 && b != -1)
					avg /= 2;
					
				dib_buf[y * line_len + x * bpp + p] += avg;
			}
		}
	}
}

static void	decode_bg(unsigned char *enc_buf, unsigned int enc_buf_len, 
					  unsigned long key, unsigned char *ret_sum, unsigned char *ret_xor)
{	
	unsigned char sum = 0;
	unsigned char _xor = 0;	
	u8 magic[2] = { 0, 0 };
	
	for (unsigned int i = 0; i < enc_buf_len; i++) {
		enc_buf[i] -= update_key(&key, magic);
		sum += enc_buf[i];
		_xor ^= enc_buf[i];
	}
	*ret_sum = sum;
	*ret_xor = _xor;
}

static unsigned int bg_decompress(bg_header_t *bg_header, unsigned int bg_len, 
								  unsigned char *image_buf, unsigned int image_size)
{
	unsigned int act_uncomprlen = 0;
	unsigned int i;		
	unsigned char *enc_buf = (unsigned char *)(bg_header + 1);
	
	/* 解密叶节点权值 */
	unsigned char sum;
	unsigned char _xor;
	decode_bg(enc_buf, bg_header->encode_length, bg_header->key, &sum, &_xor);	
	if (sum != bg_header->sum_check || _xor != bg_header->xor_check)
		return 0;

	/* 初始化叶节点权值 */
	u32 leaf_nodes_weight[256];
	unsigned int curbyte = 0;
	for (i = 0; i < 256; i++) {
		unsigned int bits = 0;		
		u32 weight = 0;
		unsigned char code;
		
		do {
			if (curbyte >= bg_header->encode_length)
				return 0;
			code = enc_buf[curbyte++];
			weight |= (code & 0x7f) << bits;
			bits += 7;		
		} while (code & 0x80);	
		leaf_nodes_weight[i] = weight;
	}

	bg_huffman_node nodes[511];
	unsigned int root_node_index = bg_create_huffman_tree(nodes, leaf_nodes_weight);
	unsigned char *zero_compr = (unsigned char *)malloc(bg_header->zero_comprlen);
	if (!zero_compr)
		return 0;

	unsigned char *compr = enc_buf + bg_header->encode_length;
	unsigned int comprlen = bg_len - sizeof(bg_header_t) - bg_header->encode_length;
	act_uncomprlen = bg_huffman_decompress(nodes, root_node_index, 
		zero_compr, bg_header->zero_comprlen, compr, comprlen);
	if (act_uncomprlen != bg_header->zero_comprlen) {
		free(zero_compr);
		return 0;
	}

	act_uncomprlen = zero_decompress(image_buf, image_size, zero_compr, bg_header->zero_comprlen);
	free(zero_compr);
	
	bg_average_defilting(image_buf, bg_header->width, bg_header->height, bg_header->color_depth / 8);

	return act_uncomprlen;
}


int write_bmp(const char* filename,
               unsigned char*   buff,
               unsigned long    len,
               unsigned long    width,
               unsigned long    height,
               unsigned short   depth_bytes)
{
  BITMAPFILEHEADER bmf;
  BITMAPINFOHEADER bmi;

  memset(&bmf, 0, sizeof(bmf));
  memset(&bmi, 0, sizeof(bmi));

  bmf.bfType     = 0x4D42;
  bmf.bfSize     = sizeof(bmf) + sizeof(bmi) + len;
  bmf.bfOffBits  = sizeof(bmf) + sizeof(bmi);

  bmi.biSize     = sizeof(bmi);
  bmi.biWidth    = width;
  bmi.biHeight   = height;
  bmi.biPlanes   = 1;
  bmi.biBitCount = depth_bytes * 8;
           

  std::string szFileName = pathA;
  szFileName += filename;
  FILE* fd = fopen(szFileName.c_str(), "wb");
  fwrite(&bmf, 1, sizeof(bmf), fd);
  fwrite(&bmi, 1, sizeof(bmi), fd);
  fwrite(buff, 1,         len, fd);
  fclose(fd);
  return 0;
}

int ExtractImage(const char *file)
{
	if(!file)
		return -1;

	FILE *fin = fopen(file, "rb");
	if(fin == NULL)
	{
		fclose(fin);
		return -1;
	}
	fseek(fin,0,SEEK_END);
	DWORD FileSize=ftell(fin);
	rewind(fin);
	char *pFile = new char[FileSize];
	fread(pFile,FileSize,1,fin);
	fclose(fin);
	
	BGHeader *bg_header;
	bg_header = (BGHeader*)pFile;

		if(!memcmp(bg_header->magic, "CompressedBG___", 16) && bg_header->reserved1 == 1)
		{
			unsigned int image_size;
			unsigned char *image_buffer;
			image_size = bg_header->width * bg_header->height * bg_header->color_depth / 8;
			image_buffer = new unsigned char[image_size];
	
			unsigned long act_uncomprlen = bg_decompress(bg_header, FileSize, 
			image_buffer, image_size);		
			if (act_uncomprlen != image_size)
			{
				delete[] image_buffer;
				return -1;			
			}
			char szFileName[MAX_PATH];
			sprintf(szFileName, "%s.bmp", file);
			//we should flip it
			write_bmp(szFileName, image_buffer, image_size, bg_header->width, - bg_header->height, bg_header->color_depth / 8);	
		    delete[] image_buffer;
		}
	
	delete[] pFile;
	return 0;
}


/*********************************************/

_declspec (dllexport)
void ShowInfo()
{
	printf("BGI ARC Tool\n");
	printf("X'moe Project\n");
}

_declspec (dllexport)
BOOL Engine(TCHAR *pszStr)
{
	
	BOOL ret0 =  0 == _tcsicmp(pszStr, L"BGI")? TRUE:FALSE;
	BOOL ret1 =  0 == _tcsicmp(pszStr, L"BGI Engine")? TRUE:FALSE;
	BOOL ret2 =  0 == _tcsicmp(pszStr, L"BGIEngine")? TRUE:FALSE;
	return ret0 || ret1 || ret2;
}

_declspec (dllexport)
BOOL Match(TCHAR *pszStr, TCHAR* szFile)
{
	BOOL ret =  0 == _tcscmp(pszStr, L"arc")? TRUE:FALSE;
	BOOL ret2 = FALSE;

	FILE *fin = NULL;
	do
	{
		fin = _wfopen(szFile, L"rb");
		if(fin == NULL)
			break;
		
		char sample[16];
		try
		{
			fread(sample, 1, 16, fin);
		}
		catch(...)
		{
			break;
		}


		if(!memcmp(sample, NewPckSign, 12))
		{
			ret2 = TRUE;
			printf("BGI testing\n");
			break;
		}
		else
		{
			if(!memcmp(sample, "PackFile    ",12))
			{
				ret2 = TRUE;
				break;
			}
			else
			{
				if(!memcmp(sample, "CompressedBG___", 16))
				{
					ret2 = TRUE;
					break;
				}
				else
				{
					if(!memcmp(sample, "SDC FORMAT 1.00", 16))
					{
						ret2 = TRUE;
						break;
					}
					else
					{
						if(!memcmp(sample, "DSC FORMAT 1.00", 16))
						{
							ret2 = TRUE;
							break;
						}
						else
						{
							break;
						}
					}
				}
			}
		}
	}while(false);

	fclose(fin);

	
	if(ret == TRUE || ret2 == TRUE)
	{
		printf("Guessing: BGI Package\n");
	}
	
	return ret || ret2;
}



/********************************************/
void ExtractModeOld(FILE* fin, unsigned long count)
{
	ArchieveEntry *EntryBuffer = NULL;
	try
	{
		EntryBuffer = new ArchieveEntry[count];
	}
	catch(...)
	{
		fclose(fin);
		delete[] EntryBuffer;
		char szErrorCode[MAX_PATH];
		printf(szErrorCode, "An Error Occurred![0x%08x]\n", GetLastError());
		return;
	}
	fread(EntryBuffer, 1, count * sizeof(ArchieveEntry), fin);
	
	for(unsigned int index = 0; index < count; index++)
	{
		fseek(fin, EntryBuffer[index].offset, SEEK_SET);
		char *pBuffer = new char[EntryBuffer[index].length];

		std::string szFileName = pathA;
		szFileName += EntryBuffer[index].name;

		FILE *toFile = fopen(szFileName.c_str(), "wb");
		fread(pBuffer, 1, EntryBuffer[index].length, fin);
		fwrite(pBuffer, EntryBuffer[index].length, 1, toFile);
		ExtractImage(szFileName.c_str());
		fclose(toFile);
		delete[] pBuffer;
	}
}

//BURIKO ARC20
void ExtractModeNew(FILE *fin, unsigned long count)
{
	ArchieveEntryNew *EntryBuffer = NULL;
	try
	{
		EntryBuffer = new ArchieveEntryNew[count];
	}
	catch(...)
	{
		fclose(fin);
		delete[] EntryBuffer;
		char szErrorCode[MAX_PATH];
		printf(szErrorCode, "An Error Occurred![0x%08x]\n", GetLastError());
		return ;
	}
	fread(EntryBuffer, 1, count * sizeof(ArchieveEntryNew), fin);
	
	for(unsigned int index = 0; index < count; index++)
	{
		fseek(fin, EntryBuffer[index].offset + 0x10 + count * sizeof(ArchieveEntryNew), SEEK_SET);
		char *pBuffer = new char[EntryBuffer[index].length];

		std::string szFileName = pathA;
		szFileName += EntryBuffer[index].name;

		FILE *toFile = fopen(szFileName.c_str(), "wb");
		fread(pBuffer, 1, EntryBuffer[index].length, fin);
		fwrite(pBuffer, EntryBuffer[index].length, 1, toFile);
		ExtractImage(szFileName.c_str());
		fclose(toFile);
		delete[] pBuffer;
	}
}

extern "C" _declspec (dllexport)
int Exec(const TCHAR *Package, DWORD dwPackParam, DWORD dwCmdLine, const TCHAR *output, const TCHAR *args)
{

	FILE *fin = NULL;

	wstring _pack((wchar_t*)Package);
	wstring packName = GetPackageName(_pack);

	if(!PathFileExistsW(packName.c_str()))
	{
		if(CreatePath(packName.c_str()))
		{
			path = packName + L"//";
			char aBuf[MAX_PATH];
			memset(aBuf, 0, sizeof(aBuf));
			CovtUniToASCII(aBuf , path.c_str(), MAX_PATH);
			pathA = aBuf;
		}
		else
		{
			wprintf(L"Failed to Create Path %s\n", packName.c_str());
		}
	}

	fin =_wfopen(Package, L"rb");
	ArchieveHeader pHeader;
	fread(&pHeader, 1, sizeof(ArchieveHeader), fin );
	
	if(!memcmp(pHeader.magic, "PackFile    ", 12))
	{
		ExtractModeOld(fin , pHeader.entries);
	}
	else if(!memcmp(pHeader.magic, NewPckSign, 12))
	{
		ExtractModeNew(fin, pHeader.entries);
	}
	
	fclose(fin);
	return 0;
}
