//
// minicompiler.cpp
//



#include <windows.h>
#include <stdio.h>
#include <mbstring.h>
#include <string.h>


#include "minicompiler.h"
#include "commanddatatype.h"

#include "..\..\systemNNN\nnnUtilLib\namelist.h"

#include "..\..\systemNNN\nnnUtilLib\scriptcommand.h"
#include "..\..\systemNNN\nnnUtilLib\scriptDefine.h"
#include "..\..\systemNNN\nyanEffectLib\effectList.h"

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"


#define TOKEN_COMMAND SCRIPT_COMMAND
#define TOKEN_FUNCTION SCRIPT_FUNCTION

//dummy
#define TOKEN_SYSTEMCOMMAND SCRIPT_FUNCTION

#define TOKEN_EFFECT 0x20000000
#define TOKEN_KAISEKISTART 0x30000000
#define TOKEN_KAISEKIEND 0x30000001

#define TOKEN_LABEL SCRIPT_LABEL
//#define TOKEN_PLACELABEL SCRIPT_PLACELABEL
//#define TOKEN_INLABEL 0x60000000

#define TOKEN_VAR SCRIPT_VAR
#define TOKEN_NUMBER SCRIPT_NUMBER
#define TOKEN_NUMBER32 SCRIPT_NUMBER32
#define TOKEN_CALCU SCRIPT_CALCU
//#define TOKEN_DAINYU SCRIPT_DAINYU
#define TOKEN_MESSAGE SCRIPT_MESSAGE
#define TOKEN_MESSAGE2 SCRIPT_MESSAGE2
#define TOKEN_BEGINEND SCRIPT_BEGINEND

#define TOKEN_KAKKO (TOKEN_CALCU | CALCU_KAKKO)
#define TOKEN_CALCUEQUAL (TOKEN_CALCU | CALCU_CALCUEQUAL)
#define TOKEN_DOUBLE (TOKEN_CALCU | CALCU_DOUBLE)

#define TOKEN_COMPARE (TOKEN_CALCU | CALCU_COMPARE)
#define TOKEN_COMPAREEQUAL (TOKEN_CALCU | CALCU_COMPAREEQUAL)

#define TOKEN_BEGIN (TOKEN_BEGINEND | '{')
#define TOKEN_END (TOKEN_BEGINEND | '}')

#define TOKEN_SEPARATER 0xffffffff

int CMiniCompiler::m_juniTable[5][5] = 
{
	{1,0,3,0,3},
	{1,0,2,1,0},
	{0,0,0,0,0},
	{1,0,0,1,2},
	{0,0,3,0,3},
};

int CMiniCompiler::m_juniTableFrom[5] = {3,1,0,1,3};

int CMiniCompiler::m_juniTableTo[5] = {1,0,3,1,3};



char CMiniCompiler::m_defaultCaption[] = "エラー";


CMiniCompiler::ENZANSHINAMETABLE CMiniCompiler::m_enzanshiName[]=
{
	{ENZANSHI_NULL,			"null",		(int)TOKEN_NUMBER | 0},
	{ENZANSHI_TRUE,			"true",		(int)TOKEN_NUMBER | 1},
	{ENZANSHI_FALSE,			"false",		(int)TOKEN_NUMBER | 0},
	{ENZANSHI_AND,			"and",		(int)TOKEN_DOUBLE | '&'},
	{ENZANSHI_XOR,			"xor",		(int)TOKEN_DOUBLE | '^'},
	{ENZANSHI_OR,				"or",		(int)TOKEN_DOUBLE | '|'},


	{-1,					"nothing",	0},
};


char CMiniCompiler::m_systemConst[][32]=
{
	"nothing_mode","1",
	"logo_mode","2",
	"opening_mode","3",
	"title_mode","4",
	"load_mode","5",
	"save_mode","6",
	"backlog_mode","7",
	"listenbgm_mode","8",
	"selectmessage_mode","9",
	"printmessage_mode","10",
	"config_mode","12",
	"systemmenu_mode","13",
	"omake_mode","15",
	"selectcgchara_mode","16",
	"selectcg_mode","17",
	"selectscene_mode","18",
	"selectscenechara_mode","20",
	"selectplace_mode","22",
	"printcg_mode","23",
	"selectmoviechara_mode","25",
	"selectmovie_mode","26",
	"printmovie_mode","27",
	"nameinput_mode","29",
	"printcalendar_mode","30",
	"selectheroin_mode","31",
	"codecerror_mode","32",
	"minigame","40",

	"on","1",
	"off","0",

	"ok","1",
	"bad","0",

	"tinyan","666",

	"","",//endmark
};



/*
CODE_SYSTEMFUNCTION_FILMENDENABLE
CODE_SYSTEMFUNCTION_FILMTYPETIME
CODE_SYSTEMFUNCTION_CONFIGMASK
CODE_SYSTEMFUNCTION_CLEARALLEFFECT
CODE_SYSTEMFUNCTION_RENAMELAYER
CODE_SYSTEMFUNCTION_CUTIN
*/


/*
	if (typ == COMMANDDATATYPE_NOP)
	if (typ == COMMANDDATATYPE_RET)
	if (typ == COMMANDDATATYPE_NEXT)
	if (typ == COMMANDDATATYPE_END)
	if (typ == COMMANDDATATYPE_EXIT)
	if (typ == COMMANDDATATYPE_ELSE)
	if (typ == COMMANDDATATYPE_ENDFILM)
	if (typ == COMMANDDATATYPE_ENDKOMA)
	if (typ == COMMANDDATATYPE_ENDIF)

*/

CMiniCompiler::MYFUNCTIONTONAME CMiniCompiler::m_functionToNameTable[]=
{
	{CODE_SYSTEMFUNCTION_FILMENDENABLE,"filmEndEnable"},
	{CODE_SYSTEMFUNCTION_FILMTYPETIME,"filmTypeTime"},
	{CODE_SYSTEMFUNCTION_CONFIGMASK,"configMask"},
	{CODE_SYSTEMFUNCTION_CLEARALLEFFECT,"clearAllEffect"},
	{CODE_SYSTEMFUNCTION_RENAMELAYER,"renameLayer"},
	{CODE_SYSTEMFUNCTION_CUTIN,"cutin"},
	{CODE_SYSTEMFUNCTION_MUSTFACE,"mustFace"},
	{CODE_SYSTEMFUNCTION_VARCONTROLLAYER,"varControlLayer"},
	{CODE_SYSTEMFUNCTION_AUTOMESSAGE,"autoMessage"},
	{CODE_SYSTEMFUNCTION_CANNOTCLICK,"cannotClick"},
	{CODE_SYSTEMFUNCTION_CANNOTSKIP,"cannotSkip"},
	{CODE_SYSTEMFUNCTION_OPTIONOFF,"optionOff"},
	{CODE_SYSTEMFUNCTION_CONFIGMASK,"configMask"},
	{CODE_SYSTEMFUNCTION_CUTIN,"cutin"},
	{CODE_SYSTEMFUNCTION_CHANGEMESSAGEFONTSIZETYPE,"changeMessageFontSizeType"},
	{CODE_SYSTEMFUNCTION_SETFILM,"setFilm"},
	{COMMANDDATATYPE_ENDKOMA,"endKoma"},
	{COMMANDDATATYPE_ENDFILM,"endFilm"},
	{CODE_SYSTEMFUNCTION_SETCGBYVAR,"setCGByVar"},
	{CODE_SYSTEMFUNCTION_SETCG,"setCG"},
	{CODE_SYSTEMFUNCTION_SETVAR,"setVar"},
	{CODE_SYSTEMFUNCTION_SETDEMOFLAG,"setDemoFlag"},
	{CODE_SYSTEMFUNCTION_NOMESSAGEWINDOW,"noMessageWindow"},
	{CODE_SYSTEMFUNCTION_PREPAREOVERRAP,"prepareOverrap"},
	{CODE_SYSTEMFUNCTION_STARTKOMA,"startKoma"},
	{CODE_SYSTEMFUNCTION_FRAMECONTROL,"frameControl"},
	{CODE_SYSTEMFUNCTION_SETDEFAULTFRAME,"setDefaultFrame"},
	{CODE_SYSTEMFUNCTION_WINDOWNUMBER,"windowNumber"},
	{CODE_SYSTEMFUNCTION_CURSORNUMBER,"cursorNumber"},
	{CODE_SYSTEMFUNCTION_MOUSENUMBER,"mouseNumber"},
	{CODE_SYSTEMFUNCTION_FILMEXPSTATUS,"filmExpStatus"},
	{CODE_SYSTEMFUNCTION_SETEFFECTRECT,"setEffectRect"},
	{CODE_SYSTEMFUNCTION_CLEAREFFECTLAYER,"clearEffectLayer"},
	{CODE_SYSTEMFUNCTION_SETEFFECT,"setEffect"},
	{CODE_SYSTEMFUNCTION_PRELOADDWQ,"preLoadDWQ"},
	{CODE_SYSTEMFUNCTION_LOADDWQ,"loadDWQ"},
	{CODE_SYSTEMFUNCTION_STARTMESSAGE,"startMessage"},
	{CODE_SYSTEMFUNCTION_MUSICVOLUMEONLY,"musicVolumeOnly"},
	{CODE_SYSTEMFUNCTION_MESSAGEEXPSTATUS,"messageExpStatus"},
	{CODE_SYSTEMFUNCTION_NEXTFADE_VOICE,"nextFadeVoice"},
	{CODE_SYSTEMFUNCTION_VOLUMEONLY_VOICE,"volumeOnlyVoice"},
	{CODE_SYSTEMFUNCTION_SETVOICEFLAG,"setVoiceFlag"},
	{CODE_SYSTEMFUNCTION_NEXTFADE_SE,"nextFadeSE"},
	{CODE_SYSTEMFUNCTION_SOUND,"sound"},
	{CODE_SYSTEMFUNCTION_VOLUMEONLY_SE,"volumeOnlySE"},
	{CODE_SYSTEMFUNCTION_SETTERM,"setTerm"},
	{CODE_SYSTEMFUNCTION_FACE,"face"},
	{CODE_SYSTEMFUNCTION_MESSAGEEFFECT,"messageEffect"},
	{CODE_SYSTEMFUNCTION_MESSAGEEFFECTTIME,"messageEffectTime"},




	{CODE_SYSTEMCOMMAND_OVERRAP,"overrap"},
	{CODE_SYSTEMCOMMAND_DRAW,"draw"},
	{CODE_SYSTEMCOMMAND_PRINT,"print"},
	{CODE_SYSTEMCOMMAND_LPRINT,"lprint"},
	{CODE_SYSTEMCOMMAND_APPEND,"append"},

	{COMMANDDATATYPE_NOP,"nop"},
	{COMMANDDATATYPE_RET,"ret"},
	{COMMANDDATATYPE_NEXT,"next"},
	{COMMANDDATATYPE_END,"end"},
	{COMMANDDATATYPE_EXIT,"exit"},
	{COMMANDDATATYPE_ELSE,"else"},
	{COMMANDDATATYPE_ENDIF,"endif"},

	{-1,"endmark"}
};

