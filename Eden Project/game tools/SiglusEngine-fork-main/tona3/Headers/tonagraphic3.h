
#include	"tona3.h"

#include	"tonagraphic3_graphic.h"
#include	"tonagraphic3_gdi.h"
#include	"tonagraphic3_dib.h"
#include	"tonagraphic3_thumb.h"
#include	"tonagraphic3_pct.h"
#include	"tonagraphic3_album.h"
#include	"tonagraphic3_sprite.h"
#include	"tonagraphic3_view.h"
#include	"tonagraphic3_g00.h"

// ****************************************************************
// ���g�̃��C�u����
// ================================================================

#ifdef _DEBUG
#pragma		comment(lib,"TonaGraphic3_d.lib")
#else
#pragma		comment(lib,"TonaGraphic3.lib")
#endif

// ****************************************************************
// png ���C�u����
// ================================================================

#ifdef _DEBUG
#pragma comment(lib, "zlib_d.lib")
#pragma comment(lib, "libpng_d.lib")
#else
#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "libpng.lib")
#endif

// ****************************************************************
// jpeg ���C�u����
// ================================================================

#ifdef __T3_USE_JPEG

#ifdef _DEBUG
#pragma comment(lib, "libmyjpeg_d.lib")
#else
#pragma comment(lib, "libmyjpeg.lib")
#endif

#endif

// ****************************************************************
// g00 ���C�u����
// ================================================================

#ifdef _DEBUG
#pragma		comment(lib,"g00_d.lib")
#else
#pragma		comment(lib,"g00.lib")
#endif
