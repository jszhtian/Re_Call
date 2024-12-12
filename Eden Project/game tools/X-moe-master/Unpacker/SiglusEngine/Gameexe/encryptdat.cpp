
#include <stdafx.h> 

inline char *GetComplierDate()
{
PIMAGE_DOS_HEADER pDH;
_asm{
mov eax,fs:[0x30]
mov eax,dword ptr[eax+8]
mov pDH,eax
}
// PIMAGE_DOS_HEADER pDH = (PIMAGE_DOS_HEADER)GetModuleHandle(NULL);
PIMAGE_NT_HEADERS32 pNtH = (PIMAGE_NT_HEADERS32)((DWORD)pDH+pDH->e_lfanew);
return ctime((long *)&pNtH->FileHeader.TimeDateStamp);
}
int main(int argc, char* argv[])
{
#ifdef _DEBUG
FILE *fin=fopen("Gameexe_ori.txt","rb");
#else
printf("SiglusEngine config file encoder\nAuthor:Azure[LCG]\nCompiled on:%s\n",GetComplierDate());
if (argc!=3)
{
printf("Useage: encryptdat.exe <Source TXT config> <Output DAT file>\n");
exit(1);
}
FILE *fin=fopen(argv[1],"rb");
#endif // _DEBUG
if (!fin)
{
printf("Error opening file.\n");
exit(1);
}
fseek(fin,0,SEEK_END);
DWORD dwFileSize=ftell(fin);
rewind(fin);
fseek(fin,2,SEEK_SET);
static char *pFile=new char [dwFileSize];
memset(pFile,0,dwFileSize);
fread(pFile,dwFileSize,1,fin);
fclose(fin);
Compress<CInfoRealLive, Container::RLDataContainer> *cmp = new Compress<CInfoRealLive, Container::RLDataContainer>;
cmp->WriteData((char*) pFile, dwFileSize);
cmp->WriteDataEnd();
printf("Encoding...\nPlease wait...\n");
cmp->Deflate();
cmp->Flush();
delete [] pFile;
DWORD dwNewFileSize=cmp->Length()+8;
char *pBuffer=new char [dwNewFileSize];
memset(pBuffer,0,dwNewFileSize);
memmove(pBuffer + 8, cmp->Data(), cmp->Length());
delete cmp;
*(PDWORD)(pBuffer)=dwNewFileSize;//encoded data size
*(PDWORD)(pBuffer+4)=dwFileSize-2;//decoded data size
char *pBuf=pBuffer;
DWORD i = 0; while (i < dwNewFileSize) *pBuf++ ^= xorTable[i++%256];
#ifdef _DEBUG
FILE *fout=fopen("Gameexe.dat","wb");
#else
FILE *fout=fopen(argv[2],"wb");
#endif // _DEBUG
if (!fin)
{
printf("Error opening file.\n");
exit(1);
}
fseek(fout,2*sizeof(DWORD),SEEK_SET);
fwrite(pBuffer,dwNewFileSize,1,fout);
printf("Encode success!\nNew file size: %d",dwNewFileSize);
delete [] pBuffer;
fclose(fout);
return 0;
}