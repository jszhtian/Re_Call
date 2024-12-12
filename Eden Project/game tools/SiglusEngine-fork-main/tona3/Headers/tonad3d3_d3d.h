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

	bool	create();	// Direct3D ���\�z
	void	release();	// Direct3D �����

	IDirect3D9*		body()			{	return m_d3d.get();	}	// LPDIRECT3D9 �̎擾
	IDirect3D9*		operator ->()	{	return m_d3d.get();	}	// LPDIRECT3D9 �̎擾

	C_d3d_device	device;		// Direct3D �f�o�C�X
	C_d3d_state		state;		// Direct3D �X�e�[�g

private:

	BIP<IDirect3D9>	m_d3d;		// Direct3D �C���^�[�t�F�C�X
};

// ****************************************************************
// �B��̃C���X�^���X
// ================================================================
extern	C_d3d		G_d3d;

// ****************************************************************
// �萔
// ================================================================
/// �[���x�N�g��
const D3DXVECTOR3 ZERO_VECTOR(0.0f, 0.0f, 0.0f);
/// �����x�N�g��
const D3DXVECTOR3 LEFT_VECTOR(-1.0f, 0.0f, 0.0f);
/// �E���x�N�g��
const D3DXVECTOR3 RIGHT_VECTOR(1.0f, 0.0f, 0.0f);
/// �O���x�N�g��
const D3DXVECTOR3 FORWARD_VECTOR(0.0f, 0.0f, 1.0f);
/// ����x�N�g��
const D3DXVECTOR3 BACKWARD_VECTOR(0.0f, 0.0f, -1.0f);
/// ����x�N�g��
const D3DXVECTOR3 UP_VECTOR(0.0f, 1.0f, 0.0f);
/// �����x�N�g��
const D3DXVECTOR3 DOWN_VECTOR(0.0f, -1.0f, 0.0f);

/// �P�ʍs��
const D3DXMATRIX IDENTITY_MATRIX(1.0f, 0.0f, 0.0f, 0.0f,
								 0.0f, 1.0f, 0.0f, 0.0f,
								 0.0f, 0.0f, 1.0f, 0.0f,
								 0.0f, 0.0f, 0.0f, 1.0f);

// ****************************************************************
// �G���[�֐�
// ================================================================

// �G���[�ԍ����� Direct3D �̃G���[���b�Z�[�W���擾����
TSTR get_d3d_error_msg(HRESULT hr);

// �Ō�̃G���[��ݒ�
void set_last_error_d3d(TSTR error_func, HRESULT hr);
void set_last_error_d3d(TSTR str, TSTR error_func, HRESULT hr);

void add_last_error_d3d(const TSTR& error_func, HRESULT hr);
void add_last_error_d3d(TSTR& error_str, const TSTR& error_func, HRESULT hr);

}
