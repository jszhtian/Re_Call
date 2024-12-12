#include	"tona3.h"

using namespace NT3;

// ****************************************************************
// DirectSound
// ================================================================
#include	<dsound5.h>

// ****************************************************************
// ogg
// ================================================================
#ifdef _DEBUG
	#pragma comment(lib, "libogg_d.lib")
	#pragma comment(lib, "libvorbis_d.lib")
	#pragma comment(lib, "libvorbisfile_d.lib")
	#pragma comment(lib, "libtheora_d.lib")
#else
	#pragma comment(lib, "libogg.lib")
	#pragma comment(lib, "libvorbis.lib")
	#pragma comment(lib, "libvorbisfile.lib")
	#pragma comment(lib, "libtheora.lib")
#endif