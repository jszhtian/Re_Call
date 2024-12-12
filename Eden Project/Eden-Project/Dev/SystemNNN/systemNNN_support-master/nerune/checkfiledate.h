//
// checkfiledate.h
//

#if !defined __TINYAN_DIANAWAVEPAC_CHECKFILEDATE__
#define __TINYAN_DIANAWAVEPAC_CHECKFILEDATE__


class CCheckFileDate
{
public:
	CCheckFileDate();
	~CCheckFileDate();
	void End(void);

	BOOL CheckNeedUpdate(LPSTR filename,LPSTR srcDir,LPSTR dstDir,LPSTR extName,BOOL taFlag);

	BOOL CheckNeedUpdate(LPSTR src,LPSTR dst);

	//BOOL CheckNeedUpdate(LPSTR dirName, LPSTR subDir,LPSTR filenamem, LPSTR extName, BOOL bUseMaskFlag = FALSE);
private:
	void PrintFatalError(LPSTR filename);
	int m_errorSkipFlag;

};

#endif
/*_*/

