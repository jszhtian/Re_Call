#include	"pch.h"
#include	"namalib_sys_token_map.h"

namespace Nnama
{

// ****************************************************************
// コンストラクタ
// ================================================================
Cnamalib_token_map::Cnamalib_token_map()
{
	clear();
}

// ****************************************************************
// クリア
// ================================================================
void Cnamalib_token_map::clear()
{
	token_map.clear();
}

// ****************************************************************
// 追加
// ================================================================
void Cnamalib_token_map::add(TCHAR** token_list, DWORD list_cnt)
{
	TOKEN_PAIR pair;
	for (DWORD i = 0; i < list_cnt; i++)	{
		pair.first = *token_list;
		pair.second = i;
		token_map.insert(pair);
		token_list++;
	}
}

// ****************************************************************
// 検索
// ================================================================
int Cnamalib_token_map::find(CTSTR& token)
{
	if (!token.empty())	{
		if (!token_map.empty())	{
			TOKEN_MAP::iterator itr = token_map.find(token);
			if (itr != token_map.end())		{	return itr->second;	}
		}
	}
	return -1;
}

}