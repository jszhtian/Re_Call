//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000-2007 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// Text read/write stream
//---------------------------------------------------------------------------
#include "tjsCommHead.h"

#include <zlib/zlib.h>
#include "TextStream.h"
#include "MsgIntf.h"
#include "DebugIntf.h"
#include "EventIntf.h"
#include "UtilStreams.h"
#include "tjsError.h"


/*
	Text stream is used by TJS's Array.save, Dictionary.saveStruct etc.
	to input/output text files.
*/


//---------------------------------------------------------------------------
// Interface to tTJSTextStream
//---------------------------------------------------------------------------
/*
	note: encryption of mode 0 or 1 ( simple crypt ) does never intend data pretection
		  security.
*/
class tTVPTextReadStream : public iTJSTextReadStream
{
	tTJSBinaryStream * Stream;
	bool DirectLoad;
	tjs_char *Buffer;
	size_t BufferLen;
	tjs_char *BufferPtr;
	tjs_int CryptMode;

public:
	tTVPTextReadStream(const ttstr  & name, const ttstr & modestr)
	{
		// following syntax of modestr is currently supported.
		// oN: read from binary offset N (in bytes)

#ifndef TVP_NO_CHECK_WIDE_CHAR_SIZE
		if(sizeof(tjs_char)  != 2)
			TVPThrowExceptionMessage(TJS_W("The host is not a 16-bit unicode system."));
#endif

		Stream = NULL;
		Buffer = NULL;
		DirectLoad = false;
		CryptMode = -1;

		// check o mode
		Stream = TVPCreateStream(name, TJS_BS_READ);

		tjs_uint64 ofs = 0;
		const tjs_char * o_ofs;
		o_ofs = TJS_strchr(modestr.c_str(), TJS_W('o'));
		if(o_ofs != NULL)
		{
			// seek to offset
			o_ofs++;
			tjs_char buf[256];
			int i;
			for(i = 0; i < 255; i++)
			{
				if(o_ofs[i] >= TJS_W('0') && o_ofs[i] <= TJS_W('9'))
					buf[i] = o_ofs[i];
				else break;
			}
			buf[i] = 0;
			ofs = ttstr(buf).AsInteger();
			Stream->SetPosition(ofs);
		}

		// check first of the file - whether the file is unicode
		try
		{
			tjs_uint8 mark[2] = {0,0};
			Stream->Read(mark, 2);
			if(mark[0] == 0xff && mark[1] == 0xfe)
			{
				// unicode
				DirectLoad = true;
			}
			else if(mark[0] == 0xfe && mark[1] == 0xfe)
			{
				// ciphered text or compressed
				tjs_uint8 mode[1];
				Stream->Read(mode, 1);
				if(mode[0] != 0 && mode[0] != 1 && mode[0] != 2)
					TVPThrowExceptionMessage(TVPUnsupportedCipherMode, name);
					// currently only mode0 and mode1, and compressed (mode2) are supported
				CryptMode = mode[0];
				DirectLoad = CryptMode != 2;

				Stream->Read(mark, 2); // original bom code comes here (is not compressed)
				if(mark[0] != 0xff || mark[1] != 0xfe)
					TVPThrowExceptionMessage(TVPUnsupportedCipherMode, name);


				if(CryptMode == 2)
				{
					// compressed text stream
					tjs_uint64 compressed = Stream->ReadI64LE();
					tjs_uint64 uncompressed = Stream->ReadI64LE();
					if(compressed != (unsigned long)compressed ||
						uncompressed != (unsigned long)uncompressed)
						TVPThrowExceptionMessage(TVPUnsupportedCipherMode, name);
							// too large stream
					unsigned long destlen ;
					tjs_uint8 *nbuf = new tjs_uint8[(unsigned long)compressed + 1];
					try
					{
						Stream->ReadBuffer(nbuf, (unsigned long)compressed);
						Buffer = new tjs_char [ (BufferLen = (destlen =
								(unsigned long)uncompressed) / 2) + 1];
						int result = uncompress( /* uncompress from zlib */
							(unsigned char*)Buffer,
							&destlen, (unsigned char*)nbuf,
							(unsigned long)compressed);
						if(result != Z_OK ||
							destlen != (unsigned long)uncompressed)
								TVPThrowExceptionMessage(TVPUnsupportedCipherMode, name);
							// uncompression failed.
					}
					catch(...)
					{
						delete [] nbuf;
						throw;
					}
					delete [] nbuf;
					Buffer[BufferLen] = 0;
					BufferPtr = Buffer;
				}
			}
			else
			{
				// ansi/mbcs
				// read whole and hold it
				Stream->SetPosition(ofs);
				tjs_uint size = (tjs_uint)(Stream->GetSize());
				tjs_uint8 *nbuf = new tjs_uint8[size + 1];
				try
				{
					Stream->ReadBuffer(nbuf, size);
					nbuf[size] = 0; // terminater
					BufferLen = TJS_narrowtowidelen((tjs_nchar*)nbuf);
					if(BufferLen == (size_t)-1) TVPThrowExceptionMessage(TJSNarrowToWideConversionError);
					Buffer = new tjs_char [ BufferLen +1];
					TJS_narrowtowide(Buffer, (tjs_nchar*)nbuf, BufferLen);
				}
				catch(...)
				{
					delete [] nbuf;
					throw;
				}
				delete [] nbuf;
				Buffer[BufferLen] = 0;
				BufferPtr = Buffer;
			}
		}
		catch(...)
		{
			delete Stream; Stream = NULL;
			throw;
		}
	}


