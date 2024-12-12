//
// packroutine.h
//

#if !defined __DEVILREVERSE_UNPACKROUTINE__
#define __DEVILREVERSE_UNPACKROUTINE__

//class CCheckFileDate;
class CMyFileSearch;

class CUnpackRoutine
{
public:
	CUnpackRoutine();
	~CUnpackRoutine();
	void End(void);

	BOOL CreateDWQ(LPSTR srcDir0,LPSTR srcDir1,LPSTR srcDir2,LPSTR dstDir,BOOL taFlag,BOOL compFlag);
	BOOL ArcDWQ(LPSTR dstDir,int version = 0,LPSTR dstDir2 = NULL);





	BOOL PackInDir(LPSTR dirName,LPSTR extName);

	static char* m_nameData;
//	static int MyCompare( const void* arg1, const void* arg2 );

	void SetProgressWindow(HWND staticMessage,HWND bar1,HWND bar2);

	BOOL BunkaiToDWQ(int version);
	BOOL ConvertToBMP(void);
	BOOL BunkaiToVAW(void);
	BOOL ConvertToWAV(void);

	int GetVawErrorCount(void){return m_vawErrorCount;}
	int GetDwqErrorCount(void){return m_dwqErrorCount;}
private:
	char* m_loadBuffer;
	char* m_loadBuffer2;
	char* m_saveBuffer;

//	static char m_headerType0[];
//	static char m_headerType2[];
//	static char m_headerType3[];
//	static char m_headerType5[];
//	static char m_headerType7[];

//	RECT CalcuCutData(LPSTR bmpjpgDir,LPSTR subDir, LPSTR fileName);
////	RECT CalcuCutData(LPSTR dirName,LPSTR filename);

	static char m_dwqType[][4];
	static char m_dwqDir[][8];

	static char m_vawDir[][4];

	BOOL m_errorSkipFlag;

//	void ChangeRGB(int sx,int sy);
//	static int* m_nameTable;

	HWND m_staticText;
	HWND m_progressBar;
	HWND m_progressBar2;

	CMyFileSearch* m_myFileSearch;

	BOOL BunkaiToDWQRoutine(LPSTR name,int version,int type);
	BOOL BunkaiToVAWRoutine(LPSTR name);

	void SetStaticMessage(LPSTR mes);

	void IncBar(void);
	int m_barCount;

	BOOL ConvertVAWRoutine(LPSTR filenameonly,LPSTR srcDir,LPSTR dstDir);
	BOOL ConvertWGQRoutine(LPSTR filenameonly);
	int m_vawErrorCount;
	int m_dwqErrorCount;
	BOOL ConvertDWQRoutine(LPSTR filenameonly,int type);

	void ReverseBMP(void);
	int UnPackBmp(void);
	int MakeMask(void);
};


#endif
/*_*/

