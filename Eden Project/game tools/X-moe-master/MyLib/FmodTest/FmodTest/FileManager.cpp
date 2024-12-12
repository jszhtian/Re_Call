#include "FileManager.h"

#include "stdafx.h"

unsigned int JSHash (char *str)
{
    unsigned int hash = 1315423911;
    while (*str)
    {
		hash ^= ((hash << 5) + (*str++) + (hash >> 2));
	}
 
    return (hash & 0x7FFFFFFF);
}

//arcfile name hash      arcfile info 
static map<unsigned int, map<unsigned int, FileAtom>> FileInfo;
map<unsigned int, map<unsigned int, FileAtom>>::iterator map_it;

bool ARCinfo::read(FILE *ARCINFO, unsigned int archash)
{
	if(!ARCINFO || !archash)
		return false;
	
	binHeader hdr;
	
	pair<unsigned int, map<unsigned int, FileAtom>> arctmp;

	#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	fread(&hdr, 1, sizeof(hdr), ARCINFO);

	binEntry* entries = new binEntry[hdr.FileCount];
	fread(entries, 1, sizeof(binEntry) * hdr.FileCount, ARCINFO);

	unsigned int filenames_len  = hdr.FileNameLength;
	unsigned char* filenames_buff = new unsigned char[filenames_len];
	fread(filenames_buff, 1, filenames_len, ARCINFO);
	
	map<unsigned int, FileAtom> single_map;
	for (unsigned int i = 0; i < hdr.FileCount; i++)
	{
		FileAtom atom;
		
		atom.Offset   = entries[i].Offset;
		atom.Length   = entries[i].Length;
		
		pair<unsigned int, FileAtom> FilePair;
		FilePair.first = JSHash((char*)filenames_buff + entries[i].FileNameOffset);
		FilePair.second= atom;
		single_map.insert(FilePair);
	}
	
	pair<unsigned int, map<unsigned int, FileAtom>> rhs;
	rhs.first = archash;
	rhs.second= single_map;
	FileInfo.insert(rhs);
	#else //power pc
	//these bin files was created on Windows OS(LE ByteOrder)
	
	fread(&hdr, 1, sizeof(hdr), handle);

	binEntry* entries = new binEntry[SDL_Swap32(hdr.FileCount)];
	fread(entries, 1, sizeof(binEntry) * SDL_Swap32(hdr.FileCount), ARCINFO);

	unsigned int filenames_len  = SDL_Swap32(hdr.FileNameLength);
	unsigned char* filenames_buff = new unsigned char[filenames_buff];
	fread(filenames_buff, 1, filenames_len, ARCINFO);
	
	map<unsigned int, FileAtom> single_map;
	for (unsigned int i = 0; i < SDL_Swap32(hdr.FileCount); i++)
	{
		FileAtom atom;
		
		atom.Offset   = SDL_Swap32(entries[i].Offset);
		atom.Length   = SDL_Swap32(entries[i].Length);
		
		pair<unsigned int, FileAtom> FilePair;
		FilePair.first = JSHash(filenames_buff + SDL_Swap32(entries[i].FileNameOffset));
		FilePair.second= atom;
		single_map.insert(FilePair);
	}
	
	pair<unsigned int, map<unsigned int, FileAtom>> rhs;
	rhs.first = archash;
	rhs.second= single_map;
	FileInfo.insert(rhs);
	#endif
	delete[] entries;
	delete[] filenames_buff;
	
	return true;
}

char* ARCinfo::getFile(const char *filename, unsigned int &size)
{
	if(!filename)
		return 0;

	FILE *debug3 = fopen("debug3.txt", "a");
	fprintf(debug3, "getFile Info[%s, %x]\n",filename, size);
	fclose(debug3);
	
	char ArcFile[512]; char TargetFile[512];
	memset(ArcFile, 0, sizeof(ArcFile));
	memset(TargetFile,0, sizeof(TargetFile));
	int i;
	for(i = 0; i < strlen(filename); i++)
	{
		if(filename[i] == '/') break;
	}
	
	memcpy(ArcFile, filename, i);
	memcpy(TargetFile, filename+i+1, strlen(filename)-i);
	unsigned int hash = JSHash(ArcFile);
	
	debug3 = fopen("debug3.txt", "a");
	fprintf(debug3, "getFile Proc[ARC = %s, Target = %s]\n",ArcFile, TargetFile);
	fclose(debug3);

	char RealArcName[256];
	memset(RealArcName, 0, sizeof(RealArcName));
	strcpy(RealArcName, ArcFile);
	strcat(RealArcName, ".bin");

	if(!isLoaded(hash))
	{
		FILE *fin_arc = fopen(RealArcName, "rb");
		if(fin_arc == NULL)
		{
			fclose(fin_arc);
			size = 0;
			return 0;
		}
		bool ret = read(fin_arc, hash);
		if(!ret)
		{
			size = 0;
			fclose(fin_arc);
			return 0;
		}
		fclose(fin_arc);
	}

	map_it = FileInfo.find(hash);
	
	map<unsigned int ,FileAtom>::iterator file_it;
	char *buffer = 0;
	//ARC name hash 
	if(map_it == FileInfo.end())
		return 0;
	else
	{
		file_it = map_it->second.find(JSHash(TargetFile));
		if(file_it == map_it->second.end())
			return 0;
			
		FILE *fin = fopen(RealArcName, "rb");
		if(!fin)
		{
			fclose(fin);
			return 0;
		}

		debug3 = fopen("debug3.txt", "a");
		fprintf(debug3, "getFile Offset[%08x]\n", file_it->second.Offset);
		fclose(debug3);
		
		#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		fseek(fin,file_it->second.Offset,SEEK_SET );
		#else
		fseek(fin,SDL_Swap32(file_it->second.Offset),SEEK_SET );
		#endif
		
		#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		buffer = new char[file_it->second.Length];
		#else
		Buffer = new char[SDL_Swap32(file_it->second.Length)];
		#endif
		
		if(!buffer)
		{
			delete[] buffer;
			return 0;
		}
		
		#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		fread(buffer, 1, file_it->second.Length, fin);
		#else
		fread(buffer, 1, SDL_Swap32(file_it->second.Length), fin);
		#endif
	}
	
	#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	size = file_it->second.Length;
	#else
	size = SDL_Swap32(file_it->second.Length);
	#endif

	debug3 = fopen("debug3.txt", "a");
	fprintf(debug3, "getFile Done[%x]\n",size);
	fclose(debug3);

	return buffer;
}

bool ARCinfo::deleteFile(void *buffer)
{
	if(buffer)
	{
		delete[] buffer;
		return true;
	}
	return false;
}

bool ARCinfo::isLoaded(unsigned int hash)
{
	bool ret = false;
	for(map_it = FileInfo.begin(); map_it!=FileInfo.end(); map_it++)
	{
		if(map_it->first == hash)
		{
			ret = true;
			break;
		}
	}
	return ret;
}

char* ARCinfo::Uncompress(char* compressed_mem, unsigned int compressed_size, unsigned int ori_size)
{
	unsigned char* out_buff = new unsigned char[ori_size];
	if(out_buff == 0)
		return 0;
	uLong fixed_size = (uLong) ori_size;
	uncompress(out_buff, &fixed_size, (unsigned char*)compressed_mem, compressed_size);
	return (char*)out_buff;
}
