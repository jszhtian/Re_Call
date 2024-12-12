#include "framework.h"

/*
void ScriptInsert(wchar_t* Old, char* PushStr)
{
	copy_str(Old, L"夏休み。", PushStr, (char*)"暑假。");
	copy_str(Old, L"セーブメニュー", PushStr, (char*)"存档界面");
	copy_str(Old, L"ロードメニュー", PushStr, (char*)"读档界面");
	copy_str(Old, L"履歴", PushStr, (char*)"履历");
	copy_str(Old, L"システムメニュー", PushStr, (char*)"系统菜单");
	copy_str(Old, L"メニューの固定", PushStr, (char*)"固定菜单");
	copy_str(Old, L"ウィンドウの消去", PushStr, (char*)"隐藏对话框");
	copy_str(Old, L"クイックロード", PushStr, (char*)"快速读档");
	copy_str(Old, L"クイックセーブ", PushStr, (char*)"快速存档");
	copy_str(Old, L"次の選択肢に進む", PushStr, (char*)"快进到下一个选项");
	copy_str(Old, L"オート", PushStr, (char*)"自动模式");

}
*/

TransText::TransText()
{
	CurIdx = -1;
	TableLen = 0;
	StorageBuf = nullptr;
	SkipList[0] = 0x00000000;
	SkipList[1] = 0x00000000;
}


TransText::~TransText()
{
}

wchar_t* TransText::searchtext(wchar_t* RawStr)
{
	CRC32 crc32;
	crc32.Init();
	int Strlen = wcslen(RawStr);
	__int32 strcrc = crc32.Calc((char*)RawStr, Strlen);

	wchar_t* Ret = nullptr;
	TextStruct* ptr = nullptr;
	for (auto& var : SkipList)
	{
		if (strcrc == var)
		{
			return nullptr;
		}
	}

	if (StorageBuf != nullptr)
	{
		ptr = StorageBuf;
	}
	else
	{
		return nullptr;
	}
	if (CurIdx == -1)
	{
		CurIdx = 0;
		Ret = NewSearch(strcrc);
	}
	else
	{
		if (CurIdx > TableLen)
		{
			Ret = NewSearch(strcrc);
		}
		if (CurIdx < 0)
		{
			Ret = NewSearch(strcrc);
		}
		else
		{
			if (ptr[CurIdx].crc32val == strcrc)
			{
				Ret = ptr[CurIdx].str;
				CurIdx++;
			}
			else
			{
				Ret = NewSearch(strcrc);
			}

		}

	}
	return Ret;
}

void TransText::setStorage(TextStruct* Buf)
{
	StorageBuf = Buf;
}

void TransText::setTableLength(__int32 len)
{
	TableLen = len;
}

wchar_t* TransText::NewSearch(__int32 crcval)
{
	wchar_t* ret = nullptr;
	TextStruct* TablePtr = nullptr;
	if (StorageBuf != nullptr)
	{
		TablePtr = StorageBuf;
	}
	else
	{
		return nullptr;
	}

	wchar_t* tmpret1 = nullptr;
	wchar_t* tmpret2 = nullptr;
	int tmpidx1 = -1;
	int tmpidx2 = -1;

	volatile bool flag1 = false;
#pragma omp parallel for shared(flag1)
	for (int i = CurIdx; i < TableLen; i++)
	{
		if (flag1) continue;
		if (TablePtr[i].crc32val == crcval)
		{
			tmpret1 = TablePtr[i].str;
			tmpidx1 = i + 1;
			flag1 = true;
		}
	}
	if (ret == nullptr)
	{
		volatile bool flag2 = false;
#pragma omp parallel for shared(flag2)
		for (int i = CurIdx - 1; i >= 0; i--)
		{
			if (flag2) continue;
			if (TablePtr[i].crc32val == crcval)
			{
				tmpret2 = TablePtr[i].str;
				tmpidx2 = i + 1;
				flag2 = true;
			}
		}
	}
	if (tmpidx1 == -1)
	{
		CurIdx = tmpidx2;
		ret = tmpret2;
	}
	if (tmpidx2 == -1)
	{
		CurIdx = tmpidx1;
		ret = tmpret1;
	}
	if (tmpidx1 != -1 && tmpidx2 != -1)
	{
		auto desIdx1 = abs(CurIdx - tmpidx1);
		auto desIdx2 = abs(CurIdx - tmpidx2);
		if (desIdx1 < desIdx2)
		{
			CurIdx = tmpidx1;
			ret = tmpret1;
		}
		else
		{
			CurIdx = tmpidx2;
			ret = tmpret2;
		}
	}
	return ret;
}