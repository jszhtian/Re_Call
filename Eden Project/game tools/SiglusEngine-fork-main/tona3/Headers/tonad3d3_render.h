#pragma once

#include "tonad3d3_render_interface.h"

namespace NT3
{




class C_d3d_render_default : public C_d3d_render_interface {
public:
	C_d3d_render_default(){};
	~C_d3d_render_default(){};
private:
};



// �Ƃ肠�������ō쐬����P�x�`��p�N���X�Q
// C_d3d_render_default�Ńf�t�H���g(�J���[���)�`����RT��؂�ւ��Ă���g�p�����\��
class C_d3d_render_bright : public C_d3d_render_interface {
public:
	C_d3d_render_bright(){};
	~C_d3d_render_bright(){};
private:
};

// �}���`�����_�[�^�[�Q�b�g�𗘗p���ăf�t�H���g(�J���[���)�`��ƋP�x�`��𓯎��ɍs��
class C_d3d_render_multi_default_bright : public C_d3d_render_interface {
public:
	C_d3d_render_multi_default_bright(){};
	~C_d3d_render_multi_default_bright(){};
private:
};









}