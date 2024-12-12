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
// コンストラクタ
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
// デストラクタ
// ================================================================
C_dib::~C_dib()
{
	destroy();
}

// ****************************************************************
// 初期化
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
// ディブを破壊
// ================================================================
void C_dib::destroy()
{
	// リソースの解放
	if (h_bitmap) {
		GdiFlush();
		::DeleteObject(h_bitmap);
	}

	// 初期化
	init();
}

// ****************************************************************
// ディブを作成
// ================================================================
bool C_dib::create(int width_, int height_, int depth_)
{
	return create(width_, height_, depth_, C_point(0, 0));
}

bool C_dib::create(int width_, int height_, int depth_, C_point center_)
{
	// 現在のディブを破壊
	destroy();

	// 不正なパラメータ
	if (width_ <= 0 || height_ <= 0 || depth_ <= 0)
		return false;

	// 8 / 32 ビット以外は未対応
	if (depth_ != 8 && depth_ != 32)
		return false;

	// パラメータのセット
	width = width_;
	height = height_;
	depth = depth_;
	center = center_;
	bytes_per_pixel = depth / 8;
	bytes_per_line = ((width * bytes_per_pixel + 3) / 4) * 4;	// ４の倍数に調整
	bytes = bytes_per_line * height;

	// ビットマップ情報
	header.info.biSize			= sizeof(BITMAPINFOHEADER);
	header.info.biWidth			= width;		// 幅
	header.info.biHeight		= -height;		// 高さ：トップダウンにするため、負の値を入れる
	header.info.biBitCount		= depth;		// 深度
	header.info.biPlanes		= 1;			// デバイスに対する面の数？必ず１らしい
	header.info.biXPelsPerMeter	= 0;			// デバイスの水平解像度を１メートルあたりのピクセルで指定・・・らしい
	header.info.biYPelsPerMeter	= 0;			// デバイスの垂直解像度を１メートルあたりのピクセルで指定・・・らしい
	header.info.biClrUsed		= 0;			// パレット数
	header.info.biClrImportant	= 0;			// 表示に使うパレット数
	header.info.biCompression	= depth == 24 ? BI_RGB : BI_BITFIELDS;	// マスク
	header.info.biSizeImage		= bytes;		// ビットサイズ

	// 深度別ビットの割り当て
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

	// API を使うバージョンの場合
	if (is_API)	{

		// ディブを作成
		HDC h_dc = ::GetDC(0);					// 0 : 画面全体
		h_bitmap = CreateDIBSection(h_dc, (BITMAPINFO *)&header, DIB_RGB_COLORS, (void **)&bit_data_API, NULL, 0);
		return h_bitmap != NULL;
	}
	
	// API を使わないバージョンの場合
	else	{

		// ビットデータの確保
		bit_data.resize(bytes);
	}

	return true;
}

// ****************************************************************
// 文字を読み込む
//		moji:		読み込む文字
//		color:		文字の色
//		LOGFONT:	フォント
//		pos_x:		文字の表示位置（左端からの距離）
//		pos_y:		文字の表示位置（グリフラインからの距離）
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

	// デバイスコンテキスト
	HDC h_dc = CreateCompatibleDC(0);

	// フォント作成
	HFONT h_font = CreateFontIndirect(lf);
	HGDIOBJ h_old_font = SelectObject(h_dc, (HFONT)h_font);

	// フォント情報を取得
	TEXTMETRIC tm;
	::GetTextMetrics(h_dc, &tm);

	// フォントデータを取得
	DWORD size = GetGlyphOutline(h_dc, moji, GGO_GRAY8_BITMAP, &gm, 0, NULL, &mat);
	if (size == GDI_ERROR)
		return false;

	// スペースの場合、size に 0 が返って来るので、特殊処理
	if (size == 0)	{
		// １×１の DIB の作成
		if (!create(1, 1, 32))		return false;
		// 0 クリア
		*(DWORD *)get_ptr(0, 0) = 0;
	}

	// スペース以外
	else	{

		// フォントのビットデータ取得
		ARRAY<BYTE> font_data(size);
		GetGlyphOutline(h_dc, moji, GGO_GRAY8_BITMAP, &gm, size, font_data.get(), &mat);
		// ピッチ（１行の横幅）を計算
		int pitch = (gm.gmBlackBoxX + 3) & 0xFFFC;
		// DIB の作成
		if (!create(gm.gmBlackBoxX, gm.gmBlackBoxY, 32))
			return false;
		// フォントの描画
		draw_glyph_outline(font_data.get(), pitch, color);
	}

	// デバイスコンテキストの解放
	SelectObject(h_dc, h_old_font);
	DeleteObject(h_font);
	DeleteDC(h_dc);

	// 位置の調整
	if (pos_x)	*pos_x = gm.gmptGlyphOrigin.x;
	if (pos_y)	*pos_y = gm.gmptGlyphOrigin.y + tm.tmDescent;

	return true;
}

