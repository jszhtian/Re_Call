// MNGProc.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
using namespace std;
BYTE MNGMagic[] = { 0x8A,0x4D,0x4E,0x47,0x0D,0x0A,0x1A,0x0A };
BYTE PNGMagic[] = { 0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A };
BYTE IHDRMagic[] = { 0x49,0x48,0x44,0x52 };
BYTE IENDMagic[] = { 0x49,0x45,0x4E,0x44 };
BYTE IDATMagic[] = { 0x49,0x44,0x41,0x54 };

struct MNGChunk {
	BYTE Magic[4];
	BYTE CRC32[4];
	DWORD Length;
	PBYTE Buffer;
};

void ChunkWriter(FILE* file, MNGChunk MNG)
{
	DWORD Length= _byteswap_ulong(MNG.Length);
	fwrite(&Length, 1, 4, file);
	fwrite(MNG.Magic, 1, 4, file);
	fwrite(MNG.Buffer, 1, MNG.Length, file);
	fwrite(MNG.CRC32, 1, 4, file);
}

inline void PNGMagicWriter(FILE* file)
{
	fwrite(PNGMagic, 1, 8, file);
}
inline void MNGMagicWriter(FILE* file)
{
	fwrite(MNGMagic, 1, 8, file);
}
int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, 0);
	if (argc!=3)
	{
		wcout << "Usage: " << argv[0] << " File Option" << endl;
		wcout << "Option:" << endl;
		wcout << "o->Output" << endl;
		wcout << "r->Replace" << endl;
		return -1;
	}
	FILE* file = _wfopen(argv[1], L"rb");
	if (!file)
	{
		wcout << "Open Fail:" << argv[1] << endl;
	}
	PBYTE FileBuffer;
	ULONG FileSize;
	fseek(file, 0, SEEK_END);
	FileSize = ftell(file);
	rewind(file);
	FileBuffer = new BYTE[FileSize];
	fread(FileBuffer, 1, FileSize, file);
	fclose(file);
	BYTE Header[8];
	ULONG BufPtr = 0;
	memcpy(Header, FileBuffer + BufPtr, 8);
	BufPtr += 8;
	if (memcmp(Header,MNGMagic,8)!=0)
	{
		cout << "Process Error: MNG Magic is mismatch!" << endl;
		return -1;
	}
	vector<MNGChunk> ChunkList;
	while (BufPtr<FileSize)
	{
		DWORD ChunkSize = *(PDWORD)(FileBuffer + BufPtr);
		ChunkSize = _byteswap_ulong(ChunkSize);
		BufPtr += 4;
		BYTE Magic[4];
		memcpy(Magic, FileBuffer + BufPtr, 4);
		BufPtr += 4;
		PBYTE Buf = FileBuffer + BufPtr;
		BufPtr += ChunkSize;
		BYTE CRC32[4];
		memcpy(CRC32, FileBuffer + BufPtr, 4);
		BufPtr += 4;
		MNGChunk MNG;
		MNG.Buffer = Buf;
		memcpy(MNG.CRC32, CRC32, 4);
		memcpy(MNG.Magic, Magic, 4);
		MNG.Length = ChunkSize;
		ChunkList.push_back(MNG);
	}
	if (wcscmp(argv[2],L"o")==0)
	{
		TCHAR name[MAX_PATH] = {0};
		wcscpy(name, argv[1]);
		PathStripPath(name);
		PathRemoveExtension(name);
		TCHAR Path[MAX_PATH] = {0};
		wcscpy(Path, argv[1]);
		PathRemoveFileSpec(Path);
		PathAddBackslash(Path);
		TCHAR NewPath[MAX_PATH] = {0};
		wcscpy(NewPath, Path);
		lstrcat(NewPath, name);
		CreateDirectory(NewPath,0);
		PathAddBackslash(NewPath);
		FILE* NewFile;
		int index = 0;
		for (auto Chunk:ChunkList)
		{
			if (memcmp(Chunk.Magic,IHDRMagic,4)==0)
			{
				TCHAR idx[128] = {0};
				_itot(index, idx, 10);
				TCHAR FullPath[MAX_PATH];
				wcscpy(FullPath, NewPath);
				lstrcat(FullPath, idx);
				lstrcat(FullPath, L".png");
				NewFile = _wfopen(FullPath, L"wb");
				PNGMagicWriter(NewFile);
				ChunkWriter(NewFile, Chunk);
				index++;
			}
			if (memcmp(Chunk.Magic, IDATMagic, 4) == 0)
			{
				if (NewFile!=NULL)
				{
					ChunkWriter(NewFile, Chunk);
				}
				else
				{
					MessageBox(0, L"Error!\nNo File Stream!", L"Output", MB_OK);
					return -1;
				}
				
			}
			if (memcmp(Chunk.Magic, IENDMagic, 4) == 0)
			{
				if (NewFile != NULL)
				{
					ChunkWriter(NewFile, Chunk);
					fclose(NewFile);
					NewFile = NULL;
				}
				else
				{
					MessageBox(0, L"Error!\nNo File Stream!", L"Output", MB_OK);
					return -1;
				}

			}
		}
	}
	if (wcscmp(argv[2], L"r") == 0)
	{
		TCHAR name[MAX_PATH] = { 0 };
		wcscpy(name, argv[1]);
		PathStripPath(name);
		PathRemoveExtension(name);
		TCHAR Path[MAX_PATH] = { 0 };
		wcscpy(Path, argv[1]);
		PathRemoveFileSpec(Path);
		PathAddBackslash(Path);
		TCHAR NewPath[MAX_PATH] = { 0 };
		wcscpy(NewPath, Path);
		lstrcat(NewPath, name);
		PathAddBackslash(NewPath);
		FILE* NewFile;
		PBYTE FileBuffer;
		TCHAR newname[MAX_PATH];
		wcscpy(newname, argv[1]);
		lstrcat(newname, L".out");
		NewFile = _wfopen(newname, L"wb");
		if (!NewFile)
		{
			MessageBox(0, L"IO Error", L"Error", MB_OK);
			return -1;
		}
		MNGMagicWriter(NewFile);
		FILE* tarfile;
		bool WriteFinish = false;
		bool canWrite = false;
		int index = 0;
		vector<MNGChunk> RepChunkList;
		for (auto Chunk : ChunkList)
		{
			
			if (memcmp(Chunk.Magic, IHDRMagic, 4) == 0)
			{
				TCHAR idx[128] = { 0 };
				_itot(index, idx, 10);
				TCHAR FullPath[MAX_PATH];
				wcscpy(FullPath, NewPath);
				lstrcat(FullPath, idx);
				lstrcat(FullPath, L".png");
				tarfile = _wfopen(FullPath, L"rb");
				if (!tarfile)
				{
					canWrite = false;
				}
				else
				{
					canWrite = true;
					ULONG FileSize;
					fseek(tarfile, 0, SEEK_END);
					FileSize = ftell(tarfile);
					rewind(tarfile);
					FileBuffer = new BYTE[FileSize];
					fread(FileBuffer, 1, FileSize, tarfile);
					fclose(tarfile);
					ULONG BufPtr = 0;
					BufPtr += 8;
					while (BufPtr < FileSize)
					{
						DWORD ChunkSize = *(PDWORD)(FileBuffer + BufPtr);
						ChunkSize = _byteswap_ulong(ChunkSize);
						BufPtr += 4;
						BYTE Magic[4];
						memcpy(Magic, FileBuffer + BufPtr, 4);
						BufPtr += 4;
						PBYTE Buf = FileBuffer + BufPtr;
						BufPtr += ChunkSize;
						BYTE CRC32[4];
						memcpy(CRC32, FileBuffer + BufPtr, 4);
						BufPtr += 4;
						MNGChunk MNG;
						MNG.Buffer = Buf;
						memcpy(MNG.CRC32, CRC32, 4);
						memcpy(MNG.Magic, Magic, 4);
						MNG.Length = ChunkSize;
						RepChunkList.push_back(MNG);
						if (memcmp(MNG.Magic, IENDMagic, 4) == 0)
						{
							break;
						}
					}
				}
				index++;
				ChunkWriter(NewFile, Chunk);
				continue;
			}
			if (memcmp(Chunk.Magic, IDATMagic, 4) == 0)
			{
				if ((!WriteFinish)&&canWrite)
				{
					for (auto Repchunk:RepChunkList)
					{
						if (memcmp(Repchunk.Magic, IDATMagic, 4) == 0)
						{
							ChunkWriter(NewFile, Repchunk);
						}
					}
					WriteFinish = true;
				}
				if (!canWrite)
				{
					ChunkWriter(NewFile, Chunk);
				}
				if (WriteFinish)
				{
					continue;
				}
				continue;
			}
			if (memcmp(Chunk.Magic, IENDMagic, 4) == 0)
			{
				if (canWrite)
				{
					canWrite = false;
					WriteFinish = false;
					delete []FileBuffer;
					FileBuffer = NULL;
				}
				RepChunkList.clear();
				ChunkWriter(NewFile, Chunk);
				continue;
			}
			ChunkWriter(NewFile, Chunk);
		}
	}
    return 0;
}

