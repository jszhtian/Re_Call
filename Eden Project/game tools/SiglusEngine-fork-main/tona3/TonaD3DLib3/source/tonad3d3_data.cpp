#include	"tonad3d3_pch.h"
#include	"tonad3d3_data.h"

namespace NT3
{

static TSTR str_add(TSTR& lhs, CTSTR& rhs)
{
	return lhs.empty() ? rhs : lhs + _T(" | ") + rhs;
}

// ****************************************************************
// D3DERR
// ================================================================
TSTR get_str_D3DERR(HRESULT hr)
{
	switch (hr)	{
		case D3D_OK:							return _T("D3DOK");
		case D3DERR_UNSUPPORTEDCOLOROPERATION:	return _T("D3DERR_UNSUPPORTEDCOLOROPERATION");
		case D3DERR_UNSUPPORTEDCOLORARG:		return _T("D3DERR_UNSUPPORTEDCOLORARG");
		case D3DERR_UNSUPPORTEDALPHAOPERATION:	return _T("D3DERR_UNSUPPORTEDALPHAOPERATION");
		case D3DERR_UNSUPPORTEDALPHAARG:		return _T("D3DERR_UNSUPPORTEDALPHAARG");
		case D3DERR_TOOMANYOPERATIONS:			return _T("D3DERR_TOOMANYOPERATIONS");
		case D3DERR_CONFLICTINGTEXTUREFILTER:	return _T("D3DERR_CONFLICTINGTEXTUREFILTER");
		case D3DERR_UNSUPPORTEDFACTORVALUE:		return _T("D3DERR_UNSUPPORTEDFACTORVALUE");
		case D3DERR_CONFLICTINGRENDERSTATE:		return _T("D3DERR_CONFLICTINGRENDERSTATE");
		case D3DERR_UNSUPPORTEDTEXTUREFILTER:	return _T("D3DERR_UNSUPPORTEDTEXTUREFILTER");
		case D3DERR_CONFLICTINGTEXTUREPALETTE:	return _T("D3DERR_CONFLICTINGTEXTUREPALETTE");
		case D3DERR_DRIVERINTERNALERROR:		return _T("D3DERR_DRIVERINTERNALERROR");
		case D3DERR_NOTFOUND:					return _T("D3DERR_NOTFOUND");
		case D3DERR_MOREDATA:					return _T("D3DERR_MOREDATA");
		case D3DERR_DEVICELOST:					return _T("D3DERR_DEVICELOST");
		case D3DERR_DEVICENOTRESET:				return _T("D3DERR_DEVICENOTRESET");
		case D3DERR_NOTAVAILABLE:				return _T("D3DERR_NOTAVAILABLE");
		case D3DERR_OUTOFVIDEOMEMORY:			return _T("D3DERR_OUTOFVIDEOMEMORY");
		case D3DERR_INVALIDDEVICE:				return _T("D3DERR_INVALIDDEVICE");
		case D3DERR_INVALIDCALL:				return _T("D3DERR_INVALIDCALL");
		case D3DERR_DRIVERINVALIDCALL:			return _T("D3DERR_DRIVERINVALIDCALL");
		case D3DERR_WASSTILLDRAWING:			return _T("D3DERR_WASSTILLDRAWING");
		case D3DOK_NOAUTOGEN:					return _T("D3DOK_NOAUTOGEN");
	}

	return _T("???");
}

// ****************************************************************
// D3DUSAGE
// ================================================================
TSTR get_str_D3DUSAGE(DWORD usage)
{
	TSTR str;

	if (usage & D3DUSAGE_AUTOGENMIPMAP)			str = str_add(str, _T("D3DUSAGE_AUTOGENMIPMAP"));
	if (usage & D3DUSAGE_DEPTHSTENCIL)			str = str_add(str, _T("D3DUSAGE_DEPTHSTENCIL"));
	if (usage & D3DUSAGE_DMAP)					str = str_add(str, _T("D3DUSAGE_DMAP"));
	if (usage & D3DUSAGE_DONOTCLIP)				str = str_add(str, _T("D3DUSAGE_DONOTCLIP"));
	if (usage & D3DUSAGE_DYNAMIC)				str = str_add(str, _T("D3DUSAGE_DYNAMIC"));
	if (usage & D3DUSAGE_NPATCHES)				str = str_add(str, _T("D3DUSAGE_NPATCHES"));
	if (usage & D3DUSAGE_POINTS)				str = str_add(str, _T("D3DUSAGE_POINTS"));
	if (usage & D3DUSAGE_RENDERTARGET)			str = str_add(str, _T("D3DUSAGE_RENDERTARGET"));
	if (usage & D3DUSAGE_RTPATCHES)				str = str_add(str, _T("D3DUSAGE_RTPATCHES"));
	if (usage & D3DUSAGE_SOFTWAREPROCESSING)	str = str_add(str, _T("D3DUSAGE_SOFTWAREPROCESSING"));
	if (usage & D3DUSAGE_WRITEONLY)				str = str_add(str, _T("D3DUSAGE_WRITEONLY"));

	return str.empty() ? _T("???") : str;
}

// ****************************************************************
// D3DFORMAT
// ================================================================
TSTR get_str_D3DFORMAT(D3DFORMAT format)
{
	switch (format)	{
		case D3DFMT_R8G8B8:			return _T("D3DFMT_R8G8B8");
		case D3DFMT_A8R8G8B8:		return _T("D3DFMT_A8R8G8B8");
		case D3DFMT_X8R8G8B8:		return _T("D3DFMT_X8R8G8B8");
		case D3DFMT_R5G6B5:			return _T("D3DFMT_R5G6B5");
		case D3DFMT_X1R5G5B5:		return _T("D3DFMT_X1R5G5B5");
		case D3DFMT_A1R5G5B5:		return _T("D3DFMT_A1R5G5B5");
		case D3DFMT_A4R4G4B4:		return _T("D3DFMT_A4R4G4B4");
		case D3DFMT_R3G3B2:			return _T("D3DFMT_R3G3B2");
		case D3DFMT_A8:				return _T("D3DFMT_A8");
		case D3DFMT_A8R3G3B2:		return _T("D3DFMT_A8R3G3B2");
		case D3DFMT_X4R4G4B4:		return _T("D3DFMT_X4R4G4B4");
		case D3DFMT_A2B10G10R10:	return _T("D3DFMT_A2B10G10R10");
		case D3DFMT_A8B8G8R8:		return _T("D3DFMT_A8B8G8R8");
		case D3DFMT_X8B8G8R8:		return _T("D3DFMT_X8B8G8R8");
		case D3DFMT_G16R16:			return _T("D3DFMT_G16R16");
		case D3DFMT_A2R10G10B10:	return _T("D3DFMT_A2R10G10B10");
		case D3DFMT_A16B16G16R16:	return _T("D3DFMT_A16B16G16R16");
		case D3DFMT_A8P8:			return _T("D3DFMT_A8P8");
		case D3DFMT_P8:				return _T("D3DFMT_P8");
		case D3DFMT_L8:				return _T("D3DFMT_L8");
		case D3DFMT_L16:			return _T("D3DFMT_L16");
		case D3DFMT_A8L8:			return _T("D3DFMT_A8L8");
		case D3DFMT_A4L4:			return _T("D3DFMT_A4L4");
		case D3DFMT_V8U8:			return _T("D3DFMT_V8U8");
		case D3DFMT_Q8W8V8U8:		return _T("D3DFMT_Q8W8V8U8");
		case D3DFMT_V16U16:			return _T("D3DFMT_V16U16");
		case D3DFMT_Q16W16V16U16:	return _T("D3DFMT_Q16W16V16U16");
		case D3DFMT_CxV8U8:			return _T("D3DFMT_CxV8U8");
		case D3DFMT_L6V5U5:			return _T("D3DFMT_L6V5U5");
		case D3DFMT_X8L8V8U8:		return _T("D3DFMT_X8L8V8U8");
		case D3DFMT_A2W10V10U10:	return _T("D3DFMT_A2W10V10U10");
		case D3DFMT_D16_LOCKABLE:	return _T("D3DFMT_D16_LOCKABLE");
		case D3DFMT_D32:			return _T("D3DFMT_D32");
		case D3DFMT_D15S1:			return _T("D3DFMT_D15S1");
		case D3DFMT_D24S8:			return _T("D3DFMT_D24S8");
		case D3DFMT_D24X8:			return _T("D3DFMT_D24X8");
		case D3DFMT_D24X4S4:		return _T("D3DFMT_D24X4S4");
		case D3DFMT_D32F_LOCKABLE:	return _T("D3DFMT_D32F_LOCKABLE");
		case D3DFMT_D24FS8:			return _T("D3DFMT_D24FS8");
		case D3DFMT_D16:			return _T("D3DFMT_D16");
		case D3DFMT_VERTEXDATA:		return _T("D3DFMT_VERTEXDATA");
		case D3DFMT_INDEX16:		return _T("D3DFMT_INDEX16");
		case D3DFMT_INDEX32:		return _T("D3DFMT_INDEX32");
		case D3DFMT_R16F:			return _T("D3DFMT_R16F");
		case D3DFMT_G16R16F:		return _T("D3DFMT_G16R16F");
		case D3DFMT_A16B16G16R16F:	return _T("D3DFMT_A16B16G16R16F");
		case D3DFMT_R32F:			return _T("D3DFMT_R32F");
		case D3DFMT_G32R32F:		return _T("D3DFMT_G32R32F");
		case D3DFMT_A32B32G32R32F:	return _T("D3DFMT_A32B32G32R32F");
		case D3DFMT_UNKNOWN:		return _T("D3DFMT_UNKNOWN");	
	}

	return _T("???");
}

// ****************************************************************
// D3DPOOL
// ================================================================
TSTR get_str_D3DPOOL(D3DPOOL pool)
{
	switch (pool)	{
		case D3DPOOL_DEFAULT:		return _T("D3DPOOL_DEFAULT");
		case D3DPOOL_MANAGED:		return _T("D3DPOOL_MANAGED");
		case D3DPOOL_SYSTEMMEM:		return _T("D3DPOOL_SYSTEMMEM");
		case D3DPOOL_SCRATCH:		return _T("D3DPOOL_SCRATCH");
	};

	return _T("???");
}

// ****************************************************************
// ディスプレイ
// ================================================================
TSTR get_str_d3d_display_adapter(UINT mode)
{
	if (mode == D3DADAPTER_DEFAULT)
		return (_T("D3DADAPTER_DEFAULT"));

	return tostr(mode);
}

// ****************************************************************
// デバイス
// ================================================================
TSTR get_str_D3DDEVTYPE(D3DDEVTYPE type)
{
	switch (type)	{
		case D3DDEVTYPE_HAL:		return _T("D3DDEVTYPE_HAL");
		case D3DDEVTYPE_REF:		return _T("D3DDEVTYPE_REF");
		case D3DDEVTYPE_SW:			return _T("D3DDEVTYPE_SW");
	}

	return _T("???");
}

TSTR get_str_d3d_vertex_process_type(DWORD type)
{
	if (false);
	else if (type & D3DCREATE_HARDWARE_VERTEXPROCESSING)		return _T("D3DCREATE_HARDWARE_VERTEXPROCESSING");
	else if (type & D3DCREATE_SOFTWARE_VERTEXPROCESSING)		return _T("D3DCREATE_SOFTWARE_VERTEXPROCESSING");
	else if (type & D3DCREATE_MIXED_VERTEXPROCESSING)			return _T("D3DCREATE_MIXED_VERTEXPROCESSING");

	return _T("???");
}


}