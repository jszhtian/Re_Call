#include	"tona3_pch.h"
#include	"tona3_util.h"
#include	"tona3_string.h"
#include	"tona3_app.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>

#include <strsafe.h>		// OS のバージョン判定用

#include "oauth.h"

namespace NT3
{
	// ****************************************************************
	// メッセージボックス
	// ================================================================
	int message_box(HWND h_wnd, CTSTR& msg, CTSTR& caption, UINT type)
	{
		return MessageBox(h_wnd, msg.c_str(), caption.c_str(), type);
	}
	int message_box(CTSTR& msg, CTSTR& caption, UINT type)
	{
		return MessageBox(G_app.h_wnd, msg.c_str(), caption.c_str(), type);
	}

	int error_box(CTSTR& msg, UINT type)
	{
		return MessageBox(G_app.h_wnd, msg.c_str(), _T("エラー"), type | MB_ICONWARNING);
	}

	int warning_box(CTSTR& msg, UINT type)
	{
		return MessageBox(G_app.h_wnd, msg.c_str(), _T("警告"), type | MB_ICONWARNING);
	}
	int warning_box(CTSTR& msg, CTSTR& caption, UINT type)
	{
		return MessageBox(G_app.h_wnd, msg.c_str(), caption.c_str(), type | MB_ICONWARNING);
	}

	int info_box(CTSTR& msg, UINT type)
	{
		return MessageBox(G_app.h_wnd, msg.c_str(), _T("情報"), type | MB_ICONINFORMATION);
	}
	int info_box(CTSTR& msg, CTSTR& caption, UINT type)
	{
		return MessageBox(G_app.h_wnd, msg.c_str(), caption.c_str(), type | MB_ICONINFORMATION);
	}

	// ****************************************************************
	// 型変換
	// ================================================================
	int bool_to_int(bool value)
	{
		return value ? 1 : 0;
	}

	// ****************************************************************
	// メモリ操作：メモリを指定した値で埋める（1byte 単位）
	// ================================================================
	void fill_memory(void* buf, int cnt, BYTE data)
	{
		// 4 byte 単位で埋めると高速
		DWORD data_4 = (data << 24) + (data << 16) + (data << 8) + data;
		int cnt_4 = cnt / 4;
		fill_memory_4(buf, cnt_4, data_4);

		// 余りを埋める
		int amari = cnt % 4;
		switch (amari)	{
			case 3:		*((BYTE *)buf + cnt - 3) = data;	// no break;
			case 2:		*((BYTE *)buf + cnt - 2) = data;	// no break;
			case 1:		*((BYTE *)buf + cnt - 1) = data;	// no break;
		}
	}

	// ****************************************************************
	// メモリ操作：メモリを指定した値で埋める（2byte 単位）
	// ================================================================
	void fill_memory_2(void* buf, int cnt, WORD data)
	{
		// 4 byte 単位で埋めると高速
		DWORD data_4 = (data << 16) + data;
		int cnt_4 = cnt / 2;
		fill_memory_4(buf, cnt_4, data_4);

		// 余りを埋める
		if (cnt % 2 == 1)
			*((WORD *)buf + cnt - 1) = data;
	}

	// ****************************************************************
	// メモリ操作：メモリを指定した値で埋める（4byte 単位）
	// ================================================================
	void fill_memory_4(void* buf, int cnt, DWORD data)
	{
		_asm	{
			cld
			mov		eax, data
			mov		edi, buf
			mov		ecx, cnt
			rep		stosd
		}
	}

	// ****************************************************************
	// ＴＰＣ暗号の暗号化
	// ================================================================

	static BYTE tpc_angou_table[256] = {
		0x8b,0xe5,0x5d,0xc3,0xa1,0xe0,0x30,0x44,0x00,0x85,0xc0,0x74,0x09,0x5f,0x5e,0x33,
		0xc0,0x5b,0x8b,0xe5,0x5d,0xc3,0x8b,0x45,0x0c,0x85,0xc0,0x75,0x14,0x8b,0x55,0xec,
		0x83,0xc2,0x20,0x52,0x6a,0x00,0xe8,0xf5,0x28,0x01,0x00,0x83,0xc4,0x08,0x89,0x45,
		0x0c,0x8b,0x45,0xe4,0x6a,0x00,0x6a,0x00,0x50,0x53,0xff,0x15,0x34,0xb1,0x43,0x00,
		0x8b,0x45,0x10,0x85,0xc0,0x74,0x05,0x8b,0x4d,0xec,0x89,0x08,0x8a,0x45,0xf0,0x84,
		0xc0,0x75,0x78,0xa1,0xe0,0x30,0x44,0x00,0x8b,0x7d,0xe8,0x8b,0x75,0x0c,0x85,0xc0,
		0x75,0x44,0x8b,0x1d,0xd0,0xb0,0x43,0x00,0x85,0xff,0x76,0x37,0x81,0xff,0x00,0x00,
		0x04,0x00,0x6a,0x00,0x76,0x43,0x8b,0x45,0xf8,0x8d,0x55,0xfc,0x52,0x68,0x00,0x00,
		0x04,0x00,0x56,0x50,0xff,0x15,0x2c,0xb1,0x43,0x00,0x6a,0x05,0xff,0xd3,0xa1,0xe0,
		0x30,0x44,0x00,0x81,0xef,0x00,0x00,0x04,0x00,0x81,0xc6,0x00,0x00,0x04,0x00,0x85,
		0xc0,0x74,0xc5,0x8b,0x5d,0xf8,0x53,0xe8,0xf4,0xfb,0xff,0xff,0x8b,0x45,0x0c,0x83,
		0xc4,0x04,0x5f,0x5e,0x5b,0x8b,0xe5,0x5d,0xc3,0x8b,0x55,0xf8,0x8d,0x4d,0xfc,0x51,
		0x57,0x56,0x52,0xff,0x15,0x2c,0xb1,0x43,0x00,0xeb,0xd8,0x8b,0x45,0xe8,0x83,0xc0,
		0x20,0x50,0x6a,0x00,0xe8,0x47,0x28,0x01,0x00,0x8b,0x7d,0xe8,0x89,0x45,0xf4,0x8b,
		0xf0,0xa1,0xe0,0x30,0x44,0x00,0x83,0xc4,0x08,0x85,0xc0,0x75,0x56,0x8b,0x1d,0xd0,
		0xb0,0x43,0x00,0x85,0xff,0x76,0x49,0x81,0xff,0x00,0x00,0x04,0x00,0x6a,0x00,0x76,
	};

