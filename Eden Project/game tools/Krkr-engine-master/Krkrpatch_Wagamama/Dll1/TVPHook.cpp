#include "stdafx.h"
#include "TVPHook.h"
#include "shlwapi.h"
#include "tp_stub.h"
#include "detours.h"
#include <algorithm> 

#include "..\3rd\cryptopp-master\sha.h"
#include "..\3rd\cryptopp-master\salsa.h"
#include "..\3rd\cryptopp-master\chacha.h"
#include "..\3rd\cryptopp-master\osrng.h"
#include "..\3rd\cryptopp-master\cryptlib.h"
#include "..\3rd\cryptopp-master\hex.h"
#include "..\3rd\cryptopp-master\files.h"
#include "..\3rd\cryptopp-master\secblock.h"
#include "..\3rd\cryptopp-master\zlib.h"

#define RELEASE

//#include <VMProtectSDK.h>
#pragma comment(lib, "detours.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "cryptlib.lib")
TVPHook* TVPHook::handle = NULL;
using namespace std;

using namespace CryptoPP;

void GenSHA256(const wchar_t* Str, byte* Salt, char** SHAContent)
{
	CryptoPP::SHA256 hash;
	hash.Update((byte*)Str, lstrlen(Str));
	hash.Update(Salt, CryptoPP::Salsa20::IV_LENGTH);
	hash.Final((byte*)*SHAContent);
}

void FileNameToLower(std::wstring& FileName)
{
	std::transform(FileName.begin(), FileName.end(), FileName.begin(), ::tolower);
}

void EncHelper(const char* Buf, char** OutBuf, const char* Key, const char* Salt, int OriLen)
{
	try
	{
		CryptoPP::Salsa20::Encryption enc;
		enc.SetKeyWithIV((byte*)Key, CryptoPP::Salsa20::MAX_KEYLENGTH, (byte*)Salt, CryptoPP::Salsa20::IV_LENGTH);
		enc.ProcessData((byte*)*OutBuf, (byte*)Buf, OriLen);
	}
	catch (const CryptoPP::Exception& e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}

}

void EncHelper_chacha(const char* buff, char** out_buff, const char* key, const char* salt, int OriLen)
{
	try
	{
		ChaCha::Encryption enc;
		enc.SetKeyWithIV((byte*)key, ChaCha::MAX_KEYLENGTH, (byte*)salt, ChaCha::IV_LENGTH);
		enc.ProcessData((byte*)*out_buff, (byte*)buff, OriLen);
	}
	catch (const CryptoPP::Exception& e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}
}

void DecHelper(const char* Buf, char** OutBuf, const char* Key, const char* Salt, int OriLen)
{
	try
	{
		CryptoPP::Salsa20::Decryption dec;
		dec.SetKeyWithIV((byte*)Key, CryptoPP::Salsa20::MAX_KEYLENGTH, (byte*)Salt, CryptoPP::Salsa20::IV_LENGTH);
		dec.ProcessData((byte*)*OutBuf, (byte*)Buf, OriLen);
	}
	catch (const CryptoPP::Exception& e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}

}

void DecHelper_chacha(const char* buff, char** out_buff, const char* key, const char* salt, int OriLen)
{
	try
	{
		ChaCha::Decryption dec;
		dec.SetKeyWithIV((byte*)key, ChaCha::MAX_KEYLENGTH, (byte*)salt, ChaCha::IV_LENGTH);
		dec.ProcessData((byte*)*out_buff, (byte*)buff, OriLen);
	}
	catch (const CryptoPP::Exception& e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}
}

void inflatHelper(const char* Buf, uint32_t oriLen, uint32_t compLen, char** OutBuf)
{
	CryptoPP::ZlibDecompressor dec;
	dec.Put((byte*)Buf, compLen);
	dec.MessageEnd();
	dec.Get((byte*)*OutBuf, (size_t)oriLen);
}

