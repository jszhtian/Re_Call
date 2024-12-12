#include	"tonagraphic3_pch.h"
#include	"tonagraphic3_dib.h"
#include	"tonagraphic3_pct.h"

#include	<png.h>

#define	XMD_H
#undef FAR
extern "C"	{
#include	<jpeglib.h>
}

namespace NT3
{

// ****************************************************************
// �R���X�g���N�^
// ================================================================
C_dib::C_dib()
{
	init();

	is_API = false;
}

C_dib::C_dib(bool is_API_)
{
	init();

	is_API = is_API_;
}

// ****************************************************************
// �f�X�g���N�^
// ================================================================
C_dib::~C_dib()
{
	destroy();
}

// ****************************************************************
// ������
// ================================================================
void C_dib::init()
{
	bit_data.clear();
	bit_data_API = NULL;
	h_bitmap = NULL;

	width = 0;
	height = 0;
	depth = 0;
	bytes_per_line = 0;
	bytes_per_pixel = 0;
	bytes = 0;
	center = C_point(0, 0);
}

// ****************************************************************
// �f�B�u��j��
// ================================================================
void C_dib::destroy()
{
	// ���\�[�X�̉��
	if (h_bitmap) {
		GdiFlush();
		::DeleteObject(h_bitmap);
	}

	// ������
	init();
}

// ****************************************************************
// �f�B�u���쐬
// ================================================================
bool C_dib::create(int width_, int height_, int depth_)
{
	return create(width_, height_, depth_, C_point(0, 0));
}

bool C_dib::create(int width_, int height_, int depth_, C_point center_)
{
	// ���݂̃f�B�u��j��
	destroy();

	// �s���ȃp�����[�^
	if (width_ <= 0 || height_ <= 0 || depth_ <= 0)
		return false;

	// 8 / 32 �r�b�g�ȊO�͖��Ή�
	if (depth_ != 8 && depth_ != 32)
		return false;

	// �p�����[�^�̃Z�b�g
	width = width_;
	height = height_;
	depth = depth_;
	center = center_;
	bytes_per_pixel = depth / 8;
	bytes_per_line = ((width * bytes_per_pixel + 3) / 4) * 4;	// �S�̔{���ɒ���
	bytes = bytes_per_line * height;

	// �r�b�g�}�b�v���
	header.info.biSize			= sizeof(BITMAPINFOHEADER);
	header.info.biWidth			= width;		// ��
	header.info.biHeight		= -height;		// �����F�g�b�v�_�E���ɂ��邽�߁A���̒l������
	header.info.biBitCount		= depth;		// �[�x
	header.info.biPlanes		= 1;			// �f�o�C�X�ɑ΂���ʂ̐��H�K���P�炵��
	header.info.biXPelsPerMeter	= 0;			// �f�o�C�X�̐����𑜓x���P���[�g��������̃s�N�Z���Ŏw��E�E�E�炵��
	header.info.biYPelsPerMeter	= 0;			// �f�o�C�X�̐����𑜓x���P���[�g��������̃s�N�Z���Ŏw��E�E�E�炵��
	header.info.biClrUsed		= 0;			// �p���b�g��
	header.info.biClrImportant	= 0;			// �\���Ɏg���p���b�g��
	header.info.biCompression	= depth == 24 ? BI_RGB : BI_BITFIELDS;	// �}�X�N
	header.info.biSizeImage		= bytes;		// �r�b�g�T�C�Y

	// �[�x�ʃr�b�g�̊��蓖��
	switch (depth) {

		case 8:
			break;

		case 16:
			header.BitField[0] = 0x7C00;		// 0111110000000000
			header.BitField[1] = 0x03E0;		// 0000001111100000
			header.BitField[2] = 0x001F;		// 0000000000011111
			break;

		case 32:
			header.BitField[0] = 0x00FF0000;	// 00000000111111110000000000000000
			header.BitField[1] = 0x0000FF00;	// 00000000000000001111111100000000
			header.BitField[2] = 0x000000FF;	// 00000000000000000000000011111111
			break;

		default:
			header.BitField[0] = 0;
			header.BitField[1] = 0;
			header.BitField[2] = 0;
			break;
	}

	// API ���g���o�[�W�����̏ꍇ
	if (is_API)	{

		// �f�B�u���쐬
		HDC h_dc = ::GetDC(0);					// 0 : ��ʑS��
		h_bitmap = CreateDIBSection(h_dc, (BITMAPINFO *)&header, DIB_RGB_COLORS, (void **)&bit_data_API, NULL, 0);
		return h_bitmap != NULL;
	}
	
	// API ���g��Ȃ��o�[�W�����̏ꍇ
	else	{

		// �r�b�g�f�[�^�̊m��
		bit_data.resize(bytes);
	}

	return true;
}

// ****************************************************************
// ������ǂݍ���
//		moji:		�ǂݍ��ޕ���
//		color:		�����̐F
//		LOGFONT:	�t�H���g
//		pos_x:		�����̕\���ʒu�i���[����̋����j
//		pos_y:		�����̕\���ʒu�i�O���t���C������̋����j
// ================================================================
bool C_dib::load_moji(TCHAR moji, C_argb color, LOGFONT* lf, int* pos_x, int* pos_y)
{
	MAT2 mat = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };

