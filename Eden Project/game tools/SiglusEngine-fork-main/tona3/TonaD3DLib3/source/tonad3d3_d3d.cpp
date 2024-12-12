#include	"tonad3d3_pch.h"
#include	"tonad3d3_d3d.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")

namespace NT3
{

// ****************************************************************
// �B��̃C���X�^���X
// ================================================================
C_d3d	G_d3d;

// ****************************************************************
// Direct3D
// ================================================================
C_d3d::C_d3d()
{
}

C_d3d::~C_d3d()
{
}

// ****************************************************************
// Direct3D �̍\�z
// ================================================================
bool C_d3d::create()
{
	// Direct3D �I�u�W�F�N�g�̍쐬
	IDirect3D9* p_d3d = Direct3DCreate9(D3D9b_SDK_VERSION);		// DirectX 9.0 �œ��������߂ɂ� D3D9b_SDK_VERSION ���w�肷��BSDK October 2004 ���L�̐ݒ�B
	if (p_d3d == NULL)	{
		set_last_error(_T("Direct3D �C���^�[�t�F�C�X�̍\�z�Ɏ��s���܂����B\n\n��q���g��\nDirectX 9.0 ���C���X�g�[������Ă��Ȃ��\��������܂��B\n���̃A�v���P�[�V�������N�����邽�߂ɂ́A�}�C�N���\�t�g�̃E�F�u�T�C�g���ŐV�ł� DirectX End-User Runtime ���C���X�g�[�����Ă��������B"), _T("Direct3DCreate9"));
		return false;
	}

	m_d3d = BIP<IDirect3D9>(p_d3d, false);

	return true;
}

// ****************************************************************
// Direct3D �̉��
// ================================================================
void C_d3d::release()
{
	m_d3d = NULL;
}

// ****************************************************************
// Direct3D �G���[���b�Z�[�W
// ================================================================
static TSTR get_d3d_hresult_str(HRESULT hr)
{
	switch (hr)	{

		// d3d9.h
		case D3DERR_WRONGTEXTUREFORMAT:			return _T("D3DERR_WRONGTEXTUREFORMAT");
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

		// d3dx9.h
		case D3DXERR_CANNOTMODIFYINDEXBUFFER:	return _T("D3DXERR_CANNOTMODIFYINDEXBUFFER");
		case D3DXERR_INVALIDMESH:				return _T("D3DXERR_INVALIDMESH");
		case D3DXERR_CANNOTATTRSORT:			return _T("D3DXERR_CANNOTATTRSORT");
		case D3DXERR_SKINNINGNOTSUPPORTED:		return _T("D3DXERR_SKINNINGNOTSUPPORTED");
		case D3DXERR_TOOMANYINFLUENCES:			return _T("D3DXERR_TOOMANYINFLUENCES");
		case D3DXERR_INVALIDDATA:				return _T("D3DXERR_INVALIDDATA");
		case D3DXERR_LOADEDMESHASNODATA:		return _T("D3DXERR_LOADEDMESHASNODATA");
		case D3DXERR_DUPLICATENAMEDFRAGMENT:	return _T("D3DXERR_DUPLICATENAMEDFRAGMENT");

		default:	return get_hresult_error_msg(hr);
	}
}

// �G���[�ԍ����� Direct3D �̃G���[���b�Z�[�W���擾����
TSTR get_d3d_error_msg(HRESULT hr)
{
	TSTR str;

	if (hr == D3DERR_OUTOFVIDEOMEMORY)
	{
		str = _T("�r�f�I�������e�ʂ�����܂���B��ʂ�����ɕ\������Ȃ��\��������܂��B");
	}
	else
	{
		// �G���[��������\�z
		str = _T("hr = ") + tostr(hr, 16) + _T(" (") + get_d3d_hresult_str(hr) + _T(")");
	}

	return str;
}

// �Ō�̃G���[�� Direct3D �ɐݒ肷��
void set_last_error_d3d(TSTR error_func, HRESULT hr)
{
	TSTR error_str = get_d3d_error_msg(hr);
	set_last_error(error_str, error_func);
}

void set_last_error_d3d(TSTR error_str, TSTR error_func, HRESULT hr)
{
	TSTR d3d_error = get_d3d_error_msg(hr);
	if (!d3d_error.empty())	{
		if (!error_str.empty())		error_str += _T("\n");	// ���s
		error_str += d3d_error;
	}

	set_last_error(error_str, error_func);
}

void add_last_error_d3d(const TSTR& error_func, HRESULT hr)
{
	TSTR error_str = get_d3d_error_msg(hr);
	add_last_error(error_str, error_func);
}

void add_last_error_d3d(TSTR& error_str, const TSTR& error_func, HRESULT hr)
{
	TSTR d3d_error = get_d3d_error_msg(hr);
	if (!d3d_error.empty())	{
		if (!error_str.empty())		error_str += _T("\n");	// ���s
		error_str += d3d_error;
	}

	add_last_error(error_str, error_func);
}

}
