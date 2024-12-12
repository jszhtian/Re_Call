#include	"tonad3d3_pch.h"
#include	"tonad3d3_d3d.h"
#include	"tonad3d3_mesh.h"


namespace NT3
{

	// ****************************************************************
	// D3D �}�e���A��
	// ================================================================
	C_d3d_mtrl::C_d3d_mtrl()
	{
		ZeroMemory(&mtrl, sizeof(mtrl));	// �}�e���A��
		disp = true;						// �\���t���O
		alpha_blend = false;				// �A���t�@�u�����h
		texture = BSP<C_d3d_texture>();
	}

	// ****************************************************************
	// D3D ���b�V���̃p�����[�^
	// ================================================================
	S_d3d_mesh_param::S_d3d_mesh_param()
	{
		effect_old_key.clear();
		p_effect = NULL;
		shadow_map_effect_old_key.clear();
		p_shadow_map_effect = NULL;

		lighting_type	= (LIGHTING_TYPE)9;		// NONE = 0
		shading_type	= (SHADING_TYPE)0;		// NONE = 0
		shader_option	= SHADER_OPTION_NONE;
		//shader_option	= SHADER_OPTION_RIM_LIGHT;		// �����������Ή�

		//pre_render_type = NONE;
		pre_render_type = SHADOW_MAP;

		use_mesh_tex = true;

		use_mrbd = false;
		mrbd = D3DXVECTOR4(0, 0, 0, 0);
		use_rgb = false;
		rgb_rate = D3DXVECTOR4(0, 0, 0, 0);
		add_rgb = D3DXVECTOR4(0, 0, 0, 0);

		use_mul_vertex_color = false;
		mul_vertex_color_rate = 1.0f;

		depth_buffer_shadow_bias = 0.03f;

		rim_light_color = D3DXVECTOR4(1, 1, 1, 1);
		//rim_light_power = 0.5f;
		rim_light_power = 1.0f;


		toon_tex = BSP<C_d3d_texture>();
		normal_tex = BSP<C_d3d_texture>();

		directional_light_id_list.clear();
		directional_light_id_list.push_back(10);
		//directional_light_id_list.push_back(11);

		point_light_id_list.clear();
		point_light_id_list.push_back(20);


		spot_light_id_list.clear();
		spot_light_id_list.push_back(30);
	}

	// ****************************************************************
	// D3D ���b�V��
	// ================================================================
	C_d3d_mesh::C_d3d_mesh()
	{
		init();
	}

	C_d3d_mesh::C_d3d_mesh(BIP<ID3DXMesh> mesh_)
	{
		init();

		mesh = mesh_;
	}

	C_d3d_mesh::~C_d3d_mesh()
	{
		release();
	}

	// ****************************************************************
	// ���b�V����������
	// ================================================================
	void C_d3d_mesh::init()
	{
		size.x = 0.0f;
		size.y = 0.0f;
		size.z = 0.0f;

		release();
	}

	// ****************************************************************
	// ���b�V�������
	// ================================================================
	void C_d3d_mesh::release()
	{
		mesh = NULL;
	}

