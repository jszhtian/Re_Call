//
// gamemessagedoc.cpp
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nyanLib\include\myFile.h"

#include "..\..\systemNNN\nyanlib\include\myGraphics.h"

#include "case.h"
//#include "..\nyanEffectLib\effect.h"
#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "windowlist.h"


#include "gamemessageview.h"

#include "gamemessagedoc.h"

#include "myapplicationBase.h"
#include "undoMemoryObject.h"

//#include "filmdata.h"
#include "komadata.h"
#include "messagedata.h"

#include "..\..\systemNNN\nnnUtilLib\namelist.h"

#include "myinputdialog.h"
#include "selectdialog.h"

char CGameMessageDoc::m_bunkatsu[16][64];

//
// ^ 音声ファイル名 チャンネル
// ~ レイヤー番号 エフェクト番号 エフェクトパラメーター
//
//


CGameMessageDoc::CGameMessageDoc(CMyApplicationBase* lpApp) : CMyDocument(lpApp)
{
	m_windowNumber = GAMEMESSAGE_WINDOW;
	m_hBitmapMessage = m_app->GetCommonBitmap(2);

	//default
	m_windowGyo = m_app->GetEditorMessageGyo();
	m_fullGyo = m_app->GetEditorMessageGyo();
	m_textWords = 20;

	m_textLimit = 32000;	//windows95,98
	m_messageBufferFlag = FALSE;

	if (m_app->CheckWinNT())
	{
		m_textLimit = 64000;
		int buf = m_app->GetConfig("inputTextLimitNT");
		if (buf > 0) m_textLimit = buf;
		if (m_textLimit < 32000) m_textLimit = 32000;
	}

	m_commentPrintMode[0] = m_app->GetConfig("commentTopPrintFlag");
	m_commentPrintMode[1] = m_app->GetConfig("commentBottomPrintFlag");



	m_view = new CGameMessageView(this,m_clientHWND,m_hInstance);
//	m_view->SetViewWindowText("メッセージ");
//	m_view->MoveViewWindow(147,681,640+GetSystemMetrics(SM_CXVSCROLL),24+24*4*2);
	m_view->InitWindowZahyo();

	m_input = new CMyInputDialog(m_app->GetFrameHWND(),m_hInstance,m_textLimit);
	m_select = new CSelectDialog(m_app->GetFrameHWND(),m_hInstance);

	for (int i=0;i<16;i++)
	{
		m_seEffectNameList[i] = m_seEffectName[i];
		m_voiceEffectNameList[i] = m_voiceEffectName[i];

	}
//	m_seEffectList = new CNameList(16);
}



CGameMessageDoc::~CGameMessageDoc()
{
	End();
}

void CGameMessageDoc::End(void)
{
//	ENDDELETECLASS(m_seEffectList);

	ENDDELETECLASS(m_select);
	ENDDELETECLASS(m_input);
}



HBITMAP CGameMessageDoc::GetMessagePic(void)
{
	return m_hBitmapMessage;
}

int CGameMessageDoc::GetMessageKosuu(void)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return 0;

	return pKoma->GetObjectKosuu();
}

int CGameMessageDoc::GetNowSelectNumber(void)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return 0;

	return pKoma->GetNowSelectNumber();
}


void CGameMessageDoc::OnNewMessage(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	int kosuu = pKoma->GetObjectKosuu();

	if (n == -1) n = pKoma->GetNowSelectNumber();
	if ((n<0) || (n>kosuu)) return;

	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_MESSAGE,UNDO_DATA_INSERT,n,n);
	}


	pKoma->CreateObjectData(n);

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	pMessage->Init();

	((CGameMessageView*)m_view)->ReCalcuScrollPara();

	m_app->SetModify();
	m_app->MessageIsChanged();

	m_view->MyInvalidateRect();

}

void CGameMessageDoc::OnCut(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	int kosuu = pKoma->GetObjectKosuu();
	if (n == -1) n = pKoma->GetNowSelectNumber();
	if ((n<0) || (n>=kosuu)) return;

	if (m_app->GetWarningDelMessage())
	{
		int rt = MessageBox(m_frameHWND,"本当に削除しますか","削除の確認",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);
		if (rt != IDYES) return;
	}

	int start = pKoma->GetSelectStart();
	int end = pKoma->GetSelectEnd();

	if ((start<0) || (start>=kosuu)) return;
	if ((end<0) || (end>=kosuu)) return;



	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_MESSAGE,UNDO_DATA_DELETE,start,end);
		for (int i=start;i<=end;i++)
		{
			CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(i));
			pMessage->Save(NULL,undo);
		}
	}

	BOOL errorFlag = FALSE;

	for (int i=start;i<=end;i++)
	{
		char filename[1024];
		wsprintf(filename,"nnndir\\tmp\\message%d.msg",i-start+1);
		FILE* file = CMyFile::Open(filename,"wb");
		if (file != NULL)
		{
			CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(i));
			if (pMessage != NULL)
			{
				pMessage->Save(file);
			}
			else
			{
				MessageBox(NULL,"SAVE KOMA ERROR","ERROR",MB_OK);
				errorFlag = TRUE;
			}

			fclose(file);
		}
		else
		{
			MessageBox(NULL,"SAVE KOMA ERROR","ERROR",MB_OK);
			errorFlag = TRUE;
		}
	}	

	FILE* file = CMyFile::Open("nnndir\\tmp\\messagenum.dat","wb");
	if (file != NULL)
	{
		int messageKosuu = end - start + 1;
		if (errorFlag) messageKosuu = 0;
		fwrite(&messageKosuu,sizeof(int),1,file);
		fclose(file);
	}


	m_messageBufferFlag = !errorFlag;

	OnDelete(n);
}


void CGameMessageDoc::OnCopy(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	int kosuu = pKoma->GetObjectKosuu();
	if (n == -1) n = pKoma->GetNowSelectNumber();
	if ((n<0) || (n>=kosuu)) return;

//	if (m_app->GetWarningDelMessage())
//	{
//		int rt = MessageBox(m_frameHWND,"本当に削除しますか","削除の確認",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);
//		if (rt != IDYES) return;
//	}

	int start = pKoma->GetSelectStart();
	int end = pKoma->GetSelectEnd();

	if ((start<0) || (start>=kosuu)) return;
	if ((end<0) || (end>=kosuu)) return;

	BOOL errorFlag = FALSE;

	for (int i=start;i<=end;i++)
	{
		char filename[1024];
		wsprintf(filename,"nnndir\\tmp\\message%d.msg",i-start+1);
		FILE* file = CMyFile::Open(filename,"wb");
		if (file != NULL)
		{
			CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(i));
			if (pMessage != NULL)
			{
				pMessage->Save(file);
			}
			else
			{
				MessageBox(NULL,"SAVE KOMA ERROR","ERROR",MB_OK);
				errorFlag = TRUE;
			}

			fclose(file);
		}
		else
		{
			MessageBox(NULL,"SAVE KOMA ERROR","ERROR",MB_OK);
			errorFlag = TRUE;
		}
	}	

	FILE* file = CMyFile::Open("nnndir\\tmp\\messagenum.dat","wb");
	if (file != NULL)
	{
		int messageKosuu = end - start + 1;
		if (errorFlag) messageKosuu = 0;
		fwrite(&messageKosuu,sizeof(int),1,file);
		fclose(file);
	}


	m_messageBufferFlag = !errorFlag;

	m_app->SetModify();
	m_app->MessageIsChanged();
}

void CGameMessageDoc::OnPaste(int n)
{
	if (m_messageBufferFlag == FALSE) return;

	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	int kosuu = pKoma->GetObjectKosuu();
	if (n == -1) n = pKoma->GetNowSelectNumber();
	if ((n<0) || (n>kosuu)) return;

	int messageKosuu = 0;
	FILE* file = CMyFile::Open("nnndir\\tmp\\messagenum.dat","rb");
	if (file == NULL) return;
	fread(&messageKosuu,sizeof(int),1,file);
	fclose(file);

	if (messageKosuu<=0) return;


	int insertNumber = n;


	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_MESSAGE,UNDO_DATA_INSERT,n,n+messageKosuu-1);
	}




	for (int i=0;i<messageKosuu;i++)
	{
		pKoma->CreateObjectData(insertNumber+i);

		CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(insertNumber+i));
		if (pMessage != NULL)
		{
			char filename[1024];
			wsprintf(filename,"nnndir\\tmp\\message%d.msg",i+1);
			FILE* file2 = CMyFile::Open(filename,"rb");
			if (file2 != NULL)
			{
				pMessage->Load(file2);
				fclose(file2);
			}
			else
			{
				MessageBox(NULL,"LOAD MESSAGE ERROR","ERROR",MB_OK);
			}
		}
	}

	((CGameMessageView*)m_view)->ReCalcuScrollPara();

	m_app->SetModify();
	m_app->MessageIsChanged();
}



