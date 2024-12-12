//
// unpackroutine.cpp
//

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <commctrl.h>

#include "myFileSearch.h"

#include "unpackroutine.h"
//#include "checkfiledate.h"


//char* CUnpackRoutine::m_nameData = NULL;
//int* CUnpackRoutine::m_nameTable = NULL;



char CUnpackRoutine::m_dwqType[][4]=
{
	"bg","ev","ta","sys","sm","sc",
};

char CUnpackRoutine::m_dwqDir[][8]=
{
	"bg","ev","ta","sys","sys\\sm","sys\\sc",
};

char CUnpackRoutine::m_vawDir[][4]=
{
	"se","sys"
};


CUnpackRoutine::CUnpackRoutine()
{
	m_errorSkipFlag = FALSE;

//	m_checkFileDate = new CCheckFileDate();
	m_loadBuffer = new char[1024*8192];
	m_loadBuffer2 = new char[1024*8192];
	m_saveBuffer = new char[1024*8192];

	m_myFileSearch = new CMyFileSearch();
	m_barCount = 0;
	m_vawErrorCount = 0;
	m_dwqErrorCount = 0;
}



CUnpackRoutine::~CUnpackRoutine()
{
	End();
}

void CUnpackRoutine::End(void)
{
	if (m_myFileSearch != NULL)
	{
		m_myFileSearch->End();
		delete m_myFileSearch;
		m_myFileSearch = NULL;
	}

	if (m_loadBuffer != NULL)
	{
		delete [] m_loadBuffer;
		m_loadBuffer = NULL;
	}

	if (m_loadBuffer2 != NULL)
	{
		delete [] m_loadBuffer2;
		m_loadBuffer2 = NULL;
	}

	if (m_saveBuffer != NULL)
	{
		delete [] m_saveBuffer;
		m_saveBuffer = NULL;
	}
}




BOOL CUnpackRoutine::BunkaiToDWQ(int version)
{
	for (int ver=0;ver<=version;ver++)
	{
		for (int i=0;i<6;i++)
		{
//			char dirname[1024];
			char filename[1024];
			wsprintf(filename,"%s%d.gtb",m_dwqType[i],ver);

			if (m_myFileSearch->SearchStart("dwq",filename))
			{
				LPSTR name = NULL;
				while((name = m_myFileSearch->GetName()) != NULL)
				{
					BunkaiToDWQRoutine(name,ver,i);
				}
				m_myFileSearch->SearchEnd();
			}
		}
	}



	return TRUE;
}

BOOL CUnpackRoutine::ConvertToBMP(void)
{
	for (int i=0;i<6;i++)

	{
		char srcDir[1024];
		wsprintf(srcDir,"reverse\\dwq\\%s",m_dwqDir[i]);
		if (m_myFileSearch->SearchStart(srcDir,"*.dwq"))
		{
			char smes[1024];
			wsprintf(smes,"画像変換中:%s",m_dwqDir[i]);
			SetStaticMessage(smes);

			LPSTR name = NULL;
			while((name = m_myFileSearch->GetName()) != NULL)
			{
				char filename[1024];
				int ln = strlen(name);
				if (ln>4)
				{
					memcpy(filename,name,ln-4);
					filename[ln-4] = 0;
					ConvertDWQRoutine(filename,i);
					IncBar();
				}
			}


			m_myFileSearch->SearchEnd();
		}
	}

	return TRUE;
}

BOOL CUnpackRoutine::BunkaiToVAW(void)
{
	if (m_myFileSearch->SearchStart("cdvaw","*.vtb"))
	{
		LPSTR name = NULL;
		while((name = m_myFileSearch->GetName()) != NULL)
		{
			char tag[3];
			tag[0] = *name;
			tag[1] = *(name+1);
			tag[2] = 0;
			BunkaiToVAWRoutine(tag);
		}
		m_myFileSearch->SearchEnd();
	}
	return TRUE;
}

