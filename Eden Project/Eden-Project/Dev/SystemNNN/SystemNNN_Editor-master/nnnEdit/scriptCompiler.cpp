//
// scriptCompiler
//

#include <windows.h>
#include <stdio.h>
#include <commctrl.h>

#include "resource.h"

#include "..\..\systemNNN\nyanlib\include\commonMacro.h"
#include "..\..\systemNNN\nyanlib\include\areaControl.h"
#include "..\..\systemNNN\nyanlib\include\myGraphics.h"
#include "..\..\systemNNN\nyanlib\include\picture.h"
#include "..\..\systemNNN\nyanlib\include\myFile.h"

#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"
#include "..\..\systemNNN\nyanLib\include\effect.h"
#include "..\..\systemNNN\nyanEffectLib\effectlist.h"

#include "..\..\systemNNN\nnnUtilLib\scriptDefine.h"
#include "commanddatatype.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"

#include "case.h"
#include "messageData.h"
#include "myApplicationBase.h"

#include "filmCaseData.h"
#include "filmData.h"
#include "komaData.h"
#include "messageData.h"

#include "storyBookData.h"
#include "storyData.h"
#include "commandData.h"

#include "..\..\systemNNN\nnnutillib\cutinNameData.h"

#include "miniCompiler.h"
#include "termControl.h"

#include "ScriptCompiler.h"




#include "rubiMaker.h"
char CScriptCompiler::m_unknownStoryName[] = "？？？";
char CScriptCompiler::m_unknownErrorMessage[] = "エラー";
char CScriptCompiler::m_unknownErrorCaption[] = "ERROR";


#define LAYER_MAX 32

CScriptCompiler::CScriptCompiler(CMyApplicationBase* app)
{
	m_app = app;

	m_bufferSetCG = TRUE;

	m_varControlSetCGMax = 1;

	m_rubiMaker = m_app->GetRubiMaker();
	m_cutinNameData = m_app->GetCutinNameData();
	m_varControlList = m_app->GetVarControlList();
	int listKosuu = m_varControlList->GetNameKosuu();

	int varKosuu = m_app->GetVarKosuu();

	m_varListNumber = new int[listKosuu/2 + 1];
	for (int i=1;i<listKosuu/2 - 1;i++)
	{
		m_varListNumber[i] = -1;
		LPSTR listName = m_varControlList->GetName(i*2+2+1);
		for (int k=1;k<varKosuu;k++)
		{
			LPSTR varName = m_app->GetVarName(k);
			if (_stricmp(varName,listName) == 0)
			{
				m_varListNumber[i] = k;
				break;
			}
		}
	}


	m_cgVoiceData = new char[1024];
	m_cgListDataOnly = new char[1024];
	m_cgVoiceData[0] = 0;
	m_cgListDataOnly[0] = 0;

	m_targetMachine = 0;

	m_sceneWork = NULL;
	m_charaOffsetTable = NULL;
	m_cgDataLoadFlag = FALSE;
	m_makeCgListFlag = FALSE;
	m_cgDataListKosuu = 0;
	int i;
	for (i=0;i<100;i++)
	{
		m_cgDataList[i] = NULL;
	}
	m_setcgListKosuu = 0;

	int bt = 1;
	for (i=0;i<32;i++)
	{
		m_bitTable[i] = bt;
		bt <<= 1;
	}
	m_bitTable[31] = 0x80000000;

	m_frameHWND = m_app->GetFrameHWND();	//dummy

	m_projectList = m_app->GetProjectList();
	m_commandList = m_app->GetCommandList();
	m_functionList = m_app->GetFunctionList();
	m_varList = m_app->GetNameList();

	m_sceneDataList = new CNameList();
	m_sceneDataLoadFlag = FALSE;

	m_miniCompiler = new CMiniCompiler();

	m_miniCompiler->SetCommandList(m_commandList);
	m_miniCompiler->SetFunctionList(m_functionList);
	m_miniCompiler->SetVarList(m_varList);

	m_voiceFlagWorkMax = 65536;
	m_voiceFlagFileName = new char[m_voiceFlagWorkMax * (8+8)];

	m_termControl = new CTermControl();
}

CScriptCompiler::~CScriptCompiler()
{
	End();
}

void CScriptCompiler::End(void)
{
	ENDDELETECLASS(m_termControl);


	DELETEARRAY(m_voiceFlagFileName);

	ENDDELETECLASS(m_miniCompiler);
	ENDDELETECLASS(m_sceneDataList);

	for (int i=0;i<100;i++)
	{
		ENDDELETECLASS(m_cgDataList[i]);
	}

	DELETEARRAY(m_cgListDataOnly);
	DELETEARRAY(m_cgVoiceData);
	DELETEARRAY(m_varListNumber);
}


