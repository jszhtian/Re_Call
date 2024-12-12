#include	"pch.h"
#include	"namalib_sys_msg.h"

namespace Nnama
{

// ****************************************************************
// コンストラクタ
// ================================================================
Cnamalib_msg::Cnamalib_msg()
{
	clear();
}

// ****************************************************************
// メッセージクリア
// ================================================================
void Cnamalib_msg::clear()
{
	msg_list.clear();
	last_msg_space = true;
}

// ****************************************************************
// メッセージ追加
// ================================================================
void Cnamalib_msg::add(NAMALIB_MSG_MOD msg_mod, CTSTR& file_path, int line_no, CTSTR& line_str, CTSTR& msg_str, CTSTR& detail_str, bool open_flag, DWORD special_flag)
{
	NAMALIB_MSG_STRUCT msgst;
	msgst.msg_mod = msg_mod;
	msgst.file_path = file_path;
	msgst.line_no = line_no;
	msgst.line_str = line_str;
	msgst.msg_str = msg_str;
	msgst.detail_str = detail_str;
	msgst.open_flag = open_flag;
	msgst.special_flag = special_flag;
	msg_list.push_back(msgst);
}

// ****************************************************************
// 空メッセージ追加
// ================================================================
void Cnamalib_msg::add_space()
{
	add(__NAMALIB_MM_NONE, _T(""), -1, _T(""), _T(""), _T(""), false, 0);
}

// ****************************************************************
// メッセージのポインタ取得
// ================================================================
NAMALIB_MSG_STRUCT* Cnamalib_msg::get(int no)
{
	if (no >= (int)msg_list.size())	{	return NULL;	}
	return &msg_list[no];
}

}