void deflatHelper(const char* Buf, uint32_t oriLen, uint32_t* compLen, char** OutBuf)
{
	CryptoPP::ZlibCompressor cmp;
	cmp.Put((byte*)Buf, oriLen);
	cmp.MessageEnd();
	*compLen = cmp.MaxRetrievable();
	*OutBuf = new char[*compLen];
	cmp.Get((byte*)*OutBuf, (size_t)*compLen);
}

char* wtoc(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_ACP, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char *out = new char[dwMinSize];
	WideCharToMultiByte(CP_OEMCP, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

int WINAPI NewMultiByteToWideChar(UINT cp, DWORD dwFg, LPCSTR lpMBS, int cbMB, LPWSTR lpWCS, int ccWC)
{
	//MessageBoxW(0, L"Hooked!", L"Info", 0);
	auto obj = GetobjTVPhook();
	UINT outcp;
	switch (cp)
	{
	case CP_ACP:
	case CP_OEMCP:
	case CP_THREAD_ACP:
		outcp = 932;
		break;

	default:
		break;
	}
	return((PfuncMultiByteToWideChar)obj->g_pOldMultiByteToWideChar)(outcp, dwFg, lpMBS, cbMB, lpWCS, ccWC);
}


TVPHook::TVPHook()
{
	g_POldV2Link = NULL;
	g_POldCreateStream = NULL;
	g_pOldMultiByteToWideChar = NULL;
	g_pOldLoadLibraryW = NULL;
	inited = false;
}


TVPHook::~TVPHook()
{
}
PVOID GetTVPCreateStreamAddress()
{
	auto baseAddr = (DWORD)GetModuleHandle(NULL);
	//FIND TVPCreateStream at TVPCreateIStream Proc, the First Call
	PVOID CallIStreamStub = (PVOID)(baseAddr + 0xF3A0);
	return CallIStreamStub;
}

iTVPFunctionExporter* GetTVPFunctionExporter()
{
	auto baseAddr = (DWORD)GetModuleHandle(NULL);
	//FIND TVPFunctionExporter at LoadLibraryW position
	auto TVPFunctionExporter = (iTVPFunctionExporter*)(baseAddr + 0x3B4538);
	return TVPFunctionExporter;
}

tTJSBinaryStream* __fastcall HookTVPCreateStream(const ttstr & _name, tjs_uint32 flags)
{
	//VMProtectBeginUltra("HookTVPCreateStream");
	TVPHook*      objhook;
	objhook = GetobjTVPhook();
	if (!objhook->inited)
	{
		objhook->TVPFunctionExporter = GetTVPFunctionExporter();
		TVPInitImportStub(objhook->TVPFunctionExporter);
		objhook->inited = true;
	}
	tTJSBinaryStream*  Stream;
	IStream*           IStream;

	IStream = objhook->CreateLocalStream(_name.c_str());
	if (IStream)
	{
		Stream = TVPCreateBinaryStreamAdapter(IStream);
		if (!Stream)
		{
			Stream = ((PfuncCreateStream)objhook->g_POldCreateStream)(_name, flags);
		}
	}
	else
	{
		Stream = ((PfuncCreateStream)objhook->g_POldCreateStream)(_name, flags);
	}
	//VMProtectEnd();
	return Stream;
}

#pragma pack (1)
typedef struct {
	unsigned int FileCount;
	unsigned int index_size;
	char w_key[Salsa20::MAX_KEYLENGTH];
	char w_iv[Salsa20::IV_LENGTH];
}cpk_header_t;
typedef struct {
	char chacha_key[ChaCha::MAX_KEYLENGTH];
	char chacha_iv[ChaCha::IV_LENGTH];
	char FileName[SHA256::DIGESTSIZE];
	unsigned int CompSize;
	unsigned int FileSize;
	unsigned int offset;
}cpk_entry_t;
#pragma pack ()
cpk_header_t header;
cpk_entry_t entry;
FILE* cpk;
FILE* idx;
cpk_entry_t* entry_buff = nullptr;
int Init()
{
	//VMProtectBeginUltra("Init");
	cpk = _wfopen(L"Sarute.cpk", L"rb");

	idx = _wfopen(L"Sarute.lst", L"rb");
	fread(&header, sizeof(cpk_header_t), 1, idx);
	cout << header.FileCount << endl;
	entry_buff = (cpk_entry_t*)malloc(header.index_size);
	if (!entry_buff)
	{
		MessageBox(0, L"NO OUT_BUFF", 0, 0);
		return -1;
	}
	fread(entry_buff, header.index_size, 1, idx);
	//VMProtectEnd();
	return 0;
}


bool TVPHook::init()
{
#ifdef RELEASE
	if (Init() != 0)
		return false;
#endif
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	g_pOldMultiByteToWideChar = DetourFindFunction("Kernel32.dll", "MultiByteToWideChar");
	DetourAttach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	DetourTransactionCommit();
	auto res = GetTVPCreateStreamAddress();
	g_POldCreateStream = res;
	if (res != NULL)
	{
		DetourTransactionBegin();
		DetourAttach(&g_POldCreateStream, HookTVPCreateStream);
		DetourTransactionCommit();
	}
	return true;
}

int ReadFileByFileName(wstring fnm, BYTE** buff, DWORD* size)
{
	//FileNameToLower(fnm);
	char* OutSHA = (char*)malloc(SHA256::DIGESTSIZE);
	GenSHA256(fnm.c_str(), (byte*)header.w_iv, &OutSHA);
	for (int i = 0; i < header.FileCount; i++)
	{
		if (!memcmp(entry_buff[i].FileName, OutSHA, SHA256::DIGESTSIZE))
		{
			cout << (unsigned long long)OutSHA << endl;
			fseek(cpk, entry_buff[i].offset, SEEK_SET);
			BYTE* out_buff = nullptr;
			char* FinBuff1 = nullptr;
			char* FinBuff2 = nullptr;
			if (entry_buff[i].CompSize != 0)
			{
				out_buff = (BYTE*)malloc(entry_buff[i].CompSize);
				fread(out_buff, entry_buff[i].CompSize, 1, cpk);
				FinBuff1 = (char*)malloc(entry_buff[i].CompSize);
				FinBuff2 = (char*)malloc(entry_buff[i].CompSize);
				size_t FinSize = 0;
				DecHelper_chacha((char*)out_buff, &FinBuff1, entry_buff[i].chacha_key, entry_buff[i].chacha_iv, entry_buff[i].CompSize);
				DecHelper(FinBuff1, &FinBuff2, header.w_key, header.w_iv, entry_buff[i].CompSize);
				char* decomp_buff = (char*)malloc(entry_buff[i].FileSize);
				size_t decomp_size = entry_buff[i].FileSize;
				inflatHelper((char*)FinBuff2, decomp_size, entry_buff[i].CompSize, &decomp_buff);
				*buff = (BYTE*)decomp_buff;
				*size = decomp_size;
				free(out_buff);
				free(FinBuff1);
				free(FinBuff2);
			}
			else
			{
				out_buff = (BYTE*)malloc(entry_buff[i].FileSize);
				fread(out_buff, entry_buff[i].FileSize, 1, cpk);
				FinBuff1 = (char*)malloc(entry_buff[i].FileSize);
				FinBuff2 = (char*)malloc(entry_buff[i].FileSize);
				size_t FinSize = 0;
				DecHelper_chacha((char*)out_buff, &FinBuff1, entry_buff[i].chacha_key, entry_buff[i].chacha_iv, entry_buff[i].FileSize);
				DecHelper((char*)FinBuff1, &FinBuff2, header.w_key, header.w_iv, entry_buff[i].FileSize);
				*buff = (BYTE*)FinBuff2;
				*size = entry_buff[i].FileSize;
				free(out_buff);
				free(FinBuff1);
			}
			free(OutSHA);
			return 1;
		}
	}
	free(OutSHA);
	//MessageBox(0, L"NO SEARCH", 0, 0);
	return 0;
}


class tTJSCriticalSection
{
	CRITICAL_SECTION CS;
public:
	tTJSCriticalSection() { InitializeCriticalSection(&CS); }
	~tTJSCriticalSection() { DeleteCriticalSection(&CS); }

	void Enter() { EnterCriticalSection(&CS); }
	void Leave() { LeaveCriticalSection(&CS); }
};

class tTJSCriticalSectionHolder
{
	tTJSCriticalSection *Section;
public:
	tTJSCriticalSectionHolder(tTJSCriticalSection &cs)
	{
		Section = &cs;
		Section->Enter();
	}

	~tTJSCriticalSectionHolder()
	{
		Section->Leave();
	}

};

static tTJSCriticalSection LocalCreateStreamCS;

std::wstring GetKrkrFileName(LPCWSTR Name)
{
	std::wstring Info(Name);

	if (Info.find_last_of(L">") != std::wstring::npos)
		Info = Info.substr(Info.find_last_of(L">") + 1, std::wstring::npos);

	if (Info.find_last_of(L"/") != std::wstring::npos)
		Info = Info.substr(Info.find_last_of(L"/") + 1, std::wstring::npos);

	return Info;
}

bool PimgCheck(LPCWSTR Path)
{
	std::wstring info(Path);
	if (info.find(L"psb://")!=std::wstring::npos)
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::wstring GetPimgFileName(LPCWSTR Name)
{
	std::wstring Info(Name);
	std::wstring fname;
	std::wstring newpath;
	fname = Info.substr(Info.find_last_of(L"/") + 1, std::wstring::npos);
	newpath = Info.substr(Info.find(L"psb://")+6, Info.find(L".pimg")-6);
	newpath += L"\\";
	newpath += fname;
	return newpath;
}

IStream* TVPHook::CreateLocalStream(LPCWSTR lpFileName)
{
	std::wstring NewPath;
	tTJSCriticalSectionHolder CSHolder(LocalCreateStreamCS);
	std::wstring FileName;
	if (PimgCheck(lpFileName))
	{
		FileName = GetPimgFileName(lpFileName);
	}
	else
	{
		FileName = GetKrkrFileName(lpFileName);
	}
	FileNameToLower(FileName);
	NewPath = L"_Sarute\\" + FileName;
	//std::cout << "Path:" << wtoc(NewPath.c_str()) << std::endl;
	IStream* pStream;
	HRESULT hr;
	BYTE* buff;
	DWORD size;

	hr = SHCreateStreamOnFileEx(NewPath.c_str(), STGM_READ, 0, FALSE, NULL, &pStream);
	if (SUCCEEDED(hr))
	{
		cout << "SHCreateStreamOnFileEx:" << wtoc(FileName.c_str()) << endl;
		return pStream;
	}
	else
	{
#ifdef DEBUG
		goto ret;
#else
		if (ReadFileByFileName(FileName, &buff, &size))
#endif
		{
			cout << "SHCreateMemStream:" << wtoc(FileName.c_str()) << endl;
			pStream = SHCreateMemStream(NULL, NULL);
			pStream->Write(buff, size, NULL);
			LARGE_INTEGER pos;
			pos.LowPart = 0; pos.HighPart = 0;
			pStream->Seek(pos, STREAM_SEEK_SET, NULL);
			free(buff);
			return pStream;
		}
		else
		{
ret:
			return NULL;
		}
	}
}
TVPHook * GetobjTVPhook()
{
	if (TVPHook::handle==NULL)
	{
		TVPHook::handle = new TVPHook;	
	}
	return TVPHook::handle;
}

bool TVPHook::uninit()
{
	DetourDetach(&g_pOldMultiByteToWideChar, NewMultiByteToWideChar);
	g_POldV2Link = NULL;
	g_POldCreateStream = NULL;
	g_pOldMultiByteToWideChar = NULL;
	g_pOldLoadLibraryW = NULL;
	return false;
}