BOOL CScriptCompiler::StartBuild(void)
{
	m_targetMachine = m_app->GetConfig("targetMachine");

	m_varControlConfig = m_app->GetConfig("varControlSetCG");

	m_varControlSetCGMax = m_varControlConfig;

	m_termConfig = m_app->GetConfig("termFlag");
	if (m_termConfig != 0)
	{
		CNameList* termList = m_app->GetTermList();
		m_termControl->SetTermList(termList);
	}

	if (m_varControlConfig == -1)
	{
		//calcu max

		m_varControlSetCGMax = 1;

		for (int j=0;j<m_cgDataListKosuu;j++)
		{
			int kosuu = m_cgDataList[j]->GetNameKosuu();
			for (int i=0;i<kosuu;i++)
			{
				LPSTR name = m_cgDataList[j]->GetName(i);
				int ln = strlen(name);

				if (strlen(name) >= 3)
				{
					BOOL suuji = FALSE;
					int n = ln-1;

					int suujiTop = -1;

					while (n>=0)
					{
						char c = *(name+n);
						if ((c >= '0') && (c<='9'))
						{
							suuji = TRUE;
							suujiTop = n;
							n--;
						}
						else
						{
							break;
						}
					}

					if (suuji)
					{
						if (n>=0)
						{
							if ((*name+n) == '_')
							{
								n--;
								if (n>=0)
								{
									if ((*name+n) == '_')
									{
										int d = atoi(name+suujiTop);
										if (d > m_varControlSetCGMax)
										{
											m_varControlSetCGMax = d;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}


	m_miniCompiler->StartBuild(m_targetMachine);
	return TRUE;
}

BOOL CScriptCompiler::CompileStory(void)
{
	//CStoryBookDoc* pDoc = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
	//CStoryBookData* pBook = pDoc->GetStoryBookData();
	CStoryBookData* pBook = m_app->GetStoryBookData();

	int kosuu = pBook->GetObjectKosuu();
	
	for (int j=0;j<kosuu;j++)
	{
		m_compilingStoryNumber = j;

		//set jump table
		int pc = m_miniCompiler->GetMakedSize();
		m_storyJumpTable[j] = pc;

		CStoryData* pStory = (CStoryData*)(pBook->GetObjectData(j));

		//set film name
		m_miniCompiler->Pass2SystemFunctionMessage(CODE_SYSTEMFUNCTION_STORYNAME,pStory->GetMyName());
		m_miniCompiler->AddNowCode();

		if (CompileStory2(pStory,0) == -1)
		{
			//compile error

			pBook->SetSelectNumber(j);
			pStory->SetSelectNumber(GetPass2ErrorLine());

			MessageBox(NULL,"pass2","ERROR",MB_OK);
			return -1;
		}

		//systemscript-top	end
		//etcscript-top next
		//etc ret

		if (j==0)	//簡易チェック
		{
			if (strcmp(m_app->GetFileNameOnly(),"sys") == 0)
			{
				m_miniCompiler->Pass2Simple(COMMANDDATATYPE_END);
			}
			else
			{
				m_miniCompiler->Pass2Simple(COMMANDDATATYPE_NEXT);
			}
		}
		else
		{
			m_miniCompiler->Pass2Simple(COMMANDDATATYPE_RET);
		}
		m_miniCompiler->AddNowCode();
	}

	return TRUE;
}



int CScriptCompiler::CompileStory2(CStoryData* pStory,int n)
{
	int kosuu = pStory->GetObjectKosuu();
	if (kosuu == 0) return 1;

	CCommandData* pCommandStart = pStory->GetCommand(n);
	int compileLevel = pCommandStart->GetLevel();

	BOOL chukakkoFlag = FALSE;
	int chukakkoStart = 0;
//	BOOL chukakkoFlag2 = FALSE;
//	int chukakkoSTart2 = 0;

	BOOL whileFlag = FALSE;
	int whileStart = 0;
	BOOL selectFlag = FALSE;

	int* objectPointerTop = m_miniCompiler->GetMakedPointer();
	int subKosuu = 0;

	BOOL shoriFlag = FALSE;

	LPSTR storyName = pStory->GetMyName();


	int chukakkoStart2 = 0;


	while (n<=kosuu)
	{
		CCommandData* pCommand = NULL;
		int level = -1;

		if (n<kosuu)
		{
			pCommand = pStory->GetCommand(n);
			level = pCommand->GetLevel();
		}

		if ((n>=kosuu) || (level<=compileLevel))
		{
			if (chukakkoFlag)
			{
				//ちゅうかっことじ処理

				if (whileFlag)
				{
					m_miniCompiler->Pass2Goto(whileStart);
					m_miniCompiler->AddNowCode();

					whileFlag = FALSE;
				}

				//	}
				m_miniCompiler->Pass2CloseChukakko();
				m_miniCompiler->AddNowCode();

				//サイズ変更
				int chukakkoEnd = m_miniCompiler->GetMakedSize();

				*(objectPointerTop+chukakkoStart+3) = chukakkoEnd - chukakkoStart;

				chukakkoFlag = FALSE;


				//選択肢の個数確認などをここにいれる?
			}

			selectFlag = FALSE;
		}

		if (level < compileLevel) break;


		if (shoriFlag)
		{
			if (level == compileLevel)
			{
				if (compileLevel > 0) break;
			}
		}

		if (level > compileLevel)
		{
			BOOL selectJogai = FALSE;
			BOOL selectJogai2 = FALSE;

			if (selectFlag)
			{
				int type = pCommand->GetCommandType();

				if (type == COMMANDDATATYPE_ELSIF) selectJogai = TRUE;
				if (type == COMMANDDATATYPE_ELSE) selectJogai = TRUE;
				if (type == COMMANDDATATYPE_ENDIF) selectJogai = TRUE;

				if (type == COMMANDDATATYPE_IF) selectJogai2 = TRUE;
				if (type == COMMANDDATATYPE_ELSIF) selectJogai2 = TRUE;
				if (type == COMMANDDATATYPE_ELSE) selectJogai2 = TRUE;

			}


			if (selectJogai == FALSE)
			{
				chukakkoStart2 = 0;

				if (selectFlag)
				{
					m_miniCompiler->Pass2Case(subKosuu+1);
					m_miniCompiler->AddNowCode();

					chukakkoStart2 = m_miniCompiler->GetMakedSize();
					m_miniCompiler->Pass2OpenChukakko();
					m_miniCompiler->AddNowCode();
				}
			}


			n = CompileStory2(pStory,n);


			if (selectFlag)
			{
				if (selectJogai2 == FALSE)
				{

					//	}
					m_miniCompiler->Pass2CloseChukakko();
					m_miniCompiler->AddNowCode();

					//サイズ変更
					int chukakkoEnd2 = m_miniCompiler->GetMakedSize();
					*(objectPointerTop+chukakkoStart2+3) = chukakkoEnd2 - chukakkoStart2;
				}
			}

			if (selectJogai == FALSE)
			{
				subKosuu++;
			}

			if (n == -1) return -1;	//compile error
		}
		else
		{
			int type = pCommand->GetCommandType();
			whileFlag = FALSE;
			selectFlag = FALSE;

			subKosuu = 0;
			if (type == COMMANDDATATYPE_WHILE)
			{
				whileStart = m_miniCompiler->GetMakedSize();
				chukakkoFlag = TRUE;
				whileFlag = TRUE;
			}

			//compile 1 command

			if (CompileCommandData(pCommand,storyName,n) == FALSE)
			{
				SetPass2ErrorLine(n);
				return -1;
			}
			m_miniCompiler->AddNowCode();



			if (type == COMMANDDATATYPE_SELECT)
			{
				selectFlag = TRUE;
			}

			if (type == COMMANDDATATYPE_IF) chukakkoFlag = TRUE;
			if (type == COMMANDDATATYPE_ELSIF) chukakkoFlag = TRUE;
			if (type == COMMANDDATATYPE_ELSE) chukakkoFlag = TRUE;

			if (chukakkoFlag)
			{
				chukakkoStart = m_miniCompiler->GetMakedSize();

				//{
				m_miniCompiler->Pass2OpenChukakko();
				m_miniCompiler->AddNowCode();
			}

			shoriFlag = TRUE;

			n++;
		}
	}

	return n;
}



BOOL CScriptCompiler::CompileCommandData(CCommandData* pCommand,LPSTR storyName,int nCommand)
{
	m_miniCompiler->Init();

	int typ = pCommand->GetCommandType();
	if ((typ == COMMANDDATATYPE_IF) || (typ == COMMANDDATATYPE_ELSIF) || (typ == COMMANDDATATYPE_WHILE))
	{
		LPSTR mes = pCommand->GetTextBuffer();
		char buf[16384];
		wsprintf(buf,"(%s)",mes);
		m_miniCompiler->SetCompileSource(buf);
		if (m_miniCompiler->Pass1() == FALSE)
		{
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS1 ERROR");
//			MessageBox(NULL,pCommand->GetTextBuffer(),"PASS1 ERROR",MB_OK);
			return FALSE;
		}

		if (typ == COMMANDDATATYPE_IF)
		{
			if (m_miniCompiler->Pass2If(CODE_CONTROL_IF) == FALSE)
			{
	//			MessageBox(NULL,"PASS2 ERROR [if]","ERROR",MB_OK);
				PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [IF]");
				return FALSE;
			}
		}
		else if (typ == COMMANDDATATYPE_ELSIF)
		{
			if (m_miniCompiler->Pass2If(CODE_CONTROL_ELSIF) == FALSE)
			{
				PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [ELSIF]");
//				MessageBox(NULL,"PASS2 ERROR [elsif]","ERROR",MB_OK);
				return FALSE;
			}
		}
		else
		{
			if (m_miniCompiler->Pass2If(CODE_CONTROL_WHILE) == FALSE)
			{
				PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [WHILE]");
//				MessageBox(NULL,"PASS2 ERROR [while]","ERROR",MB_OK);
				return FALSE;
			}
		}
	}

	if ((typ == COMMANDDATATYPE_SCRIPT) || (typ == COMMANDDATATYPE_JUMP) || (typ == COMMANDDATATYPE_SUBSCRIPT))
	{
		LPSTR scriptName = pCommand->GetTextBuffer();
		if (_stricmp(scriptName,"sys") == 0)
		{
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [script] cannot call sys");

//			MessageBox(NULL,"PASS2 ERROR script:cannot call sys","ERROR",MB_OK);
			return FALSE;
		}

		if (m_projectList->SearchName(scriptName) == -1)
		{
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [script] not found project");
//			MessageBox(NULL,"PASS2 ERROR script:not found project","ERROR",MB_OK);
			return FALSE;
		}
	}

	if (typ == COMMANDDATATYPE_SCRIPT)
	{
		if (m_miniCompiler->Pass2Script(pCommand->GetTextBuffer(),m_app->GetFileNameOnly()) == FALSE)
		{
//			MessageBox(NULL,"PASS2 ERROR script","ERROR",MB_OK);
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [script]");

			return FALSE;
		}
	}

	if (typ == COMMANDDATATYPE_JUMP)
	{
		if (m_miniCompiler->Pass2ScriptJump(pCommand->GetTextBuffer(),m_app->GetFileNameOnly()) == FALSE)
		{
//			MessageBox(NULL,"PASS2 ERROR script jump","ERROR",MB_OK);
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [script jump]");

			return FALSE;
		}
	}

	if (typ == COMMANDDATATYPE_SUBSCRIPT)
	{
		if (m_miniCompiler->Pass2SubScript(pCommand->GetTextBuffer(),m_app->GetFileNameOnly()) == FALSE)
		{
//			MessageBox(m_frameHWND,"PASS2 ERROR sub script","ERROR",MB_OK);
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [sub script]");
			return FALSE;
		}
	}

	if (typ == COMMANDDATATYPE_SYSTEM)
	{
		LPSTR mes = pCommand->GetTextBuffer();
		char buf[16384];
		wsprintf(buf,"%s;",mes);
		m_miniCompiler->SetCompileSource(buf);
		if (m_miniCompiler->Pass1() == FALSE)
		{
//			MessageBox(m_frameHWND,pCommand->GetTextBuffer(),"PASS1 ERROR",MB_OK);
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS1 ERROR [system]");

//			MessageBox(m_frameHWND,"PASS1 ERROR","ERROR",MB_OK);
			return FALSE;
		}

		if (m_miniCompiler->Pass2Func(pCommand->GetSystemCommandNumber(),pCommand->GetSystemCommandType()) == FALSE)
		{
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [system]");
//			MessageBox(m_frameHWND,"PASS2 ERROR func","ERROR",MB_OK);
			return FALSE;
		}
	}

	if (typ == COMMANDDATATYPE_CALCU)
	{
		LPSTR mes = pCommand->GetTextBuffer();
		char buf[16384];
		wsprintf(buf,"%s;",mes);
		m_miniCompiler->SetCompileSource(buf);
		if (m_miniCompiler->Pass1() == FALSE)
		{
//			MessageBox(m_frameHWND,pCommand->GetTextBuffer(),"PASS1 ERROR",MB_OK);
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS1 ERROR [calcu]");

//			MessageBox(m_frameHWND,"PASS1 ERROR","ERROR",MB_OK);
			return FALSE;
		}

		if (m_miniCompiler->Pass2Calcu() == FALSE)
		{
//			MessageBox(m_frameHWND,"PASS2 ERROR calcu","ERROR",MB_OK);
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [calcu]");
			return FALSE;
		}
	}

//#define COMMANDDATATYPE_NOP 0

//#define COMMANDDATATYPE_IF 1
//#define COMMANDDATATYPE_ELSIF 2
//#define COMMANDDATATYPE_ELSE 3

//#define COMMANDDATATYPE_SELECT 4

//#define COMMANDDATATYPE_STORY 5
//#define COMMANDDATATYPE_FILM 6

//#define COMMANDDATATYPE_BGM_MIDI 7
//#define COMMANDDATATYPE_BGM_CD 8

//#define COMMANDDATATYPE_SYSTEM 9

//#define COMMANDDATATYPE_CALCU 10

//#define COMMANDDATATYPE_RET 11
//#define COMMANDDATATYPE_NEXT 12
//#define COMMANDDATATYPE_END 13

//#define COMMANDDATATYPE_SCRIPT 14


	if ((typ == COMMANDDATATYPE_NOP) || (typ == COMMANDDATATYPE_RET) || (typ == COMMANDDATATYPE_NEXT) || (typ == COMMANDDATATYPE_END) || (typ == COMMANDDATATYPE_EXIT) || (typ == COMMANDDATATYPE_ELSE) || (typ == COMMANDDATATYPE_ENDIF))
	{
		if (m_miniCompiler->Pass2Simple(typ) == FALSE)
		{
//			MessageBox(m_frameHWND,"PASS2 ERROR nop ret next ...","ERROR",MB_OK);
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [nop,ret,next ..]");

			return FALSE;
		}
//		m_miniCompiler->AddNowCode();	//これはいらにゃい
	}



	if (typ == COMMANDDATATYPE_SELECT)
	{
		//個数を確認ちがってたらエラー


		LPSTR mes = pCommand->GetTextBuffer();
		int serial = pCommand->GetSerial();
//int mesHaveFlag = 0;
		int selectSerial = pCommand->GetSelectMessageSerial();
		if (m_miniCompiler->Pass2Select(typ,serial,mes,selectSerial) == FALSE)
		{
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [select]");
//			MessageBox(m_frameHWND,"PASS2 ERROR select ...","ERROR",MB_OK);
			return FALSE;
		}
	}

	if (typ == COMMANDDATATYPE_STORY)
	{
		LPSTR mes = pCommand->GetTextBuffer();
		int n = SearchStory(mes);
		if (n == -1)
		{
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [story] no story");
//			MessageBox(m_frameHWND,"PASS2 ERROR no Story","ERROR",MB_OK);
			return FALSE;
		}

		if (n == 0)
		{
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [story] トップストーリーを呼び出しています。\n再帰処理の知識なしで行っている場合には、これは無限ループになります","警告");
//			MessageBox(m_frameHWND,"トップストーリーを呼び出しています。\n再帰処理の知識なしで行っている場合には、これは無限ループになります","警告",MB_ICONEXCLAMATION | MB_OK);
		}

		if (n == m_compilingStoryNumber)
		{
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [story] 同一ストーリーを呼び出しています。\n再帰処理の知識なしで行っている場合には、これは無限ループになります","警告");
//			MessageBox(m_frameHWND,"同一ストーリーを呼び出しています。\n再帰処理の知識なしで行っている場合には、これは無限ループになります",mes,MB_ICONEXCLAMATION | MB_OK);
		}



		if (m_miniCompiler->Pass2Sub(n) == FALSE)
		{
//			MessageBox(m_frameHWND,"PASS2 ERROR stoty","ERROR",MB_OK);
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [story]");

			return FALSE;
		}
	}

	if (typ == COMMANDDATATYPE_DEBUG)
	{
		LPSTR mes = pCommand->GetTextBuffer();

		if (m_miniCompiler->Pass2Debug(mes) == FALSE)
		{
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [debug]");
//			MessageBox(m_frameHWND,"PASS2 ERROR debug","ERROR",MB_OK);
			return FALSE;
		}
	}



	if (typ == COMMANDDATATYPE_FILM)
	{
		LPSTR mes = pCommand->GetTextBuffer();
		int n = SearchFilm(mes);
		if (n == -1)
		{
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [film] no film");
//			MessageBox(m_frameHWND,"PASS2 ERROR no FILM","ERROR",MB_OK);
			return FALSE;
		}

	//	CStoryBookDoc* pDoc = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
	//	int kosuu = pDoc->GetStoryKosuu();
		CStoryBookData* pBook = m_app->GetStoryBookData();
		int kosuu = pBook->GetObjectKosuu();

		if (m_miniCompiler->Pass2Sub(kosuu+n) == FALSE)
		{
//			MessageBox(m_frameHWND,"PASS2 ERROR film","ERROR",MB_OK);
			PrintCompileCommandError(pCommand,storyName,nCommand,"PASS2 ERROR [film] unknown");
			return FALSE;
		}
	}
/*@@@@@@@@@@@@@@@@@
	if (typ == COMMANDDATATYPE_BGM)
	{
		int bgmnum = pCommand->GetBGMNumber();
		int cmd = pCommand->GetCommandType();

		int para[3];
		para[0] = 0;	//midi
		int c = pCommand->GetPara();
		para[1] = c;
		para[2] = bgmnum;

//ID_MENU_COMMAND_MIDI_START

		if (m_miniCompiler->Pass2BGM(para) == FALSE)
		{
			MessageBox(m_frameHWND,"PASS2 ERROR bgm","ERROR",MB_OK);
			return FALSE;
		}

//		LPSTR mes = pCommand->GetTextBuffer();
//		m_miniCompiler->SetCompileSource(mes);

//		if (m_miniCompiler->Pass2BgmMidi() == FALSE)
//		{
//			MessageBox(m_frameHWND,"PASS2 ERROR","ERROR",MB_OK);
//			return FALSE;
//		}
	}
*/


	return TRUE;

}



int CScriptCompiler::SearchFilm(LPSTR filmName, int searchStart,int noCheckNumber)
{
//	CFilmCaseDoc* pDoc = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
//	if (pDoc == NULL) return -1;

//	return pDoc->SearchFilm(filmName, searchStart,noCheckNumber);
	CFilmCaseData* pCase = m_app->GetFilmCaseData();
	return pCase->SearchFilm(filmName,searchStart,noCheckNumber);
}

int CScriptCompiler::SearchStory(LPSTR storyName,int searchStart,int noCheckNumber)
{
	CStoryBookData* pBook = m_app->GetStoryBookData();
	return pBook->SearchStory(storyName,searchStart,noCheckNumber);

//	CStoryBookDoc* pDoc = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
//	if (pDoc == NULL) return -1;
//
//	return pDoc->SearchStory(storyName,searchStart,noCheckNumber);
}


int CScriptCompiler::SearchScript(LPSTR sptName, int searchStart,int noCheckNumber)
{
	noCheckNumber = -1;
	searchStart = 0;

	return m_projectList->GetNameNumber(sptName);
}




BOOL CScriptCompiler::CompileFilm(FILE* makeJsonFlag)
{
	CFilmCaseData* pCase = m_app->GetFilmCaseData();
	int kosuu = pCase->GetObjectKosuu();
	int komaOffsetPointer = 0;

	int defaultVoiceFlag = m_app->GetConfig("addDefaultVoice");
	int varControl[LAYER_MAX];

	int layerMax = CKomaData::m_layerMax;

	int tab = 2;
	int tabHeader = tab + 2;
	int komaTab = tab + 2;
	int messageTab = tab + 4;
	int effectTab = tab + 4;

	char json[1024];

	int varType = m_app->GetVarType();
	int varKosuu = m_app->GetVarKosuu();

	for (int k=0;k<kosuu;k++)
	{
		CFilmData* pFilm = (CFilmData*)(pCase->GetObjectData(k));

		if (makeJsonFlag != NULL)
		{
			pFilm->OutputData(makeJsonFlag,"{",tab);
			wsprintf(json,"\"FilmName\" : \"%s\",",pFilm->GetMyName());
			pFilm->OutputData(makeJsonFlag,json,tab+1);

			pFilm->OutputData(makeJsonFlag,"\"FilmHeader\" :",tab+1);
			pFilm->OutputData(makeJsonFlag,"[",tab+1);
		}


		//set jump table
		int pc = m_miniCompiler->GetMakedSize();
		m_filmJumpTable[k] = pc;


		for (int i=0;i<LAYER_MAX;i++)
		{
			varControl[i] = 0;
		}


		//skiptoendenable??
		int skiptofilmendok = pFilm->GetSkipToFilmEndEnable();
		if (skiptofilmendok != 0)
		{
			//add code
			m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_FILMENDENABLE,1,&skiptofilmendok,makeJsonFlag,tabHeader);
			m_miniCompiler->AddNowCode();
		}



		//set film name
		m_miniCompiler->Pass2SystemFunctionMessage(CODE_SYSTEMFUNCTION_FILMNAME,pFilm->GetMyName());
		m_miniCompiler->AddNowCode();


		int typeTime = pFilm->GetFilmSpecialTypeTime();
		m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_FILMTYPETIME,1,&typeTime,makeJsonFlag,tabHeader);
		m_miniCompiler->AddNowCode();

		int configMask = pFilm->GetConfigMask();
		if (configMask != 0)
		{
			m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_CONFIGMASK,1,&configMask,makeJsonFlag,tabHeader);
			m_miniCompiler->AddNowCode();
		}


		BOOL cleareffectflag = pFilm->GetNoClearEffect();
		if (cleareffectflag)
		{
			m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_CLEARALLEFFECT,0,NULL,makeJsonFlag,tabHeader);
			m_miniCompiler->AddNowCode();
		}
		else
		{
//			if (aaa == 0)
//			{
//				aaa = 1;
//				char mes0[1024];
//				sprintf(mes0,"%d番目のフイルム",k);
//				MessageBox(NULL,mes0,"!",MB_OK);
//			}
//
		}

		int renameLayer = pFilm->GetRenameLayer();
		if (renameLayer != 0)
		{
			m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_RENAMELAYER,1,&renameLayer,makeJsonFlag,tabHeader);
			m_miniCompiler->AddNowCode();
		}

		int cutin = pFilm->GetCutinFlag();
		if (cutin != 0)
		{
			int cutinFlag = 1;
			m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_CUTIN,1,&cutinFlag,makeJsonFlag,tabHeader);
			m_miniCompiler->AddNowCode();
		}

		if (makeJsonFlag != NULL)
		{
			pFilm->OutputData(makeJsonFlag,"{ \"type\" : \"_\" }",tab+2);
			pFilm->OutputData(makeJsonFlag,"],",tab+1);

			pFilm->OutputData(makeJsonFlag,"\"KomaList\" :",tab+1);
			pFilm->OutputData(makeJsonFlag,"[",tab+1);
		}

		int kosuu2k = pFilm->GetObjectKosuu();


		m_komaOffsetByFilm[k] = komaOffsetPointer;

		BOOL voiceOffed[4];
		voiceOffed[0] = TRUE;
		voiceOffed[1] = TRUE;
		voiceOffed[2] = FALSE;
		voiceOffed[3] = FALSE;

		int lastAutoMessage = 0;
		int lastCannotClick = 0;
		int lastCannotSkip = 0;
		int lastFontType = 0;


		int lastOptionOff = 0;

		int lastMustFace = 0;

		int getNowPicList = 0;
		for (int j=0;j<kosuu2k;j++)
		{
			CKomaData* pKoma = pFilm->GetKoma(j);

			int overrapCount = pKoma->GetOverrapCount();
			int overrapType = pKoma->GetOverrapType();

//ここにコマスタートマーカー追加	@@@@@@@@@@@@@@@@@@@@@@@@@ いらないか

			int pc00 = m_miniCompiler->GetMakedSize();
			m_komaJumpTable[komaOffsetPointer] = pc00;
			komaOffsetPointer++;

			if (makeJsonFlag != NULL)
			{
				pKoma->OutputData(makeJsonFlag,"{",komaTab);

				pFilm->OutputData(makeJsonFlag,"\"KomaHeader\" :",komaTab+1);
				pFilm->OutputData(makeJsonFlag,"[",komaTab+1);
			}


			if (pKoma->CheckVarControl())
			{
				for (int vc=0;vc<layerMax;vc++)
				{
					int setVar = pKoma->GetVarControl(vc);
					if (setVar != 0)
					{

						int varListNumber;
						int varData;

						if (varType == 0)
						{
							varListNumber = setVar & 0x3ff;
							varData = (setVar >> 10);
						}
						else
						{
							varListNumber = setVar & 0xfff;
							varData = (setVar >> 12);
						}

						int varNumber = m_varListNumber[varListNumber];

						if ((varNumber > 0) && (varNumber < varKosuu))
						{
							int varpara[2];
							varpara[0] = varNumber;
							varpara[1] = varData;
							m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_SETVAR,2,varpara,makeJsonFlag,komaTab+2);
							m_miniCompiler->AddNowCode();
						}
					}
				}
			}



			int demo = pKoma->GetDemoFlag();
			if (demo != 0)
			{
				int demopara[1];
				demopara[0] = demo;
				m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_SETDEMOFLAG,1,demopara,makeJsonFlag,komaTab+2);
				m_miniCompiler->AddNowCode();
			}

			if (pKoma->GetWindowOffFlag())
			{
				m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_NOMESSAGEWINDOW,0,NULL,makeJsonFlag,komaTab+2);
				m_miniCompiler->AddNowCode();
			}


			//cutin koma control@@

			int cutinK = pKoma->GetCutinFlag();
			if (cutinK != 0)
			{
				if (cutinK == 1)
				{
					if (cutin == 0)
					{
						cutin = 1;
						int cutinFlag = 1;
						m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_CUTIN,1,&cutinFlag,makeJsonFlag,komaTab+2);
						m_miniCompiler->AddNowCode();
					}
				}
				else if (cutinK == -1)
				{
					if (cutin != 0)
					{
						cutin = 0;
						int cutinFlag = 0;
						m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_CUTIN,1,&cutinFlag,makeJsonFlag,komaTab+2);
						m_miniCompiler->AddNowCode();
					}
				}
			}






			//overrap準備
			if (overrapCount != 0)
			{
				//prepareoverrapstart
				m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_PREPAREOVERRAP,0,NULL,makeJsonFlag,komaTab+2);
				m_miniCompiler->AddNowCode();
			}




			int kosuu3 = pKoma->GetObjectKosuu();

			//ここにスタートコマをいれるべき
			int nextType = 3;	//dummy(Draw)
			int nextWindowMode = 0;

			if (kosuu3 > 0)
			{
				CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(0));
				nextType = pMessage->GetMessageMode();
				nextWindowMode = pKoma->GetWindowOffFlag();
				if (nextType == 3)
				{
					if (kosuu3 > 1)
					{
						CMessageData* pMessageNext = (CMessageData*)(pKoma->GetObjectData(1));
						nextType = pMessageNext->GetMessageMode();
					}
				}
			}
			else
			{
				//error!
			}

			SetStartKoma(pKoma,nextType,nextWindowMode,makeJsonFlag,komaTab+2);


			//setcg
			int setCGPlayer = pKoma->GetSetCGPlayer();
			int setCGNumber = pKoma->GetSetCGNumber();

			if (setCGPlayer != 0)
			{
				int setcgpara[2];
				setcgpara[0] = setCGPlayer;
				setcgpara[1] = setCGNumber;

				m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_SETCG,2,setcgpara,makeJsonFlag,komaTab+2);
				m_miniCompiler->AddNowCode();
			}



			//bgm
			int bgm = pKoma->GetBGMNumber();
			if (bgm > 0)
			{
				int bgmPara[5];
				bgmPara[0] = bgm;
				int loop = pKoma->GetExMusicParam(0);
				int vol = pKoma->GetExMusicParam(1);
				int fadeIn = pKoma->GetExMusicParam(2);
				int fadeOut = pKoma->GetExMusicParam(3);

				int paraKosuu = 1;
				bgmPara[1] = loop;
				bgmPara[2] = vol;
				bgmPara[3] = fadeIn;
				bgmPara[4] = fadeOut;

				if (loop != -1) paraKosuu = 2;
				if (vol != 0) paraKosuu = 3;
				if (fadeIn != -1) paraKosuu = 4;
				if (fadeOut != -1) paraKosuu = 5;

				m_miniCompiler->Pass2Music(paraKosuu,bgmPara,NULL,makeJsonFlag,komaTab+2);
				m_miniCompiler->AddNowCode();
			}
			else if (bgm == -1)
			{
				int bgmPara[2];

				int fadeOut = pKoma->GetExMusicParam(3);
				bgmPara[0] = -1;
				int paraKosuu = 1;
				if (fadeOut != -1)
				{
					bgmPara[1] = fadeOut;
					paraKosuu = 2;
				}

				m_miniCompiler->Pass2Music(paraKosuu,bgmPara,NULL,makeJsonFlag,komaTab+2);
				m_miniCompiler->AddNowCode();
			}
			else if (bgm == -2)
			{
				int bgmPara[2];
				int volume = pKoma->GetExMusicParam(4);
				int paraKosuu = 2;
				bgmPara[0] = -2;
				bgmPara[1] = volume;

				m_miniCompiler->Pass2Music(paraKosuu,bgmPara,NULL,makeJsonFlag,komaTab+2);
				m_miniCompiler->AddNowCode();
			}


			//frame control	2004-02-17
			int frameControl = pKoma->GetFrameTime();
			if (frameControl>0)
			{
				m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_FRAMECONTROL,1,&frameControl,makeJsonFlag,komaTab+2);
				m_miniCompiler->AddNowCode();
			}
			else if (frameControl == -1)
			{
				m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_SETDEFAULTFRAME,0,&frameControl,makeJsonFlag,komaTab+2);
				m_miniCompiler->AddNowCode();

			}

			int windowNumber = pKoma->GetWindowNumber();
			if (windowNumber > 0)
			{
				windowNumber--;

				m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_WINDOWNUMBER,1,&windowNumber,makeJsonFlag,komaTab+2);
				m_miniCompiler->AddNowCode();
			}

			int cursorNumber = pKoma->GetCursorNumber();
			if (cursorNumber > 0)
			{
				m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_CURSORNUMBER,1,&cursorNumber,makeJsonFlag,komaTab+2);
				m_miniCompiler->AddNowCode();
			}

			int mouseNumber = pKoma->GetMouseNumber();
			if (mouseNumber > 0)
			{
				m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_MOUSENUMBER,1,&mouseNumber,makeJsonFlag,komaTab+2);
				m_miniCompiler->AddNowCode();
			}

			int autoMessage = pKoma->GetAutoMessage();
			if (autoMessage != 0)
			{
				m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_AUTOMESSAGE,1,&autoMessage,makeJsonFlag,komaTab+2);
				m_miniCompiler->AddNowCode();
				lastAutoMessage = autoMessage;
			}

			int cannotClick = pKoma->GetCannotClick();
			if (cannotClick != 0)
			{
				m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_CANNOTCLICK,1,&cannotClick,makeJsonFlag,komaTab+2);
				m_miniCompiler->AddNowCode();
				lastCannotClick = cannotClick;
			}

			int cannotSkip = pKoma->GetCannotSkip();
			if (cannotSkip != 0)
			{
				m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_CANNOTSKIP,1,&cannotSkip,makeJsonFlag,komaTab+2);
				m_miniCompiler->AddNowCode();
				lastCannotSkip = cannotSkip;
			}


			int optionOff = pKoma->GetOptionOff();
			if (optionOff != 0)
			{
				m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_OPTIONOFF,1,&optionOff,makeJsonFlag,komaTab+2);
				m_miniCompiler->AddNowCode();
				lastOptionOff = optionOff;
			}


			int expFilmStatus = pKoma->GetExpStatus();
			if (expFilmStatus != 0)
			{
				m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_FILMEXPSTATUS,1,&expFilmStatus,makeJsonFlag,komaTab+2);
				m_miniCompiler->AddNowCode();
			}



			if (makeJsonFlag != NULL)
			{
				pKoma->OutputData(makeJsonFlag,"{ \"type\" : \"_\" }",komaTab+2);
				pKoma->OutputData(makeJsonFlag,"],",komaTab+1);

				pFilm->OutputData(makeJsonFlag,"\"EffectList\" :",komaTab+1);
				pFilm->OutputData(makeJsonFlag,"[",komaTab+1);
			}

			//effect


			//load set or clear
			for (int layer=0;layer<layerMax;layer++)
			{
				int varControlLayer = pKoma->GetVarControlLayer(layer);
				if (varControlLayer == -1)
				{
					varControl[layer] = 0;
				}
				else if (varControlLayer > 0)
				{
					varControl[layer] = varControlLayer;
				}

				if ((varControlLayer != 0) || (varControl[layer] != 0))
				{
					int varpara[2];
					varpara[0] = layer;
					varpara[1] = 0;
					if (varControl[layer] > 0)
					{
						varpara[1] = m_varListNumber[varControl[layer]];
					}

					m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_VARCONTROLLAYER,2,varpara,makeJsonFlag,effectTab);
					m_miniCompiler->AddNowCode();
				}


				if (pKoma->GetEffectFlag(layer))
				{
					//file

					if (pKoma->GetPictureNumber(layer) != -1)
					{
						LPSTR filename = pKoma->GetPicFileName(layer);
						if (filename != NULL)
						{
							if ((*filename) == 0)
							{
								char errmes[256];
								wsprintf(errmes,"NULL FILE NAME ERROR!\nnnn=%s\nFILM=%s\nKOMA=%d\nLAYER=%d",m_app->GetFileNameOnly(),pFilm->GetMyName(),j,layer);

								MessageBox(m_frameHWND,errmes,"PASS1 ERROR",MB_OK);
							}



							if (m_makeCgListFlag)
							{
								BOOL cancel = FALSE;

								if (m_bufferSetCG == FALSE)
								{
									BOOL bufferFlag = FALSE;

									int nowEffect = pKoma->GetEffect(layer);
									if (nowEffect == EFFECT_BUFFER)
									{
										bufferFlag = TRUE;
									}
									else if (nowEffect == EFFECT_MAE)
									{
										int baseEffect = GetBaseEffect(pFilm,j,layer);
										if (baseEffect == EFFECT_BUFFER)
										{
											bufferFlag = TRUE;
										}
									}

									if (bufferFlag)
									{
										cancel = !CheckMeUseEffect(pFilm,j,layer);
									}
								}

								if (cancel == FALSE)
								{


									if ((varControl[layer] == 0) || (m_varControlConfig == 0))
									{
										SearchAndSetCG(filename,layer,makeJsonFlag,effectTab);
									}
									else
									{

										int varNumber = m_varListNumber[varControl[layer]];


										SearchAndSetCGByVar(filename,layer,varNumber,makeJsonFlag,effectTab);
									}
								}
							}

							m_miniCompiler->Pass2SystemFunctionLoad(CODE_SYSTEMFUNCTION_LOADDWQ,layer,filename,makeJsonFlag,effectTab);
							m_miniCompiler->AddNowCode();

							//rect

							int para[9];
							RECT src,dst;
							pKoma->GetEffectRect(&src,layer,1);
							pKoma->GetEffectRect(&dst,layer,2);
							int ks = 0;

							//640x480のときはそのまま？？

							para[0] = layer;

							para[1] = src.left;
							para[2] = src.top;
							para[3] = src.right;
							para[4] = src.bottom;

							para[5] = dst.left;
							para[6] = dst.top;
							para[7] = dst.right;
							para[8] = dst.bottom;

							m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_SETEFFECTRECT,9,para,makeJsonFlag,effectTab);
							m_miniCompiler->AddNowCode();
						}
					}
					else
					{

						//patch 2003-02-01***************************************
						int eff0 = pKoma->GetEffect(layer);
						if (eff0 != EFFECT_MAE)
						{
							m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_CLEAREFFECTLAYER,1,&layer,makeJsonFlag,effectTab);
							m_miniCompiler->AddNowCode();
						}
					}

					//effect
					int eff = pKoma->GetEffect(layer);
					int paraKosuu = pKoma->GetEffectParaKosuu(layer);
					int paras[258];
					paras[0] = layer;
					paras[1] = eff;

					if (eff != -1)
					{
						for (int i=0;i<paraKosuu;i++)
						{
							paras[2+i] = pKoma->GetEffectPara(layer,i);
						}
					}
					else
					{
						paraKosuu = 0;
					}


					///anime to setcg
					if (eff == EFFECT_ANIMATION)
					{
						if (m_makeCgListFlag)
						{
							BOOL cancel = FALSE;
							//buffer?
							if (paraKosuu >= 8)
							{
								if (paras[2+7]) cancel = TRUE;
							}

							if (!cancel)
							{
								char filename[256];
								int animeStart = paras[2+2];
								int animeKosuu = paras[2+1];
							//	for (int anime=0;anime<animeKosuu;anime++)
								for (int anime=0;anime<1;anime++)//先頭の1個のみ登録
								{
									int animeNumber = animeStart + anime;
									wsprintf(filename,"bganime%d",animeNumber);

									SearchAndSetCG(filename,layer,makeJsonFlag,effectTab);
								}
							}
						}
					}






















					if (eff != EFFECT_KILLEFFECT)
					{
						m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_SETEFFECT,2+paraKosuu,paras,makeJsonFlag,effectTab);
						m_miniCompiler->AddNowCode();
					}
					else
					{
						m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_CLEAREFFECTLAYER,1,&layer,makeJsonFlag,effectTab);
						m_miniCompiler->AddNowCode();

					}
				}
				else
				{
					//clear??
					if (j>0)
					{
						CKomaData* pKomaMae = pFilm->GetKoma(j-1);
						if (pKomaMae->GetEffectFlag(layer))
						{
							m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_CLEAREFFECTLAYER,1,&layer,makeJsonFlag,effectTab);
							m_miniCompiler->AddNowCode();
						}
					}
				}
			}

			int preload = pKoma->GetPreload();

			if (preload)
			{
				for (int layer = 0;layer<layerMax;layer++)
				{
					if (pKoma->GetEffectFlag(layer) == FALSE)
					{
						//search
						int foundKomaSearch = -1;
						LPSTR preloadFilename = NULL;

						for (int komaSearch = j+1;komaSearch<kosuu2k;komaSearch++)
						{
							CKomaData* pSearchKoma = pFilm->GetKoma(komaSearch);
							if (pSearchKoma->GetEffectFlag(layer))
							{
								int checkEffect = pSearchKoma->GetEffect(layer);
								if (checkEffect == EFFECT_MAE) break;

								int checkPic = pSearchKoma->GetPictureNumber(layer);
								if (checkPic == -1) break;

								preloadFilename = pSearchKoma->GetPicFileName(layer);
								if (preloadFilename == NULL) break;

								//found!
								foundKomaSearch = komaSearch;
								break;
							}
						}

						if (foundKomaSearch != -1)
						{
							m_miniCompiler->Pass2SystemFunctionLoad(CODE_SYSTEMFUNCTION_PRELOADDWQ,layer,preloadFilename,makeJsonFlag,effectTab);
							m_miniCompiler->AddNowCode();

						}

					}
				}
			}




			//overrap
			if (overrapCount != 0)
			{
				//overrap effect

				int dat[2];
				dat[0] = overrapType;
				dat[1] = overrapCount;

				m_miniCompiler->Pass2SystemCommand(CODE_SYSTEMCOMMAND_OVERRAP,2,dat,makeJsonFlag,effectTab);
				m_miniCompiler->AddNowCode();
			}



			//@@@@@@@@@@@@@@@@@@message


			if (makeJsonFlag != NULL)
			{
				pKoma->OutputData(makeJsonFlag,"{ \"type\" : \"_\" }",komaTab+2);
				pKoma->OutputData(makeJsonFlag,"],",komaTab+1);

				pFilm->OutputData(makeJsonFlag,"\"MessageList\" :",komaTab+1);
				pFilm->OutputData(makeJsonFlag,"[",komaTab+1);
			}

