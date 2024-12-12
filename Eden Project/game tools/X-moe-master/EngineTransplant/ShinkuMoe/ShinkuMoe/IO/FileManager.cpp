#include "FileManager.h"
#include "Mutex.h"
#include "Surface.h"
#include <zlib.h>
#include <algorithm>
#include <string>
#include <vector>
#include <map>


static const char* AutoPath[] =
{
	"bgm.bin",
	"graph.bin",
	"graph_bg.bin",
	"graph_vis.bin",
	"se.bin",
	"se_env.bin",
	"se_sys.bin",
	"voice.bin",
	nullptr
};


enum
{
	NVSGHDR_TYPE_SINGLE_24BIT = 0,
	NVSGHDR_TYPE_SINGLE_32BIT = 1,
	NVSGHDR_TYPE_MULTI_32BIT = 2,
	NVSGHDR_TYPE_SINGLE_8BIT = 3,
	NVSGHDR_TYPE_SINGLE_1BIT = 4
};


#define MakeDword(x) ( \
                ((((AnzUInt32)x) >> 24) & 0xFF)       | \
                ((((AnzUInt32)x) >> 8 ) & 0xFF00)     | \
                ((((AnzUInt32)x) << 24) & 0xFF000000) | \
                ((((AnzUInt32)x) << 8 ) & 0x00FF0000) \
                )


AnzUInt32 ELFHash(const char* str)
{
	AnzUInt32 hash = 0;
	AnzUInt32 x = 0;

	for (AnzUInt32 i = 0; i < strlen(str); i++)
	{
		hash = (hash << 4) + str[i];
		if ((x = hash & 0xF0000000L) != 0)
		{
			hash ^= (x >> 24);
		}
		hash &= ~x;
	}
	return hash;
}


struct FileItem
{
	AnzUInt32   Offset;
	AnzUInt32   Size;
	AnzUInt32   Hash;
	std::string FileName;

	friend bool operator < (const FileItem& lhs, const FileItem& rhs)
	{
		if (lhs.Hash == rhs.Hash)
		{
			if (lhs.FileName[0] == 0)
				return true;
			else if (rhs.FileName[0] == 0)
				return false;
			else
				return strcmp(lhs.FileName.c_str(), rhs.FileName.c_str());
		}
		else return lhs.Hash < rhs.Hash;
	}

	FileItem& operator = (const FileItem& rhs)
	{
		Offset   = rhs.Offset;
		Size     = rhs.Size;
		Hash     = rhs.Hash;
		FileName = rhs.FileName;

		return *this;
	}
};

class VirtualArc
{
	std::vector<FileItem> FileList;

	FILE* file;
public:

	VirtualArc() :
		file(nullptr)
	{

	}

	~VirtualArc()
	{
		if (file)
		{
			fclose(file);
			file = nullptr;
		}
	}

	bool LoadArcFile(const char* FileName)
	{
		file = fopen(FileName, "rb");

		if (!file)
			return false;

		FavoBinHeader hdr;
		fread(&hdr, 1, sizeof(hdr), file);

		auto Chunks = new BinChunk[hdr.ChunkCount];
		fread(Chunks, 1, sizeof(BinChunk) * hdr.ChunkCount, file);

		auto  NameBuffer = new AnzUInt8[hdr.NameLength];
		fread(NameBuffer, 1, hdr.NameLength, file);

		for (AnzUInt32 i = 0; i < hdr.ChunkCount; i++) 
		{
			FileItem item;
			item.FileName = (char*)(NameBuffer + Chunks[i].NameOffset);
			item.Size     = Chunks[i].Size;
			item.Offset   = Chunks[i].Offset;
			item.Hash     = ELFHash((char*)(NameBuffer + Chunks[i].NameOffset));

			FileList.push_back(item);
		}

		std::sort(FileList.begin(), FileList.end());

		delete[] Chunks;
		delete[] NameBuffer;
		return true;
	}

