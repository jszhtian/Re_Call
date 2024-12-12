#include	"tonad3d3_pch.h"
#include	"tonad3d3_device.h"
#include	"tonad3d3_texture.h"
#include	"tonad3d3_sprite.h"

namespace NT3
{

	// ****************************************************************
	// D3D �e�N�X�`��
	// ================================================================
	C_d3d_texture::C_d3d_texture()
	{
		// �p�����[�^�̏�����
		m_width = 0;
		m_height = 0;
		m_width_ex = 0;
		m_height_ex = 0;
		m_org_width = 0;
		m_org_height = 0;
		m_center = C_point(0, 0);
		m_blend = false;
		m_mipmap_enable = false;
		m_dirty = false;
	}

	C_d3d_texture::~C_d3d_texture()
	{
	}

	// ****************************************************************
	// �e�N�X�`���̏�����
	// ================================================================
	void C_d3d_texture::init()
	{
		// �e�N�X�`�������
		release();

		// �p�����[�^�̏�����
		m_width = 0;
		m_height = 0;
		m_width_ex = 0;
		m_height_ex = 0;
		m_org_width = 0;
		m_org_height = 0;
		m_center = C_point(0, 0);
		m_blend = false;
		m_mipmap_enable = false;
		m_dirty = false;
	}

	// ****************************************************************
	// �e�N�X�`���̉��
	// ================================================================
	void C_d3d_texture::release()
	{
		m_texture = NULL;
	}

