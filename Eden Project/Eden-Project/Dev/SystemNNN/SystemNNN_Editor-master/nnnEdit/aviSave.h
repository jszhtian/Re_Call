//
//
//

#if !defined __TINYAN_NNNEDIT_AVISAVE__
#define __TINYAN_NNNEDIT_AVISAVE__


class CAviSave
{
public:
	CAviSave(int sizeX=800,int sizeY = 600);
	~CAviSave();
	void End(void);

	BOOL CreateAVIFile(LPSTR filename,int totalFrame,int dwRate = 3000,int dwScale = 100);

	void SetData(char* ptr = NULL);
	void CloseAviFile(void);

	void SetMovieSize(int n){m_movieSize = n;}
	void SetMoviePackType(int n){m_moviePackType = n;}

	typedef struct {
		DWORD dwMicroSecPerFrame;
		DWORD dwMaxBytesPerSec;
		DWORD dwReserved1;
		DWORD dwFlags;
		DWORD dwTotalFrames;
		DWORD dwInitialFrames;
		DWORD dwStreams;
		DWORD dwSuggestedBufferSize;
		DWORD dwWidth;
		DWORD dwHeight;
		DWORD dwReserved[4];
	} MainAVIHeader;

	typedef struct {
		FOURCC fccType;
		FOURCC fccHandler;
		DWORD dwFlags;
		DWORD  dwPriority;
		DWORD dwInitialFrames;
		DWORD  dwScale;
		DWORD  dwRate;
		DWORD  dwStart;
		DWORD  dwLength;
		DWORD dwSuggestedBufferSize;
		DWORD  dwQuality;
		DWORD  dwSampleSize;
	   // RECT   rcFrame;
		short top,left,right,bottom;
	} AVIStreamHeader;

private:
	int m_sizeX;
	int m_sizeY;

	int m_dataSizeX;
	int m_dataSizeY;
	int m_dwRate;
	int m_dwScale;
	int m_totalFrame;
	FILE* m_file;
	char* m_buffer;

	void PacMMX(void);

	int m_movieSize;
	int m_moviePackType;
	void Shukusho(void);
	void SuperShukusho(void);
};

#endif
/*_*/

