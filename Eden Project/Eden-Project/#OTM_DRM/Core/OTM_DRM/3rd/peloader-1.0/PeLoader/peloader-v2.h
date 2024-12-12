#ifndef PELOADER_V2_H
#define PELOADER_V2_H

#include "peloader.h"

// DLL�ļ������
typedef LPBYTE HDLLS;

#ifdef __cplusplus
extern "C" {
#endif

	// ����DLL�ļ���
	HDLLS WINAPI PeLoader_DllPackage();

	// ���DLL�ļ����� ��һ��DLL��Ϊ����DLL
	BOOL WINAPI PeLoader_DllPackage_AddData(HDLLS hDlls, LPCSTR lpName, LPBYTE lpData, DWORD dwLen);

	// �ͷ��ļ���
	VOID WINAPI PeLoader_DllPackage_Free(HDLLS hDlls);

	// ����ģ��
	HMODULE WINAPI PeLoader_LoadLibraryV2(HDLLS hDlls, DWORD dwFlags, PE_IMPORT_CALLBACK fnImportCallback, LPVOID lParam);

	// �ͷ�ģ��
	BOOL WINAPI PeLoader_FreeLibraryV2(HMODULE hMemModule);

	// ��ȡ�Զ������
	LPVOID WINAPI PeLoader_GetParamV2(HMODULE hMemModule);

	// ��ȡģ����
	BOOL WINAPI PeLoader_GetModuleFileNameV2(HMODULE hMemModule, LPSTR lpFilename, DWORD nSize);

#ifdef __cplusplus
}
#endif

#endif