	// ****************************************************************
	// �e�N�X�`���̍쐬
	// ================================================================
	bool C_d3d_texture::create(int width, int height, UINT mip_level, DWORD usage, D3DFORMAT format, D3DPOOL pool)
	{
		HRESULT hr;

		// �e�N�X�`����������
		init();

		// �p�����[�^�̃R�s�[
		m_width = width;
		m_height = height;
		m_org_width = width;
		m_org_height = height;

		// �傫���� 2^n �ɒ�������B2^n �������Ȃ��O���{�΍�B�������̈Ӗ����B
		// �傫�� 1px ���� GeForce9600 GT �ł������������i�����Ȃ����j�̂ŁA�ŏ� 2px �ɂ��܂��B
		if (!G_d3d.device.can_use_not_pow2_texture())	{
			for (m_width_ex = 2; m_width_ex < m_width; m_width_ex *= 2);
			for (m_height_ex = 2; m_height_ex < m_height; m_height_ex *= 2);
		}
		else	{
			m_width_ex = std::max(m_width, 2);
			m_height_ex = std::max(m_height, 2);
		}

		// 2048�~2048 �܂ł������Ȃ�
		if (m_width_ex > 2048 || m_height_ex > 2048)	{
			set_last_error(_T("2048�~2048 ���傫�ȃe�N�X�`���͍��܂���B"), _T("C_d3d_texture::create"));
			return false;
		}

		// �t�H�[�}�b�g�Ǝg�p���@�`�F�b�N
		if (usage & D3DUSAGE_AUTOGENMIPMAP)	{
			if (pool == D3DPOOL_SYSTEMMEM)	{
				set_last_error(_T("D3DUSAGE_AUTOGENMIPMAP �� D3DPOOL_SYSTEMMEM �͓����ɂ͎w��ł��܂���B"), _T("C_d3d_texture::create"));
				return false;
			}
			else if (!G_d3d.device.can_set_D3DUSAGE_AUTOGENMIPMAP_for_texture())	{
				usage &= ~D3DUSAGE_AUTOGENMIPMAP;	// �~�b�v�}�b�v�̎����������I�t�ɂ���
			}
		}
		if (usage & D3DUSAGE_DYNAMIC)	{
			if (pool == D3DPOOL_MANAGED)	{
				set_last_error(_T("D3DUSAGE_DYNAMIC �� D3DPOOL_MANAGED �͓����ɂ͎w��ł��܂���B"), _T("D3DXCreateTexture"));
				return false;
			}
		}

		// ���~�b�v�}�b�v�͍ő�łP�i�K
		//mip_level = 1;

		// D3DX �𗘗p���ăe�N�X�`�����쐬����
		IDirect3DTexture9* p_texture = NULL;
		hr = D3DXCreateTexture(G_d3d.device.body(), m_width_ex, m_height_ex, mip_level, usage, format, pool, &p_texture);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("CreateTexture"), hr);
			return false;
		}

		// ���I�e�N�X�`���łȂ���΃~�b�v�}�b�v������
		if (!(usage & D3DUSAGE_DYNAMIC))	{
			m_mipmap_enable = true;
		}

		// D3DXCreateTexture �͎Q�ƃJ�E���^�𑝂₷�̂ŁAfalse ���w�肵�܂��B
		m_texture = BIP<IDirect3DTexture9>(p_texture, false);

		return true;
	}

	bool C_d3d_texture::create(int width, int height)
	{
		return create(width, height, 0, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED);
	}

	// ****************************************************************
	// �T�[�t�F�X���擾
	// ================================================================
	C_d3d_surface C_d3d_texture::get_surface(int level)
	{
		HRESULT hr;

		if (!m_texture)
			return C_d3d_surface();

		IDirect3DSurface9* p_surface = NULL;
		hr = m_texture->GetSurfaceLevel(level, &p_surface);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("GetSurfaceLevel"), hr);
			return C_d3d_surface();
		}

		// GetSurfaceLevel �͓����ŎQ�ƃJ�E���^���グ�Ă��܂��BBIP �ŊǗ����Ȃ��ƃ��[�N���Ă��܂��܂��B
		return BIP<IDirect3DSurface9>(p_surface, false);
	}

	// ****************************************************************
	// �e�N�X�`���̃f�[�^���T�[�t�F�X�Ɏ擾
	// ================================================================
	bool C_d3d_texture::get_render_target_data(int level, IDirect3DSurface9* p_dst_surface)
	{
		HRESULT hr;

		if (!m_texture)	{
			set_last_error(_T("�e�N�X�`������������Ă��܂���B"), _T("C_d3d_texture::get_render_target_data"));
			return false;
		}

		// �T�[�t�F�X���擾
		IDirect3DSurface9* p_src_surface = NULL;
		hr = m_texture->GetSurfaceLevel(level, &p_src_surface);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("GetSurfaceLevel"), hr);
			return false;
		}

		// GetSurfaceLevel �͓����ŎQ�ƃJ�E���^���グ�Ă��܂��BBIP �ŊǗ����Ȃ��ƃ��[�N���Ă��܂��܂��B
		BIP<IDirect3DSurface9> bip_surface(p_src_surface, false);

		// �����_�[�^�[�Q�b�g�̃f�[�^���擾����
		hr = G_d3d.device->GetRenderTargetData(p_src_surface, p_dst_surface);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("GetRenderTargetData"), hr);
			return false;
		}

		return true;
	}

	// ****************************************************************
	// �e�N�X�`���̃��b�N
	// ================================================================
	bool C_d3d_texture::lock_rect(UINT level, D3DLOCKED_RECT* texture_bit, C_rect* rect, DWORD flags)
	{
		HRESULT hr;

		if (!m_texture)	{
			set_last_error(_T("�e�N�X�`������������Ă��܂���B"), _T("LockRect"));
			return false;
		}

		hr = m_texture->LockRect(level, texture_bit, rect, flags);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("LockRect"), hr);
			return false;
		}

		return true;
	}

	bool C_d3d_texture::unlock_rect(UINT level)
	{
		HRESULT hr;

		if (!m_texture)	{
			set_last_error(_T("�e�N�X�`������������Ă��܂���B"), _T("UnlockRect"));
			return false;
		}

		hr = m_texture->UnlockRect(level);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("UnlockRect"), hr);
			return false;
		}

		return true;
	}

	// ****************************************************************
	// �e�N�X�`���̓h��Ԃ�
	// ================================================================
	bool C_d3d_texture::clear_color_ex(C_argb color)
	{
		// �e�N�X�`���Ƀr�b�g�f�[�^�̃R�s�[
		D3DLOCKED_RECT texture_bit;
		if (!lock_rect(0, &texture_bit, NULL, 0))
			return false;

		for (int y = 0; y < m_height_ex; y++)	{
			BYTE* dst = (BYTE *)texture_bit.pBits + texture_bit.Pitch * y;
			for (int x = 0; x < m_width_ex; x++)	{
				*dst++ = color.b;
				*dst++ = color.g;
				*dst++ = color.r;
				*dst++ = color.a;
			}
		}

		if (!unlock_rect(0))
			return false;

		return true;
	}

	bool C_d3d_texture::clear_color(C_argb color)
	{
		// �e�N�X�`���Ƀr�b�g�f�[�^�̃R�s�[
		D3DLOCKED_RECT texture_bit;
		if (!lock_rect(0, &texture_bit, NULL, 0))
			return false;

		for (int y = 0; y < m_height; y++)	{
			BYTE* dst = (BYTE *)texture_bit.pBits + texture_bit.Pitch * y;
			for (int x = 0; x < m_width; x++)	{
				*dst++ = color.b;
				*dst++ = color.g;
				*dst++ = color.r;
				*dst++ = color.a;
			}
		}

		if (!unlock_rect(0))
			return false;

		return true;
	}

	// ****************************************************************
	// ��`��`��
	// ================================================================
	bool C_d3d_texture::draw_rect(C_rect rect, C_argb color)
	{
		// �e�N�X�`���Ƀr�b�g�f�[�^�̃R�s�[
		D3DLOCKED_RECT texture_bit;
		if (!lock_rect(0, &texture_bit, NULL, 0))
			return false;

		for (int y = rect.top; y < rect.bottom; y++)	{
			BYTE* dst = (BYTE *)texture_bit.pBits + texture_bit.Pitch * y;
			for (int x = rect.left; x < rect.right; x++)	{
				*dst++ = color.b;
				*dst++ = color.g;
				*dst++ = color.r;
				*dst++ = color.a;
			}
		}

		if (!unlock_rect(0))
			return false;

		return true;
	}

	// ****************************************************************
	// �ȈՂɕ�����ǂݍ��ށi�ȈՎw��j
	// ================================================================
	bool C_d3d_texture::load_moji(TCHAR moji, int moji_size, C_argb moji_color)
	{
		// �t�H���g���w�肷��
		LOGFONT lf;
		ZeroMemory(&lf, sizeof(lf));
		lf.lfCharSet = DEFAULT_CHARSET;//SHIFTJIS_CHARSET;
		lf.lfHeight = moji_size;
		_tcscpy_s(lf.lfFaceName, sizeof(lf.lfFaceName) / sizeof(TCHAR), _T("�l�r �o�S�V�b�N"));

		// ������ǂݍ���
		return load_moji(moji, &lf, moji_color);
	}

	// ****************************************************************
	// �������e�N�X�`���ɓǂݍ��ށi�t�H���g�w��j
	// ================================================================
	bool C_d3d_texture::load_moji(TCHAR moji, LOGFONT* lf, C_argb moji_color)
	{
		// �f�o�C�X�R���e�L�X�g
		HDC h_dc = CreateCompatibleDC(0);

		// �t�H���g�쐬
		HFONT h_font = CreateFontIndirect(lf);

		// �t�H���g��I��
		HGDIOBJ h_old_font = SelectObject(h_dc, (HGDIOBJ)h_font);

		// �n���h�����w�肵�ĕ�����ǂݍ���
		// �T�C�Y�� lfHeight ��n���܂��B�ilfWidth ���� 0 �̉\��������j
		if (!load_moji(moji, lf->lfHeight, moji_color, MOJI_FACE_TYPE_NONE, false, h_dc))
			return false;

		// �n���h�������
		SelectObject(h_dc, (HGDIOBJ)h_old_font);
		DeleteObject(h_font);
		DeleteDC(h_dc);

		return true;
	}

	// ****************************************************************
	// �������e�N�X�`���ɓǂݍ��ށi�f�o�C�X�R���e�L�X�g�w��j
	// ================================================================
	bool C_d3d_texture::load_moji(TCHAR moji, int moji_size, C_argb moji_color, MOJI_FACE_TYPE moji_type, bool tategaki, HDC h_dc)
	{
		int rep_x = 0;		// x ���W���}�C�i�X�ɂȂ����Ƃ��̕␳�p
		int rep_y = 0;		// y ���W���}�C�i�X�ɂȂ����Ƃ��̕␳�p

		// �t�H���g�̏����擾
		TEXTMETRIC tm = {0};
		GetTextMetrics(h_dc, &tm);

		// �c�����^�������̃t�H���g�ɂ���čs�񂪕ς��
		MAT2 mat_yokogaki = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };
		MAT2 mat_tategaki = { { 0, 0 }, { 0, -1 }, { 0, 1 }, { 0, 0 } };

		// �t�H���g�f�[�^�̎擾
		GLYPHMETRICS gm = {0};
		MAT2 mat = tategaki ? mat_tategaki : mat_yokogaki;
		DWORD data_size = GetGlyphOutline(h_dc, moji, GGO_GRAY8_BITMAP, &gm, 0, NULL, &mat);
		if (data_size == GDI_ERROR)
			return false;

		// �ꕔ�t�H���g�� GGO_GRAY8_BITMAP ���w�肷��� 0 ���Ԃ��Ă���B
		// ���̏ꍇ�� GGO_BITMAP �œǂݍ��߂΂����񂾂��ǓW�J���[�`�����Ȃ��Ƃ����Ȃ��B
		// �ł� Vista �ȍ~���� TextOut �ł���`�悳��Ȃ���I�I�I�Ƃ����킯�Ŏc�O�Ȃ���؂�̂Ă܂��B

		// �X�y�[�X
		if (data_size == 0)
		{
			// �����^�C�v�ɂ���ăT�C�Y���v�Z
			int need_size;
			switch (moji_type)	{
				case MOJI_FACE_TYPE_4:			need_size = moji_size + 1;	break;
				case MOJI_FACE_TYPE_9:			need_size = moji_size + 2;	break;
				case MOJI_FACE_TYPE_16:			need_size = moji_size + 3;	break;
				default:						need_size = moji_size;		break;
			}

			// �w�肳�ꂽ�T�C�Y�̃e�N�X�`�����쐬
			if (!create(need_size, need_size))
				return false;

			// �e�N�X�`�������̂͑厖�ł��B�傫���𓖂��蔻��Ɏg���܂��B
		}
		// �X�y�[�X�ȊO
		else
		{
			int baseline_x, baseline_y, x, y, w, h;

			// �t�H���g�̃r�b�g�f�[�^�擾
			ARRAY<BYTE> font_data(data_size);
			GetGlyphOutline(h_dc, moji, GGO_GRAY8_BITMAP, &gm, data_size, font_data.get(), &mat);

			// �x�[�X���C���̈ʒu�����߂�
			// ����̓t�H���g��񂩂�ł͂Ȃ������Ō��߂�K�v������܂��B
			// �l�r �S�V�b�N���Y��ɓ���悤�ɁA�����T�C�Y�� 86% �ɂ��Ă݂܂��B
			// ���̕��@�Ȃ烁�C���I�� internalLeading ���֌W�Ȃ��ł��B
			if (tategaki)
			{
				baseline_x = moji_size - (int)(moji_size * 0.86 + 0.5);
				baseline_y = 0;
			}
			else
			{
				baseline_x = 0;
				baseline_y = (int)(moji_size * 0.86 + 0.5);
			}

			// �r�b�g���o�͂�����W���v�Z
			if (tategaki)
			{
				x = baseline_x + gm.gmptGlyphOrigin.x;
				y = baseline_y - gm.gmptGlyphOrigin.y;
				w = (gm.gmBlackBoxX + 3) & 0xFFFC;		//  + 3) & 0xFFFC = �A���C�����g����
				h = gm.gmBlackBoxY;
			}
			else
			{
				x = baseline_x + gm.gmptGlyphOrigin.x;
				y = baseline_y - gm.gmptGlyphOrigin.y;
				w = (gm.gmBlackBoxX + 3) & 0xFFFC;		//  + 3) & 0xFFFC = �A���C�����g����
				h = gm.gmBlackBoxY;
			}

			// ���f�[�^�T�C�Y�� gmBlackBox ���犄��o�����T�C�Y������Ȃ����Ƃ�����܂��B
			// �i��F�t�H���g = "�l�r �S�V�b�N"�A�T�C�Y = 11�A���� = '��'�j
			// �f�[�^�T�C�Y���傫���ꍇ�͂�������g�p���A���̕���������ɂ��炵�܂��B
			// ���ꉽ���낤�t�H���g�̃o�O�H���Ȃ��H

			// �f�[�^�T�C�Y���獂�������o���Ă݂�
			int real_h = data_size / w;
			if (real_h > h)
			{
				y -= real_h - h;
				h = real_h;
			}

			// �����T�C�Y��␳

			// �}�C�i�X�̏ꍇ�͕␳�i�쐬��ɕ␳����j
			if (y < 0)
			{
				rep_y = - y;
				y = 0;
			}
			if (x < 0)
			{
				rep_x = - x;
				x = 0;
			}
			// ������傫������B�e�N�X�`���T�C�Y���傫���Ȃ�܂��B
			moji_size = std::max(moji_size, y + h);

			// �����^�C�v�ɂ���ăT�C�Y���v�Z
			int need_size;
			switch (moji_type)
			{
			case MOJI_FACE_TYPE_4:			need_size = moji_size + 1;	break;
			case MOJI_FACE_TYPE_9:			need_size = moji_size + 2;	break;
			case MOJI_FACE_TYPE_16:			need_size = moji_size + 3;	break;
			default:						need_size = moji_size;		break;
			}

			// �w�肳�ꂽ�T�C�Y�̃e�N�X�`�����쐬
			if (!create(need_size, need_size))
				return false;

			// �e�N�X�`�������b�N
			D3DLOCKED_RECT texture_bit;
			if (!lock_rect(0, &texture_bit, NULL, 0))
				return false;

			Cfont_copy fc;
			switch (moji_type)
			{
				case MOJI_FACE_TYPE_4:
				case MOJI_FACE_TYPE_9:
				case MOJI_FACE_TYPE_16:
					{
						ARRAY<BYTE> tmp_tc_font_data(moji_size * moji_size * 4);
						fc.gray8_copy(tmp_tc_font_data.get(), moji_size, moji_size, font_data.get(), w, h, x, y, moji_color);
						fc.alphablend_copy_face((BYTE *)texture_bit.pBits, get_width_ex(), get_height_ex(), tmp_tc_font_data.get(), moji_size, moji_size, 0, 0, moji_type);
					}
					break;
				default:					// �ʏ�
					fc.gray8_copy((BYTE *)texture_bit.pBits, get_width_ex(), get_height_ex(), font_data.get(), w, h, x, y, moji_color);
					break;
			}

			// ���S��ݒ�
			m_center.x = rep_x;
			m_center.y = rep_y;

			// �e�N�X�`���̃A�����b�N
			if (!unlock_rect(0))
				return false;
		}

		// �����̓u�����h����
		m_blend = true;

		return true;
	}

	// ****************************************************************
	// �t�@�C������ D3D �ɑΉ������摜���e�N�X�`���ɓǂݍ���
	// ================================================================
	bool C_d3d_texture::load_file(CTSTR& file_path)
	{
		HRESULT hr;

		// �e�N�X�`���̃��[�h
		D3DXIMAGE_INFO image_info;
		IDirect3DTexture9* p_texture = NULL;
		hr = D3DXCreateTextureFromFileEx(G_d3d.device.body(), file_path.c_str(), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, &image_info, NULL, &p_texture);
		if (FAILED(hr))	{
			set_last_error_d3d(file_path + _T(" �̓ǂݍ��݂Ɏ��s���܂����B"), _T("CreateTexture"), hr);
			return false;
		}

		// D3DXCreateTextureFromFileEx �͎Q�ƃJ�E���^�𑝂₷�̂ŁAfalse ���w�肷��
		m_texture = BIP<IDirect3DTexture9>(p_texture, false);

		// �����擾
		m_width  = image_info.Width;
		m_height = image_info.Height;
		m_org_width = image_info.Width;
		m_org_height = image_info.Height;

		// �傫���� 2^n �ɒ���
		for (m_width_ex = 1; m_width_ex < m_width; m_width_ex *= 2);
		for (m_height_ex = 1; m_height_ex < m_height; m_height_ex *= 2);

		// �摜�̓u�����h����
		m_blend = true;

		return true;
	}

	// ****************************************************************
	// g00 ��ǂݍ���
	// ================================================================
	bool C_d3d_texture::load_g00(CTSTR& g00_path)
	{
		// �u�����h�t���O
		m_blend = false;

		// g00 �t�@�C����ǂݍ���
		BUFFER file_data;
		if (!C_file::read_full_data(g00_path, file_data))
			return false;

		// g00 ���쐬
		C_g00 g00;
		if (!g00.set_data(file_data.get(), file_data.size()))
			return false;

		// g00 �̏����擾
		S_g00_info g00_info;
		g00.get_info(&g00_info);

		if (g00_info.cut_cnt > 0)	{

			// �J�b�g�O�Ԃ��擾
			int cut_no = 0;
			C_g00_cut* g00_cut = g00.get_cut(cut_no);
			// �J�b�g�O�Ԃ̏����擾
			S_g00_cut_info cut_info;
			g00_cut->get_info(&cut_info);
			// �e�N�X�`�����쐬
			if (!create(cut_info.width, cut_info.height))
				return false;

			// �e�N�X�`�������b�N
			D3DLOCKED_RECT texture_bit;
			if (!lock_rect(0, &texture_bit, NULL, 0))
				return false;

			// �`�b�v�̏���
			for (int chip_no = 0; chip_no < cut_info.chip_cnt; chip_no++)	{

				// �`�b�v���擾
				C_g00_chip* g00_chip = g00_cut->get_chip(chip_no);
				// �`�b�v�̏����擾
				S_g00_chip_info chip_info;
				g00_chip->get_info(&chip_info);
				g00_chip->get_data((BYTE *)texture_bit.pBits + texture_bit.Pitch * chip_info.y + chip_info.x * 4, texture_bit.Pitch);

				// �u�����h����K�v�̂���`�b�v���������Ȃ�΃u�����h
				if (!m_blend && chip_info.sprite)
					m_blend = true;
			}
			// �e�N�X�`�����A�����b�N
			if (!unlock_rect(0))
				return false;
		}

		return true;
	}

	// ****************************************************************
	// g00 �̃J�b�g��ǂݍ���
	// ================================================================

	// ****************************************************************
	// �A���t�@�����e�[�u���쐬
	// ================================================================

	// �A���t�@�����F�����x�ϊ��e�[�u���쐬�t���O
	bool	Gv_va_graphics_alphablend_table_make_flag = false;

	// �A���t�@�����F�����x�ϊ��e�[�u��
	int		Gv_va_graphics_alphablend_table_tr[256][256];
	int		*Gv_va_graphics_alphablend_table_tr_adr[256];
	DWORD	Gv_va_graphics_alphablend_table_tr_adr_offset;

	// �A���t�@�����F�����e�[�u���P
	int		Gv_va_graphics_alphablend_table_work1[256][256];
	int		*Gv_va_graphics_alphablend_table_work1_adr[256];
	DWORD	Gv_va_graphics_alphablend_table_work1_adr_offset;

	// �A���t�@�����F�����e�[�u���Q
	int		Gv_va_graphics_alphablend_table_work2[256][256];
	int		*Gv_va_graphics_alphablend_table_work2_adr[256];
	DWORD	Gv_va_graphics_alphablend_table_work2_adr_offset;

	// �A���t�@�����F�����x�ϊ��e�[�u��
	int		Gv_va_graphics_alphasub_table_tr[256][256];
	int		*Gv_va_graphics_alphasub_table_tr_adr[256];
	DWORD	Gv_va_graphics_alphasub_table_tr_adr_offset;

	// �A���t�@�����e�[�u���쐬
	void f_make_alphablend_table()
	{
		int i, j;
		int *wp;
	
		// �A���t�@�����F�����x�ϊ��e�[�u��
		wp = Gv_va_graphics_alphablend_table_tr[0];
		for (i = 0; i < 256; i++)	{
			for (j = 0; j < 256; j++)	{
				*wp++ = i + j - ((i * j) / 255);
			}
			Gv_va_graphics_alphablend_table_tr_adr[i] = Gv_va_graphics_alphablend_table_tr[i];
		}
	
		// �A���t�@�����F�����e�[�u���P
		wp = Gv_va_graphics_alphablend_table_work1[0];
		for (i = 0; i < 256; i++)	{
			for (j = 0; j < 256; j++)	{
				*wp++ = (255 - i) * j;
			}
			Gv_va_graphics_alphablend_table_work1_adr[i] = Gv_va_graphics_alphablend_table_work1[i];
		}
	
		// �A���t�@�����F�����e�[�u���Q
		wp = Gv_va_graphics_alphablend_table_work2[0];
		for (i = 0; i < 256; i++)	{
			for (j = 0; j < 256; j++)	{
				*wp++ = 255 * i * j;
			}
			Gv_va_graphics_alphablend_table_work2_adr[i] = Gv_va_graphics_alphablend_table_work2[i];
		}

		// �A���t�@�����F�����x�ϊ��e�[�u��
		wp = Gv_va_graphics_alphasub_table_tr[0];
		for (i = 0; i < 256; i++)	{
			for (j = 0; j < 256; j++)	{
				*wp++ = i * (255 - j) / 255;
			}
			Gv_va_graphics_alphasub_table_tr_adr[i] = Gv_va_graphics_alphasub_table_tr[i];
		}
	
		Gv_va_graphics_alphablend_table_tr_adr_offset = (DWORD)Gv_va_graphics_alphablend_table_tr_adr;	
		Gv_va_graphics_alphablend_table_work1_adr_offset = (DWORD)Gv_va_graphics_alphablend_table_work1_adr;
		Gv_va_graphics_alphablend_table_work2_adr_offset = (DWORD)Gv_va_graphics_alphablend_table_work2_adr;
		Gv_va_graphics_alphasub_table_tr_adr_offset = (DWORD)Gv_va_graphics_alphasub_table_tr_adr;	
	
		Gv_va_graphics_alphablend_table_make_flag = true;
	}

	// ****************************************************************
	// �A���t�@�����A�Z���u����`
	// ================================================================