BOOL CUnpackRoutine::ConvertToWAV(void)
{
	//voice
	if (m_myFileSearch->SearchDirStart("reverse\\cdvaw"))
	{
		LPSTR name = NULL;
		while((name = m_myFileSearch->GetDir()) != NULL)
		{
			char srcDir[1024];
			wsprintf(srcDir,"reverse\\cdvaw\\%s",name);

			if (m_myFileSearch->SearchStart(srcDir,"*.vaw"))
			{
				char smes[1024];
				wsprintf(smes,"音声変換中:%s",name);
				SetStaticMessage(smes);

				LPSTR vname = NULL;
				while((vname = m_myFileSearch->GetName()) != NULL)
				{
					char filename[1024];
					int ln = strlen(vname);
					if (ln>4)
					{
						memcpy(filename,vname,ln-4);
						filename[ln-4] = 0;
						ConvertVAWRoutine(filename,srcDir,"reverse\\cdwave");
						IncBar();
					}
				}

				m_myFileSearch->SearchEnd();
			}
		}

		m_myFileSearch->SearchDirEnd();
	}


	//sound

	for (int i=0;i<2;i++)
	{
		char vawdir[1024];
		wsprintf(vawdir,"vaw\\%s",m_vawDir[i]);
		char dstdir[1024];
		wsprintf(dstdir,"reverse\\wav\\%s",m_vawDir[i]);

		char smes[1024];
		wsprintf(smes,"効果音変換中:%s",m_vawDir[i]);
		SetStaticMessage(smes);


		if (m_myFileSearch->SearchStart(vawdir,"*.vaw"))
		{
			LPSTR name = NULL;
			while((name = m_myFileSearch->GetName()) != NULL)
			{
				char filename[1024];
				int ln = strlen(name);
				if (ln>4)
				{
					memcpy(filename,name,ln-4);
					filename[ln-4] = 0;
					ConvertVAWRoutine(filename,vawdir,dstdir);
					IncBar();
				}

			}
			m_myFileSearch->SearchEnd();
		}
	}



	//bgm
	if (m_myFileSearch->SearchStart("wgq","*.wgq"))
	{
		SetStaticMessage("BGM変換中");


		LPSTR name = NULL;
		while((name = m_myFileSearch->GetName()) != NULL)
		{
			char filename[1024];
			int ln = strlen(name);
			if (ln>4)
			{
				memcpy(filename,name,ln-4);
				filename[ln-4] = 0;
				ConvertWGQRoutine(filename);
				IncBar();
			}

		}
		m_myFileSearch->SearchEnd();
	}

	return TRUE;
}