	Stream* LoadFile(const char* FileName)
	{
		FileItem item;
		item.FileName = FileName;
		item.Hash = ELFHash(FileName);

		auto it = std::find(FileList.begin(), FileList.end(), item);
		if (it == FileList.end())
		{
			return nullptr;
		}
		AnzUInt8* Buffer = new AnzUInt8[it->Size];
		if (!Buffer)
		{
			return nullptr;
		}
		fseek(file, it->Offset, SEEK_SET);
		fread(Buffer, 1, it->Size, file);

		return new Stream(it->Size, Buffer);
	}

	//SDL_Surface*
	//vector<SDL_Surface*>*
	//nullptr
	void* LoadAsSurface(const char* FileName, bool& IsVector)
	{
		IsVector = false;
		
		Stream* stream = LoadFile(FileName);

		if(!stream)
			return nullptr;
		
		return ExtractNVSGImage(stream, IsVector);
	}

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	#define rmask  0xff000000
	#define gmask  0x00ff0000
	#define bmask  0x0000ff00
	#define amask  0x000000ff
#else
	#define rmask  0x000000ff
	#define gmask  0x0000ff00
	#define bmask  0x00ff0000
	#define amask  0xff000000
#endif

private:
	void* ExtractNVSGImage(Stream* stream, bool& IsVector)
	{
		IsVector = false;

		if (stream->Size < 4 ||
			*(AnzUInt32*)(stream->Buffer) != MakeDword('hzc1'))
		{
			return nullptr;
		}

		auto     hzc1Hdr = (HZC1Header*)(stream->Buffer);
		AnzUInt  DataLen = stream->Size - sizeof(HZC1Header);
		auto     DataBuffer = stream->Buffer + sizeof(HZC1Header);

		if (DataLen < 4 || *(AnzUInt32*)DataBuffer != MakeDword('NVSG'))
		{
			return nullptr;
		}

		auto ImgHeader = (NVSGImgHeader*)DataBuffer;
		DataBuffer += hzc1Hdr->HeaderLen;

		AnzUInt depth = 0;

		switch (ImgHeader->BitmapFormat)
		{
		case NVSGHDR_TYPE_SINGLE_24BIT:
			depth = 3;
			break;

		case NVSGHDR_TYPE_SINGLE_32BIT:
		case NVSGHDR_TYPE_MULTI_32BIT:
			depth = 4;
			break;

		case NVSGHDR_TYPE_SINGLE_8BIT:
			depth = 1;
			break;

		case NVSGHDR_TYPE_SINGLE_1BIT:
			depth = 1;
			break;

		default:
			return false;
		}

		uLongf OriSize = hzc1Hdr->OriSize;
		auto   OutBuff = new uint8_t[OriSize];

		if (!OutBuff)
			return  nullptr;

		if (uncompress(OutBuff, &OriSize, DataBuffer, DataLen) != Z_OK)
		{
			delete stream;
			delete OutBuff;
			return nullptr;
		}

		if (ImgHeader->BitmapFormat == NVSGHDR_TYPE_SINGLE_1BIT)
		{
			for (AnzUInt32 i = 0; i < OriSize; i++)
			{
				if (OutBuff[i] == 1)
				{
					OutBuff[i] = 0xFF;
				}
			}
		}

		SDL_Surface* src = nullptr;
		std::vector<SDL_Surface*>* vec = nullptr;

		if (ImgHeader->ImgCount)
		{
			IsVector = true;
			AnzUInt FrameSize = ImgHeader->Width * ImgHeader->Height * depth;

			vec = new std::vector<SDL_Surface*>;

			for (AnzUInt j = 0; j < ImgHeader->ImgCount; j++)
			{
				SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
					OutBuff + (j * FrameSize),
					ImgHeader->Width,
					ImgHeader->Height,
					depth,
					ImgHeader->Width * depth,
					rmask,
					gmask,
					gmask,
					amask);

				vec->push_back(surface);
			}
		}
		else
		{
			src = SDL_CreateRGBSurfaceFrom(
				OutBuff,
				ImgHeader->Width,
				ImgHeader->Height,
				depth,
				ImgHeader->Width * depth,
				rmask,
				gmask,
				gmask,
				amask);
		}

