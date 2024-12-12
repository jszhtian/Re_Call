//
//
//

#if !defined __NNNEXE_GAME__
#define __NNNEXE_GAME__


//#include "..\nnnLib\gameCallBack.h"

class CCommonSystemFile;
class CNameList;

//class CMakeParty;
//class CPartyStatusList;
//class CSelectMagicMenu;
//class CUtil;

//class CCharaStart;
//class CHouseLevel;

//class CCharaNumber;
//class CGameMouse;

//class CModePlate;

//class CKatakana;

//class CAreaHaveHouse;

//class COkimonoList;
//class CMoveMaskTable;

//class CHouseToSerial;

//class CEventList;

//class CDungeonAreaList;

//class CGameItem;
//class CHaveItem;

//class CCharaStatus;
//class CCastleLevel;

class CGameCallBack;

class CGame : public CGameCallBack
{
public:
	CGame(HWND hwnd, HINSTANCE hinstance, CCommonSystemFile* lpSystemFile, int windowSizeX, int windowSizeY, int bpp);
	~CGame();
	void End(void);

//	int MainLoop(int cnt);
//	LRESULT GameProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


//	void ReceiveScriptCommand(int cmd, int para1 = 0, LPVOID para2 = NULL);
	void ReceiveUserCommand(int cmd, int paraKosuu, int* paraPtr);
	void ReceiveUserFunction(int cmd, int paraKosuu, int* paraPtr);

//	static int m_daysOfMonth[];





	void UserCommandCallBack(void);
	



	/////////////////////////////////////////////Ç±ÇÃÇ÷ÇÒÇ…ïKóvÇ»ä÷êîÇí«â¡

	//åpè≥

//	void GetExtDataForSave(LPVOID ptr,int extNumber = 0);
//	void SetExtDataByLoad(LPVOID ptr,int extNumber = 0);

	void AfterPrint(void);

	void Create(void);

private:
	void CreateAllClass(void);
//	void AfterInitNewGame(int uraMode = 0, BOOL demoFlag = FALSE);


	int m_selectPlaceCommandNumber;
	int m_selectHeroinCommandNumber;
	int m_printCalendarCommandNumber;
	int m_printMovieCommandNumber;

	BOOL CheckDebugOk(void);

	int m_clearMessageFunctionVarNumber;

//	int m_upPrintX;
//	int m_upPrintY;
//	int m_downPrintX;
//	int m_downPrintY;


	int m_updownPageMax;

	BOOL m_updownMode;
	CPicture* m_dmy;
	CPicture* m_dmy2;

};





#endif
/*_*/

