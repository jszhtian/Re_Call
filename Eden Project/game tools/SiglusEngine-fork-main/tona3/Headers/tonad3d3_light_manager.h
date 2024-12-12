#pragma once

#include "tonad3d3_light.h"

namespace NT3
{


// ���C�g�����҂̃C���^�[�t�F�C�X
class C_d3d_light_creator_interface {
protected:
	BSP<C_d3d_light_interface>	create_instance() {
		return create();
	}
	C_d3d_light_creator_interface(){}
public:
	virtual ~C_d3d_light_creator_interface(){}
	virtual BSP<C_d3d_light_interface>	create() = 0;
};

class C_d3d_directional_light_creator : public C_d3d_light_creator_interface {
public:
	BSP<C_d3d_light_interface>	create() {
		return BSP<C_d3d_directional_light>(new C_d3d_directional_light);
	}
};
class C_d3d_point_light_creator : public C_d3d_light_creator_interface {
public:
	BSP<C_d3d_light_interface>	create() {
		return BSP<C_d3d_point_light>(new C_d3d_point_light);
	}
};
class C_d3d_spot_light_creator : public C_d3d_light_creator_interface {
public:
	BSP<C_d3d_light_interface>	create() {
		return BSP<C_d3d_spot_light>(new C_d3d_spot_light);
	}
};


// ���C�g�����҂����̍\����
struct S_d3d_light_creators {
	C_d3d_directional_light_creator	directional;
	C_d3d_point_light_creator		point;
	C_d3d_spot_light_creator		spot;
};



// ���ɂ���Ă��Ⴄ����300���炢�܂ł̃f�[�^�Ȃ�std::map���z��ŊǗ������������
// id�ƃ��X�g�̃C���f�b�N�X�͑Ή�������
// ���C�g�Ǘ��҃N���X
class C_d3d_light_manager {
public:
	static const int WORLD_LIGHT_MAX;				// ���E�ɔz�u�ł��郉�C�g�̍ő吔
	static const int OBJ_DIRECTIONAL_LIGHT_MAX;		// �I�u�W�F�N�g�Ɏg�p�ł��郉�C�g�̍ő吔
	static const int OBJ_POINT_LIGHT_MAX;			// �I�u�W�F�N�g�Ɏg�p�ł��郉�C�g�̍ő吔
	static const int OBJ_SPOT_LIGHT_MAX;			// �I�u�W�F�N�g�Ɏg�p�ł��郉�C�g�̍ő吔
public:
	C_d3d_light_manager();
	~C_d3d_light_manager();


	//BSP<C_d3d_light_interface> regist(int id, BSP<C_d3d_light_interface> light);

	BSP<C_d3d_light_interface> create(int id, LIGHT_TYPE type);
	BSP<C_d3d_light_interface> search(int id);
	bool remove(int id);
	bool remove_all();
private:
	bool regist(int id, BSP<C_d3d_light_interface> obj);
private:
	// �A�v���Ŏg�p���郉�C�g�̃��X�g(�d��ID�o�^�s��)
	ARRAY< BSP<C_d3d_light_interface> >		m_light_list;

	// ���C�g�����҂����̍\����
	S_d3d_light_creators					m_creators;
};
// �B��̃C���X�^���X
extern C_d3d_light_manager G_d3d_light_manager;






}