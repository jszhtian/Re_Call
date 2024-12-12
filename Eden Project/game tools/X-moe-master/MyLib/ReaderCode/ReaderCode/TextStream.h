#pragma once

#include "TextStreamBase.h"
#include "CharSet.h"
#include "FileStreamAdapter.h"

class TextReadStream : public iTextReadStream
{
	IStream * Stream;
	WCHAR *Buffer;
	ULONG BufferLen;

public:
	TextReadStream(const wstring& name, const ULONG encoding):
		Stream(nullptr),
		Buffer(nullptr),
		BufferLen(0)
	{
		Stream = NULL;
		Buffer = NULL;

		Stream = CreateIStream(name, BS_READ);
		if (!Stream)
		{
			throw wstring(L"Cannot open file");
		}
		ULONG nRead = 0;

		try
		{
			STATSTG stat;
			BufferLen = Stream->Stat(&stat, STATFLAG_DEFAULT);

			if (stat.cbSize.QuadPart == 0)
			{
				throw wstring(L"Null File");
			}

			unsigned char mark[2] = { 0, 0 };
			Stream->Read(mark, 2, &nRead);
			if (stat.cbSize.QuadPart > 2 && nRead == 2)
			{
				if (mark[0] == 0xff && mark[1] == 0xfe)
				{
					// unicode
					if (stat.cbSize.QuadPart % 2)
					{
						throw wstring(L"corrupted unicode stream");
					}
					else
					{
						Buffer = new WCHAR[stat.cbSize.QuadPart];
						if (!Buffer)
						{
							throw wstring(L"not enough memory");
						}
						BufferLen = stat.cbSize.LowPart;
						Stream->Read(Buffer, (BufferLen - 1) * 2, &nRead);
						Buffer[BufferLen] = NULL;
					}
				}
				//UTF-8 or ANSI
				else
				{
					unsigned char mark2[1] = { 0 };
					Stream->Read(mark2, 1, &nRead);
					if (mark[0] == 0xef && mark[1] == 0xbb && mark2[0] == 0xbf)
					{
						ULONG size = (ULONG)(stat.cbSize.LowPart - 3);
						unsigned char *nbuf = new unsigned char[size + 1];
						try
						{
							Stream->Read(nbuf, size, &nRead);
							nbuf[size] = 0; // terminater
							BufferLen = Utf8ToWideCharString((const char*)nbuf, NULL);
							if (BufferLen == (size_t)-1)
							{
								MessageBoxW(NULL, L"Cannot Convert utf8 to unicode", NULL, MB_OK);
								throw;
							}
							Buffer = new WCHAR[BufferLen + 1];
							Utf8ToWideCharString((const char*)nbuf, Buffer);
						}
						catch (...)
						{
							delete[] nbuf;
							throw;
						}
						delete[] nbuf;
						Buffer[BufferLen] = 0;
					}
					//ANSI
					else
					{
						// ansi/mbcs
						// read whole and hold it
						LARGE_INTEGER Offset;
						ULARGE_INTEGER AlterOffset;
						Offset.QuadPart = AlterOffset.QuadPart = 0;

						Stream->Seek(Offset, SEEK_SET, &AlterOffset);
						ULONG size = stat.cbSize.LowPart;
						unsigned char *nbuf = new unsigned char[size + 1];
						try
						{
							Stream->Read(nbuf, size, &nRead);
							nbuf[size] = 0; // terminater
							if (encoding == CP_UTF8) 
							{
								BufferLen = Utf8ToWideCharString((const char*)nbuf, NULL);
								if (BufferLen == (size_t)-1)
								{
									MessageBoxW(NULL, L"Cannot Convert narrow to wide", NULL, MB_OK);
									throw;
								}
								Buffer = new WCHAR[BufferLen + 1];
								Utf8ToWideCharString((const char*)nbuf, Buffer);
							}
							else
							{
								BufferLen = Narrowtowidelen((char*)nbuf, encoding);
								if (BufferLen == (size_t)-1)
								{
									MessageBoxW(NULL, L"Cannot Convert narrow to wide", NULL, MB_OK);
									throw;
								}
								Buffer = new WCHAR[BufferLen + 1];
								Narrowtowide(Buffer, (char*)nbuf, BufferLen, encoding);
							}
						}
						catch (...)
						{
							delete[] nbuf;
							throw;
						}
						delete[] nbuf;
						Buffer[BufferLen] = 0;
					}
				}
			}
			//force ANSI
			else
			{		
				// ansi/mbcs
				// read whole and hold it
				LARGE_INTEGER Offset;
				ULARGE_INTEGER AlterOffset;
				Offset.QuadPart = AlterOffset.QuadPart = 0;

				Stream->Seek(Offset, SEEK_SET, &AlterOffset);
				ULONG size = stat.cbSize.LowPart;
				unsigned char *nbuf = new unsigned char[size + 1];
				try
				{
					Stream->Read(nbuf, size, &nRead);
					nbuf[size] = 0; // terminater
					if (encoding == CP_UTF8)
					{
						BufferLen = Utf8ToWideCharString((const char*)nbuf, NULL);
						if (BufferLen == (size_t)-1)
						{
							MessageBoxW(NULL, L"Cannot Convert narrow to wide", NULL, MB_OK);
							throw;
						}
						Buffer = new WCHAR[BufferLen + 1];
						Utf8ToWideCharString((const char*)nbuf, Buffer);
					}
					else
					{
						BufferLen = Narrowtowidelen((char*)nbuf, encoding);
						if (BufferLen == (size_t)-1)
						{
							MessageBoxW(NULL, L"Cannot Convert narrow to wide", NULL, MB_OK);
							throw;
						}
						Buffer = new WCHAR[BufferLen + 1];
						Narrowtowide(Buffer, (char*)nbuf, BufferLen, encoding);
					}
				}
				catch (...)
				{
					delete[] nbuf;
					throw;
				}
				delete[] nbuf;
				Buffer[BufferLen] = 0;
			}
		}
		catch (...)
		{
			delete Stream; Stream = NULL;
			throw;
		}
	}


	~TextReadStream()
	{
		if (Stream) delete Stream;
		if (Buffer) delete[] Buffer;
	}

	ULONG NTAPI Read(wstring & targ, ULONG size)
	{
		if (size == 0)
			size = BufferLen;

		if (size)
		{
			try
			{
				targ = Buffer;
			}
			catch (...)
			{
				targ.clear();
				return 0;
			}
		}
		else
		{
			targ.clear();
		}
		return size;
	}

	void NTAPI Destruct() 
	{ 
		delete this; 
	}

};


iTextReadStream * CreateTextStreamForRead(const wstring & name, const ULONG encoding = CP_ACP)
{
	iTextReadStream* Result = nullptr;
	try
	{
		Result = new TextReadStream(name, encoding);
	}
	catch (...)
	{
		return nullptr;
	}
	return Result;
}
