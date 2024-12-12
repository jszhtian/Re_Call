/**********************************
*	Ever tried. Ever failed.      *
*	No matter. Try again.         *
*	Fail again. Fail better.      *
 **********************************/

#include "framework.h"

typedef HFONT(WINAPI* fnCreateFontA)(
	int nHeight, // logical height of font height
	int nWidth, // logical average character width
	int nEscapement, // angle of escapement
	int nOrientation, // base-line orientation angle
	int fnWeight, // font weight
	DWORD fdwItalic, // italic attribute flag
	DWORD fdwUnderline, // underline attribute flag
	DWORD fdwStrikeOut, // strikeout attribute flag
	DWORD fdwCharSet, // character set identifier
	DWORD fdwOutputPrecision, // output precision
	DWORD fdwClipPrecision, // clipping precision
	DWORD fdwQuality, // output quality
	DWORD fdwPitchAndFamily, // pitch and family
	LPCSTR lpszFace // pointer to typeface name string
	);
fnCreateFontA CreateFontAOLD;
HFONT WINAPI CreateFontAEx(int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight, DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily, LPCSTR lpszFace)
{
	fdwCharSet = 0x86;
	return CreateFontAOLD(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, "黑体");
}

PVOID g_pOldCreateWindowExA = CreateWindowExA;
typedef HWND(WINAPI* pfuncCreateWindowExA)(
	DWORD dwExStyle,
	LPCTSTR lpClassName,
	LPCTSTR lpWindowName,
	DWORD dwStyle,
	int x,
	int y,
	int nWidth,
	int nHeight,
	HWND hWndParent,
	HMENU hMenu,
	HINSTANCE hInstance,
	LPVOID lpParam);
