#include<Windows.h>
#include<stdio.h>
#include"Share.h"
#include"aplib.h"
#pragma comment(lib, "../aplib/aplib.lib")
class Pack
{
private:
	DWORD PeSize = 0;
	DWORD PeBase = 0;
	DWORD DllBase = 0;
	CHAR DllSecName[8] = { 0 };
	//��������StartOfsset��ַ
	DWORD StartOfsset = 0;
	//�ӿ�����Ǵ���֮��ͨ�ŵĽṹ��
	SHARE* pShareData = {0};
public:
	//�򿪡���ȡ���ж��ļ�
	VOID OpenPeFile(LPCSTR Path);

	//��Ӳ�����һ������ͷ
	VOID CopySection(LPCSTR DestName);

	//������������
	VOID Pack::CopySecData(CHAR* DestName);

	//����OEP
	VOID Pack::SetOep();

	//��������
	VOID Pack::Encryption(CHAR* DestName);

	//��Ǵ���ӹ�IAT��
	VOID SetIatTo0();

	//���ݲ��滻�ض�λ��
	VOID Pack::SetReloc();

	//���һ��������
	VOID Pack::AddSection(LPCSTR Name, DWORD Size);

	//��ȡָ�����ε��׵�ַ
	PIMAGE_SECTION_HEADER GetSecHeadAddr(DWORD ModuleBase, LPCSTR Name);
	//���ݴ�С�Լ��������Ȼ�ȡ����ֵ
	DWORD AlignmentToSize(DWORD Size, DWORD Algmt);
	//�����ļ�д�����
	VOID WriteToFile(LPCSTR Path);
	//�� �Ǵ����DLL �ļ�
	VOID OpenDllFile(LPCSTR Path);

	//DLL�кϲ���������
	VOID SetDllSecName(CHAR* SecName);

	//����TLS�����TLS
	VOID SetTls();

	//ѹ��
	VOID Pack::Compression();


	PIMAGE_DOS_HEADER pDosHead(DWORD Base);
	PIMAGE_NT_HEADERS pNtHead(DWORD Base);
	PIMAGE_FILE_HEADER pFileHead(DWORD Base);
	PIMAGE_OPTIONAL_HEADER pOptionHead(DWORD Base);
	VOID print(CHAR*);



	// �ԿǴ�������ض�λ
	VOID Pack::FixReloc();
};

