#pragma		once

#include	"tonad3d3_d3d.h"
#include	"tonad3d3_surface.h"
#include	"g00.h"

namespace NT3
{

// ****************************************************************
// D3D �e�N�X�`��
// ================================================================
class C_d3d_texture
{
public:
	C_d3d_texture();
	~C_d3d_texture();

	void	init();																// �e�N�X�`����������
	void	release();															// �e�N�X�`�������
	bool	create(int width, int height);										// �e�N�X�`�����\�z
	bool	create(int width, int height, UINT mip_level, DWORD usage, D3DFORMAT format, D3DPOOL pool);		// �e�N�X�`�����\�z�i�t���p�����[�^�j
	bool	clear_color_ex(C_argb color);																	// �w��F�œh��Ԃ�
	bool	clear_color(C_argb color);																		// �w��F�œh��Ԃ�
	bool	draw_rect(C_rect rect, C_argb moji_color);														// ��`��`��
	bool	load_moji(TCHAR moji, int moji_size, C_argb moji_color);										// ���������[�h�i�ȈՎw��j
	bool	load_moji(TCHAR moji, LOGFONT* lf,   C_argb moji_color);										// ���������[�h�i�t�H���g���w��j
	bool	load_moji(TCHAR moji, int moji_size, C_argb moji_color, MOJI_FACE_TYPE face_type, bool tategaki, HDC h_dc);		// ���������[�h�i�f�o�C�X�R���e�L�X�g���w��j
	bool	load_file(CTSTR& file_path);																	// �摜�t�@�C�������[�h
	bool	load_g00(CTSTR& file_path);																		// g00 �����[�h
	bool	load_g00_cut(C_g00_cut* g00_cut, bool fuchidori);												// g00 �̃J�b�g�����[�h
	bool	draw_g00_cut(C_g00_cut* g00_cut, int x, int y, int blend_type);									// g00 �J�b�g����������
	bool	add_edge_for_filterring(D3DLOCKED_RECT* p_texture_bit);											// �t�B���^�����O�����̂��߂ɃG�b�W��������i���b�N���܂���j

	int		get_width()				{ return m_width; }
	int		get_height()			{ return m_height; }
	C_size	get_size()				{ return C_size(m_width, m_height); }
	int		get_width_ex()			{ return m_width_ex; }
	int		get_height_ex()			{ return m_height_ex; }
	C_size	get_size_ex()			{ return C_size(m_width_ex, m_height_ex); }
	int		get_original_width()	{ return m_org_width; }
	int		get_original_height()	{ return m_org_height; }
	C_size	get_original_size()		{ return C_size(m_org_width, m_org_height); }
	C_point	get_center()			{ return m_center; }
	C_rect	get_rect()				{ return C_rect(0 - m_center.x, 0 - m_center.y, m_width - m_center.x, m_height - m_center.y); }
	bool	get_blend()				{ return m_blend; }
	bool	is_mipmap_enable()		{ return m_mipmap_enable; }

	void	set_dirty_flag(bool flag)	{ m_dirty = flag; }
	bool	is_dirty()					{ return m_dirty; }

	C_argb	get_color(int x, int y);							// �F���擾
	void	set_center(C_point pos)	{	m_center = pos;	}		// ���S���W��ݒ�

	bool	lock_rect(UINT level, D3DLOCKED_RECT* texture_bit, C_rect* rect, DWORD flags);	// ���b�N
	bool	unlock_rect(UINT level);														// �A�����b�N
	bool	get_render_target_data(int level, IDirect3DSurface9* p_surface);				// �����_�[�^�[�Q�b�g�̃f�[�^���擾����

	C_d3d_surface	get_surface(int level);		// �T�[�t�F�X���擾����

	IDirect3DTexture9*	body()			{	return m_texture.get();	}
	IDirect3DTexture9*	operator ->()	{	return m_texture.get();	}

private:

	BIP<IDirect3DTexture9>	m_texture;

	int		m_width;			// �e�N�X�`���̕�
	int		m_height;			// �e�N�X�`���̍���
	int		m_width_ex;			// ���ۂ̃e�N�X�`���̕��i2^n �ɒ����ς݁j
	int		m_height_ex;		// ���ۂ̃e�N�X�`���̍����i2^n �ɒ����ς݁j
	int		m_org_width;		// ���摜�̕�
	int		m_org_height;		// ���摜�̍���
	C_point	m_center;			// ���S���W
	bool	m_blend;			// �����r�b�g�����܂�
	bool	m_mipmap_enable;	// �~�b�v�}�b�v���g����
	bool	m_dirty;			// �X�V���K�v�t���O
};

}
