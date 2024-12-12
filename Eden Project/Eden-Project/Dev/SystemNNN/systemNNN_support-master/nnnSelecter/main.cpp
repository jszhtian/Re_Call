//
// main.cpp
//

#include <windows.h>
#include <stdio.h>
#include <d3dx9.h>

//#include <commctrl.h>


#include "..\..\systemNNN\nyanLib\include\commonMacro.h"
#include "..\..\systemNNN\nnnUtilLib\nameList.h"

#include "..\..\systemNNN\nnnUtilLib\commonSystemFile.h"
#include "main.h"


#include "..\..\systemNNN\nnnUtilLib\mySaveFolder.h"

#include "resource.h"

CNameList* m_setup = NULL;

//#pragma comment(lib,"Comctl32.lib")

BOOL LoadSetupFile(LPSTR filenameonly, int varKosuu);
BOOL GetInitGameParam(int* lpVar, LPSTR name);
BOOL GetInitGameString(LPSTR* lpStr, LPSTR name);


char m_default2DExeName[]="nnnExe.exe";
char m_default3DExeName[]="nnn3DExe.exe";
//char m_defaultSelectExeName[]="nnnSelect.exe";

LPDIRECT3D9 m_D3D = NULL;


#define MYRELEASE(x) if ((x) != NULL){(x)->Release(); (x) = NULL;}


#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")


#if !defined _DEBUG
//#pragma comment(lib,"nyanLib.lib")
//#pragma comment(lib,"nnnUtilLib.lib")
#else
//#pragma comment(lib,"nyanLibD.lib")
//#pragma comment(lib,"nnnUtilLibD.lib")
#endif

int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{
	int md = 2;
	int needShader = 2;
	int maxShader = 2;

	LoadSetupFile("mainControl",512);
	
	int taikenFlag = 0;
	GetInitGameParam(&taikenFlag,"taikenFlag");

	LPSTR saveFolder = CMySaveFolder::GetFullFolder();
	char filenamebuf[1024];

	FILE* file = NULL;
	if (taikenFlag == 0)
	{
		wsprintf(filenamebuf,"%s\\sysfile.sav",saveFolder);
		fopen_s(&file,filenamebuf,"rb");

//		fopen_s(&file,"sav\\sysfile.sav","rb");
//		file = fopen("sav\\sysfile.sav","rb");
	}
	else
	{
		wsprintf(filenamebuf,"%s\\tsysfile.sav",saveFolder);
		fopen_s(&file,filenamebuf,"rb");

//		fopen_s(&file,"sav\\tsysfile.sav","rb");
//		file = fopen("sav\\tsysfile.sav","rb");
	}

	if (file == NULL)
	{
		if (taikenFlag == 0)
		{
			fopen_s(&file,"sav\\sysfile.org","rb");
//			file = fopen("sav\\sysfile.org","rb");
		}
		else
		{
			fopen_s(&file,"sav\\tsysfile.org","rb");
//			file = fopen("sav\\tsysfile.org","rb");
		}
	}

	if (file != NULL)
	{
		fseek(file,0,SEEK_END);
		int ln = ftell(file);
		char* buf = new char[ln];
		fseek(file,0,SEEK_SET);
		fread(buf,sizeof(char),ln,file);
		fclose(file);

		char* ptr = buf;
		ptr += sizeof(CCommonSystemFile::SYSTEMDATAINFO);
	
		CCommonSystemFile::SYSTEMDATA* systemData = (CCommonSystemFile::SYSTEMDATA*)ptr;
		md = systemData->d2d3d;	//0
		needShader = systemData->needShader;
		maxShader = systemData->shader;
		delete [] buf;
	}

	//check D3D and shader

	m_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_D3D != NULL)
	{
		D3DCAPS9 devCaps;
		m_D3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&devCaps);

		int vsv = 0;
		if (devCaps.VertexShaderVersion >= D3DVS_VERSION(1,1)) vsv = 1;
		if (devCaps.VertexShaderVersion >= D3DVS_VERSION(2,0)) vsv = 2;
		if (devCaps.VertexShaderVersion >= D3DVS_VERSION(3,0)) vsv = 3;

		D3DCAPS9 devCapsSoft;
		m_D3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,&devCapsSoft);
		int vsvs = 0;
		if (devCapsSoft.VertexShaderVersion >= D3DVS_VERSION(1,1)) vsvs = 1;
		if (devCapsSoft.VertexShaderVersion >= D3DVS_VERSION(2,0)) vsvs = 2;
		if (devCapsSoft.VertexShaderVersion >= D3DVS_VERSION(3,0)) vsvs = 3;

		int psv = 0;
		if (devCaps.PixelShaderVersion >= D3DPS_VERSION(1,1)) psv = 1;
		if (devCaps.PixelShaderVersion >= D3DPS_VERSION(2,0)) psv = 2;
		if (devCaps.PixelShaderVersion >= D3DPS_VERSION(3,0)) psv = 3;

		BOOL flg = FALSE;

		if (psv >= needShader)
		{
			if (vsv >= needShader)
			{
				flg = TRUE;
			}
			else
			{
				if (vsvs >= needShader)
				{
					flg = TRUE;
				}
			}
		}

		//check memory?
		if (flg)
		{
			md = 3;
		}

		MYRELEASE(m_D3D);
	}


	LPSTR filename = m_default2DExeName;
	if ((md == 2) || (md == 0))
	{
		GetInitGameString(&filename,"filename2DExe");
	}

	if (md == 3)
	{
		filename = m_default3DExeName;
		GetInitGameString(&filename,"filename3DExe");
	}

	ENDDELETECLASS(m_setup);

	ShellExecute(NULL,"open",filename,NULL,NULL,SW_SHOW);


	return 0;
}


BOOL LoadSetupFile(LPSTR filenameonly, int varKosuu)
{
	if (m_setup == NULL)
	{
		m_setup = new CNameList(varKosuu * 2);
	}

	if (m_setup == NULL) return FALSE;

	return m_setup->LoadInit(filenameonly);
}


BOOL GetInitGameParam(int* lpVar, LPSTR name)
{
	int rNum = m_setup->SearchName2(name);
	if (rNum == -1) return FALSE;

	*lpVar = atoi(m_setup->GetName(rNum + 1));

	return TRUE;
}


BOOL GetInitGameString(LPSTR* lpStr, LPSTR name)
{
	int rNum = m_setup->SearchName2(name);
	if (rNum == -1) return FALSE;

	*lpStr = m_setup->GetName(rNum + 1);

	return TRUE;
}


/*_*/



