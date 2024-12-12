#include	"tonad3d3_pch.h"
#include	"tonad3d3_device.h"
#include	"tonad3d3_album.h"

#include	"g00.h"

namespace NT3
{

// ****************************************************************
// D3D �A���o��
// ================================================================
C_d3d_album::C_d3d_album()
{
}

C_d3d_album::~C_d3d_album()
{
}


// ****************************************************************
// ���� g00 ��ǂݍ���
// ================================================================

bool C_d3d_album::load_g00_composed(ARRAY<C_d3d_album_compose_param>& param_list)
{
	if (param_list.empty())
		return false;

	int file_cnt = (int)param_list.size();

	// ���ꎞ�o�b�t�@�i�}���`�X���b�h���Ή��j
	static BUFFER file_data;

	// �e�N�X�`���͂P��
	texture_list.resize(1);
	
	// �t�@�C�����[�v
	for (int file_i = 0; file_i < file_cnt; file_i++)	{

		// �t�@�C�������Ȃ��ꍇ
		if (param_list[file_i].file_path.empty())	{
			// �P���ڂȂ��o�b�t�@���쐬
			if (file_i == 0)	{
				texture_list[0] = BSP<C_d3d_texture>(new C_d3d_texture);
				texture_list[0]->create(param_list[file_i].x, param_list[file_i].y);
			}
		}
		// �t�@�C����������ꍇ
		else	{

			// g00 �t�@�C����ǂݍ���
			if (!C_file::read_full_data(param_list[file_i].file_path, file_data))
				return false;

			// g00 ���쐬
			C_g00 g00;
			if (!g00.set_data(file_data.get(), file_data.size()))
				return false;

			// g00 �̏����擾
			S_g00_info g00_info;
			g00.get_info(&g00_info);

			// �J�b�g�ԍ����擾
			int cut_no = param_list[file_i].cut_no;
			cut_no = limit(0, cut_no, g00_info.cut_cnt - 1);

			// �J�b�g���擾
			C_g00_cut* g00_cut = g00.get_cut(cut_no);
			if (g00_cut->is_data())	{

				// �P���ڂȂ�e�N�X�`�����쐬
				if (file_i == 0)
				{
					texture_list[0] = BSP<C_d3d_texture>(new C_d3d_texture);
					if (!texture_list[0]->load_g00_cut(g00_cut, false))
						return false;
				}
				// �Q���ڈȍ~�Ȃ獇��
				else
				{
					if (!texture_list[0]->draw_g00_cut(g00_cut, param_list[file_i].x, param_list[file_i].y, param_list[file_i].blend_type))
						return false;
				}
			}
		}
	}

	return true;
}

// ****************************************************************
// g00 ��ǂݍ���
// ================================================================
bool C_d3d_album::load_g00(CTSTR& file_path, bool futidori)
{
	// ���ꎞ�o�b�t�@�i�}���`�X���b�h���Ή��j
	static BUFFER file_data;

	// g00 �t�@�C����ǂݍ���
	if (!C_file::read_full_data(file_path, file_data))
		return false;

	// g00 ���쐬
	C_g00 g00;
	if (!g00.set_data(file_data.get(), file_data.size()))
		return false;

	// g00 �̏����擾
	S_g00_info g00_info;
	g00.get_info(&g00_info);

	// �J�b�g���̃e�N�X�`�����쐬
	texture_list.resize(g00_info.cut_cnt);

	// �J�b�g�̏���
	for (int cut_no = 0; cut_no < g00_info.cut_cnt; cut_no++)	{

		// �J�b�g���擾
		C_g00_cut* g00_cut = g00.get_cut(cut_no);
		if (g00_cut->is_data())	{

			// �e�N�X�`���ɃJ�b�g��ǂݍ���
			texture_list[cut_no] = BSP<C_d3d_texture>(new C_d3d_texture);
			if (!texture_list[cut_no]->load_g00_cut(g00_cut, futidori))
				return false;
		}
	}

	return true;
}

// ****************************************************************
// �e�N�X�`����ǉ�����
// ================================================================
bool C_d3d_album::add_texture(BSP<C_d3d_texture> texture)
{
	texture_list.push_back(texture);

	return true;
}

// ****************************************************************
// �������
// ================================================================
void C_d3d_album::release()
{
	texture_list.clear();
}

}
