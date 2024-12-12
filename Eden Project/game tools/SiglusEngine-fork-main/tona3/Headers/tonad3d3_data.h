#pragma		once

#include	"tonad3d3_d3d.h"

namespace NT3
{

// usage, format, pool
TSTR	get_str_D3DERR(HRESULT hr);
TSTR	get_str_D3DUSAGE(DWORD usage);
TSTR	get_str_D3DFORMAT(D3DFORMAT format);
TSTR	get_str_D3DPOOL(D3DPOOL pool);

// ディスプレイ
TSTR	get_str_d3d_display_adapter(UINT adapter);

// デバイス
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
	{0x00000002L,	_T("D3DPMISCCAPS_MASKZ"),						_T("デバイスは、ピクセル処理における深度バッファの変更の有効/無効を切り替えることができる。")	}, 
	{0x00000010L,	_T("D3DPMISCCAPS_CULLNONE"),					_T("ドライバは、三角形カリングを実行しない。これは、D3DCULL 列挙型の D3DCULL_NONE メンバに対応する。")	},
	{0x00000020L,	_T("D3DPMISCCAPS_CULLCW"),						_T("ドライバは、D3DRS_CULLMODE ステートを使って時計回りの三角形カリングをサポートする (三角形のプリミティブの場合のみ)。このフラグは、D3DCULL 列挙型の D3DCULL_CW メンバに対応する。")	},
	{0x00000040L,	_T("D3DPMISCCAPS_CULLCCW"),						_T("ドライバは、D3DRS_CULLMODE ステートを使って反時計回りのカリングをサポートする (三角形のプリミティブの場合のみ)。このフラグは、D3DCULL 列挙型の D3DCULL_CCW メンバに対応する。")	},
	{0x00000100L,	_T("D3DPMISCCAPS_COLORWRITEENABLE"),			_T("デバイスは、D3DRS_COLORWRITEENABLE ステートを使って、レンダリング ターゲットのカラー バッファへのチャンネルごとの書き込みをサポートする。")	},
	{0x00000200L,	_T("D3DPMISCCAPS_CLIPPLANESCALEDPOINTS"),		_T("デバイスは、正確に、1.0 より大きいサイズのスケーリングされたポイントを、ユーザー定義クリップ面にクリップする。")	},
	{0x00000200L,	_T("D3DPMISCCAPS_CLIPTLVERTS"),					_T("デバイスは、トランスフォーム後の頂点プリミティブをクリップする。パイプラインでクリッピングを行わないようにするには、D3DUSAGE_DONOTCLIP を指定する。D3DPMISCCAPS_CLIPTLVERTS を設定すると、出力頂点バッファはシステム メモリ (D3DPOOL_SYSTEMMEM) 内に配置される。それ以外の場合、頂点バッファはデフォルトのメモリ プール (D3DPOOL_DEFAULT) 内に配置される。")	},
	{0x00000400L,	_T("D3DPMISCCAPS_TSSARGTEMP"),					_T("デバイスは、テンポラリ レジスタ用の D3DTA をサポートする。")	},
	{0x00000800L,	_T("D3DPMISCCAPS_BLENDOP"),						_T("デバイスは、D3DBLENDOP_ADD 以外のアルファ ブレンディング処理をサポートする。")	},
	{0x00000100L,	_T("D3DPMISCCAPS_NULLREFERENCE"),				_T("レンダリングしないリファレンス デバイス。")	},
	{0x00004000L,	_T("D3DPMISCCAPS_INDEPENDENTWRITEMASKS"),		_T("デバイスは、複数要素のテクスチャまたは複数のレンダリング ターゲットで個別の書き込みマスクの使用をサポートする。")	},
	{0x00008000L,	_T("D3DPMISCCAPS_PERSTAGECONSTANT"),			_T("デバイスは、ステージごとの定数をサポートする。「D3DTEXTURESTAGESTATETYPE」の「D3DTSS_CONSTANT」を参照すること。")	},
	{0x00010000L,	_T("D3DPMISCCAPS_FOGANDSPECULARALPHA"),			_T("デバイスは、個別のフォグおよびスペキュラ アルファをサポートする。多数のデバイスは、フォグ係数の保存にスペキュラ アルファ チャンネルを使う。")	},
	{0x00020000L,	_T("D3DPMISCCAPS_SEPARATEALPHABLEND"),			_T("デバイスは、アルファ チャンネルに対する個別のブレンド設定をサポートする。")	},
	{0x00040000L,	_T("D3DPMISCCAPS_MRTINDEPENDENTBITDEPTHS"),		_T("デバイスは、複数のレンダリング ターゲットに対して異なるビット深度をサポートする。")	},
	{0x00080000L,	_T("D3DPMISCCAPS_MRTPOSTPIXELSHADERBLENDING"),	_T("デバイスは、複数のレンダリング ターゲットに対するピクセル シェーダ後の処理をサポートする。")	},
	{0x00100000L,	_T("D3DPMISCCAPS_FOGVERTEXCLAMPED"),			_T("デバイスは、頂点ごとのフォグ ブレンド係数を制限する。")	},
};


// クラス
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

// クラス
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
