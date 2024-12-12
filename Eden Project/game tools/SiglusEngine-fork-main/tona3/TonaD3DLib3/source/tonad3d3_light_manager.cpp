#include "tonad3d3_pch.h"
#include "tonad3d3_light_manager.h"

namespace NT3
{

// �B��̃C���X�^���X
C_d3d_light_manager G_d3d_light_manager;

const int C_d3d_light_manager::WORLD_LIGHT_MAX = 128;			// ���E�ɔz�u�ł��郉�C�g�̍ő吔
const int C_d3d_light_manager::OBJ_DIRECTIONAL_LIGHT_MAX = 4;	// �I�u�W�F�N�g�Ɏg�p�ł��郉�C�g�̍ő吔
const int C_d3d_light_manager::OBJ_POINT_LIGHT_MAX = 4;			// �I�u�W�F�N�g�Ɏg�p�ł��郉�C�g�̍ő吔
const int C_d3d_light_manager::OBJ_SPOT_LIGHT_MAX = 4;			// �I�u�W�F�N�g�Ɏg�p�ł��郉�C�g�̍ő吔



C_d3d_light_manager::C_d3d_light_manager()
{
	m_light_list.clear();
	m_light_list.reserve(WORLD_LIGHT_MAX);
	m_light_list.resize(WORLD_LIGHT_MAX);
}

C_d3d_light_manager::~C_d3d_light_manager()
{
	remove_all();
}

BSP<C_d3d_light_interface> C_d3d_light_manager::create(int id, LIGHT_TYPE type)
{
	// NULL���ǂ�
	BSP<C_d3d_light_interface> light = BSP<C_d3d_light_interface>();

	// ID = �z��C���f�b�N�X�Ȃ̂ōő吔���z����ꍇ�̓G���[�Ƃ��ď���
	if(id >= WORLD_LIGHT_MAX) {
		set_last_error(_T("���C�g�̐����Ɏ��s���܂����B"), _T("create"));
		return BSP<C_d3d_light_interface>();
	}

	if(false);

	else if(type == LIGHT_TYPE_DIRECTIONAL) {
		light = m_creators.directional.create();
	}
	else if(type == LIGHT_TYPE_POINT) {
		light = m_creators.point.create();
	}
	else if(type == LIGHT_TYPE_SPOT) {
		light = m_creators.spot.create();
	}
	else {
		// �G���[�����H
	}

	// �������琶������Ă����
	if(light) {
		// �o�^����
		if(!regist(id, light)) {
			// �o�^�Ɏ��s�����ꍇ�͉��
			light = BSP<C_d3d_light_interface>();
		}
		else {
			// �o�^�ɐ����������_�Ń��C�g�ɂ�ID��o�^����
			light->m_id = id;
		}
	}

	return light;
}

BSP<C_d3d_light_interface> C_d3d_light_manager::search(int id)
{
	return m_light_list[id];
}
bool C_d3d_light_manager::remove(int id)
{
	m_light_list[id] = BSP<C_d3d_light_interface>();
	return true;
}
bool C_d3d_light_manager::remove_all()
{
	m_light_list.clear();
	return true;
}
bool C_d3d_light_manager::regist(int id, BSP<C_d3d_light_interface> obj)
{
	// �I�u�W�F�N�g���Ȃ������玸�s
	if(!obj)
		return false;

	m_light_list[id] = obj;
	return true;
}













}
