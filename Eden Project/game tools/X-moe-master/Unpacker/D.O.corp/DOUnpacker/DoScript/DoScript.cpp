#include "WinFile.h"

#include <stdio.h>
#include <vector>
#include <string>

using std::vector;
using std::string;

//这个文件必须保存为UTF-16
typedef struct UserCodePage
{
	BYTE CharCode;
	WCHAR* UnicodeChar;
}UserCodePage;


#define BUILD_EXTRA_LINE 1

#define JPN

/*
智障引擎欢乐多
 Translation Port:  sub_408150

CPU Dump
Address   Hex dump                                         Multibyte (ANSI/O>
00468C84  81 40 81 40|81 41 81 42|81 45 81 48|81 49 81 69| 　　、。・？！（
00468C94  81 6A 81 75|81 76 82 4F|82 50 82 51|82 52 82 53| ）「」０１２３４
00468CA4  82 54 82 55|82 56 82 57|82 58 82 A0|82 A2 82 A4| ５６７８９あいう
00468CB4  82 A6 82 A8|82 A9 82 AA|82 AB 82 AC|82 AD 82 AE| えおかがきぎくぐ
00468CC4  81 40 82 B0|82 B1 82 B2|82 B3 82 B4|82 B5 82 B6| 　げこごさざしじ
00468CD4  82 B7 82 B8|82 B9 82 BA|82 BB 82 BC|82 BD 82 BE| すずせぜそぞただ
00468CE4  82 BF 82 C0|82 C1 82 C2|82 C3 82 C4|82 C5 82 C6| ちぢっつづてでと
00468CF4  82 C7 82 C8|82 C9 82 CA|82 CB 82 CC|82 CD 82 CE| どなにぬねのはば
00468D04  82 D0 82 D1|82 D3 82 D4|82 D6 82 D7|82 D9 82 DA| ひびふぶへべほぼ
00468D14  82 DC 82 DD|82 DE 82 DF|82 E0 82 E1|82 E2 82 E3| まみむめもゃやゅ
00468D24  82 E4 82 E5|82 E6 82 E7|82 E8 82 E9|82 EA 82 EB| ゆょよらりるれろ
00468D34  82 ED 82 F0|82 F1 83 41|83 43 83 45|83 47 83 49| わをんアイウエオ
00468D44  83 4A 83 4C|83 4E 83 50|83 52 83 54|83 56 83 58| カキクケコサシス
00468D54  83 5A 83 5C|83 5E 83 60|83 62 83 63|83 65 83 67| セソタチッツテト
00468D64  83 69 83 6A|82 AF 83 6C|83 6D 83 6E|83 71 83 74| ナニけネノハヒフ
00468D74  83 77 83 7A|83 7D 83 7E|83 80 83 81|83 82 83 84| ヘホマミムメモヤ
*/
UserCodePage UserCode[] = 
{
	{ 0x00, L"　" },
	{ 0x01, L"　" },
	{ 0x02, L"、" },
	{ 0x03, L"。" },
	{ 0x04, L"・" },
	{ 0x05, L"？" },
	{ 0x06, L"！" },
	{ 0x07, L"（" },
	{ 0x08, L"）" },
	{ 0x09, L"「" },
	{ 0x0a, L"」" },
	{ 0x0b, L"０" },
	{ 0x0c, L"１" },
	{ 0x0d, L"２" },
	{ 0x0e, L"３" },
	{ 0x0f, L"４" },
	{ 0x10, L"５" },
	{ 0x11, L"６" },
	{ 0x12, L"７" },
	{ 0x13, L"８" },
	{ 0x14, L"９" },
	/*****************************/
	{ 0x15, L"あ" },
	{ 0x16, L"い" },
	{ 0x17, L"う" },
	{ 0x18, L"え" },
	{ 0x19, L"お" },
	{ 0x1a, L"か" },
	{ 0x1b, L"が" },
	{ 0x1c, L"き" },
	{ 0x1d, L"ぎ" },
	{ 0x1e, L"く" },
	{ 0x1f, L"ぐ" },
	{ 0x20, L"　" },
	{ 0x21, L"げ" },
	{ 0x22, L"こ" },
	{ 0x23, L"ご" },
	{ 0x24, L"さ" },
	{ 0x25, L"ざ" },
	{ 0x26, L"し" },
	{ 0x27, L"じ" },
	{ 0x28, L"す" },
	{ 0x29, L"ず" },
	{ 0x2a, L"せ" },
	{ 0x2b, L"ぜ" },
	{ 0x2c, L"そ" },
	{ 0x2d, L"ぞ" },
	{ 0x2e, L"た" },
	{ 0x2f, L"だ" },
	{ 0x30, L"ち" },
	{ 0x31, L"ぢ" },
	{ 0x32, L"っ" },
	{ 0x33, L"つ" },
	{ 0x34, L"づ" },
	{ 0x35, L"て" },
	{ 0x36, L"で" },
	{ 0x37, L"と" },
	{ 0x38, L"ど" },
	{ 0x39, L"な" },
	{ 0x3a, L"に" },
	{ 0x3b, L"ぬ" },
	{ 0x3c, L"ね" },
	{ 0x3d, L"の" },
	{ 0x3e, L"は" },
	{ 0x3f, L"ば" },
	{ 0x40, L"ひ" },
	{ 0x41, L"び" },
	{ 0x42, L"ふ" },
	{ 0x43, L"ぶ" },
	{ 0x44, L"へ" },
	{ 0x45, L"べ" },
	{ 0x46, L"ほ" },
	{ 0x47, L"ぼ" },
	{ 0x48, L"ま" },
	{ 0x49, L"み" },
	{ 0x4a, L"む" },
	{ 0x4b, L"め" },
	{ 0x4c, L"も" },
	{ 0x4d, L"ゃ" },
	{ 0x4e, L"や" },
	{ 0x4f, L"ゅ" },
	{ 0x50, L"ゆ" },
	{ 0x51, L"ょ" },
	{ 0x52, L"よ" },
	{ 0x53, L"ら" },
	{ 0x54, L"り" },
	{ 0x55, L"る" },
	{ 0x56, L"れ" },
	{ 0x57, L"ろ" },
	{ 0x58, L"わ" },
	{ 0x59, L"を" },
	{ 0x5a, L"ん" },

	/***************************/
	{ 0x5b, L"ア" },
	{ 0x5c, L"イ" },
	{ 0x5d, L"ウ" },
	{ 0x5e, L"エ" },
	{ 0x5f, L"オ" },
	{ 0x60, L"カ" },
	{ 0x61, L"キ" },
	{ 0x62, L"ク" },
	{ 0x63, L"ケ" },
	{ 0x64, L"コ" },
	{ 0x65, L"サ" },
	{ 0x66, L"シ" },
	{ 0x67, L"ス" },
	{ 0x68, L"セ" },
	{ 0x69, L"ソ" },
	{ 0x6a, L"タ" },
	{ 0x6b, L"チ" },
	{ 0x6c, L"ッ" },
	{ 0x6d, L"ツ" },
	{ 0x6e, L"テ" },
	{ 0x6f, L"ト" },
	{ 0x70, L"ナ" },
	{ 0x71, L"ニ" },
	{ 0x72, L"け" }, 
	{ 0x73, L"ネ" },
	{ 0x74, L"ノ" },
	{ 0x75, L"ハ" },
	{ 0x76, L"ヒ" },
	{ 0x77, L"フ" },
	{ 0x78, L"ヘ" },
	{ 0x79, L"ホ" },
	{ 0x7a, L"マ" },
	{ 0x7b, L"ミ" },
	{ 0x7c, L"ム" },
	{ 0x7d, L"メ" },
	{ 0x7e, L"モ" },
	{ 0x7f, L"ヤ" }
};

