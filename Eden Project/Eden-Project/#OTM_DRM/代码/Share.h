#pragma once
struct SHARE {
	int DestOEP;
	int ImpTableVA;
	int dwRelocRva;
	int dwRelocSize;
	int OldImageBase;
	int TlsVirtualAddress;
	int TlsCallBackTableVa;
	int TlsDataSize;



	int UncompressAddr;
	int TextCompressSize;
	int TextRVA;
	int SizeOfRawData;
};
