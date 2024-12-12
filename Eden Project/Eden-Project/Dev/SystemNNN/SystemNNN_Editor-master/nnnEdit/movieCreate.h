//
//
//

#if !defined __NNNEDIT_MOVIECREATE__
#define __NNNEDIT_MOVIECREATE__

class CMyApplication;
class CAviSave;
class CEffect;
class CPicture;
class CMovieCreate
{
public:
	CMovieCreate(CMyApplicationBase* app);
	~CMovieCreate();
	void End(void);

	static BOOL CALLBACK MyProceedDialogProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	static HWND m_staticText;
	static HWND m_progressBar;
	static HWND m_progressBar2;
	HWND m_proceedHWND;

	BOOL CreateProceedDialog(void);
	void SetProceedMessage(LPSTR mes);
	void SetProceedBar(int pos,int n = 0);
	BOOL DestroyProceedDialog(void);

	void MakeMovieAll(void);
	void MakeMovieAllFilm(void);
	void MakeMovieFilm(void);

	BOOL OpenMovieFile(LPSTR name,int frameTime);
	int MakeMovieFilmSub(CFilmData* pFilm,int amari = 0);

private:
	CMyApplicationBase* m_app;
	CAviSave* m_aviSave;
	HINSTANCE m_hInstance;
	CEffect* m_effect;

	CPicture* m_overrapPic;
	CPicture* m_overrapPic2;

};



#endif
/*_*/