CMiniCompiler::CMiniCompiler()
{
	m_pass1OkFlag = FALSE;
	m_pass2OkFlag = FALSE;
	m_sourceFileSize = 0;
	m_commandKosuu = 0;

	m_errorMessage[0] = 0;
	m_errorMessage2[0] = 0;

	m_makedCode = new int[65536*4*4];

	m_functionList = NULL;
	m_commandList = NULL;
	m_varList = NULL;
	m_filmList = NULL;
	m_storyList = NULL;

	m_strPointer = NULL;
	m_mesPointer = NULL;


	m_fileBufferSize = 65536*8 * 4;
	m_fileBuffer = new char[m_fileBufferSize];
	if (m_fileBuffer == NULL)
	{
		MessageBox(NULL,"メモリ不足です。ただちにプログラムを中止してくださいにゃ","重大なエラー",MB_OK);
		m_fileBufferSize = 0;
	}

	m_strPointerSize = 65536*2*4;
	m_mesPointerSize = 65536*2*4;


	m_strPointer = new int[m_strPointerSize];
	m_mesPointer = new int[m_mesPointerSize];

//	m_strPointer = new int[65536*2*4];
//	m_mesPointer = new int[65536*2*4];


	m_strTableSize = 4096*4;
	m_mesTableSize = 4096*4;


	m_strTable = new int[m_strTableSize];
	m_mesTable = new int[m_mesTableSize];

	m_subTableSize = 16384;
	m_subTable = new int[m_subTableSize];

	m_scriptCallTableSize = 16384;
	m_scriptCallTable = new int[m_scriptCallTableSize];

	m_commandCallTableSize = 16384;
	m_commandCallTable = new int[m_commandCallTableSize];

	m_selectTableSize = 16384;
	m_selectTable = new int[m_selectTableSize];

	int n = 0;

	m_iPhoneString = new char[16384];

	n = 0;
	while (m_enzanshiName[n].code != -1)
	{
		n++;
	}
	m_enzanshiKosuu = n;

	m_functionToNameNumber = 0;
	while (m_functionToNameTable[m_functionToNameNumber].type != -1)
	{
		m_functionToNameNumber++;
	}
}




CMiniCompiler::~CMiniCompiler()
{
	End();
}

void CMiniCompiler::End()
{
	DELETEARRAY(m_iPhoneString);

	DELETEARRAY(m_selectTable);
	DELETEARRAY(m_commandCallTable);
	DELETEARRAY(m_scriptCallTable);
	DELETEARRAY(m_subTable);

	DELETEARRAY(m_makedCode);

	if (m_fileBuffer != NULL)
	{
		delete [] m_fileBuffer;
		m_fileBuffer = NULL;
		m_fileBufferSize = 0;
	}

	DELETEARRAY(m_strTable);
	DELETEARRAY(m_mesTable);

	DELETEARRAY(m_strPointer);
	DELETEARRAY(m_mesPointer);
}


BOOL CMiniCompiler::SetCompileSource(LPSTR src)
{
	int ln = strlen(src);
	
//	if (ln>65536) return FALSE;
	memcpy(m_fileBuffer,src,ln+1);
	m_sourceFileSize = ln;
	return TRUE;
}



BOOL CMiniCompiler::Pass1(void)
{
	int kakko = 0;
	int n = 0;
//
//ここからが真のPASS-1
//
	m_gyo = 1;
	m_tokenKosuu = 0;

	n = 0;

	wsprintf(m_errorCode,"PASS1 OK");
	wsprintf(m_errorMessage,"no error");

	while (n<m_sourceFileSize)
	{
		if (m_tokenKosuu>=65536)
		{
			MessageBox(NULL,"Token数が多すぎます。コンパイル不能ですにゃ","ERROR",MB_OK);
			return FALSE;
		}

		int sk = SkipSource(m_fileBuffer+n);
		n += sk;

		m_tokenSource[m_tokenKosuu] = n;
		m_tokenGyo[m_tokenKosuu] = m_gyo;

		char mae = *(m_fileBuffer + n - 1);
		char c = *(m_fileBuffer + n);
		char c2 = *(m_fileBuffer + n + 1);

		//終了
		if (c == 0)
		{
			break;	//終了
		}

		//コメントスキップ
		if ((c == '/') && (c2 == '/'))
		{
			int rem = SkipToReturn(m_fileBuffer+n);
			n+= rem;
//			m_remKosuu++;
			continue;
		}

		if ((c == ',') || (c == ';'))
		{
			m_token[m_tokenKosuu] = TOKEN_SEPARATER;
			m_tokenKosuu++;
			n++;
			continue;
		}


		if ((c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '%') || (c == '&') || (c == '|') || (c == '<') || (c == '>') || (c == '=') || (c == '!') || (c == '^'))
		{
			n++;

			if ((c == c2) && (c != '='))
			{
				m_tokenKosuu++;
				n++;

				if ((c == '&') || (c == '|') || (c == '^'))
				{
					m_token[m_tokenKosuu-1] = TOKEN_DOUBLE | c;
					continue;
				}

				//error
				AddError("存在しにゃい演算子",n-2);
				return FALSE;

			}


			if (c2 == '=')
			{
				n++;
				m_tokenKosuu++;


				if ((c == '<') || (c == '>') || (c == '!') || (c == '='))
				{
					m_token[m_tokenKosuu-1] = TOKEN_COMPAREEQUAL | c;
					continue;
				}

				m_token[m_tokenKosuu-1] = TOKEN_CALCUEQUAL | c;
				continue;
			}
			else
			{
				if (c == '-')
				{
					int maecmd = m_token[m_tokenKosuu-1] & 0xf0000000;
					if ((maecmd != TOKEN_VAR) && (maecmd != TOKEN_NUMBER) && (maecmd != TOKEN_NUMBER32))
					{
						m_token[m_tokenKosuu] = TOKEN_NUMBER | 0;	//0-xにへんかん
						m_tokenKosuu++;
					}
				}

				if ((c == '=') && (kakko>0))
				{
//					m_token[m_tokenKosuu] = TOKEN_DOUBLE | c;	//()のなかの = は == に変換
					m_token[m_tokenKosuu] = TOKEN_COMPAREEQUAL | c;	//()のなかの = は == に変換
				}
				else
				{
//					if ((c == '<') || (c == '>') || (c == '!'))
					if ((c == '<') || (c == '>'))
					{
						m_token[m_tokenKosuu] = TOKEN_COMPARE | c;
					}
					else
					{
						m_token[m_tokenKosuu] = TOKEN_CALCU | c;
					}
//					m_token[m_tokenKosuu] = TOKEN_DAINYU | c;
				}
				m_tokenKosuu++;
			}
			continue;
		}


		if ( ( c == '(' ) || ( c == ')' ) )
		{
			n++;

			int tst = 0;


			if (c == '(')
			{
				kakko++;
			}

			if (c == ')')
			{
				kakko--;
				tst = kakko;
			}





			if (tst<0)
			{
				AddError("()対応エラー(とじるのがおおい)",n-2);
				return FALSE;
			}

			m_token[m_tokenKosuu] = TOKEN_KAKKO | c;
			m_tokenKosuu++;
			continue;
		}


		char c0 = *(m_fileBuffer+n-1);


		int skp = SkipToken(m_fileBuffer+n);

		if (skp > 16383)
		{
			MessageBox(NULL,"Tokenが長すぎますにゃ","ERROR",MB_OK);
			return FALSE;
		}

		char tkn[16384];
		memcpy(tkn,m_fileBuffer+n,skp);
		tkn[skp] = 0;
		n += skp;

//		OutputDebugString("【");
//		OutputDebugString(tkn);
//		OutputDebugString("】");
		// check code


		//check function
		int func = GetFunctionNumber(tkn);
		if (func != -1)
		{
			m_token[m_tokenKosuu] = TOKEN_FUNCTION | func;
			m_tokenKosuu++;
//			m_functionKosuu++;
			continue;
		}

		int syscom = GetSystemCommandNumber(tkn);
		if (syscom != -1)
		{
			m_token[m_tokenKosuu] = TOKEN_SYSTEMCOMMAND | syscom;
			m_tokenKosuu++;
//			m_functionKosuu++;
			continue;
		}



///		//check enzanshi
		int enz = GetEnzanshiNameNumber(tkn);
		if (enz != -1)
		{
			m_token[m_tokenKosuu] = m_enzanshiName[enz].settoken;
			m_tokenKosuu++;
			continue;
		}


		// check var
		int var = m_varList->GetNameNumber(tkn);
		if (var != -1)
		{
			m_token[m_tokenKosuu] = TOKEN_VAR | var;
			m_tokenKosuu++;
			m_varKosuu++;
			continue;
		}



		// check 数値

		BOOL b = TRUE;
		for (int ii=0;ii<skp;ii++)
		{
			if ((tkn[ii] < '0') || (tkn[ii] > '9'))
			{
				b = FALSE;
				break;
			}
		}

		//check system定数
		if (!b)
		{
			int sysconst = -1;
			if (GetSystemConst(tkn,&sysconst))
			{

				if ((sysconst >= 0) && (sysconst < 10000))
				{
					m_token[m_tokenKosuu] = TOKEN_NUMBER | sysconst;
				}
				else
				{
					if (m_largeConstKosuu < 4096)
					{
						m_token[m_tokenKosuu] = TOKEN_NUMBER32 | m_largeConstKosuu;
						m_largeConst[m_largeConstKosuu] = sysconst;
						m_largeConstKosuu++;
					}
					else
					{
						m_token[m_tokenKosuu] = TOKEN_NUMBER | 9999;
						AddError("int定数個数が4096を超えました",n);
						return FALSE;
					}
				}

				m_tokenKosuu++;
				continue;
			}
		}


		if (b)
		{
			int d = 0;
			for (int kk=0;kk<skp;kk++)
			{
				d *= 10;
				d += (tkn[kk] - '0');
			}
//@@@@@
			if (d < 10000)
			{
				m_token[m_tokenKosuu] = TOKEN_NUMBER | d;
			}
			else
			{
				if (m_largeConstKosuu < 4096)
				{
					m_token[m_tokenKosuu] = TOKEN_NUMBER32 | m_largeConstKosuu;
					m_largeConst[m_largeConstKosuu] = d;
					m_largeConstKosuu++;
				}
				else
				{
					m_token[m_tokenKosuu] = TOKEN_NUMBER | 9999;
					AddError("int定数個数が4096を超えました",n);
					return FALSE;
				}
			}

			if (m_tokenKosuu>=2)
			{
				if (m_token[m_tokenKosuu-1] == (TOKEN_CALCU | '-'))
				{
					int ctk = m_token[m_tokenKosuu-2] & 0xffff0000;
	
// 					if ((ctk == TOKEN_CALCU) || (ctk == TOKEN_CALCUEQUAL) )	
//					{
//						m_tokenKosuu--;
//						m_token[m_tokenKosuu] = TOKEN_NUMBER | ((-d) & 0xffff);
//					}
				}
			}

			m_tokenKosuu++;

			continue;
		}


		//解析不能なものたち

//		AddError("解析失敗",m_tokenGyo[m_tokenKosuu]);
		AddError("解析失敗",n);

//		sprintf(m_errorCode,"解析失敗 line %d",m_tokenGyo[m_tokenKosuu]);
//		memcpy(m_errorMessage,m_fileBuffer + m_tokenSource[m_tokenKosuu],158);
//		m_errorMessage[158] = 0;
//		m_errorMessage[159] = 0;

//		MessageBox(NULL,tkn,"解析失敗",MB_OK);
		return FALSE;


	}

	m_pass1OkFlag = TRUE;

	return TRUE;
}








int CMiniCompiler::GetEnzanshiNameNumber(LPSTR name)
{
	int l1 = strlen(name);

	for (int i=0;i<m_enzanshiKosuu;i++)
	{
		LPSTR name2 = m_enzanshiName[i].name;
		int l2 = strlen(name2);
		if (l1 == l2)
		{
			if (strcmp(name,name2) == 0)
			{
				return m_enzanshiName[i].code;
			}
		}
	}

	return -1;
}



