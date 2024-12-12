//
// myloadwave.h
//

#if !defined __TINYAN_DIANAWAVEPAC_MYLOADWAVE__
#define __TINYAN_DIANAWAVEPAC_MYLOADWAVE__

class CMyLoadWave
{
public:
	CMyLoadWave();
	~CMyLoadWave();
	void End(void);

	int LoadWave(LPSTR subDir,LPSTR filename,void* headerBuffer, void* dataBuffer,int type = 1);
	int NoiseCut(void* buf, int sz);
	void NoiseCut2(void* buf, int sz);

	static char m_srcDir[];
	static char m_dstDir[];
	static char m_srcDir2[];
	static char m_dstDir2[];
	static char m_srcDir3[];
	static char m_dstDir3[];
	static char m_srcExt[];
	static char m_dstExt[];
	static char m_srcExt3[];

private:
};

#endif
/*_*/
