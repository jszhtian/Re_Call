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
	//导出函数StartOfsset地址
	DWORD StartOfsset = 0;
	//加壳器与壳代码之间通信的结构体
	SHARE* pShareData = {0};
public:
	//打开、读取、判断文件
	VOID OpenPeFile(LPCSTR Path);

	//添加并拷贝一个区段头
	VOID CopySection(LPCSTR DestName);

	//拷贝区段数据
	VOID Pack::CopySecData(CHAR* DestName);

	//设置OEP
	VOID Pack::SetOep();

	//加密区段
	VOID Pack::Encryption(CHAR* DestName);

	//令壳代码接管IAT表
	VOID SetIatTo0();

	//备份并替换重定位表
	VOID Pack::SetReloc();

	//添加一个新区段
	VOID Pack::AddSection(LPCSTR Name, DWORD Size);

	//获取指定区段的首地址
	PIMAGE_SECTION_HEADER GetSecHeadAddr(DWORD ModuleBase, LPCSTR Name);
	//根据大小以及对齐粒度获取对齐值
	DWORD AlignmentToSize(DWORD Size, DWORD Algmt);
	//将新文件写入磁盘
	VOID WriteToFile(LPCSTR Path);
	//打开 壳代码的DLL 文件
	VOID OpenDllFile(LPCSTR Path);

	//DLL中合并的区段名
	VOID SetDllSecName(CHAR* SecName);

	//备份TLS，清空TLS
	VOID SetTls();

	//压缩
	VOID Pack::Compression();


	PIMAGE_DOS_HEADER pDosHead(DWORD Base);
	PIMAGE_NT_HEADERS pNtHead(DWORD Base);
	PIMAGE_FILE_HEADER pFileHead(DWORD Base);
	PIMAGE_OPTIONAL_HEADER pOptionHead(DWORD Base);
	VOID print(CHAR*);



	// 对壳代码进行重定位
	VOID Pack::FixReloc();
};

