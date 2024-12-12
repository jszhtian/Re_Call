
int tona_main();

// ****************************************************************
// WinMain
// ================================================================
int WINAPI _tWinMain(HINSTANCE h_inst, HINSTANCE h_pre_inst, LPTSTR lpsz_cmd_line, int n_cmd_show)
{
	// ���P�[����ݒ�i���̂��߂ɕK�v���Y�ꂽ�c�j
#if defined(__SIGLUS_ZH) || defined(__SIGLUS_ZHTW)
	_tsetlocale(LC_ALL, _T("chinese"));
#else
	_tsetlocale(LC_ALL, _T("japanese"));
#endif

	// app �ɃC���X�^���X�Ȃǂ�o�^
	NT3::G_app.h_inst = h_inst;
	NT3::G_app.command_line = lpsz_cmd_line;

	// ���W���[���p�X�̎擾
	TCHAR module_path_buf[MAX_PATH];
	::GetModuleFileName(NULL, module_path_buf, MAX_PATH);
	NT3::TSTR module_path = module_path_buf;

	// ���W���[���f�B���N�g���̎擾
	NT3::G_app.module_dir = NT3::get_dir_path(module_path);
	NT3::G_app.module_name = NT3::erase_extension_ret(NT3::get_file_name(module_path));

	// �J�����g�f�B���N�g�������W���[���f�B���N�g���̐ݒ�
	::SetCurrentDirectory(NT3::G_app.module_dir.c_str());

	// �A�v���P�[�V�����f�B���N�g���̎擾�i�}�C�h�L�������g�ɐݒ�j
	TCHAR app_dir_buf[MAX_PATH];
	SHGetSpecialFolderPath(NULL, app_dir_buf, CSIDL_PERSONAL, FALSE);
	NT3::G_app.app_dir = app_dir_buf;

	// COM �̏�����
	CoInitialize(NULL);

	// �R�����R���g���[���̏�����
	INITCOMMONCONTROLSEX iccex;
	iccex.dwSize = sizeof(iccex);
	iccex.dwICC = ICC_INTERNET_CLASSES | ICC_LISTVIEW_CLASSES | ICC_TAB_CLASSES;
	InitCommonControlsEx(&iccex);

	// ���b�`�G�f�B�b�g
	HMODULE h_module_re32 = LoadLibrary(_T("riched32.dll"));
	if (!h_module_re32)	{
		NT3::error_box(_T("riched32.dll �̓ǂݍ��݂Ɏ��s���܂����B�A�v���P�[�V�������I�����܂��B"));
		return 0;
	}

#if ___USE_NG___

	// ng �p���b�g�̏�����
	init_palette_table();
	make_palette_table();

#endif

	// ���C��
	tona_main();
	
#if ___USE_NG___

	// ng �p���b�g�̉��
	free_palette_table();

#endif

	// ���b�`�G�f�B�b�g�̉��
	FreeLibrary(h_module_re32);

	// COM �̉��
	CoUninitialize();

	return 0;
}