	~tTVPTextReadStream()
	{
		if(Stream) delete Stream;
		if(Buffer) delete [] Buffer;
	}

	tjs_uint TJS_INTF_METHOD Read(tTJSString & targ, tjs_uint size)
	{
		if(DirectLoad)
		{
			if(sizeof(tjs_char) == 2)
			{
				if(size == 0) size = Stream->GetSize() - Stream->GetPosition();
				if(!size)
				{
					targ.Clear();
					return 0;
				}
				tjs_char *buf = targ.AllocBuffer(size);
				tjs_uint read = Stream->Read(buf, size * 2); // 2 = BMP unicode size
				read /= 2;
#if TJS_HOST_IS_BIG_ENDIAN
				// re-order input
				for(tjs_uint i = 0; i<read; i++)
				{
					tjs_char ch = buf[i];
					buf[i] = ((ch >> 8) & 0xff) + ((ch & 0xff) << 8);
				}
#endif
				if(CryptMode == 0)
				{
					// simple crypt
					for(tjs_uint i = 0; i<read; i++)
					{
						tjs_char ch = buf[i];
						if(ch >= 0x20) buf[i] = ch ^ (((ch&0xfe) << 8)^1);
					}
				}
				else if(CryptMode == 1)
				{
					// simple crypt
					for(tjs_uint i = 0; i<read; i++)
					{
						tjs_char ch = buf[i];
						ch = ((ch & 0xaaaaaaaa)>>1) | ((ch & 0x55555555)<<1);
						buf[i] = ch;
					}
				}
				buf[read] = 0;
				targ.FixLen();
				return read;
			}
			else
			{
				// sizeof(tjs_char) is 4
				// FIXME: NOT TESTED CODE
				if(size == 0) size = Stream->GetSize() - Stream->GetPosition();
				tjs_uint16 *buf = new tjs_uint16[size / 2];
				tjs_uint read;
				try
				{
					read = Stream->Read(buf, size * 2); // 2 = BMP unicode size
					read /= 2;
#if TJS_HOST_IS_BIG_ENDIAN
					// re-order input
					for(tjs_uint i = 0; i<read; i++)
					{
						tjs_char ch = buf[i];
						buf[i] = ((ch >> 8) & 0xff) + ((ch & 0xff) << 8);
					}
#endif
					if(CryptMode == 0)
					{
						// simple crypt (buggy version)
						for(tjs_uint i = 0; i<read; i++)
						{
							tjs_char ch = buf[i];
							if(ch >= 0x20) buf[i] = ch ^ (((ch&0xfe) << 8)^1);
						}
					}
					else if(CryptMode == 1)
					{
						// simple crypt
						for(tjs_uint i = 0; i<read; i++)
						{
							tjs_char ch = buf[i];
							ch = ((ch & 0xaaaaaaaa)>>1) | ((ch & 0x55555555)<<1);
							buf[i] = ch;
						}
					}
					buf[read] = 0;
				}
				catch(...)
				{
					delete [] buf;
					throw;
				}
				targ = TVPStringFromBMPUnicode(buf);
				delete [] buf;
				return read;
			}
		}
		else
		{
			if(size == 0) size = BufferLen;
			if(size)
			{
				tjs_char *buf = targ.AllocBuffer(size);
				TJS_strncpy(buf, BufferPtr, size);
				buf[size] = 0;
				BufferPtr += size;
				BufferLen -= size;
				targ.FixLen();
			}
			else
			{
				targ.Clear();
			}
			return size;
		}
	}

