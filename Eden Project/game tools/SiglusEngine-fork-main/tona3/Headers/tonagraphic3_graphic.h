#pragma		once

#define		___USE_TG___			0	// ��肩���̂ƂȃO���t�B�b�N�B�ʏ�͎g��Ȃ��B
#define		___USE_NG___			0	// ���Ȃ܃O���t�B�b�N�B�ʏ�͎g��Ȃ��B
#define		___USE_NAMALIB_GRP___	1	// �V�Ȃ܃O���t�B�b�N�B�ʏ�͂�����g���B

// ****************************************************************
// TG / NG / namalib
// ================================================================

#if ___USE_TG___

namespace Ntona
{
void tc_draw_box_copy(
		BYTE* dst, int dst_w, int dst_h, 
		int x, int y, int w, int h,			// �{�b�N�X��`
		BYTE a, BYTE r, BYTE g, BYTE b,		// �{�b�N�X�F
		int blend_type,												// �������@
		BYTE alpha, BYTE mono, BYTE reverse, int bright,			// �s�����x�A���m�N���x�A���]�x�A���Óx
		BYTE color_r, BYTE color_g, BYTE color_b, BYTE color_rate,	// �q�f�a�ϊ�
		int rep_color_r, int rep_color_g, int rep_color_b			// �q�f�a�␳
	);

void tc_draw_dib_copy(
		BYTE* dst, int dst_w, int dst_h,	// �`���
		BYTE* src, int src_w, int src_h,	// �`�挳
		int x, int y,						// �`��ʒu
		int blend_type,													// �������@
		BYTE alpha, BYTE mono, BYTE reverse, int bright,				// �s�����x�A���m�N���x�A���]�x�A���Óx
		BYTE color_r, BYTE color_g, BYTE color_b, BYTE color_rate,		// �q�f�a�ϊ�
		int rep_color_r, int rep_color_g, int rep_color_b,				// �q�f�a�␳
		int src_rect_x, int src_rect_y, int src_rect_w, int src_rect_h	// �`���`
	);

void tc_draw_box_blend(
		BYTE* dst, int dst_w, int dst_h, 
		int x, int y, int w, int h,			// �{�b�N�X��`
		BYTE a, BYTE r, BYTE g, BYTE b,		// �{�b�N�X�F
		int blend_type,												// �������@
		BYTE alpha, BYTE mono, BYTE reverse, int bright,			// �s�����x�A���m�N���x�A���]�x�A���Óx
		BYTE color_r, BYTE color_g, BYTE color_b, BYTE color_rate,	// �q�f�a�ϊ�
		int rep_color_r, int rep_color_g, int rep_color_b			// �q�f�a�␳
	);

void tc_draw_dib_blend(
		BYTE* dst, int dst_w, int dst_h,	// �`���
		BYTE* src, int src_w, int src_h,	// �`�挳
		int x, int y,						// �`��ʒu
		int blend_type,													// �������@
		BYTE alpha, BYTE mono, BYTE reverse, int bright,				// �s�����x�A���m�N���x�A���]�x�A���Óx
		BYTE color_r, BYTE color_g, BYTE color_b, BYTE color_rate,		// �q�f�a�ϊ�
		int rep_color_r, int rep_color_g, int rep_color_b,				// �q�f�a�␳
		int src_rect_x, int src_rect_y, int src_rect_w, int src_rect_h	// �`���`
	);
};

#elif ___USE_NG___

#define		SC		signed char				// 1�޲�
#define		SS		signed short int		// 2�޲�
#define		SI		signed int				// 4�޲�
#define		SL		signed long int			// 4�޲�
#define		SD		signed double			// 8�޲�

#define		UC		unsigned char			// 1�޲�
#define		US		unsigned short int		// 2�޲�
#define		UI		unsigned int			// 4�޲�
#define		UL		unsigned long int		// 4�޲�
#define		UD		unsigned double			// 8�޲�

#include	<np_palt.h>
#include	<ng_grp.h>

#undef		SC
#undef		SS
#undef		SI
#undef		SL
#undef		SD

#undef		UC
#undef		US
#undef		UI
#undef		UL
#undef		UD

#pragma comment(lib, "ng_grp_ga.lib")

#elif ___USE_NAMALIB_GRP___

#include	<namalib_grp.h>

#endif

namespace NT3
{

	// ****************************************************************
	// �t�H���g�\����
	// ================================================================
	class C_log_font : public LOGFONT
	{
	public:

		C_log_font()	
		{
			// �f�t�H���g�l��ݒ肷��
			ZeroMemory(this, sizeof(*this));
			lfCharSet = DEFAULT_CHARSET;//SHIFTJIS_CHARSET;
			lfHeight = 16;
			_tcscpy_s(lfFaceName, LF_FACESIZE, _T("�l�r �o�S�V�b�N"));
		}
	};