//			int kosuu3 = pKoma->GetObjectKosuu();
			for (int i=0;i<kosuu3;i++)
			{
				if (makeJsonFlag != NULL)
				{
					pKoma->OutputData(makeJsonFlag,"[",messageTab);
				}



				LPSTR picFileName[16];
				if (i==0)
				{
					getNowPicList = 0;
				}

				CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(i));
				int typ = pMessage->GetMessageMode();


				int id[1];
				id[0] = pMessage->GetID();
				if (typ == 3) id[0] = -1;


				m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_STARTMESSAGE,1,id,makeJsonFlag,messageTab+1);
				m_miniCompiler->AddNowCode();


				int musicVolume = pMessage->GetMusicVolume();
				if (musicVolume != 0)
				{
					int musicFadeTime = pMessage->GetMusicFadeTime();
					int mpara[2];
					if (musicVolume == 101) musicVolume = 0;

					mpara[0] = musicVolume;
					mpara[1] = musicFadeTime;
					m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_MUSICVOLUMEONLY,2,mpara,makeJsonFlag,messageTab+1);
					m_miniCompiler->AddNowCode();
				}

				int expStatus = pMessage->GetExpStatus();
				if (expStatus != 0)
				{
					m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_MESSAGEEXPSTATUS,1,&expStatus,makeJsonFlag,messageTab+1);
					m_miniCompiler->AddNowCode();
				}

				int messageFontSizeType = pMessage->GetMessageFontSizeType();
				if (messageFontSizeType > 0)
				{
					m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_CHANGEMESSAGEFONTSIZETYPE,1,&messageFontSizeType,makeJsonFlag,messageTab+1);
					m_miniCompiler->AddNowCode();
					lastFontType = messageFontSizeType;
				}
				else
				{
					BOOL fontMustChange = FALSE;
					if (i == 0)
					{
						if (j == 0)
						{
							if (cleareffectflag == FALSE)
							{
								fontMustChange = TRUE;
							}
						}
					}

					if (lastFontType > 0)
					{
						fontMustChange = TRUE;
					}


					if (fontMustChange)
					{
						int fontSizeType1 = 0;
						m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_CHANGEMESSAGEFONTSIZETYPE,1,&fontSizeType1,makeJsonFlag,messageTab+1);
						m_miniCompiler->AddNowCode();

						lastFontType = 0;
					}

				}


				//voice
				int ch;
				for (ch=0;ch<4;ch++)
				{
					int voiceMode = pMessage->GetVoiceMode(ch);

//					if (voiceMode == 3)
//					{
//						int bbb = 0;
//						bbb++;
//					}

					if (voiceMode != 0)
					{
						//fade?
						if (pMessage->GetVoiceFadeInOut(ch))
						{
							int tm = pMessage->GetVoiceFadeTime(ch);
							int fpara[1];
							fpara[0] = tm;
							m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_NEXTFADE_VOICE,1,fpara,makeJsonFlag,messageTab+1);
							m_miniCompiler->AddNowCode();
						}
					}

					if (voiceMode == 3)
					{
						int vpara[2];
						vpara[0] = ch;
						vpara[1] = pMessage->GetVoiceVolume(ch);
						m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_VOLUMEONLY_VOICE,2,vpara,makeJsonFlag,messageTab+1);
						m_miniCompiler->AddNowCode();
					}
					else if (pMessage->CheckVoiceFlag(ch) == FALSE)
					{
						if (voiceOffed[ch] == FALSE)
						{
							BOOL voff = FALSE;
							if (ch < 2)
							{
								if (pMessage->CheckVoiceContinue(ch) == FALSE) voff = TRUE;
							}
							else
							{
								if (pMessage->CheckVoiceOff(ch)) voff = TRUE;
							}

							if (voff)
							{
								voiceOffed[ch] = TRUE;
								int useDef = 0;
								if (defaultVoiceFlag)
								{
									useDef = pMessage->CheckIncludeSeimei(defaultVoiceFlag);
								}
								m_miniCompiler->Pass2Voice(NULL,ch,0,NULL,useDef,makeJsonFlag,messageTab+1);
								m_miniCompiler->AddNowCode();
							//	m_miniCompiler->Pass2VoiceFlag(m_voiceFlagNumber);
							//	m_miniCompiler->AddNowCode();
							//	AddVoiceFlagFilename(vfile);

							}
						}

					}
					else
					{
						if (0)
						{
							m_miniCompiler->Pass2Voice(NULL,ch,0,NULL,0,makeJsonFlag,messageTab+1);
							m_miniCompiler->AddNowCode();
						}
						else
						{
							voiceOffed[ch] = FALSE;

							LPSTR vfile = pMessage->GetVoiceFileName(ch);
							
							int vpKosuu = 0;
							int vpara[16];	//vol,(loop),teiiXYZ,move,teii2XYZ,dop
							for (int vv=0;vv<16;vv++)
							{
								vpara[vv] = 0;
							}



							if (pMessage->CheckVoiceVolumeExist(ch))
							{
								vpara[0] = pMessage->GetVoiceVolume(ch);
								vpKosuu = 1;
							}

							//not check loop flag
							if (pMessage->CheckVoiceLoop(ch))
							{
								vpara[1] = 1;
								vpKosuu = 2;
							}

							if (pMessage->CheckVoiceTeiiExist(ch))
							{
								if ((ch & 1)== 0)
								{
									int tx = pMessage->GetVoiceTeii(0,ch);
									if (tx != 0)
									{
										vpara[2] = tx;
										vpKosuu = 3;
									}
								}
								else
								{
									vpara[2] = pMessage->GetVoiceTeii(0,ch);
									vpara[3] = pMessage->GetVoiceTeii(1,ch);
									vpara[4] = pMessage->GetVoiceTeii(2,ch);
									vpKosuu = 5;
								}
							}

							if ((ch & 1) == 1)
							{
								if (pMessage->CheckVoiceMoveFlag(ch))
								{
									vpara[5] = pMessage->GetVoiceMoveTime(ch);
									if (pMessage->CheckVoiceTeii2Exist(ch))
									{
										vpara[6] = pMessage->GetVoiceTeii2(0,ch);
										vpara[7] = pMessage->GetVoiceTeii2(1,ch);
										vpara[8] = pMessage->GetVoiceTeii2(2,ch);
									}
									vpKosuu = 9;

									if (pMessage->CheckVoiceDopplerSoutou(ch))
									{
										vpara[9] = 1;
										vpKosuu = 10;
									}
								}
							}

							if (pMessage->CheckVoiceNoWaitSameCharaVoice(ch))
							{
								vpara[10] = 1;
								vpKosuu = 11;

							}



							int useDef = 0;
							if (defaultVoiceFlag)
							{
								useDef = pMessage->CheckIncludeSeimei(defaultVoiceFlag);
							}

							m_miniCompiler->Pass2Voice(vfile,ch,vpKosuu,vpara,useDef,makeJsonFlag,messageTab+1);

	//						m_miniCompiler->Pass2SystemFunctionMessage(CODE_SYSTEMFUNCTION_VOICE,vfile);
							m_miniCompiler->AddNowCode();

							m_miniCompiler->Pass2VoiceFlag(m_voiceFlagNumber,makeJsonFlag,messageTab+1);
							m_miniCompiler->AddNowCode();
							AddVoiceFlagFilename(vfile);

						}
					}
				}





				//wave
				for (ch = 0;ch<8;ch++)
				{
					int seMode = pMessage->GetSeMode(ch);

					if (seMode != 0)
					{
						//fade?
						if (pMessage->GetSEFadeInOut(ch))
						{
							int tm = pMessage->GetSEFadeTime(ch);
							int fpara[1];
							fpara[0] = tm;
							m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_NEXTFADE_SE,1,fpara,makeJsonFlag,messageTab+1);
							m_miniCompiler->AddNowCode();
						}
					}


					if (seMode == 1)
					{
						int senum = pMessage->GetSENumber(ch);

						int spKosuu = 0;
						int spara[16];	//vol,(loop),teiiXYZ,move,teii2XYZ,dop
						for (int ss=0;ss<16;ss++)
						{
							spara[ss] = 0;
						}

						if (pMessage->CheckSeVolumeExist(ch))
						{
							spara[0] = pMessage->GetSeVolume(ch);
							spKosuu = 1;
						}

						//loop	flag
						if (pMessage->CheckSeLoop(ch))
						{
							spara[1] = 1;
							spKosuu = 2;
						}

						if (pMessage->CheckSeTeiiExist(ch))
						{
							if (ch < 4)
							{
								int tx = pMessage->GetSeTeii(0,ch);
								if (tx != 0)
								{
									spara[2] = tx;
									spKosuu = 3;
								}
							}
							else
							{
								spara[2] = pMessage->GetSeTeii(0,ch);
								spara[3] = pMessage->GetSeTeii(1,ch);
								spara[4] = pMessage->GetSeTeii(2,ch);
								spKosuu = 5;
							}
						}

//						if (ch > 0)//???
						if (ch > 0)//must
						{
							if (pMessage->CheckSeMoveFlag(ch))
							{
								spara[5] = pMessage->GetSeMoveTime(ch);
								if (pMessage->CheckSeTeii2Exist(ch))
								{
									spara[6] = pMessage->GetSeTeii2(0,ch);
									spara[7] = pMessage->GetSeTeii2(1,ch);
									spara[8] = pMessage->GetSeTeii2(2,ch);
								}
								spKosuu = 9;

								if (pMessage->CheckDopplerSoutou(ch))
								{
									spara[9] = 1;
									spKosuu = 10;
								}
							}
						}


						if (pMessage->CheckSEIsSystem(ch))
						{
							spara[10] = 1;
							spKosuu = 11;
						}

						m_miniCompiler->Pass2Sound(senum,ch,spKosuu,spara,makeJsonFlag,messageTab+1);
						m_miniCompiler->AddNowCode();

					}
					else if (seMode == 2)//stop
					{
						m_miniCompiler->Pass2Sound(-1,ch,0,NULL,makeJsonFlag,messageTab+1);
						m_miniCompiler->AddNowCode();
					}
					else if (seMode == 3)//volume only
					{
						int vpara[2];
						vpara[0] = ch;
						vpara[1] = pMessage->GetSeVolume(ch);
						m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_VOLUMEONLY_SE,2,vpara,makeJsonFlag,messageTab+1);
						m_miniCompiler->AddNowCode();
					}
				}



				//message

				if ((typ == 0) || (typ == 1) || (typ==2))
				{
//					LPSTR mes = pMessage->GetMessageData();
					LPSTR mes = pMessage->GetMessageDataSkipAllComment();


					if (m_termConfig != 0)
					{
						m_termControl->Start(mes);
						int term = -1;
						while((term = m_termControl->GetNextTerm()) != -1)
						{
							int termPara[1];
							termPara[0] = term;
							m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_SETTERM,1,termPara,makeJsonFlag,messageTab+1);
							m_miniCompiler->AddNowCode();
						}
					}

				//	int mustFace = pMessage->GetMustFace();
					int mustFace = pMessage->GetMustAndFixFace();
					//change 2015-03-18
					if ((mustFace == 0) && (lastMustFace != 0))
					{
						int mustFaceData[1];
						mustFaceData[0] = 0;

						m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_MUSTFACE,1,mustFaceData,makeJsonFlag,messageTab+1);
						m_miniCompiler->AddNowCode();
						lastMustFace = 0;
					}

					int face = pMessage->GetFace();

					if ((face != 0) || (mustFace != 0))
					{
						int faceChara = 0;
						int mustFace16 = mustFace & 0xffff;
						if (mustFace16 != 0)
						{
							faceChara = mustFace;
						}
						else
						{
							char charaNames[256];
							for (int cnm = 0;cnm<254;cnm++)
							{
								char cnmd = *(mes+cnm);
								charaNames[cnm] = cnmd;
								if ((cnmd == 0) || (cnmd == 0x0d) || (cnmd == 0x0a))
								{
									charaNames[cnm] = 0;
									charaNames[cnm+1] = 0;
									break;
								}
							}
							charaNames[254] = 0;
							charaNames[255] = 0;

							faceChara = m_app->SearchFaceChara(charaNames);
						}

						if (faceChara > 0)
						{
							if (mustFace != 0)
							{
								int mustFaceData[1];
								mustFaceData[0] = mustFace;

								m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_MUSTFACE,1,mustFaceData,makeJsonFlag,messageTab+1);
								m_miniCompiler->AddNowCode();

								lastMustFace = mustFace;
							}

							if (face != 0)
							{
								int faceData[2];
								faceData[0] = faceChara;
								faceData[1] = face;
								m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_FACE,2,faceData,makeJsonFlag,messageTab+1);
								m_miniCompiler->AddNowCode();
							}
						}
					}

					int messageEffect = pMessage->GetMessageEffect();
					if (messageEffect > 0)
					{
						int messageEffectPara[1];
						messageEffectPara[0] = messageEffect;
						m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_MESSAGEEFFECT,1,messageEffectPara,makeJsonFlag,messageTab+1);
						m_miniCompiler->AddNowCode();


						int messageEffectTime = pMessage->GetMessageEffectTime();
						if (messageEffectTime > 0)
						{
							int messageEffectTimePara[1];
							messageEffectTimePara[0] = messageEffectTime;
							m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_MESSAGEEFFECTTIME,1,messageEffectTimePara,makeJsonFlag,messageTab+1);
							m_miniCompiler->AddNowCode();
						}
					}



					if (m_rubiBuildFlag == 1)
					{
						//convert2
						int gyo2 = GetMessageGyo(mes);
						int rbptr = 0;

						for (int rb=0;rb<gyo2;rb++)
						{
							GetMessage1Gyo(rb,mes);
							LPSTR mes4 = m_rubiMaker->RubiConvert(m_tmpMessage);
							
							int ln4 = strlen(mes4);
							memcpy(m_rubiMakedBuffer+rbptr,mes4,ln4);
							rbptr += ln4;
							if (rb < gyo2-1)
							{
								m_rubiMakedBuffer[rbptr] = 0x0d;
								rbptr++;
								m_rubiMakedBuffer[rbptr] = 0x0a;
								rbptr++;
							}
							else
							{
								m_rubiMakedBuffer[rbptr] = 0;
								rbptr++;
								m_rubiMakedBuffer[rbptr] = 0;
								rbptr++;
							}
						}

						mes = m_rubiMakedBuffer;
					}

					int cmd = CODE_SYSTEMCOMMAND_PRINT;
					if (typ == 1) cmd = CODE_SYSTEMCOMMAND_LPRINT;
					if (typ == 2) cmd = CODE_SYSTEMCOMMAND_APPEND;



					int cutinCode = 0;

					if (cutin)
					{
						if (m_cutinBuildFlag == 1)
						{
							char charaNames[256];
							for (int cnm = 0;cnm<254;cnm++)
							{
								char cnmd = *(mes+cnm);
								charaNames[cnm] = cnmd;
								if ((cnmd == 0) || (cnmd == 0x0d) || (cnmd == 0x0a))
								{
									charaNames[cnm] = 0;
									charaNames[cnm+1] = 0;
									break;
								}
							}
							charaNames[254] = 0;
							charaNames[255] = 0;

							int cutinNumber = m_cutinNameData->SearchFirst(charaNames);
							while (cutinNumber > 0)
							{
								if (getNowPicList == 0)
								{
									pFilm->GetNowPicList(picFileName,j);
									getNowPicList = 1;
								}

								LPSTR tagName = m_cutinNameData->GetFileNameTag(cutinNumber);

								if (CheckTagPicFile(tagName,picFileName))
								{
									break;
								}
								cutinNumber = m_cutinNameData->GetNext();
							}

							if (cutinNumber > 0)
							{
								cutinCode = cutinNumber;
							}
						}
					}

					m_miniCompiler->Pass2SystemCommandMessage(cmd,mes,pMessage->GetID(),cutinCode,makeJsonFlag,messageTab+1);
				}
				else
				{
					int ln = pMessage->GetDrawCount();
					m_miniCompiler->Pass2SystemCommand(CODE_SYSTEMCOMMAND_DRAW,1,&ln,makeJsonFlag,messageTab+1);

					if (makeJsonFlag != NULL)
					{
						pMessage->OutputData(makeJsonFlag,"{ \"type\" : \"_\" }",messageTab+1);
					}
				}
				m_miniCompiler->AddNowCode();



				if (makeJsonFlag != NULL)
				{
				//	pMessage->OutputData(makeJsonFlag,"{ \"type\" : \"_\" }",messageTab+1);
					if (i < kosuu3-1)
					{
						pMessage->OutputData(makeJsonFlag,"],",messageTab);
					}
					else
					{
						pMessage->OutputData(makeJsonFlag,"]",messageTab);
					}
				}

			}				

			if (makeJsonFlag != NULL)
			{
				pKoma->OutputData(makeJsonFlag,"],",komaTab+1);
			}




			if (makeJsonFlag != NULL)
			{
				pKoma->OutputData(makeJsonFlag,"\"KomaFooter\" : ",komaTab+1);
				pKoma->OutputData(makeJsonFlag,"[",komaTab+1);
			}