void CGameMessageDoc::OnDelete(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	int kosuu = pKoma->GetObjectKosuu();

	if (n == -1) n = pKoma->GetNowSelectNumber();
	if ((n<0) || (n>=kosuu)) return;

	if (m_app->GetWarningDelMessage())
	{
		int rt = MessageBox(m_frameHWND,"本当に削除しますか","削除の確認",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);
		if (rt != IDYES) return;
	}




	int start = pKoma->GetSelectStart();
	int end = pKoma->GetSelectEnd();

	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_MESSAGE,UNDO_DATA_DELETE,start,end);
		for (int i=start;i<=end;i++)
		{
			CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(i));
			pMessage->Save(NULL,undo);
		}
	}



	for (int i=end;i>=start;i--)
	{
		pKoma->DeleteObjectData(i);
	}

	pKoma->SetSelectNumber(start);
	pKoma->SetSelectSubNumber(-1);




	((CGameMessageView*)m_view)->ReCalcuScrollPara();

	m_app->SetModify();
	m_app->MessageIsChanged();
}

void CGameMessageDoc::CheckAndGetUndo(CKomaData* pKoma,int start,int end)
{
	if (pKoma == NULL) return;

	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear(UNDO_TYPE_MESSAGE,UNDO_DATA_MODIFY,start,end);
		for (int i=start;i<=end;i++)
		{
			CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(i));
			if (pMessage != NULL)
			{
				pMessage->Save(NULL,undo);
			}
		}
	}
}

void CGameMessageDoc::OnWindowOnOff(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	int kosuu = pKoma->GetObjectKosuu();

	if (n == -1) n = pKoma->GetNowSelectNumber();
	if ((n<0) || (n>=kosuu)) return;

	CheckAndGetUndo(pKoma,n,n);


	BOOL b = pKoma->GetWindowOffFlag();
	if (b == TRUE)
	{
		b = FALSE;
	}
	else
	{
		b = TRUE;
	}
	pKoma->SetWindowOffFlag(b);



	((CGameMessageView*)m_view)->ReCalcuScrollPara();

	m_app->SetModify();
	m_app->MessageIsChanged();
}



void CGameMessageDoc::OnSelectMessage(int n,WPARAM wParam)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	ClearUndo();
	//CheckAndGetUndo(pKoma,n,n);

	if (wParam & (MK_CONTROL | MK_SHIFT))
	{
		pKoma->SetSelectSubNumber(n);
	}
	else
	{
		pKoma->SetSelectNumber(n);
	}


	m_app->MessageIsChanged();
}


void CGameMessageDoc::OnChangeMessageType(int n, int typ)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	CheckAndGetUndo(pKoma,n,n);

	pMessage->SetMessageMode(typ);

	if (typ == 3)
	{
		int d = 0;
		if (m_input->GetNumber(pMessage->GetDrawCount(),&d))
		{
			if (d<=0) d = 1;
			if (d>9999) d = 9999;

			pMessage->SetDrawCount(d);
		}
	}

	m_app->SetModify();
	m_app->MessageIsChanged();
}


