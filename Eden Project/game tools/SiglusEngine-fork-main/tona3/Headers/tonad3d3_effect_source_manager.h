#pragma once

#include	"tonad3d3_d3d.h"
#include	"tonad3d3_mesh.h"

namespace NT3
{


class C_d3d_effect_data;
class C_d3d_effect_source_manager;

typedef std::map< TSTR, BSP<C_d3d_effect_data> > SHADER_MAP;

// �f�[�^(ID3DXEffect�x�^�����ł�OK�����Ǌg�����̎����l���ăN���X��)
class C_d3d_effect_data {
	friend class C_d3d_effect_source_manager;
private:
	BIP<ID3DXEffect>	m_effect;
	// ��������p�̃J�E���^�ϐ�etc
	// key(�폜�p)
public:
	C_d3d_effect_data();
	~C_d3d_effect_data();
	ID3DXEffect* get_effect();

};


// �}�l�[�W��
class C_d3d_effect_source_manager {
public:
	C_d3d_effect_source_manager();
	~C_d3d_effect_source_manager();

	// ����(���sNULL)
	C_d3d_effect_data* create_effect_source(CTSTR& key, CMBSTR& data);
	// ����(���sNULL)
	C_d3d_effect_data* search_effect_source(CTSTR& key);

	// ���
	void destroy_effect_source(CTSTR& key);
	// �S���
	void destroy_effect_source_all();


	// �f�o�b�O�p�H�J���p�H
	// �t�@�C������ǂݍ��݁A������}�l�[�W���ɓo�^����
	C_d3d_effect_data* create_effect_source(CTSTR& key);

private:
	SHADER_MAP m_ShaderMap;
};

// �B��̃C���X�^���X
extern C_d3d_effect_source_manager G_d3d_effect_source_manager;



// �w���p�[
ID3DXEffect* create_effect_for_mesh_from_file(CTSTR& file_path, C_d3d_mesh* p_mesh);
ID3DXEffect* create_effect_for_mesh_from_mesh(C_d3d_mesh* p_mesh, C_d3d_mtrl* p_mtrl, ID3DXSkinInfo* p_skin_info);
ID3DXEffect* create_effect_for_shadow_map_from_mesh(C_d3d_mesh* p_mesh, C_d3d_mtrl* p_mtrl, ID3DXSkinInfo* p_skin_info);


}