int CMiniCompiler::SkipSource(LPSTR ptr)
{
	char c = *ptr;
	int n = 0;

	while ((c == ' ') || (c == '\t') || (c == 0x0d) || (c == 0x0a))
	{
		if (c == 0x0d) m_gyo++;

		//漢字スキップ
		int cc = (int)c;
		cc &= 0xff;
		if (((cc>=0x80) && (cc<=0x9f)) || ((cc>=0xe0) && (cc<=0xff)))
		{
			ptr++;
			n++;
		}

		ptr++;
		c = *ptr;
		n++;
	}

	return n;
}


int CMiniCompiler::SkipToReturn(LPSTR ptr)
{
	char c = *ptr;
	int n = 0;

	while ((c != 0x0a) && (c != 0))
	{
		ptr++;
		c = *ptr;
		n++;
	}

	n++;

	m_gyo++;

	return n;
}



int CMiniCompiler::SkipToken(LPSTR ptr)
{
	char c = *ptr;
	int n = 0;

	while (
			(c != '+') &&
			(c != '-') &&
			(c != '*') &&
			(c != '/') &&
			(c != '%') &&
			(c != '&') &&
			(c != '|') &&
			(c != '^') &&
			(c != '<') &&
			(c != '>') &&
			(c != '=') &&
			(c != '!') &&
			(c != ' ') &&
			(c != '\t') &&
			(c != 0x0d) &&
			(c != 0x0a) &&
			(c != ',') &&
			(c != ';') &&
			(c != 0x22) &&
			(c != 0) &&
			(c != '{') &&
			(c != '}') &&
			(c != '(') &&
			(c != ')') &&
			(c != '[') &&
			(c != 0) &&
			(c != ']')
		)
	{
		if (c == 0x0a) m_gyo++;

		int cc = (int)c;
		cc &= 0xff;
		//漢字スキップ
		if (((cc>=0x80) && (cc<=0x9f)) || ((cc>=0xe0) && (cc<=0xff)))
		{
			ptr++;
//			c = *ptr;
			n++;
		}

		ptr++;
		c = *ptr;
		n++;

	}

	return n;
}


int CMiniCompiler::SearchToken(int n, int cd,int cd2)
{
	int k = 0;

	int stk = 0;

	while (n<m_tokenKosuu)
	{
		int tkn = m_token[n];
		if (cd2 != -1)
		{
			if (tkn == cd2)
			{
				stk++;
			}
		}

		if (tkn == cd)
		{
			if (stk>0)
			{
				stk--;
			}
			else
			{
				return k;
			}
		}

		n++;
		k++;
	}

	return -1;
}


//
// num,loop,vol,in,out
// -1,out
//
//
BOOL CMiniCompiler::Pass2Music(int paraKosuu,int* paraPtr, LPSTR filename,FILE* jsonFlag,int tab)
{
	m_objectCode[0] = 4+paraKosuu;
	m_objectCode[1] = IDENTIFY_SYSTEMFUNCTION;
	m_objectCode[2] = CODE_SYSTEMFUNCTION_MUSIC;
	m_objectCode[3] = paraKosuu;

	for (int i=0;i<paraKosuu;i++)
	{
		m_objectCode[4+i] = paraPtr[i];
	}

	if (jsonFlag != NULL)
	{
		char mes[1024];
		int n = paraKosuu;

		LPSTR functionName = "Music";
		if (n == 0)
		{
			wsprintf(mes,"{ \"type\" : \"%s\" , \"Param\" : null },",functionName);
			OutputData(jsonFlag,mes,tab);
		}
		else
		{
			wsprintf(mes,"{ \"type\" : \"%s\" , \"Param\" : [%d",functionName,n);
			for (int i=0;i<n;i++)
			{
				char nummes[256];
				wsprintf(nummes,",%d",*(paraPtr+i));
				strcat_s(mes,1024,nummes);
			}
			strcat_s(mes,1024,"] },");

			OutputData(jsonFlag,mes,tab);
		}
	}




	m_objectSize = 4+paraKosuu;
	m_pass2OkFlag = TRUE;

	return TRUE;
}



BOOL CMiniCompiler::Pass2SystemCommand(int cmd, int n, int* pData,FILE* jsonFlag,int tab)
{
	m_objectCode[0] = 4+n;
	m_objectCode[1] = IDENTIFY_SYSTEMCOMMAND;
	m_objectCode[2] = cmd;
	m_objectCode[3] = n;

	for (int i=0;i<n;i++)
	{
		m_objectCode[4+i] = *(pData+i);
	}


	if (jsonFlag != NULL)
	{
		char mes[1024];
		LPSTR functionName = GetFunctionName(cmd);
		if (n == 0)
		{
			wsprintf(mes,"{ \"type\" : \"%s\" , \"Param\" : null },",functionName);
			OutputData(jsonFlag,mes,tab);
		}
		else
		{
			wsprintf(mes,"{ \"type\" : \"%s\" , \"Param\" : [%d",functionName,n);
			for (int i=0;i<n;i++)
			{
				char nummes[256];
				wsprintf(nummes,",%d",*(pData+i));
				strcat_s(mes,1024,nummes);
			}
			strcat_s(mes,1024,"] },");

			OutputData(jsonFlag,mes,tab);
		}
	}


	m_objectSize = 4+n;
	m_pass2OkFlag = TRUE;

	return TRUE;
}


BOOL CMiniCompiler::Pass2SystemCommandMessage(int cmd, LPSTR message,int id,int cutin,FILE* jsonFlag,int tab)
{
	AddMes(message);

	m_objectCode[0] = 6;
	m_objectCode[1] = IDENTIFY_SYSTEMCOMMAND;
	m_objectCode[2] = cmd;
	m_objectCode[3] = m_mesKosuu-1;
	m_objectCode[4] = id;

//	int cutin = 0;
//#if defined _DEBUG
//	cutin = (rand() % 4)+1;
//#endif

	m_objectCode[5] = cutin;	//cutin data

	m_objectSize = 6;
	m_pass2OkFlag = TRUE;

	if (jsonFlag)
	{
		LPSTR functionName = GetFunctionName(cmd);
		char mes[1024];
		wsprintf(mes,"{ \"type\" : \"%s\" , \"Param\" :",functionName);
		OutputData(jsonFlag,mes,tab);
		OutputData(jsonFlag,"{",tab+1);

		wsprintf(mes,"\"messageID\" : %d ,",id);
		OutputData(jsonFlag,mes,tab+2);

		wsprintf(mes,"\"cutinData\" : %d ,",cutin);
		OutputData(jsonFlag,mes,tab+2);

		OutputData(jsonFlag,"\"message\" :",tab+2);
		OutputData(jsonFlag,"[",tab+2);

	
		int n = GetMessageGyo(message);
		for (int i=0;i<n;i++)
		{
			int ln = GetMessage1Gyo(i,message);
		//	if (ln > 0)
			if (1)
			{
				LPSTR message1gyo = m_tmpMessage;
				if (i < n-1)
				{
					wsprintf(mes,"\"%s\",",message1gyo);
				}
				else
				{
					wsprintf(mes,"\"%s\"",message1gyo);
				}
			}
			OutputData(jsonFlag,mes,tab+3);
		}

		OutputData(jsonFlag,"]",tab+2);

		OutputData(jsonFlag,"}",tab+1);
		OutputData(jsonFlag,"}",tab);
	}



	return TRUE;
}


int CMiniCompiler::GetMessageGyo(LPSTR mes)
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



int CMiniCompiler::GetMessage1Gyo(int n,LPSTR mes)
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


BOOL CMiniCompiler::Pass2SystemFunctionMessage(int func, LPSTR str)
{

	m_objectCode[0] = 5;
	m_objectCode[1] = IDENTIFY_SYSTEMFUNCTION;
	m_objectCode[2] = func;
	m_objectCode[3] = 1;
	m_objectCode[4] = m_strKosuu;

	AddStr(str);

	m_objectSize = 5;
	m_pass2OkFlag = TRUE;

	return TRUE;
}

BOOL CMiniCompiler::Pass2SystemFunctionLoad(int func, int n, LPSTR filename,FILE* jsonFlag,int tab)
{
	AddStr(filename);

	m_objectCode[0] = 5;
	m_objectCode[1] = IDENTIFY_SYSTEMFUNCTION;
	m_objectCode[2] = func;
	m_objectCode[3] = n;
	m_objectCode[4] = m_strKosuu-1;

	m_objectSize = 5;
	m_pass2OkFlag = TRUE;

	if (jsonFlag)
	{
		char mes[1024];
		LPSTR functionName = GetFunctionName(func);
		//if (n == 0)
		if (1)
		{
			wsprintf(mes,"{ \"type\" : \"%s\" , \"Param\" : {\"layer\" : %d , \"filename\" : \"%s\"} },",functionName,n,filename);
			OutputData(jsonFlag,mes,tab);
		}
	}

	return TRUE;
}



BOOL CMiniCompiler::Pass2SystemFunction(int func, int n, int* pData,FILE* jsonFlag,int tab)
{
	m_objectCode[0] = 4+n;
	m_objectCode[1] = IDENTIFY_SYSTEMFUNCTION;
	m_objectCode[2] = func;
	m_objectCode[3] = n;	//paraKosuu


	for (int i=0;i<n;i++)
	{
		m_objectCode[4+i] = *(pData+i);
	}

	m_objectSize = 4+n;
	m_pass2OkFlag = TRUE;

	if (jsonFlag != NULL)
	{
		char mes[1024];
		LPSTR functionName = GetFunctionName(func);
		if (n == 0)
		{
//			wsprintf(mes,"{ \"type\" : \"%s\" , \"Param\" : null },",functionName);
			wsprintf(mes,"{ \"type\" : \"%s\" },",functionName);
			OutputData(jsonFlag,mes,tab);
		}
		else
		{
			wsprintf(mes,"{ \"type\" : \"%s\" , \"Param\" : [%d",functionName,n);
			for (int i=0;i<n;i++)
			{
				char nummes[256];
				wsprintf(nummes,",%d",*(pData+i));
				strcat_s(mes,1024,nummes);
			}
			strcat_s(mes,1024,"] },");

			OutputData(jsonFlag,mes,tab);
		}
	}



	return TRUE;
}



BOOL CMiniCompiler::Pass2OpenChukakko(void)
{
	m_objectCode[0] = 4;
	m_objectCode[1] = IDENTIFY_CONTROL;
	m_objectCode[2] = COMMAND_OPENCHUKAKKO1;
	m_objectCode[3] = 4;

	m_objectSize = 4;
	m_pass2OkFlag = TRUE;

	return TRUE;
}

BOOL CMiniCompiler::Pass2CloseChukakko(void)
{
	m_objectCode[0] = 3;
	m_objectCode[1] = IDENTIFY_CONTROL;
	m_objectCode[2] = COMMAND_CLOSECHUKAKKO1;

	m_objectSize = 3;
	m_pass2OkFlag = TRUE;

	return TRUE;
}

BOOL CMiniCompiler::Pass2Debug(LPSTR mes)
{
	AddMes(mes);

	m_objectCode[0] = 4;
	m_objectCode[1] = IDENTIFY_SYSTEMFUNCTION;
	m_objectCode[2] = CODE_SYSTEMFUNCTION_DEBUG;
	m_objectCode[3] = m_mesKosuu-1;

	m_objectSize = 4;
	m_pass2OkFlag = TRUE;

	return TRUE;

}