void CGameMessageDoc::OnClickModeButton(int n)
{
	//内容変更
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	CheckAndGetUndo(pKoma,n,n);


	BOOL nextKomaFlag = FALSE;
	BOOL komaChangeFlag = FALSE;

	int screenSizeX = CMyGraphics::GetScreenSizeX();
	int screenSizeY = CMyGraphics::GetScreenSizeY();

	BOOL picChangeFlag = FALSE;


	int defaultLoadEffectType = m_app->GetConfig("loadEffectType");

	BOOL nagashikomiYoyaku = FALSE;
	int nagashikomiVoiceChannel = 0;

	char nagashikomiVoice[256];

	int typ = pMessage->GetMessageMode();
	if ((typ == 0) || (typ == 1) || (typ == 2))
	{
		LPSTR oldText = pMessage->GetMessageData();
		LPSTR newText = m_input->GetText2(oldText);

		BOOL autoFlag = m_input->GetAutoFlag();
		BOOL fullFlag = m_input->GetFullFlag();
		BOOL autoCRFlag = m_input->GetAutoCRFlag();


		if (newText != NULL)
		{
			BOOL b = FALSE;


			int defaultBGLayer = m_app->GetConfig("defaultBGLayer");
			int defaultTALayer = m_app->GetConfig("defaultTALayer");

			int bgLayer = defaultBGLayer;
			int taLayer = defaultTALayer;

			int lastBGLayer = bgLayer;
			int lastTALayer = taLayer;

			int effectLayer = lastBGLayer;

			BOOL dataExistFlag = FALSE;
			int defaultInputMode = typ;
			if (fullFlag) defaultInputMode = 1;

			int nextInputMode = defaultInputMode;
//			int nowInputMode = nextInputMode;

			BOOL overrapFlag = FALSE;
			int overrapType = 0;
			int overrapTime = 5;

			int drawCount = 5;

			while (newText != NULL)
			{
				BOOL lastFlag = FALSE;

				int ln = strlen(newText);

				LPSTR nextText = newText;

				char newText2[32100];
				newText2[0] = 0;
				int newTextSize = 0;

				BOOL flg = TRUE;
				int gyo = 0;



				int nowInputMode = defaultInputMode;
				if (nextInputMode != -1)
				{
					nowInputMode = nextInputMode;
					nextInputMode = -1;
				}


				while (TRUE)
				{
					char* oldPtr = nextText;

					nextText = strstr(nextText,"\r\n");

					if (nextText ==NULL)
					{
						nextText = oldPtr + strlen(oldPtr);
						lastFlag = TRUE;
					}
					else
					{
						if (autoCRFlag == FALSE)
						{
							nextText += 2;
						}
						else
						{
							//強制改行チェック




							nextText += 2;
						}
					}


					char chk = *oldPtr;

//					if (((*oldPtr) == '@') || ((*oldPtr) == '%'))
					if ((chk == '@') || (chk == '%') || (chk == '&') || (chk == '!') || (chk== '^') || (chk=='~'))
					{


						int change = -1;
						char* oldPtr2 = oldPtr;
						oldPtr2++;
						char subc = *oldPtr2;

						if ((chk == '@') || (chk == '%'))
						{
							if (subc == 'N') change = 0;
							if (subc == 'L') change = 1;
							if (subc == 'A') change = 2;
						}

						if (chk == '@')
						{
							if (subc == 'D')
							{
								change = 3;

								int cmdKosuu = Bunkatsu(oldPtr2+1);

								drawCount = 5;
								if (cmdKosuu > 0)
								{
									drawCount = atoi(m_bunkatsu[0]);
								}
							}
						}

						if (change != -1)
						{
							oldPtr2++;

							if (dataExistFlag)
							{
								nextInputMode = change;
							}
							else
							{
								nowInputMode = change;
							}
						}


//						if ((*oldPtr) == '@')
						if (chk == '@')
						{
							if (change != 3)
							{
								if (dataExistFlag)
								{
									break;
								}
							}
							else
							{
								if (pMessage == NULL)
								{
									//create
									n++;
									pKoma->CreateObjectData(n);
									pMessage = (CMessageData*)(pKoma->GetObjectData(n));
									b = TRUE;
								}

								pMessage->SetMessageMode(change);
								pMessage->SetDrawCount(drawCount);


								m_app->SetModify();
								m_app->MessageIsChanged();
								break;
							}
						}
//						else if ((*oldPtr) == '%')
						else if (chk == '%')
						{
							//overrap
							int cmdKosuu = Bunkatsu(oldPtr2);

							overrapTime = 0;
							if (m_app->GetConfig("autoSetOverrapFlag"))
							{
								overrapTime = m_app->GetConfig("defaultOverrapTime");
							}

							overrapType = 0;

							if (cmdKosuu > 0)
							{
								overrapTime = atoi(m_bunkatsu[0]);

								if (cmdKosuu>1)
								{
									overrapType = atoi(m_bunkatsu[1]);
								}
							}

							if (overrapTime > 0)
							{
								overrapFlag = TRUE;
							}


							bgLayer = defaultBGLayer;
							taLayer = defaultTALayer;

							lastBGLayer = bgLayer;
							lastTALayer = taLayer;

							effectLayer = lastBGLayer;

							nextKomaFlag = TRUE;
							komaChangeFlag = TRUE;
							break;
						}
						else if (chk == '&')
						{
							picChangeFlag = TRUE;

							// set picture sub command
							int cmdKosuu = Bunkatsu(oldPtr2);

							if (cmdKosuu>0)
							{
								LPSTR picFileName = m_bunkatsu[0];

								char tag[3];
								memcpy(tag,picFileName,2);
								tag[2]=0;
								BOOL taFlag = FALSE;
								if (_stricmp(tag,"ta") == 0)
								{
									taFlag = TRUE;
								}

								int layer = bgLayer;
								if (taFlag) layer = taLayer;

								int sizeX = screenSizeX;
								int sizeY = screenSizeY;

								if (cmdKosuu>1)
								{
									layer = atoi(m_bunkatsu[1]);

									if (layer >= 0)
									{
										if (taFlag)
										{
											taLayer = layer;
										}
										else
										{
											bgLayer = layer;
										}
									}
								}

								if (layer<0)
								{
									if (taFlag)
									{
										taLayer = defaultTALayer;
										layer = taLayer;
									}
									else
									{
										bgLayer = defaultBGLayer;
										layer = bgLayer;
									}
								}

								int setx = 0;
								if (cmdKosuu>2)
								{
									setx = atoi(m_bunkatsu[2]);
								}
								int sety = 0;
								if (cmdKosuu>3)
								{
									sety = atoi(m_bunkatsu[3]);
								}

								if (cmdKosuu>4)
								{
									sizeX = atoi(m_bunkatsu[4]);
								}
								if (cmdKosuu>5)
								{
									sizeY = atoi(m_bunkatsu[5]);
								}

								int setx2 = setx;
								int sety2 = sety;
								int sizeX2 = sizeX;
								int sizeY2 = sizeY;


								if (cmdKosuu > 9)
								{
									setx2 = atoi(m_bunkatsu[6]);
									sety2 = atoi(m_bunkatsu[7]);
									sizeX2 = atoi(m_bunkatsu[8]);
									sizeY2 = atoi(m_bunkatsu[9]);
								}

								int layerError = 0;

								if (taLayer < 0)
								{
									taLayer = 0;
									layerError = 1;
								}

								if (taLayer > 15)
								{
									layerError = 2;
									taLayer = 15;
								}

								if (bgLayer < 0)
								{
									layerError = 3;
									bgLayer = 0;
								}

								if (bgLayer > 15)
								{
									layerError = 4;
									bgLayer = 15;
								}

								if (layer < 0)
								{
									layer = 0;
									layerError = 5;
								}

								if (layer > 15)
								{
									layerError = 6;
									layer = 15;
								}


								if (layerError != 0)
								{
									MessageBox(NULL,"絵のレイヤー番号が不正です","ERROR",MB_OK | MB_ICONEXCLAMATION);
								}

								pKoma->SetPicFileName(layer,picFileName);
								RECT rc;
								RECT rc2;
								SetRect(&rc,setx,sety,sizeX,sizeY);
								SetRect(&rc2,0,0,sizeX,sizeY);
								RECT rc3 = rc;


								if (cmdKosuu > 9)
								{
									SetRect(&rc3,setx2,sety2,sizeX2,sizeY2);
									pKoma->SetEffectRect(&rc3,layer,0x03);
									pKoma->SetEffectRect(&rc,layer,0x04);
									pKoma->SetEffectRect(&rc2,layer,0x38);
								}
								else
								{
									pKoma->SetEffectRect(&rc,layer,0x07);
									pKoma->SetEffectRect(&rc2,layer,0x38);
								}

								effectLayer = layer;

								if (taFlag)
								{
									lastTALayer = taLayer;
									taLayer++;
									if (taLayer > 15) taLayer = 4;
								}
								else
								{
									lastBGLayer = bgLayer;
									bgLayer++;
									if (bgLayer > 3) bgLayer = 0;
								}
							}
						}
						else if (chk == '!')
						{
							picChangeFlag = TRUE;
							int effectType = 0;
							char subsubc = *oldPtr2;
							if (subsubc == '!')//仮
							{
								effectType = 1;
								oldPtr2++;
							}

							int cmdKosuu = Bunkatsu(oldPtr2);

							if (cmdKosuu>0)
							{
								LPSTR effectFileName = m_bunkatsu[0];
								int effectLoadType = defaultLoadEffectType;

								if (effectType == 1)	//all
								{
									if (cmdKosuu > 1)
									{
										effectLoadType = atoi(m_bunkatsu[1]);
									}
									pKoma->LoadEffectByFileName(effectFileName,effectLoadType);

								}
								else if (effectType == 0)	//one layer
								{
									if (cmdKosuu > 1)
									{
										int eLayer = atoi(m_bunkatsu[1]);
										if ((eLayer >= 0) && (eLayer <= 15))
										{
											effectLayer = eLayer;
										}
									}

									if (cmdKosuu > 2)
									{


										effectLoadType = atoi(m_bunkatsu[2]);
									}

									pKoma->LoadLayerEffectByFileName(effectLayer,effectFileName,effectLoadType);
								}

							}
						}
						else if (chk == '^')
						{
							//if (pMessage != NULL)
							if (1)
							{
								int cmdKosuu = Bunkatsu(oldPtr2);

								if (cmdKosuu>0)
								{
									nagashikomiYoyaku = TRUE;

									LPSTR voicename = m_bunkatsu[0];
									int vc = 0;
									if (cmdKosuu > 1)
									{
										vc = atoi(m_bunkatsu[1]);
									}

									int ln = strlen(voicename);
									if (ln>254) ln = 254;
									memcpy(nagashikomiVoice,voicename,strlen(voicename));
									nagashikomiVoice[ln] = 0;
									nagashikomiVoice[ln+1] = 0;
									nagashikomiVoiceChannel = vc;

OutputDebugString("\nbbbbbb");

//									pMessage->SetVoiceFileName(voicename,vc);

//									m_app->SetModify();
//									m_app->MessageIsChanged();
								}

								break;
							}

						}
						else if (chk == '~')
						{
							int cmdKosuu = Bunkatsu(oldPtr2);
							if (cmdKosuu > 2)
							{
								int commandEffectLayer = atoi(m_bunkatsu[0]);
								int commandEffectEffect = atoi(m_bunkatsu[1]);
								if (pKoma != NULL)
								{
									if (commandEffectLayer == -1)
									{
										int overrapTypeTmp = atoi(m_bunkatsu[1]);
										int overrapTimeTmp = atoi(m_bunkatsu[2]);
										pKoma->SetOverrapType(overrapTypeTmp);
										pKoma->SetOverrapCount(overrapTimeTmp);

									}
									else
									{
										pKoma->SetEffect(commandEffectLayer,commandEffectEffect);
										pKoma->SetEffectParaKosuu(commandEffectLayer,cmdKosuu-2);
										for (int ec = 2;ec<cmdKosuu;ec++)
										{
											int commandEffectParam = atoi(m_bunkatsu[ec]);
											pKoma->SetEffectPara(commandEffectLayer,ec-2,commandEffectParam);
										}
									}
								}

							}
						}
					}
					else
					{
						if ((*oldPtr) == 0)
						{
							break;
						}

						//copy
						int ln2 = nextText - oldPtr;
						if ((ln+newTextSize) >= m_textLimit)
						{
							MessageBox(NULL,"textが長すぎます","ERROR",MB_OK);
							break;
						}

						memcpy(newText2+newTextSize,oldPtr,ln2);
						newTextSize += ln2;

						if (((*oldPtr) != '/') || ((*(oldPtr+1)) != '/'))
						{
							dataExistFlag = TRUE;
							gyo++;
							if (nowInputMode == 0)
							{
								if (gyo >= m_windowGyo) break;
							}
							else if ((nowInputMode == 1) || (nowInputMode == 2))
							{
								if (gyo >= m_fullGyo) break;
							}
						}


						if (lastFlag) break;
					}

				}

				newText2[newTextSize] = 0;

//				if (nextText != NULL)
//				{
//					if (*(nextText) == 0) nextText = NULL;
//				}


//				if (nextText != NULL)
//				{
//					ln = nextText - newText;
//				}
//
//				if (ln<=0) break;


				//cut \r\n\r\n\0 -> \r\n\0
				for (int ii=0;ii<4;ii++)
				{
					if (newTextSize>=4)
					{
						if ( (*(newText2+newTextSize-4) == '\r') && (*(newText2+newTextSize-4+1) == '\n') && (*(newText2+newTextSize-4+2) == '\r') && (*(newText2+newTextSize-4+3) == '\n'))
						{
							newTextSize -= 2;
						}
					}
				}

				if (gyo>0)
				{
					if (newTextSize>0)
					{
						if ((*newText2) != 0)
						{
							if (pMessage == NULL)
							{
								//create
								n++;
								pKoma->CreateObjectData(n);
								pMessage = (CMessageData*)(pKoma->GetObjectData(n));
								b = TRUE;
							}

							pMessage->SetMessageMode(nowInputMode);
							pMessage->SetMessageData(newText2,newTextSize);

							if (nagashikomiYoyaku)
							{
OutputDebugString("\naaaaaa");
								pMessage->SetVoiceFlag(TRUE,nagashikomiVoiceChannel);
								pMessage->SetVoiceFileName(nagashikomiVoice,nagashikomiVoiceChannel);
								nagashikomiYoyaku = FALSE;
							}

						}
					}

				}

				pMessage = NULL;

				if (nextKomaFlag)
				{
					m_app->CreateNextKoma();

					pKoma = GetNowSelectKoma();
					n = pKoma->GetNowSelectNumber();
					kosuu = pKoma->GetObjectKosuu();

					nextKomaFlag = FALSE;

					pMessage = (CMessageData*)(pKoma->GetNowSelectObject());

					if (overrapFlag)
					{
						pKoma->SetOverrapType(overrapType);
						pKoma->SetOverrapCount(overrapTime);
						overrapFlag = FALSE;
					}
				}



				//make message
				newText = nextText;

				if (nextText != NULL)
				{
					if ((autoFlag == FALSE) && ((*nextText) != 0))
					{
						newText = m_input->GetText2(nextText);


						autoFlag = m_input->GetAutoFlag();
						fullFlag = m_input->GetFullFlag();
						autoCRFlag = m_input->GetAutoCRFlag();

					}
					else
					{
						newText = nextText;
					}

					if (newText != NULL)
					{
						if ((*newText) == 0)
						{
							newText = NULL;
						}
					}
				}
			}

			if (b)
			{
				//resize window

				((CGameMessageView*)m_view)->ReCalcuScrollPara();

			}

			m_app->SetModify();
			m_app->MessageIsChanged();
			if (komaChangeFlag)
			{
				m_app->KomaIsChanged();
			}
		}
	}
	else if (typ == 3)
	{
		int d = 0;
		if (m_input->GetNumber(pMessage->GetDrawCount(),&d))
		{
			if (d<=0) d = 1;
			if (d>9999) d = 9999;

			pMessage->SetDrawCount(d);

			m_app->SetModify();
			m_app->MessageIsChanged();
		}
/*
		char nm[16];
		sprintf(nm,"%d",pMessage->GetDrawCount());
		LPSTR newText = m_input->GetText(nm,TRUE);
		if (newText != NULL)
		{
			int d = 0;
			int ln = strlen(newText);
			for (int i=0;i<ln;i++)
			{
				int c = (int)(*(newText+i));
				c &= 0xff;

				c -= '0';
				if ((c<0) || (c>9)) break;

				d *= 10;
				d += c;
			}

			if (d<0) d = 1;
			if (d>9999) d = 9999;

			pMessage->SetDrawCount(d);

			m_app->SetModify();
			m_app->MessageIsChanged();
		}
		*/
	}

	//reload all pic
	if (pKoma != NULL)
	{
		if (picChangeFlag)
		{
			pKoma->ReloadAllPic();

			pKoma->SetAllEffect();

			m_app->LayerIsChanged();
			pKoma->SetModifyFlag();
			m_app->KomaIsChanged();

//			m_app->FilmIsChanged();
//			m_app->KomaIsChanged();
//			m_app->ZahyoIsChanged();
//			m_app->UpdateMyWindow(LAYER_WINDOW);
//			m_app->UpdateMyWindow(EFFECT_WINDOW);
//			m_app->UpdateMyWindow(FILM_WINDOW);
//			m_app->UpdateMyWindow(ZAHYO_WINDOW);
		}
	}

}


