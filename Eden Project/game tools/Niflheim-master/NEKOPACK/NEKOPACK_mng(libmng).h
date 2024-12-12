#define _CRT_SECURE_NO_WARNINGS
#define _USE_32BIT_TIME_T
//˵�Ƕ�д�߼�֧�ֱ����
#define MNG_SUPPORT_READ
#define MNG_SUPPORT_WRITE
//˵�Ƕ�д�ļ�����ģ����׵ĺ�
#define MNG_STORE_CHUNKS
#define MNG_ACCESS_CHUNKS
//cms���������˰��컹û�ҵ����Ծ�û������ȥ���룬ֻ������libjpeg
#define MNG_NO_CMS
//˵�ǰ�����ͷ�ļ��Ǳ���ģ��ƺ�������Ҳ���Ա���ͨ������
#define MNG_USE_DLL
/*
���Ҫ�ر�˵����libmng�Ȿ������cmake���ɵ��ٱ��룬libmng 2.0.3�е�libmng_types.h����ôһ�仰
TODO: this may require some elaboration for other platforms;
only works with BCB for now
�ǵģ�win32�����bcb��C++ Builder��д�ģ���vs�±���û���⣬��ʹ���д������
����__stdcall��__cdecl�����⣬libmngʹ�õ���__stdcall������ȫ���ĳ�__cdecl���û������
���˵�ħ�ķ�ʽ��
��libmng_types.h�ĵ�258��#define MNG_DECL __stdcall�ĳ���
#if defined(_WINDOWS)
#define MNG_DECL __cdecl
#else
#define MNG_DECL __stdcall
#endif
Ȼ���ٱ��롣
_WINDOWS����cmake���ɵģ�vs��û���ⶫ�������������ٶ���һ�£�û��ʲô���������
��ȻҪ������������ʽ�Ͳ���Ҫ����_WINDOWS�ˡ�
*/
#define _WINDOWS
//mng�ı�׼�ĵ���http://www.libpng.org/pub/mng/spec/
//mng�Ŀ���ָ�ϣ�https://libmng.com/developers.html
#include <stdio.h>
#include <direct.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <libmng.h>

using namespace std;

typedef struct mng_s
{
	FILE* src;
	FILE* info;
	DWORD filenum;
	char filename[MAX_PATH];
	char dirname[MAX_PATH];
} mng_t;
typedef mng_t* mng_p;