	return load_moji(moji, color, lf, pos_x, pos_y, mat);
}

bool C_dib::load_moji(TCHAR moji, C_argb color, LOGFONT* lf, int* pos_x, int* pos_y, MAT2 mat)
{
	GLYPHMETRICS gm;
	ZeroMemory(&gm, sizeof(GLYPHMETRICS));

	// �f�o�C�X�R���e�L�X�g
	HDC h_dc = CreateCompatibleDC(0);

	// �t�H���g�쐬
	HFONT h_font = CreateFontIndirect(lf);
	HGDIOBJ h_old_font = SelectObject(h_dc, (HFONT)h_font);

	// �t�H���g�����擾
	TEXTMETRIC tm;
	::GetTextMetrics(h_dc, &tm);

	// �t�H���g�f�[�^���擾
	DWORD size = GetGlyphOutline(h_dc, moji, GGO_GRAY8_BITMAP, &gm, 0, NULL, &mat);
	if (size == GDI_ERROR)
		return false;

	// �X�y�[�X�̏ꍇ�Asize �� 0 ���Ԃ��ė���̂ŁA���ꏈ��
	if (size == 0)	{
		// �P�~�P�� DIB �̍쐬
		if (!create(1, 1, 32))		return false;
		// 0 �N���A
		*(DWORD *)get_ptr(0, 0) = 0;
	}

	// �X�y�[�X�ȊO
	else	{

		// �t�H���g�̃r�b�g�f�[�^�擾
		ARRAY<BYTE> font_data(size);
		GetGlyphOutline(h_dc, moji, GGO_GRAY8_BITMAP, &gm, size, font_data.get(), &mat);
		// �s�b�`�i�P�s�̉����j���v�Z
		int pitch = (gm.gmBlackBoxX + 3) & 0xFFFC;
		// DIB �̍쐬
		if (!create(gm.gmBlackBoxX, gm.gmBlackBoxY, 32))
			return false;
		// �t�H���g�̕`��
		draw_glyph_outline(font_data.get(), pitch, color);
	}

	// �f�o�C�X�R���e�L�X�g�̉��
	SelectObject(h_dc, h_old_font);
	DeleteObject(h_font);
	DeleteDC(h_dc);

	// �ʒu�̒���
	if (pos_x)	*pos_x = gm.gmptGlyphOrigin.x;
	if (pos_y)	*pos_y = gm.gmptGlyphOrigin.y + tm.tmDescent;

	return true;
}