//max16ko 1komax=64byte
int CGameMessageDoc::Bunkatsu(LPSTR mes)
{
	int kosuu = 0;

	BOOL found = FALSE;
	int foundN = 0;
	int last = 0;

	for (int i=0;i<16*64;i++)
	{
		char c = *(mes+i);

		if ((c == 0xd) || (c == 0))
		{
			last = i;
			break;
		}

		if ((c == ' ') || (c == ','))
		{
			if (found)
			{
				int ln = i - foundN;
				if (ln>0)
				{
					if (ln>62) ln = 62;
					memcpy(m_bunkatsu[kosuu],mes+foundN,ln);
					m_bunkatsu[kosuu][ln] = 0;
					m_bunkatsu[kosuu][ln+1] = 0;
					kosuu++;
				}

				found = FALSE;
			}
		}
		else
		{
			if (found == FALSE)
			{
				found = TRUE;
				foundN = i;
			}
		}
	}

	if (found)
	{
		int ln = last - foundN;
		if (ln>0)
		{
			if (ln>62) ln = 62;
			memcpy(m_bunkatsu[kosuu],mes+foundN,ln);
			m_bunkatsu[kosuu][ln] = 0;
			m_bunkatsu[kosuu][ln+1] = 0;
			kosuu++;
		}
	}



	return kosuu;
}


void CGameMessageDoc::OnClickVoiceButton(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	BOOL flg = pMessage->GetVoiceFlag();
	if (flg == FALSE) return;

	if (pMessage->CheckVoiceFlag(channel) == FALSE) return;

	m_app->PlayScriptMessageVoice(pMessage,channel);

	int oldChannel = pMessage->GetVoiceChannel();
	if (channel != oldChannel)
	{
		pMessage->SetVoiceChannel(channel);
		UpdateMyWindow();
	}
}

void CGameMessageDoc::OnClickSeButton(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	BOOL flg = pMessage->GetSEFlag();
	if (flg == FALSE) return;

	if (pMessage->CheckSEFlag(channel) == FALSE) return;

	int se = pMessage->GetSENumber(channel);
	if (se<0) return;

	m_app->PlayScriptMessageSound(pMessage,channel);


	int oldChannel = pMessage->GetSeChannel();
	if (channel != oldChannel)
	{
		pMessage->SetSeChannel(channel);
		UpdateMyWindow();
	}
}


void CGameMessageDoc::ReCalcuScrollPara(void)
{
	((CGameMessageView*)m_view)->ReCalcuScrollPara();
}

LPSTR CGameMessageDoc::GetCharaName(int n)
{
	return m_app->GetCharaName(n);
}

LPSTR CGameMessageDoc::GetSEName(int n)
{
	return m_app->GetSEName(n);
}

LPSTR CGameMessageDoc::GetSEFileName(int n)
{
	return m_app->GetSEFileName(n);
}



void CGameMessageDoc::OnChangeVoicePlayer(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();


	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	if (pMessage->CheckVoiceLock(channel)) return;


	BOOL flg = pMessage->GetVoiceFlag();
	int pl = 0;
	if (flg) pl = pMessage->GetVoicePlayerNumber(channel);


//	pMessage->SetVoicePlayerNumber(0);
//	pMessage->SetVoiceNumber(0);

	CNameList* namelist = m_app->GetCharaList();


	int rt = m_select->GetSelect(namelist,pl);

	if (rt == -1) return;
	pMessage->SetVoiceChannel(channel);

	pMessage->SetVoiceFlag(TRUE,channel);
	pMessage->SetVoicePlayerNumber(rt,channel);
	pMessage->SetVoiceFileName("",channel);
	pMessage->SetVoiceStop(channel,FALSE);
	pMessage->SetRenameFlag(FALSE);


	m_app->SetModify();
	m_app->MessageIsChanged();

}


void CGameMessageDoc::OnVoiceVolume(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error
	if (n == -1) n = pKoma->GetNowSelectNumber();
	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;
	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;
	if (pMessage->CheckVoiceLock(channel)) return;

	if (pMessage->CheckVoiceFlag(channel) == FALSE) return;

	CheckAndGetUndo(pKoma,n,n);

	int vol = 0;
	if (pMessage->CheckVoiceFlag(channel))
	{
		if (pMessage->CheckVoiceVolumeExist(channel))
		{
			vol = pMessage->GetVoiceVolume(channel);
		}
	}
	int oldVolume = vol;

	if (m_input->GetNumber(oldVolume,&vol))
	{
		pMessage->SetVoiceChannel(channel);
		pMessage->SetVoiceVolume(vol,channel);
		m_app->SetModify();
		m_app->MessageIsChanged();
	}
}



