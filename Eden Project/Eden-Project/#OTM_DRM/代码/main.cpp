#include"pack.h"

//1. 编写加壳器：
//    添加新区段，打开DLL，拷贝指定区段头、区段数据，将OEP设置为壳代码OEP
//2. 编写壳代码：壳代码的最后一步 -> 执行完壳代码后，跳转回原OEP
//3. 编写加壳器：对壳代码进行IAT修复 -> 在壳代码中添加代码，修复壳代码重定位(基址、区段偏移)、
//3. 编写壳代码：加密IAT -> 接管IAT，自己修复IAT，加密IAT
//4. 加密程序：加密、压缩被加壳指定的区段
//5. 在 dll 中编写解压、解密程序和解密IAT的码

int main() {
	Pack PackObj;
	PackObj.SetDllSecName(".text");			
	PackObj.OpenPeFile("testa.exe");			//打开被加密程序
	PackObj.OpenDllFile("DllMain.dll");			//打开壳代码dll
	PackObj.CopySection(".pack");				//拷贝区段头
	PackObj.SetOep();							//设置OEP为壳代码OEP
	PackObj.Encryption(".text");				//对被加壳程序的text段加密
	PackObj.Compression();						//压缩代码段
	PackObj.SetIatTo0();						//令壳代码接管IAT表
	PackObj.SetTls();							//备份TLS后清空TLS
	PackObj.FixReloc();							//对Dll模块重定位	
	PackObj.SetReloc();							//备份并替换重定位表
	PackObj.CopySecData(".pack");				//拷贝区段数据
	PackObj.WriteToFile("Test_packa.exe");		//将加壳后的程序写入磁盘
	return 0;
}