// ****************************************************************
// �t�H���g�̕`��
// ================================================================
void C_dib::draw_glyph_outline(BYTE* font_data, int pitch, C_argb color)
{
	// �A�Z���u�����Ŏg���l�����[�J���Ɏ擾
	BYTE* src_ = font_data;
	BYTE* dst_ = get_ptr();
	BYTE* pal_ = C_palette::get_instance().p_ggo_gray8;
	DWORD color_ = color;
	int width_ = width;
	int height_ = height;
	int pitch_ = pitch;
	int bpl_ = bytes_per_line;

	_asm	{
		cld
		mov		esi, src_		// �]�����̐ݒ�
		mov		edi, dst_		// �]����̐ݒ�
		mov		ecx, height_	// ���������[�v�J�E���^�ɃZ�b�g

	loop_y:

		push	esi				// �]�����̌��݂̈ʒu���L��
		push	edi				// �]����̌��݂̈ʒu���L��
		push	ecx				// ���[�v�J�E���^���L��
		mov		ecx, width_		// �������[�v�J�E���^�ɃZ�b�g

	loop_x:

		// bgr
		mov		eax, color_
		mov		[edi], eax

		// �O���[�l��s�����x�ɕϊ�
		xor		eax, eax
		mov		ebx, pal_
		mov		al, [esi]
		add		ebx, eax
		mov		al, [ebx]
		mov		[edi+3], al

		add		esi, 1
		add		edi, 4

		loop	loop_x

		pop		ecx
		pop		edi
		pop		esi
		add		esi, pitch_
		add		edi, bpl_

		loop	loop_y
	}
}

