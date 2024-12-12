//
// makefont.cpp
//



#include <windows.h>
#include <stdio.h>
#include <math.h>


#include "main.h"

#include "resource.h"


BITMAPV4HEADER m_bmi;


char fontname[256] ="‚l‚r ƒSƒVƒbƒN";
char m_userFontName[256] = "‚l‚r ƒSƒVƒbƒN";


int* m_lpBuffer;
int* m_makedData = NULL;
char* m_kosuuWork;
int* m_kosuuTable;

void MakeData(void);

int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme,  int FontType,   LPARAM lParam );
BOOL CALLBACK DlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
void SetSelectUserFontList(void);
void GetSelectUserFontList(void);


char m_filename[256] = "default";


int m_fontKosuu = 0;


int m_fontSize = 24;
int m_realFontSize = 24;

//2bit type,4bittype,8bittype

char fontHeader[]="\
MFT             \
                \
FONTSIZE=000    \
PACKTYPE=0     ";



int m_bai = 3;
int m_mul = 2;

HWND m_editSeido;
HWND m_radio2Bit;
HWND m_radio4Bit;
HWND m_radio8Bit;
HWND m_editFileName;
HWND m_fontNameList;
HWND m_editFontSize;




int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{
	int rt = DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG_MAKEFONT),NULL,DlgProc,(LPARAM)NULL);

	m_fontSize = (m_realFontSize + 7) & (~7);


	

//	return 0;

	if (m_bai>8) m_bai = 8;
	if (m_bai<2) m_bai = 2;

	if (m_mul<2) m_mul = 2;
	if (m_mul>8) m_mul = 8;
	if ((m_mul>=3) && (m_mul<=7)) m_mul = 4;

	


	int nMax = (1<<m_bai);
	m_kosuuTable = new int[nMax+1];

	int mMax = (1<<m_mul);

	for (int i=0;i<nMax+1;i++)
	{
		int d = (i * (mMax-1) + nMax / 2) / nMax;
		if (d<0) d = 0;
		if (d>=mMax) d = mMax - 1;

		m_kosuuTable[i] = d;
	}


	m_makedData = new int[m_fontSize * m_fontSize / sizeof(int) * m_mul / 8];
	m_kosuuWork = new char[m_fontSize * m_fontSize *m_bai*m_bai];

	HFONT font = CreateFont(
								m_realFontSize*m_bai,m_realFontSize*m_bai/2,
								0,0,
								FW_BOLD,FALSE,FALSE,FALSE,
								SHIFTJIS_CHARSET,
								OUT_DEFAULT_PRECIS, 
								CLIP_DEFAULT_PRECIS ,
								DEFAULT_QUALITY,
								FIXED_PITCH | FF_DONTCARE,
								m_userFontName);


	ZeroMemory(&m_bmi,sizeof(BITMAPV4HEADER));


	m_bmi.bV4BitCount = 32;
	m_bmi.bV4AlphaMask = 0xff000000;
	m_bmi.bV4BlueMask = 0x000000ff;
	m_bmi.bV4GreenMask = 0x0000ff00;
	m_bmi.bV4Width = m_fontSize * m_bai;
	m_bmi.bV4V4Compression = BI_RGB;
	m_bmi.bV4Size = sizeof(BITMAPV4HEADER);
	m_bmi.bV4RedMask = 0x00ff0000;
	m_bmi.bV4Planes = 1;
	m_bmi.bV4Height = -m_fontSize*m_bai;


	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetDC(hwnd);

	HBITMAP m_hBitmap = CreateDIBSection(hdc,(BITMAPINFO*)&m_bmi,DIB_RGB_COLORS,(void**)(&m_lpBuffer),NULL,0);

	HDC m_src = CreateCompatibleDC(hdc);
	HGDIOBJ m_oldBitmap = SelectObject(m_src,m_hBitmap);
	int m_oldBkMode = SetBkMode(m_src,TRANSPARENT);
	int m_oldTextColor = SetTextColor(m_src,0xffffff);

	ReleaseDC(hwnd,hdc);



	HGDIOBJ m_oldFont = SelectObject(m_src,font);


	char filename[256];
	wsprintf(filename,"%s.mft",m_filename);
	FILE* file = NULL;
	fopen_s(&file,filename,"wb");

	//


	fontHeader[32+9] = (m_realFontSize / 100) + '0';
	fontHeader[32+10] = ((m_realFontSize / 10) % 10) + '0';
	fontHeader[32+11] = (m_realFontSize % 10) + '0';

	fontHeader[48+9] = m_mul + '0';
	fwrite(fontHeader,sizeof(char),64,file);


	char mes[3];
	mes[2] = 0;


	for (int n=0;n<47;n++)
	{
		int h = 0x81 + n;

		if (n>=31)
		{
			h = 0xe0 + (n-31);
		}

		for (int l=64-1;l<256-1;l++)
		{
			ZeroMemory(m_lpBuffer,m_fontSize*m_bai*m_fontSize*m_bai*4);

			mes[0] = h;
			mes[1] = l;

			TextOut(m_src,0,0,mes,2);

			//make data
			MakeData();

			//save data
			fwrite(m_makedData,sizeof(char),m_fontSize*m_fontSize*m_mul/8,file);
		}
	}


	fclose(file);













	SelectObject(m_src,(HGDIOBJ)m_oldFont);


	SelectObject(m_src,(HGDIOBJ)m_oldBitmap);
	SetBkMode(m_src,m_oldBkMode);

	if (m_oldTextColor != CLR_INVALID)
	{
		SetTextColor(m_src,m_oldTextColor);
	}

	DeleteDC(m_src);
	DeleteObject(m_hBitmap);

	DeleteObject(font);

	delete [] m_kosuuTable;
	delete [] m_kosuuWork;
	delete [] m_makedData;

	return 0;
}


