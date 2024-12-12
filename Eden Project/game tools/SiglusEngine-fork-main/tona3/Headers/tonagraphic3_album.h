#pragma		once

#include	"tonagraphic3_pct.h"

namespace NT3
{

// ****************************************************************
// �A���o��
//		�����̃s�N�`���̏W��
// ================================================================
class C_album
{
public:
	C_album();
	~C_album();

	bool	create(ARRAY< BSP<C_pct> > pct_list);	// �A���o�����\�z
	void	destroy();								// �A���o����j��

	void	draw_to_dib(C_dib* dib, int pct_no, int x, int y, bool sprite);												// dib �ɕ`��
	void	draw_to_dib(C_dib* dib, int pct_no, S_disp_param* dp);														// dib �ɕ`��
	void	draw_to_dib(C_dib* dib, int pct_no, S_disp_param* dp, bool draw_flag, ARRAY<C_rect>* ret_draw_rect_list);	// dib �ɕ`��

	bool	expand_chips();		// �`�b�v���P���G�ɓW�J

	BSP<C_pct>	get_pct(int pct_no);		// �s�N�`�����擾�i�s�N�`���ԍ����w��j
	int			get_pct_cnt();				// �s�N�`�������擾
	int			get_width(int pct_no);		// �����擾
	int			get_height(int pct_no);		// �������擾

private:

	// �s�N�`�����X�g
	typedef	ARRAY< BSP<C_pct> >	PCTLIST;
	PCTLIST	pct_list;
};

}
