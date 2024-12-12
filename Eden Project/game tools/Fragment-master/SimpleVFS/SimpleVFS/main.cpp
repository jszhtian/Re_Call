// SimpleVFS.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include "pch.h"
#include <iostream>
#include "SimpleVFS.h"

struct KeyFile
{
	char EncKey[CryptoPP::Salsa20::MAX_KEYLENGTH];
	char salt[CryptoPP::Salsa20::IV_LENGTH];
};
using namespace std;
int main()
{
	std::cout << "Hello World!\n";
	SimpleVFS VFS;
	fstream fKey;
	KeyFile KeyInfo;
	fKey.open("pack.key", ios::binary | std::fstream::in);
	fKey.read((char*)&KeyInfo, sizeof(KeyFile));
	VFS.SetKey(KeyInfo.EncKey, KeyInfo.salt);
	VFS.Mount("pack.idx", "pack.pck");
	DWORD FileLen = 0;
	char* buf=VFS.GetFileBufferByName("TEST.txt", &FileLen);
	char* txt1 = new char[FileLen + 1];
	memset(txt1, 0, FileLen + 1);
	memcpy(txt1, buf, FileLen);
	delete[] buf;
	cout << "Len" << FileLen << endl << "Content" << txt1 << endl;
	HANDLE TMPHd = VFS.GetFileHandleByName("AACF.txt");
	char* buf2 = new char[0x10000];
	DWORD ToRead = 0;
	ReadFile(TMPHd, buf2, 0x10000, &ToRead, NULL);
	CloseHandle(TMPHd);
	char* txt2 = new char[ToRead + 1];
	memset(txt2, 0, ToRead + 1);
	memcpy(txt2, buf2, ToRead);
	cout << "Len" << ToRead << endl << "Content" << txt2 << endl;
	

}

// ���г���: Ctrl + F5 ����� >����ʼִ��(������)���˵�
// ���Գ���: F5 ����� >����ʼ���ԡ��˵�

// ������ʾ: 
//   1. ʹ�ý��������Դ�������������/�����ļ�
//   2. ʹ���Ŷ���Դ�������������ӵ�Դ�������
//   3. ʹ��������ڲ鿴���������������Ϣ
//   4. ʹ�ô����б��ڲ鿴����
//   5. ת������Ŀ��>���������Դ����µĴ����ļ�����ת������Ŀ��>�����������Խ����д����ļ���ӵ���Ŀ
//   6. ��������Ҫ�ٴδ򿪴���Ŀ����ת�����ļ���>���򿪡�>����Ŀ����ѡ�� .sln �ļ