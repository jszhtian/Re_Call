#include	"tonagraphic3_pch.h"
#include	"tonagraphic3_album.h"

namespace NT3
{

// ****************************************************************
// �A���o��
// ================================================================
C_album::C_album()
{
}

C_album::~C_album()
{
}

// ****************************************************************
// �A���o����j��
// ================================================================
void C_album::destroy()
{
	pct_list.clear();
}

// ****************************************************************
// �A���o�����쐬
// ================================================================
bool C_album::create(ARRAY< BSP<C_pct> > pct_list_)
{
	destroy();

	pct_list = pct_list_;

	return true;
}

// ****************************************************************
// �f�B�u�ɕ`��
// ================================================================
void C_album::draw_to_dib(C_dib* dib, int pct_no, int x, int y, bool sprite)
{
	S_disp_param dp;
	dp.pos.x = x;
	dp.pos.y = y;
	dp.sprite = sprite;

	draw_to_dib(dib, pct_no, &dp);
}

void C_album::draw_to_dib(C_dib* dib, int pct_no, S_disp_param* dp)
{
	draw_to_dib(dib, pct_no, dp, true, NULL);
}

void C_album::draw_to_dib(C_dib* dib, int pct_no, S_disp_param* dp, bool draw_flag, ARRAY<C_rect>* ret_draw_rect_list)
{
	// �s�N�`����`��
	if (0 <= pct_no && pct_no < (int)pct_list.size())
		pct_list[pct_no]->draw_to_dib(dib, dp, draw_flag, ret_draw_rect_list);
}

// ****************************************************************
// �f�B�u�ɕ`��
// ================================================================
bool C_album::expand_chips()
{
	for (int i = 0; i < (int)pct_list.size(); i++)	{
		if (!pct_list[i]->expand_chips())
			return false;
	}

	return true;
}

// ****************************************************************
// �s�N�`�����擾
// ================================================================
BSP<C_pct> C_album::get_pct(int pct_no)
{
	if (0 <= pct_no && pct_no < (int)pct_list.size())
		return pct_list[pct_no];

	return BSP<C_pct>();
}

// ****************************************************************
// �s�N�`�������擾
// ================================================================
int C_album::get_pct_cnt()
{
	return (int)pct_list.size();
}

// ****************************************************************
// �����擾
// ================================================================
int C_album::get_width(int pct_no)
{
	BSP<C_pct> pct = get_pct(pct_no);
	if (pct)
		return pct->get_width();

	return 0;
}

// ****************************************************************
// �������擾
// ================================================================
int C_album::get_height(int pct_no)
{
	BSP<C_pct> pct = get_pct(pct_no);
	if (pct)
		return pct->get_height();

	return 0;
}

}