BOOL CUnpackRoutine::BunkaiToDWQRoutine(LPSTR name,int version,int type)
{
	char tableFileName[1024];
	char packFileName[1024];
	wsprintf(tableFileName,"dwq\\%s%d.gtb",m_dwqType[type],version);
	wsprintf(packFileName,"dwq\\%s%d.gpk",m_dwqType[type],version);

	FILE* tableFile = NULL;
	if (fopen_s(&tableFile,tableFileName,"rb") != 0)
	{
		char mes[1024];
		wsprintf(mes,"Filename:%s",tableFileName);
		MessageBox(NULL,mes,"読み込みエラー",MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	FILE* packFile = NULL;
	if (fopen_s(&packFile,packFileName,"rb") != 0)
	{
		fclose(tableFile);
		char mes[1024];
		wsprintf(mes,"Filename:%s",tableFileName);
		MessageBox(NULL,mes,"読み込みエラー",MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	char smes[1024];
	wsprintf(smes,"画像分解中:%s%d",m_dwqType[type],version);

	SetStaticMessage(smes);
	//static mes


	fread(m_loadBuffer,1,1024*8192,tableFile);
	int* tablePtr = (int*)m_loadBuffer;
	int tableKosuu = *tablePtr;
	
	fseek(packFile,0,SEEK_END);
	int packFileSize = (int)(ftell(packFile));
	fseek(packFile,0,SEEK_SET);

	LPSTR namePtr = m_loadBuffer + (tableKosuu*2+1)*sizeof(int);

	for (int i=0;i<tableKosuu;i++)
	{
		IncBar();

		int nameN = *(tablePtr+i+1);

		int packStart = *(tablePtr+1+tableKosuu+i);
		int packEnd = *(tablePtr+1+tableKosuu+i+1);
		if (i>=tableKosuu-1)
		{
			packEnd = packFileSize;
		}
		int dstFileSize = packEnd - packStart;

		fseek(packFile,packStart,SEEK_SET);
		fread(m_loadBuffer2,1,dstFileSize,packFile);

		char dstFileName[1024];

		wsprintf(dstFileName,"reverse\\dwq\\%s\\%s.dwq",m_dwqDir[type],namePtr+nameN);

		FILE* file2 = NULL;
		if (fopen_s(&file2,dstFileName,"wb") == 0)
		{
			fwrite(m_loadBuffer2,1,dstFileSize,file2);
			fclose(file2);
		}
	}

	fclose(packFile);
	fclose(tableFile);

	return TRUE;
}


BOOL CUnpackRoutine::BunkaiToVAWRoutine(LPSTR tag)
{
	char tableFileName[1024];
	char packFileName[1024];
	wsprintf(tableFileName,"cdvaw\\%s.vtb",tag);
	wsprintf(packFileName,"cdvaw\\%s.vpk",tag);

	FILE* tableFile = NULL;
	if (fopen_s(&tableFile,tableFileName,"rb") != 0)
	{
		char mes[1024];
		wsprintf(mes,"Filename:%s",tableFileName);
		MessageBox(NULL,mes,"読み込みエラー",MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	FILE* packFile = NULL;
	if (fopen_s(&packFile,packFileName,"rb") != 0)
	{
		fclose(tableFile);
		char mes[1024];
		wsprintf(mes,"Filename:%s",tableFileName);
		MessageBox(NULL,mes,"読み込みエラー",MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	char smes[1024];
	wsprintf(smes,"音声分解中:%s",tag);

	SetStaticMessage(smes);
	//static mes


	fread(m_loadBuffer,1,1024*8192,tableFile);
	int* tablePtr = (int*)m_loadBuffer;


	fseek(tableFile,0,SEEK_END);
	int tableKosuu = ((int)(ftell(tableFile)) / 12)-1;
	fseek(tableFile,0,SEEK_SET);

	char newDir[1024];
	wsprintf(newDir,"reverse\\cdvaw\\%s",tag);
	CreateDirectory(newDir,NULL);


	for (int i=0;i<tableKosuu;i++)
	{
		IncBar();

		char voicename[9];
		memcpy(voicename,tablePtr+i*3,8);
		voicename[8] = 0;

		int packStart = *(tablePtr+i*3+2);
		int packEnd = *(tablePtr+(i+1)*3+2);
		int dstFileSize = packEnd - packStart;

		fseek(packFile,packStart,SEEK_SET);
		fread(m_loadBuffer2,1,dstFileSize,packFile);

		char dstFileName[1024];

		wsprintf(dstFileName,"reverse\\cdvaw\\%s\\%s.vaw",tag,voicename);

		FILE* file2 = NULL;
		if (fopen_s(&file2,dstFileName,"wb") == 0)
		{
			fwrite(m_loadBuffer2,1,dstFileSize,file2);
			fclose(file2);
		}
	}

	fclose(packFile);
	fclose(tableFile);

	return TRUE;
}



BOOL CUnpackRoutine::ConvertWGQRoutine(LPSTR filenameonly)
{
	char srcFileName[1024];
	char dstFileName[1024];
	wsprintf(srcFileName,"wgq\\%s.wgq",filenameonly);
	wsprintf(dstFileName,"reverse\\bgm\\%s.ogg",filenameonly);

	FILE* srcFile = NULL;
	if (fopen_s(&srcFile,srcFileName,"rb") != 0)
	{
		char mes[1024];
		wsprintf(mes,"Filename:%s",srcFileName);
		MessageBox(NULL,mes,"読み込みエラー",MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	fread(m_loadBuffer,1,64,srcFile);	//からよみ
	FILE* file2 = NULL;
	if (fopen_s(&file2,dstFileName,"wb") == 0)
	{
		while (TRUE)
		{
			int readSize = fread(m_loadBuffer,1,1024*1024*8,srcFile);

			if (readSize <= 0)
			{
				break;
			}

			fwrite(m_loadBuffer,1,readSize,file2);
			if (readSize < 1024*1024*8)
			{
				break;
			}
		}

		fclose(file2);
	}
	fclose(srcFile);

	return TRUE;
}



BOOL CUnpackRoutine::ConvertVAWRoutine(LPSTR filenameonly,LPSTR srcDir,LPSTR dstDir)
{
	char srcFileName[1024];
	char dstFileName[1024];
	wsprintf(srcFileName,"%s\\%s.vaw",srcDir,filenameonly);

	FILE* srcFile = NULL;
	if (fopen_s(&srcFile,srcFileName,"rb") != 0)
	{
		char mes[1024];
		wsprintf(mes,"Filename:%s",srcFileName);
		MessageBox(NULL,mes,"読み込みエラー",MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	fread(m_loadBuffer,1,64,srcFile);	//ヘッダー
	char t = *(m_loadBuffer + 57);

	int type = (int)t;
	type -= '0';

	if (type == 0)
	{
		wsprintf(dstFileName,"%s\\%s.wav",dstDir,filenameonly);
	}
	else if (type == 2)
	{
		fread(m_loadBuffer,1,108-64,srcFile);
		wsprintf(dstFileName,"%s\\%s.ogg",dstDir,filenameonly);
	}
	else
	{
		m_vawErrorCount++;
		fclose(srcFile);
		return FALSE;
	}

	FILE* file2 = NULL;
	if (fopen_s(&file2,dstFileName,"wb") == 0)
	{
		while (TRUE)
		{
			int readSize = fread(m_loadBuffer,1,1024*1024*8,srcFile);

			if (readSize <= 0)
			{
				break;
			}

			fwrite(m_loadBuffer,1,readSize,file2);
			if (readSize < 1024*1024*8)
			{
				break;
			}
		}

		fclose(file2);
	}
	fclose(srcFile);

	return TRUE;
}


BOOL CUnpackRoutine::ConvertDWQRoutine(LPSTR filenameonly,int type)
{
	char srcFileName[1024];
	char dstFileName[1024];
	char maskFileName[1024];
	wsprintf(srcFileName,"reverse\\dwq\\%s\\%s.dwq",m_dwqDir[type],filenameonly);

	FILE* srcFile = NULL;
	if (fopen_s(&srcFile,srcFileName,"rb") != 0)
	{
		char mes[1024];
		wsprintf(mes,"Filename:%s",srcFileName);
		MessageBox(NULL,mes,"読み込みエラー",MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	char header[64];
	fread(header,1,64,srcFile);

	char t = header[57];
	int fileType = (int)t;
	fileType -= '0';


	if ((fileType == 0) || (fileType == 1))	//bmp
	{
		wsprintf(dstFileName,"reverse\\bmp\\%s\\%s.bmp",m_dwqDir[type],filenameonly);
	}
	else if ((fileType == 2) || (fileType == 3))	//bmp(2:nonpack(full,256) 3:pack(256))+mask
	{
		if (type != 3)
		{
			wsprintf(dstFileName,"reverse\\bmp\\%s\\%s.bmp",m_dwqDir[type],filenameonly);
			wsprintf(maskFileName,"reverse\\bmp\\%s\\%sm.bmp",m_dwqDir[type],filenameonly);
		}
		else
		{
			wsprintf(dstFileName,"reverse\\bmp\\%s\\ta\\%s.bmp",m_dwqDir[type],filenameonly);
			wsprintf(maskFileName,"reverse\\bmp\\%s\\ta\\%sm.bmp",m_dwqDir[type],filenameonly);
		}
	}
	else if (fileType == 5)	//jpg
	{
		wsprintf(dstFileName,"reverse\\jpg\\%s\\%s.jpg",m_dwqDir[type],filenameonly);
	}
	else if (fileType == 7)	//jpg+mask
	{
		if (type != 3)
		{
			wsprintf(dstFileName,"reverse\\jpg\\%s\\%s.jpg",m_dwqDir[type],filenameonly);
			wsprintf(maskFileName,"reverse\\jpg\\%s\\%sm.bmp",m_dwqDir[type],filenameonly);
		}
		else
		{
			wsprintf(dstFileName,"reverse\\jpg\\%s\\ta\\%s.jpg",m_dwqDir[type],filenameonly);
			wsprintf(maskFileName,"reverse\\jpg\\%s\\ta\\%sm.bmp",m_dwqDir[type],filenameonly);
		}
	}
	else
	{
		m_dwqErrorCount++;
		fclose(srcFile);
		return FALSE;
	}

//fclose(srcFile);
//return TRUE;

	if ((fileType == 0) || (fileType == 1) || (fileType == 5))
	{
		int readSize = fread(m_loadBuffer,1,1024*1024*8,srcFile);
		int fileSize = readSize;

		if (fileType == 1)
		{
			fileSize = UnPackBmp();
		}

		if ((fileType == 0) || (fileType == 1))
		{
			ReverseBMP();
		}

		FILE* file2 = NULL;
		if (fopen_s(&file2,dstFileName,"wb") == 0)
		{
			fwrite(m_loadBuffer,1,fileSize,file2);
			fclose(file2);
		}
	}
	else
	{
		int readSize = *((int*)(header + 32));
		fread(m_loadBuffer,1,readSize,srcFile);
		int fileSize = readSize;

		if (fileType == 3)
		{
			fileSize = UnPackBmp();
		}

		if ((fileType == 2) || (fileType == 3))
		{
			ReverseBMP();
		}

		FILE* file2 = NULL;
		if (fopen_s(&file2,dstFileName,"wb") == 0)
		{

			fwrite(m_loadBuffer,1,fileSize,file2);
			fclose(file2);
		}

		//mask
		fread(m_loadBuffer,1,1024*1024*8,srcFile);
		fileSize = MakeMask();

		FILE* file3 = NULL;
		if (fopen_s(&file3,maskFileName,"wb") == 0)
		{
			fwrite(m_loadBuffer,1,fileSize,file3);
			fclose(file3);
		}
	}

	fclose(srcFile);
	return TRUE;
}


void CUnpackRoutine::ReverseBMP(void)
{
	BITMAPFILEHEADER* lpBMF = (BITMAPFILEHEADER*)(m_loadBuffer + 0);
	BITMAPINFOHEADER* lpBMI = (BITMAPINFOHEADER*)(m_loadBuffer + sizeof(BITMAPFILEHEADER));

	int sx = lpBMI->biWidth;
	int sy = lpBMI->biHeight;
	if (sy<0) sy *= -1;
	lpBMI->biHeight = -sy;


	char* pic = m_loadBuffer + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	if (lpBMI->biBitCount == 8)
	{
		//change palette??
		int p = lpBMI->biClrUsed;
		if (p == 0) p = 256;



		pic += p*4;
	}


	int sizeX = sx*(lpBMI->biBitCount/8);
	sizeX += 3;
	sizeX &= (~3);


	//color change
	if (lpBMI->biBitCount == 24)
	{
		for (int j=0;j<sy;j++)
		{
			for (int i=0;i<sx;i++)
			{
				char b = *(pic+j*sizeX+i*3+0);
				char g = *(pic+j*sizeX+i*3+1);
				char r = *(pic+j*sizeX+i*3+2);

				*(pic+j*sizeX+i*3+0) = r;
				*(pic+j*sizeX+i*3+1) = g;
				*(pic+j*sizeX+i*3+2) = b;
			}
		}
	}
}



int CUnpackRoutine::UnPackBmp(void)
{
	return MakeMask();

	BITMAPFILEHEADER* lpBMF = (BITMAPFILEHEADER*)(m_loadBuffer + 0);
	BITMAPINFOHEADER* lpBMI = (BITMAPINFOHEADER*)(m_loadBuffer + sizeof(BITMAPFILEHEADER));
	if (lpBMI->biBitCount != 8)
	{
		m_dwqErrorCount++;
	}

	int palKosuu = lpBMI->biClrUsed;
	if (palKosuu == 0) palKosuu = 256;

	int bit = lpBMI->biBitCount;
	int sx = lpBMI->biWidth;
	int sy = lpBMI->biHeight;
	if (sy<0)
	{
		sy *= -1;
	}
	lpBMI->biHeight = -sy;

	//unpack







	int sx2 = (sx+3) & (~3);
	int sz = sx2 * sy + 4*palKosuu;
	sz += sizeof(BITMAPFILEHEADER);
	sz += sizeof(BITMAPINFOHEADER);
	return sz;
}





int CUnpackRoutine::MakeMask(void)
{
	BITMAPFILEHEADER* lpBMF = (BITMAPFILEHEADER*)(m_loadBuffer + 0);
	BITMAPINFOHEADER* lpBMI = (BITMAPINFOHEADER*)(m_loadBuffer + sizeof(BITMAPFILEHEADER));
	if (lpBMI->biBitCount != 8)
	{
		m_dwqErrorCount++;
	}

	int palKosuu = lpBMI->biClrUsed;
	if (palKosuu == 0) palKosuu = 256;

	int bit = lpBMI->biBitCount;
	int sx = lpBMI->biWidth;
	int sy = lpBMI->biHeight;
	if (sy<0)
	{
		sy *= -1;
	}
	lpBMI->biHeight = -sy;

	//unpack

	int ptr = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	ptr += palKosuu * 4;

	int pictureSizeX = sx;
	int pictureSizeY = sy;


	char old[8192];
	char unpack[8192];

	for (int i=0;i<pictureSizeX;i++)
	{
		old[i] = 0;
	}

	for (int j=0;j<pictureSizeY;j++)
	{
		//unpack
		int xPtr = 0;
		while (xPtr < pictureSizeX)
		{
			char c = m_loadBuffer[ptr];
			ptr++;
			if (c != 0)
			{
				unpack[xPtr] = c;
				xPtr++;
			}
			else
			{
				int ln = m_loadBuffer[ptr] & 0xff;
				ptr++;
				for (int ii=0;ii<ln;ii++)
				{
					unpack[xPtr] = 0;
					xPtr++;
				}
			}
		}

		//xor
		for (int i=0;i<pictureSizeX;i++)
		{
			unpack[i] ^= old[i];
			old[i] = unpack[i];
		}

		for (int i=0;i<pictureSizeX;i++)
		{
			int c = ((int)unpack[i]) & 0xff;
			*(m_loadBuffer2 + i + j*pictureSizeX) = (char)c;
//			int r = rgbquad[c].rgbRed;
//			*(m_maskPic + i + j * m_pictureSizeX) = (char)r;
		}
	}

	int sx2 = (sx+3) & (~3);

	int offsets = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	offsets += palKosuu * 4;

	for (int j=0;j<pictureSizeY;j++)
	{
		for (int i=0;i<pictureSizeX;i++)
		{
			*(m_loadBuffer+offsets+i+j*sx2) = *(m_loadBuffer2+i+j*pictureSizeX);
		}
	}


//	int sx2 = (sx+3) & (~3);
	int sz = sx2 * sy + 4*palKosuu;
	sz += sizeof(BITMAPFILEHEADER);
	sz += sizeof(BITMAPINFOHEADER);
	return sz;

}



void CUnpackRoutine::SetProgressWindow(HWND staticMessage,HWND bar1,HWND bar2)
{
	m_staticText = staticMessage;
	m_progressBar = bar1;
	m_progressBar2 = bar2;
}


void CUnpackRoutine::SetStaticMessage(LPSTR mes)
{
	SetWindowText(m_staticText,mes);
	UpdateWindow(m_staticText);
}

void CUnpackRoutine::IncBar(void)
{
	m_barCount++;
//	m_barCount %= 100;
	if ((m_barCount % 10) == 0)
	{
		SendMessage(m_progressBar2,PBM_SETPOS,m_barCount % 100,0);
	}
}


/*_*/
