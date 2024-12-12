//
//
//

#if !defined __TINYAN_MYFINDFILE__
#define __TINYAN_MYFINDFILE__


class CMyFindFile
{
public:
	CMyFindFile();
	~CMyFindFile();
	void End(void);

	BOOL SearchStart(LPSTR searchName);
	LPSTR SearchNext(void);
	LPSTR SearchNextDir(void);
	void SearchEnd(void);

private:
	HANDLE m_handle;
	WIN32_FIND_DATA m_findData;
	BOOL m_firstFlag;

	char m_filenameOnly[1024];
};

#endif
/*_*/