#if __USE_ASM_INTEL

	// �e�[�u����`
	#define		__TABLE_ALPHABLEND_TR		Gv_va_graphics_alphablend_table_tr_adr_offset
	#define		__TABLE_ALPHABLEND_WORK1	Gv_va_graphics_alphablend_table_work1_adr_offset
	#define		__TABLE_ALPHABLEND_WORK2	Gv_va_graphics_alphablend_table_work2_adr_offset

#endif

	// ****************************************************************
	// �A���t�@�����`��
	//		IN		BYTE *d_buf  : �]����|�C���^
	//				int d_xl     : �]����w�k
	//				int d_yl     : �]����x�k
	//				BYTE *s_buf  : �]�����|�C���^
	//				int s_xl     : �]�����w�k
	//				int s_yl     : �]�����x�k
	//				int x        : �]�����W
	//				int y        : �]�����W
	// ================================================================
	void f_draw_alphablend(BYTE *d_buf, int d_xl, int d_yl, BYTE *s_buf, int s_xl, int s_yl, int x, int y, int blend_type)
	{
		const int _PIXEL_SIZE = 4;	// ���Œ�

		if (s_buf == NULL || s_xl <= 0 || s_yl <= 0 || d_buf == NULL || d_xl <= 0 || d_yl <= 0)
			return;
	
		// �A���t�@�����e�[�u���쐬
		if (!Gv_va_graphics_alphablend_table_make_flag)	{	f_make_alphablend_table();	}
	
		if (x >= d_xl || y >= d_yl)	{ return; }

		int cnt_x, cnt_y, d_free_x, d_free_y, s_free_x, s_free_y;

		if (x < 0) { s_free_x = -x; cnt_x = s_xl + x; d_free_x = 0; }
		else { s_free_x = 0; cnt_x = s_xl; d_free_x = x; }
		if ((d_xl - d_free_x) < cnt_x) { cnt_x = d_xl - d_free_x; }
		if (cnt_x <= 0)	{ return; }

		if (y < 0) { s_free_y = -y;	cnt_y = s_yl + y; d_free_y = 0; }
		else { s_free_y = 0; cnt_y = s_yl; d_free_y = y; }
		if ((d_yl - d_free_y) < cnt_y) { cnt_y = d_yl - d_free_y; }
		if (cnt_y <= 0)	{ return; }

		s_buf += (s_free_x + (s_free_y * s_xl)) * _PIXEL_SIZE;
		d_buf += (d_free_x + (d_free_y * d_xl)) * _PIXEL_SIZE;
		s_free_x = (s_xl - cnt_x) * _PIXEL_SIZE;
		d_free_x = (d_xl - cnt_x) * _PIXEL_SIZE;

#if __USE_ASM_INTEL
		_asm{
			mov		edi, d_buf
			mov		esi, s_buf
			mov		ecx, cnt_y
		__loop_y:
			push	ecx
			mov		ecx, cnt_x
		__loop_x:
		
			// �]�����̓����x�� 0 �̎��͉������Ȃ�
			xor		ebx, ebx
			mov		bl, [esi + 3]
			and		ebx, ebx
			jnz		__step_draw
			jmp		__step_no_draw
		
		__step_draw:
		
			// �]�����̓����x�� 255 �̎��͂��̂܂܃R�s�[
			cmp		ebx, 255
			jne		__step_draw_check_dst_alpha
			movsd
			jmp		__step_next		// ����
		
		__step_draw_check_dst_alpha:
		
			// �]����̓����x�� 0 �̎��͂��̂܂܃R�s�[
			xor		eax, eax
			mov		al, [edi + 3]
			and		eax, eax
			jnz		__step_draw_main
			movsd
			jmp		__step_next		// ����
		
		__step_draw_main:		
			push	ecx

			// �����x���S�{�ɂ���
			shl		ebx, 2					// ebx = �]�����̓����x
			shl		eax, 2					// eax = �]����̓����x
		
			// �����x�̍����i���ʂ� ecx �Ɋi�[�j
			mov		ecx, ebx				// ebx = �]�����̓����x
			add		ecx, __TABLE_ALPHABLEND_TR
			mov		ecx, [ecx]
			add		ecx, eax				// eax = �]����̓����x
			mov		ecx, [ecx]				// ecx = �������ꂽ�����x
		
			// �������ꂽ�����x�� 0 �̎��͉������Ȃ�
			and		ecx, ecx
			jnz		__abcopy_alpha_check_ok
			jmp		__abcopy_alphablend_skip
		
		__abcopy_alpha_check_ok:
		
			// WORK1 �̌��ʒl���Z�o�i���ʂ� edx �Ɋi�[�j
			mov		edx, ebx				// ebx = �]�����̓����x
			add		edx, __TABLE_ALPHABLEND_WORK1
			mov		edx, [edx]
			add		edx, eax				// eax = �]����̓����x
			mov		edx, [edx]				// edx = WORK1 �̌��ʒl

			// WORK2 �̒��Ԍ��ʒl���Z�o�i���ʂ� eax �Ɋi�[�j
			mov		eax, ebx				// ebx = �]�����̓����x
			add		eax, __TABLE_ALPHABLEND_WORK2
			mov		eax, [eax]				// eax = WORK2 �̒��Ԍ��ʒl

			// R
			xor		ebx, ebx
			mov		bl, [esi + 2]
			shl		ebx, 2
			add		ebx, eax				// eax = WORK2 �̒��Ԍ��ʒl
			mov		ebx, [ebx]
			push	ebx						// R push
		
			// G
			xor		ebx, ebx
			mov		bl, [esi + 1]
			shl		ebx, 2
			add		ebx, eax				// eax = WORK2 �̒��Ԍ��ʒl
			mov		ebx, [ebx]
			push	ebx						// G push
		
			// B
			xor		ebx, ebx
			mov		bl, [esi]
			shl		ebx, 2
			add		ebx, eax				// eax = WORK2 �̒��Ԍ��ʒl
			mov		ebx, [ebx]
			push	ebx						// B push

			// ��� mul ���Z�̓s����Aedx �̒l���ω����Ă��܂��̂ŁAebx �ɒl���ڂ�
			mov		ebx, edx				// edx = WORK1 �̌��ʒl
	
			// push / pop �̊֌W������̂� RGB �̏������t���ɂȂ鎖�ɒ���
		
			// B
			xor		eax, eax
			mov		al, [edi]
			mul		ebx						// ebx = WORK1 �̌��ʒl
			pop		edx						// B pop
			add		eax, edx
			shr		eax, 8
			xor		edx, edx				// div �̑O�ɃN���A
			div		ecx						// ecx = �������ꂽ�����x
			mov		[edi], al
		
			// G
			xor		eax, eax
			mov		al, [edi + 1]
			mul		ebx						// ebx = WORK1 �̌��ʒl
			pop		edx						// G pop
			add		eax, edx
			shr		eax, 8
			xor		edx, edx				// div �̑O�ɃN���A
			div		ecx						// ecx = �������ꂽ�����x
			mov		[edi + 1], al
		
			// R
			xor		eax, eax
			mov		al, [edi + 2]
			mul		ebx						// ebx = WORK1 �̌��ʒl
			pop		edx						// R pop
			add		eax, edx
			shr		eax, 8
			xor		edx, edx				// div �̑O�ɃN���A
			div		ecx						// ecx = �������ꂽ�����x
			mov		[edi + 2], al

			// A
			mov		[edi + 3], cl
		
		__abcopy_alphablend_skip:
			pop		ecx
		
		__step_no_draw:
			add		edi, _PIXEL_SIZE
			add		esi, _PIXEL_SIZE
		__step_next:
			dec		ecx
			jz		__loop_x_end
			jmp		__loop_x
		__loop_x_end:
			add		edi, d_free_x
			add		esi, s_free_x
			pop		ecx
			dec		ecx
			jz		__loop_y_end
			jmp		__loop_y
		__loop_y_end:
		}
#else

		// �Q�l �� http://misohena.jp/article/pixel_composite.html

		if (blend_type == E_d3d_blend_type_add)
		{
			BYTE *dp = d_buf;
			BYTE *sp = s_buf;
			int d_a, s_a, r_a;
			for (int i = 0; i < cnt_y; i++)	{
				for (int j = 0; j < cnt_x; j++)	{
					s_a = *(sp + 3);
					if (s_a)	{
						d_a = *(dp + 3);
						r_a = 255 - (255 - s_a) * (255 - d_a) / 255;
						for (int k = 0; k < 3; k++)	{
							int d_c = *(dp + k);
							int s_c = *(sp + k);
							int r_c = (s_a * d_a * std::min(s_c + d_c, 255) + s_a * (255 - d_a) * s_c + (255 - s_a) * d_a * d_c) / r_a / 255;
							*(dp + k) = r_c;
						}
						*(dp + 3) = r_a;
					}
					dp += _PIXEL_SIZE;
					sp += _PIXEL_SIZE;
				}
				dp += d_free_x;
				sp += s_free_x;
			}
		}
		else if (blend_type == E_d3d_blend_type_mul)
		{
			BYTE *dp = d_buf;
			BYTE *sp = s_buf;
			int d_a, s_a, r_a;
			for (int i = 0; i < cnt_y; i++)	{
				for (int j = 0; j < cnt_x; j++)	{
					s_a = *(sp + 3);
					if (s_a)	{
						d_a = *(dp + 3);
						r_a = 255 - (255 - s_a) * (255 - d_a) / 255;
						for (int k = 0; k < 3; k++)	{
							int d_c = *(dp + k);
							int s_c = *(sp + k);
							int r_c = (s_a * d_a * (s_c * d_c / 255) + s_a * (255 - d_a) * s_c + (255 - s_a) * d_a * d_c) / r_a / 255;
							*(dp + k) = r_c;
						}
						*(dp + 3) = r_a;
					}
					dp += _PIXEL_SIZE;
					sp += _PIXEL_SIZE;
				}
				dp += d_free_x;
				sp += s_free_x;
			}
		}
		else
		{
			BYTE *dp = d_buf;
			BYTE *sp = s_buf;
			int d_tr, s_tr, tr, work1;
			int *work2;
			for (int i = 0; i < cnt_y; i++)	{
				for (int j = 0; j < cnt_x; j++)	{
					s_tr = *(sp + 3);
					if (s_tr)	{
						if (s_tr == 255)	{
							// �]�����̓����x�� 255 �̎��͂��̂܂܃R�s�[
							*(DWORD *)dp = *(DWORD *)sp;
						}
						else	{
							d_tr = *(dp + 3);
							if (d_tr == 0)	{
								// �]����̓����x�� 0 �̎��͂��̂܂܃R�s�[
								*(DWORD *)dp = *(DWORD *)sp;
							}
							else	{
								tr = *(*(Gv_va_graphics_alphablend_table_tr_adr + s_tr) + d_tr);
								if (tr)	{
									work1 = *(*(Gv_va_graphics_alphablend_table_work1_adr + s_tr) + d_tr);
									work2 = *(Gv_va_graphics_alphablend_table_work2_adr + s_tr);
									*dp = ((*(work2 + *sp) + (work1 * *dp)) >> 8) / tr;
									*(dp + 1) = ((*(work2 + *(sp + 1)) + (work1 * *(dp + 1))) >> 8) / tr;
									*(dp + 2) = ((*(work2 + *(sp + 2)) + (work1 * *(dp + 2))) >> 8) / tr;
									*(dp + 3) = (BYTE)tr;
								}
							}
						}
					}
					dp += _PIXEL_SIZE;
					sp += _PIXEL_SIZE;
				}
				dp += d_free_x;
				sp += s_free_x;
			}
		}
#endif
	}


	bool C_d3d_texture::load_g00_cut(C_g00_cut* g00_cut, bool futidori)
	{
		int BURE = 1;	// �����쐬���̃u���̑傫��

		// �u�����h�t���O
		m_blend = true;

		// �J�b�g�̏����擾
		S_g00_cut_info cut_info;
		g00_cut->get_info(&cut_info);

		// �e�N�X�`�����쐬�i�I���W�i���T�C�Y�ł͂Ȃ��ŏ���`�������쐬����j
		// �����ꍇ�� 2px ���傫�����
		if (futidori)	{
			if (!create(cut_info.disp_rect.right - cut_info.disp_rect.left + BURE * 2, cut_info.disp_rect.bottom - cut_info.disp_rect.top + BURE * 2))
				return false;
		}
		else	{
			if (!create(cut_info.disp_rect.right - cut_info.disp_rect.left, cut_info.disp_rect.bottom - cut_info.disp_rect.top))
				return false;
		}

		// �I���W�i���̑傫����ݒ�
		m_org_width = cut_info.width;
		m_org_height = cut_info.height;

		// �e�N�X�`�������b�N
		D3DLOCKED_RECT texture_bit;
		if (!lock_rect(0, &texture_bit, NULL, 0))
			return false;

		// �e�N�X�`���𓧖��ɓh��Ԃ�
		for (int y = 0; y < m_height_ex; y++)
			ZeroMemory((BYTE *)texture_bit.pBits + texture_bit.Pitch * y, m_width_ex * sizeof(C_argb));

		// �`�b�v�̏���
		if (futidori)	{

			// �����ꍇ�́A�_�~�[�̃r�b�g�}�b�v�Ɉ�x�W�J����
			// �e�N�X�`���ɂX�����ɕ`�悷�邱�Ƃŉ���肷��
			// ���\�d�������ł��B

			// �_�~�[�̃r�b�g�}�b�v������
			// m_width �Ȃǂ��g���Ă͂����܂���B����蕪�̃T�C�Y�������Ă��邩��ł��B
			C_dib dib;
			dib.create(cut_info.disp_rect.right - cut_info.disp_rect.left, cut_info.disp_rect.bottom - cut_info.disp_rect.top, 32);

			// �e�N�X�`���ɑS�`�b�v��`�悵�āA�P���̃J�b�g���\�z����
			for (int chip_no = 0; chip_no < cut_info.chip_cnt; chip_no++)	{

				// �`�b�v���擾
				C_g00_chip* g00_chip = g00_cut->get_chip(chip_no);
				// �`�b�v�̏����擾
				S_g00_chip_info chip_info;
				g00_chip->get_info(&chip_info);
				g00_chip->get_data(dib.get_ptr(chip_info.x - cut_info.disp_rect.left, chip_info.y - cut_info.disp_rect.top), dib.get_width() * 4);
			}

			// �e�N�X�`���ɂX�����ɕ`�悷�邱�Ƃŉ�������������
			f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), 0, 0, E_d3d_blend_type_alpha);
			f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), BURE * 2, 0, E_d3d_blend_type_alpha);
			f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), 0, BURE * 2, E_d3d_blend_type_alpha);
			f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), BURE * 2, BURE * 2, E_d3d_blend_type_alpha);
			f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), BURE * 1, 0, E_d3d_blend_type_alpha);
			f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), 0, BURE * 1, E_d3d_blend_type_alpha);
			f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), BURE * 2, BURE * 1, E_d3d_blend_type_alpha);
			f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), BURE * 1, BURE * 2, E_d3d_blend_type_alpha);
			f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), BURE * 1, BURE * 1, E_d3d_blend_type_alpha);

			// �u�����h����
			m_blend = true;
		}
		else	{

			// �e�N�X�`���ɑS�`�b�v��`�悵�āA�P���̃J�b�g���\�z����
			for (int chip_no = 0; chip_no < cut_info.chip_cnt; chip_no++)	{

				// �`�b�v���擾
				C_g00_chip* g00_chip = g00_cut->get_chip(chip_no);
				// �`�b�v�̏����擾
				S_g00_chip_info chip_info;
				g00_chip->get_info(&chip_info);
				g00_chip->get_data((BYTE *)texture_bit.pBits + texture_bit.Pitch * (chip_info.y - cut_info.disp_rect.top) + (chip_info.x - cut_info.disp_rect.left) * 4, texture_bit.Pitch);

				// �G�b�W�����i�ׂ̃s�N�Z���ɓ����F��u�������j
				// �⊮�g�債���Ƃ��A�ׂɍ�������ƍ����ɂ���ł��܂��̂�h�����߂̏����ł��B
				add_edge_for_filterring(&texture_bit);

				// �u�����h����K�v�̂���`�b�v���������Ȃ�΃u�����h
				if (!m_blend && chip_info.sprite)
					m_blend = true;
			}
		}

		// �e�N�X�`�����A�����b�N
		if (!unlock_rect(0))
			return false;

		// ���S���W��ݒ�
		set_center(C_point(cut_info.center) - C_point(cut_info.disp_rect.left, cut_info.disp_rect.top));

		return true;
	}

	// ****************************************************************
	// g00 �J�b�g����������
	// ================================================================
	bool C_d3d_texture::draw_g00_cut(C_g00_cut* g00_cut, int x, int y, int blend_type)
	{
		// �e�N�X�`�������b�N
		D3DLOCKED_RECT texture_bit;
		if (!lock_rect(0, &texture_bit, NULL, 0))
			return false;

		// �J�b�g�̏����擾
		S_g00_cut_info cut_info;
		g00_cut->get_info(&cut_info);

		// �_�~�[�̃r�b�g�}�b�v������
		C_dib dib;
		dib.create(cut_info.disp_rect.right - cut_info.disp_rect.left, cut_info.disp_rect.bottom - cut_info.disp_rect.top, 32);

		// �e�N�X�`���ɑS�`�b�v��`�悵�āA�P���̃J�b�g���\�z����
		for (int chip_no = 0; chip_no < cut_info.chip_cnt; chip_no++)	{

			// �`�b�v���擾
			C_g00_chip* g00_chip = g00_cut->get_chip(chip_no);
			// �`�b�v�̏����擾
			S_g00_chip_info chip_info;
			g00_chip->get_info(&chip_info);
			g00_chip->get_data(dib.get_ptr(chip_info.x - cut_info.disp_rect.left, chip_info.y - cut_info.disp_rect.top), dib.get_width() * 4);
		}

		// ���S�␳
		x += m_center.x + cut_info.disp_rect.left - cut_info.center.x;
		y += m_center.y + cut_info.disp_rect.top - cut_info.center.y;

		// �e�N�X�`���ɕ`��
		f_draw_alphablend((BYTE *)texture_bit.pBits, texture_bit.Pitch / 4, m_height, dib.get_ptr(), dib.get_width(), dib.get_height(), x, y, blend_type);

		// �e�N�X�`�����A�����b�N
		if (!unlock_rect(0))
			return false;

		return true;
	}

	// ****************************************************************
	// �t�B���^�����O�����̂��߂ɃG�b�W��������i���b�N���Ȃ��j
	// ================================================================
	bool C_d3d_texture::add_edge_for_filterring(D3DLOCKED_RECT* p_texture_bit)
	{
		// ��
		if (m_height_ex > m_height)	{
			BYTE* p_dst = (BYTE *)p_texture_bit->pBits + p_texture_bit->Pitch * m_height;
			BYTE* p_src = p_dst - p_texture_bit->Pitch;
			memcpy(p_dst, p_src, m_width * sizeof(C_argb));
		}
		// �E
		if (m_width_ex > m_width)	{
			BYTE* p_dst = (BYTE *)p_texture_bit->pBits + m_width * sizeof(C_argb);
			BYTE* p_src = p_dst - sizeof(C_argb);
			for (int y = 0; y < m_height; y++)	{
				*(C_argb *)p_dst = *(C_argb *)p_src;
				p_dst += p_texture_bit->Pitch;
				p_src += p_texture_bit->Pitch;
			}
			// �E��
			if (m_height_ex > m_height)	{
				*(C_argb *)p_dst = *(C_argb *)p_src;
			}
		}

		return true;
	}

	// ****************************************************************
	// �s�N�Z���̃J���[���擾
	// ================================================================
	C_argb C_d3d_texture::get_color(int x, int y)
	{
		// ���S���ʒu�����炷
		x += m_center.x;
		y += m_center.y;

		// �͈̓`�F�b�N
		if (x < 0 || m_width <= x || y < 0 || m_height <= y)
			return C_argb(0, 0, 0, 0);

		// �e�N�X�`�������b�N
		D3DLOCKED_RECT texture_bit;
		if (!lock_rect(0, &texture_bit, NULL, 0))
			return C_argb(0, 0, 0, 0);

		C_argb color = *(C_argb *)(((BYTE *)texture_bit.pBits + texture_bit.Pitch * y) + x * sizeof(C_argb));

		// �e�N�X�`�����A�����b�N
		if (!unlock_rect(0))
			return C_argb(0, 0, 0, 0);

		return color;
	}
}
