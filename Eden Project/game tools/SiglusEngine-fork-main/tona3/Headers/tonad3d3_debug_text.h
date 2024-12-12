//============================================================================================
// Name : tonad3d3_debug_text.h
// Spec : �f�o�b�O�p������\���w�b�_
// Author : ��،����Y
//============================================================================================
#pragma once

#include	"tonad3d3_d3d.h"


namespace NT3
{

/**
//
* �f�o�b�O�p�e�L�X�g�`��N���X
* �o�b�t�@�Ƀf�[�^�𒙂߂ĕ`�悷��
* �F�͑S���ꂳ���̂ŕ�����ύX�����ꍇ�͍Ō�̂��K�p�����
* �����܂Ŋ�{�@�\�݂̂łƂǂ߂Ă���
*/ 
class C_d3d_debug_text {
	/**
	//
	* �f�o�b�O�p�e�L�X�g�f�[�^�\����
	*/ 
	struct S_debug_text_data {
		TSTR	str;			// �f�o�b�O������
		int		x;				// �o��X���W
		int		y;				// �o��Y���W
	};
private:
	BIP<ID3DXFont>				m_font;				// �t�H���g�I�u�W�F�N�g
	ARRAY<S_debug_text_data>	m_array;
	unsigned long				m_color;			// �e�L�X�g�J���[

public:
	// �R���X�g���N�^
	C_d3d_debug_text();	

	// �f�X�g���N�^
	~C_d3d_debug_text();


	// ������
	bool init(bool release_flag);

	// ���
	void release();

	// �\�z
	bool create(bool init_flag);


	// �����̐F��ύX����
	// @param[in] r �J���[R
	// @param[in] g �J���[G
	// @param[in] b �J���[B
	void set_color(int r, int g, int b);

	// �e�L�X�g��`�悷��
	void draw();

	// �ҏW������������o�b�t�@�ɒ��߂�
	// @param[in] x X���W
	// @param[in] y Y���W
	// @param[in] format �ϒ�����
	void printf(int x, int y, TCHAR* format, ...);
	void printf(TCHAR* format, ...);

	// �f�o�C�X�̏���������
	bool on_lost_device();
	bool on_reset_device();
};

// �B��̃C���X�^���X
extern C_d3d_debug_text G_debug_text;


}