BOOL CMiniCompiler::Pass2Sub(int n)
{
//	AddSub();

	m_objectCode[0] = 5;
	m_objectCode[1] = IDENTIFY_CONTROL;
	m_objectCode[2] = CODE_CONTROL_SUB;
	m_objectCode[3] = n;
	
	m_objectCode[4] = m_subKosuu;	//from number	ver 4.0

	if (m_subKosuu >= m_subTableSize)
	{
		return FALSE;
	}
	m_subTable[m_subKosuu] = m_pc;

	m_objectSize = 5;
	m_subKosuu++;

	m_pass2OkFlag = TRUE;

	return TRUE;
}

BOOL CMiniCompiler::Pass2Goto(int n)
{
	m_objectCode[0] = 4;
	m_objectCode[1] = IDENTIFY_CONTROL;
	m_objectCode[2] = CODE_CONTROL_GOTO;
	m_objectCode[3] = n;
	
	m_objectSize = 4;
	m_subKosuu++;

	m_pass2OkFlag = TRUE;

	return TRUE;
}


BOOL CMiniCompiler::Pass2Script(LPSTR mes,LPSTR checkSrcFileName)
{
	if (checkSrcFileName != NULL)
	{
		if (_stricmp(checkSrcFileName,"sys") != 0) return FALSE;
		if ((*checkSrcFileName) == '_') return FALSE;
	}



	AddStr(mes);

	m_objectCode[0] = 5;
	m_objectCode[1] = IDENTIFY_CONTROL;
	m_objectCode[2] = CODE_CONTROL_SCRIPT;
	m_objectCode[3] = m_strKosuu-1;
	m_objectCode[4] = m_scriptCallKosuu;

	if (m_scriptCallKosuu >= m_scriptCallTableSize)
	{
		return FALSE;
	}
	m_scriptCallTable[m_scriptCallKosuu] = m_pc;
	m_scriptCallKosuu++;

	m_objectSize = 5;
	m_pass2OkFlag = TRUE;

	return TRUE;
}


BOOL CMiniCompiler::Pass2ScriptJump(LPSTR mes,LPSTR checkSrcFileName)
{
	if (checkSrcFileName != NULL)
	{
		if (_stricmp(checkSrcFileName,"sys") == 0) return FALSE;

		if ((*checkSrcFileName) == '_')
		{
			if ((*mes) != '_') return FALSE;
		}
		else
		{
			if ((*mes) == '_') return FALSE;
		}
	}



	AddStr(mes);

	m_objectCode[0] = 4;
	m_objectCode[1] = IDENTIFY_CONTROL;
	m_objectCode[2] = CODE_CONTROL_SCRIPTJUMP;
	m_objectCode[3] = m_strKosuu-1;

	m_objectSize = 4;
	m_pass2OkFlag = TRUE;

	return TRUE;
}

BOOL CMiniCompiler::Pass2SubScript(LPSTR mes,LPSTR checkSrcFileName)
{
	if (checkSrcFileName != NULL)
	{
		if ((*checkSrcFileName) == '_')
		{
			if ((*(checkSrcFileName+1)) == '_')
			{
				return FALSE;//subsubからはよべない
			}

			//subからはsubsubしかよべない
			if ((*mes) != '_')
			{
				return FALSE;
			}

			if ((*(mes+1)) != '_')
			{
				return FALSE;
			}
		}
	}






	AddStr(mes);

	m_objectCode[0] = 5;
	m_objectCode[1] = IDENTIFY_CONTROL;
	m_objectCode[2] = CODE_CONTROL_SUBSCRIPT;
	m_objectCode[3] = m_strKosuu-1;
	m_objectCode[4] = m_scriptCallKosuu;

	if (m_scriptCallKosuu >= m_scriptCallTableSize)
	{
		return FALSE;
	}
	m_scriptCallTable[m_scriptCallKosuu] = m_pc;
	m_scriptCallKosuu++;

	m_objectSize = 5;
	m_pass2OkFlag = TRUE;

	return TRUE;
}

//たぶん未使用
BOOL CMiniCompiler::Pass2Message(int typ, LPSTR mes)
{
	//check max

	int ln = strlen(mes);
	int sz = (ln+1+3) / 4;

	m_objectCode[0] = 4;
	m_objectCode[1] = IDENTIFY_SYSTEMCOMMAND;
	m_objectCode[2] = typ;
m_objectCode[2] = CODE_SYSTEMCOMMAND_PRINT;
	m_objectCode[3] = m_mesKosuu;

	AddMes(mes);
//	m_mesTable[m_mesKosuu] = m_mesSize;
//	memcpy(m_mesPointer+m_mesSize,mes,ln+1);
//	m_mesSize += sz;
//	m_mesKosuu++;


	m_objectSize = 4;

	return TRUE;
}

BOOL CMiniCompiler::Pass2Case(int n)
{
	m_objectCode[0] = 4;
	m_objectCode[1] = IDENTIFY_CONTROL;
	m_objectCode[2] = CODE_CONTROL_CASE;
	m_objectCode[3] = n;

	m_objectSize = 4;

	return TRUE;
}