// ****************************************************************
// �r�b�g�}�b�v��ǂݍ���
// ================================================================
bool C_dib::load_bmp(CTSTR& file_path)
{
	destroy();

	// �t�@�C������r�b�g�}�b�v�̓ǂݍ���
	HBITMAP h_temp_bitmap = (HBITMAP)LoadImage(G_app.h_inst, file_path.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if (!h_temp_bitmap)
		return false;

	// �ǂݍ��񂾃r�b�g�}�b�v����e���|���� Dib �̍쐬
	DIBSECTION temp_dib;
	if (GetObject(h_temp_bitmap, sizeof(DIBSECTION), &temp_dib) != sizeof(DIBSECTION)) {
		DeleteObject(h_temp_bitmap);
		return false;
	}

	// �Ή����Ă��Ȃ��r�b�g���̂��̂��͂���
	if (temp_dib.dsBmih.biBitCount != 24 && temp_dib.dsBmih.biBitCount != 32)	{
		error_box(_T("24bit / 32bit �ȊO�̃r�b�g�}�b�v�ɂ͑Ή����Ă��܂���B"));
		DeleteObject(h_temp_bitmap);
		return false;
	}

	// �e���|���� Dib �Ɠ����T�C�Y�� 32bit Dib ���쐬
	create(temp_dib.dsBmih.biWidth, temp_dib.dsBmih.biHeight, 32);

	// �f�[�^�̃R�s�[����
	int   temp_dipth = temp_dib.dsBmih.biBitCount;
	BYTE* temp_data = (BYTE *)temp_dib.dsBm.bmBits;
	int   temp_bytes_per_line = temp_dib.dsBmih.biSizeImage / height;

	// 32bit �̏ꍇ
	if (temp_dipth == 32)	{
		for (int y = 0; y < height; y++)
			memcpy(get_ptr() + y * bytes_per_line, temp_data + y * temp_bytes_per_line, bytes_per_line);
	}

	// 24bit �̏ꍇ
	else if (temp_dipth == 24)	{

		// ������A�Z���u���ɗ��Ƃ������_(^o^)�^
		for (int y = 0; y < height; y++)	{
			BYTE* dst = get_ptr() + y * bytes_per_line;
			BYTE* src = temp_data + (height - y - 1) * temp_bytes_per_line;
			for (int x = 0; x < width; x++)	{
				*dst++ = *src++;
				*dst++ = *src++;
				*dst++ = *src++;
				*dst++ = 255;
			}
		}
	}

	return true;
}

// ****************************************************************
// png ��ǂݍ���
// ================================================================
bool C_dib::load_png(CTSTR& file_path)
{
	destroy();

	// �� png �N���X�����Ă܂���
	//    �r���ŃG���[���o��Ƃ��Ԃ񃊁[�N���܂�

	// png �t�@�C�����J��
	C_file file;
	if (!file.open(file_path, _T("rb")))
		return false;

	// png �`�F�b�N
	png_byte sig[8];
	if (file.read(sig, 8) != 8 || !png_check_sig(sig, 8))
		return false;

	// png �\���̂̏�����
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png)
		return false;

	// info �\���̂̏�����
	png_infop info = png_create_info_struct(png);
	if (!info)
		return false;

	// png �ɏ����Z�b�g
	png_init_io(png, file.get_fp());
	png_set_sig_bytes(png, 8);
	png_read_info(png, info);

	// ���̎擾
	png_uint_32 png_w, png_h;
	int png_d, color_type;
	png_get_IHDR(png, info, &png_w, &png_h, &png_d, &color_type, NULL, NULL, NULL);

	// �Ή����Ă��Ȃ��t�H�[�}�b�g��ϊ�
	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_expand(png);
	if (color_type == PNG_COLOR_TYPE_GRAY && png_d < 8)
		png_set_expand(png);
	if (png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_expand(png);
	if (png_d > 8)
		png_set_strip_16(png);
	if (color_type == PNG_COLOR_TYPE_GRAY)
		png_set_gray_to_rgb(png);

	// dib �ɂ��킹�� BGR �ɓW�J����
	png_set_bgr(png);
	// �A���t�@��񂪂Ȃ��ꍇ�A���t�@�����Z�b�g�i0xFF �ŏ������j
	if(!(color_type & PNG_COLOR_MASK_ALPHA))
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

	// ���̍ēǂݍ���
	png_read_update_info(png, info);
	png_get_IHDR(png, info, &png_w, &png_h, &png_d, &color_type, NULL, NULL, NULL);

	// PNG_COLOR_TYPE_RGB �ɂȂ��Ă��Ȃ������玸�s
	if (!(color_type & PNG_COLOR_TYPE_RGB))
		return false;

	// dib �̍쐬
	if (!create(int(png_w), int(png_h), 32))
		return false;

	// row �̍쐬
	ARRAY<BYTE *> row(height);
	for (int y = 0; y < height; y++)
		row[y] = get_ptr(0, y);

	// �C���[�W�̓ǂݍ���
	png_read_image(png, row.get());

	// �\���̂̊J��
	png_destroy_read_struct(&png, &info, NULL);

	return true;
}

// ****************************************************************
// jpg ��ǂݍ���
// ================================================================

struct my_error_mgr
{
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;

static void my_error_exit(j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr)cinfo->err;
	longjmp(myerr->setjmp_buffer, 1);
}

/*
bool C_dib::load_jpg(CTSTR& file_path)
{
	destroy();

	jpeg_decompress_struct cinfo;
	my_error_mgr jerr;

	// jpeg �t�@�C�����J��
	C_file file;
	if (!file.open(file_path, _T("rb")))	return false;

	// �G���[�̐ݒ�
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer))	{
		jpeg_destroy_decompress(&cinfo);
		return false;
	}

	// jpeg �̉𓀏���
	jpeg_create_decompress(&cinfo);
	// jpeg �Ƀt�@�C�����Z�b�g
	jpeg_stdio_src(&cinfo, file.get_fp());
	// jpeg �w�b�_��ǂ݂���
	jpeg_read_header(&cinfo, TRUE);
	// jpeg �𓀃p�����[�^��ݒ�i�������j
	;
	// jpeg ����
	jpeg_start_decompress(&cinfo);

	// dib ���쐬
	if (!create(cinfo.image_width, cinfo.image_height, 32))
		return false;

	// �W�J�p�̈ꎞ�o�b�t�@�̍쐬
	int row_stride = cinfo.output_width * cinfo.output_components;
	JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	// jpeg �� dib �ɓW�J
	for (int y = 0; cinfo.output_scanline < cinfo.output_height; y++)	{
		jpeg_read_scanlines(&cinfo, buffer, 1);		// �P�s�W�J

		// 32bit �ɓW�J���Ă���
		BYTE* dst = get_ptr(0, y);
		BYTE* src = buffer[0];
		for (int x = 0; x < (int)cinfo.output_width; x++)	{
			*(dst + 0) = *(src + 2);
			*(dst + 1) = *(src + 1);
			*(dst + 2) = *(src + 0);
			*(dst + 3) = 0xff;

			dst += 4;
			src += cinfo.output_components;
		}
	}

	// jpeg �̊J��
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	return true;
}
*/

// ****************************************************************
// �t�@�~�R���`�b�v��ǂݍ���
// ================================================================
/*
bool C_dib::load_fc_chip(BYTE* gra_data, BYTE* pal, int pal_pos, bool rev_x, bool rev_y)
{
	BYTE* fc_pal = Cpalette::get_instance().p_famicom;

	// dib �̍쐬�i�Œ�T�C�Y�j
	if (!create(8, 8, 32))
		return false;

	// ���[�J���ȃp���b�g�n�b�V�����쐬
	BYTE* local_pal_p[4];
	local_pal_p[0] = fc_pal + pal[pal_pos * 4]     * 3;
	local_pal_p[1] = fc_pal + pal[pal_pos * 4 + 1] * 3;
	local_pal_p[2] = fc_pal + pal[pal_pos * 4 + 2] * 3;
	local_pal_p[3] = fc_pal + pal[pal_pos * 4 + 3] * 3;

	for (int y = 0; y < 8; y++)	{

		// �O���t�B�b�N�f�[�^�̓W�J
		BYTE data[8] = {0};
		if (*gra_data & 0x80)		data[0] += 1;
		if (*gra_data & 0x40)		data[1] += 1;
		if (*gra_data & 0x20)		data[2] += 1;
		if (*gra_data & 0x10)		data[3] += 1;
		if (*gra_data & 0x08)		data[4] += 1;
		if (*gra_data & 0x04)		data[5] += 1;
		if (*gra_data & 0x02)		data[6] += 1;
		if (*gra_data & 0x01)		data[7] += 1;
		if (*(gra_data + 8) & 0x80)	data[0] += 2;
		if (*(gra_data + 8) & 0x40)	data[1] += 2;
		if (*(gra_data + 8) & 0x20)	data[2] += 2;
		if (*(gra_data + 8) & 0x10)	data[3] += 2;
		if (*(gra_data + 8) & 0x08)	data[4] += 2;
		if (*(gra_data + 8) & 0x04)	data[5] += 2;
		if (*(gra_data + 8) & 0x02)	data[6] += 2;
		if (*(gra_data + 8) & 0x01)	data[7] += 2;

		// �Q�{�Ɋg�債�ĕ`��
		BYTE* dst = get_ptr( (rev_x ? 7 : 0), (rev_y ? 7 - y : y) );

		for (int i = 0; i < 8; i++)	{
			BYTE* src = local_pal_p[data[i]] + 2;
			*dst++ = *src--;
			*dst++ = *src--;
			*dst++ = *src--;
			if (data[i] == 0)		*dst++ = 0;
			else					*dst++ = 255;
			dst += rev_x ? -8 : 0;		// ����Ɏ��̃o�C�g��
		}

		gra_data ++;
	}

	return true;
}
*/

// ****************************************************************
// �w��F�œh��Ԃ�
// ================================================================
void C_dib::clear_color(C_argb color)
{
	S_disp_param dp;
	dp.plane.color = color;
	dp.plane.rect.left = 0;
	dp.plane.rect.top = 0;
	dp.plane.rect.right = width;
	dp.plane.rect.bottom = height;
	dp.plane.type = E_disp_plane_type_box_fill;
	dp.sprite = false;

	draw_rect(&dp);
}

// ****************************************************************
// �_��`��
// ================================================================
void C_dib::draw_point(int x, int y, C_argb color, bool sprite)
{
	S_disp_param dp;
	dp.plane.color = color;
	dp.plane.rect.left = x;
	dp.plane.rect.top = y;
	dp.plane.rect.right = x + 1;
	dp.plane.rect.bottom = y + 1;
	dp.sprite = sprite;

	draw_rect(&dp);
}

// ****************************************************************
// ��`��`��
// ================================================================
void C_dib::draw_rect(int x, int y, int w, int h, C_argb color, bool fill, bool sprite)
{
	S_disp_param dp;
	dp.plane.color = color;
	dp.plane.rect.left = x;
	dp.plane.rect.top = y;
	dp.plane.rect.right = x + w;
	dp.plane.rect.bottom = y + h;
	dp.plane.type = E_disp_plane_type_box_fill;
	dp.sprite = sprite;

	draw_rect(&dp);
}

void C_dib::draw_rect(S_disp_param* dp)
{
	draw_rect(dp, true, NULL, NULL);
}

void C_dib::draw_rect(S_disp_param* dp, bool draw_flag, bool* ret_draw_rect_flag, C_rect* ret_draw_rect)
{
	if (!is_init())		return;

	tc_draw_plane(get_ptr(), width, height, center, dp, draw_flag, ret_draw_rect_flag, ret_draw_rect);
}

// ****************************************************************
// dib ��`��
// ================================================================
void C_dib::draw_dib(C_dib* dib, int x, int y, bool sprite)
{
	S_disp_param dp;
	dp.pos.x = x;
	dp.pos.y = y;
	dp.sprite = sprite;

	draw_dib(dib, &dp);
}

void C_dib::draw_dib(C_dib* dib, S_disp_param* dp)
{
	draw_dib(dib, dp, true, NULL, NULL);
}

void C_dib::draw_dib(C_dib* dib, S_disp_param* dp, bool draw_flag, bool* ret_draw_rect_flag, C_rect* ret_draw_rect)
{
	if (!is_init())		return;

	tc_draw_dib(get_ptr(), width, height, center, dib->get_ptr(), dib->get_width(), dib->get_height(), dp, draw_flag, ret_draw_rect_flag, ret_draw_rect);
}

void C_dib::draw_dib_easy(C_dib* dib, int x, int y)
{
	if (!is_init())		return;

	tc_draw_dib_easy(get_ptr(), width, height, dib->get_ptr(), dib->get_width(), dib->get_height(), x, y);
}

// ****************************************************************
// �ϊ��`��
// ================================================================
void C_dib::transform_draw(S_disp_param *dp)
{
	if (!is_init())		return;

	tc_draw_dib(get_ptr(), width, height, center, get_ptr(), width, height, dp);
}

// ****************************************************************
// �g��i�{�����w��j
// ================================================================
bool C_dib::create_magnified_dib_scale(C_dib* src_dib, int scale_x, int scale_y)
{
	// �V���� dib �̍쐬
	if (!create(src_dib->width * scale_x, src_dib->height * scale_y, src_dib->depth))
		return false;

	for (int y = 0; y < src_dib->height; y++)	{
		DWORD* dst = (DWORD *)get_ptr(0, y * scale_y);
		DWORD* src = (DWORD *)src_dib->get_ptr(0, y);

		// ���Ɋg��
		for (int x = 0; x < src_dib->width; x++)	{
			for (int s = 0; s < scale_x; s++)	{
				*dst++ = *src;
			}
			src++;
		}
		// �c�Ɋg��
		for (int yy = 1; yy < scale_y; yy++)	{
			memcpy(get_ptr(0, y * scale_y + yy), get_ptr(0, y * scale_y), width * 4);
		}
	}

	return true;
}

// ****************************************************************
// �g��i�T�C�Y���w��j
// ================================================================
bool C_dib::create_magnified_dib_size(C_dib* src_dib, int dst_w, int dst_h)
{
	int src_w = src_dib->width;
	int src_h = src_dib->height;
	if (!create(dst_w, dst_h, src_dib->depth))
		return false;

	// �c�Ɋg��
	DWORD* dst_y = (DWORD *)get_ptr(0, 0);
	DWORD* src_y = (DWORD *)src_dib->get_ptr(0, 0);
	DWORD* end_y = (DWORD *)src_dib->get_ptr(0, src_h);
	int h_meter = 0;			// ���[�^�[
	while (src_y < end_y)	{
		for (; h_meter < dst_h; h_meter += src_h)	{

			// ���Ɋg��
			DWORD* dst_x = dst_y;
			DWORD* src_x = src_y;
			DWORD* end_x = src_y + src_w;
			int w_meter = 0;			// ���[�^�[
			while (src_x < end_x)	{
				for (; w_meter < dst_w; w_meter += src_w)	*dst_x++ = *src_x;
				for (; w_meter >= dst_w; w_meter -= dst_w)	src_x++;
			}
			dst_y += dst_w;
		}
		for (; h_meter >= dst_h; h_meter -= dst_h)
			src_y += src_w;
	}

	return true;
}

// ****************************************************************
// �E�ɂX�O�x��]
// ================================================================
bool C_dib::create_rotate_90_dib(C_dib* p_src_dib)
{
	// �V���� dib ���쐬
	if (!create(p_src_dib->height, p_src_dib->width, p_src_dib->depth))
		return false;

	// �r�b�g�R�s�[
	for (int y = 0; y < p_src_dib->height; y++)	{
		DWORD* p_src = (DWORD *)p_src_dib->get_ptr(0, y);
		DWORD* p_dst = (DWORD *)get_ptr(width - 1 - y, 0);
		for (int x = 0; x < p_src_dib->width; x++)	{
			*p_dst = *p_src;
			p_src ++;
			p_dst += width;
		}
	}

	return true;


	// ���{�c�āB�����`�ł����g���Ȃ������I�I�I�I�I

	//int unit_width = (width + 1) / 2;
	//int unit_height = height / 2;

	//for (int y = 0; y < unit_height; y++)	{
	//	DWORD* p1 = (DWORD *)get_ptr(0, y);
	//	DWORD* p2 = (DWORD *)get_ptr(width - 1 - y, 0);
	//	DWORD* p3 = (DWORD *)get_ptr(width - 1, height - 1 - y);
	//	DWORD* p4 = (DWORD *)get_ptr(y, height - 1);

	//	for (int x = 0; y < unit_width; x++)	{
	//		DWORD tmp = *p1;
	//		*p1 = *p2;
	//		*p2 = *p3;
	//		*p3 = *p4;
	//		*p4 = tmp;

	//		p1 ++;
	//		p2 += width;
	//		p3 --;
	//		p4 -= width;
	//	}
	//}

	//return true;
}

// ****************************************************************
// �X�N���[���ɕ`��
// ================================================================
bool C_dib::display_to_screen(HDC h_dst_dc)
{
	if (!is_init())
		return false;

	// �f�B�X�v���C�֓]���i�r�b�g�}�b�v�n���h�����g��Ȃ����@�j
	SetDIBitsToDevice(h_dst_dc, 0, 0, width, height, 0, 0, 0, height, get_ptr(), (BITMAPINFO *)&header, DIB_RGB_COLORS);

	// ���{�c��܂����i�r�b�g�}�b�v�n���h���̏���������Ƃ��̂Łj

	//// dib �Z�N�V������ DC ������
	//HDC h_screen_dc = GetDC(0);
	//HDC h_dib_dc = CreateCompatibleDC(h_screen_dc);
	//HGDIOBJ h_old_bitmap = SelectObject(h_dib_dc, get_handle());

	//// dib �Z�N�V��������ʂ֕`�悷��
	//BitBlt(h_dst_dc, x, y, w, h, h_dib_dc, x, y, SRCCOPY);

	//// dib �Z�N�V������ DC ���J��
	//GdiFlush();
	//SelectObject(h_dib_dc, h_old_bitmap);
	//DeleteDC(h_dib_dc);
	//if (h_screen_dc)
	//	::ReleaseDC(0, h_screen_dc);

	return true;
}

}