void MakeData(void)
{
	//‚©‚¼‚¦‚é

	int bai = m_bai;

	int n = 0;
	for (int j=0;j<m_fontSize*bai;j+=bai)
	{
		for (int i=0;i<m_fontSize*bai;i+=bai)
		{
			int* ptr = m_lpBuffer + i + j * m_fontSize*bai;

			int kosuu = 0;

			for (int jj=0;jj<bai;jj++)
			{
				for (int ii=0;ii<bai;ii++)
				{
					if ((*(ptr+ii+jj*m_fontSize*bai)) != 0) kosuu++;
				}
			}
			
			int k = m_kosuuTable[kosuu];
			m_kosuuWork[n] = k;
			n++;
		}
	}




	char* src = m_kosuuWork;
	int* dst = m_makedData;

	int srcPitch = m_fontSize;
	int srcPitch4 = srcPitch * (8/m_mul);

//	int srcNext = m_fontSize; 

	int loopX = (m_fontSize) / 8;
	int loopY = (m_fontSize) / (8/m_mul);

	int types = m_mul;


	__asm
	{
		push eax
		push ebx
		push ecx
		push edx
		push esi
		push edi

		pxor mm0,mm0

		mov ebx,srcPitch
		mov esi,src
		mov edi,dst

		mov eax,03030303h
		movd mm7,eax
		punpckldq mm7,mm7

		mov eax,0f0f0f0fh
		movd mm6,eax
		punpckldq mm6,mm6

		mov eax,types
		cmp eax,8
		jz TYPE8
		cmp eax,4
		jz TYPE4

		mov ecx,loopY
LOOP2:
		push ecx
		push esi

		mov ecx,loopX
		mov edx,esi
		add edx,ebx
		add edx,ebx
LOOP3:
		movq mm1,[esi]
		movq mm2,[ebx+esi]
		movq mm3,[edx]
		movq mm4,[edx+ebx]

		pand mm1,mm7
		pand mm2,mm7
		pand mm3,mm7
		pand mm4,mm7
		psllw mm2,2	//‚¾‚¢‚¶‚å‚¤‚Ô
		psllw mm3,4
		psllw mm4,6
		por mm1,mm2
		por mm1,mm3
		por mm1,mm4

		movq [edi],mm1
		add edi,8

		add esi,8
		add edx,8
		dec ecx
		jnz LOOP3

		pop esi
		pop ecx
		add esi,srcPitch4
		dec ecx
		jnz LOOP2

		jmp EXIT1



TYPE4:
		mov ecx,loopY
LOOP12:
		push ecx
		push esi

		mov ecx,loopX
	//	mov edx,esi
LOOP13:
		movq mm1,[esi]
		movq mm2,[ebx+esi]

		pand mm1,mm6
		pand mm2,mm6
		psllw mm2,4	//‚¾‚¢‚¶‚å‚¤‚Ô
		por mm1,mm2

		movq [edi],mm1
		add edi,8

		add esi,8
	//	add edx,8
		dec ecx
		jnz LOOP13

		pop esi
		pop ecx
		add esi,srcPitch4
		dec ecx
		jnz LOOP12

		jmp EXIT1



TYPE8:

		mov ecx,loopY
LOOP22:
		push ecx
		push esi

		mov ecx,loopX
		mov edx,esi
LOOP23:
		movq mm1,[esi]
		movq [edi],mm1
		add edi,8

		add esi,8
		add edx,8
		dec ecx
		jnz LOOP23

		pop esi
		pop ecx
		add esi,srcPitch4
		dec ecx
		jnz LOOP22


EXIT1:
		emms

		pop edi
		pop esi
		pop edx
		pop ecx
		pop ebx
		pop eax
	}
}





