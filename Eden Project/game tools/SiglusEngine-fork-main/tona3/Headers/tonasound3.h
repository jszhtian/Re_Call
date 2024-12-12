
#include	"tona3.h"
#include	<dsound.h>
#include	"tonasound3_dsound.h"
#include	"tonasound3_sound.h"
#include	"tonasound3_player.h"
#include	"tonasound3_wave.h"
#include	"tonasound3_ogg.h"
#include	"tonasound3_nwa.h"
#include	"tonasound3_omv.h"

// ****************************************************************
// é©êgÇÃÉâÉCÉuÉâÉä
// ================================================================
#ifdef _DEBUG
	#pragma		comment(lib,"TonaSound3_d.lib")
#else
	#pragma		comment(lib,"TonaSound3.lib")
#endif

// ****************************************************************
// DirectSound5
// ================================================================
#pragma comment(lib, "dsound5.lib")

// ****************************************************************
// ogg
// ================================================================
#ifdef _DEBUG

#if __T3_DONT_USE_OGG
#else
	#pragma comment(lib, "libogg_d.lib")
	#pragma comment(lib, "libvorbis_d.lib")
	#pragma comment(lib, "libvorbisfile_d.lib")
	#pragma comment(lib, "libtheora_d.lib")
#endif

#else

#if __T3_DONT_USE_OGG
#else
#pragma comment(lib, "libogg.lib")
	#pragma comment(lib, "libvorbis.lib")
	#pragma comment(lib, "libvorbisfile.lib")
	#pragma comment(lib, "libtheora.lib")
#endif

#endif