	// ****************************************************************
	// �e�L�X�g�t�H�[�}�b�g�\����
	// ================================================================
	struct C_text_format
	{
		C_text_format() : width(16), height(16)	{}

		int width;
		int height;
	};

	// ****************************************************************
	// �p���b�g
	// ================================================================
	class C_palette
	{
		C_palette();

	public:
		static	C_palette&	get_instance()
		{
			static C_palette instance;
			return instance;
		}

		BYTE*	p_ggo_gray8;

	private:
		BYTE	ggo_gray8[256];
	};

	// ****************************************************************
	// �`��F�v���[���^�C�v
	// ================================================================
	enum E_disp_plane_type
	{
		E_disp_plane_type_line = 0,
		E_disp_plane_type_box = 1,
		E_disp_plane_type_box_fill = 2,
		E_disp_plane_type_max = 3,
	};

	// ****************************************************************
	// �`��F�`��\����
	// ================================================================
	struct S_disp_param_plane
	{
		C_rect				rect;		// ��`
		C_argb				color;		// �F
		E_disp_plane_type	type;		// �^�C�v�iE_disp_plane_type �̂����P���w��j
	};

	struct S_disp_param_filter
	{
		int		r;		// �q
		int		g;		// �f
		int		b;		// �a
		BYTE	a;		// �`
	};

	struct S_disp_param_tile
	{
		BYTE*	ptr;		// �^�C���F�f�[�^�|�C���^
		C_size	size;		// �^�C���F�T�C�Y
		C_point	center;		// �^�C���F���S���W
		bool	reverse;	// �^�C���F���]
		BYTE	shift;		// �^�C���F�V�t�g�i���x 0 �` 8�j
		int		add;		// �^�C���F���Z�i�g�䏈���j
	};

	struct S_disp_param
	{
		S_disp_param();
		void	init();

		// ��{�p�����[�^
		C_point	pos;			// �`����W
		C_point	center;			// ���S���W
		bool	sprite;			// ���߁itrue / false�j
		TRUE_COLOR_GRP_DRAW_MOD		blend_type;		// �������@
		BYTE	alpha;			// �s�����x�i0�`255�j
		BYTE	mono;			// ���m�N���x�i0�`255�j
		BYTE	reverse;		// ���]�x�i0�`255�j
		int		bright;			// ���Óx�i-255�`255�j
		C_rgb	color;			// �q�f�a�ϊ�
		BYTE	color_rate;		// �q�f�a�ϊ��x�i0�`255�j
		int		rep_color_r;	// �q�C���x�i-255�`255�j
		int		rep_color_g;	// �q�C���x�i-255�`255�j
		int		rep_color_b;	// �q�C���x�i-255�`255�j

		bool	dst_clip_use;	// �N���b�v�t���O
		C_rect	dst_clip;		// �N���b�v
		bool	src_rect_use;	// �`��͈̓t���O
		C_rect	src_rect;		// �`��͈�

		// �^�C���p�����[�^
		S_disp_param_tile	tile;
		// �v���[���p�����[�^
		S_disp_param_plane	plane;
		// �t�B���^�p�����[�^
		S_disp_param_filter	filter;
		// �g���p�����[�^
		C_size	scale;			// �{���i���{ = 1000�j
		int		rotate;			// ��]�i1�� = 10�j

		S_disp_param& operator = (const S_disp_param& rhs)	{ return pod_assign(this, &rhs); }
		bool operator == (const S_disp_param& rhs)			{ return pod_equal(this, &rhs); }
		bool operator != (const S_disp_param& rhs)			{ return !pod_equal(this, &rhs); }
	};

	// ****************************************************************
	// �`��֐�
	// ================================================================
	void tc_draw_plane(BYTE* dst, int dst_w, int dst_h, C_point dst_center, S_disp_param* dp);
	void tc_draw_plane(BYTE* dst, int dst_w, int dst_h, C_point dst_center, S_disp_param* dp, bool draw_flag, bool* ret_draw_rect_flag, C_rect* draw_rect);
	void tc_draw_dib(BYTE* dst, int dst_w, int dst_h, C_point dst_center, BYTE* src, int src_w, int src_h, S_disp_param* dp);
	void tc_draw_dib(BYTE* dst, int dst_w, int dst_h, C_point dst_center, BYTE* src, int src_w, int src_h, S_disp_param* dp, bool draw_flag, bool* ret_draw_rect_flag, C_rect* draw_rect);

	void tc_draw_dib_easy(BYTE* dst, int dst_w, int dst_h, BYTE* src, int src_w, int src_h, int x, int y);
}