	void tpc_angou_encode(BYTE* data, int size)
	{
		for (int i = 0; i < size; i++ )	{
			*data++ ^= tpc_angou_table[i % 256];
		}
	}

	// ****************************************************************
	// ＴＰＣ暗号の復号化
	// ================================================================
	void tpc_angou_decode(BYTE* data, int size)
	{
		for (int i = 0; i < size; i++ )	{
			*data++ ^= tpc_angou_table[i % 256];
		}
	}

	// ****************************************************************
	// URL エンコード
	// ================================================================
	TSTR url_encode(CTSTR& value) {

		TSTR result;

		MBSTR utf8 = TSTR_to_UTF8(value);

		for (MBSTR::const_iterator i = utf8.begin(), n = utf8.end(); i != n; ++i) {
			unsigned char c = (*i);

			if (c >= 0 && (('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '-' || c == '_' || c == '.' || c == '~')) {
				result.push_back(c);
				continue;
			}
			result += _T("%") + str_format(_T("%02X"), c);
		}

		return result;
	}

	// ****************************************************************
	// base64 エンコード
	// ================================================================
	TSTR base64_encode(ARRAY<BYTE>& buf) {

		std::string str = oauth_encode_base64(buf.get(), buf.size());
		return MBSTR_to_TSTR(str);
	}

	// ****************************************************************
	// HMAC-SHA1 base64 エンコード
	// ================================================================
	TSTR hmac_base64_encode(CTSTR& key, CTSTR& data) {
		MBSTR mb_key = TSTR_to_UTF8(key);
		MBSTR mb_data = TSTR_to_UTF8(data);

		MBSTR mb_result = oauth_sign_hmac_sha1((char *)mb_data.c_str(), (char *)mb_key.c_str());
		TSTR result = UTF8_to_TSTR(mb_result);

		return result;
	}

	// ****************************************************************
	// ＣＰＵＩＤが取得可能かを判定
	// ================================================================
	//	CPUID 命令は Pentium 以降に実装された命令なので、命令が使用できるかどうか判定する
	//	フラグレジスタの２１ビット目が書き換えられるかどうかをチェックする事で判定できる
	// ================================================================
	bool check_cpuid_enable()
	{
		DWORD dwTemp1;
		DWORD dwTemp2; 
		_asm{
			pushfd
			pop eax
			mov dwTemp1, eax
			xor eax, 00200000h
			push eax
			popfd
			pushfd
			pop eax
			mov dwTemp2, eax
		}

		if ( dwTemp1 != dwTemp2 )
			return true;

		return false;
	} 

	// ****************************************************************
	// コンピュータＩＤを取得
	// ================================================================
	DWORD get_cpu_id()
	{
		// ＣＰＵＩＤ取得
		DWORD cpu_id = 0;
		if (check_cpuid_enable())	{
			_asm{
				mov		eax, 1
				cpuid
				mov		cpu_id, eax
			}
		}

		return cpu_id;
	}

	DWORD get_computer_id()
	{
		// ＣＰＵＩＤ取得
		DWORD cpu_id = 0;
		if (check_cpuid_enable())	{
			_asm{
				mov		eax, 1
				cpuid
				mov		cpu_id, eax
			}
		}

		// ユーザー名
		// バッファのサイズが不足してユーザー名の一部を格納できない場合、この関数は失敗します。
		// UNLEN + 1 のバッファサイズを割り当てておくと、ユーザー名の最大の長さと終端の NULL 文字を格納できます。
		// UNLEN は、Lmcons.h 内で定義されています。（既定では 256 です）
		BYTE user_name[2048];
		DWORD user_name_size = 2048;
		GetUserNameA((char *)user_name, &user_name_size);	// マルチバイト版を使う

		// ユーザＩＤを取得
		DWORD *user_id = (DWORD *)user_name;
		*user_id = 0x64a3be9a;

		// コンピュータＩＤはＣＰＵＩＤとユーザＩＤから計算できる
		return (cpu_id ^ (*user_id) ^ 0x35a28bc7);
	}

	// ****************************************************************
	// OS のバージョン判別 (Windows 7 あたりまで対応)  
	// ================================================================
	// Getting the System Version (Windows)  
	// http://msdn2.microsoft.com/en-us/library/ms724429.aspx  
	// C言語から Windows のバージョンを判別する (Vistaまで)  
	// http://yandyle.blog121.fc2.com/blog-entry-174.html  
	// を参考に改良 
	// ================================================================
	//		http://pmakino.jp/tdiary/20090627.html よりこぴぺ。
	// ================================================================

	#pragma comment(lib, "user32.lib")  
	#pragma comment(lib, "advapi32.lib")  
    
	// GetVersionEx() で使われる定数 (VC++2005 未定義)  
	#define VER_SUITE_WH_SERVER 0x00008000  
    
	// GetSystemMetrics() で使われる定数 (VC++2005 未定義)  
	#define SM_TABLETPC     86  
	#define SM_MEDIACENTER  87  
	#define SM_STARTER      88  
	#define SM_SERVERR2     89  
    
	// GetProductInfo() で使われる定数 (VC++2005 未定義)  
	#define PRODUCT_UNDEFINED                           0x00000000 // An unknown product  
	#define PRODUCT_ULTIMATE                            0x00000001 // Ultimate Edition  
	#define PRODUCT_HOME_BASIC                          0x00000002 // Home Basic Edition  
	#define PRODUCT_HOME_PREMIUM                        0x00000003 // Home Premium Edition  
	#define PRODUCT_ENTERPRISE                          0x00000004 // Enterprise Edition  
	#define PRODUCT_HOME_BASIC_N                        0x00000005 // Home Basic Edition  
	#define PRODUCT_BUSINESS                            0x00000006 // Business Edition  
	#define PRODUCT_STANDARD_SERVER                     0x00000007 // Server Standard Edition  
	#define PRODUCT_DATACENTER_SERVER                   0x00000008 // Server Datacenter Edition  
	#define PRODUCT_SMALLBUSINESS_SERVER                0x00000009 // Small Business Server  
	#define PRODUCT_ENTERPRISE_SERVER                   0x0000000A // Server Enterprise Edition  
	#define PRODUCT_STARTER                             0x0000000B // Starter Edition  
	#define PRODUCT_DATACENTER_SERVER_CORE              0x0000000C // Server Datacenter Edition (core installation)  
	#define PRODUCT_STANDARD_SERVER_CORE                0x0000000D // Server Standard Edition (core installation)  
	#define PRODUCT_ENTERPRISE_SERVER_CORE              0x0000000E // Server Enterprise Edition (core installation)  
	#define PRODUCT_ENTERPRISE_SERVER_IA64              0x0000000F // Server Enterprise Edition for Itanium-based Systems  
	#define PRODUCT_BUSINESS_N                          0x00000010 // Business Edition  
	#define PRODUCT_WEB_SERVER                          0x00000011 // Web Server Edition  
	#define PRODUCT_CLUSTER_SERVER                      0x00000012 // Cluster Server Edition  
	#define PRODUCT_HOME_SERVER                         0x00000013 // Home Server Edition  
	#define PRODUCT_STORAGE_EXPRESS_SERVER              0x00000014 // Storage Server Express Edition  
	#define PRODUCT_STORAGE_STANDARD_SERVER             0x00000015 // Storage Server Standard Edition  
	#define PRODUCT_STORAGE_WORKGROUP_SERVER            0x00000016 // Storage Server Workgroup Edition  
	#define PRODUCT_STORAGE_ENTERPRISE_SERVER           0x00000017 // Storage Server Enterprise Edition  
	#define PRODUCT_SERVER_FOR_SMALLBUSINESS            0x00000018 // Server for Small Business Edition  
	#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM        0x00000019 // Small Business Server Premium Edition  
	#define PRODUCT_HOME_PREMIUM_N                      0x0000001A // Home Premium Edition  
	#define PRODUCT_ENTERPRISE_N                        0x0000001B // Enterprise Edition  
	#define PRODUCT_ULTIMATE_N                          0x0000001C // Ultimate Edition  
	#define PRODUCT_WEB_SERVER_CORE                     0x0000001D // Web Server Edition (core installation)  
	#define PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT    0x0000001E // Windows Essential Business Server Management Server  
	#define PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY      0x0000001F // Windows Essential Business Server Security Server  
	#define PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING     0x00000020 // Windows Essential Business Server Messaging Server  
	#define PRODUCT_SMALLBUSINESS_SERVER_PRIME          0x00000021 // Server Foundation  
	#define PRODUCT_HOME_PREMIUM_SERVER                 0x00000022 // Home Premium Server Edition?  
	#define PRODUCT_SERVER_FOR_SMALLBUSINESS_V          0x00000023 // Server 2008 without Hyper-V for Windows Essential Server Solutions  
	#define PRODUCT_STANDARD_SERVER_V                   0x00000024 // Server Standard Edition without Hyper-V  
	#define PRODUCT_DATACENTER_SERVER_V                 0x00000025 // Server Datacenter Edition without Hyper-V  
	#define PRODUCT_ENTERPRISE_SERVER_V                 0x00000026 // Server Enterprise Edition without Hyper-V  
	#define PRODUCT_DATACENTER_SERVER_CORE_V            0x00000027 // Server Datacenter Edition without Hyper-V (core installation)  
	#define PRODUCT_STANDARD_SERVER_CORE_V              0x00000028 // Server Standard Edition without Hyper-V (core installation)  
	#define PRODUCT_ENTERPRISE_SERVER_CORE_V            0x00000029 // Server Enterprise Edition without Hyper-V (core installation)  
	#define PRODUCT_HYPERV                              0x0000002A // Microsoft Hyper-V Server  
	#define PRODUCT_STORAGE_EXPRESS_SERVER_CORE         0x0000002B // Storage Server Express Edition (core installation)  
	#define PRODUCT_STORAGE_STANDARD_SERVER_CORE        0x0000002C // Storage Server Standard Edition (core installation)  
	#define PRODUCT_STORAGE_WORKGROUP_SERVER_CORE       0x0000002D // Storage Server Workgroup Edition (core installation)  
	#define PRODUCT_STORAGE_ENTERPRISE_SERVER_CORE      0x0000002E // Storage Server Enterprise Edition (core installation)  
	#define PRODUCT_STARTER_N                           0x0000002F // Starter Edition  
	#define PRODUCT_PROFESSIONAL                        0x00000030 // Professional Edition  
	#define PRODUCT_PROFESSIONAL_N                      0x00000031 // Professional Edition  
	#define PRODUCT_SB_SOLUTION_SERVER                  0x00000032  
	#define PRODUCT_SERVER_FOR_SB_SOLUTIONS             0x00000033  
	#define PRODUCT_STANDARD_SERVER_SOLUTIONS           0x00000034  
	#define PRODUCT_STANDARD_SERVER_SOLUTIONS_CORE      0x00000035 // (core installation)  
	#define PRODUCT_SB_SOLUTION_SERVER_EM               0x00000036  
	#define PRODUCT_SERVER_FOR_SB_SOLUTIONS_EM          0x00000037  
	#define PRODUCT_SOLUTION_EMBEDDEDSERVER             0x00000038  
	#define PRODUCT_SOLUTION_EMBEDDEDSERVER_CORE        0x00000039 // (core installation)  
	#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM_CORE   0x0000003F // (core installation)  
	#define PRODUCT_ESSENTIALBUSINESS_SERVER_MGMT       0x0000003B  
	#define PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL       0x0000003C  
	#define PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC    0x0000003D  
	#define PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC    0x0000003E  
	#define PRODUCT_CLUSTER_SERVER_V                    0x00000040 // Cluster Server Edition without Hyper-V  
	#define PRODUCT_EMBEDDED                            0x00000041  
	#define PRODUCT_UNLICENSED                          0xABCDABCD  
    
	typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);  
	typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);  
    
