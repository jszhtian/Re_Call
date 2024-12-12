//
// main.h
//

#if !defined __TINYAN_RAVINCONFIG_MAIN__
#define __TINYAN_RAVINCONFIG_MAIN__

BOOL LoadSystemFile(void);
BOOL SaveSystemFile(void);

BOOL CALLBACK DlgProc0( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK DlgProc1( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK DlgProc2( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK DlgProc3( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK DlgProc4( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK DlgProc5( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK DlgProc6( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

void SetSoundModeButton(void);
void SetFontButton(void);
void Set16BitColorTileButton(void);

#endif
/*_*/

