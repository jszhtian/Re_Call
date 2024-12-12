#pragma		once

#include	"tonad3d3_d3d.h"

namespace NT3
{

// usage, format, pool
TSTR	get_str_D3DERR(HRESULT hr);
TSTR	get_str_D3DUSAGE(DWORD usage);
TSTR	get_str_D3DFORMAT(D3DFORMAT format);
TSTR	get_str_D3DPOOL(D3DPOOL pool);

// �f�B�X�v���C
TSTR	get_str_d3d_display_adapter(UINT adapter);

// �f�o�C�X
TSTR	get_str_D3DDEVTYPE(D3DDEVTYPE type);
TSTR	get_str_d3d_vertex_process_type(DWORD type);

struct S_d3d_caps_primitive_misc_caps_data
{
	long	mask;
	TCHAR*	define;
	TCHAR*	explanation;
};

const S_d3d_caps_primitive_misc_caps_data G_d3d_caps_primitive_misc_caps_data[] =
{
	{0x00000002L,	_T("D3DPMISCCAPS_MASKZ"),						_T("�f�o�C�X�́A�s�N�Z�������ɂ�����[�x�o�b�t�@�̕ύX�̗L��/������؂�ւ��邱�Ƃ��ł���B")	}, 
	{0x00000010L,	_T("D3DPMISCCAPS_CULLNONE"),					_T("�h���C�o�́A�O�p�`�J�����O�����s���Ȃ��B����́AD3DCULL �񋓌^�� D3DCULL_NONE �����o�ɑΉ�����B")	},
	{0x00000020L,	_T("D3DPMISCCAPS_CULLCW"),						_T("�h���C�o�́AD3DRS_CULLMODE �X�e�[�g���g���Ď��v���̎O�p�`�J�����O���T�|�[�g���� (�O�p�`�̃v���~�e�B�u�̏ꍇ�̂�)�B���̃t���O�́AD3DCULL �񋓌^�� D3DCULL_CW �����o�ɑΉ�����B")	},
	{0x00000040L,	_T("D3DPMISCCAPS_CULLCCW"),						_T("�h���C�o�́AD3DRS_CULLMODE �X�e�[�g���g���Ĕ����v���̃J�����O���T�|�[�g���� (�O�p�`�̃v���~�e�B�u�̏ꍇ�̂�)�B���̃t���O�́AD3DCULL �񋓌^�� D3DCULL_CCW �����o�ɑΉ�����B")	},
	{0x00000100L,	_T("D3DPMISCCAPS_COLORWRITEENABLE"),			_T("�f�o�C�X�́AD3DRS_COLORWRITEENABLE �X�e�[�g���g���āA�����_�����O �^�[�Q�b�g�̃J���[ �o�b�t�@�ւ̃`�����l�����Ƃ̏������݂��T�|�[�g����B")	},
	{0x00000200L,	_T("D3DPMISCCAPS_CLIPPLANESCALEDPOINTS"),		_T("�f�o�C�X�́A���m�ɁA1.0 ���傫���T�C�Y�̃X�P�[�����O���ꂽ�|�C���g���A���[�U�[��`�N���b�v�ʂɃN���b�v����B")	},
	{0x00000200L,	_T("D3DPMISCCAPS_CLIPTLVERTS"),					_T("�f�o�C�X�́A�g�����X�t�H�[����̒��_�v���~�e�B�u���N���b�v����B�p�C�v���C���ŃN���b�s���O���s��Ȃ��悤�ɂ���ɂ́AD3DUSAGE_DONOTCLIP ���w�肷��BD3DPMISCCAPS_CLIPTLVERTS ��ݒ肷��ƁA�o�͒��_�o�b�t�@�̓V�X�e�� ������ (D3DPOOL_SYSTEMMEM) ���ɔz�u�����B����ȊO�̏ꍇ�A���_�o�b�t�@�̓f�t�H���g�̃����� �v�[�� (D3DPOOL_DEFAULT) ���ɔz�u�����B")	},
	{0x00000400L,	_T("D3DPMISCCAPS_TSSARGTEMP"),					_T("�f�o�C�X�́A�e���|���� ���W�X�^�p�� D3DTA ���T�|�[�g����B")	},
	{0x00000800L,	_T("D3DPMISCCAPS_BLENDOP"),						_T("�f�o�C�X�́AD3DBLENDOP_ADD �ȊO�̃A���t�@ �u�����f�B���O�������T�|�[�g����B")	},
	{0x00000100L,	_T("D3DPMISCCAPS_NULLREFERENCE"),				_T("�����_�����O���Ȃ����t�@�����X �f�o�C�X�B")	},
	{0x00004000L,	_T("D3DPMISCCAPS_INDEPENDENTWRITEMASKS"),		_T("�f�o�C�X�́A�����v�f�̃e�N�X�`���܂��͕����̃����_�����O �^�[�Q�b�g�Ōʂ̏������݃}�X�N�̎g�p���T�|�[�g����B")	},
	{0x00008000L,	_T("D3DPMISCCAPS_PERSTAGECONSTANT"),			_T("�f�o�C�X�́A�X�e�[�W���Ƃ̒萔���T�|�[�g����B�uD3DTEXTURESTAGESTATETYPE�v�́uD3DTSS_CONSTANT�v���Q�Ƃ��邱�ƁB")	},
	{0x00010000L,	_T("D3DPMISCCAPS_FOGANDSPECULARALPHA"),			_T("�f�o�C�X�́A�ʂ̃t�H�O����уX�y�L���� �A���t�@���T�|�[�g����B�����̃f�o�C�X�́A�t�H�O�W���̕ۑ��ɃX�y�L���� �A���t�@ �`�����l�����g���B")	},
	{0x00020000L,	_T("D3DPMISCCAPS_SEPARATEALPHABLEND"),			_T("�f�o�C�X�́A�A���t�@ �`�����l���ɑ΂���ʂ̃u�����h�ݒ���T�|�[�g����B")	},
	{0x00040000L,	_T("D3DPMISCCAPS_MRTINDEPENDENTBITDEPTHS"),		_T("�f�o�C�X�́A�����̃����_�����O �^�[�Q�b�g�ɑ΂��ĈقȂ�r�b�g�[�x���T�|�[�g����B")	},
	{0x00080000L,	_T("D3DPMISCCAPS_MRTPOSTPIXELSHADERBLENDING"),	_T("�f�o�C�X�́A�����̃����_�����O �^�[�Q�b�g�ɑ΂���s�N�Z�� �V�F�[�_��̏������T�|�[�g����B")	},
	{0x00100000L,	_T("D3DPMISCCAPS_FOGVERTEXCLAMPED"),			_T("�f�o�C�X�́A���_���Ƃ̃t�H�O �u�����h�W���𐧌�����B")	},
};


// �N���X
struct C_float2
{
	float	x;
	float	y;