BOOL CALLBACK DlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{

	switch (message)
	{
	case WM_INITDIALOG:

		m_fontNameList = GetDlgItem(hWnd,IDC_COMBO_FONTNAME);

		m_editSeido = GetDlgItem(hWnd,IDC_EDIT_SEIDO);
		m_radio2Bit = GetDlgItem(hWnd,IDC_RADIO_2BIT);
		m_radio4Bit = GetDlgItem(hWnd,IDC_RADIO_4BIT);
		m_radio8Bit = GetDlgItem(hWnd,IDC_RADIO_8BIT);
		m_editFileName = GetDlgItem(hWnd,IDC_EDIT_FILENAME);
		m_editFontSize = GetDlgItem(hWnd,IDC_EDIT_FONTSIZE);
		SendMessage(m_radio2Bit,BM_SETCHECK,BST_CHECKED,0);
		SetWindowText(m_editSeido,"4");
		SetWindowText(m_editFileName,"default");
		SetWindowText(m_editFontSize,"24");

		HDC hdc;
		hdc = GetDC(hWnd);
		
		LOGFONT lf;
		ZeroMemory(&lf,sizeof(lf));
		lf.lfCharSet = SHIFTJIS_CHARSET;
		lf.lfFaceName[0] = 0;
		lf.lfPitchAndFamily = 0;
		
		EnumFontFamiliesEx(hdc,&lf,(FONTENUMPROC)EnumFontFamExProc,(LPARAM)0,0);

		ReleaseDC(hWnd,hdc);

		SetSelectUserFontList();

		RECT rc;
		GetWindowRect(hWnd,&rc);
		int sx,sy;
		sx = rc.right - rc.left;
		sy = rc.bottom - rc.top;

		HWND hwnd0;
		hwnd0 = GetDesktopWindow();

		HDC hdc0;
		hdc0 = GetDC(hwnd0);

		int wx,wy;
		wx = GetDeviceCaps(hdc0,HORZRES);
		wy = GetDeviceCaps(hdc0,VERTRES);
		ReleaseDC(hwnd0,hdc0);


		MoveWindow(hWnd,(wx-sx)/2,(wy-sy)/2,sx,sy,TRUE);

		return 1;
		break;


	case WM_CLOSE:
		EndDialog(hWnd,1);
		break;


	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			//get data
			//fontname,fontsize,ì¬¸“x(2-8),ƒf[ƒ^¸“x(2,4,8),filename

			if (SendMessage(m_radio2Bit,BM_GETCHECK,0,0) == BST_CHECKED) m_mul = 2;
			if (SendMessage(m_radio4Bit,BM_GETCHECK,0,0) == BST_CHECKED) m_mul = 4;
			if (SendMessage(m_radio8Bit,BM_GETCHECK,0,0) == BST_CHECKED) m_mul = 8;

			GetWindowText(m_editFileName,m_filename,254);
			char tmp[16];
			GetWindowText(m_editSeido,tmp,16);
			m_bai = atoi(tmp);

			GetWindowText(m_editFontSize,tmp,16);
			m_realFontSize = atoi(tmp);

			//font–¼
			GetSelectUserFontList();


			EndDialog(hWnd,0);
			break;
		case IDCANCEL:
			EndDialog(hWnd,1);
			break;
		}
		break;
	}

	return FALSE;
}



int CALLBACK EnumFontFamExProc(

  ENUMLOGFONTEX *lpelfe, // pointer to logical-font data

  NEWTEXTMETRICEX *lpntme, // pointer to physical-font data

  int FontType, // type of font

  LPARAM lParam // application-defined data
)
{
	LPSTR name = lpelfe->elfLogFont.lfFaceName;
	int ln = strlen(name);

	if (strlen(name)>=254) return FALSE;

//	memcpy(m_fontName + m_fontKosuu*256,name,ln);
//	m_fontName[m_fontKosuu*256+ln] = 0;
//	m_fontName[m_fontKosuu*256+ln+1] = 0;

	SendMessage(m_fontNameList,CB_INSERTSTRING,(WPARAM)m_fontKosuu,(LPARAM)name);

	m_fontKosuu++;
	if (m_fontKosuu>=255) return FALSE;
	return TRUE;
}




void SetSelectUserFontList(void)
{
	int found = 0;

	for (int i=0;i<m_fontKosuu;i++)
	{
		char name2[256];
		SendMessage(m_fontNameList,CB_GETLBTEXT,(WPARAM)i,(LPARAM)name2);
		if (strcmp(fontname,name2) == 0)
		{
			//found
			found = i;
			break;
		}
	}

	SendMessage(m_fontNameList,CB_SETCURSEL,(WPARAM)found,(LPARAM)0);
}


void GetSelectUserFontList(void)
{
	int sel = SendMessage(m_fontNameList,CB_GETCURSEL,(WPARAM)0,(LPARAM)0);
	if ((sel<0) || (sel>=m_fontKosuu)) sel = 0;

	char name[256];
	name[0] = 0;
	SendMessage(m_fontNameList,CB_GETLBTEXT,(WPARAM)sel,(LPARAM)name);
	int ln = strlen(name);
	if (ln>254) ln = 254;
	if (ln<0) ln = 0;
	memcpy(m_userFontName,name,ln);
	m_userFontName[ln] = 0;
	m_userFontName[ln+1] = 0;
}



/*_*/