void CGameMessageDoc::OnVoiceEffect(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error
	if (n == -1) n = pKoma->GetNowSelectNumber();
	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;
	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	if (pMessage->CheckVoiceFlag(channel) == FALSE) return;
	if (pMessage->CheckVoiceLock(channel)) return;


	CheckAndGetUndo(pKoma,n,n);

//	if (channel < 1)
	if ((channel & 1) == 0)
	{
		int pan = pMessage->GetVoiceTeii(0,channel);
		int oldPan = pan;

		if (m_input->GetNumber(oldPan,&pan))
		{
			if (pan<-100) pan = -100;
			if (pan>100) pan = 100;

			if (pan != oldPan)
			{
				pMessage->SetVoiceTeii(0,pan,channel);
				pMessage->SetVoiceChannel(channel);

				m_app->SetModify();
				m_app->MessageIsChanged();
				UpdateMyWindow();
			}
		}
	}
	else
	{
		//effect menu teii1,2,doppu,move

		int cmMode = 0;
		int cmKosuu = 3;

		int dop = 0;

		int xyz[3];
		int xyz2[3];

		BOOL editFlag = FALSE;

		int i;
		for (i=0;i<3;i++)
		{
			xyz[i] = 0;
			xyz2[i] = 0;
		}

		if (pMessage->CheckVoiceTeiiExist(channel) == FALSE)
		{
			wsprintf(m_voiceEffectName[0],"定位X(未設定)");
			wsprintf(m_voiceEffectName[1],"定位Y(未設定)");
			wsprintf(m_voiceEffectName[2],"定位Z(未設定)");
		}
		else
		{
			cmMode = 1;
			for (i=0;i<3;i++)
			{
				xyz[i] = pMessage->GetVoiceTeii(i,channel);
			}

			wsprintf(m_voiceEffectName[0],"定位X(%d)",xyz[0]);
			wsprintf(m_voiceEffectName[1],"定位Y(%d)",xyz[1]);
			wsprintf(m_voiceEffectName[2],"定位Z(%d)",xyz[2]);

			if (pMessage->CheckVoiceMoveFlag(channel) == FALSE)
			{
				cmKosuu = 4;
				wsprintf(m_voiceEffectName[3],"定位移動設定(なし)");
			}
			else
			{
				int tm = pMessage->GetVoiceMoveTime(channel);
				wsprintf(m_voiceEffectName[3],"定位移動設定(%d.%d秒)",tm / 10,tm % 10);

				cmKosuu = 8;

				if (pMessage->CheckVoiceTeii2Exist(channel) == FALSE)
				{
					wsprintf(m_voiceEffectName[4],"定位移動後X(未設定)");
					wsprintf(m_voiceEffectName[5],"定位移動後Y(未設定)");
					wsprintf(m_voiceEffectName[6],"定位移動後Z(未設定)");
				}
				else
				{
					for (i=0;i<3;i++)
					{
						xyz2[i] = pMessage->GetVoiceTeii2(i,channel);
					}

					wsprintf(m_voiceEffectName[4],"定位移動後X(%d)",xyz2[0]);
					wsprintf(m_voiceEffectName[5],"定位移動後Y(%d)",xyz2[1]);
					wsprintf(m_voiceEffectName[6],"定位移動後Z(%d)",xyz2[2]);
				}

				if (pMessage->CheckVoiceDopplerSoutou(channel) == FALSE)
				{
					dop = 0;
					wsprintf(m_voiceEffectName[7],"ドップラー効果設定(なし)");
				}
				else
				{
					dop = 1;
					wsprintf(m_voiceEffectName[7],"ドップラー効果設定(あり)");
				}
			}
		}


		int n = m_select->GetSelect(m_voiceEffectNameList,cmKosuu);
		if (n >= 0)
		{
			if ((n>=0) && (n<=2))
			{
				int newxyz = xyz[n];
				if (m_input->GetNumber(xyz[n],&newxyz))
				{
					pMessage->SetVoiceTeii(n,newxyz,channel);
					editFlag = TRUE;
				}
			}
			else if (n==3)	//時間
			{
				int oldTime = 0;
				if (pMessage->CheckVoiceMoveFlag(channel))
				{
					oldTime = pMessage->GetVoiceMoveTime(channel);
				}

				int newTime = oldTime;

				if (m_input->GetNumber(oldTime,&newTime))
				{
					if (newTime >= 0)
					{
						pMessage->SetVoiceMoveTime(newTime,channel);
						editFlag = TRUE;
					}
				}
			}
			else if ((n >=4) && (n<=6))
			{
				int newxyz2 = xyz2[n-4];
				if (m_input->GetNumber(xyz2[n-4],&newxyz2))
				{
					pMessage->SetVoiceTeii2(n-4,newxyz2,channel);
					editFlag = TRUE;
				}
			}
			else if (n == 7)
			{
				if (dop == 0)
				{
					pMessage->SetVoiceDopplerSoutou(TRUE,channel);
				}
				else
				{
					pMessage->SetVoiceDopplerSoutou(FALSE,channel);
				}
				editFlag = TRUE;
			}

			//check all effect clear?

			pMessage->CheckAllVoiceEffectClear(channel);
		}


		if (editFlag)
		{
			pMessage->SetVoiceChannel(channel);

			m_app->SetModify();
			m_app->MessageIsChanged();
			UpdateMyWindow();
		}
	}
}




void CGameMessageDoc::OnChangeVoiceFileName(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();


	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	if (pMessage->CheckVoiceLock(channel)) return;

//	BOOL flg = pMessage->GetVoiceFlag();



	LPSTR oldText = NULL;

	if (pMessage->CheckVoiceFlag(channel))
	{
		oldText = pMessage->GetVoiceFileName(channel);
	}

	LPSTR newText = m_input->GetText(oldText);

	if (newText == NULL) return;


	CheckAndGetUndo(pKoma,n,n);

	pMessage->SetVoiceFlag(TRUE,channel);
	//pMessage->SetVoicePlayerNumber(0,channel);
	pMessage->SetVoiceNumber(0,channel);
	pMessage->SetVoiceStop(channel,FALSE);
	pMessage->SetVoiceFileName(newText,channel);
	pMessage->SetRenameFlag(FALSE);

	pMessage->SetVoiceChannel(channel);

	if (channel >= 2)
	{
		pMessage->SetVoiceLoop(TRUE,channel);
	}

	m_app->SetModify();
	m_app->MessageIsChanged();
}


void CGameMessageDoc::PlaySeTest(int n)
{
	m_app->PlaySeTest(n,m_testSeChannel);
}



void CGameMessageDoc::OnChangeSeNumber(int n,int seChannel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;


	CheckAndGetUndo(pKoma,n,n);


	m_testSeChannel = seChannel;


	int nm = 0;
	if (pMessage->CheckSEFlag(seChannel))
	{
		nm = pMessage->GetSENumber(seChannel);
	}

	int rt = m_select->GetSelect(m_app->GetSEList(),nm,this);

	if (rt == 0)
	{
		pMessage->SetSeStop(FALSE,seChannel);
		pMessage->SetSENumber(rt,seChannel);
		pMessage->SetSEFlag(FALSE,seChannel);
		pMessage->SetSeChannel(seChannel);
		m_app->SetModify();
		m_app->MessageIsChanged();
	}
	else if (rt != -1)
	{
		pMessage->SetSeStop(FALSE,seChannel);
		pMessage->SetSEFlag(TRUE,seChannel);
		pMessage->SetSENumber(rt,seChannel);
		pMessage->SetSeFileName(m_app->GetSEFileName(rt),seChannel);
		pMessage->SetSeChannel(seChannel);

		m_app->SetModify();
		m_app->MessageIsChanged();
	}
}

void CGameMessageDoc::OnDeleteVoice(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	if (pMessage->CheckVoiceLock(channel)) return;

	CheckAndGetUndo(pKoma,n,n);

	pMessage->SetVoiceFlag(FALSE,channel);

	m_app->SetModify();
	m_app->MessageIsChanged();
}

void CGameMessageDoc::OnVoiceStop(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error
	if (n == -1) n = pKoma->GetNowSelectNumber();
	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;
	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;
	if (pMessage->CheckVoiceLock(channel)) return;

	if (pMessage->CheckVoiceFlag(channel)) return;

	CheckAndGetUndo(pKoma,n,n);

	pMessage->ClearAllVoiceEffect(channel);
	pMessage->SetVoiceStop(channel,TRUE);
//	pMessage->SetVoiceChannel(channel);

	m_app->SetModify();
	m_app->MessageIsChanged();
}

void CGameMessageDoc::OnVoiceLoop(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error
	if (n == -1) n = pKoma->GetNowSelectNumber();
	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;
	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;
	if (pMessage->CheckVoiceLock(channel)) return;

	if (pMessage->CheckVoiceFlag(channel) == FALSE) return;

	CheckAndGetUndo(pKoma,n,n);

	BOOL f = pMessage->CheckVoiceLoop(channel);
	if (f == FALSE)
	{
		pMessage->SetVoiceLoop(TRUE,channel);
	}
	else
	{
		pMessage->SetVoiceLoop(FALSE,channel);
		pMessage->CheckAllVoiceEffectClear(channel);
	}
	pMessage->SetVoiceChannel(channel);



	m_app->SetModify();
	m_app->MessageIsChanged();
}

