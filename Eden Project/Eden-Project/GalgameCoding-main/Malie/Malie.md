# Malie引擎分析笔记

本篇不讲分析过程，只讲分析结果。

## 一、封包

这个引擎的封包有两种，扩展名是“lib”和“dat”。其中“dat”封包使用“Camellia”算法加密整个文件。此外，两种封包的结构不一样，详细格式可参考[GARbro](https://github.com/morkt/GARbro/blob/master/ArcFormats/Malie/ArcLIB.cs)的代码。

## 二、IO

这个引擎实现了一套简单的通用IO接口，称为`StreamIo`，引擎读写资源文件基本上都是通过这个接口。

在`WinMain`中可以找到一个叫做`StreamIOMan_Setup`的函数，函数里注册了所有IO接口。

```c
void StreamIOMan_Setup()
{
  DbgPrintW(L"StreamIOMan : Setup\n");
  ...
  RegIo_FILE();            // register FILE 从封包中读取资源文件。
  RegIo_FILE_O();          // register FILE_O 以写方式打开文件。
  RegIo_FILE_IO();         // register FILE_IO 以读写方式打开文件。
  RegIo_MEM_O();           // register MEM_O 写内存文件。
  RegIo_MEM_IO();          // register MEM_IO 读写内存文件。
  RegIo_SOCKETIN();        // register SOCKETIN 读套接字。
  RegIo_UTF8_I();          // register UTF8_I 读 UTF-8 文件。
  RegIo_UTF8_O();          // register UTF8_O 写 UTF-8 文件。
  RegIo_XML_I();           // register XML_I 读 XML 文件。
  RegIo_MBC_I();           // register MBC_I 读 ANSI 文件。
  RegIo_MBC_O();           // register MBC_O 写 ANSI 文件。
  RegIo_TC_I();            // register TC_I
  RegIo_WC_I();            // register WC_I
  RegIo_RES_I();           // register RES_I 读PE中的资源。
  RegIo_CFI();             // register CFI 读加密文件。
  RegIo_CFO();             // register CFO 写加密文件。
  RegIo_MLS();             // register MLS
  RegIo_ZLO();             // register ZLO ZLIB 压缩。
  RegIo_ZLI();             // register ZLI ZLIB 解压。
  RegIo_LFILE_I();         // register LFILE_I 以读方式打开文件。
}
```

下面是注册一个`IoProvider`的例子：

```c
void RegIo_FILE_IO()
{
  IO_PROVIDER iop;

  memset(&iop, 0, sizeof(iop));

  iop.pfn_open = FILE_IO_open;
  iop.pfn_close = fclose;
  iop.pfn_write = FILE_IO_write;
  iop.pfn_read = FILE_IO_read;
  iop.pfn_tell = ftell;
  iop.pfn_seek = fseek;

  wcscpy(iop.name, L"FILE_IO");

  IoRegister(&iop);
}
```

其中`IO_PROVIDER`的定义如下：

```c
struct IO_PROVIDER
{
	int (*pfn_getc)(void* fp);
	int (*pfn_putc)(void* fp, int c);
	int (*pfn_read)(void* fp, void* buffer, int size);
	int (*pfn_write)(void* fp, const void* buffer, int size);
	int (*pfn_tell)(void* fp);
	int (*pfn_seek)(void* fp, int offset, int origin);
	void* (*pfn_open)(const wchar_t* name, const wchar_t* iop);
	void (*pfn_close)(void* fp);

	wchar_t name[34];
};
```

想要读写文件时，调用`IoCreateFile`函数，传入`文件名`与`IoProvider`的名字即可。

```c
IoFile fp = IoCreateFile(L"im.png", L"LFILE_I");
IoRead(fp, buf, size);
IoCloseFile(fp);
```

> 可以注册一个自己的`IoProvider`来让引擎读取各种封包。

## 三、脚本

这个引擎脚本被编译成二进制虚拟机代码，文件名通常是`exec.dat`。

脚本分为以下几个区段：

```
.seg "Symbol"          ; 符号定义
.seg "Function"        ; 函数定义
.seg "Label"           ; 标签定义（汇编含义）
.seg "String"          ; 字符串
.seg "Code"            ; 虚拟机代码
.seg "Message Entry"   ; 对话文本数据定义
.seg "Message"         ; 对话文本数据
```

这个虚拟机比较简单，就是标准的可变长指令。

比较有意思的是，游戏中对话的文本放在了一个单独的区段，对话文本中还包含了一些控制指令，用于播放语音等等。

详细的脚本格式可以参考我写的工具[MalieScriptTool](https://github.com/crskycode/Malie_Script_Tool)。

## 四、汉化

这个引擎的实现是基于`Unicode`的，脚本文件中存储的也是`Unicode`字符串，所以不需要处理编码相关的问题，重新构建脚本的`String`和`Message`区段即可。

另外，字体渲染是使用`FreeType`库实现的，所以需要把字体文件替换掉。

最后，如果不想构建封包，可以Hook`IoCreateFile`函数来改掉`IoProvider`参数，让引擎从文件夹中读取资源文件。

