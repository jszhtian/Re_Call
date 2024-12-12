#pragma		once

/*
g—p—á

	MD5init(&md5st, data_total_size);		// šƒf[ƒ^‚Ì‘±‚«‚ª‚ ‚éê‡‚Í‚U‚SƒoƒCƒg‚Ì”{”’PˆÊ‚Å‘—‚è‚Ş–
	ret = MD5funk(&md5st, p, read_size);	// ret : 0=‘±‚­ 1=I—¹ 2=‚d‚q‚q

	BYTE code = (UC *)md5st.code;
	format(md5code_str, (UC *)"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
		*(code+ 0), *(code+ 1), *(code+ 2), *(code+ 3),
		*(code+ 4), *(code+ 5), *(code+ 6), *(code+ 7),
		*(code+ 8), *(code+ 9), *(code+10), *(code+11),
		*(code+12), *(code+13), *(code+14), *(code+15));
  
*/


// ‚l‚c‚T
#define		MD5_ADD_DATA_MAX		(1 + 56 + (64 - 56) + 8)
struct MD5_STRUCT
{
	BYTE add_data[MD5_ADD_DATA_MAX];
	DWORD code[4];
	UINT loop_cnt, nokori;
};

void MD5init(MD5_STRUCT *md5st, UINT total_data_cnt);
int  MD5funk(MD5_STRUCT *md5st, BYTE *data, UINT data_cnt);

