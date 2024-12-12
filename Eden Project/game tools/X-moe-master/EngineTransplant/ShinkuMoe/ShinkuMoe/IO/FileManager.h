#pragma once
#include "Anz.h"

inline AnzUInt64 ShinkuHash(const void* str)
{
	return (*(AnzUInt64*)str) & 0xDFDFDFDFDFDFDFDF;
}


struct FavoBinHeader 
{
	AnzUInt32 ChunkCount;
	AnzUInt32 NameLength;
};

struct BinChunk
{
	AnzUInt32 NameOffset;
	AnzUInt32 Offset;
	AnzUInt32 Size;
};

struct HZC1Header
{
	union
	{
		AnzUInt8   Sign[4];  // "hzc1"
		AnzUInt32  Dword;
	}Magic;
	AnzUInt32      OriSize;
	AnzUInt32      HeaderLen;
};

struct NVSGImgHeader
{
	union
	{
		AnzUInt8  Sign[4]; // "NVSG"
		AnzUInt32 DWord;
	}Magic;
	AnzUInt16 Unknown1;
	AnzUInt16 BitmapFormat;
	AnzUInt16 Width;
	AnzUInt16 Height;
	AnzUInt16 InitPosX;
	AnzUInt16 InitPosY;
	AnzUInt32 Unknown2;
	AnzUInt32 ImgCount;
	AnzUInt32 Unknown3;
	AnzUInt32 Unknown4;
};

class Stream
{
public:
	AnzUInt   Size;
	AnzUInt8* Buffer;
	AnzUInt   Refs;

private:

	void AddRefer()
	{
		Refs++;
	}

	void Release()
	{
		if (Refs - 1 <= 0)
		{
			delete[] Buffer;
			Size = 0;
			Buffer = nullptr;
		}
		else
		{
			Refs--;
		}
	}

public:
	Stream(AnzUInt FileSize, AnzUInt8* FileBuffer):
		Size(FileSize),
		Buffer(FileBuffer),
		Refs(1)
	{
	}

	~Stream()
	{
		Release();
	}

	void Copy(Stream** stream)
	{
		delete (*stream);
		this->AddRefer();
		(*stream)->Buffer = Buffer;
		(*stream)->Refs   = Refs;
		(*stream)->Size   = Size;
	}

	AnzUInt GetSize()
	{
		return Size;
	}
};

bool         InitFileSystem();
bool         UnInitFileSystem();
Stream*      LoadFile(const char* FileName);

//1.nullptr
//2.SDL_Surface
//3.vector<SDL_Surface*>*
void*        LoadFileAsSurface(const char* FileName, bool& IsVector);


