//
//
//

#if !defined __NYANDIRECTX_SHOW__
#define __NYANDIRECTX_SHOW__



class CMyDirectShow
{
public:
	CMyDirectShow(HWND hwnd,int message,int useVMR9Flag = 0);
	~CMyDirectShow();
	void End(void);

	int CheckCodec(LPSTR filename);
	BOOL PlayMovie(LPSTR filename,LONGLONG seekTime = -1);
	BOOL StopMovie(void);

	BOOL OnNotify(void);

	BOOL OnPaint(HWND hwnd,HDC hdc);
	BOOL OnDisplayChanged(void);
	BOOL OnSize(void);

	BOOL CheckComplete(void){return m_completeFlag;}

	BOOL Pause(void);
	BOOL Resume(void);

	void SetWindowSize(SIZE sz){m_windowSize = sz;}
	void SetFullMonitorSize(SIZE sz){m_fullMonitorSize = sz;}
	void ChangeVolume(int vol);
	/*
	
	
	void ChangeToFullScreen(HWND hwnd);
	void ChangeToWindowScreen(HWND hwnd);

	void SetWindowMode(int md){m_fullScreenFlag = md;}
*/
private:
	HWND m_parentHWnd;
	int m_graphNotify;

//	IFilterGraph* m_filterGraph;
//	IGraphBuilder*	m_graphBuilder;
//	IBaseFilter* m_vmr9;
//	IVMRFilterConfig9* m_vmrFilterConfig9;
//	IVMRWindowlessControl9* m_vmrWindowLessControl9;
//	IMediaEventEx*	m_mediaEventEx;
//	IMediaControl*	m_mediaControl;

	LPVOID m_filterGraph_;
	LPVOID	m_graphBuilder;
	LPVOID m_vmr9;
	LPVOID m_vmrFilterConfig9;
	LPVOID m_vmrWindowLessControl9;
	LPVOID	m_mediaEventEx;
	LPVOID	m_mediaControl;
	LPVOID m_basicAudio;
	LPVOID m_mediaSeek;

//	LPVOID m_sourceFilter;
//	LPVOID m_directSoundFilter;

	LPVOID m_captureBuilder;

	BOOL m_playFlag;
	BOOL m_completeFlag;

	int m_volume;



	LPVOID	m_videoWindow;

/*
	ISampleGrabber* m_sampleGrabber;
	IMediaControl*	m_mediaControl;
	IMediaEvent*	m_mediaEvent;
	IBasicAudio*	m_basicAudio;
	IVideoWindow*	m_videoWindow;
	IBasicVideo*	m_basicVideo;

	IBaseFilter*	m_baseFilter;
	IBaseFilter*	m_pNullFilter;

	BOOL m_childFlag;
	BOOL m_playFlag;


	BOOL m_completeFlag;
	BOOL m_fullScreenFlag;
*/

	void ReleaseRoutine(void);


	void RemoveAllFilter(LPVOID graphBuilder);
	void ReleaseBuilders(void);
	void ReleaseAllFilter(void);

	void ReleaseVMR9(void);
	BOOL CreateVMR9(void);

	LONGLONG m_pauseTime;

	int m_useVMR9Flag;
	int m_vmr9RenderType;

	char m_lastFileName[256];

	BOOL CheckVMR9Connected(void);
	BOOL m_vmrErrorFlag;

	HRESULT SourceRenderEx(LPVOID pSourceFilter);
	SIZE m_windowSize;
	SIZE m_fullMonitorSize;

	int OpenMovieFile(LPSTR filename);

	HRESULT CreateGraphBuilder(void);

	HRESULT GetVideoWindowInterface(void);
	HRESULT GetMediaControlInterface(void);
	HRESULT GetMediaEventExInterface(void);
	HRESULT GetBasicAudioInterface(void);
	HRESULT GetMediaSeekInterface(void);

	HRESULT WaitMediaControl(int ms = 50,int loop = 10);

};

#endif
/*_*/