void CGameMessageDoc::OnVoiceContinue(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error
	if (n == -1) n = pKoma->GetNowSelectNumber();
	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;
	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;
	if (pMessage->CheckVoiceLock(channel)) return;

	if (pMessage->CheckVoiceFlag(channel)) return;

	CheckAndGetUndo(pKoma,n,n);

	pMessage->ClearAllVoiceEffect(channel);
	pMessage->SetVoiceContinue(channel,TRUE);
//	pMessage->SetVoiceChannel(channel);

	m_app->SetModify();
	m_app->MessageIsChanged();
}

void CGameMessageDoc::OnVoiceComplete(int n, int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error
	if (n == -1) n = pKoma->GetNowSelectNumber();
	int kosuu = pKoma->GetObjectKosuu();
	if ((n < 0) || (n > kosuu)) return;
	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;
	if (pMessage->CheckVoiceLock(channel)) return;

	if (!pMessage->CheckVoiceFlag(channel)) return;

	CheckAndGetUndo(pKoma, n, n);

//	pMessage->ClearAllVoiceEffect(channel);
	pMessage->ChangeVoiceComplete(channel);
	//	pMessage->SetVoiceChannel(channel);

	m_app->SetModify();
	m_app->MessageIsChanged();
}

void CGameMessageDoc::OnVoiceNoWaitSameCharaVoice(int n, int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error
	if (n == -1) n = pKoma->GetNowSelectNumber();
	int kosuu = pKoma->GetObjectKosuu();
	if ((n < 0) || (n > kosuu)) return;
	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;
	if (pMessage->CheckVoiceLock(channel)) return;

	if (!pMessage->CheckVoiceFlag(channel)) return;

	CheckAndGetUndo(pKoma, n, n);

	//	pMessage->ClearAllVoiceEffect(channel);
	pMessage->ChangeVoiceNoWaitSameCharaVoice(channel);
	//	pMessage->SetVoiceChannel(channel);

	m_app->SetModify();
	m_app->MessageIsChanged();
}

void CGameMessageDoc::OnDeleteSe(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error
	if (n == -1) n = pKoma->GetNowSelectNumber();
	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;
	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	CheckAndGetUndo(pKoma,n,n);

	pMessage->SetSEFlag(FALSE,channel);

	m_app->SetModify();
	m_app->MessageIsChanged();
}


void CGameMessageDoc::OnSeStop(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error
	if (n == -1) n = pKoma->GetNowSelectNumber();
	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;
	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	if (pMessage->CheckSEFlag(channel)) return;

	CheckAndGetUndo(pKoma,n,n);

	pMessage->SetSeStop(TRUE,channel);
	pMessage->SetSeChannel(channel);

	m_app->SetModify();
	m_app->MessageIsChanged();
	UpdateMyWindow();
}

void CGameMessageDoc::OnSeSystem(int n, int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error
	if (n == -1) n = pKoma->GetNowSelectNumber();
	int kosuu = pKoma->GetObjectKosuu();
	if ((n < 0) || (n > kosuu)) return;
	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	//if (pMessage->CheckSEFlag(channel)) return;
	CheckAndGetUndo(pKoma, n, n);

	BOOL b = pMessage->CheckSEIsSystem(channel);
	pMessage->SetSEIsSystem(!b,channel);

	pMessage->SetSeChannel(channel);

	m_app->SetModify();
	m_app->MessageIsChanged();
	UpdateMyWindow();
}


void CGameMessageDoc::OnSeVolume(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error
	if (n == -1) n = pKoma->GetNowSelectNumber();
	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;
	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	if (pMessage->CheckSEFlag(channel) == FALSE) return;

	CheckAndGetUndo(pKoma,n,n);

	int vol = pMessage->GetSeVolume(channel);
	int oldVolume = vol;

	if (m_input->GetNumber(oldVolume,&vol))
	{
		if (vol != oldVolume)
		{
			if (vol<-100) vol = -100;
			if (vol>100) vol = 100;

			pMessage->SetSeVolume(vol,channel);
			pMessage->SetSeChannel(channel);

			m_app->SetModify();
			m_app->MessageIsChanged();
			UpdateMyWindow();
		}
	}
}

void CGameMessageDoc::OnSeLoop(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error
	if (n == -1) n = pKoma->GetNowSelectNumber();
	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;
	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	CheckAndGetUndo(pKoma,n,n);

	BOOL f = pMessage->CheckSeLoop(channel);
	if (f == FALSE)
	{
		pMessage->SetSeLoop(TRUE,channel);
	}
	else
	{
		pMessage->SetSeLoop(FALSE,channel);
		pMessage->CheckAllSeEffectClear(channel);
	}
	pMessage->SetSeChannel(channel);

	m_app->SetModify();
	m_app->MessageIsChanged();
	UpdateMyWindow();
}


void CGameMessageDoc::OnSeEffect(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error
	if (n == -1) n = pKoma->GetNowSelectNumber();
	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;
	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	if (pMessage->CheckSEFlag(channel) == FALSE) return;

	CheckAndGetUndo(pKoma,n,n);

	if (channel < 4)
	{
		int pan = pMessage->GetSeTeii(0,channel);
		int oldPan = pan;

		if (m_input->GetNumber(oldPan,&pan))
		{
			if (pan<-100) pan = -100;
			if (pan>100) pan = 100;

			if (pan != oldPan)
			{
				pMessage->SetSeTeii(0,pan,channel);

				m_app->SetModify();
				m_app->MessageIsChanged();
				UpdateMyWindow();
			}
		}
	}
	else
	{
		//effect menu teii1,2,doppu,move

		int cmMode = 0;
		int cmKosuu = 3;

		int dop = 0;

		int xyz[3];
		int xyz2[3];

		BOOL editFlag = FALSE;

		int i;
		for (i=0;i<3;i++)
		{
			xyz[i] = 0;
			xyz2[i] = 0;
		}

		if (pMessage->CheckSeTeiiExist(channel) == FALSE)
		{
			wsprintf(m_seEffectName[0],"定位X(未設定)");
			wsprintf(m_seEffectName[1],"定位Y(未設定)");
			wsprintf(m_seEffectName[2],"定位Z(未設定)");
		}
		else
		{
			cmMode = 1;
			for (i=0;i<3;i++)
			{
				xyz[i] = pMessage->GetSeTeii(i,channel);
			}

			wsprintf(m_seEffectName[0],"定位X(%d)",xyz[0]);
			wsprintf(m_seEffectName[1],"定位Y(%d)",xyz[1]);
			wsprintf(m_seEffectName[2],"定位Z(%d)",xyz[2]);

			if (pMessage->CheckSeMoveFlag(channel) == FALSE)
			{
				cmKosuu = 4;
				wsprintf(m_seEffectName[3],"定位移動設定(なし)");
			}
			else
			{
				int tm = pMessage->GetSeMoveTime(channel);
				wsprintf(m_seEffectName[3],"定位移動設定(%d.%d秒)",tm / 10,tm % 10);

				cmKosuu = 8;

				if (pMessage->CheckSeTeii2Exist(channel) == FALSE)
				{
					wsprintf(m_seEffectName[4],"定位移動後X(未設定)");
					wsprintf(m_seEffectName[5],"定位移動後Y(未設定)");
					wsprintf(m_seEffectName[6],"定位移動後Z(未設定)");
				}
				else
				{
					for (i=0;i<3;i++)
					{
						xyz2[i] = pMessage->GetSeTeii2(i,channel);
					}

					wsprintf(m_seEffectName[4],"定位移動後X(%d)",xyz2[0]);
					wsprintf(m_seEffectName[5],"定位移動後Y(%d)",xyz2[1]);
					wsprintf(m_seEffectName[6],"定位移動後Z(%d)",xyz2[2]);
				}

				if (pMessage->CheckDopplerSoutou(channel) == FALSE)
				{
					dop = 0;
					wsprintf(m_seEffectName[7],"ドップラー効果設定(なし)");
				}
				else
				{
					dop = 1;
					wsprintf(m_seEffectName[7],"ドップラー効果設定(あり)");
				}
			}
		}


		int n = m_select->GetSelect(m_seEffectNameList,cmKosuu);
		if (n >= 0)
		{
			if ((n>=0) && (n<=2))
			{
				int newxyz = xyz[n];
				if (m_input->GetNumber(xyz[n],&newxyz))
				{
					pMessage->SetSeTeii(n,newxyz,channel);
					editFlag = TRUE;
				}
			}
			else if (n==3)	//時間
			{
				int oldTime = 0;
				if (pMessage->CheckSeMoveFlag(channel))
				{
					oldTime = pMessage->GetSeMoveTime(channel);
				}

				int newTime = oldTime;

				if (m_input->GetNumber(oldTime,&newTime))
				{
					if (newTime >= 0)
					{
						pMessage->SetSeMoveTime(newTime,channel);
						editFlag = TRUE;
					}
				}
			}
			else if ((n >=4) && (n<=6))
			{
				int newxyz2 = xyz2[n-4];
				if (m_input->GetNumber(xyz2[n-4],&newxyz2))
				{
					pMessage->SetSeTeii2(n-4,newxyz2,channel);
					editFlag = TRUE;
				}
			}
			else if (n == 7)
			{
				if (dop == 0)
				{
					pMessage->SetDopplerSoutou(TRUE,channel);
				}
				else
				{
					pMessage->SetDopplerSoutou(FALSE,channel);
				}
				editFlag = TRUE;
			}

			//check all effect clear?

			pMessage->CheckAllSeEffectClear(channel);
		}


		if (editFlag)
		{
			pMessage->SetSeChannel(channel);

			m_app->SetModify();
			m_app->MessageIsChanged();
			UpdateMyWindow();
		}

	}
}