//ここにコマエンドマーカー追加	@@@@@@@@@@@@@@@@@
			m_miniCompiler->Pass2Simple(COMMANDDATATYPE_ENDKOMA,makeJsonFlag,komaTab+2);
			m_miniCompiler->AddNowCode();




			if (makeJsonFlag != NULL)
			{
				pKoma->OutputData(makeJsonFlag,"{ \"type\" : \"_\" }",komaTab+2);
				pKoma->OutputData(makeJsonFlag,"]",komaTab+1);
			}


			if (makeJsonFlag != NULL)
			{
				if (j < kosuu2k - 1)
				{
					pKoma->OutputData(makeJsonFlag,"},",komaTab);
				}
				else
				{
					pKoma->OutputData(makeJsonFlag,"}",komaTab);
				}
			}

		}

		if (makeJsonFlag != NULL)
		{
			pFilm->OutputData(makeJsonFlag,"],",tab+1);

			pFilm->OutputData(makeJsonFlag,"\"FilmFooter\" :",tab+1);
			pFilm->OutputData(makeJsonFlag,"[",tab+1);
		}

		if (lastMustFace != 0)
		{
			int mustFaceData[1];
			mustFaceData[0] = 0;

			m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_MUSTFACE,1,mustFaceData,makeJsonFlag,tabHeader);
			m_miniCompiler->AddNowCode();
			lastMustFace = 0;
		}


		for (int layer=0;layer<layerMax;layer++)
		{
			if (varControl[layer] != 0)
			{
				int varpara[2];
				varpara[0] = layer;
				varpara[1] = 0;

				m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_VARCONTROLLAYER,2,varpara,makeJsonFlag,tabHeader);
				m_miniCompiler->AddNowCode();
				varControl[layer] = 0;
			}
		}


		//各種オフにする
		if (lastAutoMessage != 0)
		{
			int autoMessage2 = -1;
			m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_AUTOMESSAGE,1,&autoMessage2,makeJsonFlag,tabHeader);
			m_miniCompiler->AddNowCode();
		}

		if (lastCannotClick == 1)
		{
			int cannotClick2 = -1;
			m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_CANNOTCLICK,1,&cannotClick2,makeJsonFlag,tabHeader);
			m_miniCompiler->AddNowCode();
		}

		if (lastCannotSkip == 1)
		{
			int cannotSkip2 = -1;
			m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_CANNOTSKIP,1,&cannotSkip2,makeJsonFlag,tabHeader);
			m_miniCompiler->AddNowCode();
		}


		if (lastOptionOff == 1)
		{
			int optionOff2 = -1;
			m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_OPTIONOFF,1,&optionOff2,makeJsonFlag,tabHeader);
			m_miniCompiler->AddNowCode();
		}

		//コンフィグマスクをオフにする
		if (configMask != 0)
		{
			int configMask2 = -1;
			m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_CONFIGMASK,1,&configMask2,makeJsonFlag,tabHeader);
			m_miniCompiler->AddNowCode();
		}


		if (cutin != 0)
		{
			int cutinFlag = 0;
			m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_CUTIN,1,&cutinFlag,makeJsonFlag,tabHeader);
			m_miniCompiler->AddNowCode();
		}

		if (lastFontType > 0)
		{
			int fontSizeType1 = 0;
			m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_CHANGEMESSAGEFONTSIZETYPE,1,&fontSizeType1,makeJsonFlag,tabHeader);
			m_miniCompiler->AddNowCode();
		}

		if (renameLayer != 0)
		{
			int renameLayerOff = 0;

			m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_RENAMELAYER,1,&renameLayerOff,makeJsonFlag,tabHeader);
			m_miniCompiler->AddNowCode();
		}


