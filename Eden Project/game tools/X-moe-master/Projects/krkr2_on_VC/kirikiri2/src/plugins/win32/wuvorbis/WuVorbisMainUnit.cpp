//---------------------------------------------------------------------------
// Ogg Vorbis plugin for TSS ( stands for TVP Sound System )
// This is released under TVP(Kirikiri)'s license.
// See details for TVP source distribution.
//---------------------------------------------------------------------------

extern "C" {
#include "vorbis/vorbisfile.h"
}

#include <windows.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#ifndef NOT_HAVE_TP_STUB
#include "tp_stub.h"
#endif
#pragma hdrstop

#include "tvpsnd.h" // TSS sound system interface definitions

static bool FloatExtraction = false; // true if output format is IEEE 32-bit float

#ifndef NOT_HAVE_TP_STUB
static bool TVPAlive = false; // true if the DLL is called from TVP
#endif

#define WARN_OLD_VORBIS_DATE 20020717
	// warn if the stream is generated by old vorbis encoder earlier than this
#define WARN_PREFER_ENCODER_VERSION "Vorbis encoder 1.0 (dated 2002/07/17) or later is recommended."
static bool OldEncoderWarned = false;
static LONG AllocCount = 0; // memory block allocation count for decoder

static bool Look_Replay_Gain = false; // whether to look replay gain information
static bool Use_Album_Gain = false; // whether to use album gain, otherwise use track gain

//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}
//---------------------------------------------------------------------------
void strcpy_limit(LPWSTR dest, LPWSTR  src, int n)
{
	// string copy with limitation
	// this will add a null terminater at destination buffer
	wcsncpy(dest, src, n-1);
	dest[n-1] = '\0';
}
//---------------------------------------------------------------------------
ITSSStorageProvider *StorageProvider = NULL;
//---------------------------------------------------------------------------
class VorbisModule : public ITSSModule // module interface
{
	ULONG RefCount; // reference count

public:
	VorbisModule();
	~VorbisModule();

public:
	// IUnknown
	HRESULT __stdcall QueryInterface(REFIID iid, void ** ppvObject);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);
	
	// ITSSModule
	HRESULT __stdcall GetModuleCopyright(LPWSTR buffer, unsigned long buflen );
	HRESULT __stdcall GetModuleDescription(LPWSTR buffer, unsigned long buflen );
	HRESULT __stdcall GetSupportExts(unsigned long index, LPWSTR mediashortname, LPWSTR buf, unsigned long buflen );
	HRESULT __stdcall GetMediaInfo(LPWSTR url, ITSSMediaBaseInfo ** info );
	HRESULT __stdcall GetMediaSupport(LPWSTR url );
	HRESULT __stdcall GetMediaInstance(LPWSTR url, IUnknown ** instance );
};
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
class VorbisWaveDecoder : public ITSSWaveDecoder // decoder interface
{
	ULONG RefCount; // refernce count
	bool InputFileInit; // whether InputFile is inited
	OggVorbis_File InputFile; // OggVorbis_File instance
	IStream *InputStream; // input stream
	TSSWaveFormat Format; // output PCM format
	int CurrentSection; // current section in vorbis stream

public:
	VorbisWaveDecoder();
	~VorbisWaveDecoder();

public:
	// IUnkown
	HRESULT __stdcall QueryInterface(REFIID iid, void ** ppvObject);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// ITSSWaveDecoder
	HRESULT __stdcall GetFormat(TSSWaveFormat *format);
	HRESULT __stdcall Render(void *buf, unsigned long bufsamplelen,
            unsigned long *rendered, unsigned long *status);
	HRESULT __stdcall SetPosition(unsigned __int64 samplepos);

