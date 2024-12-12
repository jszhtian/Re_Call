
int tona_main();

// ****************************************************************
// WinMain
// ================================================================
int WINAPI _tWinMain(HINSTANCE h_inst, HINSTANCE h_pre_inst, LPTSTR lpsz_cmd_line, int n_cmd_show)
{
	// ロケールを設定（何のために必要か忘れた…）
#if defined(__SIGLUS_ZH) || defined(__SIGLUS_ZHTW)
	_tsetlocale(LC_ALL, _T("chinese"));
#else
	_tsetlocale(LC_ALL, _T("japanese"));
#endif

	// app にインスタンスなどを登録
	NT3::G_app.h_inst = h_inst;
	NT3::G_app.command_line = lpsz_cmd_line;

	// モジュールパスの取得
	TCHAR module_path_buf[MAX_PATH];
	::GetModuleFileName(NULL, module_path_buf, MAX_PATH);
	NT3::TSTR module_path = module_path_buf;

	// モジュールディレクトリの取得
	NT3::G_app.module_dir = NT3::get_dir_path(module_path);
	NT3::G_app.module_name = NT3::erase_extension_ret(NT3::get_file_name(module_path));

	// カレントディレクトリをモジュールディレクトリの設定
	::SetCurrentDirectory(NT3::G_app.module_dir.c_str());

	// アプリケーションディレクトリの取得（マイドキュメントに設定）
	TCHAR app_dir_buf[MAX_PATH];
	SHGetSpecialFolderPath(NULL, app_dir_buf, CSIDL_PERSONAL, FALSE);
	NT3::G_app.app_dir = app_dir_buf;

	// COM の初期化
	CoInitialize(NULL);

	// コモンコントロールの初期化
	INITCOMMONCONTROLSEX iccex;
	iccex.dwSize = sizeof(iccex);
	iccex.dwICC = ICC_INTERNET_CLASSES | ICC_LISTVIEW_CLASSES | ICC_TAB_CLASSES;
	InitCommonControlsEx(&iccex);

	// リッチエディット
	HMODULE h_module_re32 = LoadLibrary(_T("riched32.dll"));
	if (!h_module_re32)	{
		NT3::error_box(_T("riched32.dll の読み込みに失敗しました。アプリケーションを終了します。"));
		return 0;
	}

#if ___USE_NG___

	// ng パレットの初期化
	init_palette_table();
	make_palette_table();

#endif

	// メイン
	tona_main();
	
#if ___USE_NG___

	// ng パレットの解放
	free_palette_table();

#endif

	// リッチエディットの解放
	FreeLibrary(h_module_re32);

	// COM の解放
	CoUninitialize();

	return 0;
}