	void TJS_INTF_METHOD Destruct() { delete this; }

};
//---------------------------------------------------------------------------
class tTVPTextWriteStream : public iTJSTextWriteStream
{
	// TODO: 32bit wchar_t support


	tTJSBinaryStream * Stream;
	tjs_int CryptMode;
		// -1 for no-crypt
		// 0: (unused)	(old buggy crypt mode)
		// 1: simple crypt
		// 2: complessed
    int CompressionLevel; // compression level of zlib

	tTVPMemoryStream * DataBuf; // uncompressed data buffer

public:
	tTVPTextWriteStream(const ttstr & name, const ttstr &modestr)
	{
		// modestr supports following modes:
		// dN: deflate(compress) at mode N ( currently not implemented )
		// cN: write in cipher at mode N ( currently n is ignored )
		// zN: write with compress at mode N ( N is compression level )
		// oN: write from binary offset N (in bytes)
		Stream = NULL;
		CryptMode = -1;
		DataBuf = NULL;
		CompressionLevel = Z_DEFAULT_COMPRESSION;


		// check c/z mode
		const tjs_char *p;
		if((p = TJS_strchr(modestr.c_str(), TJS_W('c'))) != NULL)
		{
			CryptMode = 1; // simple crypt
			if(p[1] >= TJS_W('0') && p[1] <= TJS_W('9'))
				CryptMode = p[1] - TJS_W('0');
		}

		if((p = TJS_strchr(modestr.c_str(), TJS_W('z'))) != NULL)
		{
			CryptMode = 2; // compressed (cannot be with 'c')
			if(p[1] >= TJS_W('0') && p[1] <= TJS_W('9'))
				CompressionLevel = p[1] - TJS_W('0');
		}

		if(CryptMode != -1 && CryptMode != 1 && CryptMode != 2)
			TVPThrowExceptionMessage(TVPUnsupportedModeString,
				TJS_W("unsupported cipher mode"));

		// check o mode
		const tjs_char * o_ofs;
		o_ofs = TJS_strchr(modestr.c_str(), TJS_W('o'));
		if(o_ofs != NULL)
		{
			// seek to offset
			o_ofs++;
			tjs_char buf[256];
			int i;
			for(i = 0; i < 255; i++)
			{
				if(o_ofs[i] >= TJS_W('0') && o_ofs[i] <= TJS_W('9'))
					buf[i] = o_ofs[i];
				else break;
			}
			buf[i] = 0;
			tjs_uint64 ofs = ttstr(buf).AsInteger();
			Stream = TVPCreateStream(name, TJS_BS_UPDATE);
			Stream->SetPosition(ofs);
		}
		else
		{
			Stream = TVPCreateStream(name, TJS_BS_WRITE);
		}


		if(CryptMode == 1 || CryptMode == 2)
		{
			// simple crypt or compressed
			tjs_uint8 crypt_mode_sig[4];
			crypt_mode_sig[0] = crypt_mode_sig[1] = 0xfe;
			crypt_mode_sig[2] = (tjs_uint8)CryptMode;
			crypt_mode_sig[3] = 0;
			Stream->WriteBuffer(crypt_mode_sig, 3);
		}

		if(CryptMode == 2)
		{
			// Compression
			DataBuf = new tTVPMemoryStream();
		}

		// now output text stream will write unicode texts
		static tjs_uint8 bommark[4] = { 0xff, 0xfe,
										0x00, 0x00/*dummy 2bytes*/ };
		Stream->WriteBuffer(bommark, 2);


	}