		delete[] OutBuff;

		if (IsVector)
			return vec;
		else
			return src;
	}
};

static class VirtualSystem
{
	std::map<AnzUInt64, VirtualArc*> ArcList;

public:
	void Release()
	{
		for (auto it : ArcList)
		{
			delete it.second;
		}
		ArcList.clear();
	}

	void InitLoad(const char* FileName)
	{
		AnzUInt64 Hash = ShinkuHash(FileName);
		
		auto it = ArcList.find(Hash);
		if (it == ArcList.end())
		{
			VirtualArc* arc = new VirtualArc;
			if (arc->LoadArcFile(FileName))
			{
				ArcList.insert(std::make_pair(Hash, arc));
			}
			else
			{
				delete arc;
			}
		}
	}

	Stream* LoadFile(const char* FileName)
	{
		std::string Name(FileName);
		
		std::string::size_type Pos = Name.find_first_of('/');
		if (Pos == std::string::npos)
		{
			return nullptr;
		}
		else
		{
			std::string ArcName = Name.substr(0, Pos);
			std::string File = Name.substr(Pos + 1, Name.length());
			AnzUInt64 Hash = ShinkuHash(ArcName.c_str());

			auto it = ArcList.find(Hash);
			if (it == ArcList.end())
			{
				VirtualArc* arc = new VirtualArc;
				if (arc->LoadArcFile(ArcName.c_str()))
				{
					ArcList.insert(std::make_pair(Hash, arc));
				}
				else
				{
					delete arc;
					arc = nullptr;
				}

				if (arc != nullptr)
					return arc->LoadFile(File.c_str());
				else
					return nullptr;
			}
			else
			{
				return (it->second)->LoadFile(File.c_str());
			}
		}
	}


	void* LoadFileAsSurface(const char* FileName, bool IsVector)
	{
		IsVector = false;
		std::string Name(FileName);

		std::string::size_type Pos = Name.find_first_of('/');
		if (Pos == std::string::npos)
		{
			return nullptr;
		}
		else
		{
			std::string ArcName = Name.substr(0, Pos);
			std::string File = Name.substr(Pos + 1, Name.length());
			AnzUInt64 Hash = ShinkuHash(ArcName.c_str());

			auto it = ArcList.find(Hash);
			if (it == ArcList.end())
			{
				VirtualArc* arc = new VirtualArc;
				if (arc->LoadArcFile(FileName))
				{
					ArcList.insert(std::make_pair(Hash, arc));
				}
				else
				{
					delete arc;
					arc = nullptr;
				}

				if (arc != nullptr)
					return arc->LoadAsSurface(File.c_str(), IsVector);
				else
					return nullptr;
			}
			else
			{
				return (it->second)->LoadAsSurface(File.c_str(), IsVector);
			}
		}
	}

}VirtualFileSystem;


bool InitFileSystem()
{
	AnzUInt Index = 0;
	while (AutoPath[Index])
	{
		VirtualFileSystem.InitLoad(AutoPath[Index]);
		Index++;
	}
	return true;
}


bool UnInitFileSystem()
{
	AnzCriticalSectionHolder Holder(FileProtector);
	VirtualFileSystem.Release();
	return true;
}


static AnzCriticalSection FileProtector;

Stream* LoadFile(const char* FileName)
{
	AnzCriticalSectionHolder Holder(FileProtector);

	return VirtualFileSystem.LoadFile(FileName);
}


void* LoadFileAsSurface(const char* FileName, bool& IsVector)
{
	AnzCriticalSectionHolder Holder(FileProtector);

	return VirtualFileSystem.LoadFileAsSurface(FileName, IsVector);
}