	C_float2()	{}
	C_float2(float x_, float y_) : x(x_), y(y_)	{}
	C_float2(C_size size) : x((float)size.cx), y((float)size.cy)	{}

	bool operator == (const C_float2& rhs) const	{	return x == rhs.x && y == rhs.y;			}
	bool operator != (const C_float2& rhs) const	{	return x != rhs.x || y != rhs.y;			}
	C_float2 operator +(C_float2& rhs) const		{	return C_float2(x + rhs.x, y + rhs.y);		}
	C_float2 operator -(C_float2& rhs) const		{	return C_float2(x - rhs.x, y - rhs.y);		}
	C_float2& operator +=(C_float2& rhs)			{	x += rhs.x;	y += rhs.y;		return *this;	}
	C_float2& operator -=(C_float2& rhs)			{	x -= rhs.x;	y -= rhs.y;		return *this;	}

	C_float2& operator =(const D3DXVECTOR2& rhs)	{	x = rhs.x;	y = rhs.y;		return *this;	}
	C_float2& operator +=(D3DXVECTOR2& rhs)			{	x += rhs.x;	y += rhs.y;		return *this;	}

	operator C_size() const	{	return C_size((int)x, (int)y);	}
};

// �N���X
struct C_float3
{
	float	x;
	float	y;
	float	z;

	C_float3()	{}
	C_float3(float x_, float y_, float z_) : x(x_), y(y_), z(z_)	{}

	bool operator == (const C_float3& rhs) const	{	return x == rhs.x && y == rhs.y && z == rhs.z;			}
	bool operator != (const C_float3& rhs) const	{	return x != rhs.x || y != rhs.y || z != rhs.z;			}
	C_float3 operator +(C_float3& rhs) const		{	return C_float3(x + rhs.x, y + rhs.y, z + rhs.z);		}
	C_float3 operator -(C_float3& rhs) const		{	return C_float3(x - rhs.x, y - rhs.y, z - rhs.z);		}
	C_float3& operator +=(C_float3& rhs)			{	x += rhs.x;	y += rhs.y;	z += rhs.z;		return *this;	}
	C_float3& operator -=(C_float3& rhs)			{	x -= rhs.x;	y -= rhs.y;	z -= rhs.z;		return *this;	}

	C_float3 operator *(float rhs) const			{	return C_float3(x * rhs, y * rhs, z * rhs);				}
	C_float3& operator *=(float rhs)				{	x *= rhs;	y *= rhs;	z *= rhs;		return *this;	}
	C_float3& operator =(const D3DXVECTOR3& rhs)	{	x = rhs.x;	y = rhs.y;	z = rhs.z;		return *this;	}
	C_float3& operator +=(D3DXVECTOR3& rhs)			{	x += rhs.x;	y += rhs.y;	z += rhs.z;		return *this;	}

	C_float2 xy()	{	return C_float2(x, y);	}
};

}