	~tTVPTextWriteStream()
	{
		if(CryptMode == 2)
		{
			// compressed; compress here

			// allocate outout buffer
			unsigned long databuflen = (unsigned long)DataBuf->GetSize();
			unsigned long outbuflen;
			tjs_uint8 *outbuf = new tjs_uint8
				[outbuflen = (databuflen + databuflen / 900 + 23)];
			try
			{
				// compress
				int status = compress2(outbuf, &outbuflen,
					(unsigned char *)DataBuf->GetInternalBuffer(),
					databuflen, CompressionLevel);
				if(status != Z_OK)
					TVPThrowExceptionMessage(TVPCompressionFailed);

				WriteI64LE((tjs_uint64)outbuflen);
				WriteI64LE((tjs_uint64)databuflen);

				Stream->WriteBuffer(outbuf, outbuflen);
			}
			catch(...)
			{
				delete [] outbuf;
				throw;
			}
			delete [] outbuf;
		}

		if(Stream) delete Stream;
		if(DataBuf) delete DataBuf;;
	}

	void WriteI64LE(tjs_uint64 v)
	{
		// write 64bit little endian value to the file.
		tjs_uint8 buf[8];
		buf[0] = (tjs_uint8)(v	>>	(0*8));
		buf[1] = (tjs_uint8)(v	>>	(1*8));
		buf[2] = (tjs_uint8)(v	>>	(2*8));
		buf[3] = (tjs_uint8)(v	>>	(3*8));
		buf[4] = (tjs_uint8)(v	>>	(4*8));
		buf[5] = (tjs_uint8)(v	>>	(5*8));
		buf[6] = (tjs_uint8)(v	>>	(6*8));
		buf[7] = (tjs_uint8)(v	>>	(7*8));

		Stream->WriteBuffer(buf, 8);
	}

	void TJS_INTF_METHOD Write(const ttstr & targ)
	{
		tjs_uint16 *buf;
		tjs_int len = targ.GetLen();
		buf = new tjs_uint16 [len + 1];
		try
		{
			const tjs_char *src = targ.c_str();
			tjs_int i;
			for(i = 0; i < len; i++)
			{
				if(src[i] < 0x10000)
					buf[i] = src[i];
				else
					buf[i] = TJS_W('?');
			}
			buf[i] = 0;

#if TJS_HOST_IS_BIG_ENDIAN
			tjs_uint16 *p;
			if(CryptMode == 1)
			{
				// simple crypt
				p = buf;
				if(p)
				{
					while(*p)
					{
						tjs_char ch = *p;
						ch = ((ch & 0xaaaaaaaa)>>1) | ((ch & 0x55555555)<<1);
						*p = ch;
						p++;
					}
				}
			}

			// re-order to little endian unicode text
			p = buf;
			if(p)
			{
				while(*p)
				{
					*p = ((*p >> 8) & 0xff) + ((*p & 0xff) << 8);
					p++;
				}
			}

			WriteRawData(str.c_str(), len * sizeof(tjs_char));
#else
			if(CryptMode == 1)
			{
				// simple crypt
				tjs_uint16 *p;
				p = buf;
				if(p)
				{
					while(*p)
					{
						tjs_char ch = *p;
						ch = ((ch & 0xaaaaaaaa)>>1) | ((ch & 0x55555555)<<1);
						*p = ch;
						p++;
					}
				}

				WriteRawData(buf, len * sizeof(tjs_uint16));

			}
			else
			{
				WriteRawData(buf, len * sizeof(tjs_uint16));
			}
#endif
		}
		catch(...)
		{
			delete [] buf;
			throw;
		}
		delete [] buf;
	}

	void WriteRawData(void *ptr, size_t size)
	{
		if(CryptMode == 2)
		{
			// compressed
			// once store to the memory
			if(!DataBuf) DataBuf = new tTVPMemoryStream();
			DataBuf->Write(ptr, size);
		}
		else
		{
			Stream->WriteBuffer(ptr, size); // write directly
		}
	}

	void TJS_INTF_METHOD Destruct() { delete this; }
};
//---------------------------------------------------------------------------
iTJSTextReadStream * TVPCreateTextStreamForRead(const ttstr & name,
	const ttstr & modestr)
{
	return new tTVPTextReadStream(name, modestr);
}
//---------------------------------------------------------------------------
iTJSTextWriteStream * TVPCreateTextStreamForWrite(const ttstr & name,
	const ttstr & modestr)
{
	return new tTVPTextWriteStream(name, modestr);
}
//---------------------------------------------------------------------------