//sub_42EF00
string RebuildString(PBYTE in)
{
	string JPString;
	JPString.resize(500);
	JPString.clear();
	string UTF8String;

	ULONG i = 0;
	while (i < lstrlenA((PCHAR)in))
	{
#ifdef ENG
		if (in[i] == 0x7F)
		{
			i++;
			continue;
		}
		JPString += (CHAR)in[i];
		i++;
		//JPString += (CHAR)in[i];
		//i++;

		WCHAR TempWChar[10] = { 0 };
		char TempChar[10] = { 0 };
		MultiByteToWideChar(932, 0, JPString.c_str(), JPString.length(), TempWChar, 10);
		WideCharToMultiByte(CP_UTF8, 0, TempWChar, lstrlenW(TempWChar), TempChar, 10,
			nullptr, nullptr);
		UTF8String += TempChar;
		JPString.clear();
#else
		if (in[i] & 0x80)
		{
			JPString += (CHAR)in[i];
			i++;
			JPString += (CHAR)in[i];
			i++;

			WCHAR TempWChar[10] = { 0 };
			char TempChar[10] = { 0 };
			MultiByteToWideChar(932, 0, JPString.c_str(), JPString.length(), TempWChar, 10);
			WideCharToMultiByte(CP_UTF8, 0, TempWChar, lstrlenW(TempWChar), TempChar, 10,
				nullptr, nullptr);
			UTF8String += TempChar;
			JPString.clear();
		}
		else
		{
			CHAR CodeName[MAX_PATH] = { 0 };
			if (lstrlenW(UserCode[in[i]].UnicodeChar))
			{
				char TempChar[10] = { 0 };
				WideCharToMultiByte(CP_UTF8, 0, UserCode[in[i]].UnicodeChar,
					lstrlenW(UserCode[in[i]].UnicodeChar), TempChar, 10, nullptr, nullptr);
				UTF8String += TempChar;
			}
			else
			{
				wsprintfA(CodeName, "<Code:%02x>", in[i]);
				UTF8String += CodeName;
			}
			i++;
		}
#endif
	}
	return UTF8String;
}


