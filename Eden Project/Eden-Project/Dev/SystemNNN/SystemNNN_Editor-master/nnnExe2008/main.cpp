//
// main.cpp
//


#include <windows.h>
#include <stdio.h>

//#include <Math.h>
//#include <bits.nan.h>

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "..\..\systemNNN\nnnUtilLib\commongameVersion.h"

#include "..\..\systemNNN\nnnLib\mainControl.h"
#include "..\..\systemNNN\nnnLib\gameCallBack.h"

#include "..\..\systemNNN\nnnUtilLib\currentDirControl.h"

#include "game.h"
#include "main.h"


#include "..\..\systemNNN\nyanlib\include\libinclude.h"



#if !defined _DEBUG
#pragma comment(lib,"d:\\libpng\\lib\\zlib.lib")
#pragma comment(lib,"d:\\libpng\\lib\\libpng.lib")
#else
#pragma comment(lib,"d:\\libpng\\lib\\zlibd.lib")
#pragma comment(lib,"d:\\libpng\\lib\\libpngd.lib")
//#pragma comment(lib,"d:\\libpng\\lib\\1.2.4\\zlibD.lib")
//#pragma comment(lib,"d:\\libpng\\lib\\1.2.4\\libpngD.lib")
#endif












//402387260077093773543702433923003985719374864210714632543799910429938512398629020592044208486969404800479988610197196058631666872994808558901323829669944590997424504087073759918823627727188732519779505950995276120874975462497043601418278094646496291056393887437886487337119181045825783647849977012476632889835955735432513185323958463075557409114262417474349347553428646576611667797396668820291207379143853719588249808126867838374559731746136085379534524221586593201928090878297308431392844403281231558611036976801357304216168747609675871348312025478589320767169132448426236131412508780208000261683151027341827977704784635868170164365024153691398281264810213092761244896359928705114964975419909342221566832572080821333186116811553615836546984046708975602900950537616475847728421889679646244945160765353408198901385442487984959953319101723355556602139450399736280750137837615307127761926849034352625200015888535147331611702103968175921510907788019393178114194545257223865541461062892187960223838971476088506276862967146674697562911234082439208160153780889893964518263243671616762179168909779911903754031274622289988005195444414282012187361745992642956581746628302955570299024324153181617210465832036786906117260158783520751516284225540265170483304226143974286933061690897968482590125458327168226458066526769958652682272807075781391858178889652208164348344825993266043367660176999612831860788386150279465955131156552036093988180612138558600301435694527224206344631797460594682573103790084024432438465657245014402821885252470935190620929023136493273497565513958720559654228749774011413346962715422845862377387538230483865688976461927383814900140767310446640259899490222221765904339901886018566526485061799702356193897017860040811889729918311021171229845901641921068884387121855646124960798722908519296819372388642614839657382291123125024186649353143970137428531926649875337218940694281434118520158014123344828015051399694290153483077644569099073152433278288269864602789864321139083506217095002597389863554277196742822248757586765752344220207573630569498825087968928162753848863396909959826280956121450994871701244516461260379029309120889086942028510640182154399457156805941872748998094254742173582401063677404595741785160829230135358081840096996372524230560855903700624271243416909004153690105933983835777939410970027753472000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000


//#define __DEMO_VERSION__

