#include "filesystem.h"
int ver = 1;
void add_files()
{
	HANDLE hFind;
	WIN32_FIND_DATAA findData;
	char szDirectory[MAX_PATH];
	char szTmpFileName[MAX_PATH];
	char szFindDir[MAX_PATH];

	GetCurrentDirectoryA(sizeof(szDirectory),szDirectory);
	strcpy(szFindDir,szDirectory);
	strcat(szFindDir,"\\yumeiro_cn\\*.*");
	hFind = FindFirstFileA(szFindDir,&findData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		if(!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			strcpy(szTmpFileName,szDirectory);
			strcat(szTmpFileName,".\\yumeiro_cn\\");
			strcat(szTmpFileName,findData.cFileName);

			fs_putfile(szTmpFileName,findData.cFileName);

			//printf("packet file:%s\n",findData.cFileName);
		}
	}while(FindNextFileA(hFind,&findData));

	//fs_putfile("Kze.kzs","Kze.kzs");
}



int main(int argc,char** argv)
{
	if (ver == 1) 
	{
		void* f_buf;
		unsigned long f_length;



		if (fs_create("yumeiro_cn.kze")) {
			printf("file open failed\n");
			return 1;
		}

		add_files();

		fs_save();

		fs_close();
		system("pause");
		if (fs_open("yumeiro_cn.kze")) {
			printf("file open failed\n");
			//	return 1;
		}
		char* fn = "TG0-06-1.png";
		fs_readfile(fn, &f_buf, &f_length);
		FILE* fs = fopen(fn, "wb");
		fwrite(f_buf, f_length, 1, fs);
		fclose(fs);
		//fs_readfile("TEXT.DAT",&f_buf,&f_length);
		system("pause");
	}
	return 0;
}