void CGameMessageDoc::OnChangeID(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	if (pMessage->GetMessageMode() == 3) return;


	char nm[16];
	wsprintf(nm,"%d",pMessage->GetID());
	LPSTR newText = m_input->GetText(nm,TRUE);

	if (newText == NULL) return;

	CheckAndGetUndo(pKoma,n,n);

	int d = 0;
	int ln = strlen(newText);
	for (int i=0;i<ln;i++)
	{
		int c = (int)(*(newText+i));
		c &= 0xff;

		c -= '0';
		if ((c<0) || (c>9)) break;

		d *= 10;
		d += c;
	}

	if ((d<1) || (d>8191))
	{
		MessageBox(m_app->GetFrameHWND(),"範囲外の番号です","ERROR",MB_OK);
		return;
	}

	//check??


	pMessage->SetID(d);

	m_app->SetModify();
	m_app->MessageIsChanged();
}


void CGameMessageDoc::OnLockVoice(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	BOOL flg = pMessage->GetVoiceFlag();
	if (flg == FALSE) return;

	CheckAndGetUndo(pKoma,n,n);

	pMessage->SetVoiceLock(TRUE,channel);
	m_app->SetModify();
	m_app->MessageIsChanged();
}

void CGameMessageDoc::OnUnlockVoice(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	BOOL flg = pMessage->GetVoiceFlag();
	if (flg == FALSE) return;

	CheckAndGetUndo(pKoma,n,n);

	pMessage->SetVoiceLock(FALSE,channel);
	m_app->SetModify();
	m_app->MessageIsChanged();
}


int CGameMessageDoc::GetScrollObjectKosuu(int vh)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return 0;

	int kosuu = pKoma->GetObjectKosuu() + 1;
//char mes[256];
//sprintf(mes,"kosuu=%d\n",kosuu);
//OutputDebugString(mes);


	return kosuu;
}


void CGameMessageDoc::OnChangeMessageColor(int n,int col,BOOL globalFlag)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int st = n;
	int ed = n;
	
	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return;

	if (pKoma->CheckInSubArea(n))
	{
		globalFlag = TRUE;
	}

	if (globalFlag)
	{
		//
		st = pKoma->GetSelectStart();
		ed = pKoma->GetSelectEnd();

		if ((st<0) || (st>=kosuu)) return;
		if ((ed<0) || (ed>=kosuu)) return;
	}

	CheckAndGetUndo(pKoma,st,ed);

	for (int i=st;i<=ed;i++)
	{
		CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(i));
		if (pMessage != NULL)
		{
			pMessage->SetMessageColor(col);
		}
	}

	m_app->SetModify();
	m_app->MessageIsChanged();
}


int CGameMessageDoc::GetMessageColor(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return 0;

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>=kosuu)) return 0;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return 0;

	return pMessage->GetMessageColor();
}


int CGameMessageDoc::GetCommentPrintMode(int n)
{
	return m_commentPrintMode[n];
}


void CGameMessageDoc::OnClickCommentTopButton(void)
{
	if (m_commentPrintMode[0] == 0)
	{
		m_commentPrintMode[0] = 1;
	}
	else
	{
		m_commentPrintMode[0] = 0;
	}

	m_app->SetConfig("commentTopPrintFlag",m_commentPrintMode[0]);
	m_app->MessageIsChanged();
}

void CGameMessageDoc::OnClickCommentBottomButton(void)
{
	if (m_commentPrintMode[1] == 0)
	{
		m_commentPrintMode[1] = 1;
	}
	else
	{
		m_commentPrintMode[1] = 0;
	}

	m_app->SetConfig("commentBottomPrintFlag",m_commentPrintMode[0]);
	m_app->MessageIsChanged();
}

int CGameMessageDoc::GetExpStatus(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return 0;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return 0;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return 0;

	return pMessage->GetExpStatus();
}

int CGameMessageDoc::GetMessageEffect(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return 0;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return 0;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return 0;

	return pMessage->GetMessageEffect();
}

int CGameMessageDoc::GetMessageEffectTime(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return 0;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return 0;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return 0;

	return pMessage->GetMessageEffectTime();
}


int CGameMessageDoc::GetMessageFontSizeType(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return 0;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return 0;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return 0;

	return pMessage->GetMessageFontSizeType();
}

int CGameMessageDoc::GetFace(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return 0;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return 0;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return 0;

	return pMessage->GetFace();
}

int CGameMessageDoc::GetMustFace(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return 0;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return 0;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return 0;

	return pMessage->GetMustFace();
}

int CGameMessageDoc::GetFixFace(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return 0;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return 0;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return 0;

	return pMessage->GetFixFace();
}

void CGameMessageDoc::OnClickExpStatusButton(void)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	int n = -1;
	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;


	CheckAndGetUndo(pKoma,n,n);

	int d = pMessage->GetExpStatus();
	if (m_input->GetNumber(d,&d))
	{
		pMessage->SetExpStatus(d);
	}

	m_app->SetModify();
	m_app->MessageIsChanged();
}

void CGameMessageDoc::OnClickMessageEffectButton(void)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	int n = -1;
	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	CheckAndGetUndo(pKoma,n,n);


	int d = pMessage->GetMessageEffect();
	if (m_input->GetNumber(d,&d))
	{
		pMessage->SetMessageEffect(d);
	}

	m_app->SetModify();
	m_app->MessageIsChanged();
}

void CGameMessageDoc::OnClickMessageEffectTimeButton(void)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	int n = -1;
	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	CheckAndGetUndo(pKoma,n,n);


	int d = pMessage->GetMessageEffectTime();
	if (m_input->GetNumber(d,&d))
	{
		pMessage->SetMessageEffectTime(d);
	}

	m_app->SetModify();
	m_app->MessageIsChanged();
}

void CGameMessageDoc::OnClickMessageFontSizeTypeButton(void)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	int n = -1;
	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	CheckAndGetUndo(pKoma,n,n);


	int d = pMessage->GetMessageFontSizeType();
	if (m_input->GetNumber(d,&d))
	{
		pMessage->SetMessageFontSizeType(d);
	}

	m_app->SetModify();
	m_app->MessageIsChanged();
}


void CGameMessageDoc::OnClickFaceButton(void)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	int n = -1;
	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;


	CheckAndGetUndo(pKoma,n,n);

	int d = pMessage->GetFace();
	if (m_input->GetNumber(d,&d,"非表示 = -1 , 表情種類 = 1-n"))
	{
		pMessage->SetFace(d);
	}

	m_app->SetModify();
	m_app->MessageIsChanged();
}


void CGameMessageDoc::OnClickMustFaceButton(void)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	int n = -1;
	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	CheckAndGetUndo(pKoma,n,n);


	int d = pMessage->GetMustFace();
	if (m_input->GetNumber(d,&d,"キャラ特定 = 1-n"))
	{
		pMessage->SetMustFace(d);
	}

	m_app->SetModify();
	m_app->MessageIsChanged();
}


void CGameMessageDoc::OnClickFixFaceButton(void)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	int n = -1;
	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;


	CheckAndGetUndo(pKoma,n,n);

	int d = pMessage->GetMustFace();
	if (m_input->GetNumber(d,&d,"キャラ固定 = 1:左 2:右 3:フリー"))
	{
		pMessage->SetFixFace(d);
	}

	m_app->SetModify();
	m_app->MessageIsChanged();
}

