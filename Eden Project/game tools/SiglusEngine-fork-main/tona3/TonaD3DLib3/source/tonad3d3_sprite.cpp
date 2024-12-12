#include	"tonad3d3_pch.h"
#include	"tonad3d3_mesh.h"
#include	"tonad3d3_sprite.h"

namespace NT3
{

// ****************************************************************
// �X�v���C�g�����_�[�p�����[�^
// ================================================================
S_d3d_render_param::S_d3d_render_param()
{
	init();
}

// ****************************************************************
// �X�v���C�g�����_�[�p�����[�^�F������
// ================================================================
void S_d3d_render_param::init()
{
	disp = false;					// �\���t���O
	d2_rect = false;				// �Q�c��`�i�Q�c�̏ꍇ true�j
	d3_rect = false;				// �R�c��`�i�R�c�̏ꍇ true�j
	d3_billboard = false;			// �R�c�r���{�[�h

	size = C_float2(0.0f, 0.0f);	// �傫��
	size_fit_to_texture = -1;		// �傫�����e�N�X�`���ɍ��킹��

	// ���_
	pos.x = 0.0f;					// �ʒu
	pos.y = 0.0f;					// �ʒu
	pos.z = 0.0f;					// �ʒu
	center.x = 0.0f;				// ���S
	center.y = 0.0f;				// ���S
	center.z = 0.0f;				// ���S
	scale.x = 1.0f;					// �g��
	scale.y = 1.0f;					// �g��
	scale.z = 1.0f;					// �g��
	rotate.x = 0.0f;				// ��]
	rotate.y = 0.0f;				// ��]
	rotate.z = 0.0f;				// ��]
	quaternion.x = 0.0f;			// ���N�H�[�^�j�I��
	quaternion.y = 0.0f;			// ���N�H�[�^�j�I��
	quaternion.z = 0.0f;			// ���N�H�[�^�j�I��
	quaternion.w = 1.0f;			// ���N�H�[�^�j�I��
	dst_clip_use = false;			// �N���b�s���O
	dst_clip = C_rect(0, 0, 0, 0);	// �N���b�s���O��`
	src_clip_use = false;			// �N���b�s���O
	src_clip = C_rect(0, 0, 0, 0);	// �N���b�s���O��`

	// �s�N�Z��
	tr = 255;						// �s�����x
	mono = 0;						// ���m�N���x
	reverse = 0;					// ���]�x
	bright = 0;						// ���x
	dark = 0;						// �Óx
	color_r = 0;					// �F�ω�
	color_g = 0;					// �F�ω�
	color_b = 0;					// �F�ω�
	color_rate = 0;					// �F�ω�
	color_add_r = 0;				// �F���Z
	color_add_g = 0;				// �F���Z
	color_add_b = 0;				// �F���Z

	// �s�N�Z�����p
	tone_curve_no = D3D_TONE_CURVE_NONE;	// �g�[���J�[�u�ԍ�
	tone_curve_sat = 0;						// �g�[���J�[�u�ʓx
	light_no = D3D_LIGHT_NONE;				// ���C�g�ԍ�
	mask_x = 0;
	mask_y = 0;

	// �������̑�
	culling = false;						// �J�����O
	alpha_test = false;						// �A���t�@�e�X�g
	alpha_blend = false;					// �A���t�@�u�����h
	use_mask = false;						// �}�X�N
	use_fog = false;						// �t�H�O���g��
	blend_type = E_d3d_blend_type_alpha;	// �u�����h�^�C�v

	// �t�B���^�[
	filter.color = C_argb(255, 255, 255, 255);	// �t�B���^�[�F
}

// ****************************************************************
// D3D �X�v���C�g
// ================================================================
C_d3d_sprite::C_d3d_sprite()
{
	// �o�n�c�ȃf�[�^������������

	// �^�C�v�̏�����
	type = E_d3d_sprite_type_none;

	// �v���~�e�B�u�֘A
	m_vertex_offset = 0;
	m_vertex_cnt = 0;
	m_index_offset = 0;
	m_index_cnt = 0;
	m_primitive_cnt = 0;

	// �G�t�F�N�g�֘A
	m_effect = NULL;
	m_effect_technique = NULL;
	m_effect_constant_f[0][0] = 0.0f;
	m_effect_constant_f[0][1] = 0.0f;
	m_effect_constant_f[0][2] = 0.0f;
	m_effect_constant_f[0][3] = 0.0f;
	m_effect_constant_f[1][0] = 0.0f;
	m_effect_constant_f[1][1] = 0.0f;
	m_effect_constant_f[1][2] = 0.0f;
	m_effect_constant_f[1][3] = 0.0f;
}

C_d3d_sprite::~C_d3d_sprite()
{
	// ���
	release();
}

// ****************************************************************
// �X�v���C�g�̏�����
// ================================================================
void C_d3d_sprite::init()
{
	// ���
	release();

	// ������
	init_no_release();
}

// ****************************************************************
// �X�v���C�g�̏������i������s��Ȃ��j
// ================================================================
void C_d3d_sprite::init_no_release()
{
	// �^�C�v�̏�����
	type = E_d3d_sprite_type_none;

	// �����_�[�p�����[�^
	rp.init();

	// �v���~�e�B�u�֘A
	m_vertex_offset = 0;
	m_vertex_cnt = 0;
	m_index_offset = 0;
	m_index_cnt = 0;
	m_primitive_cnt = 0;

	// �G�t�F�N�g�֘A
	m_effect = NULL;
	m_effect_technique = NULL;
	m_effect_constant_f[0][0] = 0.0f;
	m_effect_constant_f[0][1] = 0.0f;
	m_effect_constant_f[0][2] = 0.0f;
	m_effect_constant_f[0][3] = 0.0f;
	m_effect_constant_f[1][0] = 0.0f;
	m_effect_constant_f[1][1] = 0.0f;
	m_effect_constant_f[1][2] = 0.0f;
	m_effect_constant_f[1][3] = 0.0f;
}

// ****************************************************************
// �X�v���C�g�̉��
// ================================================================
void C_d3d_sprite::release()
{
	// �v���~�e�B�u�֘A
	m_vertex_buffer.reset();
	m_index_buffer.reset();

	// ���b�V���֘A
	m_mesh.reset();
	m_frame_set.reset();

	// �e�N�X�`���֘A
	for (int t = 0; t < D3D_TEXTURE_CNT_MAX; t++)
		m_texture_list[t].reset();
}

// ****************************************************************
// �|���S���X�v���C�g�̍쐬�i���������Ȃ��j
// ================================================================
bool C_d3d_sprite::create_polygon_no_init()
{
	// �����o�̏�����
	type = E_d3d_sprite_type_polygon;

	return true;
}

// ****************************************************************
// ���b�V���X�v���C�g�̍쐬�i���������Ȃ��j
// ================================================================
bool C_d3d_sprite::create_mesh_no_init()
{
	// �����o�̏�����
	type = E_d3d_sprite_type_mesh;

	return true;
}

// ****************************************************************
// �t���[���Z�b�g�X�v���C�g�̍쐬�i���������Ȃ��j
// ================================================================
bool C_d3d_sprite::create_frame_set_no_init()
{
	// �����o�̏�����
	type = E_d3d_sprite_type_frame_set;

	return true;
}

// ****************************************************************
// �ʒu���Q�c���R�c�ɕϊ�
// ================================================================
//		update_d2_vertex_transform �Ŏg����⏕�֐��B
//		�g�k�A��]�A�ړ����v�Z���܂��B
// ================================================================
D3DXVECTOR4 update_d2_vertex_sprite_to_client(D3DXVECTOR4 vertex_2d, const S_d3d_render_param* rp)
{
	// ���S�̃N���C�A���g���W�����߂�
	D3DXVECTOR4 pos((float)(rp->pos.x), (float)(rp->pos.y), 0.0f, 1.0f);

	// ���_�̃N���C�A���g���W�����߂�
	D3DXVECTOR4 vertex(pos.x + vertex_2d.x, pos.y + vertex_2d.y, 0.0f, 1.0f);

	// ���S�����_�ɗ���悤�Ɉړ�����
	vertex.x -= pos.x;
	vertex.y -= pos.y;

	// ��]�p�����[�^
	float yaw   = rp->rotate.x;
	float pitch = rp->rotate.y;
	float roll  = rp->rotate.z;

	// ���[���h���W�ϊ��s����쐬
	D3DXMATRIXA16 mat_world, mat_temp;
	D3DXMatrixIdentity(&mat_world);
	D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixScaling(&mat_temp, rp->scale.x, rp->scale.y, 1.0f));			// �g�k
	D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixRotationYawPitchRoll(&mat_temp, yaw, pitch, roll));			// ��]