int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{



//	float aa = NAN;
//	float bb = NaN;



/*
aaa
/* aaa */
/*
bbb
	*/

	
	//	for (int a=1,i,j;;scanf("%d",i),j=i%10,i/=10,a=a-i?a-j:a?i:j,printf("%d",a));

/*
	int* ans=new int[400000];
	int* buf=new int[400000];
	int ans_len;
	int buf_len;
	ans_len = 1;
	buf_len = 1;
	for (int i=0;i<400000;i++)
	{
		ans[i] = 0;
		buf[i] = 0;
	}

	ans[0] = 1;
	for (int i=2;i<=1000;i++)
	{
		buf_len = ans_len;
		for (int k=0;k<buf_len;k++)
		{
			buf[k] = 0;
		}

		for (int k=0;k<ans_len;k++)
		{
			int d = ans[k] * i;
			
			int keta = k;
			while (d>0)
			{
				d += buf[keta];
				buf[keta] = d % 10;
				d /= 10;
				keta++;
				if (d>0)
				{
					if (keta > buf_len-1)
					{
						buf[keta]=0;
						buf_len = keta+1;
					}
				}
			}
		}


		ans_len = buf_len;
		for (int k=0;k<buf_len;k++)
		{
			ans[k] = buf[k];
		}
	}


	for (int i=ans_len-1;i>=0;i--)
	{
		char mes[256];
		sprintf_s(mes,256,"%d",ans[i]);
		OutputDebugString(mes);
	}

	delete [] buf;
	delete [] ans;

	return 0;
*/


/*



	FILE* file0 = NULL;
	FILE* file1 = NULL;


	char oldfile[1024];
	WIN32_FIND_DATA m_findData;

	HANDLE m_handle = FindFirstFile("cdvaw\\*.vtb",&m_findData);
	while (m_handle)
	{
		char filename1[1024];
		char filename2[1024];
		sprintf_s(filename1,1024,"cdvaw\\%s",m_findData.cFileName);

		for (int i=0;i<1022;i++)
		{
			filename2[i] = filename1[i];
			if (filename2[i] == '.')
			{
				filename2[i+1] = 'v';
				filename2[i+2] = 'p';
				filename2[i+3] = 'k';
				filename2[i+4] = 0;
				break;
			}
		}

		fopen_s(&file0,filename1,"rb");
		fopen_s(&file1,filename2,"rb");

		if ((file0 != NULL) && (file1 != NULL))
		{
			OutputDebugString("\nchecking");
			OutputDebugString(filename1);
			OutputDebugString("...");
			char tmp[10];
			tmp[8] = tmp[9] = 0;
			int offset;
			while (fread(tmp,sizeof(char),8,file0) > 0)
			{
				fread(&offset,sizeof(int),1,file0);

				fseek(file1,offset,SEEK_SET);
				char data[16];
				fread(data,sizeof(char),16,file1);

				if ((data[0] != 'I') || (data[1] != 'F'))
				{
					OutputDebugString(tmp);
					char ermes[1024];
					sprintf_s(ermes,1024,"bad file=%s",oldfile);
					MessageBox(NULL,ermes,"問題発見",MB_OK | MB_ICONEXCLAMATION);
					break;
				}

				memcpy(oldfile,tmp,10);
			}



			fclose(file0);
			fclose(file1);
		}
		else
		{
			OutputDebugString("open error");
		}


		if (FindNextFile(m_handle,&m_findData) == FALSE)
		{
			FindClose(m_handle);
			m_handle = NULL;
			break;
		}
	}



	MessageBox(NULL,"チェック終了","end",MB_OK);

	return 0;

*/




	CCurrentDirControl::ChangeCurrentToExe();

/*
	int m_winNTFlag = FALSE;
	OSVERSIONINFO osv;
	ZeroMemory(&osv,sizeof(osv));
	osv.dwOSVersionInfoSize = sizeof(osv);
	if (GetVersionEx(&osv))
	{
		if (osv.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			if (osv.dwMajorVersion >= 4)
			{
				m_winNTFlag = TRUE;
			}
		}
	}
*/



	//
	// 必要ならここで体験版フラグなどをたてる
	//
//	CCommonGameVersion::SetTaikenVersion();
//	CCommonGameVersion::SetLoadOk();
//	CCommonGameVersion::SetSaveOk();

	//
	// 必要ならここで独自のwheelMouse,systemfileを作成する
	//
	m_game = NULL;
//	m_wheelMouse = new CWheelMouse();
//	m_systemFile = new CCommonSystemFile();
//	m_systemFile->Load(TRUE);


	m_mainControl = new CMainControl();	//コンストラクタでwheelMouse,systemfileを指定できる 指定なし、またはNULLだと内部で標準のを作成する

	if (m_mainControl->CheckSystem())
	{
		HICON icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		HWND hWnd = m_mainControl->CreateWindowRoutine(hInstance,icon);//hinstance,icon,proc    proc == NULLだとCMainControl内部の標準callBack

		CCommonSystemFile* lpSystemFile = m_mainControl->GetSystemFile();
		SIZE sz = m_mainControl->GetWindowSize();
		int bpp = m_mainControl->GetBpp();
		m_game = new CGame(hWnd,hInstance,lpSystemFile,sz.cx,sz.cy,bpp);
		m_mainControl->Game(m_game);
	}

	ENDDELETECLASS(m_game);
	ENDDELETECLASS(m_mainControl);

	CCurrentDirControl::RestoreCurrent();

	return 0;
}






/*_*/