// ****************************************************************
// フォントの描画
// ================================================================
void C_dib::draw_glyph_outline(BYTE* font_data, int pitch, C_argb color)
{
	// アセンブラ内で使う値をローカルに取得
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
		mov		esi, src_		// 転送元の設定
		mov		edi, dst_		// 転送先の設定
		mov		ecx, height_	// 高さをループカウンタにセット

	loop_y:

		push	esi				// 転送元の現在の位置を記憶
		push	edi				// 転送先の現在の位置を記憶
		push	ecx				// ループカウンタを記憶
		mov		ecx, width_		// 幅をループカウンタにセット

	loop_x:

		// bgr
		mov		eax, color_
		mov		[edi], eax

		// グレー値を不透明度に変換
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
// ビットマップを読み込む
// ================================================================
bool C_dib::load_bmp(CTSTR& file_path)
{
	destroy();

	// ファイルからビットマップの読み込み
	HBITMAP h_temp_bitmap = (HBITMAP)LoadImage(G_app.h_inst, file_path.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if (!h_temp_bitmap)
		return false;

	// 読み込んだビットマップからテンポラリ Dib の作成
	DIBSECTION temp_dib;
	if (GetObject(h_temp_bitmap, sizeof(DIBSECTION), &temp_dib) != sizeof(DIBSECTION)) {
		DeleteObject(h_temp_bitmap);
		return false;
	}

	// 対応していないビット数のものをはじく
	if (temp_dib.dsBmih.biBitCount != 24 && temp_dib.dsBmih.biBitCount != 32)	{
		error_box(_T("24bit / 32bit 以外のビットマップには対応していません。"));
		DeleteObject(h_temp_bitmap);
		return false;
	}

	// テンポラリ Dib と同じサイズの 32bit Dib を作成
	create(temp_dib.dsBmih.biWidth, temp_dib.dsBmih.biHeight, 32);

	// データのコピー準備
	int   temp_dipth = temp_dib.dsBmih.biBitCount;
	BYTE* temp_data = (BYTE *)temp_dib.dsBm.bmBits;
	int   temp_bytes_per_line = temp_dib.dsBmih.biSizeImage / height;

	// 32bit の場合
	if (temp_dipth == 32)	{
		for (int y = 0; y < height; y++)
			memcpy(get_ptr() + y * bytes_per_line, temp_data + y * temp_bytes_per_line, bytes_per_line);
	}

	// 24bit の場合
	else if (temp_dipth == 24)	{

		// ■これアセンブラに落とそうぜ＼(^o^)／
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
// png を読み込む
// ================================================================
bool C_dib::load_png(CTSTR& file_path)
{
	destroy();

	// ※ png クラス化してません
	//    途中でエラーが出るとたぶんリークします

	// png ファイルを開く
	C_file file;
	if (!file.open(file_path, _T("rb")))
		return false;

	// png チェック
	png_byte sig[8];
	if (file.read(sig, 8) != 8 || !png_check_sig(sig, 8))
		return false;

	// png 構造体の初期化
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png)
		return false;

	// info 構造体の初期化
	png_infop info = png_create_info_struct(png);
	if (!info)
		return false;

	// png に情報をセット
	png_init_io(png, file.get_fp());
	png_set_sig_bytes(png, 8);
	png_read_info(png, info);

	// 情報の取得
	png_uint_32 png_w, png_h;
	int png_d, color_type;
	png_get_IHDR(png, info, &png_w, &png_h, &png_d, &color_type, NULL, NULL, NULL);

	// 対応していないフォーマットを変換
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

	// dib にあわせて BGR に展開する
	png_set_bgr(png);
	// アルファ情報がない場合アルファ情報をセット（0xFF で初期化）
	if(!(color_type & PNG_COLOR_MASK_ALPHA))
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

	// 情報の再読み込み
	png_read_update_info(png, info);
	png_get_IHDR(png, info, &png_w, &png_h, &png_d, &color_type, NULL, NULL, NULL);

	// PNG_COLOR_TYPE_RGB になっていなかったら失敗
	if (!(color_type & PNG_COLOR_TYPE_RGB))
		return false;

	// dib の作成
	if (!create(int(png_w), int(png_h), 32))
		return false;

	// row の作成
	ARRAY<BYTE *> row(height);
	for (int y = 0; y < height; y++)
		row[y] = get_ptr(0, y);

	// イメージの読み込み
	png_read_image(png, row.get());

	// 構造体の開放
	png_destroy_read_struct(&png, &info, NULL);

	return true;
}

// ****************************************************************
// jpg を読み込む
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

	// jpeg ファイルを開く
	C_file file;
	if (!file.open(file_path, _T("rb")))	return false;

	// エラーの設定
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer))	{
		jpeg_destroy_decompress(&cinfo);
		return false;
	}

	// jpeg の解凍準備
	jpeg_create_decompress(&cinfo);
	// jpeg にファイルをセット
	jpeg_stdio_src(&cinfo, file.get_fp());
	// jpeg ヘッダを読みこみ
	jpeg_read_header(&cinfo, TRUE);
	// jpeg 解凍パラメータを設定（未実装）
	;
	// jpeg を解凍
	jpeg_start_decompress(&cinfo);

	// dib を作成
	if (!create(cinfo.image_width, cinfo.image_height, 32))
		return false;

	// 展開用の一時バッファの作成
	int row_stride = cinfo.output_width * cinfo.output_components;
	JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	// jpeg を dib に展開
	for (int y = 0; cinfo.output_scanline < cinfo.output_height; y++)	{
		jpeg_read_scanlines(&cinfo, buffer, 1);		// １行展開

		// 32bit に展開していく
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

	// jpeg の開放
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	return true;
}
*/

// ****************************************************************
// ファミコンチップを読み込む
// ================================================================
/*
bool C_dib::load_fc_chip(BYTE* gra_data, BYTE* pal, int pal_pos, bool rev_x, bool rev_y)
{
	BYTE* fc_pal = Cpalette::get_instance().p_famicom;

	// dib の作成（固定サイズ）
	if (!create(8, 8, 32))
		return false;

	// ローカルなパレットハッシュを作成
	BYTE* local_pal_p[4];
	local_pal_p[0] = fc_pal + pal[pal_pos * 4]     * 3;
	local_pal_p[1] = fc_pal + pal[pal_pos * 4 + 1] * 3;
	local_pal_p[2] = fc_pal + pal[pal_pos * 4 + 2] * 3;
	local_pal_p[3] = fc_pal + pal[pal_pos * 4 + 3] * 3;

	for (int y = 0; y < 8; y++)	{

		// グラフィックデータの展開
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

		// ２倍に拡大して描画
		BYTE* dst = get_ptr( (rev_x ? 7 : 0), (rev_y ? 7 - y : y) );

		for (int i = 0; i < 8; i++)	{
			BYTE* src = local_pal_p[data[i]] + 2;
			*dst++ = *src--;
			*dst++ = *src--;
			*dst++ = *src--;
			if (data[i] == 0)		*dst++ = 0;
			else					*dst++ = 255;
			dst += rev_x ? -8 : 0;		// さらに次のバイトへ
		}

		gra_data ++;
	}

	return true;
}
*/

// ****************************************************************
// 指定色で塗りつぶす
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
// 点を描画
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
// 矩形を描画
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
// dib を描画
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
// 変換描画
// ================================================================
void C_dib::transform_draw(S_disp_param *dp)
{
	if (!is_init())		return;

	tc_draw_dib(get_ptr(), width, height, center, get_ptr(), width, height, dp);
}

// ****************************************************************
// 拡大（倍率を指定）
// ================================================================
bool C_dib::create_magnified_dib_scale(C_dib* src_dib, int scale_x, int scale_y)
{
	// 新しい dib の作成
	if (!create(src_dib->width * scale_x, src_dib->height * scale_y, src_dib->depth))
		return false;

	for (int y = 0; y < src_dib->height; y++)	{
		DWORD* dst = (DWORD *)get_ptr(0, y * scale_y);
		DWORD* src = (DWORD *)src_dib->get_ptr(0, y);

		// 横に拡張
		for (int x = 0; x < src_dib->width; x++)	{
			for (int s = 0; s < scale_x; s++)	{
				*dst++ = *src;
			}
			src++;
		}
		// 縦に拡張
		for (int yy = 1; yy < scale_y; yy++)	{
			memcpy(get_ptr(0, y * scale_y + yy), get_ptr(0, y * scale_y), width * 4);
		}
	}

	return true;
}

// ****************************************************************
// 拡大（サイズを指定）
// ================================================================
bool C_dib::create_magnified_dib_size(C_dib* src_dib, int dst_w, int dst_h)
{
	int src_w = src_dib->width;
	int src_h = src_dib->height;
	if (!create(dst_w, dst_h, src_dib->depth))
		return false;

	// 縦に拡張
	DWORD* dst_y = (DWORD *)get_ptr(0, 0);
	DWORD* src_y = (DWORD *)src_dib->get_ptr(0, 0);
	DWORD* end_y = (DWORD *)src_dib->get_ptr(0, src_h);
	int h_meter = 0;			// メーター
	while (src_y < end_y)	{
		for (; h_meter < dst_h; h_meter += src_h)	{

			// 横に拡張
			DWORD* dst_x = dst_y;
			DWORD* src_x = src_y;
			DWORD* end_x = src_y + src_w;
			int w_meter = 0;			// メーター
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
// 右に９０度回転
// ================================================================
bool C_dib::create_rotate_90_dib(C_dib* p_src_dib)
{
	// 新しい dib を作成
	if (!create(p_src_dib->height, p_src_dib->width, p_src_dib->depth))
		return false;

	// ビットコピー
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


	// ↓ボツ案。正方形でしか使えないじゃん！！！！！

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
// スクリーンに描画
// ================================================================
bool C_dib::display_to_screen(HDC h_dst_dc)
{
	if (!is_init())
		return false;

	// ディスプレイへ転送（ビットマップハンドルを使わない方法）
	SetDIBitsToDevice(h_dst_dc, 0, 0, width, height, 0, 0, 0, height, get_ptr(), (BITMAPINFO *)&header, DIB_RGB_COLORS);

	// ↓ボツりました（ビットマップハンドルの上限がうっといので）

	//// dib セクションの DC を準備
	//HDC h_screen_dc = GetDC(0);
	//HDC h_dib_dc = CreateCompatibleDC(h_screen_dc);
	//HGDIOBJ h_old_bitmap = SelectObject(h_dib_dc, get_handle());

	//// dib セクションを画面へ描画する
	//BitBlt(h_dst_dc, x, y, w, h, h_dib_dc, x, y, SRCCOPY);

	//// dib セクションの DC を開放
	//GdiFlush();
	//SelectObject(h_dib_dc, h_old_bitmap);
	//DeleteDC(h_dib_dc);
	//if (h_screen_dc)
	//	::ReleaseDC(0, h_screen_dc);

	return true;
}

}
