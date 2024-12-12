#include	"tonagraphic3_pch.h"
#include	"tonagraphic3_thumb.h"

namespace NT3
{

// ****************************************************************
// �C���[�W���X�g
// ================================================================
Cimage_list::Cimage_list()
{
	h_il = NULL;
}

Cimage_list::~Cimage_list()
{
	destroy();
}

// ****************************************************************
// �C���[�W���X�g�F�쐬
// ================================================================
bool Cimage_list::create(int width, int height)
{
	destroy();

	// �C���[�W���X�g���쐬�i������ 32�A������ 8�j
	h_il = ImageList_Create(width, height, ILC_COLOR32, 32, 8);
	if (h_il == NULL)
		return false;

	return true;
}

// ****************************************************************
// �C���[�W���X�g�F�j��
// ================================================================
void Cimage_list::destroy()
{
	if (h_il)	{
		ImageList_Destroy(h_il);
		h_il = NULL;
	}
}

// ****************************************************************
// �C���[�W���X�g�F�C���[�W��ǉ�
// ================================================================
bool Cimage_list::add_image(HBITMAP h_bitmap)
{
	if (!h_il)
		return false;

	ImageList_Add(h_il, h_bitmap, NULL);

	return true;
}

}