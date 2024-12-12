#pragma		once

#include	"tonagraphic3_dib.h"

namespace NT3
{

// ****************************************************************
// dib �`�b�v
//		dib �ɉ����āA�`��ʒu�Ȃǂ̏�������
//		��Ƀs�N�`�������Ŏg�p����
// ================================================================
struct C_dib_chip : public C_dib
{
	C_dib_chip()
	{
		x = 0;
		y = 0;
		sprite = false;
	}

	int		x;
	int		y;
	bool	sprite;		// ���߂���
};

// ****************************************************************
// �s�N�`��
//		������ dib �`�b�v�̏W��
// ================================================================
class C_pct
{
public:
	C_pct();
	~C_pct();

	void	destroy();	// �j��

	bool	create(ARRAY< BSP<C_dib_chip> > chip_list, int width, int height, C_point center_, C_rect disp_rect);	// �s�N�`���̍쐬
	bool	load_moji(TCHAR moji, C_argb color, LOGFONT* lf, C_text_format* tf);				// ������ǂݍ���
	bool	load_moji(TCHAR moji, C_argb color, LOGFONT* lf, C_text_format* tf, MAT2 mat);		// ������ǂݍ���
	bool	load_bmp(CTSTR& file_path);															// bmp ��ǂݍ���
	bool	load_png(CTSTR& file_path);															// png ��ǂݍ���
	bool	load_jpg(CTSTR& file_path);															// jpg ��ǂݍ���

	BSP<C_dib_chip>	expand_chips();			// �`�b�v���P���ɓW�J
	bool			magnify(int scale);		// �s�N�`�����g��
	bool			rotate_90();			// �E�ɂX�O�x��]

	void	draw_to_dib(C_dib* dib, int x, int y, bool sprite);												// �s�N�`���� dib �ɕ`��
	void	draw_to_dib(C_dib* dib, S_disp_param* dp);														// �s�N�`���� dib �ɕ`��
	void	draw_to_dib(C_dib* dib, S_disp_param* dp, bool draw_flag, ARRAY<C_rect>* ret_draw_rect_list);	// �s�N�`���� dib �ɕ`��

	int		get_width()		{	return width;					}	// �����擾
	int		get_height()	{	return height;					}	// �������擾
	C_point	get_center()	{	return center;					}	// ���S���W���擾
	C_rect	get_disp_rect()	{	return disp_rect - center;		}	// �`���`���擾
	int		get_chip_cnt()	{	return (int)chip_list.size();	}	// �`�b�v�����擾

private:

	// �`�b�v���X�g
	typedef ARRAY< BSP<C_dib_chip> >	CHIPLIST;
	CHIPLIST							chip_list;

	int		width;		// ��
	int		height;		// ����
	C_point	center;		// ���S���W
	C_rect	disp_rect;	// �`���`

	// ================================================================
	// disp_rect �́A���̊O�ɓ��� (a = 0) �f�[�^�ȊO���Ȃ����Ƃ�ۏ؂����`�ł��B
	// �������@�� Edisp_blend_type_copy �ȊO�̂Ƃ��́Adisp_rect �ȊO��`�悷��K�v�͂���܂���B
	// �������@�� Edisp_blend_type_copy �̂Ƃ��́Adisp_rect ���g�킸�ɁAwidth - height �S�̂�`�悷��K�v������܂��B
	// ----------------------------------------------------------------
};

}