//		setfilm
		int setfilmpara[1];
		setfilmpara[0] = k+1;

		m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_SETFILM,1,setfilmpara,makeJsonFlag,tabHeader);
		m_miniCompiler->AddNowCode();

//(endfilm)
		m_miniCompiler->Pass2Simple(COMMANDDATATYPE_ENDFILM,makeJsonFlag,tabHeader);
		m_miniCompiler->AddNowCode();

		//set return
		m_miniCompiler->Pass2Simple(COMMANDDATATYPE_RET,makeJsonFlag,tabHeader);
		m_miniCompiler->AddNowCode();


		if (makeJsonFlag != NULL)
		{
			pFilm->OutputData(makeJsonFlag,"{ \"type\" : \"_\" }",tab+2);
			pFilm->OutputData(makeJsonFlag,"]",tab+1);

			if (k < kosuu-1)
			{
				pFilm->OutputData(makeJsonFlag,"},",tab);
			}
			else
			{
				pFilm->OutputData(makeJsonFlag,"}",tab);
			}
		}

	}

	m_komaTableKosuu = komaOffsetPointer;

	return TRUE;
}




void CScriptCompiler::SetStartKoma(CKomaData* pKoma,int nextType,int nextWindowMode,FILE* makeJsonFlag,int tab)
{
	int kosuu = pKoma->GetObjectKosuu();
	int found = -1;
	
	for (int i=kosuu-1;i>=0;i--)
	{
		CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(i));
		int typ = pMessage->GetMessageMode();
		if ((typ == 0) || (typ == 1) || (typ == 2))
		{
			found = i;
			break;
		}
	}

	int id[3];
	id[0] = -1;
	if (found != -1)
	{
		CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(found));
		id[0] = pMessage->GetID();
	}

	id[1] = nextType;
	id[2] = nextWindowMode;

	m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_STARTKOMA,3,id,makeJsonFlag,tab);
	m_miniCompiler->AddNowCode();
}

BOOL CScriptCompiler::BuildSpt(BOOL bMesFlag,FILE* makeJsonFlag)
{
	m_targetMachine = m_app->GetConfig("targetMachine");

	m_miniCompiler->StartBuild(m_targetMachine);

	if (makeJsonFlag != NULL)
	{

		if (CompileFilm(makeJsonFlag)== FALSE)
		{
			MessageBox(m_frameHWND,"CompileFilm Error","ERROR",MB_OK);
		//	UpdateMyWindow(-1);
			return FALSE;
		}

		m_miniCompiler->EndBuild();
		return TRUE;
	}

	if (CompileStory()== FALSE)
	{
MessageBox(m_frameHWND,"CompileStory Error","ERROR",MB_OK);

	//	UpdateMyWindow(-1);
		return FALSE;
	}

	if (CompileFilm()== FALSE)
	{
MessageBox(m_frameHWND,"CompileFilm Error","ERROR",MB_OK);
	//	UpdateMyWindow(-1);
		return FALSE;
	}



	//set jump table


//	CStoryBookDoc* pDoc = (CStoryBookDoc*)m_document[STORYBOOK_WINDOW];
//	CStoryBookData* pBook = pDoc->GetStoryBookData();
	CStoryBookData* pBook = m_app->GetStoryBookData();

//	CFilmCaseDoc* pDoc2 = (CFilmCaseDoc*)m_document[FILMCASE_WINDOW];
//	CFilmCaseData* pCase = pDoc2->GetFilmCaseData();
	CFilmCaseData* pCase = m_app->GetFilmCaseData();


	int kosuu = pBook->GetObjectKosuu();
	int kosuu2 = pCase->GetObjectKosuu();

	int* obj = m_miniCompiler->GetMakedPointer();
	int pc = m_miniCompiler->GetMakedSize();

	*(obj+8) = kosuu + kosuu2;
	*(obj+9) = pc+4;

	int dat[4];
	dat[0] = 4+kosuu+kosuu2;
	dat[1] = IDENTIFY_DATA;
	dat[2] = CODE_DATA_TABLE;
	dat[3] = TYPE_TABLE_LABEL;
	//
	m_miniCompiler->AddCode(dat,4);

	m_miniCompiler->AddCode(m_storyJumpTable,kosuu);
	m_miniCompiler->AddCode(m_filmJumpTable,kosuu2);

	m_miniCompiler->SetStoryKosuu(kosuu);




	pc = m_miniCompiler->GetMakedSize();
	*(obj+16) = pc;

	//こまジャンプテーブル追加する@@@
	dat[0] = 4 + kosuu2;
	dat[1] = IDENTIFY_DATA;
	dat[2] = CODE_DATA_TABLE;
	dat[3] = TYPE_TABLE_LABEL;	//???

	m_miniCompiler->AddCode(dat,4);
	m_miniCompiler->AddCode(m_komaOffsetByFilm,kosuu2);



	pc = m_miniCompiler->GetMakedSize();
	*(obj+17) = pc;

	dat[0] = 4 + m_komaTableKosuu;
	dat[1] = IDENTIFY_DATA;
	dat[2] = CODE_DATA_TABLE;
	dat[3] = TYPE_TABLE_LABEL;	//???

	m_miniCompiler->AddCode(dat,4);
	m_miniCompiler->AddCode(m_komaJumpTable,m_komaTableKosuu);



	m_miniCompiler->EndBuild();	//set str mes table and data



if (bMesFlag)
{
	MessageBox(m_frameHWND,"OK","OK",MB_OK);
}


	char filename[1024];
	wsprintf(filename,"spt\\%s.spt",m_app->GetFileNameOnly());
	FILE* file = CMyFile::Open(filename,"wb");

	int angouXorCode = m_app->GetConfig("angouXorCode");

	int angouIncCode = 0;
	int angouXorAddCode = m_app->GetConfig("angouXorAddCode");
	int angouIncAddCode = m_app->GetConfig("angouIncAddCode");

	if (file != NULL)
	{
		int* ptr = m_miniCompiler->GetMakedPointer();
		int sz = m_miniCompiler->GetMakedSize();

//暗号化
		for (int i=0;i<sz;i++)
		{
			int d = *(ptr+i);

			d -= angouIncCode;
			d ^= angouXorCode;

			angouXorCode += angouXorAddCode;
			angouIncCode += angouIncAddCode;

			//d ^= 0xffffffff;

			*(ptr+i) = d;
		}

		fwrite(ptr,sizeof(int),sz,file);

		fclose(file);
	}

	return TRUE;

}