//�����ڴ�Ļص�����
mng_ptr NEKOPACK_mng_calloc(mng_size_t size)
{
	return (mng_ptr)calloc(1, size);
}
//�ͷ��ڴ�Ļص�����
void NEKOPACK_mng_free(mng_ptr buff, mng_size_t size)
{
	free(buff);
	return;
}
//openstream�Ļص�����
mng_bool NEKOPACK_mng_openstream(mng_handle mng)
{
	return MNG_TRUE;
}
//closestream�Ļص�����
mng_bool NEKOPACK_mng_closestream(mng_handle mng)
{
	return MNG_TRUE;
}
//��ȡ���ݵĻص�����
mng_bool NEKOPACK_mng_readdata(mng_handle mng, mng_ptr buff, mng_uint32 size, mng_uint32p mread)
{
	mng_p mng_data = (mng_p)mng_get_userdata(mng);
	//���뷵�صĲ���ʡ�ԣ�mread�ڿ��ڲ���Ҫ����
	*mread = fread(buff, 1, size, mng_data->src);
	return MNG_TRUE;
}
//�������ݿ�Ļص�����
mng_bool NEKOPACK_mng_read_iteratechunks(mng_handle mng, mng_handle chunk, mng_chunkid chunktype, mng_uint32 chunkseq)
{
	/*if (chunktype == MNG_UINT_MHDR)//MHDR�鴦��
	{
		//MHDR��ǩ����
		mng_uint32 width;
		mng_uint32 height;
		mng_uint32 ticks;
		mng_uint32 layercount;
		mng_uint32 framecount;
		mng_uint32 playtime;
		mng_uint32 profile;
		mng_p mng_data = (mng_p)mng_get_userdata(mng);
		mng_getchunk_mhdr(mng, chunk, &width, &height, &ticks, &layercount, &framecount, &playtime, &profile);
		printf("filename:%s width:%d height:%d ticks:%d layercount:%d framecount:%d playtime:%d profile:%d\n", mng_data->filename, width, height, ticks, layercount, framecount, playtime, profile);
		char infoname[MAX_PATH];
		sprintf(infoname, "%s.ini", mng_data->filename);
		mng_data->info = fopen(infoname, "wt,ccs=UNICODE");
		if (mng_data->info == NULL)
		{
			printf("����info�ļ�ʧ�ܣ�\n");
			return MNG_FALSE;
		}
		fwprintf(mng_data->info, L";ALL\n");
		fwprintf(mng_data->info, L"width=%d\n", width);
		fwprintf(mng_data->info, L"height=%d\n", height);
		fwprintf(mng_data->info, L"ticks=%d\n", ticks);
		fwprintf(mng_data->info, L"layercount=%d\n", layercount);
		fwprintf(mng_data->info, L"framecount=%d\n", framecount);
		fwprintf(mng_data->info, L"playtime=%d\n", playtime);
		fwprintf(mng_data->info, L"profile=%d\n\n", profile);
	}
	else if (chunktype == MNG_UINT_IHDR)
	{
		//IHDR��ǩ����
		mng_uint32 width;
		mng_uint32 height;
		mng_uint8 bpp;
		mng_uint8 colortype;
		mng_uint8 comp;
		mng_uint8 filter;
		mng_uint8 interlace;
		mng_p mng_data = (mng_p)mng_get_userdata(mng);
		mng_getchunk_ihdr(mng, chunk, &width, &height, &bpp, &colortype, &comp, &filter, &interlace);
		fwprintf(mng_data->info, L"width=%d\n", width);
		fwprintf(mng_data->info, L"height=%d\n", height);
		fwprintf(mng_data->info, L"bpp=%d\n", bpp);
	}
	else if (chunktype == MNG_UINT_tEXt)
	{
		//tEXt��ǩ����
		mng_uint32 keywordsize;
		mng_pchar keyword;
		mng_uint32 ztextsize;
		mng_pchar ztext;
		mng_p mng_data = (mng_p)mng_get_userdata(mng);
		mng_getchunk_text(mng, chunk, &keywordsize, &keyword, &ztextsize, &ztext);
		fwprintf(mng_data->info, L";tEXt\n");
		WCHAR wkeyword[200];
		DWORD wkeywordsize = MultiByteToWideChar(932, 0, keyword, keywordsize, wkeyword, 0);
		MultiByteToWideChar(932, 0, keyword, keywordsize, wkeyword, wkeywordsize);
		wkeyword[wkeywordsize] = L'\0';
		WCHAR wztext[200];
		DWORD wztextsize = MultiByteToWideChar(932, 0, ztext, ztextsize, wztext, 0);
		MultiByteToWideChar(932, 0, ztext, ztextsize, wztext, wztextsize);
		wztext[wztextsize] = L'\0';
		fwprintf(mng_data->info, L"%ls\t%ls\n", wkeyword, wztext);
	}
	else if (chunktype == MNG_UINT_MEND)
	{
		//mng_p mng_data = (mng_p)mng_get_userdata(mng);
		//fclose(mng_data->info);
	}*/
	return MNG_TRUE;
}

bool NEKOPACK_mng_read(char* filename)
{
	mng_retcode mng_rc;
	mng_p mng_data = (mng_p)calloc(1, sizeof(mng_p));
	if (mng_data == NULL)
	{
		printf("����mng_dataʧ�ܣ�\n");
		return false;
	}
	strcpy(mng_data->filename, filename);
	strcpy(mng_data->dirname, filename);
	*(char*)strchr(mng_data->dirname, '.') = '\0';
	if (_access(mng_data->dirname, 0) == -1)
		_mkdir(mng_data->dirname);
	mng_data->filenum = 0;
	if ((mng_data->src = fopen(filename, "rb")) == NULL)
	{
		printf("�ļ���ʧ�ܣ�\n");
		free(mng_data);
		return false;
	}
	mng_handle mng = mng_initialize((mng_ptr)mng_data, NEKOPACK_mng_calloc, NEKOPACK_mng_free, MNG_NULL);
	if (!mng)
	{
		printf("mng_initialize��ʼ��ʧ�ܣ�\n");
		mng_cleanup(&mng);
		fclose(mng_data->src);
		free(mng_data);
		return false;
	}
	if ((((mng_rc = mng_setcb_openstream(mng, NEKOPACK_mng_openstream)) != 0) || ((mng_rc = mng_setcb_closestream(mng, NEKOPACK_mng_closestream)) != 0) || ((mng_rc = mng_setcb_readdata(mng, NEKOPACK_mng_readdata)) != 0)))
	{
		printf("mng_setcb��ʼ��ʧ�ܣ�\n");
		mng_cleanup(&mng);
		fclose(mng_data->src);
		free(mng_data);
		return false;
	}
	if ((mng_rc = mng_read(mng)) != 0)
	{
		printf("mng_read��ʼ��ʧ�ܣ�\n");
		mng_cleanup(&mng);
		fclose(mng_data->src);
		free(mng_data);
		return false;
	}
	if (mng_rc = mng_iterate_chunks(mng, 0, NEKOPACK_mng_read_iteratechunks) != 0)
	{
		printf("mng_iterate_chunks����ʧ�ܣ�\n");
		mng_cleanup(&mng);
		fclose(mng_data->src);
		free(mng_data);
		return false;
	}
	mng_cleanup(&mng);
	fclose(mng_data->src);
	free(mng_data);
	return true;
}