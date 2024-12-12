//
// myload.h
//

#if !defined __TINYAN_NERUNE_MYLOAD__
#define __TINYAN_NERUNE_MYLOAD__

class CMyLoad
{
public:
	CMyLoad();
	~CMyLoad();
	void End(void);

	int LoadWave(LPSTR waveFilename,void* headerBuffer, void* dataBuffer);
	int LoadOgg(LPSTR waveFilename,LPSTR oggFilename,void* headerBuffer, void* dataBuffer);
private:
};

#endif
/*_*/