BOOL CMiniCompiler::Pass2Select(int typ, int serial, LPSTR mes,int selectSerial)
{
	int kosuu = 0;
	int mesHaveGyo = 0;
	BOOL selFoundFlag = FALSE;

	int timeLimit = 0;
	int autoSelect = 0;

	char buf[1024];

	int specialFlag = 0;

	while ((*mes) != 0)
	{
		//get message
		LPSTR ptr = mes;


		BOOL cmdFlag = FALSE;

		if ((*mes) == '%')
		{
			if (selFoundFlag == FALSE)
			{
				mesHaveGyo++;
			}
		}
		else if ((*mes) == '!')
		{
			specialFlag = 1;
			if (*(mes+1) == '!')
			{
				specialFlag = 2;
			}
			cmdFlag = TRUE;
		}
		else if ((*mes) == '#')
		{
			char ccc = *(mes+1);
			if ((ccc >= '0') && (ccc<='9'))
			{
				cmdFlag = TRUE;
				char* ptr1 = ptr;
				ptr1++;

				int ln1 = 0;
				while (((*ptr1) != 0x0d) && ((*ptr1) != 0) && (ln1<16))
				{
					ptr1++;
					ln1++;
				}

				char suuji[18];
				memcpy(suuji,ptr+1,ln1);
				suuji[ln1]=0;
				suuji[ln1+1]=0;
				timeLimit = atoi(suuji);
			}
			else
			{
				selFoundFlag = TRUE;
			}
		}
		else if ((*mes) == '?')
		{
			char varname[256];
			int ln1 = 0;
			char* ptr1 = ptr;
			ptr1++;
			while (((*ptr1) != 0x0d) && ((*ptr1) != 0) && (ln1<64))
			{
				varname[ln1] = *ptr1;
				ptr1++;
				ln1++;
			}
			varname[ln1] = 0;
			varname[ln1+1] = 0;

			cmdFlag = TRUE;

			int varnum = m_varList->GetNameNumber(varname);
			if (varnum == -1)
			{
				MessageBox(NULL,"選択時間として指定された変数が存在しません",varname,MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				timeLimit = -varnum;
			}
		}
		else if ((*mes) == '@')
		{
			cmdFlag = TRUE;
			char* ptr1 = ptr;
			ptr1++;

			int ln1 = 0;
			while (((*ptr1) != 0x0d) && ((*ptr1) != 0) && (ln1<16))
			{
				ptr1++;
				ln1++;
			}

			char suuji[18];
			memcpy(suuji,ptr+1,ln1);
			suuji[ln1]=0;
			suuji[ln1+1]=0;
			autoSelect = atoi(suuji);
		}
		else
		{
			selFoundFlag = TRUE;
		}


		int ln = 0;
		while (((*ptr) != 0x0d) && ((*ptr) != 0))
		{
			ptr++;
			ln++;
		}






		if (cmdFlag == FALSE)
		{
			memcpy(buf,mes,ln);
			buf[ln]=0;
			buf[ln+1]=0;
			m_objectCode[4+kosuu] = m_strKosuu;
			AddStr(buf);		
//			m_strTable[m_strKosuu] = m_strSize;
//			int sz = (ln+1+3) / 4;

//			*(m_strPointer+m_strSize+sz-1) = 0;
//			memcpy(m_strPointer+m_strSize,mes,ln);
//			m_strSize += sz;
			
//			m_objectCode[4+kosuu] = m_strKosuu;
//			m_strKosuu++;
			kosuu++;
		}

		if ((*ptr) == 0x0d)
		{
			ptr += 2;
		}

		mes = ptr;
	}

//	m_objectCode[0] = 5+kosuu;
//	m_objectCode[1] = IDENTIFY_SYSTEMCOMMAND;
//	m_objectCode[2] = CODE_SYSTEMCOMMAND_SELECT;
//	m_objectCode[3] = kosuu;
//	m_objectCode[4+kosuu] = serial;	//あとで追加したためこんなところにいれた
//	m_objectSize = 5+kosuu;

	int sentakushiKosuu = kosuu;
	sentakushiKosuu -= mesHaveGyo;

	if (sentakushiKosuu < 1)
	{
		MessageBox(NULL,"選択肢が存在しません","error",MB_OK);
		return FALSE;
	}

//	int selectSerial = 0;


	m_objectCode[0] = 6+kosuu+2+1+1;
	m_objectCode[1] = IDENTIFY_SYSTEMCOMMAND;
	m_objectCode[2] = CODE_SYSTEMCOMMAND_SELECT;
	m_objectCode[3] = sentakushiKosuu;
	m_objectCode[4+kosuu] = serial;	//あとで追加したためこんなところにいれた
	//うしろから使う

	m_objectCode[9+kosuu] = mesHaveGyo;
	m_objectCode[8+kosuu] = timeLimit;
	m_objectCode[7+kosuu] = autoSelect;
	m_objectCode[6+kosuu] = specialFlag;
	m_objectCode[5 + kosuu] = selectSerial;
	m_objectSize = 6+kosuu+2+1+1;


	if (serial >= m_selectTableSize)
	{
		return FALSE;
	}
	if (serial >= m_selectKosuu)
	{
		m_selectKosuu = serial;
	}
	m_selectTable[m_selectKosuu] = m_pc;
	m_selectKosuu++;

	return TRUE;
}


BOOL CMiniCompiler::Pass2Voice(LPSTR filename,int ch,int paraKosuu,int* pPara,int useDef,FILE* jsonFlag,int tab)
{
	m_objectSize = 4 + 1 + 1 + paraKosuu + 1;
	
	m_objectCode[0] = m_objectSize;
	m_objectCode[1] = IDENTIFY_SYSTEMFUNCTION;
	m_objectCode[2] = CODE_SYSTEMFUNCTION_VOICE;
	m_objectCode[3] = 1+1+paraKosuu;

	if (filename == NULL)
	{
		m_objectCode[4] = -1;
	}
	else
	{
		m_objectCode[4] = m_strKosuu;
		AddStr(filename);
	}

	m_objectCode[5] = ch;

	for (int i=0;i<paraKosuu;i++)
	{
		m_objectCode[6+i] = *(pPara+i);
	}

	m_objectCode[6+paraKosuu] = useDef;//default voice wo fukumu


	if (jsonFlag != NULL)
	{
		char mes[1024];

		int n = paraKosuu;
		OutputData(jsonFlag,"{ \"type\" : \"voice\" , \"Param\" : ",tab);
		OutputData(jsonFlag,"{",tab+1);


		if (filename != NULL)
		{
			wsprintf(mes,"\"filename\" : \"%s\" ,",filename);
		}
		else
		{
			wsprintf(mes,"\"filename\" : null ,");
		}
		OutputData(jsonFlag,mes,tab+2);

		wsprintf(mes,"\"useDef\" : %d ,",useDef);
		OutputData(jsonFlag,mes,tab+2);



		wsprintf(mes,"\"voiceParam\" : [%d",n+2);
		for (int i=0;i<n+2;i++)
		{
			char nummes[256];
			wsprintf(nummes,",%d",m_objectCode[4+i]);
			strcat_s(mes,1024,nummes);
		}
		strcat_s(mes,1024,"]");

		OutputData(jsonFlag,mes,tab+2);
		OutputData(jsonFlag,"}",tab+1);



		OutputData(jsonFlag,"},",tab);

	}




	return TRUE;
}

BOOL CMiniCompiler::Pass2VoiceFlag(int flagNumber,FILE* jsonFlag,int tab)
{
	m_objectSize = 6;
	
	m_objectCode[0] = m_objectSize;
	m_objectCode[1] = IDENTIFY_SYSTEMFUNCTION;
	m_objectCode[2] = CODE_SYSTEMFUNCTION_SETVOICEFLAG;
	m_objectCode[3] = 2;
	m_objectCode[4] = flagNumber;
	m_objectCode[5] = 0;//拡張用



	if (jsonFlag != NULL)
	{
		char mes[1024];
		LPSTR functionName = GetFunctionName(CODE_SYSTEMFUNCTION_SETVOICEFLAG);
		int n = 2;
		wsprintf(mes,"{ \"type\" : \"%s\" , \"Param\" : [%d",functionName,n);
		for (int i=0;i<n;i++)
		{
			char nummes[256];
			wsprintf(nummes,",%d",m_objectCode[4+i]);
			strcat_s(mes,1024,nummes);
		}
		strcat_s(mes,1024,"] },");
		OutputData(jsonFlag,mes,tab);
	}


	return TRUE;
}


BOOL CMiniCompiler::Pass2Sound(int seNum,int ch,int paraKosuu,int* pPara,FILE* jsonFlag ,int tab)
{
	m_objectSize = 4 + 1 + 1 + paraKosuu;
	

	for (int i = 0; i < 32; i++)
	{
		m_objectCode[i] = 0;
	}
	m_objectCode[0] = m_objectSize;
	m_objectCode[1] = IDENTIFY_SYSTEMFUNCTION;
	m_objectCode[2] = CODE_SYSTEMFUNCTION_SOUND;
	m_objectCode[3] = 1+1+paraKosuu;

	m_objectCode[4] = seNum;
	m_objectCode[5] = ch;

	for (int i=0;i<paraKosuu;i++)
	{
		m_objectCode[6+i] = *(pPara+i);
	}


	if (jsonFlag != NULL)
	{
		char mes[1024];
		LPSTR functionName = GetFunctionName(CODE_SYSTEMFUNCTION_SOUND);
		int n = paraKosuu+2;
		wsprintf(mes,"{ \"type\" : \"%s\" , \"Param\" : [%d",functionName,n);
		for (int i=0;i<n;i++)
		{
			char nummes[256];
			wsprintf(nummes,",%d",m_objectCode[4+i]);
			strcat_s(mes,1024,nummes);
		}
		strcat_s(mes,1024,"] },");
		OutputData(jsonFlag,mes,tab);
	}

	return TRUE;
}


BOOL CMiniCompiler::Pass2SimpleData(int subData)
{
	m_objectSize = 3;
	m_pass2OkFlag = TRUE;

	m_objectCode[0] = m_objectSize;
	m_objectCode[1] = IDENTIFY_DATA;
	m_objectCode[2] = subData;

	return TRUE;
}

BOOL CMiniCompiler::Pass2Simple(int typ,FILE* jsonFlag,int tab)
{
	m_objectSize = 3;
	m_pass2OkFlag = TRUE;

	m_objectCode[0] = 3;
	m_objectCode[1] = IDENTIFY_CONTROL;


	if (jsonFlag != NULL)
	{
		char mes[1024];
		LPSTR functionName = GetFunctionName(typ);
//		wsprintf(mes,"{ \"type\" : \"%s\" , \"Param\" : null },",functionName);
		wsprintf(mes,"{ \"type\" : \"%s\" },",functionName);
		OutputData(jsonFlag,mes,tab);
	}





	if (typ == COMMANDDATATYPE_NOP)
	{
		m_objectCode[2] = CODE_CONTROL_NOP;
		return TRUE;
	}

	if (typ == COMMANDDATATYPE_RET)
	{
		m_objectCode[2] = CODE_CONTROL_RETURN;
		return TRUE;
	}

	if (typ == COMMANDDATATYPE_NEXT)
	{
		m_objectCode[2] = CODE_CONTROL_NEXT;
		return TRUE;
	}

	if (typ == COMMANDDATATYPE_END)
	{
		m_objectCode[2] = CODE_CONTROL_END;
		return TRUE;
	}

	if (typ == COMMANDDATATYPE_EXIT)
	{
		m_objectCode[2] = CODE_CONTROL_EXIT;
		return TRUE;
	}

	if (typ == COMMANDDATATYPE_ELSE)
	{
		m_objectCode[2] = CODE_CONTROL_ELSE;
		return TRUE;
	}

	if (typ == COMMANDDATATYPE_ENDFILM)
	{
		m_objectCode[2] = CODE_CONTROL_ENDFILM;
		return TRUE;
	}

	if (typ == COMMANDDATATYPE_ENDKOMA)
	{
		m_objectCode[2] = CODE_CONTROL_ENDKOMA;
		return TRUE;
	}

	if (typ == COMMANDDATATYPE_ENDIF)
	{
		m_objectCode[2] = CODE_CONTROL_ENDIF;
		return TRUE;
	}


	m_pass2OkFlag = FALSE;
	return FALSE;
}


BOOL CMiniCompiler::Pass2If(int code)
{
	if (m_pass1OkFlag == FALSE)
	{
		MessageBox(NULL,"Pass1が成功していません","error",MB_OK);
		return FALSE;
	}

	wsprintf(m_errorCode,"compile OK");
	wsprintf(m_errorMessage,"no error");

	int n = 0;
	int pc = 0;


	n++;

	int k = KoubunKaiseki(n);

	if ((k<=0) || (k>256))
	{
		Pass2Error(n,"なんか長いか短い");
		return FALSE;
	}



	// set
	m_objectCode[pc] = 3+m_kaisekiKekkaLength[0];
	m_objectCode[pc+1] = IDENTIFY_CONTROL;
	m_objectCode[pc+2] = code;//CODE_CONTROL_IF;


//			int shurui = token & 0xffff0000;
//			int dat = token & 0x0000ffff;
//			int shuruicmd = token & 0xf0000000;

	//set data
	BOOL compareFound = FALSE;
	for (int i=0;i<m_kaisekiKekkaLength[0];i++)
	{
		int kdt = m_kaisekiData[i];
		if (compareFound == FALSE)
		{
			int shurui = kdt & 0xf0000000;
			int subShurui = kdt & 0xffff0000;
			int dat = kdt & 0x0000ffff;

			if (shurui == SCRIPT_CALCU) 
			{
				if ((subShurui == SCRIPT_CALCU_DOUBLE) && ((dat == '=') || (dat == '&') || (dat == '|') || (dat == '^'))) compareFound = TRUE;
				if ((subShurui == SCRIPT_CALCU_COMPARE) || (subShurui == SCRIPT_CALCU_COMPAREEQUAL)) compareFound = TRUE;
			}



		}

		m_objectCode[pc+3+i] = kdt;
	}

	if ((m_kaisekiKekkaLength[0]>=3) && (compareFound == FALSE))
	{
		MessageBox(NULL,"IFの中に比較演算子がにゃい","警告(現時点では全てエラー)",MB_OK);
		return FALSE;
		//warning!
	}



	pc += m_objectCode[pc];

	m_objectSize = pc;
	m_pass2OkFlag = TRUE;

	return TRUE;
}



//len,sys or func, num, paraKosuu, paras
BOOL CMiniCompiler::Pass2Func(int funcNum,int type)
{
	if (m_pass1OkFlag == FALSE)
	{
		MessageBox(NULL,"Pass1が成功していません","error",MB_OK);
		return FALSE;
	}

	wsprintf(m_errorCode,"compile OK");
	wsprintf(m_errorMessage,"no error");

	int n = 0;
	int pc = 0;

//	//最初ファンクション又はコマンドかどうか
//	int token = m_token[n];
//	int shurui = token  & 0xf0000000;
//	int dat = token & 0xffff;
//	if ((shurui != TOKEN_FUNCTION) && (shurui != TOKEN_SYSTEMCOMMAND))
//	{
//		Pass2Error(n,"Error not function and systemcommand");
//		return FALSE;
//	}
//
//	n++;

	int k = KoubunKaiseki(n,FALSE,m_tokenKosuu);

	if ((k<0) || (k>256))
	{
		Pass2Error(n,"なんか長いか短い");
		return FALSE;
	}




	// set
	int sz = 0;
	for (int i=0;i<m_kaisekiKosuu;i++)
	{
		sz += m_kaisekiKekkaLength[i];
	}

	m_objectCode[pc] = 4+sz;


	if (type == 0)
	{
		m_objectCode[pc+1] = IDENTIFY_FUNCTION;
	}
	else
	{
		m_objectCode[pc+1] = IDENTIFY_COMMAND;
		m_objectCode[pc] += 1;

		if (m_commandCallKosuu >= m_commandCallTableSize)
		{
			return FALSE;
		}
		m_objectCode[pc+4+sz] = m_commandCallKosuu;	//last is id
		m_commandCallTable[m_commandCallKosuu] = m_pc;
		m_commandCallKosuu++;
	}

	m_objectCode[pc+2] = funcNum;
	m_objectCode[pc+3] = m_kaisekiKosuu;

	//set data
	int kk = 0;
	for (int j=0;j<m_kaisekiKosuu;j++)
	{
		int st = m_kaisekiStart[j];

//		m_objectCode[pc+4+kk] = m_kaisekiKekkaLength[j];
//		kk++;

		for (int i=0;i<m_kaisekiKekkaLength[j];i++)
		{
			m_objectCode[pc+4+kk] = m_kaisekiData[st+i];
			kk++;
		}
	}


	pc += m_objectCode[pc];

	m_objectSize = pc;
	m_pass2OkFlag = TRUE;

	return TRUE;

}


BOOL CMiniCompiler::Pass2Calcu(void)
{
	if (m_pass1OkFlag == FALSE)
	{
		MessageBox(NULL,"Pass1が成功していません","error",MB_OK);
		return FALSE;
	}

	wsprintf(m_errorCode,"compile OK");
	wsprintf(m_errorMessage,"no error");


	int n = 0;
	int pc = 0;


	while (n<m_tokenKosuu)
	{
		int token = m_token[n];
		int shurui = token  & 0xf0000000;
		int dat = token & 0xffff;

		n++;

		if (token == TOKEN_SEPARATER)
		{
			continue;
		}


		if (shurui == TOKEN_VAR)
		{
			int k = KoubunKaiseki(n-1,TRUE);

			if ((k<2) || (k>=256) || (m_kaisekiKekkaLength[0] <= 1))
			{
				Pass2Error(n,"計算式に問題がありますにゃ");
				return FALSE;
			}


			m_objectCode[pc] = 3 + m_kaisekiKekkaLength[0];
			m_objectCode[pc+1] = IDENTIFY_CALCU;
			m_objectCode[pc+2] = 0;	//未定
			
			SetKaisekiKekka(pc+3,0);

			pc += m_objectCode[pc];
			n += k-1;
			continue;
		}


		wsprintf(m_errorCode,"構文エラー in %d",m_tokenGyo[n-1]);
		memcpy(m_errorMessage,m_fileBuffer + m_tokenSource[n-1],158);
		m_errorMessage[158] = 0;
		m_errorMessage[159] = 0;
		return FALSE;
	}

//	最終飛びさき決定
	
	m_objectSize = pc;

	m_pass2OkFlag = TRUE;

	return TRUE;
}

/*
BOOL CMiniCompiler::Pass2(void)
{
	if (m_pass1OkFlag == FALSE)
	{
		MessageBox(NULL,"Pass1が成功していません","error",MB_OK);
		return FALSE;
	}

	sprintf(m_errorCode,"compile OK");
	sprintf(m_errorMessage,"no error");


	int n = 0;


	int pc = 0;

//	m_objectCode[pc+0] = 4;
//	m_objectCode[pc+1] = SCRIPT_TABLE | 0;	//messagetable1
//	m_objectCode[pc+2] = 0;	//kosuu
//	m_objectCode[pc+3] = 0;	//ptr
//	pc += m_objectCode[pc];

//	m_objectCode[pc+0] = 4;
//	m_objectCode[pc+1] = SCRIPT_TABLE | 1;	//messagetable1
//	m_objectCode[pc+2] = 0;	//kosuu
//	m_objectCode[pc+3] = 0;	//ptr
//	pc += m_objectCode[pc];

	while (n<m_tokenKosuu)
	{
		if (pc<65535) m_objToToken[pc] = n;

		int token = m_token[n];
		int shurui = token  & 0xf0000000;
		int dat = token & 0xffff;

		n++;

		if (token == TOKEN_SEPARATER)
		{
			continue;
		}



//////////////command
		if (shurui == TOKEN_COMMAND)
		{
			if (dat == COMMAND_NOP)
			{
				m_objectCode[pc] = 2;
				m_objectCode[pc+1] = SCRIPT_COMMAND_NOP;
				pc += m_objectCode[pc];
				continue;
			}

			if ( (dat == COMMAND_IF) || (dat == COMMAND_ELSIF) || (dat == COMMAND_DRAW))	//if および elsif wait
			{
				if (m_token[n] != (TOKEN_KAKKO | '('))
				{
					Pass2Error(n);
					return FALSE;
				}

				n++;

				int k = KoubunKaiseki(n);

				if ((k<=0) || (k>256))
				{
					Pass2Error(n,"なんか長いか短い");
					return FALSE;
				}

				if (m_kaisekiKosuu != 1)
				{
					Pass2Error(n,"条件のなかがへん");
					return FALSE;
				}

				//set data
				for (int i5 = 0;i5<m_kaisekiKekkaLength[0];i5++)
				{
					m_objectCode[pc+2+i5] = m_kaisekiData[i5];
				}


				//etc set
				m_objectCode[pc] = 2+m_kaisekiKekkaLength[0];
				m_objectCode[pc+1] = SCRIPT_COMMAND | dat;
				pc += m_objectCode[pc];
				n += k;
				n += 1;	//とじかっこのぶん
				continue;
			}



			Pass2Error(n);
		}



/////////////////function
		if (shurui == TOKEN_FUNCTION)
		{
			int token1 = m_token[n];
			int shurui1 = token1 & 0xffff0000;
			int dat1 = token1 & 0xffff;
			n++;

			if (token1 != (TOKEN_KAKKO | '('))
			{
				Pass2Error(n,"関数には()が必要ですにゃ");
				return FALSE;
			}

			int k = KoubunKaiseki(n);




			if (k < 0)
			{
				Pass2Error(n,"関数に問題あり");
				return FALSE;
			}

			//check parakosuu and paratype


			for (int i7=0;i7<m_kaisekiKosuu;i7++)
			{
//				int tp = m_functionName[dat].paraType[i7];
				int tp = 0;	//dummy


				int kd = m_kaisekiData[m_kaisekiStart[i7]];





				int cmd7 = kd & 0xffff0000;
				
				if (tp == 1)
				{
					//文字列だったらだめ
					if (m_kaisekiKekkaLength[i7] == 1)
					{
						if ((cmd7 == TOKEN_MESSAGE) || (cmd7 == TOKEN_MESSAGE2))
						{
							Pass2Error(n,"関数のパラメーターとして文字列はつかえないところがあるよ");
							return FALSE;
						}
					}
				}

				if (tp == 2)
				{
					//数値もしくは式
					if (m_kaisekiKekkaLength[i7] != 1)
					{
						Pass2Error(n,"パラメーターを文字列にしてください");
						return FALSE;
					}

					if ((cmd7 != TOKEN_MESSAGE) && (cmd7 != TOKEN_MESSAGE2))
					{
						Pass2Error(n,"パラメーターを文字列にしてください");
						return FALSE;
					}
				}

			}




			int fsz = 0;
			for (int i8=0;i8<m_kaisekiKosuu;i8++)
			{
				fsz += m_kaisekiKekkaLength[i8];
			}
			m_objectCode[pc] = 3+fsz;
			m_objectCode[pc+1] = SCRIPT_FUNCTION | dat;
			m_objectCode[pc+2] = m_kaisekiKosuu;

			int k8 = 0;
			for (int j8=0;j8<m_kaisekiKosuu;j8++)
			{
				int ln8 = m_kaisekiKekkaLength[j8];
				for (int i8=0;i8<ln8;i8++)
				{
					m_objectCode[pc+3+k8] = m_kaisekiData[m_kaisekiStart[j8] + i8];
					k8++;
				}
//				k8 += m_kaisekiLebgth[j8];
			}

			pc += m_objectCode[pc];
			n += (k+1);
			continue;
		}


		if (shurui == TOKEN_VAR)
		{
			int k = KoubunKaiseki(n-1,TRUE);






			if ((k<2) || (k>=256) || (m_kaisekiKekkaLength[0] <= 1))
			{
				Pass2Error(n,"計算式に問題がありますにゃ");
				return FALSE;
			}


			m_objectCode[pc] = 2 + m_kaisekiKekkaLength[0];
			m_objectCode[pc+1] = SCRIPT_CALCU;	//きょうつう
			
			SetKaisekiKekka(pc+2,0);

			pc += m_objectCode[pc];
			n += k-1;
			continue;
		}

		sprintf(m_errorCode,"構文エラー in %d",m_tokenGyo[n-1]);
		memcpy(m_errorMessage,m_fileBuffer + m_tokenSource[n-1],158);
		m_errorMessage[158] = 0;
		m_errorMessage[159] = 0;
		return FALSE;
	}

//	最終飛びさき決定
	

	
	int ppc = 0;
	int kakko = 0;


	m_objectCode[pc] = 0;
	pc++;


	m_objectSize = pc;

	m_pass2OkFlag = TRUE;

	return TRUE;
}
*/



void CMiniCompiler::Pass2Error(int n, LPSTR mes)
{
		wsprintf(m_errorCode,"構文エラー in %d",m_tokenGyo[n]);
		memcpy(m_errorMessage,m_fileBuffer + m_tokenSource[n],158);
		m_errorMessage[158] = 0;
		m_errorMessage[159] = 0;
		if (mes == NULL)
		{
			wsprintf(m_errorMessage2,"");
		}
		else
		{
			wsprintf(m_errorMessage2,mes);
		}
}




char* CMiniCompiler::GetErrorCode(void)
{
	return m_errorCode;
}

char* CMiniCompiler::GetErrorMessage(void)
{
	return m_errorMessage;
}

char* CMiniCompiler::GetErrorMessage2(void)
{
	return m_errorMessage2;
}




// return length ,if error return 0 or minus
//
// + - * / % = ( ) [ ] { } += -= *= /= %= space tab , ;  ret //
//
/*
int CMiniCompiler::GetToken(LPSTR* pStrSrc, LPSTR strDest, int bufferMax)
{
	unsigned char* src = (unsigned char*)*pStrSrc;
	unsigned char* dst = (unsigned char*)strDest;
	int ln = 0;

	unsigned char c;

	c = 0;
//	while (1);



	return 0;
}
*/


// if end return FALSE
BOOL CMiniCompiler::SkipSpace(LPSTR* pStrSrc)
{
	unsigned char* src = (unsigned char*)*pStrSrc;
	unsigned char c;

	while ((c = *src++) != 0)
	{
		if (c >= 0x80)
		{
			if ((c = *src++) == 0)
			{
				*pStrSrc = (char*)src;
				return FALSE;
			}
			else
			{
				if (c>=0x21)
				{
					*pStrSrc = (char*)src;
					return TRUE;
				}
			}
		}
	}

	*pStrSrc = (char*)src;
	return FALSE;
}


// if error return false
BOOL CMiniCompiler::SkipComment(LPSTR* pStrSrc)
{
	unsigned char* src = (unsigned char*)*pStrSrc;
	unsigned char c;

	while ((c = *src++) != 0xa)
	{
		if (c == 0)
		{
			*pStrSrc = (char*)src;
			return FALSE;
		}
	}

	*pStrSrc = (char*)src;
	return TRUE;
}



void CMiniCompiler::AddErrorPass0(LPSTR errorMessage, int dataPtr)
{
		wsprintf(m_errorCode,"errorPass0 in %d %s",m_gyo,errorMessage);
		memcpy(m_errorMessage,m_fileBuffer + dataPtr,158);
		m_errorMessage[158] = 0;
		m_errorMessage[159] = 0;
}

void CMiniCompiler::AddErrorPass1(LPSTR errorMessage, int dataPtr)
{
		wsprintf(m_errorCode,"errorPass1 in %d %s",m_gyo,errorMessage);
		memcpy(m_errorMessage,m_fileBuffer + dataPtr,158);
		m_errorMessage[158] = 0;
		m_errorMessage[159] = 0;

}

void CMiniCompiler::AddError(LPSTR errorMessage, int dataPtr)
{
		wsprintf(m_errorCode,"Error in %d %s",m_gyo,errorMessage);
		memcpy(m_errorMessage,m_fileBuffer + dataPtr,158);
		m_errorMessage[158] = 0;
		m_errorMessage[159] = 0;

}




/*
#define TOKEN_VAR SCRIPT_VAR
#define TOKEN_NUMBER SCRIPT_NUMBER
#define TOKEN_CALCU SCRIPT_CALCU
//#define TOKEN_DAINYU
#define TOKEN_MESSAGE SCRIPT_MESSAGE
#define TOKEN_MESSAGE2 SCRIPT_MESSAGE2
#define TOKEN_BEGINEND SCRIPT_BEGINEND

#define TOKEN_KAKKO (TOKEN_CALCU | 0x01000000)
#define TOKEN_CALCUEQUAL (TOKEN_CALCU | 0x02000000)
#define TOKEN_DOUBLE (TOKEN_CALCU | 0x03000000)
#define TOKEN_LESSEQUAL (TOKEN_CALCU | 0x04000000)
#define TOKEN_GREATEQUAL (TOKEN_CALCU | 0x05000000)
*/

int CMiniCompiler::GetJuni(int token)
{
	if (token == TOKEN_KAISEKISTART) return 1;
	if (token == TOKEN_KAISEKIEND) return 2;

	int cmd = token & 0xffff0000;
	int dat = token & 0x0000ffff;

	if ((cmd == TOKEN_VAR) || (cmd == TOKEN_NUMBER) || (cmd == TOKEN_NUMBER32)) return 0;


//	if (cmd == TOKEN_CALCU) return -1; //unknown


	if (cmd == TOKEN_KAKKO)
	{
		if (dat == '(') return 3;
		if (dat == ')') return 4;
		return -1;	//bad
	}

	if (cmd == TOKEN_CALCU)
	{
		if (dat == '!') return 5;
	}


	if (cmd == TOKEN_CALCU)
	{
		if ((dat == '*') || (dat == '/') || (dat == '%')) return 6;
		if ((dat == '+') || (dat == '-')) return 7;
	}

	if (cmd == TOKEN_COMPARE)
	{
		if ((dat == '<') || (dat == '>')) return 9;
	}

	if (cmd == TOKEN_COMPAREEQUAL)
	{
		if ((dat == '<') || (dat == '>')) return 9;
		if ((dat == '=') || (dat == '!')) return 10;

	}

	if (cmd == TOKEN_CALCU)
	{
		if (dat == '&') return 11;
		if (dat == '^') return 12;
		if (dat == '|') return 13;
	}


	if (cmd == TOKEN_DOUBLE)
	{
		if (dat == '&') return 14;
		if (dat == '^') return 15;
		if (dat == '|') return 16;
	}

//	if (cmd == TOKEN_DAINYU) return 18;
	if (cmd == TOKEN_CALCUEQUAL) return 18;

	if (cmd == TOKEN_CALCU)
	{
		if (dat == '=') return 18;
	}

	return -1	;//unknown

}

int CMiniCompiler::GetJuniTable(int from, int to)
{
	if ((from<0) || (from>=20) || (to<0) || (to>=20)) return 0;

	if ((from<=4) && (to<=4))
	{
		return m_juniTable[from][to];
	}

	if ((from>=5) && (to>=5))
	{
		if (from >= to) return 1;
		return 3;
	}

	if (from<=4)
	{
		return m_juniTableFrom[from];
	}

	return m_juniTableTo[to];

	return 0;
}

int CMiniCompiler::KoubunKaiseki(int n,BOOL bVarFlag, int sz)
{
	int k;

	m_kaisekiKosuu = 0;

	if (sz == -1)
	{
		if (bVarFlag)
		{
			k = SearchToken(n,TOKEN_SEPARATER);
		}
		else
		{
			k = SearchToken(n,TOKEN_KAKKO | ')',TOKEN_KAKKO | '(');
		}
	}
	else
	{
		k = sz;
	}


	int ptr = 0;
	//checkしながらセットする
	for (int i=0;i<k;i++)
	{
		int d = m_token[n];
		int checkShurui = d & 0xf0000000;

//		if (checkShurui == TOKEN_SEPARATER)
		if (d == TOKEN_SEPARATER)
		{
			//解析
			if (ptr > 0)
			{
				int ln = KaisekiSub(ptr);
				if (ln<0) return -1;

				ptr = 0;
			}

			n ++;
			continue;
		}


		if ((checkShurui != TOKEN_VAR) && (checkShurui != TOKEN_NUMBER) && (checkShurui != TOKEN_NUMBER32) && (checkShurui != TOKEN_CALCU) && (checkShurui != TOKEN_MESSAGE) && (checkShurui != TOKEN_MESSAGE2) )
		{
			return -1;
		}

		if (ptr>0)
		{
			int d0 = m_token[n-1];
			int checkShurui0 = d0 & 0xf0000000;

			if (checkShurui == TOKEN_CALCU)
			{
				if (d == (TOKEN_KAKKO | '('))
				{
					if (d0 == (TOKEN_KAKKO | ')')) return -1;
					if ((checkShurui == TOKEN_VAR) || (checkShurui == TOKEN_NUMBER) || (checkShurui == TOKEN_NUMBER32)) return -1;
				}


				if (d == (TOKEN_KAKKO | ')'))
				{
					if ((checkShurui0 != TOKEN_VAR) && (checkShurui0 != TOKEN_NUMBER) && (checkShurui0 != TOKEN_NUMBER32))
					{
						if (d0 != (TOKEN_KAKKO | ')'))
						{
							return -1;
						}
					}
				}

				if ((d != (TOKEN_KAKKO | '(')) && (d != (TOKEN_KAKKO | ')')))
				{
					if (checkShurui0 == TOKEN_CALCU)
					{
						if (d0 != (TOKEN_KAKKO | ')'))
						{
							return -1;
						}
					}
				}



			}

			if ((checkShurui == TOKEN_VAR) || (checkShurui == TOKEN_NUMBER) || (checkShurui == TOKEN_NUMBER32))
			{
				if ((checkShurui0 == TOKEN_VAR) || (checkShurui0 == TOKEN_NUMBER) || (checkShurui0 == TOKEN_NUMBER32)) return -1;
			}

		}


		m_kaisekiSrc[ptr] = d;
		ptr++;
		n++;
	}

	if (ptr>0)
	{
		int ln = KaisekiSub(ptr);
		if (ln<0) return -1;
	}

	return k;
}

int CMiniCompiler::KaisekiSub(int n)
{
	int dst = 0;
	for (int i=0;i<m_kaisekiKosuu;i++)
	{
		dst += m_kaisekiKekkaLength[i];
	}
	m_kaisekiStart[m_kaisekiKosuu] = dst;

	int dst000 = dst;

	int ln = 0;

	int kekka = 0;

	if (n == 1)
	{
		m_kaisekiData[dst] = m_kaisekiSrc[0];
		ln = 1;
		kekka = 1;
	}
	else
	{
		m_kaisekiData[dst] = SCRIPT_CALCU;
		dst++;
		ln++;
		kekka++;
		m_kaisekiData[dst] = n;	//dummy
		dst++;
		ln++;
		kekka++;

		m_kaisekiSrc[n] = TOKEN_KAISEKIEND;
		n++;
	
		for (int i=0;i<n;i++)
		{
			m_kaisekiType[i] = GetJuni(m_kaisekiSrc[i]);
		}

		m_tana[0] = TOKEN_KAISEKISTART;
		m_tana[1] = GetJuni(m_tana[0]);


		int src = 0;
		int tana = 1;

		while (src<n)
		{
			int token = m_kaisekiSrc[src];
			src++;

			int shurui = token & 0xffff0000;
			int dat = token & 0x0000ffff;
			int cmd = token & 0xf0000000;

			if ((shurui == TOKEN_VAR) || (shurui == TOKEN_NUMBER) || (shurui == TOKEN_NUMBER32))
			{
				m_kaisekiData[dst] = token;
				dst++;
				ln++;
				kekka++;

/*				
if (shurui == TOKEN_VAR)
{
OutputDebugString(" 解析出力[i] ");
}
else
{
OutputDebugString(" 解析出力[n] ");
}
*/
				continue;
			}

			if ((cmd == TOKEN_CALCU) || (token == TOKEN_KAISEKIEND))	//
			{
				if (tana<=0) return -1;

				int juni = m_kaisekiType[src-1];
				int tanaJuni = m_tana[(tana-1)*2+1];

				int chk = GetJuniTable(tanaJuni,juni);
				
				if (chk == 3)
				{
					do
					{
						tana--;
						if (tana<=0) return -1;

						int dbg = m_tana[tana*2] & 0xffff;

						m_kaisekiData[dst] = m_tana[tana*2];
						dst++;
						ln++;
						kekka++;
/*
if (dbg == '=')
{
OutputDebugString(" 解析出力[=] ");
}
else if (dbg == '+')
{
OutputDebugString(" 解析出力[+] ");
}
else if (dbg == '*')
{
OutputDebugString(" 解析出力[*] ");
}
else
{
OutputDebugString(" 解析出力[?] ");
}
*/

						tanaJuni = m_tana[(tana-1)*2+1];
						chk = GetJuniTable(tanaJuni,juni);
					}
					while (chk == 3);

//					continue;
				}

				if (chk == 1)
				{
					m_tana[tana*2] = token;
					m_tana[tana*2+1] = juni;
					tana++;
//OutputDebugString("棚に積む ");
					if (tana>256) return -1;
					continue;
				}

				if (chk == 2)
				{
					if (tanaJuni == 3)
					{
						tana--;
//OutputDebugString("棚からおろす ");
						if (tana <= 0) return -1;
						continue;

					}

					if (tanaJuni == 1)
					{
						continue;
					}
				}


				return -1;	//error

			}

			return -1;
		}

		m_kaisekiData[dst000+1] = kekka - 2;
	
	}



	m_kaisekiLength[m_kaisekiKosuu] = ln;
	m_kaisekiKekkaLength[m_kaisekiKosuu] = kekka;
	m_kaisekiKosuu++;

	return ln;
}




int CMiniCompiler::GetFunctionNumber(LPSTR name)
{
	return m_functionList->GetNameNumber(name,2);
}

int CMiniCompiler::GetSystemCommandNumber(LPSTR name)
{
	return m_commandList->GetNameNumber(name,2);
}


void CMiniCompiler::SetKaisekiKekka(int pc, int n)
{
	int st = m_kaisekiStart[n];
	int ln = m_kaisekiKekkaLength[n];

	for (int i=0;i<ln;i++)
	{
		m_objectCode[pc+i] = m_kaisekiData[st+i];
	}
}



void CMiniCompiler::Init(void)
{
//	m_strKosuu = 0;
//	m_mesKosuu = 0;
//	m_filmKosuu = 0;
//	m_storyKosuu = 0;
}


void CMiniCompiler::SetFunctionList(CNameList* functionList)
{
	m_functionList = functionList;
}

void CMiniCompiler::SetCommandList(CNameList* commandList)
{
	m_commandList = commandList;
}

void CMiniCompiler::SetVarList(CNameList* varList)
{
	m_varList = varList;
}

void CMiniCompiler::SetFilmList(CNameList* filmList)
{
	m_filmList = filmList;
}

void CMiniCompiler::SetStoryList(CNameList* storyList)
{
	m_storyList = storyList;
}



int CMiniCompiler::GetObjectSize(void)
{
	return m_objectSize;
}

int* CMiniCompiler::GetObjectPointer(void)
{
	return m_objectCode;
}


void CMiniCompiler::StartBuild(int targetMachine)
{
	m_targetMachine = targetMachine;

	m_pc = 0;
//	m_strPointer = 0;
//	m_mesPointer = 0;
	m_strKosuu = 0;
	m_mesKosuu = 0;
	m_mesSize = 0;
	m_strSize = 0;

	m_subKosuu = 0;
	m_largeConstKosuu = 0;

	m_selectKosuu = 0;
	m_scriptCallKosuu = 0;
	m_commandCallKosuu = 0;
	
	//header
	m_makedCode[0] = 32;
	m_makedCode[1] = IDENTIFY_DATA;
	m_makedCode[2] = CODE_DATA_HEADER0;
	m_makedCode[3] = 0;	//allSize

	m_makedCode[4] = 0;	//mesKosuu
	m_makedCode[5] = 0;	//mesTableへのポインター
	m_makedCode[6] = 0;	//strKosuu
	m_makedCode[7] = 0;	//strTableへのポインター
	m_makedCode[8] = 0;	//labelKosuu
	m_makedCode[9] = 0;	//labelTableへのポインター
	m_makedCode[10] = 0;	//ラベルのうちストーリー個数
	m_makedCode[11] = 0;	//dummy

	memcpy(&m_makedCode[12],"SPTHEADER0     ",16);

	m_makedCode[16] = 0;	//こまのフィルムごとのオフセット
	m_makedCode[17] = 0;	//こまジャンプテーブルへのポインター

	m_makedCode[18] = 0;	//dummy
	m_makedCode[19] = 0;	//dummy

	m_makedCode[20] = 0;	//dummy sub
	m_makedCode[21] = 0;	//dummy select
	m_makedCode[22] = 0;	//dummy command
	m_makedCode[23] = 0;	//dummy script
	m_makedCode[24] = 0;	//dummy
	m_makedCode[25] = 0;	//dummy
	m_makedCode[26] = 0;	//dummy
	m_makedCode[27] = 0;	//dummy
	m_makedCode[28] = 0;	//dummy
	m_makedCode[29] = 0;	//dummy
	m_makedCode[30] = 0;	//dummy
	m_makedCode[31] = 0;	//dummy

	m_pc += m_makedCode[m_pc];
}


void CMiniCompiler::EndBuild(void)
{
	int i;

	//set str data and table
	m_makedCode[m_pc] = 4+m_mesKosuu+m_mesSize;
	m_makedCode[m_pc+1] = IDENTIFY_DATA;
	m_makedCode[m_pc+2] = CODE_DATA_TABLE;
	m_makedCode[m_pc+3] = TYPE_TABLE_MES;

	for (i=0;i<m_mesKosuu;i++)
	{
		m_makedCode[m_pc+4+i] = m_mesTable[i] + m_pc+4+m_mesKosuu;
	}
//	memcpy(&m_makedCode[m_pc+4],m_mesTable,m_mesKosuu*4);
	memcpy(&m_makedCode[m_pc+4+m_mesKosuu],m_mesPointer,m_mesSize*4);

	m_makedCode[4] = m_mesKosuu;
	m_makedCode[5] = m_pc + 4;
	m_pc += m_makedCode[m_pc];



	//set mes data and table

	m_makedCode[m_pc] = 4+m_strKosuu+m_strSize;
	m_makedCode[m_pc+1] = IDENTIFY_DATA;
	m_makedCode[m_pc+2] = CODE_DATA_TABLE;
	m_makedCode[m_pc+3] = TYPE_TABLE_STR;

	for (i=0;i<m_strKosuu;i++)
	{
		m_makedCode[m_pc+4+i] = m_strTable[i] + m_pc+4+m_strKosuu;
	}
//	memcpy(&m_makedCode[m_pc+4],m_strTable,m_strKosuu*4);
	memcpy(&m_makedCode[m_pc+4+m_strKosuu],m_strPointer,m_strSize*4);

	m_makedCode[6] = m_strKosuu;
	m_makedCode[7] = m_pc + 4;
	m_pc += m_makedCode[m_pc];

	m_makedCode[18] = m_pc+4;
	m_makedCode[m_pc] = 4 + m_largeConstKosuu;
	m_makedCode[m_pc+1] = IDENTIFY_DATA;
	m_makedCode[m_pc+2] = CODE_DATA_TABLE;
	m_makedCode[m_pc+3] = TYPE_TABLE_INT32;
	for (i=0;i<m_largeConstKosuu;i++)
	{
		m_makedCode[m_pc+4+i] = m_largeConst[i];
	}
	m_pc += m_makedCode[m_pc];

	m_makedCode[20] = m_pc+4;
	int subKosuu = m_subKosuu;
	m_makedCode[m_pc] = 4 + subKosuu;
	m_makedCode[m_pc+1] = IDENTIFY_DATA;
	m_makedCode[m_pc+2] = CODE_DATA_TABLE;
	m_makedCode[m_pc+3] = TYPE_TABLE_SUB;
	for (i=0;i<subKosuu;i++)
	{
		m_makedCode[m_pc+4+i] = m_subTable[i];
	}
	m_pc += m_makedCode[m_pc];

	m_makedCode[21] = m_pc+4;
	int selectKosuu = m_selectKosuu;
	m_makedCode[m_pc] = 4 + selectKosuu;
	m_makedCode[m_pc+1] = IDENTIFY_DATA;
	m_makedCode[m_pc+2] = CODE_DATA_TABLE;
	m_makedCode[m_pc+3] = TYPE_TABLE_SELECT;
	for (i=0;i<selectKosuu;i++)
	{
		m_makedCode[m_pc+4+i] = m_selectTable[i];
	}
	m_pc += m_makedCode[m_pc];

	m_makedCode[22] = m_pc+4;
	int commandKosuu = m_commandCallKosuu;
	m_makedCode[m_pc] = 4 + commandKosuu;
	m_makedCode[m_pc+1] = IDENTIFY_DATA;
	m_makedCode[m_pc+2] = CODE_DATA_TABLE;
	m_makedCode[m_pc+3] = TYPE_TABLE_COMMAND;
	for (i=0;i<commandKosuu;i++)
	{
		m_makedCode[m_pc+4+i] = m_commandCallTable[i];
	}
	m_pc += m_makedCode[m_pc];

	m_makedCode[23] = m_pc+4;
	int scriptCallKosuu = m_scriptCallKosuu;
	m_makedCode[m_pc] = 4 + scriptCallKosuu;
	m_makedCode[m_pc+1] = IDENTIFY_DATA;
	m_makedCode[m_pc+2] = CODE_DATA_TABLE;
	m_makedCode[m_pc+3] = TYPE_TABLE_SCRIPT;
	for (i=0;i<scriptCallKosuu;i++)
	{
		m_makedCode[m_pc+4+i] = m_scriptCallTable[i];
	}
	m_pc += m_makedCode[m_pc];

}





void CMiniCompiler::SetStoryKosuu(int n)
{
	SetCodeDirect(10,n);
}


void CMiniCompiler::SetCodeDirect(int n,int d)
{
	m_makedCode[n] = d;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@このへんもおーばーふろーちぇっくすべき20080604

BOOL CMiniCompiler::AddStr(LPSTR str)
{
	int nulSize = 1;

	if (m_targetMachine == 1)
	{
		str = iPhoneString(str);
		nulSize = 2;
	}



	int ln = strlen(str);
	int sz = (ln+nulSize+3) / 4;

	if (m_strKosuu >= m_strTableSize)
	{
		int* tmp = new int[m_strTableSize + 4096];
		memcpy(tmp,m_strTable,m_strTableSize*sizeof(int));
		m_strTableSize += 4096;
		delete [] m_strTable;
		m_strTable = tmp;
		MessageBox(NULL,"バッファー不足の為メモリの再確保を行いました:AddStr Table","報告",MB_OK | MB_ICONEXCLAMATION);
	}

	m_strTable[m_strKosuu] = m_strSize;

	if ((m_strSize+sz) > m_strPointerSize)
	{
		int* tmp = new int[m_strPointerSize + 65536];
		memcpy(tmp,m_strPointer,m_strPointerSize*sizeof(int));
		m_strPointerSize += 65536;
		delete [] m_strPointer;
		m_strPointer = tmp;
		MessageBox(NULL,"バッファー不足の為メモリの再確保を行いました:AddStr Buff","報告",MB_OK | MB_ICONEXCLAMATION);
	}

	memcpy(m_strPointer+m_strSize,str,ln+nulSize);
	m_strSize += sz;
	m_strKosuu++;

	return TRUE;
}




BOOL CMiniCompiler::AddMes(LPSTR mes)
{
	int nulSize = 1;

	if (m_targetMachine == 1)
	{
		mes = iPhoneString(mes);
		nulSize = 2;
	}

	int ln = strlen(mes);
	int sz = (ln+nulSize+3) / 4;

	if (m_mesKosuu >= m_mesTableSize)
	{
		int* tmp = new int[m_mesTableSize + 4096];
		memcpy(tmp,m_mesTable,m_mesTableSize*sizeof(int));
		m_mesTableSize += 4096;
		delete [] m_mesTable;
		m_mesTable = tmp;
		MessageBox(NULL,"バッファー不足の為メモリの再確保を行いました:AddMes Table","報告",MB_OK | MB_ICONEXCLAMATION);
	}

	m_mesTable[m_mesKosuu] = m_mesSize;

	if ((m_mesSize+sz) > m_mesPointerSize)
	{
		int* tmp = new int[m_mesPointerSize + 65536];
		memcpy(tmp,m_mesPointer,m_mesPointerSize*sizeof(int));
		m_mesPointerSize += 65536;
		delete [] m_mesPointer;
		m_mesPointer = tmp;
		MessageBox(NULL,"バッファー不足の為メモリの再確保を行いました:AddMes Buff","報告",MB_OK | MB_ICONEXCLAMATION);
	}


	memcpy(m_mesPointer+m_mesSize,mes,ln+nulSize);
	m_mesSize += sz;
	m_mesKosuu++;

	return TRUE;
}

//BOOL CMiniCompiler::AddSub(void)
//{
//	m_subTable[m_subKosuu] = m_;
//	return TRUE;
//}



BOOL CMiniCompiler::AddNowCode(void)
{
	if (m_objectSize == 0) return FALSE;

	//check max

	for (int i=0;i<m_objectSize;i++)
	{
		m_makedCode[m_pc] = m_objectCode[i];
		m_pc++;
	}

	m_objectSize = 0;
	return TRUE;
}


BOOL CMiniCompiler::AddCode(int* pCode, int n)
{
	//check max

	for (int i=0;i<n;i++)
	{
		m_makedCode[m_pc] = *(pCode+i);
		m_pc++;
	}


	return TRUE;
}

int CMiniCompiler::GetMakedSize(void)
{
	return m_pc;
}

int* CMiniCompiler::GetMakedPointer(void)
{
	return m_makedCode;
}


void CMiniCompiler::PrintMyError(LPSTR text,LPSTR caption)
{
	if (caption == NULL)
	{
		caption = m_defaultCaption;
	}

	if (m_errorSkipFlag) return;

	char mes[256];
	wsprintf(mes,"%s\nエラー表示を続けますか?",text);

	int rt = MessageBox(NULL,text,caption,MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2);
	if (rt == IDYES)
	{
		m_errorSkipFlag = TRUE;
	}
}

BOOL CMiniCompiler::GetSystemConst(LPSTR name,int* lpVal)
{
	int n = 0;
	while (n<100)
	{
		LPSTR checkName = m_systemConst[n*2];
		if ((*checkName) == 0) break;
		if (strcmp(name,checkName) == 0)
		{
			*lpVal = atoi(m_systemConst[n*2+1]);
			return TRUE;
		}

		n++;
	}


	return FALSE;
}


LPSTR CMiniCompiler::iPhoneString(LPSTR str)
{
	int sizeWide = MultiByteToWideChar(CP_UTF8,0,str,strlen(str),NULL,0);
	if (sizeWide == 0)
	{
		m_iPhoneString[0] = 0;
		m_iPhoneString[0] = 1;
	}
	else
	{
		MultiByteToWideChar(CP_UTF8,0,str,-1,(LPWSTR)m_iPhoneString,sizeWide);
	}

	return m_iPhoneString;
}

void CMiniCompiler::OutputData(FILE* file,LPSTR mes,int tab,BOOL crFlag)
{
	if (tab > 0)
	{
		for (int i=0;i<tab;i++)
		{
			fwrite("\t",sizeof(char),1,file);
		}
	}

	int ln = strlen(mes);
	fwrite(mes,sizeof(char),ln,file);

	if (crFlag)
	{
		fwrite("\x00d\x00a",sizeof(char),2,file);
	}
}




LPSTR CMiniCompiler::GetFunctionName(int type)
{
	for (int i=0;i<m_functionToNameNumber;i++)
	{
		if (m_functionToNameTable[i].type == type)
		{
			return m_functionToNameTable[i].name;
		}
	}

	return "Dummy";
}


