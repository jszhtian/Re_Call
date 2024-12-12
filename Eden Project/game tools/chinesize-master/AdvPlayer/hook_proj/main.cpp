#include <windows.h>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <D3D9.h>

#include "ilhook.h"
#include "FileReader.h"
#include "FuncHelper.h"
#include "PicSupport.h"
#include "TxtSupport.h"

#include "ws2.h"

using namespace std;

#define DP(name,addr,pat,hex) {name,addr,pat,hex,sizeof(hex)-1},
PatchStruct g_Patches[] = {
	DP(nullptr, 0x44ddd,"\x75\x81","\xb8\xa1") //quote
};
#undef DP

__declspec(naked) void D3dCreate9()
{
	__asm jmp Direct3DCreate9
}

bool IsHalf(const string& s)
{
	for (auto c : s)
	{
		if ((unsigned char)c >= 0x80)
		{
			return false;
		}
	}
	return true;
}

void HOOKFUNC MyCFI(LPLOGFONTW lfi)
{
	//"�ͣ� �����å�"
	if (wcscmp(lfi->lfFaceName, L"MS UI Gothic") == 0 ||
		wcscmp(lfi->lfFaceName, L"�ͣ� �����å�") == 0)
	{
		wcscpy_s(lfi->lfFaceName, L"SimHei");
	}
}

void HOOKFUNC MyCW(wchar_t** strp)
{
	if (*strp && wcscmp(*strp, L"��Ů����Ǥ����Υ��ꥢ�����������륢�󥳩`�롫") == 0)
	{
		*strp = L"������Ů����İ�֮���� ��Ϊ�������ϵ���������İ� | �����ǿպ����� ���� | ����Ⱥ�ţ�451450373";
	}
}

BOOL WINAPI DllMain(_In_ void* _DllHandle, _In_ unsigned long _Reason, _In_opt_ void* _Reserved)
{
	if (_Reason == DLL_PROCESS_ATTACH)
	{
		InitWs2();

		PatchMemory(g_Patches, ARRAYSIZE(g_Patches));

		static const HookPointStruct points[] = {
			//_wfopen_s�����ں�����һ���Ǻ���ͷ��һ�������м�call _wfopen_s������ȡarc��λ��
			{ nullptr, 0xCF6E0, MyOpenFile1, "12", false, 0 }, //����λ��Ҳ��Ҫ����
		{ nullptr, 0xCFC20, MyOpenFile, "r", false, 0 },

		//1.5�汾 0F B6 ?? ?? ?? 89 5C 24 ?? 8B 2C 85 ?? ?? ?? 00 85 ED 0F 84
		//�����������ҵ�һ�����������������ζ�ReadInst������call��hook ReadInst�ĺ���ͷ
		{ nullptr, 0xC6970, MyReadInst_v15, "rf", true, 0x10 },

		//1.7�汾 0f b6 45 ?? 33 ff 89 4d ?? 8b 04 85 ?? ?? ?? ?? 89 45 
		//�����������ҵ�һ�����������������ζ�ReadInst������call��hook ReadInst�ĺ���ͷ
		//{ nullptr, 0xCBD80, MyReadInst_v17, "rf", true, 0x14 },

		//1.5�汾 8B 45 ?? 2B 45 ?? 8B 33 C7 44 24 ?? 01 00 00 00 3B F0 72 05 E8
		//��������ͷ����Ϊhook��ַ�����¼�ʮ��֮���и�������lpMultiByteStr��call
		{ nullptr, 0xC6640, MySelString_v15, "r", false, 0 },

		//1.7�汾 C7 45 FC 02 00 00 00 8d 55 ?? 8b 4e ?? 03 0f e8
		//��������β����callָ�Ϊhook��ַ���˴�call�ĺ�������MyMbtowc hookλ�����ڵĺ���
		//�����ҵ�MyReadInst_v17�����������ڵĺ��������ϻ���һ�����㣬�ҵ�Variant��صĺ�����Ȼ���Ҳ�����lpMultiByteStr��call
		//{ nullptr, 0xCBB76, MySelString_v17, "r", false, 0 },

		//��λ���������Ǹ�call֮�ڣ�call MultiByteToWideChar��λ��
		{ nullptr, 0xAE4B8, MyMbtowc, "r", false, 0 },

		//wcsncmp�ĵڶ����������ڵĺ���ͷ
		{ nullptr, 0x2C4D0, MyChangeFont_v15, "r", false, 0 },
		};

		if (!HookFunctions(points))
		{
			MessageBox(0, L"Hook ʧ�ܣ�", 0, 0);
			return TRUE;
		}

		static const HookPointStructWithName points2[] = {
			{ "gdi32.dll", "CreateFontIndirectW", MyCFI, "1", false, 0 },
		{ "user32.dll", "CreateWindowExW", MyCW, "\x03", false, 0 },
		};
		if (!HookFunctions(points2))
		{
			MessageBox(0, L"Hook2 ʧ�ܣ�", 0, 0);
		}
	}
	return TRUE;
}