//	D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixTranslation(&mat_temp, rp->pos.x, rp->pos.y, rp->pos.z));		// �ړ�

	// ���[���h�ϊ����W��K�p
	D3DXVec4Transform(&vertex, &vertex, &mat_world);

	// �ʒu�����ɖ߂�
	vertex.x += pos.x;
	vertex.y += pos.y;

	return vertex;
}

// ****************************************************************
// �Q�c�X�v���C�g�̐ݒ�
// ================================================================
//		���_�o�b�t�@�̈ʒu��C���f�b�N�X�̈ʒu�A�v���~�e�B�u���Ȃǂ��w�肵�܂��B
//		�܂��Arp ����v�Z���ꂽ�����A���_�o�b�t�@�Ɏ��ۂɏ������݂܂��B
// ================================================================
//		rp �ɗ��炸���W�f�[�^�𒼂Ɏw�肵�����ꍇ�́Ap_vertex_pos ���w�肵�ĉ������B�ʏ�� NULL �ł��B
//		rp �ɗ��炸�e�N�X�`�����W�f�[�^�𒼂Ɏw�肵�����ꍇ�́Ap_texture_uv ���w�肵�ĉ������B�ʏ�� NULL �ł��B
// ================================================================
bool C_d3d_sprite::set_d2_vertex_param(int vertex_offset, int vertex_cnt, int index_offset, int index_cnt, int primitive_cnt
									   , BYTE* p_vertex_buffer, D3DXVECTOR4* p_vertex_pos, D3DXVECTOR2* p_texture_uv)
{
	// ���_�o�b�t�@�̃I�t�Z�b�g���w��
	m_vertex_offset = vertex_offset;
	m_index_offset = index_offset;
	m_vertex_cnt = vertex_cnt;
	m_index_cnt = index_cnt;
	m_primitive_cnt = primitive_cnt;

	// �������ݐ�̒��_�o�b�t�@���w�肳��ĂȂ��Ƃ��͂����ŏI��
	if (!p_vertex_buffer)
		return true;

	// ���_�o�b�t�@���ݒ肳��Ă��Ȃ���Ύ��s�iFVF �Ȃǂ��擾�ł��Ȃ����߁j
	if (!m_vertex_buffer)
		return false;

	// �e���_���������ވʒu���擾����
	DWORD FVF = m_vertex_buffer->get_FVF();
	int FVF_size = m_vertex_buffer->get_FVF_size();
	BYTE* vertex[4] = {(BYTE *)p_vertex_buffer + m_vertex_offset * FVF_size
		, (BYTE *)p_vertex_buffer + (m_vertex_offset + 1) * FVF_size
		, (BYTE *)p_vertex_buffer + (m_vertex_offset + 2) * FVF_size
		, (BYTE *)p_vertex_buffer + (m_vertex_offset + 3) * FVF_size};
	int offset = 0;

	// ���_���W
	D3DXVECTOR4 vertex_pos[4];
	float uv_left = 0.0f, uv_top = 0.0f, uv_right = 1.0f, uv_bottom = 1.0f;

	// �ʒu��񂪂Ȃ��ꍇ�� rp ���瓱���o��
	if (p_vertex_pos == NULL)	{

		// ���S���W�ƃT�C�Y
		C_float2 size = rp.size;
		C_float3 center = rp.center;

		// �傫�����e�N�X�`���T�C�Y�ɍ��킹��
		if (0 <= rp.size_fit_to_texture && rp.size_fit_to_texture < D3D_TEXTURE_CNT_MAX)	{
			if (m_texture_list[rp.size_fit_to_texture])	{
				size.x = (float)m_texture_list[rp.size_fit_to_texture]->get_width();
				size.y = (float)m_texture_list[rp.size_fit_to_texture]->get_height();

				// ���S���W�����킹��
				center.x += m_texture_list[rp.size_fit_to_texture]->get_center().x;
				center.y += m_texture_list[rp.size_fit_to_texture]->get_center().y;
			}
		}

		// �]������`��ݒ肷��
		float src_clip_left = 0.0f - center.x;
		float src_clip_top = 0.0f - center.y;
		float src_clip_right = size.x - center.x;
		float src_clip_bottom = size.y - center.y;

		// �]������`�̕␳���s��
		if (rp.src_clip_use)	{

			src_clip_left = std::max(src_clip_left, (float)rp.src_clip.left);
			src_clip_top = std::max(src_clip_top, (float)rp.src_clip.top);
			src_clip_right = std::min(src_clip_right, (float)rp.src_clip.right);
			src_clip_bottom = std::min(src_clip_bottom, (float)rp.src_clip.bottom);

			// ��������e�N�X�`�����W��␳����
			uv_left = (src_clip_left + center.x) / size.x;
			uv_top = (src_clip_top + center.y) / size.y;
			uv_right = (src_clip_right + center.x) / size.x;
			uv_bottom = (src_clip_bottom + center.y) / size.y;
		}

		// ��]���s��Ȃ��ꍇ�i�R�c�̏ꍇ�͒��_�V�F�[�_�ŉ�]���s���j
		if (fabs(rp.rotate.z) < FLT_EPSILON)	{

			// �Q�c�̏ꍇ
			if ((FVF & D3DFVF_POSITION_MASK) == D3DFVF_XYZRHW)	{

				// ����ƉE���������v�Z����
				vertex_pos[0] = D3DXVECTOR4(src_clip_left, src_clip_top, 0.0f - center.z, 1.0f);
				vertex_pos[3] = D3DXVECTOR4(src_clip_right, src_clip_bottom, 0.0f - center.z, 1.0f);

				// �g�k��K�p����
				vertex_pos[0] = update_d2_vertex_sprite_to_client(vertex_pos[0], &rp);
				vertex_pos[3] = update_d2_vertex_sprite_to_client(vertex_pos[3], &rp);

				// �E��ƍ����ɂ��R�s�[����
				vertex_pos[1].x = vertex_pos[3].x;
				vertex_pos[1].y = vertex_pos[0].y;
				vertex_pos[2].x = vertex_pos[0].x;
				vertex_pos[2].y = vertex_pos[3].y;
			}
			// �R�c�̏ꍇ
			else	{

				// �S���_���v�Z����
				vertex_pos[0] = D3DXVECTOR4(src_clip_left, src_clip_top, 0.0f - center.z, 1.0f);
				vertex_pos[1] = D3DXVECTOR4(src_clip_right, src_clip_top, 0.0f - center.z, 1.0f);
				vertex_pos[2] = D3DXVECTOR4(src_clip_left,src_clip_bottom, 0.0f - center.z, 1.0f);
				vertex_pos[3] = D3DXVECTOR4(src_clip_right, src_clip_bottom, 0.0f - center.z, 1.0f);
			}
		}
		// ��]���s���ꍇ
		else	{

			// �S���_���v�Z����
			vertex_pos[0] = D3DXVECTOR4(src_clip_left, src_clip_top, 0.0f - center.z, 1.0f);
			vertex_pos[1] = D3DXVECTOR4(src_clip_right, src_clip_top, 0.0f - center.z, 1.0f);
			vertex_pos[2] = D3DXVECTOR4(src_clip_left, src_clip_bottom, 0.0f - center.z, 1.0f);
			vertex_pos[3] = D3DXVECTOR4(src_clip_right, src_clip_bottom, 0.0f - center.z, 1.0f);

			// �Q�c�̏ꍇ�͕ό`
			if ((FVF & D3DFVF_POSITION_MASK) == D3DFVF_XYZRHW)	{
				vertex_pos[0] = update_d2_vertex_sprite_to_client(vertex_pos[0], &rp);
				vertex_pos[1] = update_d2_vertex_sprite_to_client(vertex_pos[1], &rp);
				vertex_pos[2] = update_d2_vertex_sprite_to_client(vertex_pos[2], &rp);
				vertex_pos[3] = update_d2_vertex_sprite_to_client(vertex_pos[3], &rp);
			}
		}

		// xy ���W�� 0.5px ����ɂ��炷�B����d�v�B�Ȃ���ΐ������ɂڂ��Ă��܂��B
		// z ���W�͎g��Ȃ��̂œK���� 0.5f �ɂ��Ă����B�i0.0f �� 1.0f �ł͕\������Ȃ��O���{������j
		if ((FVF & D3DFVF_POSITION_MASK) == D3DFVF_XYZRHW)	{
			vertex_pos[0].x -= 0.5f;	vertex_pos[0].y -= 0.5f;	vertex_pos[0].z = 0.5f;		
			vertex_pos[1].x -= 0.5f;	vertex_pos[1].y -= 0.5f;	vertex_pos[1].z = 0.5f;		
			vertex_pos[2].x -= 0.5f;	vertex_pos[2].y -= 0.5f;	vertex_pos[2].z = 0.5f;		
			vertex_pos[3].x -= 0.5f;	vertex_pos[3].y -= 0.5f;	vertex_pos[3].z = 0.5f;		
		}

		// w �� 1.0 �Œ�
		vertex_pos[0].w = 1.0f;
		vertex_pos[1].w = 1.0f;
		vertex_pos[2].w = 1.0f;
		vertex_pos[3].w = 1.0f;

		// ��������_�����Z�b�g����
		p_vertex_pos = vertex_pos;
	}

	// �ʒu
	if ((FVF & D3DFVF_POSITION_MASK) == D3DFVF_XYZRHW)	{

		*(D3DXVECTOR3 *)(vertex[0] + offset) = D3DXVECTOR3(p_vertex_pos[0].x, p_vertex_pos[0].y, p_vertex_pos[0].z);
		*(D3DXVECTOR3 *)(vertex[1] + offset) = D3DXVECTOR3(p_vertex_pos[1].x, p_vertex_pos[1].y, p_vertex_pos[1].z);
		*(D3DXVECTOR3 *)(vertex[2] + offset) = D3DXVECTOR3(p_vertex_pos[2].x, p_vertex_pos[2].y, p_vertex_pos[2].z);
		*(D3DXVECTOR3 *)(vertex[3] + offset) = D3DXVECTOR3(p_vertex_pos[3].x, p_vertex_pos[3].y, p_vertex_pos[3].z);
		offset += sizeof(D3DXVECTOR3);
		*(FLOAT *)(vertex[0] + offset) = p_vertex_pos[0].w;
		*(FLOAT *)(vertex[1] + offset) = p_vertex_pos[1].w;
		*(FLOAT *)(vertex[2] + offset) = p_vertex_pos[2].w;
		*(FLOAT *)(vertex[3] + offset) = p_vertex_pos[3].w;
		offset += sizeof(FLOAT);
	}
	else if ((FVF & D3DFVF_POSITION_MASK) == D3DFVF_XYZ)	{

		*(D3DXVECTOR3 *)(vertex[0] + offset) = D3DXVECTOR3(p_vertex_pos[0].x, p_vertex_pos[0].y, p_vertex_pos[0].z);
		*(D3DXVECTOR3 *)(vertex[1] + offset) = D3DXVECTOR3(p_vertex_pos[1].x, p_vertex_pos[1].y, p_vertex_pos[1].z);
		*(D3DXVECTOR3 *)(vertex[2] + offset) = D3DXVECTOR3(p_vertex_pos[2].x, p_vertex_pos[2].y, p_vertex_pos[2].z);
		*(D3DXVECTOR3 *)(vertex[3] + offset) = D3DXVECTOR3(p_vertex_pos[3].x, p_vertex_pos[3].y, p_vertex_pos[3].z);
		offset += sizeof(D3DXVECTOR3);
	}

	// �@��
	if (FVF & D3DFVF_NORMAL)	{

		*(D3DXVECTOR3 *)(vertex[0] + offset) = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		*(D3DXVECTOR3 *)(vertex[1] + offset) = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		*(D3DXVECTOR3 *)(vertex[2] + offset) = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		*(D3DXVECTOR3 *)(vertex[3] + offset) = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		offset += sizeof(D3DXVECTOR3);
	}

	// �f�B�t���[�Y�F
	if (FVF & D3DFVF_DIFFUSE)	{
		*(D3DCOLOR *)(vertex[0] + offset) = D3DCOLOR_ARGB(rp.tr * rp.filter.color.a / 255, rp.filter.color.r, rp.filter.color.g, rp.filter.color.b);
		*(D3DCOLOR *)(vertex[1] + offset) = D3DCOLOR_ARGB(rp.tr * rp.filter.color.a / 255, rp.filter.color.r, rp.filter.color.g, rp.filter.color.b);
		*(D3DCOLOR *)(vertex[2] + offset) = D3DCOLOR_ARGB(rp.tr * rp.filter.color.a / 255, rp.filter.color.r, rp.filter.color.g, rp.filter.color.b);
		*(D3DCOLOR *)(vertex[3] + offset) = D3DCOLOR_ARGB(rp.tr * rp.filter.color.a / 255, rp.filter.color.r, rp.filter.color.g, rp.filter.color.b);
		offset += sizeof(D3DCOLOR);
	}

	// �e�N�X�`���i�Q���܂őΉ��j
	int tex_cnt = 0;
	if (false);
	else if ((FVF & D3DFVF_TEXCOUNT_MASK) == D3DFVF_TEX1)	tex_cnt = 1;
	else if ((FVF & D3DFVF_TEXCOUNT_MASK) == D3DFVF_TEX2)	tex_cnt = 2;
	//else if ((FVF & D3DFVF_TEXCOUNT_MASK) == D3DFVF_TEX3)	tex_cnt = 3;
	//else if ((FVF & D3DFVF_TEXCOUNT_MASK) == D3DFVF_TEX4)	tex_cnt = 4;
	//else if ((FVF & D3DFVF_TEXCOUNT_MASK) == D3DFVF_TEX5)	tex_cnt = 5;
	//else if ((FVF & D3DFVF_TEXCOUNT_MASK) == D3DFVF_TEX6)	tex_cnt = 6;
	//else if ((FVF & D3DFVF_TEXCOUNT_MASK) == D3DFVF_TEX7)	tex_cnt = 7;
	//else if ((FVF & D3DFVF_TEXCOUNT_MASK) == D3DFVF_TEX8)	tex_cnt = 8;

	// �f�t�H���g�ݒ���s���e�N�X�`���̌�
	int default_set_tex_cnt = tex_cnt;
	if ((FVF & D3DFVF_POSITION_MASK) == D3DFVF_XYZRHW && rp.use_mask)	{
		default_set_tex_cnt = 1;	// �e�N�X�`���P�Ԃ��}�X�N�Ƃ��Ďg���ꍇ�͂O�Ԃ̂݃f�t�H���g�ݒ�
	}

	// �e�N�X�`���̃f�t�H���g�ݒ���܂��s��
	for (int t = 0; t < default_set_tex_cnt; t++)	{
		D3DXVECTOR2 texture_uv[4];

		// p_texture_uv ���w�肳��Ă���ꍇ�͗^����ꂽ�l���g��
		if (p_texture_uv)	{

			// �e�N�X�`�����W�����߂�
			if (m_texture_list[t])	{

				// �e�N�X�`���̎��̈���l���ăe�N�X�`�����W���v�Z����
				// �Ⴆ�΃e�N�X�`������ 800 �̏ꍇ�A���ۂ� 1024 �ɕ␳����Ă���\��������܂��B
				// ���̏ꍇ�A�e�N�X�`�����W�� 800/1024 �{����K�v������̂ł��B
				float x_rep = (float)m_texture_list[t]->get_width() / (float)m_texture_list[t]->get_width_ex();
				float y_rep = (float)m_texture_list[t]->get_height() / (float)m_texture_list[t]->get_height_ex();
				texture_uv[0].x = p_texture_uv[0].x * x_rep;
				texture_uv[0].y = p_texture_uv[0].y * y_rep;
				texture_uv[1].x = p_texture_uv[1].x * x_rep;
				texture_uv[1].y = p_texture_uv[1].y * y_rep;
				texture_uv[2].x = p_texture_uv[2].x * x_rep;
				texture_uv[2].y = p_texture_uv[2].y * y_rep;
				texture_uv[3].x = p_texture_uv[3].x * x_rep;
				texture_uv[3].y = p_texture_uv[3].y * y_rep;
			}
		}

		// p_texture_uv ���w�肳��Ă��Ȃ��ꍇ�͌v�Z�����l���g��
		else	{

			// �e�N�X�`���̎��̈���l���ăe�N�X�`�����W���v�Z����
			// �Ⴆ�΃e�N�X�`������ 800 �̏ꍇ�A���ۂ� 1024 �ɕ␳����Ă���\��������܂��B
			// ���̏ꍇ�A�e�N�X�`�����W�� 800/1024 �{����K�v������̂ł��B
			float uv_l = uv_left, uv_t = uv_top, uv_r = uv_right, uv_b = uv_bottom;
			if (m_texture_list[t])	{
				float x_rep = (float)m_texture_list[t]->get_width() / (float)m_texture_list[t]->get_width_ex();
				float y_rep = (float)m_texture_list[t]->get_height() / (float)m_texture_list[t]->get_height_ex();
				uv_l = uv_l * x_rep;
				uv_t = uv_t * y_rep;
				uv_r = uv_r * x_rep;
				uv_b = uv_b * y_rep;
			}

			texture_uv[0] = D3DXVECTOR2(uv_l, uv_t);
			texture_uv[1] = D3DXVECTOR2(uv_r, uv_t);
			texture_uv[2] = D3DXVECTOR2(uv_l, uv_b);
			texture_uv[3] = D3DXVECTOR2(uv_r, uv_b);
		}

		*(D3DXVECTOR2 *)(vertex[0] + offset) = texture_uv[0];
		*(D3DXVECTOR2 *)(vertex[1] + offset) = texture_uv[1];
		*(D3DXVECTOR2 *)(vertex[2] + offset) = texture_uv[2];
		*(D3DXVECTOR2 *)(vertex[3] + offset) = texture_uv[3];
		offset += sizeof(D3DXVECTOR2);
	}

	// �e�N�X�`���P�Ԃ��}�X�N�Ƃ��Ďg���ꍇ
	if ((FVF & D3DFVF_POSITION_MASK) == D3DFVF_XYZRHW && rp.use_mask)	{
		C_d3d_texture* p_mask_texture = m_texture_list[1].get();
		float center_x = (float)p_mask_texture->get_center().x;
		float center_y = (float)p_mask_texture->get_center().y;
		float width_ex = (float)p_mask_texture->get_width_ex();
		float height_ex = (float)p_mask_texture->get_height_ex();
		for (int i = 0; i < 4; i++)	{
			float u = linear(p_vertex_pos[i].x + 0.5f - rp.mask_x, - center_x, 0.0f, - center_x + width_ex, 1.0f);
			float v = linear(p_vertex_pos[i].y + 0.5f - rp.mask_y, - center_y, 0.0f, - center_y + height_ex, 1.0f);
			*(D3DXVECTOR2 *)(vertex[i] + offset) = D3DXVECTOR2(u, v);
		}
		offset += sizeof(D3DXVECTOR2);
	}

	return true;
}