//@sub_42EB20
int wmain(int argc, WCHAR* argv[])
{
	if (argc != 2)
		return 0;

	WinFile InFile, OutFile;

	PBYTE Buffer = nullptr;
	ULONG Size;

	PBYTE newBuffer = nullptr;
	ULONG newSize = 0;
	BOOL  NextDecode = FALSE;
	BOOL  ooxx = FALSE;

	do
	{
		if (InFile.Open(argv[1], WinFile::FileRead) != S_OK)
			break;

		Size = InFile.GetSize32();
		Buffer = (PBYTE)HeapAlloc(GetProcessHeap(), 0, Size);

		if (!Buffer)
			break;

		vector<DWORD> ResourcePool;

		InFile.Read(Buffer, Size);

		//sub_42EB20
		{
			DWORD Result = 0;
			__asm
			{
				mov eax, Buffer
				xor ebx, ebx
				xor edi, edi
				mov cx, [eax + 4]
				mov dl, [eax + 6]
				lea edi, [eax + 8]
				xor eax, eax
				mov ah, cl
				mov bl, ch
				and eax, 0x0000FFFF
				and ebx, 0x000000FF
				or  eax, ebx
				mov Result, eax
			}


			if (Result > 38295)
			{
				if (Result == 38865)
				{
					DWORD DecodeSize = Size - 8;
					DWORD iPos = 8;
					if (DecodeSize > 0)
					{
						do
						{
							Buffer[iPos] = ~Buffer[iPos];
							iPos++;
							DecodeSize--;
						} while (DecodeSize);
					}
					NextDecode = TRUE;
				}
				else
				{

				}
			}
			else if (Result == 38295)
			{
				DWORD DecodeSize = Size - 8;
				DWORD iPos = 8;
				do
				{
					Buffer[iPos] = (Buffer[iPos] >> 2) | (Buffer[iPos] << 6);
					iPos++;
					DecodeSize--;
				} while (DecodeSize);
				NextDecode = TRUE;
			}
			else
			{
				if (Result != 35278)
				{
					if (Result == 36343)
					{
						NextDecode = TRUE;
						ooxx = TRUE;
					}
				}
				else
				{
					DWORD DecodeSize = Size - 8;
					DWORD iPos = 8;
					BYTE XorKey = Buffer[6];
					if (DecodeSize > 0)
					{
						do
						{
							Buffer[iPos] ^= XorKey;
							iPos++;
							DecodeSize--;
						} while (DecodeSize);
					}
				}
			}

			//goto WriteNo2Pass;

			if (ooxx)
			{
				MessageBoxW(NULL, L"Failed to decode", 0, 0);
				return 0;
			}

			if (NextDecode)
			{
				//Offset
				DWORD v21 = *(DWORD *)Buffer;

				PBYTE PtrOffset = nullptr;
				DWORD AlignMemSize = 0;
				if (ooxx == 0)
				{
					PtrOffset = Buffer + 8;
					AlignMemSize = (v21 - 8) / 4;
				}
				else
				{
					PtrOffset = Buffer + 16;
					AlignMemSize = (v21 - 16) / 4;
				}

				for (ULONG i = 0; i < AlignMemSize; ++i)
				{
					DWORD v25 = *(DWORD *)PtrOffset + *(DWORD *)Buffer;
					PtrOffset += 4;
					ResourcePool.push_back(v25);
				}

				DWORD AdjustSize = 0;
				if (ooxx)
					AdjustSize = 4 * AlignMemSize + 16;
				else
					AdjustSize = 4 * AlignMemSize + 8;
				
				WCHAR lpFileName[MAX_PATH] = { 0 };
				wsprintfW(lpFileName, L"%s.txt", argv[1]);

				FILE * fout = _wfopen(lpFileName, L"wb");
				while (AdjustSize < Size - 4)
				{
					if (*(PDWORD)(Buffer + AdjustSize) == 0xFF800000)
					{
						AdjustSize += 4;
						/*
#ifndef BUILD_EXTRA_LINE
						fprintf(fout, "[0x%08x]%s\r\n", AdjustSize, RebuildString((Buffer + AdjustSize)).c_str());
						fprintf(fout, ";[0x%08x]\r\n\r\n", AdjustSize);
#else
						fprintf(fout, "[0x%08x]%s\r\n", AdjustSize, RebuildString((Buffer + AdjustSize)).c_str());
						fprintf(fout, ";[0x%08x]\r\n", AdjustSize);
						fprintf(fout, ">[0x%08x]\r\n\r\n", AdjustSize);
#endif
						AdjustSize += lstrlenA((PCHAR)(Buffer + AdjustSize)) + 1;
						*/
						//fprintf(fout, "%s\r\n", RebuildString((Buffer + AdjustSize)).c_str());
						fprintf(fout, "[0x%08x|0x%08x]%s\r\n", AdjustSize, AdjustSize - v21, RebuildString((Buffer + AdjustSize)).c_str());
					}
					/*
					else if (*(PDWORD)(Buffer + AdjustSize) == 0x01FF8000)
					{
						AdjustSize += 4;
						fprintf(fout, "%s\r\n", RebuildString((Buffer + AdjustSize)).c_str());
						AdjustSize += lstrlenA((PCHAR)(Buffer + AdjustSize)) + 1;
					}
					else if (*(PDWORD)(Buffer + AdjustSize) == 0x09FF8000)
					{
						AdjustSize += 4;
						fprintf(fout, "%s\r\n", RebuildString((Buffer + AdjustSize)).c_str());
						AdjustSize += lstrlenA((PCHAR)(Buffer + AdjustSize)) + 1;
					}
					*/
					else
					{
						AdjustSize++;
					}
				}
				fclose(fout);

				goto WriteNo2Pass;

				//ptr to encrypted code
				PBYTE PtrOffset2 = Buffer + AdjustSize;

				PBYTE BufferStart = Buffer;
				DWORD DataSize = (Size - AdjustSize + 3) / 4;
				if (DataSize > 0)
				{
					do
					{
						DWORD v30 = *(DWORD *)PtrOffset2;
						PtrOffset2 += 4;
						*(DWORD *)BufferStart = v30;
						BufferStart += 4;
						--DataSize;
					} while (DataSize);
				}
			}
		}

	WriteNo2Pass:

		/*
		WCHAR lpFileName[MAX_PATH] = { 0 };
		wsprintfW(lpFileName, L"%s.out", argv[1]);
		
		if (OutFile.Open(lpFileName, WinFile::FileWrite) != S_OK)
			break;

		OutFile.Write(Buffer, Size);
		*/
		//OutFile.Write(newBuffer, newSize);
		__asm nop;
	} 
	while (0);

	if (Buffer)
		HeapFree(GetProcessHeap(), 0, Buffer);

	InFile.Release();
	OutFile.Release();
	return 0;
}