	BOOL GetOSDisplayString(LPTSTR pszOS, size_t bufsize)  
	{  
	  // GetVersionEx による基礎判別  
	  OSVERSIONINFOEX osvi;  
	  ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX)); // 必要あるのか?  
	  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);  
	  BOOL bOsVersionInfoEx;  
	  if ((bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*)&osvi)) == FALSE) {  
		// Windows NT 4.0 SP5 以前と Windows 9x  
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);  
		if (!GetVersionEx((OSVERSIONINFO *)&osvi)) return FALSE;  
	  }  
    
	  // Windows XP 以降では GetNativeSystemInfo を、それ以外では GetSystemInfo を使う  
	  SYSTEM_INFO si;  
	  ZeroMemory(&si, sizeof(SYSTEM_INFO)); // 必要あるのか?  
	  PGNSI pGNSI = (PGNSI)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo");  
	  if (pGNSI) pGNSI(&si);  
	  else GetSystemInfo(&si);  
    
	  // ここからバージョン別  
	  StringCchCopy(pszOS, bufsize, TEXT("")); // 念のため  
	  if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion >= 3) { // NT 系  
    
		// Vista / 2008 / 7 / 2008 R2  
		if (osvi.dwMajorVersion == 6) {  
		  TCHAR MajVerStr[9] = TEXT(""); // " 2008" か " 2008 R2" を入れる  
		  if (osvi.wProductType == VER_NT_WORKSTATION) { // Vista / 7  
			if (osvi.dwMinorVersion == 0)  
			  StringCchCopy(pszOS, bufsize, TEXT("Windows Vista "));  
			else if (osvi.dwMinorVersion == 1)  
			  StringCchCopy(pszOS, bufsize, TEXT("Windows 7 "));  
			else 
			  StringCchCopy(pszOS, bufsize, TEXT("Windows NT 6.x(unknown) "));  
		  }  
		  else { // Server 2008 / R2  
			StringCchCopy(pszOS, bufsize, TEXT("Windows Server"));  
			if (osvi.dwMinorVersion == 0)  
			  StringCchCopy(MajVerStr, bufsize, TEXT(" 2008"));  
			else if (osvi.dwMinorVersion == 1)  
			  StringCchCopy(MajVerStr, bufsize, TEXT(" 2008 R2"));  
			else 
			  StringCchCopy(MajVerStr, bufsize, TEXT(" 6.x(unknown)"));  
			StringCchCat(pszOS, bufsize, MajVerStr);  
			StringCchCat(pszOS, bufsize, TEXT(" "));  
		  }  
		  // Vista 以降では GetProductInfo が使える  
		  PGPI pGPI = (PGPI)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetProductInfo");  
		  DWORD dwType;  
		  pGPI(osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);  
		  switch (dwType) {  
			// Windows Vista / 7  
			case PRODUCT_ULTIMATE:  
			case PRODUCT_ULTIMATE_N:  
			  StringCchCat(pszOS, bufsize, TEXT("Ultimate Edition"));  
			  break;  
			case PRODUCT_HOME_BASIC:  
			case PRODUCT_HOME_BASIC_N:  
			  StringCchCat(pszOS, bufsize, TEXT("Home Basic Edition"));  
			  break;  
			case PRODUCT_HOME_PREMIUM:  
			case PRODUCT_HOME_PREMIUM_N:  
			  StringCchCat(pszOS, bufsize, TEXT("Home Premium Edition"));  
			  break;  
			case PRODUCT_ENTERPRISE:  
			case PRODUCT_ENTERPRISE_N:  
			  StringCchCat(pszOS, bufsize, TEXT("Enterprise Edition"));  
			  break;  
			case PRODUCT_BUSINESS:  
			case PRODUCT_BUSINESS_N:  
			  StringCchCat(pszOS, bufsize, TEXT("Business Edition"));  
			  break;  
			case PRODUCT_STARTER:  
			case PRODUCT_STARTER_N:  
			  StringCchCat(pszOS, bufsize, TEXT("Starter Edition"));  
			  break;  
			case PRODUCT_PROFESSIONAL:  
			case PRODUCT_PROFESSIONAL_N:  
			  StringCchCat(pszOS, bufsize, TEXT("Professional Edition"));  
			  break;  
			// Windows Server 2008 / R2  
			case PRODUCT_STANDARD_SERVER:  
			case PRODUCT_STANDARD_SERVER_CORE:  
			case PRODUCT_STANDARD_SERVER_V:  
			case PRODUCT_STANDARD_SERVER_CORE_V:  
			  StringCchCat(pszOS, bufsize, TEXT("Standard Edition"));  
			  break;  
			case PRODUCT_DATACENTER_SERVER:  
			case PRODUCT_DATACENTER_SERVER_CORE:  
			case PRODUCT_DATACENTER_SERVER_V:  
			case PRODUCT_DATACENTER_SERVER_CORE_V:  
			StringCchCat(pszOS, bufsize, TEXT("Datacenter Edition"));  
			  break;  
			case PRODUCT_ENTERPRISE_SERVER:  
			case PRODUCT_ENTERPRISE_SERVER_CORE:  
			case PRODUCT_ENTERPRISE_SERVER_V:  
			case PRODUCT_ENTERPRISE_SERVER_CORE_V:  
			case PRODUCT_ENTERPRISE_SERVER_IA64:  
			  StringCchCat(pszOS, bufsize, TEXT("Enterprise Edition"));  
			  if (dwType == PRODUCT_ENTERPRISE_SERVER_IA64)  
				StringCchCat(pszOS, bufsize, TEXT(" for Itanium-based Systems"));  
			  break;  
			case PRODUCT_WEB_SERVER:  
			case PRODUCT_WEB_SERVER_CORE:  
			  StringCchCopy(pszOS, bufsize, TEXT("Windows Web Server"));  
			  StringCchCat(pszOS, bufsize, MajVerStr);  
			  break;  
			case PRODUCT_CLUSTER_SERVER: // Cluster Server Edition?  
			  StringCchCopy(pszOS, bufsize, TEXT("Windows HPC Server"));  
			  StringCchCat(pszOS, bufsize, MajVerStr);  
			  break;  
			case PRODUCT_HOME_SERVER:  
			case PRODUCT_HOME_PREMIUM_SERVER: // Premium Edition?  
			  StringCchCopy(pszOS, bufsize, TEXT("Windows Home Server"));  
			  StringCchCat(pszOS, bufsize, MajVerStr);  
			  if (dwType == PRODUCT_HOME_PREMIUM_SERVER)  
				StringCchCat(pszOS, bufsize, TEXT(" Premium Edition"));  
			  break;  
			case PRODUCT_SMALLBUSINESS_SERVER:  
			case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:  
			  StringCchCopy(pszOS, bufsize, TEXT("Windows Small Business Server"));  
			  StringCchCat(pszOS, bufsize, MajVerStr);  
			  if (dwType == PRODUCT_SMALLBUSINESS_SERVER_PREMIUM)  
				StringCchCat(pszOS, bufsize, TEXT(" Premium Edition"));  
			  break;  
			case PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT:  
			case PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY:  
			case PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING:  
			  StringCchCopy(pszOS, bufsize, TEXT("Windows Essential Business Server"));  
			  StringCchCat(pszOS, bufsize, MajVerStr);  
			  StringCchCat(pszOS, bufsize, TEXT(" "));  
			  switch (dwType) {  
				case PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT:  
				  StringCchCat(pszOS, bufsize, TEXT("Management Server"));  
				case PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY:  
				  StringCchCat(pszOS, bufsize, TEXT("Security Server"));  
				case PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING:  
				  StringCchCat(pszOS, bufsize, TEXT("Messaging Server"));  
			  }  
			  break;  
			case PRODUCT_SMALLBUSINESS_SERVER_PRIME:  
			  StringCchCat(pszOS, bufsize, TEXT("Foundation"));  
			  break;  
			case PRODUCT_SERVER_FOR_SMALLBUSINESS:  
			  StringCchCat(pszOS, bufsize, TEXT("for Small Business Edition"));  
			  break;  
			case PRODUCT_SERVER_FOR_SMALLBUSINESS_V:  
			  StringCchCat(pszOS, bufsize, TEXT("for Windows Essential Server Solutions"));  
			  break;  
			case PRODUCT_HYPERV:  
			  StringCchCopy(pszOS, bufsize, TEXT("Hyper-V Server"));  
			  StringCchCat(pszOS, bufsize, MajVerStr);  
			  break;  
			case PRODUCT_STORAGE_EXPRESS_SERVER:  
			case PRODUCT_STORAGE_EXPRESS_SERVER_CORE:  
			case PRODUCT_STORAGE_STANDARD_SERVER:  
			case PRODUCT_STORAGE_STANDARD_SERVER_CORE:  
			case PRODUCT_STORAGE_WORKGROUP_SERVER:  
			case PRODUCT_STORAGE_WORKGROUP_SERVER_CORE:  
			case PRODUCT_STORAGE_ENTERPRISE_SERVER:  
			case PRODUCT_STORAGE_ENTERPRISE_SERVER_CORE:  
			  StringCchCopy(pszOS, bufsize, TEXT("Windows Storage Server"));  
			  StringCchCat(pszOS, bufsize, MajVerStr);  
			  StringCchCat(pszOS, bufsize, TEXT(" "));  
			  switch (dwType) {  
				case PRODUCT_STORAGE_EXPRESS_SERVER:  
				case PRODUCT_STORAGE_EXPRESS_SERVER_CORE:  
				  StringCchCopy(pszOS, bufsize, TEXT("Express"));  
				  break;  
				case PRODUCT_STORAGE_STANDARD_SERVER:  
				case PRODUCT_STORAGE_STANDARD_SERVER_CORE:  
				  StringCchCopy(pszOS, bufsize, TEXT("Standard"));  
				  break;  
				case PRODUCT_STORAGE_WORKGROUP_SERVER:  
				case PRODUCT_STORAGE_WORKGROUP_SERVER_CORE:  
				  StringCchCopy(pszOS, bufsize, TEXT("Workgroup"));  
				  break;  
				case PRODUCT_STORAGE_ENTERPRISE_SERVER:  
				case PRODUCT_STORAGE_ENTERPRISE_SERVER_CORE:  
				  StringCchCopy(pszOS, bufsize, TEXT("Enterprise"));  
				  break;  
			  }  
			  StringCchCat(pszOS, bufsize, TEXT(" Edition"));  
			  break;  
			default:  
			  StringCchCat(pszOS, bufsize, TEXT("Unknown Edition"));  
		  }  
		  // without Hyper-V  
		  switch (dwType) {  
			case PRODUCT_STANDARD_SERVER_V:  
			case PRODUCT_DATACENTER_SERVER_V:  
			case PRODUCT_ENTERPRISE_SERVER_V:  
			case PRODUCT_STANDARD_SERVER_CORE_V:  
			case PRODUCT_DATACENTER_SERVER_CORE_V:  
			case PRODUCT_ENTERPRISE_SERVER_CORE_V:  
			case PRODUCT_SERVER_FOR_SMALLBUSINESS_V:  
			  StringCchCat(pszOS, bufsize, TEXT(" without Hyper-V"));  
		  }  
		  // core installation  
		  switch (dwType) {  
			case PRODUCT_STANDARD_SERVER_CORE:  
			case PRODUCT_DATACENTER_SERVER_CORE:  
			case PRODUCT_ENTERPRISE_SERVER_CORE:  
			case PRODUCT_WEB_SERVER_CORE:  
			case PRODUCT_STANDARD_SERVER_CORE_V:  
			case PRODUCT_DATACENTER_SERVER_CORE_V:  
			case PRODUCT_ENTERPRISE_SERVER_CORE_V:  
			case PRODUCT_STORAGE_EXPRESS_SERVER_CORE:  
			case PRODUCT_STORAGE_STANDARD_SERVER_CORE:  
			case PRODUCT_STORAGE_WORKGROUP_SERVER_CORE:  
			case PRODUCT_STORAGE_ENTERPRISE_SERVER_CORE:  
			  StringCchCat(pszOS, bufsize, TEXT(" (core installation)"));  
		  }  
		  // x86 or x64  
		  if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)  
			StringCchCat(pszOS, bufsize, TEXT(", 64-bit"));  
		  else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)  
			StringCchCat(pszOS, bufsize, TEXT(", 32-bit"));  
		}  
    
		// Windows Server 2003 / XP x64 Edition  
		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) {  
		  if (osvi.wProductType == VER_NT_WORKSTATION) { // Windows XP Professional x64 Edition  
			StringCchCopy(pszOS, bufsize, TEXT("Windows XP Professional"));  
			if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) // 必ず該当するはずだが  
			  StringCchCat(pszOS, bufsize, TEXT(" x64 Edition"));  
		  }  
		  else { // Windows Server 2003 ファミリー  
			// Compute Cluster Server には R2 も x64 もつかない  
			if (osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER)  
			  StringCchCopy(pszOS, bufsize, TEXT("Windows Compute Cluster Server 2003"));  
			// Windows Home Server にも R2 はつかない (x64 は無い?)  
			else if (osvi.wSuiteMask & VER_SUITE_WH_SERVER)  
			  StringCchCopy(pszOS, bufsize, TEXT("Windows Home Server"));  
			else {  
			  // Small Business か Storage か無印か  
			  if (osvi.wSuiteMask & (VER_SUITE_SMALLBUSINESS | VER_SUITE_SMALLBUSINESS_RESTRICTED))  
				StringCchCopy(pszOS, bufsize, TEXT("Windows Small Business Server 2003"));  
			  else if (osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER)  
				StringCchCopy(pszOS, bufsize, TEXT("Windows Storage Server 2003"));  
			  else StringCchCopy(pszOS, bufsize, TEXT("Windows Server 2003"));  
			  // R2 か  
			  if (GetSystemMetrics(SM_SERVERR2))  
				StringCchCat(pszOS, bufsize, TEXT(" R2"));  
			  // Edition  
			  StringCchCat(pszOS, bufsize, TEXT(", "));  
			  if (osvi.wSuiteMask & VER_SUITE_DATACENTER)  
				StringCchCat(pszOS, bufsize, TEXT("Datacenter"));  
			  else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)  
				StringCchCat(pszOS, bufsize, TEXT("Enterprise"));  
			  else if (osvi.wSuiteMask & VER_SUITE_BLADE)  
				StringCchCat(pszOS, bufsize, TEXT("Web"));  
			  else StringCchCat(pszOS, bufsize, TEXT("Standard"));  
			  // x64 か  
			  if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)  
				StringCchCat(pszOS, bufsize, TEXT(" x64"));  
			  StringCchCat(pszOS, bufsize, TEXT(" Edition"));  
			  // IA64 か  
			  if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)  
				StringCchCat(pszOS, bufsize, TEXT(" for Itanium-based Systems"));  
			}  
		  }  
		}  
    
		// XP  
		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) {  
		  StringCchCopy(pszOS, bufsize, TEXT("Windows XP "));  
		  if (GetSystemMetrics(SM_TABLETPC))  
			StringCchCat(pszOS, bufsize, TEXT("Tablet PC Edition"));  
		  else if (GetSystemMetrics(SM_MEDIACENTER))  
			StringCchCat(pszOS, bufsize, TEXT("Media Center Edition"));  
		  else if (GetSystemMetrics(SM_STARTER))  
			StringCchCat(pszOS, bufsize, TEXT("Starter Edition"));  
		  else if (osvi.wSuiteMask & VER_SUITE_PERSONAL)  
			StringCchCat(pszOS, bufsize, TEXT("Home Edition"));  
		  else if (osvi.wSuiteMask & (VER_SUITE_EMBEDDEDNT | VER_SUITE_EMBEDDED_RESTRICTED))  
			StringCchCat(pszOS, bufsize, TEXT("Embedded"));  
		  else StringCchCat(pszOS, bufsize, TEXT("Professional"));  
		}  
    
		// 2000  
		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0) {  
		  StringCchCopy(pszOS, bufsize, TEXT("Windows 2000 "));  
		  if (osvi.wProductType == VER_NT_WORKSTATION)  
			StringCchCat(pszOS, bufsize, TEXT("Professional"));  
		  else {  
			if (osvi.wSuiteMask & VER_SUITE_DATACENTER)  
			  StringCchCat(pszOS, bufsize, TEXT("Datacenter "));  
			else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)  
			  StringCchCat(pszOS, bufsize, TEXT("Advanced "));  
			StringCchCat(pszOS, bufsize, TEXT("Server"));  
		  }  
		}  
    
		// NT 3 / 4  
		if (osvi.dwMajorVersion == 4 || osvi.dwMajorVersion == 3) {  
		  StringCchCopy(pszOS, bufsize, TEXT("Windows NT "));  
		  if (bOsVersionInfoEx) { // NT 4.0 SP6 だけのはず  
			if (osvi.wProductType == VER_NT_WORKSTATION) {  
			  if (osvi.dwMajorVersion == 4)  
				StringCchCat(pszOS, bufsize, TEXT("4.0"));  
			  else StringCchCat(pszOS, bufsize, TEXT("3.51")); // ないはずだが  
			  StringCchCat(pszOS, bufsize, TEXT(" Workstation"));  
			}  
			else {  
			  StringCchCat(pszOS, bufsize, TEXT("Server"));  
			  if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)  
				StringCchCat(pszOS, bufsize, TEXT(", Enterprise Edition"));  
			  else if (osvi.wSuiteMask & VER_SUITE_TERMINAL)  
				StringCchCat(pszOS, bufsize, TEXT(", Terminal Server Edition"));  
			  else if (osvi.wSuiteMask & (VER_SUITE_EMBEDDEDNT | VER_SUITE_EMBEDDED_RESTRICTED))  
				StringCchCat(pszOS, bufsize, TEXT(", Embedded Edition"));  
			  if (osvi.dwMajorVersion == 4)  
				StringCchCat(pszOS, bufsize, TEXT(" 4.0"));  
			  else StringCchCat(pszOS, bufsize, TEXT(" 3.51")); // ないはずだが  
			}  
		  }  
		  else { // NT 4.0 SP5 以前はレジストリから取得  
			HKEY hKey;  
			TCHAR szProductType[9] = TEXT("");  
			DWORD dwBufLen = sizeof(TCHAR) * sizeof(szProductType);  
			LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"), 0, KEY_QUERY_VALUE, &hKey);  
			if (lRet == ERROR_SUCCESS) {  
			  lRet = RegQueryValueEx(hKey, TEXT("ProductType"), NULL, NULL, (LPBYTE)szProductType, &dwBufLen);  
			  RegCloseKey(hKey);  
			}  
			if (lstrcmpi(szProductType, TEXT("LANMANNT")) == 0)  
			  StringCchCat(pszOS, bufsize, TEXT(" Server"));  
			if (lstrcmpi(szProductType, TEXT("SERVERNT")) == 0)  
			  StringCchCat(pszOS, bufsize, TEXT(" Server, Enterprise Edition"));  
			if (osvi.dwMajorVersion == 4)  
			  StringCchCat(pszOS, bufsize, TEXT("4.0"));  
			else StringCchCat(pszOS, bufsize, TEXT("3.51"));  
			if (lstrcmpi(szProductType, TEXT("WINNT")) == 0)  
			  StringCchCat(pszOS, bufsize, TEXT(" Workstation"));  
		  }  
		}  
    
		// Service Pack  
		if (_tcslen(osvi.szCSDVersion) > 0) {  
		  StringCchCat(pszOS, bufsize, TEXT(" ") );  
		  StringCchCat(pszOS, bufsize, osvi.szCSDVersion);  
		}  
    
		// build number  
		TCHAR buf[80];  
		StringCchPrintf(buf, 80, TEXT(" (build %d)"), osvi.dwBuildNumber);  
		StringCchCat(pszOS, bufsize, buf);  
		return TRUE;   
    
	  }  
    
	  // 9x  
	  else if (osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {  
		switch (osvi.dwMinorVersion) {  
		  case 0:  
			StringCchCopy(pszOS, bufsize, TEXT("Windows 95"));  
			if (_tcschr(osvi.szCSDVersion, 'A'))  
			  StringCchCat(pszOS, bufsize, TEXT(" OSR1"));  
			else if (_tcschr(osvi.szCSDVersion, 'B'))  
			  StringCchCat(pszOS, bufsize, TEXT(" OSR2"));  
			else if (_tcschr(osvi.szCSDVersion, 'C'))  
			  StringCchCat(pszOS, bufsize, TEXT(" OSR2.5"));  
			else StringCchCat(pszOS, bufsize, osvi.szCSDVersion); // ないはずだが  
			break;  
		  case 10:  
			StringCchCopy(pszOS, bufsize, TEXT("Windows 98"));  
			if (_tcschr(osvi.szCSDVersion, 'A'))  
			  StringCchCat(pszOS, bufsize, TEXT(" Second Edition"));  
			else StringCchCat(pszOS, bufsize, osvi.szCSDVersion); // ないはずだが  
			break;  
		  case 90:  
			StringCchCopy(pszOS, bufsize, TEXT("Windows Me"));  
			break;  
		  default:  
			StringCchCopy(pszOS, bufsize, TEXT("Windows 9x(unknown)")); // ないはずだが  
		}  
		return TRUE;  
	  }  
    
	  // NT でも 9x でもない (Win32s?)  
	  else {  
		StringCchCopy(pszOS, bufsize, TEXT("Unknown Windows"));  
		return FALSE;  
	  }  
	}  


	TSTR get_os_version_str()
	{
		TCHAR pszOS[256] = {0};
		if (GetOSDisplayString(pszOS, sizeof(pszOS)))
			return pszOS;
		else
			return _T("不明なバージョン");
	}



	// ****************************************************************
	// 現在の日付を文字列で取得
	// ================================================================
	TSTR get_sys_time_str()
	{
		// 起動日時を取得して、エラーログに出力します。
		SYSTEMTIME st;
		GetLocalTime(&st);

		return str_format(_T("%04d/%02d/%02d %02d:%02d:%02d.%04d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	}

	// ****************************************************************
	// UUID を文字列で取得
	// ================================================================
	TSTR get_uuid()
	{
		boost::uuids::uuid id = boost::uuids::random_generator()();
		return MBSTR_to_TSTR(boost::lexical_cast<std::string>(id).c_str());
	}

}