// ****************************************************************
// �A�j���[�V������i�߂�
// ================================================================
//void C_d3d_sprite::advance_animation(double past_time)
//{
//	if (m_frame_set)
//		m_frame_set->advance_animation(past_time);
//}

// ****************************************************************
// �e�N�X�`�����N���A
// ================================================================
void C_d3d_sprite::clear_texture(int stage)
{
	if (0 <= stage && stage < D3D_TEXTURE_CNT_MAX)
		m_texture_list[stage].reset();
}

// ****************************************************************
// �e�N�X�`����ݒ�
// ================================================================
void C_d3d_sprite::set_texture(BSP<C_d3d_texture> texture, int stage)
{
	if (0 <= stage && stage < D3D_TEXTURE_CNT_MAX)
		m_texture_list[stage] = texture;
}

// ****************************************************************
// �e�N�X�`�����擾
// ================================================================
BSP<C_d3d_texture> C_d3d_sprite::get_texture(int stage)
{
	if (0 <= stage && stage < D3D_TEXTURE_CNT_MAX)
		return m_texture_list[stage];

	return BSP<C_d3d_texture>();
}

// ****************************************************************
// �G�t�F�N�g�̐ݒ�Ǝ擾
// ================================================================
void C_d3d_sprite::set_effect(ID3DXEffect* effect)
{
	m_effect = effect;
}

LPD3DXEFFECT C_d3d_sprite::get_effect()
{
	return m_effect;
}

// ****************************************************************
// �G�t�F�N�g�̃e�N�j�b�N�̐ݒ�Ǝ擾
// ================================================================
void C_d3d_sprite::set_effect_technique(D3DXHANDLE technique)
{
	m_effect_technique = technique;
}

D3DXHANDLE C_d3d_sprite::get_effect_technique()
{
	return m_effect_technique;
}

// ****************************************************************
// �G�t�F�N�g�̒萔�̐ݒ�Ǝ擾
// ================================================================
void C_d3d_sprite::set_effect_constant_f(int idx, float f0, float f1, float f2, float f3)
{
	m_effect_constant_f[idx][0] = f0;
	m_effect_constant_f[idx][1] = f1;
	m_effect_constant_f[idx][2] = f2;
	m_effect_constant_f[idx][3] = f3;
}

float* C_d3d_sprite::get_effect_constant_f(int idx)
{
	return m_effect_constant_f[idx];
}



}
