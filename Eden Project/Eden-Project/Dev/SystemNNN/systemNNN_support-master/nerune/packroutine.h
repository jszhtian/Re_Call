//
// packroutine.h
//

#if !defined __TINYAN_NERUNE_PACKROUTINE__
#define __TINYAN_NERUNE_PACKROUTINE__

class CCheckFileDate;
class CPackRoutine
{
public:
	CPackRoutine();
	~CPackRoutine();
	void End(void);

	BOOL CreateDWQ(LPSTR srcDir0,LPSTR srcDir1,LPSTR srcDir2,LPSTR dstDir,BOOL taFlag,BOOL compFlag);
	BOOL ArcDWQ(LPSTR dstDir,int version = 0,LPSTR dstDir2 = NULL);





	BOOL PackInDir(LPSTR dirName,LPSTR extName);

	static char* m_nameData;
	static int MyCompare( const void* arg1, const void* arg2 );

private:
	BOOL CheckFileTailIsM(LPSTR filename);
	CCheckFileDate* m_checkFileDate;
	BOOL CreateDWQSub(WIN32_FIND_DATA* lpFind,LPSTR extName,LPSTR srcDir,LPSTR dstDir,BOOL taFlag,BOOL compFlag);
	BOOL Check256(LPSTR filename);

	void PrintFatalError(LPSTR filename,LPSTR caption);

	BOOL PackRoutine0(LPSTR srcDir,LPSTR filename,LPSTR extName,LPSTR dstDir);
	BOOL PackRoutine2(LPSTR srcDir,LPSTR filename,LPSTR extName,LPSTR dstDir);
	BOOL PackRoutine3(LPSTR srcDir,LPSTR filename,LPSTR extName,LPSTR dstDir);
	BOOL PackRoutine5(LPSTR srcDir,LPSTR filename,LPSTR extName,LPSTR dstDir);
	BOOL PackRoutine7(LPSTR srcDir,LPSTR filename,LPSTR extName,LPSTR dstDir);
	

//	int MakeAndSaveMask(FILE* file2, LPSTR bmpjpgDir,LPSTR subDir, LPSTR fileName);
	int MakeAndSaveMask(FILE* file2, LPSTR dirName,LPSTR filename);

	BOOL GetJpegSize(char* jpegData, SIZE* lpSize);

	char* HuffmanSearchCommand(char* dataPtr, int maxDataSize);
	int GetWORDData(char* jpegPtr);

	char* m_loadBuffer;
	char* m_loadBuffer2;
	char* m_saveBuffer;


	static char m_headerType0[];
	static char m_headerType2[];
	static char m_headerType3[];
	static char m_headerType5[];
	static char m_headerType7[];

//	RECT CalcuCutData(LPSTR bmpjpgDir,LPSTR subDir, LPSTR fileName);
	RECT CalcuCutData(LPSTR dirName,LPSTR filename);

	BOOL m_errorSkipFlag;

	void ChangeRGB(int sx,int sy);
	static int* m_nameTable;


};


#endif
/*_*/

