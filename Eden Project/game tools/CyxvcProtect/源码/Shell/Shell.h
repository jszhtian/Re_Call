#ifdef SHELL_EXPORTS
#define SHELL_API __declspec(dllexport)
#else
#define SHELL_API __declspec(dllimport)
#endif

//����ShellData�ṹ��
extern"C"  typedef struct _SHELL_DATA
{
	DWORD dwStartFun;							//��������
	DWORD dwPEOEP;								//������ڵ�
	DWORD dwXorKey;								//����KEY
	DWORD dwCodeBase;							//�������ʼ��ַ
	DWORD dwXorSize;							//����μ��ܴ�С
	DWORD dwPEImageBase;						//PE�ļ�ӳ���ַ

	IMAGE_DATA_DIRECTORY	stcPERelocDir;		//�ض�λ����Ϣ
	IMAGE_DATA_DIRECTORY	stcPEImportDir;		//�������Ϣ

	DWORD					dwIATSectionBase;	//IAT���ڶλ�ַ
	DWORD					dwIATSectionSize;	//IAT���ڶδ�С

	BOOL					bIsShowMesBox;		//�Ƿ���ʾMessageBox

}SHELL_DATA, *PSHELL_DATA;

//����ShellData�ṹ�����
extern"C" SHELL_API SHELL_DATA g_stcShellData;



//Shell�����õ��ĺ��������Ͷ���
typedef DWORD(WINAPI *fnGetProcAddress)(_In_ HMODULE hModule, _In_ LPCSTR lpProcName);
typedef HMODULE(WINAPI *fnLoadLibraryA)(_In_ LPCSTR lpLibFileName);
typedef HMODULE(WINAPI *fnGetModuleHandleA)(_In_opt_ LPCSTR lpModuleName);
typedef BOOL(WINAPI *fnVirtualProtect)(_In_ LPVOID lpAddress, _In_ SIZE_T dwSize, _In_ DWORD flNewProtect, _Out_ PDWORD lpflOldProtect);
typedef LPVOID(WINAPI *fnVirtualAlloc)(_In_opt_ LPVOID lpAddress, _In_ SIZE_T dwSize, _In_ DWORD flAllocationType, _In_ DWORD flProtect);
typedef void(WINAPI *fnExitProcess)(_In_ UINT uExitCode);
typedef int(WINAPI *fnMessageBox)(HWND hWnd, LPSTR lpText, LPSTR lpCaption, UINT uType);
