#if !defined __TINYAN_MYFILESEARCH__
#define __TINYAN_MYFILESEARCH__


class CMyFileSearch
{
public:
	CMyFileSearch();
	~CMyFileSearch();
	void End(void);

	BOOL SearchStart(LPSTR dirName,LPSTR fileName);
	LPSTR GetName(void);
	void SearchEnd(void);

	BOOL SearchDirStart(LPSTR dirName);
	LPSTR GetDir(void);
	void SearchDirEnd(void);
private:
	char* m_win32FindData;
	char* m_win32FindData2;

	char* m_win32FindDataDir;
	char* m_win32FindData2Dir;

	BOOL m_startFlag;
	BOOL m_secondFlag;
	HANDLE m_handle;

	BOOL m_startFlagDir;
	BOOL m_secondFlagDir;
	HANDLE m_handleDir;

};

#endif
/*_*/

