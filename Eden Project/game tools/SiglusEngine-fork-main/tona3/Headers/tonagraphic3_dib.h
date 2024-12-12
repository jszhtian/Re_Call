#pragma		once

#include	"tonagraphic3_graphic.h"

namespace NT3
{

// ****************************************************************
// �f�B�u�Z�N�V����
// ================================================================
//		�P��̃r�b�g�}�b�v�i��`�摜�j���Ǘ�����N���X�ł��B
//		�N���X���̓f�B�u�Z�N�V�����ƂȂ��Ă܂����A���̂͂����̃r�b�g�f�[�^�ň����Ă��܂��B
// ================================================================
//		true �ŏ��������邱�ƂŁADIBSECTION ���쐬���邱�Ƃ��ł��܂��B
// ================================================================
class C_dib
{
public:
	C_dib();
	~C_dib();

	explicit C_dib(bool is_API);		// DIBSECTION �𗘗p����Ȃ� true ���w��

	void	init();
	bool	create(int width, int height, int depth);						// �f�B�u���\�z
	bool	create(int width, int height, int depth, C_point center);		// �f�B�u���\�z
	void	destroy();														// �f�B�u��j��

	bool	is_init()				{ return is_API ? h_bitmap != NULL : !bit_data.empty(); }			// �f�[�^�̗L���𒲂ׂ�
	HBITMAP	get_handle()			{ return is_API ? h_bitmap : NULL; }								// �n���h�����擾
	BYTE*	get_ptr()				{ return is_API ? bit_data_API : bit_data.get(); }					// ���f�[�^�ւ̃|�C���^���擾
	BYTE*	get_ptr(int x, int y)	{ return get_ptr() + y * bytes_per_line + x * bytes_per_pixel; }	// ���f�[�^�ւ̃|�C���^���擾
	int		get_size()				{ return is_API ? width * height * depth / 8 : bit_data.size(); }	// �T�C�Y���擾
	int		get_width()				{ return width; }	// �����擾
	int		get_height()			{ return height; }	// �������擾

	void	clear_color(C_argb color);	// �w��F�ŃN���A

	bool	load_moji(TCHAR moji, C_argb color, LOGFONT* lf, int* off_x, int* off_y);				// ���������[�h
	bool	load_moji(TCHAR moji, C_argb color, LOGFONT* lf, int* off_x, int* off_y, MAT2 mat);		// ���������[�h
	bool	load_bmp(CTSTR& file_path);																// bmp �����[�h
	bool	load_png(CTSTR& file_path);																// png �����[�h
	bool	load_jpg(CTSTR& file_path);																// jpg �����[�h

	bool	create_magnified_dib_scale(C_dib* src, int scale_x, int scale_y);		// �g��i�{���w��j
	bool	create_magnified_dib_size(C_dib* src, int dst_w, int dst_h);			// �g��i�T�C�Y�w��j
	bool	create_rotate_90_dib(C_dib* p_src_dib);									// �E�ɂX�O�x��]

	void	draw_point(int x, int y, C_argb color, bool sprite);														// �_��`��
	void	draw_rect(int x, int y, int w, int h, C_argb color, bool fill, bool sprite);								// ��`��`��i�ȈՁj
	void	draw_rect(S_disp_param* dp);																				// ��`��`��i�t���p�����[�^�j
	void	draw_rect(S_disp_param* dp, bool draw_flag, bool* ret_draw_rect_flag, C_rect* ret_draw_rect);				// ��`��`��i�t���p�����[�^�{�`���`�擾�j
	void	draw_dib(C_dib* dib, int x, int y, bool sprite);															// dib ��`��i�ȈՁj
	void	draw_dib(C_dib* dib, S_disp_param* dp);																		// dib ��`��i�t���p�����[�^�j
	void	draw_dib(C_dib* dib, S_disp_param* dp, bool draw_flag, bool* ret_draw_rect_flag, C_rect* ret_draw_rect);	// dib ��`��i�t���p�����[�^�{�`���`�擾�j
	void	draw_dib_easy(C_dib* dib, int x, int y);																	// dib ��`��i�ȈՔŁF�O���t�B�b�N���C�u�������g�p���܂���j

	void	transform_draw(S_disp_param* dp);	// �ϊ��`��
	void	transform_to_gray_scale();			// �O���[�X�P�[���ɕϊ�

	bool	display_to_screen(HDC h_dst_dc);	// �X�N���[���ɕ`��

private:

	// �r�b�g�}�b�v�w�b�_�\����
	struct HEADER	{
		BITMAPINFOHEADER	info;
		DWORD				BitField[3];
	};
	
	// �r�b�g�}�b�v�w�b�_�̐ݒ�
	HEADER	header;

	// �r�b�g�}�b�v���
	bool	is_API;				// API ���g���Ă��邩�ǂ���
	BUFFER	bit_data;			// �r�b�g�f�[�^
	BYTE*	bit_data_API;		// �r�b�g�f�[�^�iAPI�̏ꍇ�j
	HBITMAP	h_bitmap;			// �r�b�g�}�b�v�n���h���iAPI�̏ꍇ�j
	int		bytes_per_line;		// �P�s������̃o�C�g��
	int		bytes_per_pixel;	// �P�s�N�Z��������̃o�C�g��
	int		bytes;				// �r�b�g�}�b�v�S�̂̃o�C�g��
	int		width;				// ��
	int		height;				// ����
	int		depth;				// �F��
	C_point	center;				// ���S

	bool	create_ready(int width, int height, int depth, C_point center);
	void	draw_glyph_outline(BYTE* font, int pitch, C_argb color);

	// �R�s�[���֎~�ɂ��邽�߁A�R�s�[�R���X�g���N�^�Ƒ�����Z�q���B��
	C_dib(C_dib&);
	C_dib& operator = (C_dib& rop);
};

}
