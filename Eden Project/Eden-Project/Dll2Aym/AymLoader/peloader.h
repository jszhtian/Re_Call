#ifndef PELOADER_H
#define PELOADER_H

#include <Windows.h>

//���ط�ʽ
#define DONT_RESOLVE_DLL_REFERENCES	0x00000001	//����ʼ��ģ��
#define LOAD_LIBRARY_AS_DATAFILE	0x00000002	//��ִ����ڵ�

// �����ص�����
typedef enum PE_IMPORTS_TYPE {
	PE_IMPORTS_TYPE_INIT,
	PE_IMPORTS_TYPE_FREE
} PE_IMPORTS_TYPE;

// �����ص�
// �Ѵ����� TRUE
typedef BOOL(WINAPI* PE_IMPORT_CALLBACK)(
	LPVOID   lParam,		// �ص�����
	PE_IMPORTS_TYPE dwType, // ��������
	LPCSTR   lpModuleName,	// ģ����
	LPCSTR   lpProcName,	// ��������ֵ <= 65535 ��Ϊ��ţ�
	FARPROC* lppAddress		// ���ص�ַ���ͷ�ʱ���ԣ�
);

#ifdef __cplusplus
extern "C" {
#endif

	// ����ģ��
	HMODULE WINAPI PeLoader_LoadLibrary(LPBYTE lpData, DWORD dwLen, DWORD dwFlags, PE_IMPORT_CALLBACK fnImportCallback, LPVOID lParam);

	// �ͷ�ģ��
	BOOL WINAPI PeLoader_FreeLibrary(HMODULE hMemModule);

	// ȡ������ַ
	FARPROC WINAPI PeLoader_GetProcAddress(HMODULE hMemModule, LPCSTR lpProcName);

	// ȡ��ڵ�
	FARPROC WINAPI PeLoader_GetEntryPoint(HMODULE hMemModule);

	// ��ȡ�Զ������
	LPVOID WINAPI PeLoader_GetParam(HMODULE hMemModule);

	// �жϾ���Ƿ���Ч
	BOOL WINAPI PeLoader_IsModule(HMODULE hMemModule);

#ifdef __cplusplus
}
#endif

#endif