	// ****************************************************************
	// �w�t�@�C�����烁�b�V����ǂݍ���
	// ================================================================
	bool C_d3d_mesh::load_x(CTSTR& file_path, DWORD FVF, ARRAY<TSTR>& texture_name_list)
	{
		HRESULT hr;

		init();

		ID3DXMesh* p_mesh = NULL;
		ID3DXBuffer* p_mtrl_buffer = NULL;
		int mtrl_cnt = 0;

		// �K�w�Ȃ��̂w�t�@�C����ǂݍ���
		hr = D3DXLoadMeshFromX(file_path.c_str(), D3DXMESH_MANAGED, G_d3d.device.body(), NULL, &p_mtrl_buffer, NULL, (DWORD *)&mtrl_cnt, &p_mesh);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("���b�V���̍쐬�Ɏ��s���܂����B"), _T("LoadMesh"), hr);
			return false;
		}

		mesh = BIP<ID3DXMesh>(p_mesh, false);
		BIP<ID3DXBuffer> mtrl_buffer(p_mtrl_buffer, false);

		// �w��� FVF �ɂ��킹�ă��b�V����蒼��
		hr = mesh->CloneMeshFVF(mesh->GetOptions(), FVF, G_d3d.device.body(), &p_mesh);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("���b�V���̕����Ɏ��s���܂����B"), _T("CloneMeshFVF"), hr);
			return false;
		}

		mesh = BIP<ID3DXMesh>(p_mesh, false);

		// �}�e���A�����ւ̃|�C���^���擾
		D3DXMATERIAL* mtrl = (D3DXMATERIAL *)mtrl_buffer->GetBufferPointer();

		// �}�e���A�����쐬
		mtrl_list.resize(mtrl_cnt);
		texture_name_list.resize(mtrl_cnt);

		// �}�e���A�����̃R�s�[
		for (int m = 0; m < mtrl_cnt; m++)	{
			mtrl_list[m].mtrl = mtrl[m].MatD3D;
			mtrl_list[m].mtrl.Ambient = mtrl_list[m].mtrl.Diffuse;
				// �A���r�G���g�͂w�t�@�C���Œ�`�ł��Ȃ��H�f�B�t���[�Y�F���R�s�[���Ă����܂��B
				// ���̃T���v�����������Ƃ��Ă���ۂ����炠���Ă�Ǝv���܂��B
				// �X�y�L�����[�Ƃ��͂ǂ����悤�B

			// �e�N�X�`�����̐ݒ�
			if (mtrl[m].pTextureFilename)	{
				// �e�N�X�`�����̓}���`�o�C�g�݂����Ȃ̂ŁATSTR �ɕϊ�
				TSTR texture_name = MBSTR_to_TSTR(mtrl[m].pTextureFilename);
				texture_name_list[m] = texture_name;
			}
		}

		// ���b�V���̑傫�����擾
		IDirect3DVertexBuffer9* p_vertex = NULL;
		D3DXVECTOR3* p_vertex_buffer = NULL;
		D3DXVECTOR3 v_min, v_max;

		// ���_�o�b�t�@���擾
		mesh->GetVertexBuffer(&p_vertex);
		BIP<IDirect3DVertexBuffer9> vertex(p_vertex, false);

		// ���_�o�b�t�@�����b�N
		if (FAILED(hr = vertex->Lock(0, 0, (void **)&p_vertex_buffer, 0)))
			return false;

		// ���b�V���̑傫�����擾
		::D3DXComputeBoundingBox(p_vertex_buffer, mesh->GetNumVertices(), D3DXGetFVFVertexSize(mesh->GetFVF()), &v_min, &v_max);
		size = v_max - v_min;

		// ���_�o�b�t�@���A�����b�N
		p_vertex->Unlock();

		return true;
	}

	// ****************************************************************
	// �}�e���A����ǉ�����
	// ================================================================
	bool C_d3d_mesh::add_material(C_d3d_mtrl mtrl)
	{
		mtrl_list.push_back(mtrl);

		return true;
	}

	// ****************************************************************
	// �}�e���A���Ƀe�N�X�`����ݒ肷��
	// ================================================================
	bool C_d3d_mesh::set_texture(int mtrl_no, BSP<C_d3d_texture> texture)
	{
		if (mtrl_no < 0 || (int)mtrl_list.size() <= mtrl_no)
			return false;

		mtrl_list[mtrl_no].texture = texture;

		return true;
	}

	bool C_d3d_mesh::set_toon_texture(BSP<C_d3d_texture> texture)
	{
		m_param.toon_tex = texture;
		return true;
	}
	bool C_d3d_mesh::set_normal_texture(BSP<C_d3d_texture> texture)
	{
		m_param.normal_tex = texture;
		return true;
	}
}
