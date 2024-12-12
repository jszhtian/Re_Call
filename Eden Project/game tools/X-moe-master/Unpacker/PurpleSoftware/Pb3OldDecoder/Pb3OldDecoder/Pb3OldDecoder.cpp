#include "stdafx.h"
#include <Windows.h>
#include <string>

using std::wstring;

#pragma pack(1)
typedef struct {
	UCHAR magic[4];			// "BP3B"
	ULONG payload_length;		// 负载数据的长度
	ULONG pictures;			// 负载的图片数
} pb3b_header_t;
#pragma pack()


int bp3_process(BYTE *bp3, DWORD pb3_length, BYTE **ret_data, DWORD *ret_data_len)
{
	int ret;

	if (!memcmp(bp3, "PB3B", 4)) 
	{
		pb3b_header_t *pb3b = (pb3b_header_t *)bp3;
		WORD xor = *(WORD *)(bp3 + pb3_length - 3);

		for (DWORD i = 8; i < 52; i += 2)
			*(WORD *)&bp3[i] ^= xor;

		BYTE *p = bp3 + pb3_length - 47;
		for (int i = 0; i < 44;) {
			bp3[8 + i++] -= *p++;
			bp3[8 + i++] -= *p++;
			bp3[8 + i++] -= *p++;
			bp3[8 + i++] -= *p++;
		}


		ret = 1;
	}
	else if (!memcmp(bp3, "BM", 2))		// TODO: test	
		ret = 2;
	else if (!memcmp(bp3, "\x89PNG", 4))	// TODO: test
		ret = 3;
	else if (!memcmp(bp3, "MSK0", 4)) 
	{	// TODO: test
		msk0_header_t *msk0 = (msk0_header_t *)bp3;
		BYTE *dib = bp3 + msk0->data_offset;
		if (MyBuildBMPFile(dib, msk0->width * msk0->height, NULL, 0,
			msk0->width, msk0->height, 8, ret_data, ret_data_len,
			my_malloc))
			return -CUI_EMEM;
		ret = 4;
	}
	else
		ret = 0;

	return ret;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 2)
	{
		return 0;
	}

	FILE* InFile = _wfopen(argv[1], L"rb");
	if (InFile == nullptr)
	{
		return 0;
	}
	wstring OutName(argv[1]);
	OutName += L".bmp";
	fseek(InFile, 0, SEEK_END);
	ULONG FileLength = ftell(InFile);
	rewind(InFile);
	BYTE* Pb3Buff = new BYTE[FileLength];
	fread(Pb3Buff, 1, FileLength, InFile);
	Pb3ResourceRelease(Pb3Buff, OutName.c_str());
	delete[] Pb3Buff;
	return 0;
}

