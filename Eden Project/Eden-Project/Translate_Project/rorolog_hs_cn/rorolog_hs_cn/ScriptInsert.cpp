#include "framework.h"

void ScriptInsert(wchar_t* Old, char* PushStr)
{
	copy_str(Old, L"夏休み。", PushStr, (char*)"暑假。");
	copy_str(Old, L"セーブメニュー", PushStr, (char*)"存档界面 ");
	copy_str(Old, L"ロードメニュー", PushStr, (char*)"读档界面 ");
	copy_str(Old, L"履歴", PushStr, (char*)"履历 ");
	copy_str(Old, L"システムメニュー", PushStr, (char*)"系统菜单 ");
	copy_str(Old, L"メニューの固定", PushStr, (char*)"固定菜单 ");
	copy_str(Old, L"ウィンドウの消去", PushStr, (char*)"隐藏对话框 ");
	copy_str(Old, L"クイックロード", PushStr, (char*)"快速读档 ");
	copy_str(Old, L"クイックセーブ", PushStr, (char*)"快速存档 ");
	copy_str(Old, L"次の選択肢に進む", PushStr, (char*)"快进到下一个选项 ");
	copy_str(Old, L"オート", PushStr, (char*)"自动模式 ");
}