HWND WINAPI NewCreateWindowExA(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	const char* szWndName = "【-COKEZIGE汉化组-】Da Capo Ⅲ Platinum Partner - Beta v0.5";

	return ((pfuncCreateWindowExA)g_pOldCreateWindowExA)(dwExStyle, lpClassName, (LPCTSTR)szWndName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

LPWSTR ctowJIS(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(932, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(932, 0, str, -1, out, dwMinSize);//转换
	return out;
}

char* wtocGBK(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char* out = new char[dwMinSize];
	WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}



//log
wchar_t szTitle[] = L"提示";
typedef int (WINAPI* fnMessageboxA)(
	_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType
	);
fnMessageboxA MessageBoxAOLD;
int WINAPI MessageBoxAEx(_In_opt_ HWND    hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_     UINT    uType)
{
	char* Pstr = wtocGBK(ctowJIS((char*)lpText));
	if (strcmp(Pstr, "終了しますか？") == 0)
	{
		strcpy(Pstr, "结束游戏吗？");
	}
	return MessageBoxAOLD(hWnd, Pstr, wtocGBK(szTitle), uType);
}

PVOID g_pOldCreateFileA = CreateFileA;
typedef HANDLE(WINAPI* PfuncCreateFileA)(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
	);

HANDLE WINAPI NewCreateFileA(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
{
	string strOldName(lpFileName);
	string strName = strOldName.substr(strOldName.find_last_of("\\") + 1);
	string fstReadName = "Kaze\\" + strName;
	if (GetFileAttributesA(fstReadName.c_str()) != INVALID_FILE_ATTRIBUTES) 
	{
		cout << "ReadFileFromKaze:" << fstReadName << endl;
		return ((PfuncCreateFileA)(g_pOldCreateFileA))(
			fstReadName.c_str(),
			dwDesiredAccess,
			dwShareMode,
			lpSecurityAttributes,
			dwCreationDisposition,
			dwFlagsAndAttributes,
			hTemplateFile);
	}
	return ((PfuncCreateFileA)(g_pOldCreateFileA))(
		lpFileName,
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);
}

void Init()
{

	CreateFontAOLD = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
	MessageBoxAOLD = (fnMessageboxA)GetProcAddress(GetModuleHandle(L"User32.dll"), "MessageBoxA");

	DetourTransactionBegin();
	DetourAttach((void**)& CreateFontAOLD, CreateFontAEx);
	DetourAttach(&g_pOldCreateWindowExA, NewCreateWindowExA);
	//DetourAttach(&g_pOldCreateFileA, NewCreateFileA);
	DetourAttach((void**)& MessageBoxAOLD, MessageBoxAEx);

	if (DetourTransactionCommit() != NOERROR) 
		MessageBox(NULL, L"INIT FUNCTION ERROR", L"System", MB_OK | MB_ICONERROR);
	else 
		MessageBox(NULL, L"本补丁由【-COKEZIGE汉化组-】制作\n禁止一切录播直播和商业行为", L"警告", MB_OK | MB_ICONINFORMATION);
}
/**********************************************************************************************************************/

void GetSelTitle()
{
	//char** Start = (char**)(BaseAddr + 0x98878);
	char** Start = (char**)(BaseAddr + 0x88460);
	for (int i = 0; i < 0x998; i++)
	{
		//cout << hex << *(char**)(Start + i) << "|" << *(char**)(Start + i + 1) << endl;
		cout << "\t{ \"" << *(char**)(Start + i) << "\", \"" << *(char**)(Start + i + 1) << "\" }," << endl;
		i++;

		//cout << hex << *(char**)(Start + i) << endl;
	}
}

 
const char* SaveTitleJpn[][2]{
        { "DC3PP_0518_FREE_AOI", "葵と電話" },
        { "DC3PP_0518_FREE_HMN", "姫乃と電話" },
        { "DC3PP_0518_FREE_RCC", "立夏と電話" },
        { "DC3PP_0518_FREE_SRA", "さらと電話" },
        { "DC3PP_0518_FREE_SRR", "シャルルと会話" },
        { "DC3PP_1_0516_E1_PRO", "編集会議！" },
        { "DC3PP_1_0516_F1_PRO", "るる姉のアブないマッサージ" },
        { "DC3PP_1_0517_A1_PRO", "非公式新聞部からの挑戦状" },
        { "DC3PP_1_0517_C1_PRO", "まさかの助っ人" },
        { "DC3PP_1_0517_E1_PRO", "リレーメンバー決定！" },
        { "DC3PP_1_0518_A1_PRO", "立夏さんとの個人レッスン？" },
        { "DC3PP_1_0518_B1_PRO", "江戸川姉弟の力関係" },
        { "DC3PP_1_0518_D1_PRO", "とらぶる三人四脚" },
        { "DC3PP_1_0518_E1_PRO", "愛情たっぷりスープ" },
        { "DC3PP_1_0518_F1_FRE", "誰に電話しよう" },
        { "DC3PP_1_0519_A1_PRO", "体育祭、開幕！" },
        { "DC3PP_1_0519_B1_PRO", "白熱、三人四脚！" },
        { "DC3PP_1_0519_C1_PRO", "幻のメニュー" },
        { "DC3PP_1_0519_E1_PRO", "誰を応援する？" },
        { "DC3PP_1_0519_E2_PRO", "とっておきの秘策" },
        { "DC3PP_1_0519_E3_PRO", "決戦の時" },
        { "DC3PP_1_0519_F1_PRO", "祝勝会" },
        { "DC3PP_2_0628_E1_PRO", "夏休み活動計画" },
        { "DC3PP_2_0628_F1_PRO", "波乱の勉強会" },
        { "DC3PP_2_0629_B1_PRO", "期末テストにむけて" },
        { "DC3PP_2_0630_C1_PRO", "四季さんの予定は？" },
        { "DC3PP_2_0701_A1_PRO", "最後のひと踏ん張り" },
        { "DC3PP_2_0701_E1_PRO", "旅行の打ち合わせ" },
        { "DC3PP_2_0722_B1_PRO", "夏休みを前に" },
        { "DC3PP_2_0722_E1_PRO", "魅惑の水着ショー" },
        { "DC3PP_2_0723_A1_PRO", "旅行のはじまり" },
        { "DC3PP_2_0723_B1_PRO", "となりの立夏さん" },
        { "DC3PP_2_0723_C1_PRO", "プールサイドの女神たち" },
        { "DC3PP_2_0723_E1_PRO", "懐かしの味" },
        { "DC3PP_2_0723_F1_PRO", "お風呂のお約束" },
        { "DC3PP_2_0723_F20_PRO", "ラブジョンガ" },
        { "DC3PP_2_0723_F21_PRO", "ラブジョンガ：白" },
        { "DC3PP_2_0723_F22_PRO", "ラブジョンガ：中ピンク" },
        { "DC3PP_2_0723_F23_PRO", "ラブジョンガ：下ピンク" },
        { "DC3PP_2_0723_F30_PRO", "ＬＯＶＥキッス" },
        { "DC3PP_2_0724_B1_PRO", "宝探し" },
        { "DC3PP_2_0724_D2_PRO", "宝箱の中身は？" },
        { "DC3PP_2_0724_E1_PRO", "夏合宿の終わり" },
        { "DC3PP_2_0724_F2_AOI", "夜の逢瀬・葵" },
        { "DC3PP_2_0724_F2_HMN", "夜の逢瀬・姫乃" },
        { "DC3PP_2_0724_F2_PRO", "夜の逢瀬" },
        { "DC3PP_2_0724_F2_RCC", "夜の逢瀬・立夏" },
        { "DC3PP_2_0724_F2_SRA", "夜の逢瀬・さら" },
        { "DC3PP_2_0724_F2_SRR", "夜の逢瀬・シャルル" },
        { "DC3PP_2_0725_B1_PRO", "夏の思い出" },
        { "DC3PP_3_0922_E1_PRO", "わたしも行きたいの！" },
        { "DC3PP_3_0922_F1_CMN", "エスパーるる姉" },
        { "DC3PP_3_0923_A1_CMN", "出発の朝" },
        { "DC3PP_3_0923_B1_CMN", "空港にて" },
        { "DC3PP_3_0923_D10_CMN", "旅のお供" },
        { "DC3PP_3_0923_D11_HMN", "兄さんの予定は？" },
        { "DC3PP_3_0923_D12_SRA", "サイトシーイング" },
        { "DC3PP_3_0924_B1_CMN", "時を経て" },
        { "DC3PP_3_0924_D1_CMN", "チャンスを活かせ！" },
        { "DC3PP_3_0924_F1_CMN", "１つ目の教訓" },
        { "DC3PP_3_0924_F2_CMN", "四季さんって……" },
        { "DC3PP_3_0925_A1_CMN", "自由行動の予定" },
        { "DC3PP_3_0925_B1_CMN", "ハロッズ" },
        { "DC3PP_3_0925_F1_CMN", "季節外れの百物語" },
        { "DC3PP_3_0926_A1_CMN", "旧クリサリス邸" },
        { "DC3PP_3_0926_B10_CMN", "半分こ" },
        { "DC3PP_3_0926_B11_HMN", "半分こ：姫乃" },
        { "DC3PP_3_0926_B12_SRA", "半分こ：さら" },
        { "DC3PP_3_0926_F1_CMN", "決意の夜" },
        { "DC3PP_3_0927_B10_CMN", "心配なあの子" },
        { "DC3PP_3_0927_B10_HMN", "心配な姫乃" },
        { "DC3PP_3_0927_B10_SRA", "心配なさら" },
        { "DC3PP_3_0927_B11_HMN", "姫乃は、姫乃だから" },
        { "DC3PP_3_0927_B12_SRA", "さらは、さらだから" },
        { "DC3PP_3_0930_A11_HMN", "おふくろの味" },
        { "DC3PP_3_0930_A12_SRA", "先輩がいるだけで" },
        { "DC3PP_3_1001_A11_HMN", "これから先も、ずっと" },
        { "DC3PP_3_1001_A12_SRA", "茜色の告白" },
        { "DC3PP_3_1101_E1_PRO", "文化祭の出し物といえば？" },
        { "DC3PP_3_1101_E2_CMN", "自分で蒔いた忙しい種" },
        { "DC3PP_3_1101_F1_RCC", "運命的なふたりの下校" },
        { "DC3PP_3_1101_F2_SRR", "るる姉の危険な手料理" },
        { "DC3PP_3_1102_A1_AOI", "運命的なふたりの登校" },
        { "DC3PP_3_1102_B1_CMN", "文化祭を盛り上げるために" },
        { "DC3PP_3_1102_C1_CMN", "立夏さんミスコン出場決意" },
        { "DC3PP_3_1102_D1_CMN", "文化祭準備の道のりは長い" },
        { "DC3PP_3_1102_D2_CMN", "生徒会のお手伝い？" },
        { "DC3PP_3_1102_F10_CMN", "深夜のメール着信" },
        { "DC3PP_3_1102_F11_RCC", "立夏さんとサンドイッチ" },
        { "DC3PP_3_1102_F12_SRR", "るる姉と奇跡？のおにぎり" },
        { "DC3PP_3_1102_F13_AOI", "葵ちゃんと青春の味" },
        { "DC3PP_3_1103_A1_CMN", "男のロマン？" },
        { "DC3PP_3_1103_B1_CMN", "文化祭開始！" },
        { "DC3PP_3_1103_B2_RCC", "寝顔とドキドキの責任" },
        { "DC3PP_3_1103_C1_SRR", "プライベート・サービス" },
        { "DC3PP_3_1103_D1_CMN", "動き出した非公式新聞部" },
        { "DC3PP_3_1103_D2_CMN", "サンタさんへのリクエスト" },
        { "DC3PP_3_1103_D3_AOI", "感情豊かな葵ちゃん" },
        { "DC3PP_3_1103_E1_CMN", "ミスコン本番！" },
        { "DC3PP_3_1103_F10_CMN", "一番大切な人" },
        { "DC3PP_3_1103_F11_RCC", "鼓動で伝わるキモチ" },
        { "DC3PP_3_1103_F12_SRR", "従姉弟としてじゃなく……" },
        { "DC3PP_3_1103_F13_AOI", "少しだけ、ロマンチックに" },
        { "DC3PP_4_1213_E1_RCC", "もうすぐクリスマス" },
        { "DC3PP_4_1213_E1_SRR", "ラブイチャな新婚さん" },
        { "DC3PP_4_1213_E2_RCC", "ぎこちないふたり" },
        { "DC3PP_4_1213_F1_RCC", "立夏さんへの想い" },
        { "DC3PP_4_1213_F1_SRR", "恋人としての初めて" },
        { "DC3PP_4_1213_F2_SRR", "タカくんの彼女さん" },
        { "DC3PP_4_1214_A1_RCC", "昨日の会議は" },
        { "DC3PP_4_1214_A1_SRR", "いってらっしゃいのお約束" },
        { "DC3PP_4_1214_D1_RCC", "意外に鋭い耕助" },
        { "DC3PP_4_1214_E1_RCC", "振り回される日常" },
        { "DC3PP_4_1214_E1_SRR", "クリパとるる姉の上の空" },
        { "DC3PP_4_1214_F1_SRR", "受け継がれる想い" },
        { "DC3PP_4_1215_A1_SRR", "クリスマスに何が欲しい？" },
        { "DC3PP_4_1215_D1_SRR", "雪村すももとあみぐるみ" },
        { "DC3PP_4_1215_E1_SRA", "突然の手紙" },
        { "DC3PP_4_1215_F1_SRA", "初めてのお泊り" },
        { "DC3PP_4_1215_F1_SRR", "クリスマスイベントの提案" },
        { "DC3PP_4_1215_F2_SRA", "先輩色に……" },
        { "DC3PP_4_1216_A1_SRA", "意外な保護者" },
        { "DC3PP_4_1216_A1_SRR", "雪村との内緒話" },
        { "DC3PP_4_1216_C1_SRR", "お姫さまと召使い" },
        { "DC3PP_4_1216_D1_SRA", "謎解き迷路の舞台裏" },
        { "DC3PP_4_1216_E1_SRA", "仲間に祝福される幸せ" },
        { "DC3PP_4_1216_F1_SRR", "一緒にお風呂！？" },
        { "DC3PP_4_1217_B1_SRR", "初デート" },
        { "DC3PP_4_1217_D1_SRR", "るる姉が教えてくれた景色" },
        { "DC3PP_4_1217_E1_RCC", "特集号の企画会議" },
        { "DC3PP_4_1217_E2_RCC", "修行の成果" },
        { "DC3PP_4_1217_F1_RCC", "魔法のこと" },
        { "DC3PP_4_1218_A1_RCC", "カウントダウン" },
        { "DC3PP_4_1218_B1_HMN", "引っ張りだこ" },
        { "DC3PP_4_1218_D1_SRR", "クリパの裏イベント！？" },
        { "DC3PP_4_1218_E1_HMN", "編集会議" },
        { "DC3PP_4_1218_E1_SRR", "トラブルに次ぐトラブル" },
        { "DC3PP_4_1218_E2_HMN", "教室でふたり" },
        { "DC3PP_4_1218_F1_SRR", "やりたいこと、もう一つ" },
        { "DC3PP_4_1218_F2_SRR", "前世のシャルルの物語" },
        { "DC3PP_4_1219_A1_HMN", "寒し、恥ずかし、通学路" },
        { "DC3PP_4_1219_B1_HMN", "ご愁傷様です……" },
        { "DC3PP_4_1219_D1_SRR", "枯れない桜からの贈りもの" },
        { "DC3PP_4_1219_E1_SRA", "出口の無い迷路" },
        { "DC3PP_4_1219_F1_HMN", "姫乃さんに予約して！" },
        { "DC3PP_4_1219_F1_SRA", "父と向き合う決意" },
        { "DC3PP_4_1219_F1_SRR", "人形劇台本の変更版" },
        { "DC3PP_4_1220_A1_SRA", "準備という名の戦場" },
        { "DC3PP_4_1220_A1_SRR", "人形劇の台本、完成！" },
        { "DC3PP_4_1220_B1_HMN", "合コンのお誘い" },
        { "DC3PP_4_1220_D1_SRR", "心強い助っ人の参戦！" },
        { "DC3PP_4_1220_E1_HMN", "小人の靴屋" },
        { "DC3PP_4_1220_E1_SRR", "動き出す、公式新聞部" },
        { "DC3PP_4_1220_F1_HMN", "アイラブユー？！" },
        { "DC3PP_4_1221_A1_SRR", "色々と貴重なお昼休み" },
        { "DC3PP_4_1221_B1_HMN", "進んでいくクリパ準備" },
        { "DC3PP_4_1221_E1_HMN", "緊急招集っ！" },
        { "DC3PP_4_1221_E1_SRR", "小道具作りを始めよう！" },
        { "DC3PP_4_1221_F1_SRR", "楽しくも騒がしい帰り道" },
        { "DC3PP_4_1222_A1_HMN", "滑って滑って、滑らない" },
        { "DC3PP_4_1222_A1_SRR", "進捗はどうですか？" },
        { "DC3PP_4_1222_B1_AOI", "参上、葵ちゃん" },
        { "DC3PP_4_1222_B1_HMN", "ラストスパート" },
        { "DC3PP_4_1222_B1_SRR", "脱出ゲーム、完成！" },
        { "DC3PP_4_1222_C1_AOI", "さむイチャドキドキお弁当" },
        { "DC3PP_4_1222_E1_AOI", "定例会議……ではなくて？" },
        { "DC3PP_4_1222_F1_HMN", "もう少しだけ素直に" },
        { "DC3PP_4_1222_F1_SRR", "さらなる意外な助っ人" },
        { "DC3PP_4_1223_A1_AOI", "ロリでボインはお好き？" },
        { "DC3PP_4_1223_A1_SRA", "クリパ開始！" },
        { "DC3PP_4_1223_B1_AOI", "いらっしゃいませ葵ちゃん" },
        { "DC3PP_4_1223_B1_HMN", "クリパ開幕！" },
        { "DC3PP_4_1223_B1_SRR", "クリパデート" },
        { "DC3PP_4_1223_B2_HMN", "いらっしゃいませ、皆々様" },
        { "DC3PP_4_1223_D1_HMN", "明日は一緒に" },
        { "DC3PP_4_1223_D1_SRA", "恋人と過ごす特別な時間" },
        { "DC3PP_4_1223_D1_SRR", "それぞれのクリパ" },
        { "DC3PP_4_1223_E1_RCC", "前夜祭" },
        { "DC3PP_4_1223_E1_SRR", "明日のための飾りつけ" },
        { "DC3PP_4_1223_F1_AOI", "勤労少女との帰り道" },
        { "DC3PP_4_1223_F1_SRR", "突撃！　立夏さんのお部屋" },
        { "DC3PP_4_1224_A1_SRA", "初めてのプレゼント" },
        { "DC3PP_4_1224_B1_AOI", "ベストカップル？！" },
        { "DC3PP_4_1224_B1_HMN", "姫乃さんといっしょ" },
        { "DC3PP_4_1224_B1_RCC", "クリスマスパーティー" },
        { "DC3PP_4_1224_B2_AOI", "葵ちゃんとクリパデート" },
        { "DC3PP_4_1224_D1_SRA", "さらがミスコン出場！？" },
        { "DC3PP_4_1224_D1_SRR", "るる姉＋サンタ衣装＝？" },
        { "DC3PP_4_1224_D2_SRR", "急遽出場、ミスコン司会！" },
        { "DC3PP_4_1224_E1_SRR", "子供に笑顔を" },
        { "DC3PP_4_1224_E2_SRR", "シャルルの願った物語" },
        { "DC3PP_4_1224_F1_AOI", "恋人の証明" },
        { "DC3PP_4_1224_F1_HMN", "粉雪の中、君を想う" },
        { "DC3PP_4_1224_F1_RCC", "キスまで数ｃｍ" },
        { "DC3PP_4_1224_F1_SRA", "新婚さんのような生活" },
        { "DC3PP_4_1224_F1_SRR", "お疲れ様でした！" },
        { "DC3PP_4_1224_F2_AOI", "雪の降る聖夜に" },
        { "DC3PP_4_1224_F2_HMN", "私は知りたい" },
        { "DC3PP_4_1224_F2_SRR", "プレゼントはあ・た・し" },
        { "DC3PP_4_1224_F3_SRR", "ホワイトクリスマス" },
        { "DC3PP_4_1225_A1_AOI", "ちょっとガス欠" },
        { "DC3PP_4_1225_A1_HMN", "フーアムアイ？！" },
        { "DC3PP_4_1225_A1_SRA", "部屋でイチャイチャ" },
        { "DC3PP_4_1225_B1_HMN", "俺が姫乃で、姫乃が俺で" },
        { "DC3PP_4_1225_F1_AOI", "お姫様の看病" },
        { "DC3PP_4_1225_F1_HMN", "女体の神秘" },
        { "DC3PP_4_1225_F1_SRA", "説得の方法" },
        { "DC3PP_4_1226_A1_AOI", "はらぺこるる姉" },
        { "DC3PP_4_1226_A1_HMN", "早朝の葛木家" },
        { "DC3PP_4_1226_B1_RCC", "冬休みも部活" },
        { "DC3PP_4_1226_C1_HMN", "お互いのこと" },
        { "DC3PP_4_1226_D1_HMN", "美琴の勘" },
        { "DC3PP_4_1226_D1_RCC", "調査ｏｒデート" },
        { "DC3PP_4_1226_F1_AOI", "無理は禁物" },
        { "DC3PP_4_1226_F1_RCC", "何かが起きている" },
        { "DC3PP_4_1227_A1_AOI", "添い寝をしましょう" },
        { "DC3PP_4_1227_B1_HMN", "兄さんの見ていた景色" },
        { "DC3PP_4_1227_C1_HMN", "チョコバナナキッス！" },
        { "DC3PP_4_1227_E1_AOI", "刺激的な着替え" },
        { "DC3PP_4_1227_F1_AOI", "あーんして" },
        { "DC3PP_4_1227_F1_HMN", "大切な想い出" },
        { "DC3PP_4_1228_A1_AOI", "お医者さんへ行こう" },
        { "DC3PP_4_1228_B1_AOI", "スーパーでお買い物" },
        { "DC3PP_4_1228_B1_HMN", "葵ちゃんの追及" },
        { "DC3PP_4_1228_D1_HMN", "さらの洞察" },
        { "DC3PP_4_1228_E1_HMN", "立夏さんにはお見通し" },
        { "DC3PP_4_1228_F1_AOI", "本日のメインディッシュ" },
        { "DC3PP_4_1229_A1_AOI", "悪化する体調" },
        { "DC3PP_4_1229_B1_AOI", "頼れる人は……" },
        { "DC3PP_4_1229_B1_HMN", "さくらのアドバイス" },
        { "DC3PP_4_1229_B1_RCC", "類例をたどって" },
        { "DC3PP_4_1229_E1_AOI", "残酷な仮説" },
        { "DC3PP_4_1229_E1_HMN", "深く、深く……" },
        { "DC3PP_4_1229_E1_RCC", "枯れない桜の調査" },
        { "DC3PP_4_1229_F1_AOI", "救うために" },
        { "DC3PP_4_1229_F1_HMN", "同じ布団の中で" },
        { "DC3PP_4_1229_F1_SRA", "少し早めの年越しそば" },
        { "DC3PP_4_1230_A1_AOI", "笑顔の下に" },
        { "DC3PP_4_1230_A1_HMN", "今度こそ本当に" },
        { "DC3PP_4_1230_B1_SRA", "さらの実家" },
        { "DC3PP_4_1230_E1_AOI", "みんなのお見舞い" },
        { "DC3PP_4_1230_F1_HMN", "お背中流しますっ！" },
        { "DC3PP_4_1230_F1_SRA", "瑠川家の家族団欒" },
        { "DC3PP_4_1230_F2_SRA", "理想の夫婦像" },
        { "DC3PP_4_1231_A1_AOI", "葵ちゃんと朝風呂" },
        { "DC3PP_4_1231_A1_SRA", "残酷な真実" },
        { "DC3PP_4_1231_B1_AOI", "大掃除をしましょう" },
        { "DC3PP_4_1231_B1_HMN", "ふたりきりの大晦日" },
        { "DC3PP_4_1231_D1_RCC", "年越しは我が家で♪" },
        { "DC3PP_4_1231_E1_RCC", "謎の少女" },
        { "DC3PP_4_1231_E1_SRA", "真実の告白" },
        { "DC3PP_4_1231_F1_AOI", "ふたりっきりの年越し" },
        { "DC3PP_4_1231_F1_HMN", "特別な年越し" },
        { "DC3PP_4_1231_F1_SRA", "年越し打ち上げ花火" },
        { "DC3PP_4_1231_F2_HMN", "初日の出に誓って" },
        { "DC3PP_4_1301_A1_SRA", "瑠川家で過ごす元旦" },
        { "DC3PP_4_1301_B1_RCC", "一年の計はどこにあり？" },
        { "DC3PP_4_1301_E1_RCC", "正義の魔法使い１" },
        { "DC3PP_4_1301_E1_SRA", "父のために出来ること" },
        { "DC3PP_4_1301_F1_AOI", "葵ちゃんと初詣デート" },
        { "DC3PP_4_1301_F1_SRA", "キャッチボールの誘い方" },
        { "DC3PP_4_1301_F2_AOI", "ふたりの思い出を" },
        { "DC3PP_4_1302_A1_SRA", "託されたボール" },
        { "DC3PP_4_1302_B1_AOI", "年賀状を書きましょう" },
        { "DC3PP_4_1302_B1_RCC", "寝正月" },
        { "DC3PP_4_1302_C1_SRA", "明るい人生設計" },
        { "DC3PP_4_1302_D1_RCC", "そっけない立夏さん" },
        { "DC3PP_4_1302_E1_AOI", "届かない似顔絵" },
        { "DC3PP_4_1303_B1_RCC", "遅れ馳せの初詣" },
        { "DC3PP_4_1304_B1_AOI", "大切な笑顔" },
        { "DC3PP_4_1305_A1_SRA", "ただいま、初音島" },
        { "DC3PP_4_1305_B1_RCC", "何かとあれば立夏さん" },
        { "DC3PP_4_1308_B1_RCC", "思い出のキス" },
        { "DC3PP_4_1308_F1_RCC", "君を想う" },
        { "DC3PP_4_1309_B1_AOI", "明日の為に" },
        { "DC3PP_4_1309_D1_RCC", "暖かい場所" },
        { "DC3PP_4_1309_F1_RCC", "頬と頬、肌と肌" },
        { "DC3PP_4_1310_B1_AOI", "楽しい思い出とともに" },
        { "DC3PP_4_1310_F1_AOI", "さようなら……" },
        { "DC3PP_4_1310_F2_AOI", "とても大好きで大切な……" },
        { "DC3PP_4_1311_A1_RCC", "不安な風" },
        { "DC3PP_4_1311_A1_SRA", "父を安心させる方法" },
        { "DC3PP_4_1311_B1_AOI", "忘れられない想い" },
        { "DC3PP_4_1311_B1_RCC", "嵐の前の" },
        { "DC3PP_4_1311_B2_AOI", "祝福の鐘" },
        { "DC3PP_4_1311_E1_RCC", "忘却の海" },
        { "DC3PP_4_1311_E1_SRA", "送別試合に出るために" },
        { "DC3PP_4_1311_F1_RCC", "自覚" },
        { "DC3PP_4_1312_B10_RCC", "あの少女を捜せ" },
        { "DC3PP_4_1312_B11_RCC", "ゆずと捜索" },
        { "DC3PP_4_1312_B12_RCC", "夕陽と捜索" },
        { "DC3PP_4_1312_E1_RCC", "正義の魔法使い２" },
        { "DC3PP_4_1312_E1_SRA", "理論と努力で補う運動神経" },
        { "DC3PP_4_1312_E2_RCC", "間違い電話" },
        { "DC3PP_4_1313_F1_RCC", "立夏の気持ち１" },
        { "DC3PP_4_1313_F2_RCC", "立夏の気持ち２" },
        { "DC3PP_4_1316_A1_RCC", "一大決心" },
        { "DC3PP_4_1316_E1_RCC", "百年の想い" },
        { "DC3PP_4_1401_E1_SRA", "さらの得意分野" },
        { "DC3PP_4_1414_E1_SRA", "バレンタイン・プレゼント" },
        { "DC3PP_4_1420_E1_SRA", "努力の成果" },
        { "DC3PP_4_1500_A1_RCC", "いつまでもずっと" },
        { "DC3PP_4_1513_E1_SRA", "ホワイトデー・プレゼント" },
        { "DC3PP_4_1515_A1_SRA", "お祭り騒ぎの卒業式" },
        { "DC3PP_4_1515_D1_SRA", "送別試合、本番！" },
        { "DC3PP_4_1515_E1_SRA", "伝えたかった言葉" },
        { "DC3PP_4_9999_A1_SRA", "幸せな家族計画" },
};

void CopyBlockJpn()
{
    memcpy((void*)(BaseAddr + 0x88460), SaveTitleJpn, sizeof(SaveTitleJpn));
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		make_console();
		//GetSelTitle();
        CopyBlockJpn();
		Init();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) void dummy(void) {
	return;
}