BOOL CScriptCompiler::KaisekiScenePass1(void)
{
	int i;
	BOOL b = TRUE;

	int n = m_sceneDataList->GetNameKosuu();
	int workKosuu = n;
	int charaKosuu = 0;

	for (i=0;i<n;i++)
	{
		LPSTR name = m_sceneDataList->GetName(i);
		char c = *name;
		if (c == '[')
		{
			charaKosuu++;
		}
		if (c == '@')
		{
			workKosuu++;
		}
	}

	m_sceneDataSize = (workKosuu+2) * sizeof(int);
	m_sceneCharaKosuu = charaKosuu;


	m_sceneWork = new int[workKosuu];
	m_charaOffsetTable = new int[charaKosuu];


	ZeroMemory(m_sceneWork,sizeof(int)*workKosuu);
	ZeroMemory(m_charaOffsetTable,sizeof(int)*charaKosuu);

	//初期化
	int pc = 0;
	int chara = 0;
	int filmKosuu = 0;

	BOOL filmFlag = FALSE;
	BOOL sceneFlag = FALSE;

	int scenePtr = 0;
	int filmPtr = 1;

	int erc = -1;

	int sceneKosuu = 0;
	for (i=0;i<n;i++)
	{
		LPSTR name = m_sceneDataList->GetName(i);
		char c = *name;
		if (c == '[')
		{
			if (sceneFlag)
			{
				m_sceneWork[scenePtr] = sceneKosuu;

				sceneFlag = FALSE;
			}

			if (filmFlag)
			{
				m_sceneWork[filmPtr+1] = filmKosuu;
				filmFlag = FALSE;
			}

			sceneFlag = TRUE;
			scenePtr = pc;

			m_charaOffsetTable[chara] = pc;

			chara++;
			pc++;
			filmKosuu = 0;
			sceneKosuu = 0;
		}
		else if (c == '@')
		{
			if (filmFlag)
			{
				m_sceneWork[filmPtr+1] = filmKosuu;
				filmFlag = FALSE;
			}

			filmPtr = pc;
			filmKosuu = 0;


			//scriptname check
			LPSTR name = m_sceneDataList->GetName(i);
			name++;
			int sgn = 1;
			if ((*name) == '#')
			{
				sgn = -1;
				name++;
			}

			int sptNum = m_projectList->SearchName(name);
			if (sptNum == -1)
			{
				MessageBox(m_frameHWND,"シーン登録リストのそのスクリプトがないよ",name,MB_OK);
				erc = i;
				b = FALSE;
				break;
			}
			else
			{
				m_sceneWork[filmPtr] = sptNum * sgn;
			}

			sceneKosuu++;			
			pc += 2;
		}
		else	//filmname or story
		{
			m_sceneWork[pc] = 0xCCCC0000 | i;
			pc++;
			filmKosuu++;

			filmFlag = TRUE;
		}
	}


	//last 1

	if (sceneFlag)
	{
		m_sceneWork[scenePtr] = sceneKosuu;

		sceneFlag = FALSE;
	}

	if (filmFlag)
	{
		m_sceneWork[filmPtr+1] = filmKosuu;
		filmFlag = FALSE;
	}


	return b;
}



BOOL CScriptCompiler::KaisekiScenePass2(int n)
{
	BOOL b = TRUE;

	for (int k=0;k<m_sceneCharaKosuu;k++)
	{
//MessageBox(NULL,"LOOP1","test",MB_OK);
		if (b)
		{
			int pc = m_charaOffsetTable[k];
			int sceneKosuu = m_sceneWork[pc];
			pc++;

			for (int j=0;j<sceneKosuu;j++)
			{
//MessageBox(NULL,"LOOP2","test",MB_OK);

				if (b)
				{
					int pc0 = pc;

					int sptNum = m_sceneWork[pc];
					if (sptNum<0) sptNum *= -1;

					int filmKosuu = m_sceneWork[pc+1];

					pc += 2;

					if (sptNum == n)
					{
						for (int i=0;i<filmKosuu;i++)
						{
							int p = m_sceneWork[pc];
							int s = p & 0x0000ffff;

							LPSTR name = m_sceneDataList->GetName(s);
							BOOL sgn = 1;
							if ((*name) == '!')
							{
								name++;
								sgn = -1;
							}

							if ((*name) == '%')
							{
								name++;
								//story
								int checkStory = SearchStory(name);
								if (checkStory == -1)
								{
									char mes[256];
									wsprintf(mes,"%d番目のキャラのシーン登録で、\nシーン登録用のストーリー[%s]が\nスクリプト[%s]にはいってにゃいよ",k,name,m_projectList->GetName(n));
									MessageBox(m_frameHWND,mes,"ERROR",MB_OK);
									b = FALSE;
									break;
								}


								int fkosuu = 0;
								CStoryData* story = (CStoryData*)((m_app->GetStoryBookData())->GetObjectData(checkStory));
								//フィルム個数数える
								int skosuu = story->GetObjectKosuu();

								for (int aa=0;aa<skosuu;aa++)
								{
									CCommandData* pcom = (CCommandData*)(story->GetObjectData(aa));
									if (pcom->GetCommandType() == COMMANDDATATYPE_FILM)
									{
										fkosuu++;
									}
								}

								if (fkosuu == 0)
								{
									char mes[256];
									wsprintf(mes,"%d番目のキャラのシーン登録で、\nシーン登録用のストーリー[%s](spt=%s)にフィルムがひとつもにゃいよ",k,name,m_projectList->GetName(n));
									MessageBox(m_frameHWND,mes,"ERROR",MB_OK);
									b = FALSE;
									break;
								}



								//ワーク再取得、変更

								if (fkosuu > 1)
								{
									int workKosuu = m_sceneDataSize / sizeof(int);
									int* tmp = new int[workKosuu + (fkosuu-1)];

									int cc;
									for (cc=0;cc<=pc;cc++)
									{
										tmp[cc]= m_sceneWork[cc];
									}

									for (cc=pc+1;cc<workKosuu;cc++)
									{
										tmp[cc+fkosuu-1]= m_sceneWork[cc];
									}

									workKosuu += (fkosuu-1);

									delete [] m_sceneWork;
									m_sceneWork = tmp;

//MessageBox(NULL,"test1111","test",MB_OK);

									m_sceneDataSize = workKosuu*sizeof(int);

									for (int bb=k+1;bb<m_sceneCharaKosuu;bb++)
									{
										m_charaOffsetTable[bb] += (fkosuu-1);
									}

									filmKosuu += (fkosuu-1);
									i += (fkosuu-1);


									m_sceneWork[pc0+1] += (fkosuu-1);
								}


//MessageBox(NULL,"test2222","test",MB_OK);

								for (int ee=0;ee<skosuu;ee++)
								{
									CCommandData* pcom = (CCommandData*)(story->GetObjectData(ee));
									if (pcom->GetCommandType() == COMMANDDATATYPE_FILM)
									{
										//LPSTR sfilmname = pcom->GetMyName();
										LPSTR sfilmname = pcom->GetTextBuffer();
										int filmNumber = SearchFilm(sfilmname);
										if (filmNumber == -1)
										{
											char mes[256];
											wsprintf(mes,"%d番目のキャラのシーン登録で、\nシーン登録用のストーリー[%s](スクリプト[%s])がおかしいよ",k,name,m_projectList->GetName(n));
											MessageBox(m_frameHWND,mes,"ERROR",MB_OK);
											b = FALSE;
											break;
										}
										else
										{
											m_sceneWork[pc] = (filmNumber + 1) * sgn;
										}
										pc++;
									}
								}
//MessageBox(NULL,"test3333","test",MB_OK);

							}
							else
							{
								int filmNumber = SearchFilm(name);
								if (filmNumber == -1)
								{
									char mes[256];
									wsprintf(mes,"%d番目のキャラのシーン登録で、\nシーン登録用のフィルム[%s]が\nスクリプト[%s]にはいってにゃいよ",k,name,m_projectList->GetName(n));
									MessageBox(m_frameHWND,mes,"ERROR",MB_OK);
									b = FALSE;
									break;
								}
								else
								{
									m_sceneWork[pc] = (filmNumber + 1) * sgn;
								}
								pc++;
							}
						}
					}
					else
					{
						pc += filmKosuu;
					}
				}
			}
		}
	}

	return b;
}