	// others
	HRESULT SetStream(IStream *stream, LPWSTR url);

private:
	size_t static _cdecl read_func(void *ptr, size_t size, size_t nmemb, void *datasource);
	int static _cdecl seek_func(void *datasource, __int64 offset, int whence);
	int static _cdecl close_func(void *datasource);
	long static _cdecl tell_func(void *datasource);
};
//---------------------------------------------------------------------------
// VorbisModule implementation ##############################################
//---------------------------------------------------------------------------
VorbisModule::VorbisModule()
{
	// VorbisModule constructor
	RefCount = 1;
}
//---------------------------------------------------------------------------
VorbisModule::~VorbisModule()
{
	// VorbisModule destructor
}
//---------------------------------------------------------------------------
HRESULT __stdcall VorbisModule::QueryInterface(REFIID iid, void ** ppvObject)
{
	// IUnknown::QueryInterface

	if(!ppvObject) return E_INVALIDARG;

	*ppvObject=NULL;
	if(!memcmp(&iid,&IID_IUnknown,16))
		*ppvObject=(IUnknown*)this;
	else if(!memcmp(&iid,&IID_ITSSModule,16))
		*ppvObject=(ITSSModule*)this;

	if(*ppvObject)
	{
		AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}
//---------------------------------------------------------------------------
ULONG __stdcall VorbisModule::AddRef()
{
	return ++RefCount;
}
//---------------------------------------------------------------------------
ULONG __stdcall VorbisModule::Release()
{
	if(RefCount == 1)
	{
		delete this;
		return 0;
	}
	else
	{
		return --RefCount;
	}
}
//---------------------------------------------------------------------------
HRESULT __stdcall VorbisModule::GetModuleCopyright(LPWSTR buffer, unsigned long buflen)
{
	// return module copyright information
	strcpy_limit(buffer, L"OggVorbis Plug-in for TVP Sound System (C) 2000-2007 W.Dee <dee@kikyou.info>",
			buflen);
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall VorbisModule::GetModuleDescription(LPWSTR buffer, unsigned long buflen )
{
	// return module description
	strcpy_limit(buffer, L"Ogg Vorbis (*.ogg) decoder", buflen);
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall VorbisModule::GetSupportExts(unsigned long index, LPWSTR mediashortname,
												LPWSTR buf, unsigned long buflen )
{
	// return supported file extensios
	if(index >= 1) return S_FALSE;
	wcscpy(mediashortname, L"Ogg Stream Format");
	strcpy_limit(buf, L".ogg", buflen);
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall VorbisModule::GetMediaInfo(LPWSTR url, ITSSMediaBaseInfo ** info )
{
	// return media information interface
	return E_NOTIMPL; // not implemented
}
//---------------------------------------------------------------------------
HRESULT __stdcall VorbisModule::GetMediaSupport(LPWSTR url )
{
	// return media support interface
	return E_NOTIMPL; // not implemented
}
//---------------------------------------------------------------------------
HRESULT __stdcall VorbisModule::GetMediaInstance(LPWSTR url, IUnknown ** instance )
{
	// create a new media interface
	HRESULT hr;

	// retrieve input stream interface
	IStream *stream;
	hr = StorageProvider->GetStreamForRead(url, (IUnknown**)&stream);
	if(FAILED(hr)) return hr;

	// create vorbis decoder
	VorbisWaveDecoder * decoder = new VorbisWaveDecoder();
	hr = decoder->SetStream(stream, url);
	if(FAILED(hr))
	{
		// error; stream may not be a vorbis stream
		delete decoder;
		stream->Release();
		return hr;
	}

	*instance = (IUnknown*)decoder; // return as IUnknown
	stream->Release(); // release stream because the decoder already holds it

	return S_OK;
}
//---------------------------------------------------------------------------
// VorbisWaveDecoder implementation #########################################
//---------------------------------------------------------------------------
VorbisWaveDecoder::VorbisWaveDecoder()
{
	// VorbisWaveDecoder constructor
	RefCount = 1;
	InputFileInit = false;
	InputStream = NULL;
	CurrentSection = -1;
}
//---------------------------------------------------------------------------
VorbisWaveDecoder::~VorbisWaveDecoder()
{
	// VorbisWaveDecoder destructor
	if(InputFileInit)
	{
		ov_clear(&InputFile);
		InputFileInit = false;
	}
	if(InputStream)
	{
		InputStream->Release();
		InputStream = NULL;
	}
}
//---------------------------------------------------------------------------
HRESULT VorbisWaveDecoder::QueryInterface(REFIID iid, void ** ppvObject)
{
	// IUnknown::QueryInterface

	if(!ppvObject) return E_INVALIDARG;

	*ppvObject=NULL;
	if(!memcmp(&iid,&IID_IUnknown,16))
		*ppvObject=(IUnknown*)this;
	else if(!memcmp(&iid,&IID_ITSSWaveDecoder,16))
		*ppvObject=(ITSSWaveDecoder*)this;

	if(*ppvObject)
	{
		AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}
//---------------------------------------------------------------------------
ULONG __stdcall VorbisWaveDecoder::AddRef()
{
	return ++RefCount;
}
//---------------------------------------------------------------------------
ULONG __stdcall VorbisWaveDecoder::Release()
{
	if(RefCount == 1)
	{
		delete this;
		return 0;
	}
	else
	{
		return --RefCount;
	}
}
//---------------------------------------------------------------------------
HRESULT __stdcall VorbisWaveDecoder::GetFormat(TSSWaveFormat *format)
{
	// return PCM format
	if(!InputFileInit)
	{
		return E_FAIL;
	}

	*format = Format;

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall VorbisWaveDecoder::Render(void *buf, unsigned long bufsamplelen,
            unsigned long *rendered, unsigned long *status)
{
	// render output PCM
	if(!InputFileInit) return E_FAIL; // InputFile is yet not inited

	int pcmsize = FloatExtraction?4:2;

	int res;
	int pos = 0; // decoded PCM (in bytes)
	int remain = bufsamplelen * Format.dwChannels * pcmsize; // remaining PCM (in bytes)

	while(remain)
	{
		do
		{
			res = ov_read(&InputFile, ((char*)buf + pos), remain,
				0, pcmsize, 1, &CurrentSection); // decode via ov_read
		} while(res<0); // ov_read would return a negative number
						// if the decoding is not ready
		if(res==0) break;
		pos += res;
		remain -= res;
	}

	pos /= (Format.dwChannels * pcmsize); // convert to PCM position
	
	if(status)
	{
		*status = ((unsigned int)pos < bufsamplelen)?0:1;
			// *status will be 0 if the decoding is ended
	}

	if(rendered)
	{
		*rendered = pos; // return renderd PCM samples
	}

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall VorbisWaveDecoder::SetPosition(unsigned __int64 samplepos)
{
	// set PCM position (seek)
	if(!InputFileInit) return E_FAIL;

	if(0 != ov_pcm_seek(&InputFile, samplepos))
	{
		return E_FAIL;
	}

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VorbisWaveDecoder::SetStream(IStream *stream, LPWSTR url)
{
	// set input stream
	InputStream = stream;
	InputStream->AddRef(); // add-ref

	ov_callbacks callbacks = {read_func, seek_func, close_func, tell_func};
		// callback functions

	// open input stream via ov_open_callbacks
	if(ov_open_callbacks(this, &InputFile, NULL, 0, callbacks) < 0)
	{
		// error!
		InputStream->Release();
		InputStream = NULL;
		return E_FAIL;
	}

	InputFileInit = true;

	// retrieve PCM information
	vorbis_info *vi;
	vi = ov_info(&InputFile, -1);
	if(!vi)
	{
		return E_FAIL;
	}

	// set Format up
	ZeroMemory(&Format, sizeof(Format));
	Format.dwSamplesPerSec = vi->rate;
	Format.dwChannels = vi->channels;
	Format.dwBitsPerSample = FloatExtraction ? (0x10000 + 32) :  16;
	Format.dwSeekable = 2;

	__int64 pcmtotal = ov_pcm_total(&InputFile, -1); // PCM total samples
	if(pcmtotal<0) pcmtotal = 0;
	Format.ui64TotalSamples = pcmtotal;

	double timetotal = ov_time_total(&InputFile, -1); // total time in sec.
	if(timetotal<0) Format.dwTotalTime = 0; else Format.dwTotalTime = timetotal * 1000.0;

#ifndef NOT_HAVE_TP_STUB
	// check old vorbis stream, which is generated by old encoder.
	if(TVPAlive && !OldEncoderWarned)
	{
		vorbis_comment *vc = ov_comment(&InputFile, -1);
		if(vc->vendor)
		{
			const char * vorbis_date = strstr(vc->vendor, "libVorbis I ");
			if(vorbis_date)
			{
				int date = atoi(vorbis_date + 12);
				if(date < WARN_OLD_VORBIS_DATE)
				{
					OldEncoderWarned = true;
					TVPAddLog(TJS_W("wuvorbis: warning: The Vorbis stream \"") + ttstr(url) + TJS_W("\" seems to be generated by old Vorbis encoder [") + 
						ttstr(vc->vendor) + TJS_W("]. Check that your encoder is new enough, unless you prefer the old encoder. The old stream can still be decoded, but ") + ttstr(WARN_PREFER_ENCODER_VERSION) );
				}
			}
		}
	}

	if(Look_Replay_Gain)
	{
		for(int i = 0; i < InputFile.links; i++)
		{
			// for each stream
			float gain = 1.0;
			vorbis_comment *vc = ov_comment(&InputFile, i);
			const char * track = vorbis_comment_query(vc, "replaygain_track_gain", 0);
			const char * album = vorbis_comment_query(vc, "replaygain_album_gain", 0);

			const char * sel = Use_Album_Gain ? ( album ? album : track ) : track;
			if(sel)
			{
				float db = (float)atof(sel); // in db
				gain *= (float)pow(10.0, db / 20); // convert db to multiplier
			}
			
			vorbis_info_set_global_gain(ov_info(&InputFile, i), gain);
		}
	}
#endif

	return S_OK;
}
//---------------------------------------------------------------------------
size_t _cdecl VorbisWaveDecoder::read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	// read function (wrapper for IStream)

	VorbisWaveDecoder * decoder = (VorbisWaveDecoder*)datasource;
	if(!decoder->InputStream) return 0;

	ULONG bytesread;
	if(FAILED(decoder->InputStream->Read(ptr, (ULONG)(size * nmemb), &bytesread)))
	{
		return 0; // failed
	}

	return bytesread / size;
}
//---------------------------------------------------------------------------
int _cdecl VorbisWaveDecoder::seek_func(void *datasource, __int64 offset, int whence)
{
	// seek function (wrapper for IStream)

	VorbisWaveDecoder * decoder = (VorbisWaveDecoder*)datasource;
	if(!decoder->InputStream) return -1;

	LARGE_INTEGER newpos;
	ULARGE_INTEGER result;
	newpos.QuadPart = offset;
	int seek_type = STREAM_SEEK_SET;
	
	switch(whence)
	{
	case SEEK_SET:
		seek_type = STREAM_SEEK_SET;
		break;
	case SEEK_CUR:
		seek_type = STREAM_SEEK_CUR;
		break;
	case SEEK_END:
		seek_type = STREAM_SEEK_END;
		break;
	}

	if(FAILED(decoder->InputStream->Seek(newpos, seek_type, &result)))
	{
		return -1;
	}

	return 0;
}
//---------------------------------------------------------------------------
int _cdecl VorbisWaveDecoder::close_func(void *datasource)
{
	// close function (wrapper for IStream)

	VorbisWaveDecoder * decoder = (VorbisWaveDecoder*)datasource;
	if(!decoder->InputStream) return EOF;
	
	decoder->InputStream->Release();
	decoder->InputStream = NULL;

	return 0;
}
//---------------------------------------------------------------------------
long _cdecl VorbisWaveDecoder::tell_func(void *datasource)
{
	// tell function (wrapper for IStream)

	VorbisWaveDecoder * decoder = (VorbisWaveDecoder*)datasource;
	if(!decoder->InputStream) return EOF;

	LARGE_INTEGER newpos;
	ULARGE_INTEGER result;
	newpos.QuadPart = 0;

	if(FAILED(decoder->InputStream->Seek(newpos, STREAM_SEEK_CUR, &result)))
	{
		return EOF;
	}
	return (long)(result.QuadPart); // ?? this will collapse int64 to int32
}
//---------------------------------------------------------------------------
// ##########################################################################
//---------------------------------------------------------------------------
extern "C"
{
	int CPU_SSE = 0;
	int CPU_MMX = 0;
	int CPU_3DN = 0;
	void scale_FLOOR1_fromdB_LOOKUP(float fac) ;
	void scale_FROMdB2_LOOKUP(float fac) ;
}
//---------------------------------------------------------------------------
HRESULT _stdcall GetModuleInstance(ITSSModule **out, ITSSStorageProvider *provider,
	IStream * config, HWND mainwin)
{
	// GetModuleInstance function (exported)

	StorageProvider = provider;
	*out = new VorbisModule();
	return S_OK;
}
//---------------------------------------------------------------------------
static void InternalSetCPUType(tjs_uint32 cputype)
{
	CPU_SSE = cputype & TVP_CPU_HAS_SSE;
	CPU_MMX = cputype & TVP_CPU_HAS_MMX;
	CPU_3DN = cputype & TVP_CPU_HAS_3DN;
	if(!CPU_MMX) CPU_3DN = 0; // 3D Now! uses MMX registers
	if(CPU_SSE && CPU_3DN) CPU_SSE = 0; // Athlon XP; 3DNow! is faster than SSE on this decoder.
}
//---------------------------------------------------------------------------
extern "C" HRESULT _stdcall _export V2Link(iTVPFunctionExporter *exporter)
{
#ifndef NOT_HAVE_TP_STUB
	// exported function, only called by kirikiri ver 2+
	TVPAlive = true;
	TVPInitImportStub(exporter);

	// retrieve CPU information
	tjs_uint32 cputype = TVPGetCPUType();

	InternalSetCPUType(cputype);

	ttstr debug_str(L"wuvorbis:");

	if(CPU_SSE) debug_str += L" SSE enabled."; else debug_str += L" SSE disabled.";
	if(CPU_MMX) debug_str += L" MMX enabled."; else debug_str += L" MMX disabled.";
	if(CPU_3DN) debug_str += L" 3DNow! enabled."; else debug_str += L" 3DNow! disabled.";

	TVPAddImportantLog(debug_str); 

	tTJSVariant val;
	if(TVPGetCommandLine(TJS_W("-vorbis_gain"), &val))
	{
		double db = (tTVReal)val;
		double fac = pow(10.0, db / 20);
		debug_str = TJS_W("wuvorbis: Setting global gain to ");
		val = (tTVReal)db;
		debug_str += ttstr(val);
		debug_str += TJS_W("dB (");
		val = (tTVReal)(fac * 100);
		debug_str += ttstr(val);
		debug_str += TJS_W("%)");
		scale_FLOOR1_fromdB_LOOKUP(fac);
		scale_FROMdB2_LOOKUP(fac);
		TVPAddLog(debug_str);
	}

	if(TVPGetCommandLine(TJS_W("-vorbis_pcm_format"), &val))
	{
		ttstr sval(val);
		if(sval == TJS_W("f32"))
		{
			FloatExtraction = true;
			scale_FLOOR1_fromdB_LOOKUP(1.0f/32768.0f);
			scale_FROMdB2_LOOKUP(1.0f/32768.0f);
			TVPAddLog(TJS_W("wuvorbis: IEEE 32bit float output enabled."));
		}
	}

	Look_Replay_Gain = true; // whether to look replay gain information
	Use_Album_Gain = false; // whether to use album gain, otherwise use track gain

	if(TVPGetCommandLine(TJS_W("-vorbis_rg"), &val))
	{
		ttstr sval(val);
		if(sval == TJS_W("none") || sval == TJS_W("no"))
		{
			Look_Replay_Gain = false;
		}
		else if(sval == TJS_W("track"))
		{
			Use_Album_Gain = false;
		}
		else if(sval == TJS_W("album"))
		{
			Use_Album_Gain = true;
		}
	}

#endif

	return S_OK;
}
//---------------------------------------------------------------------------
extern "C" HRESULT _stdcall _export V2Unlink()
{
#ifndef NOT_HAVE_TP_STUB
	TVPUninitImportStub();
#endif

	if(AllocCount != 0)
	{
		// Not all memory blockes are freed
//		MessageBox(NULL, "Memory leak detected in wuvorbis.dll", "DEBUG", MB_OK);
	}

	return S_OK;
}
//---------------------------------------------------------------------------
extern "C" const wchar_t * _stdcall _export GetOptionDesc()
{
	if(GetACP() == 932) // 932 == Japan
	{
		return 
L"OggVorbisデコーダ:ゲイン;OggVorbis デコーダの音量(ゲイン)の設定です。|"
L"vorbis_gain|select,"
L"*0;0dB,-0.1;-0.1dB,-0.2;-0.2dB,-0.3;-0.3dB,-0.4;-0.4dB,-0.5;-0.5dB,"
L"-0.6;-0.6dB,-0.7;-0.7dB,-0.8;-0.8dB,-0.9;-0.9dB,-1;-1dB,-1.1;-1.1dB,"
L"-1.2;-1.2dB,-1.3;-1.3dB,-1.4;-1.4dB,-1.5;-1.5dB,-1.6;-1.6dB,"
L"-1.7;-1.7dB,-1.8;-1.8dB,-1.9;-1.9dB,-2;-2dB,-3;-3dB,-4;"
L"-4dB,-5;-5dB,-6;-6dB,-7;-7dB,-8;-8dB,-9;-9dB,-10;-10dB\n"
L"OggVorbisデコーダ:出力形式;OggVorbis デコーダが出力する PCM 形式の設定です。|"
L"vorbis_pcm_format|select,*i16;16bit 整数 PCM,f32;32bit 浮動小数点数 PCM\n"
L"OggVorbisデコーダ:ReplayGain;ReplayGainに対応するかどうかの設定です。|"
L"vorbis_rg|select,none;対応しない,album;Album Gain を見る,*track;Track Gain を見る\n"
/*
L"OggVorbisデコーダ:音質;OggVorbisデコーダの音質設定です。「低い」を選択すると「通常」よりも再生の品質は低くなりますが、"
L"OggVorbisのデコードに関するCPUへの負担が軽くなります。|"
L"vorbis_quality,*normal;通常,low;低い\n"*/;
	}
	else
	{
		return
L"OggVorbis decoder:Gain;Specify default gain for OggVorbis decoder.|"
L"vorbis_gain|select,"
L"*0;0dB,-0.1;-0.1dB,-0.2;-0.2dB,-0.3;-0.3dB,-0.4;-0.4dB,-0.5;-0.5dB,"
L"-0.6;-0.6dB,-0.7;-0.7dB,-0.8;-0.8dB,-0.9;-0.9dB,-1;-1dB,-1.1;-1.1dB,"
L"-1.2;-1.2dB,-1.3;-1.3dB,-1.4;-1.4dB,-1.5;-1.5dB,-1.6;-1.6dB,"
L"-1.7;-1.7dB,-1.8;-1.8dB,-1.9;-1.9dB,-2;-2dB,-3;-3dB,-4;"
L"-4dB,-5;-5dB,-6;-6dB,-7;-7dB,-8;-8dB,-9;-9dB,-10;-10dB\n"
L"OggVorbis decoder:Output format;Specify PCM format that the OggVorbis decoder outputs.|"
L"vorbis_pcm_format|select,*i16;16bit integer PCM,f32;32bit floating-point PCM\n"
L"OggVorbis decoder:ReplayGain;Whether to apply ReplayGain tag information.|"
L"vorbis_rg|select,none;Ignore,album;Apply Album Gain,*track;Apply Track Gain\n"
/*
L"OggVorbis decoder:Quality;Quality setting for the OggVorbis decoder. "
L"\"Low\" performs faster speed and lower CPU usage than \"Normal\", but the sound quality will be degraded.|"
L"vorbis_quality,*normal;Normal,low;Low\n"*/;
	}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ##########################################################################
//---------------------------------------------------------------------------
// overriding memory allocation functions
struct alloc_record
{
	void *org_ptr;
	size_t org_size;
};
extern "C"
{
//---------------------------------------------------------------------------
void * dee_ogg_malloc(size_t bytes)
{
	bytes += 16;
	tjs_int align = 16;
	void *ptr = malloc(bytes + align + sizeof(alloc_record));
	if(!ptr) return NULL;
	void *org_ptr = ptr;
	DWORD *iptr =
		reinterpret_cast<DWORD *>(&ptr);
	*iptr += align + sizeof(alloc_record);
	*iptr &= ~(DWORD)(align - 1);
	(reinterpret_cast<alloc_record*>(ptr))[-1].org_ptr = org_ptr;
	(reinterpret_cast<alloc_record*>(ptr))[-1].org_size = bytes;
	InterlockedIncrement(&AllocCount);
	return ptr;
}
//---------------------------------------------------------------------------
void * dee_ogg_calloc(size_t num, size_t size)
{
	size += 16;
	size_t sz = num * size;
	void *ptr = dee_ogg_malloc(sz);
	memset(ptr, 0, sz);
	return ptr;
}
//---------------------------------------------------------------------------
void dee_ogg_free(void *ptr)
{
	free((reinterpret_cast<alloc_record*>(ptr))[-1].org_ptr);
	InterlockedDecrement(&AllocCount);
}
//---------------------------------------------------------------------------
void * dee_ogg_realloc(void *block, size_t bytes)
{
	bytes += 16;
	size_t org_size = (reinterpret_cast<alloc_record*>(block))[-1].org_size;

	if(org_size == bytes) return block;

	void * newptr = dee_ogg_malloc(bytes);
	if(!newptr) return NULL;

	memcpy(newptr, block, org_size);

	dee_ogg_free(block);
	
	return newptr;
}
//---------------------------------------------------------------------------
}

//---------------------------------------------------------------------------
// ##########################################################################
//---------------------------------------------------------------------------
extern "C" int _stdcall _export Query_sizeof_OggVorbis_File()
{
	// returns sizeof(OggVorbis_File)
	return sizeof(OggVorbis_File);
}
//---------------------------------------------------------------------------


// wuvorbis service functions

//---------------------------------------------------------------------------
extern "C" void SetCPUType(unsigned __int32 type)
{
	InternalSetCPUType(type);
}
//---------------------------------------------------------------------------
extern "C"  unsigned __int32 _cdecl TVPCheckCPU(void);
extern "C"  unsigned __int32 TVPCPUType = 0;
extern "C" unsigned __int32 DetectCPU(void)
{
	TVPCPUType = TVPCheckCPU();

	if(TVPCPUType & TVP_CPU_HAS_SSE)
	{
		// do SSE check
		__try
		{
			_asm { xorps xmm0, xmm0 } //  (SSE)
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			// exception had been ocured
			// execution of 'xorps' is failed (XMM registers not available)
			TVPCPUType &=~ TVP_CPU_HAS_SSE;
			TVPCPUType &=~ TVP_CPU_HAS_SSE2;
		}
	}
	
	return TVPCPUType;
}
//---------------------------------------------------------------------------
extern "C" void ScaleOutput(float scale)
{
	scale_FLOOR1_fromdB_LOOKUP((float)scale) ;
	scale_FROMdB2_LOOKUP((float)scale) ;
}
//---------------------------------------------------------------------------

#define IN_WUVORBISMAINUNIT
#include "decapi/include/wuvorbisfile.h"
#include "wuvorbisfile_stub.c"