void CGameMessageDoc::OnSeFade(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	if (pMessage->GetSeMode(channel) == 0) return;

	CheckAndGetUndo(pKoma,n,n);

	int d = pMessage->GetSEFadeInOut(channel);
	if (d != 0)
	{
		pMessage->SetSEFadeInOut(0,channel);
	}
	else
	{
		int tm = 0;
		if (m_input->GetNumber(tm,&tm,"フェード時間x10"))
		{
			if (tm > 0)
			{
				pMessage->SetSEFadeInOut(1,channel);
				pMessage->SetSEFadeTime(tm,channel);
			}
		}

	}

	m_app->SetModify();
	m_app->MessageIsChanged();
}

void CGameMessageDoc::OnSeVolumeOnly(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	CheckAndGetUndo(pKoma,n,n);

	int vol = 0;
	if (pMessage->GetSEVolumeOnly(channel))
	{
		vol = pMessage->GetSeVolume(channel);
	}

	if (m_input->GetNumber(vol,&vol,"Volume"))
	{
		pMessage->SetSEVolumeOnly(1,channel);
		pMessage->SetSeVolume(vol,channel);
		m_app->SetModify();
		m_app->MessageIsChanged();
	}
}

void CGameMessageDoc::OnVoiceFade(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;


	CheckAndGetUndo(pKoma,n,n);




	int d = pMessage->GetVoiceFadeInOut(channel);
	if (d != 0)
	{
		pMessage->SetVoiceFadeInOut(0,channel);
	}
	else
	{
		int tm = 0;
		if (m_input->GetNumber(tm,&tm,"フェード時間x10"))
		{
			if (tm > 0)
			{
				pMessage->SetVoiceFadeInOut(1,channel);
				pMessage->SetVoiceFadeTime(tm,channel);
			}
		}

	}

	m_app->SetModify();
	m_app->MessageIsChanged();

}





void CGameMessageDoc::OnClickMusicFadeButton(void)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;

	int n = -1;
	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	CheckAndGetUndo(pKoma,n,n);

	int vol = pMessage->GetMusicVolume();
	if (m_input->GetNumber(vol,&vol,"ボリューム 元にもどす=101"))
	{
		int tm = 0;
		if (vol != 0)
		{
			tm = pMessage->GetMusicFadeTime();
			m_input->GetNumber(tm,&tm,"フェード時間x10");
		}

		pMessage->SetMusicVolume(vol);
		pMessage->SetMusicFadeTime(tm);

		m_app->SetModify();
		m_app->MessageIsChanged();
	}
}


int CGameMessageDoc::GetMusicFadeVolume(int n)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return 0;

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return 0;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return 0;


	return pMessage->GetMusicVolume();
}



void CGameMessageDoc::OnVoiceVolumeOnly(int n,int channel)
{
	CKomaData* pKoma = GetNowSelectKoma();
	if (pKoma == NULL) return;	//error

	if (n == -1) n = pKoma->GetNowSelectNumber();

	int kosuu = pKoma->GetObjectKosuu();
	if ((n<0) || (n>kosuu)) return;

	CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(n));
	if (pMessage == NULL) return;

	CheckAndGetUndo(pKoma,n,n);

	int vol = 0;
	if (pMessage->GetVoiceVolumeOnly(channel))
	{
		vol = pMessage->GetVoiceVolume(channel);
	}

	if (m_input->GetNumber(vol,&vol,"Volume"))
	{
		pMessage->SetVoiceVolumeOnly(1,channel);
		pMessage->SetVoiceVolume(vol,channel);
		m_app->SetModify();
		m_app->MessageIsChanged();
	}

}



void CGameMessageDoc::ScrollToSelect(void)
{
	CGameMessageView* pView = (CGameMessageView*)m_view;
	CKomaData* pKoma = GetNowSelectKoma();


	pView->ReCalcuScrollPara();

	int n = pKoma->GetNowSelectNumber();
	pView->ScrollTo(n);
}



CCase* CGameMessageDoc::GetNowSelectCaseObject(void)
{
	return GetNowSelectKoma();
}

void CGameMessageDoc::OnSelectNumber(int n)
{
	ClearUndo();

	m_app->MessageIsChanged();
//	m_app->ConteIsChanged();
}

void CGameMessageDoc::OnDeleteKey(void)
{
	if (m_app->GetConfig("deleteKeyMessage") == 0) return;
	OnDelete();
}

void CGameMessageDoc::OnEnterKey(void)
{
	CKomaData* pKoma = m_app->GetNowSelectKoma();
	if (pKoma == NULL) return;

	int n = pKoma->GetNowSelectNumber();
	int kosuu = pKoma->GetObjectKosuu();

	if (n == kosuu)
	{
		OnNewMessage();
	}
	else
	{
		OnClickModeButton(-1);
	}
}

void CGameMessageDoc::OnInsertKey(void)
{
	OnNewMessage();
}

void CGameMessageDoc::OnEscapeKey(void)
{
	m_app->ChangeWindowIfCan(FILM_WINDOW);
}


int CGameMessageDoc::GetEditorMessageGyo(void)
{
	return m_app->GetEditorMessageGyo();
}

int CGameMessageDoc::GetMessageWidthGuideLine(void)
{
	return m_app->GetMessageWidthGuideLine();
}


BOOL CGameMessageDoc::CheckExistUndo(void)
{
	CUndoMemoryObject* undo = m_app->GetUndoObject();
	if (undo != NULL)
	{
		int undoType = undo->GetUndoType();
		if (undoType == UNDO_TYPE_MESSAGE)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CGameMessageDoc::OnUndo(int n)
{
	BOOL f = FALSE;

	if (m_app->GetUndoMode())
	{
		CKomaData* pKoma = m_app->GetNowSelectKoma();
		if (pKoma == NULL) return FALSE;

		CUndoMemoryObject* undo = m_app->GetUndoObject();
		if (undo != NULL)
		{
			if (CheckExistUndo())
			{
				int dataType = undo->GetUndoDataType();
				int startN = undo->GetUndoStartN();
				int endN = undo->GetUndoEndN();
				int numN = endN - startN + 1;
				

				if (dataType == UNDO_DATA_INSERT)
				{
					pKoma->DeleteObjectData(startN,numN);
					undo->Clear();
					f = TRUE;
				}
				else if (dataType == UNDO_DATA_DELETE)
				{
					for (int i=startN;i<=endN;i++)
					{
						pKoma->CreateObjectData(i);
						CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(i));
						pMessage->Init();
						pMessage->Load(NULL,undo);
					}
					undo->Clear();
					f = TRUE;
				}
				else if (dataType == UNDO_DATA_MODIFY)
				{
					for (int i=startN;i<=endN;i++)
					{
						CMessageData* pMessage = (CMessageData*)(pKoma->GetObjectData(i));
						pMessage->Init();
						pMessage->Load(NULL,undo);
					}
					undo->Clear();
					f = TRUE;
				}
			}
		}
	}




	if (f)
	{
		((CGameMessageView*)m_view)->ReCalcuScrollPara();
		m_app->SetModify();
		m_app->MessageIsChanged();
		m_view->MyInvalidateRect();
	}


	return f;
}

void CGameMessageDoc::ClearUndo(void)
{
	if (m_app->GetUndoMode())
	{
		CUndoMemoryObject* undo = m_app->GetUndoObject();
		undo->Clear();
	}
}



/*　
 3. 禁則処理の対象となる文字

 
　対象文字には，行頭にきてはいけないもの（行頭禁止文字），行末にきてはいけないもの（行末禁止文字），そして2行にまたがってはいけないもの（分離禁止文字）の3種類があり，それぞれやってはいけないことを避けて処理することを「禁則処理」といいます。
　MS-Wordでは，連数字（2つ以上数字が重なったもの），略語（英語の略―m：meterの略etc.）など一部を除きほとんど自動処理の設定がなされています（項目4.，5.参照）。

 
① 行頭禁止文字 
句読点類 ，　、(読点） 。　．(句点)　・(中黒)　：　；　?　!　など 
つなぎ記号 ―（ダッシュ）　…　　→←↑↓　ー(長音)　-（ハイフン）など 
くり返し記号 ゝ　ゞ（ひら仮名）　ヽ　ヾ（片仮名） 々（漢字の送り）など 
受けカッコ類 ）　」　〕　｝　】など 
促音・拗音類 ぁ ぃ ぅ ぇ ぉ っ ゃ ゅ ょ ゎ ヵ ん 
ァ ィ ゥ ェ ォ ッ ャ ュ ョ　ヮ ヶ ン  
 
② 行末禁止文字 
始まりカッコ類 （　「　〔　｛　【 など 
 
③ 分離禁止文字 
連数字 1,000円　42.195 km　2000年　平成12年500坪などの2つ以上の数字の重なり　　 
数字についた単位を表す記号類 ￥　$　￠　￡　％　℃　＠　kg　g　mg　　　km　m　cm　mm 
など 
*/
 
 


/*_*/