void CScriptCompiler::AllBuild(FILE* makeJsonFlag)
{
	LPSTR fileNameOnly = m_app->GetFileNameOnly();
	int ln = strlen(fileNameOnly);
	if (ln>254) ln = 254;

	char lastFileName[256];
	memcpy(lastFileName,fileNameOnly,ln);
	lastFileName[ln] = 0;
	lastFileName[ln+1] = 0;

	if (m_app->CheckSave(TRUE) == FALSE) return;

	ClearVoiceFlag();

	int sceneMakedFlag = m_app->GetSceneMakeFlag();

	if (sceneMakedFlag)
	{
		if (m_sceneDataLoadFlag == FALSE)
		{
			if (m_sceneDataList->LoadFile("nnndir\\シーン回想登録テキスト.txt"))
			{
				m_sceneDataLoadFlag = TRUE;
			}
		}
	}


	m_cutinBuildFlag = m_app->GetConfig("cutinBuildFlag");
	if (m_cutinBuildFlag == -1)
	{
		int rt = MessageBox(m_frameHWND,"カットイン処理も行いますか","質問",MB_OKCANCEL | MB_APPLMODAL);
		if (rt == IDOK)
		{
			m_cutinBuildFlag = 1;
		}
	}
	if (m_cutinBuildFlag == 1)
	{
		m_app->ReloadCutinFile();
	}

	m_rubiBuildFlag = m_app->GetConfig("rubiBuildFlag");
	if (m_rubiBuildFlag == -1)
	{
		int rt = MessageBox(m_frameHWND,"ルビ追加処理も行いますか","質問",MB_OKCANCEL | MB_APPLMODAL);
		if (rt == IDOK)
		{
			m_rubiBuildFlag = 1;
		}
	}

	m_makeCgListFlag = FALSE;
	int autoCGMode = m_app->GetAutoSetCGFlag();
	if (autoCGMode == -1)
	{
		int rt = MessageBox(m_frameHWND,"SETCGもつけますか","質問",MB_OKCANCEL | MB_APPLMODAL);
		if (rt == IDOK)
		{
			autoCGMode = 1;
		}
	}

	if (autoCGMode == 1)
	{
		if (m_cgDataLoadFlag == FALSE)
		{
			LoadCGList();
			m_cgDataLoadFlag = TRUE;
		}
		m_makeCgListFlag = TRUE;
	}


	if (m_makeCgListFlag)
	{
		if (m_app->GetCheckSameCGNameFlag() != 0)
		{
			int errorCG = CheckSameCGList();
			if (errorCG != -1)
			{
				char errmes[256];
				int errorNum1 = errorCG / 10000;
				int errorNum2 = errorCG % 10000;
				LPSTR errorName = m_cgDataList[errorNum1]->GetName(errorNum2);
				wsprintf(errmes,"CGLISTに同一の名前が存在します\nname=%s\nlist=%d num=%d",errorName,errorNum1+1,errorNum2+1);
				MessageBox(m_frameHWND,errmes,"警告",MB_OK | MB_ICONEXCLAMATION);
			}
		}
	}



	m_varControlConfig = m_app->GetConfig("varControlSetCG");

	m_varControlSetCGMax = m_varControlConfig;

	if (m_varControlConfig == -1)
	{
		//calcu max

		m_varControlSetCGMax = 1;

		for (int j=0;j<m_cgDataListKosuu;j++)
		{
			int kosuu = m_cgDataList[j]->GetNameKosuu();
			for (int i=0;i<kosuu;i++)
			{
				LPSTR name = m_cgDataList[j]->GetName(i);
				int ln = strlen(name);

				if (strlen(name) >= 3)
				{
					BOOL suuji = FALSE;
					int n = ln-1;

					int suujiTop = -1;

					while (n>=0)
					{
						char c = *(name+n);
						if ((c >= '0') && (c<='9'))
						{
							suuji = TRUE;
							suujiTop = n;
							n--;
						}
						else
						{
							break;
						}
					}

					if (suuji)
					{
						if (n>=0)
						{
							if ((*(name+n)) == '_')
							{
								n--;
								if (n>=0)
								{
									if ((*(name+n)) == '_')
									{
										int d = atoi(name+suujiTop);
										if (d > m_varControlSetCGMax)
										{
											m_varControlSetCGMax = d;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}



	if (m_makeCgListFlag)
	{
		//clear work
		for (int j=0;j<320;j++)
		{
			for (int i=0;i<64;i++)
			{
				m_cgUseFlag[j][i] = 0x00000000;
			}
		}
	}


	m_kaisekiScenePass1Flag = FALSE;

	if (m_sceneDataLoadFlag)
	{
		m_kaisekiScenePass1Flag = KaisekiScenePass1();
	}

	m_termConfig = m_app->GetConfig("termFlag");
	if (m_termConfig != 0)
	{
		CNameList* termList = m_app->GetTermList();
		m_termControl->SetTermList(termList);
	}


	BOOL flg = FALSE;

	m_kaisekiScenePass2Flag = TRUE;

	int nnnKosuu = m_projectList->GetNameKosuu();
	if (makeJsonFlag != NULL)
	{
		if (m_kaisekiScenePass1Flag)
		{
			if (m_kaisekiScenePass2Flag)
			{
				m_kaisekiScenePass2Flag = TRUE;//KaisekiScenePass2(pp);
			}

			if (BuildSpt(FALSE,makeJsonFlag) == FALSE)
			{
				MessageBox(m_frameHWND,fileNameOnly,"Compile ERROR",MB_OK | MB_ICONEXCLAMATION);
			}

		}
		return;
	}

	for (int pp=0;pp<nnnKosuu;pp++)
	{
		LPSTR nnnfilename = m_projectList->GetName(pp);
		if (nnnfilename != NULL)
		{
//			memcpy(m_fileNameOnly,nnnfilename,strlen(nnnfilename)+1);
//			memcpy(m_lastFileName,m_fileNameOnly,strlen(m_fileNameOnly)+1);
			m_app->SetFileNameOnly(nnnfilename);
			if (m_app->LoadByFileNameOnly(nnnfilename) == FALSE)
			{
				MessageBox(m_frameHWND,nnnfilename,"Load ERROR",MB_OK | MB_ICONEXCLAMATION);
				flg = TRUE;
				m_kaisekiScenePass2Flag = FALSE;
				break;
			}

//			memcpy(m_fileNameOnly,nnnfilename,strlen(nnnfilename)+1);
//			memcpy(m_lastFileName,m_fileNameOnly,strlen(m_fileNameOnly)+1);
			m_app->SetFileNameOnly(nnnfilename);


			if (m_kaisekiScenePass1Flag)
			{
				if (m_kaisekiScenePass2Flag)
				{
					m_kaisekiScenePass2Flag = KaisekiScenePass2(pp);
				}
			}


			if (BuildSpt(FALSE) == FALSE)
			{
//				memcpy(m_fileNameOnly,nnnfilename,strlen(nnnfilename)+1);
//				memcpy(m_lastFileName,m_fileNameOnly,strlen(m_fileNameOnly)+1);
				m_app->SetFileNameOnly(nnnfilename);
				MessageBox(m_frameHWND,nnnfilename,"Compile ERROR",MB_OK | MB_ICONEXCLAMATION);
				m_kaisekiScenePass2Flag = FALSE;
				flg = TRUE;
				break;
			}
		}
	}


	





//	if (m_kaisekiScenePass1Flag && m_kaisekiScenePass2Flag)
	if (sceneMakedFlag)
	{
//		FILE* file000 = fopen("nnndir\\scene.dat","wb");	
		FILE* file000 = CMyFile::Open("spt\\scene.dat","wb");	//2003-06-16修正
		if (file000 != NULL)
		{
			fwrite(&m_sceneDataSize,sizeof(int),1,file000);
			fwrite(&m_sceneCharaKosuu,sizeof(int),1,file000);
			fwrite(m_sceneWork,sizeof(char),m_sceneDataSize - 8,file000);
			fclose(file000);

			int sceneCheckMode = m_app->GetConfig("sceneCheckMode");
			if (sceneCheckMode == 1)
			{
				m_app->CheckSceneData(FALSE);
			}
			else if (sceneCheckMode == 2)
			{
				m_app->CheckSceneData(TRUE);
			}
		}
	}





	//var暗号化
	if (1)
	{
		BOOL oldModify = m_varList->CheckModify();
		LPSTR varFxfFilename = m_app->GetVarFxfFilename();
		m_varList->SaveFile(varFxfFilename,TRUE);
		m_varList->SetModify(oldModify);
	}



	CopyFile("nnndir\\project.txt","spt\\project.xtx",FALSE);
	CopyFile("nnndir\\command.txt","spt\\command.xtx",FALSE);
	CopyFile("nnndir\\function.txt","spt\\function.xtx",FALSE);
	CopyFile("nnndir\\selist.txt","spt\\selist.xtx",FALSE);
	CopyFile("nnndir\\charaname.txt","spt\\charaname.xtx",FALSE);
	CopyFile("nnndir\\facelist.txt","spt\\facelist.xtx",FALSE);
	CopyFile("nnndir\\bgmlist.txt","spt\\bgmlist.xtx",FALSE);
	CopyFile("nnndir\\namecol.txt","spt\\namecol.xtx",FALSE);
	CopyFile("nnndir\\movielist.txt","spt\\movielist.xtx",FALSE);
	CopyFile("nnndir\\varinitdata.txt","spt\\varinitdata.xtx",FALSE);
	CopyFile("nnndir\\varinitdata2.txt", "spt\\varinitdata2.xtx", FALSE);
	CopyFile("nnndir\\termlist.txt","spt\\termlist.xtx",FALSE);

	//shoplist 暗号
	if (1)
	{
		FILE* shopSrc = NULL;
		fopen_s(&shopSrc,"nnndir\\shopitem.txt","rb");
		if (shopSrc != NULL)
		{
			fclose(shopSrc);

			CopyFile("nnndir\\shopitem.txt","nya\\shopitem.fxf",FALSE);

			CNameList* tmpInit = new CNameList();
			tmpInit->SaveAngouRoutine("nya\\shopitem.fxf");


			delete tmpInit;
		}
	}





	if (m_app->GetConfig("cutin"))
	{
		CopyFile("nnndir\\cutinChara.txt","spt\\cutinChara.xtx",FALSE);
	}


//	CopyFile("nnndir\\heroin.txt","spt\\heroin.xtx",FALSE);

	//CopyFile("nnndir\\scenevoice.txt","spt\\scenevoice.txt",FALSE);
	//CopyFile("nnndir\\systemse.txt","spt\\systemse.txt",FALSE);

//#define SETCG_FILE_NAME "nnndir\\setcg.txt"copy???

	if (m_makeCgListFlag)
	{
		FILE* cgVoiceFile = NULL;
		fopen_s(&cgVoiceFile,"spt\\cgVoice.xtx","wb");

		BOOL cgVoiceCreateFlag = FALSE;

		for (int i=0;i<m_cgDataListKosuu;i++)
		{
			char filenameSrc[256];
			char filenameDst[256];
			int k = i + 1;
			wsprintf(filenameSrc,"nnndir\\cglist%d%d.txt",k/10,k%10);
			wsprintf(filenameDst,"spt\\cglist%d%d.xtx",k/10,k%10);

			FILE* cgListFile = NULL;
			fopen_s(&cgListFile,filenameDst,"wb");
			int cgListKosuu = m_cgDataList[i]->GetNameKosuu();

			for (int k=0;k<cgListKosuu;k++)
			{
				LPSTR name = m_cgDataList[i]->GetName(k);
				if (CheckAndSplitCGList(name))
				{
					char cgVoiceBuf[256];
					wsprintf(cgVoiceBuf,"%d,%d,%s\x00d\x00a",i+1,k+1,m_cgVoiceData);
					fwrite(cgVoiceBuf,sizeof(char),strlen(cgVoiceBuf),cgVoiceFile);
					fwrite(m_cgListDataOnly,sizeof(char),strlen(m_cgListDataOnly),cgListFile);
					fwrite("\x00d\x00a",sizeof(char),2,cgListFile);
					cgVoiceCreateFlag = TRUE;
				}
				else
				{
					fwrite(name,sizeof(char),strlen(name),cgListFile);
					fwrite("\x00d\x00a",sizeof(char),2,cgListFile);
				}
			}
			fclose(cgListFile);

//			CopyFile(filenameSrc,filenameDst,FALSE);
		}

		fclose(cgVoiceFile);
		if (cgVoiceCreateFlag == FALSE)
		{
			DeleteFile("spt\\cgVoice.xtx");
		}
	}


	SortAndOutputVoiceFlagFilename();

	//その他設定保存
	if (1)
	{
		FILE* exesetup = NULL;
		fopen_s(&exesetup,"spt\\exesetup.xtx","wb");
		if (exesetup != NULL)
		{
			char wk[1024];
			wsprintf(wk,"name exesetup\x00d\x00a");
			fwrite(wk,sizeof(char),strlen(wk),exesetup);

			LPSTR name1 = "mainScreenWindowSizeX1";
			wsprintf(wk,"%s %d\x00d\x00a",name1,m_app->GetConfig(name1));
			fwrite(wk,sizeof(char),strlen(wk),exesetup);

			LPSTR name2 = "mainScreenWindowSizeY1";
			wsprintf(wk,"%s %d\x00d\x00a",name2,m_app->GetConfig(name2));
			fwrite(wk,sizeof(char),strlen(wk),exesetup);

			LPSTR name3 = "pngGammaFlag";
			wsprintf(wk,"%s %d\x00d\x00a",name3,m_app->GetConfig(name3));
			fwrite(wk,sizeof(char),strlen(wk),exesetup);

			fclose(exesetup);
		}
	}


	//initangou
	if (m_app->GetConfig("initAngou"))
	{
		InitAngou();
	}




	if (flg)
	{
		MessageBox(m_frameHWND,"エラーです。loadしなおしてから修正してください","確認",MB_OK);
	}
	else
	{
		if (m_makeCgListFlag)
		{
			int n1 = -1;
			int n2 = -1;

			BOOL b = TRUE;
			for (int j=0;j<m_cgDataListKosuu;j++)
			{
				if (b)
				{
					int kosuu = m_cgDataList[j]->GetNameKosuu();
					for (int i=0;i<kosuu;i++)
					{
						if (CheckUseCG(j,i) == FALSE)
						{
							b = FALSE;
							n1 = j;
							n2 = i;
							break;
						}
					}
				}
			}

			if (b == FALSE)
			{
				char mes[256];
				wsprintf(mes,"%d番目のキャラの%d番目のCGが登録されてないよ",n1+1,n2+1);
				MessageBox(m_frameHWND,mes,"ERROR",MB_OK | MB_ICONEXCLAMATION);
			}
		}

		MessageBox(m_frameHWND,"すべておわりましたにゃ","確認",MB_OK);

	}



//	memcpy(m_fileNameOnly,lastFileName,strlen(lastFileName)+1);
	m_app->SetFileNameOnly(lastFileName);
	m_app->LoadByFileNameOnly(lastFileName);
}


void CScriptCompiler::SearchAndSetCG(LPSTR filename,int layer,FILE* makeJsonFlag,int tab)
{
	for (int j=0;j<m_cgDataListKosuu;j++)
	{
		int kosuu = m_cgDataList[j]->GetNameKosuu();
		for (int i=0;i<kosuu;i++)
		{
			LPSTR name = m_cgDataList[j]->GetName(i);
			name = SkipCGCommand(name);						//change 2003-11-11
//			if ((*name) == '@') name++;						//debug 2003-06-16

			if (_stricmp(name,filename) == 0)
			{
				//m_miniCompiler

				int setcgpara[3];
				setcgpara[0] = j+1;
				setcgpara[1] = i+1;
				setcgpara[2] = layer;

				m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_SETCG,3,setcgpara,makeJsonFlag,tab);
				m_miniCompiler->AddNowCode();

				SetUseCg(j,i);
				break;
			}
		}
	}
}


void CScriptCompiler::SearchAndSetCGByVar(LPSTR filename,int layer,int var,FILE* makeJsonFlag,int tab)
{
	for (int j=0;j<m_cgDataListKosuu;j++)
	{
		int kosuu = m_cgDataList[j]->GetNameKosuu();
		for (int i=0;i<kosuu;i++)
		{
			LPSTR name = m_cgDataList[j]->GetName(i);

			char filename1[1024];
			for (int k=0;k<m_varControlSetCGMax;k++)
			{
				if (k == 0)
				{
					sprintf_s(filename1,sizeof(filename1),"%s",filename);
				}
				else
				{
					sprintf_s(filename1,sizeof(filename1),"%s__%d",filename,k+1);
				}


				name = SkipCGCommand(name);						//change 2003-11-11
//			if ((*name) == '@') name++;						//debug 2003-06-16

				if (_stricmp(name,filename1) == 0)
				{
				//m_miniCompiler

					int setcgpara[5];
					setcgpara[0] = j+1;
					setcgpara[1] = i+1;
					setcgpara[2] = layer;
					setcgpara[3] = var;
					setcgpara[4] = k + 1;

					m_miniCompiler->Pass2SystemFunction(CODE_SYSTEMFUNCTION_SETCGBYVAR,5,setcgpara,makeJsonFlag,tab);
					m_miniCompiler->AddNowCode();

					SetUseCg(j,i);
					break;
				}
			}
		}
	}
}

BOOL CScriptCompiler::LoadCGList(void)
{
	m_cgDataListKosuu = 0;
	for (int i=0;i<100;i++)
	{ 
		char filename[256];
		wsprintf(filename,"nnndir\\cglist%d%d.txt",(i+1) / 10, (i+1) % 10);
		FILE* file = CMyFile::Open(filename,"rb");
		if (file != NULL)
		{
			fclose(file);
			if (m_cgDataList[i] == NULL)
			{
				m_cgDataList[i] = new CNameList();
				m_cgDataList[i]->LoadFile(filename);
			}

			m_cgDataListKosuu++;
		}
		else
		{
			break;
		}
	}

	return TRUE;
}


int CScriptCompiler::CheckSameCGList(void)
{
	for (int k=0;k<m_cgDataListKosuu;k++)
	{
		CNameList* nameList = m_cgDataList[k];

		int kosuu = nameList->GetNameKosuu();
		if (kosuu > 1)
		{
			for (int j=0;j<kosuu-1;j++)
			{
				LPSTR name1 = nameList->GetName(j);
				name1 = SkipCGCommand(name1);

				for (int i=j+1;i<kosuu;i++)
				{
					LPSTR name2 = nameList->GetName(i);
					name2 = SkipCGCommand(name2);

					if (_stricmp(name1,name2) == 0)
					{
						return k * 10000 + i;
					}
				}
			}
		}
	}

	return -1;
}



LPSTR CScriptCompiler::SkipCGCommand(LPSTR name)
{
	LPSTR nameOrg = name;

	if ((*name) == '@') name++;

	if ((*name) == '!') name++;

	int animeCommand = FALSE;
	BOOL animeEndFound = FALSE;

	if ((*name) == '#')
	{
		animeCommand = TRUE;

		name++;
		int lna = strlen(name);
		if (lna == 0) return name;//error!
		while(lna>0)
		{
			char c = *name;
		//	int d = (int)c;
		//	d &= 0xff;
			if (c == '/')
			{
				name++;
				animeEndFound = TRUE;
				break;
			}

			name++;
			lna--;
		}
	}


/*
	if (animeCommand)
	{
		if (animeEndFound == FALSE)
		{
			char mes[1024];
			wsprintf(mes,"cglistコンパイルエラー\n%s\n\nスクリプト[%s]\nストーリー[%s]%d行め\n\n内容\n%s",errorMessage,scriptName,storyName,n+1,pCommand->GetTextBuffer());
			MessageBox(m_frameHWND,mes,errorCaption,MB_OK);
		}
	}
*/


	int ln = strlen(name);
	if (ln == 0) return name;

	if ((*name) != '[') return name;

	name++;
	ln--;

	while(ln>0)
	{
		char c = *name;

		if (c == ']')
		{
			name++;
			ln--;
			break;
		}

		int d = (int)c;
		d &= 0xff;

//			((c >= '0') && (c <= '9')) || (c == '_') || (c == '#') || (c ==':'))
		if (((d >= 0x80) && (d < 0xa0)) || ((d >= 0xe0) && (d < 0x100)))
		{
			name += 2;
			ln -= 2;
		}
		else
		{
			name += 1;
			ln -= 1;
		}
	}

	return name;
}

void CScriptCompiler::SetUseCg(int pl, int nm)
{
	int bt = nm & 0x1f;
	int n2 = nm >> 5;
	m_cgUseFlag[pl][n2] |= m_bitTable[bt];
}



BOOL CScriptCompiler::CheckUseCG(int pl, int nm)
{
	int bt = nm & 0x1f;
	int n2 = nm >> 5;

	if (m_cgUseFlag[pl][n2] & m_bitTable[bt]) return TRUE;

	return FALSE;
}

void CScriptCompiler::PrintCompileCommandError(CCommandData* pCommand,LPSTR storyName,int n,LPSTR errorMessage,LPSTR errorCaption)
{
	if (storyName == NULL)
	{
		storyName = m_unknownStoryName;
	}

	if (errorMessage == NULL)
	{
		errorMessage = m_unknownErrorMessage;
	}

	if (errorCaption == NULL)
	{
		errorCaption = m_unknownErrorCaption;
	}

	LPSTR scriptName = m_app->GetFileNameOnly();
	char mes[1024];
	wsprintf(mes,"コマンドコンパイルエラー\n%s\n\nスクリプト[%s]\nストーリー[%s]%d行め\n\n内容\n%s",errorMessage,scriptName,storyName,n+1,pCommand->GetTextBuffer());
	MessageBox(m_frameHWND,mes,errorCaption,MB_OK);
}





int CScriptCompiler::GetMessageGyo(LPSTR mes)
{
	if (mes == NULL) return 0;

	int kosuu = 0;
	int ln = strlen(mes);
	int ptr = 0;

	while (ptr<ln)
	{
		if (*(mes+ptr) == 0x00d)
		{
			kosuu++;
			ptr++;
		}
		ptr++;
	}

	if (*(mes+ln-2) != 0x00d)
	{
		kosuu++;
	}

	return kosuu;
}



int CScriptCompiler::GetMessage1Gyo(int n,LPSTR mes)
{
	if (mes == NULL) return 0;

	int ln = strlen(mes);
	
	int ptr = 0;

	int found = -1;
	if (n == 0) found = 0;
	int k = 0;

	if (n>0)
	{
		while (ptr<ln)
		{
			if (*(mes+ptr) == 0x00d)
			{
				k++;
				ptr += 2;
				if (n == k)
				{
					found = ptr;
					break;
				}
			}
			else
			{
				ptr++;
			}
		}
	}

	if (found == -1) return 0;

	int ptr2 = found;
	int ln2 = 0;

	while (ptr2<ln)
	{
		if (*(mes+ptr2) == 0x00d)
		{
			memcpy(m_tmpMessage,mes+found,ln2);
			m_tmpMessage[ln2] = 0;
			m_tmpMessage[ln2+1] = 0;
			return ln2;
		}

		ptr2++;
		ln2++;
	}

	memcpy(m_tmpMessage,mes+found,ln2);
	m_tmpMessage[ln2] = 0;
	m_tmpMessage[ln2+1] = 0;


	return ln2;
}

BOOL CScriptCompiler::CheckTagPicFile(LPSTR tagName,LPSTR* fileList)
{
	int ln = strlen(tagName);
	char work[1024];
	for (int i=0;i<16;i++)
	{
		LPSTR checkFile = fileList[i];
		if (checkFile != NULL)
		{
			int ln2 = strlen(checkFile);
			if (ln2 >= ln)
			{
				memcpy(work,checkFile,ln);
				work[ln] = 0;
				work[ln+1] = 0;
				if (strcmp(work,tagName) == 0)
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

int CScriptCompiler::GetBaseEffect(CFilmData* pFilm,int komaPlace,int layer,int* foundKomaPlace)
{
	if (pFilm == NULL) return -1;

	for (int i=komaPlace-1;i>=0;i--)
	{
		CKomaData* pKoma = pFilm->GetKoma(i);
		if (pKoma != NULL)
		{
			if (pKoma->GetEffectFlag(layer) == FALSE)
			{
				return -1;
			}

			int effect = pKoma->GetEffect(layer);
			if (effect == EFFECT_MAE) continue;
			if (foundKomaPlace != NULL)
			{
				*foundKomaPlace = i;
			}
			return effect;
		}
	}

	return -1;
}

BOOL CScriptCompiler::CheckMeUseEffect(CFilmData* pFilm,int komaPlace,int layer)
{
	for (int j=0;j<16;j++)
	{
		if (j != layer)
		{
			CKomaData* pKoma = pFilm->GetKoma(komaPlace);

			if (pKoma->GetEffectFlag(j))
			{
				int fromKoma = komaPlace;
				int fromEffect = pKoma->GetEffect(j);
				if (fromEffect == EFFECT_MAE)
				{
					int foundPlace = -1;
					fromEffect = GetBaseEffect(pFilm,komaPlace,j,&foundPlace);
					if (fromEffect != -1)
					{
						fromKoma = foundPlace;
						pKoma = pFilm->GetKoma(fromKoma);
					}
				}


				if (fromEffect != -1)
				{
					int paraKosuu = pKoma->GetEffectParaKosuu(j);

					int targetLayer = -1;

					if (fromEffect == EFFECT_SIMPLEWIPE)
					{
						if (paraKosuu >= 2)
						{
							int targetGetPut = 0;

							if (paraKosuu >= 7)
							{
								targetGetPut = pKoma->GetEffectPara(j,7);
							}
							if (targetGetPut == 0)
							{
								targetLayer = pKoma->GetEffectPara(j,2);
							}
						}
						else
						{
							targetLayer = 1;
						}
					}
					else if (fromEffect == EFFECT_MASKWIPE)
					{
						if (paraKosuu >= 1)
						{
							int targetGetPut = 0;

							if (paraKosuu >= 7)
							{
								targetGetPut = pKoma->GetEffectPara(j,7);
							}
							if (targetGetPut == 0)
							{
								targetLayer = pKoma->GetEffectPara(j,1);
							}
						}
						else
						{
							targetLayer = 12;
						}
					}
					else if (fromEffect == EFFECT_MOVEMASKWIPE)
					{
						if (paraKosuu >= 1)
						{
							int targetGetPut = 0;

							if (paraKosuu >= 6)
							{
								targetGetPut = pKoma->GetEffectPara(j,6);
							}
							if (targetGetPut == 0)
							{
								targetLayer = pKoma->GetEffectPara(j,1);
							}
						}
						else
						{
							targetLayer = 12;
						}
					}
					else if (fromEffect == EFFECT_HAMON)
					{
						if (paraKosuu >= 1)
						{
							targetLayer = pKoma->GetEffectPara(j,1);
						}
						else
						{
							targetLayer = 0;
						}
					}
					else if (fromEffect == EFFECT_CHANGE)
					{
						if (paraKosuu >= 1)
						{
							targetLayer = pKoma->GetEffectPara(j,1);
						}
						else
						{
							targetLayer = 13;
						}
					}
					else if (fromEffect == EFFECT_ANIME)
					{
						int pat = 4;
						int start = 12;
						if (paraKosuu >= 1)
						{
							pat = pKoma->GetEffectPara(j,1);
						}
						if (paraKosuu >= 4)
						{
							start = pKoma->GetEffectPara(j,4);
						}

						if ((layer >= start) && (layer < (start + pat)))
						{
							return TRUE;
						}
					}

					if (targetLayer == layer) return TRUE;
				}
			}
		}
	}

	return FALSE;
}

BOOL CScriptCompiler::CheckAndSplitCGList(LPSTR name)
{
	int ln = strlen(name);
	BOOL flg = FALSE;
	
	int found = -1;

	int openKakko = -1;
	int closeKakko = -1;

	int n = 0;
	while (n<ln)
	{
		char c = *(name+n);
		if (c == '#')
		{
			found = n;
			n++;
			while (n<ln)
			{
				char cc = *(name+n);
				if (cc == ']')
				{
					closeKakko = n;
					break;
				}

				int dd = (int)cc;
				dd &= 0xff;

				n++;
				if (((dd >= 0x80) && (dd < 0xa0)) || ((dd >= 0xe0) && (dd < 0x100)))
				{
					n++;
				}
			}
			break;
		}

		if (c == '[')
		{
			openKakko = n;
		}


		int d = (int)c;
		d &= 0xff;

		n++;
		if (((d>=0x80) && (d<0xa0)) || ((d >= 0xe0) && (d < 0x100)))
		{
			n++;
		}
	}

	if (found == -1) return FALSE;
	if ((openKakko == -1) || (closeKakko == -1)) return FALSE;//error


	int ln2 = closeKakko - found - 1;
	if (ln2>0)
	{
		memcpy(m_cgVoiceData,name+found+1,ln2);
	}
	else
	{
		m_cgVoiceData[ln2] = '-';
		ln2 = 1;
	}
	m_cgVoiceData[ln2] = 0;
	m_cgVoiceData[ln2+1] = 0;


	int ln3 = found;
	int start = closeKakko;
	if (found == (openKakko+1))
	{
		ln3--;
		start++;
	}
	if (ln3>0)
	{
		memcpy(m_cgListDataOnly,name,ln3);
	}
	int ln4 = ln - start;
	if (ln4>0)
	{
		memcpy(m_cgListDataOnly+ln3,name+start,ln4);
	}
	m_cgListDataOnly[ln3+ln4] = 0;
	m_cgListDataOnly[ln3+ln4+1] = 0;
	return TRUE;
}


BOOL CScriptCompiler::InitAngou(void)
{
	//init暗号
	CreateDirectory("init2",NULL);
	WIN32_FIND_DATA findFileData;
	HANDLE hFind;
	hFind = FindFirstFile("init\\*.xtx",&findFileData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		MessageBox(m_frameHWND,"initファイルが存在しません","警告",MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	else
	{
		CNameList* tmpInit = new CNameList();

		while (hFind)
		{
			char initnameonly[256];
			wsprintf(initnameonly,"%s",findFileData.cFileName);
			int initln = strlen(initnameonly);
			if (initln >= 4)
			{
				initnameonly[initln-4] = 0;
			}

			char srcInit[1024];
			char dstInit[1024];
			wsprintf(srcInit,"init\\%s.xtx",initnameonly);
			wsprintf(dstInit,"init2\\%s.fxf",initnameonly);

			CopyFile(srcInit,dstInit,FALSE);
			tmpInit->SaveAngouRoutine(dstInit);


			if (FindNextFile(hFind,&findFileData) == 0)
			{
				FindClose(hFind);
				hFind = NULL;
			}
		}

		delete tmpInit;
	}

	return TRUE;
}

void CScriptCompiler::AddVoiceFlagFilename(LPSTR filename)
{
	if (m_voiceFlagNumber < 65536*8)
	{
		if (m_voiceFlagNumber >= m_voiceFlagWorkMax)
		{
			//expand
			char* tmp = new char[(m_voiceFlagWorkMax + 65536) * (8+8)];
			memcpy(tmp,m_voiceFlagFileName,m_voiceFlagNumber*(8+8));
			delete [] m_voiceFlagFileName;
			m_voiceFlagFileName = tmp;
		}
	
		int ln = strlen(filename);
		if (ln>8) ln = 8;
		ZeroMemory(m_voiceFlagFileName+(8+8)*m_voiceFlagNumber,16);
		memcpy(m_voiceFlagFileName+(8+8)*m_voiceFlagNumber,filename,ln);
		int* ptr = (int*)(m_voiceFlagFileName+(8+8)*m_voiceFlagNumber+12);
		*ptr = m_voiceFlagNumber;

		m_voiceFlagNumber++;
	}

}

void CScriptCompiler::ClearVoiceFlag(void)
{
	m_voiceFlagNumber = 0;
}

//for sort
int compare_int(const void* data1,const void* data2)
{
	char* mes1 = (char*)data1;
	char* mes2 = (char*)data2;
	return _strcmpi(mes1,mes2);
}

void CScriptCompiler::SortAndOutputVoiceFlagFilename(void)
{
	//sort
	if (m_voiceFlagNumber >= 2)
	{
		qsort(m_voiceFlagFileName,m_voiceFlagNumber,16,compare_int);
	}

	//check same
	int st = 0;
	for (int i=1;i<m_voiceFlagNumber;i++)
	{
		char* name0 = m_voiceFlagFileName+st*(8+8);
		char* name1 = m_voiceFlagFileName+i*(8+8);
		if (_stricmp(name0,name1) == 0)
		{
			short* ptr = (short*)(m_voiceFlagFileName+i*(8+8) + 8+2);
			*ptr = i - st;
		}
		else
		{
			st = i;
		}
	}



	//output
	FILE* file = CMyFile::Open("spt\\voiceflag.tbl","wb");

	fwrite("VOICEFLAGANDNAME",sizeof(char),16,file);
	fwrite(&m_voiceFlagNumber,sizeof(int),1,file);
	int zero = 0;
	fwrite(&zero,sizeof(int),1,file);
	fwrite(&zero,sizeof(int),1,file);
	fwrite(&zero,sizeof(int),1,file);

	fwrite(m_voiceFlagFileName,sizeof(char),m_voiceFlagNumber*(8+8),file);
	
	fclose(file);
}




/*_*/
