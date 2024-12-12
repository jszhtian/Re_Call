//
// myfileopen.h
//

#if !defined __TINYAN_NNNEDIT_MYFILEOPEN__
#define __TINYAN_NNNEDIT_MYFILEOPEN__


class CPicture;

class CMyFileOpen
{
public:
	CMyFileOpen(HWND parentHWND,HINSTANCE hinstance,LPSTR lpStrDefExt, LPSTR lpStrFilter = NULL);
	~CMyFileOpen();
	void End(void);

	LPSTR SelectFile(LPSTR defaultName = NULL, LPSTR defaultDirName = NULL,LPSTR ext =NULL,int filetLength = 0);
	FILE* OpenLoad(LPSTR defaultName = NULL, LPSTR defaultDirName = NULL);
	FILE* OpenSave(LPSTR defaultName = NULL, LPSTR defaultDirName = NULL, BOOL backupFlag = FALSE);

	LPSTR GetFileName(void);
	LPSTR GetFileNameOnly(void);

	void SetDefaultExt(LPSTR lpStrDefExt);
	void SetDefaultFilter(LPSTR lpStrDefFilter);

	void InitFileName(LPSTR filename);

	static UINT CALLBACK OFNHookProcOldStyle(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
	static UINT CALLBACK OFNHookProc(HWND hdlg, UINT uiMsg,  WPARAM wParam,  LPARAM lParam);

	UINT MyDialogProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);

	static void* m_readBuffer;

	static HBITMAP m_dib1;
	static HBITMAP m_dib2;
	static LPVOID m_dibBuffer1;
	static LPVOID m_dibBuffer2;
	static BOOL m_dibExistFlag1;
	static BOOL m_dibExistFlag2;
	static char m_infoText[];

	static int m_256x192Pic[];
	static void MakeDWQInfo(LPSTR filename);
	static void MakeNNNInfo(LPSTR filename);

	static CPicture* m_pic;
	static char m_currentDir[];

	typedef struct _tagMYOPENFILENAME
	{
		BOOL enableFlag;
		BOOL checkFlag;
		int filterLength;
		LPSTR filename;
	} MYOPENFILENAME;

	void SetSameEnable(BOOL b = TRUE);
	static void SetSameZahyoFlag(BOOL b);
	static BOOL GetSameZahyoFlag(void);
	int GetFilterLength(void){return m_filterLength;}

	static int* Get256x192Buffer(void){return m_256x192Pic;}
	BOOL CheckFilenameHaveSpace(LPSTR fullName);

	static HWND m_filenameEditBox;
	static BOOL CALLBACK MyEditBoxEnum(HWND hwnd,LPARAM lParam);

	static void ChangeDWQFilter(MYOPENFILENAME* lpMy);

//	static BOOL m_sameCheckFlag;
//	static void SetSameCheckFlag(BOOL flg = TRUE){m_sameCheckFlag = flg;}
//	static BOOL GetSameCheckFlag(void){return m_sameCheckFlag;}

private:
	void MakeFileNameOnly(void);

	void Init(void);

	void CreateMyDIB(HBITMAP* lpBitmap, LPVOID* lplpBuffer,int sizeX,int sizeY);

	char m_fileName[1024];
	char m_fileTitle[256];
	char m_fileNameOnly[256];

	char m_defaultDir[1024];

	char m_defaultExt[1024];
	char m_defaultFilter[1024];

	static char m_filterName[1024];


	HWND m_parentHWND;
	HINSTANCE m_hInstance;

	static BOOL m_sameZahyoFlag;
	BOOL m_sameEnableFlag;

	int m_filterLength;
};



#endif
/*_*/

