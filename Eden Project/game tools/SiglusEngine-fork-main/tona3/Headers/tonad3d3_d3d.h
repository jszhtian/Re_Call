#pragma once

#include	"tonad3d3_device.h"
#include	"tonad3d3_state.h"

namespace NT3
{

// ****************************************************************
// Direct3D
// ================================================================
class C_d3d
{
public:
	C_d3d();
	~C_d3d();

	bool	create();	// Direct3D を構築
	void	release();	// Direct3D を解放

	IDirect3D9*		body()			{	return m_d3d.get();	}	// LPDIRECT3D9 の取得
	IDirect3D9*		operator ->()	{	return m_d3d.get();	}	// LPDIRECT3D9 の取得

	C_d3d_device	device;		// Direct3D デバイス
	C_d3d_state		state;		// Direct3D ステート

private:

	BIP<IDirect3D9>	m_d3d;		// Direct3D インターフェイス
};

// ****************************************************************
// 唯一のインスタンス
// ================================================================
extern	C_d3d		G_d3d;

// ****************************************************************
// 定数
// ================================================================
/// ゼロベクトル
const D3DXVECTOR3 ZERO_VECTOR(0.0f, 0.0f, 0.0f);
/// 左方ベクトル
const D3DXVECTOR3 LEFT_VECTOR(-1.0f, 0.0f, 0.0f);
/// 右方ベクトル
const D3DXVECTOR3 RIGHT_VECTOR(1.0f, 0.0f, 0.0f);
/// 前方ベクトル
const D3DXVECTOR3 FORWARD_VECTOR(0.0f, 0.0f, 1.0f);
/// 後方ベクトル
const D3DXVECTOR3 BACKWARD_VECTOR(0.0f, 0.0f, -1.0f);
/// 上方ベクトル
const D3DXVECTOR3 UP_VECTOR(0.0f, 1.0f, 0.0f);
/// 下方ベクトル
const D3DXVECTOR3 DOWN_VECTOR(0.0f, -1.0f, 0.0f);

/// 単位行列
const D3DXMATRIX IDENTITY_MATRIX(1.0f, 0.0f, 0.0f, 0.0f,
								 0.0f, 1.0f, 0.0f, 0.0f,
								 0.0f, 0.0f, 1.0f, 0.0f,
								 0.0f, 0.0f, 0.0f, 1.0f);

// ****************************************************************
// エラー関数
// ================================================================

// エラー番号から Direct3D のエラーメッセージを取得する
TSTR get_d3d_error_msg(HRESULT hr);

// 最後のエラーを設定
void set_last_error_d3d(TSTR error_func, HRESULT hr);
void set_last_error_d3d(TSTR str, TSTR error_func, HRESULT hr);

void add_last_error_d3d(const TSTR& error_func, HRESULT hr);
void add_last_error_d3d(TSTR& error_str, const TSTR& error_func, HRESULT hr);

}
