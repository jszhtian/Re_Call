#include	"tona3_pch.h"
#include	"tona3_util.h"
#include	"tona3_font.h"

namespace NT3
{
	// ****************************************************************
	// �t�H���g���X�g�쐬
	// ================================================================
	bool C_font_name_list::make_font_list(HWND h_wnd, bool truetype_only, bool toufuku_only, BYTE char_set, FONT_YOKOTATE_MODE yokotate_mode)
	{
		// �t�H���g���X�g�J��
		m_font_list.clear();

		// �����ϐ��ݒ�
		work.truetype_only = truetype_only;
		work.toufuku_only = toufuku_only;
		work.char_set = char_set;
		work.yokotate_mode = yokotate_mode;

		// �񋓂���t�H���g���X�g
		LOGFONT logfont = { 0 };
		logfont.lfCharSet = DEFAULT_CHARSET;	// �S�ė�

		// ���X�g�쐬�R�[���o�b�N
		HDC hdc = GetDC(h_wnd);
		EnumFontFamiliesEx(hdc, &logfont, (FONTENUMPROC)&make_callback_func, (LPARAM)&work, 0);
		ReleaseDC(h_wnd, hdc);

		// �c���̗����ɑΉ��̃t�H���g���X�g���쐬����
		if (work.yokotate_mode == FONT_YOKOTATE_MODE_BOTH)	{
			if (!work.yoko_gaki_font_list.empty() && !work.tate_gaki_font_list.empty())	{
				for (ARRAY<ENUMLOGFONT>::iterator yoko_itr = work.yoko_gaki_font_list.begin(); yoko_itr != work.yoko_gaki_font_list.end(); ++yoko_itr)	{
					TSTR comp_name = _T('@') + yoko_itr->elfLogFont.lfFaceName;
					for (ARRAY<ENUMLOGFONT>::iterator tate_itr = work.tate_gaki_font_list.begin(); tate_itr != work.tate_gaki_font_list.end(); ++tate_itr)	{
						if (comp_name == tate_itr->elfLogFont.lfFaceName)	{
							m_font_list.push_back(*yoko_itr);
							break;
						}
					}
				}
			}
		}
		else	{
			m_font_list = work.temp_font_list;		// �p�u���b�N�̃t�H���g���X�g�ɃR�s�[����
		}

		// ���O���Ƀ\�[�g
		std::sort(m_font_list.begin(), m_font_list.end(), font_less_func);

		// �w�肳�ꂽ�����R�[�h�Z�b�g�ōi�荞�݁i��Ń\�[�g���Ă邱�ƑO��̃A���S���Y���ł��j
		if (char_set != DEFAULT_CHARSET)	{

			// ��Ɨ̈�Ɛ؂�ւ�
			work.temp_font_list = m_font_list;
			m_font_list.clear();

			// ANSI �̏ꍇ�͓���i�f�o�b�O�̂��� SHIFT_JIS �D��j
			if (char_set == ANSI_CHARSET)	{
				ENUMLOGFONT last_font;
				for (ARRAY<ENUMLOGFONT>::iterator itr = work.temp_font_list.begin(); itr != work.temp_font_list.end(); ++itr)	{

					if (TSTR(itr->elfLogFont.lfFaceName) == TSTR(last_font.elfLogFont.lfFaceName))	{
						if (last_font.elfLogFont.lfCharSet == ANSI_CHARSET && itr->elfLogFont.lfCharSet == SHIFTJIS_CHARSET)	{
							m_font_list.pop_back();			// ANSI ���폜����
							m_font_list.push_back(*itr);	// SHIFTJIS ��ǉ�
							last_font = *itr;
						}
					}
					else	{
						if (itr->elfLogFont.lfCharSet == ANSI_CHARSET || itr->elfLogFont.lfCharSet == SHIFTJIS_CHARSET)	{
							m_font_list.push_back(*itr);
							last_font = *itr;
						}
					}
				}
			}
			// ANSI �ȊO���w�肳�ꂽ�ꍇ�͂���̂ݗ�
			else	{
				for (ARRAY<ENUMLOGFONT>::iterator itr = work.temp_font_list.begin(); itr != work.temp_font_list.end(); ++itr)	{
					if (itr->elfLogFont.lfCharSet == char_set)	{
						m_font_list.push_back(*itr);
					}
				}
			}
		}

		// ��ƕϐ��J��
		work.temp_font_list.clear();
		work.yoko_gaki_font_list.clear();
		work.tate_gaki_font_list.clear();

		// �t�H���g���Ȃ�
		if (m_font_list.empty())	{ return false; }

		return true;
	}

	// ****************************************************************
	// �t�H���g���X�g�쐬�R�[���o�b�N�֐�
	// ================================================================
	LRESULT CALLBACK C_font_name_list::make_callback_func(ENUMLOGFONT *elf, NEWTEXTMETRIC *lpntm, int type, LPARAM lp)
	{
		WORK *work = (WORK *)lp;

		// ���{�ꔻ��
		//if (work->japanese_only && elf->elfLogFont.lfCharSet != SHIFTJIS_CHARSET)	{
		//	return 1;
		//}

		// TrueType ����
		if (work->truetype_only && !(type & TRUETYPE_FONTTYPE))	{
			return 1;
		}

		// ��������
		if (work->toufuku_only && (elf->elfLogFont.lfPitchAndFamily & 0x03) == VARIABLE_PITCH)	{
			return 1;
		}

		// ���O�ւ̃|�C���^
		TCHAR *name_p = &elf->elfLogFont.lfFaceName[0];

		// ����������
		if (work->yokotate_mode == FONT_YOKOTATE_MODE_YOKO_ONLY && *name_p == '@')	{
			return 1;
		}

		// �c��������
		if (work->yokotate_mode == FONT_YOKOTATE_MODE_TATE_ONLY && *name_p != '@')	{
			return 1;
		}

		// �c���̗����ɑΉ�
		if (work->yokotate_mode == FONT_YOKOTATE_MODE_BOTH)
		{
			if (*name_p != '@')	{ work->yoko_gaki_font_list.push_back(*elf); }
			else				{ work->tate_gaki_font_list.push_back(*elf); }
		}
		// �c���̕Е��ɑΉ�
		else
		{
			work->temp_font_list.push_back(*elf);
		}

		return 1;
	}

	// ****************************************************************
	// �t�H���g����
	// ================================================================
	bool C_font_name_list::find_font(HWND h_wnd, bool truetype_only, bool toufuku_only, BYTE char_set, FONT_YOKOTATE_MODE yokotate_mode, TSTR font_name)
	{
		// �t�H���g���X�g���Ȃ�������쐬����
		bool font_list_make_flag = false;
		if (m_font_list.empty())	{
			make_font_list(h_wnd, truetype_only, toufuku_only, char_set, yokotate_mode);
			if (m_font_list.empty())	{
				return false;
			}
			font_list_make_flag = true;
		}

		// ����
		bool find_flag = false;
		ARRAY<ENUMLOGFONT>::iterator itr = m_font_list.begin();
		for (; itr != m_font_list.end(); ++itr)	{
			if (itr->elfLogFont.lfFaceName == font_name)	{
				find_flag = true;
				break;
			}
		}

		// �t�H���g���X�g���쐬�����ꍇ�͊J������
		if (font_list_make_flag)	{
			free_font_list();
		}

		return find_flag;
	}

	// ****************************************************************
	// �U�U�K���t�H���g���R�Q�r�b�g�ɃR�s�[�iGGO_GRAY8_BITMAP �� 32bitBMP�j
	//		IN		BYTE *d_buf  : �]����|�C���^
	//				int d_xl     : �]����w�k
	//				int d_yl     : �]����x�k
	//				BYTE *s_buf  : �]�����|�C���^
	//				int s_xl     : �]�����w�k
	//				int s_yl     : �]�����x�k
	//				int x        : �]�����W
	//				int y        : �]�����W
	//				C_argb color  : �J���[
	// ================================================================
	void Cfont_copy::gray8_copy(BYTE *d_buf, int d_xl, int d_yl, BYTE *s_buf, int s_xl, int s_yl, int x, int y, C_argb color)
	{
		// �J���[�e�[�u���쐬
		C_argb rgba[66];
		fill_memory_4(rgba, 66, color);
		BYTE alpha = color.a;
		BYTE *p_rgba = (BYTE *)rgba + 3;
		for (int i = 0; i < 66; i++)	{	*p_rgba = (alpha * i) / 65;	p_rgba += 4;	}
		p_rgba = (BYTE *)rgba;

		int cnt_x, cnt_y, d_free_x, d_free_y, s_free_x, s_free_y;

		if (x >= d_xl || y >= d_yl)	{	return;	}

		if (x < 0)	{	s_free_x = -x;	cnt_x = s_xl + x;	d_free_x = 0;	}
		else		{	s_free_x = 0;	cnt_x = s_xl;		d_free_x = x;	}
		if ((d_xl - d_free_x) < cnt_x)	{	cnt_x = d_xl - d_free_x;	}
		if (cnt_x <= 0)	{	return;	}

		if (y < 0)	{	s_free_y = -y;	cnt_y = s_yl + y;	d_free_y = 0;	}
		else		{	s_free_y = 0;	cnt_y = s_yl;		d_free_y = y;	}
		if ((d_yl - d_free_y) < cnt_y)	{	cnt_y = d_yl - d_free_y;	}
		if (cnt_y <= 0)	{	return;	}

		s_buf += (s_free_x + (s_free_y * s_xl));
		d_buf += (d_free_x + (d_free_y * d_xl)) * 4;
		s_free_x = (s_xl - cnt_x);
		d_free_x = (d_xl - cnt_x) * 4;

		_asm{
			mov		edi, d_buf
			mov		esi, s_buf
			mov		edx, p_rgba
			mov		ecx, cnt_y

		__loop_y:
			push	ecx
			mov		ecx, cnt_x

		__loop_x:
			mov		ebx, edx
			xor		eax, eax
			lodsb
			shl		eax, 2
			add		ebx, eax
			mov		eax, [ebx]
			stosd
			loop	__loop_x

			add		edi, d_free_x
			add		esi, s_free_x
			pop		ecx
			loop	__loop_y
		}
	}

	// ****************************************************************
	// �A���t�@�����e�[�u���쐬
	// ================================================================

	// �A���t�@�����F�����x�ϊ��e�[�u���쐬�t���O
	bool	Gfont_alphablend_table_make_flag = false;

	// �A���t�@�����F�����x�ϊ��e�[�u��
	int		Gfont_alphablend_table_tr[256][256];
	int		*Gfont_alphablend_table_tr_adr[256];

	// �A���t�@�����F�����e�[�u���P
	int		Gfont_alphablend_table_work1[256][256];
	int		*Gfont_alphablend_table_work1_adr[256];

	// �A���t�@�����F�����e�[�u���Q
	int		Gfont_alphablend_table_work2[256][256];
	int		*Gfont_alphablend_table_work2_adr[256];

	// �A���t�@�����e�[�u���쐬
	void Cfont_copy::make_alphablend_table()
	{
		int i, j;
		int *wp;

		// �A���t�@�����F�����x�ϊ��e�[�u��
		wp = Gfont_alphablend_table_tr[0];
		for (i = 0; i < 256; i++)	{
			for (j = 0; j < 256; j++)	{
				*wp++ = i + j - ((i * j) / 255);
			}
			Gfont_alphablend_table_tr_adr[i] = Gfont_alphablend_table_tr[i];
		}

		// �A���t�@�����F�����e�[�u���P
		wp = Gfont_alphablend_table_work1[0];
		for (i = 0; i < 256; i++)	{
			for (j = 0; j < 256; j++)	{
				*wp++ = (255 - i) * j;
			}
			Gfont_alphablend_table_work1_adr[i] = Gfont_alphablend_table_work1[i];
		}

		// �A���t�@�����F�����e�[�u���Q
		wp = Gfont_alphablend_table_work2[0];
		for (i = 0; i < 256; i++)	{
			for (j = 0; j < 256; j++)	{
				*wp++ = 255 * i * j;
			}
			Gfont_alphablend_table_work2_adr[i] = Gfont_alphablend_table_work2[i];
		}

		Gfont_alphablend_table_make_flag = true;
	}

	// ****************************************************************
	// �A���t�@�����A�Z���u����`
	// ================================================================

	// �e�[�u����`
	#define		__TABLE_ALPHABLEND_TR		OFFSET Gfont_alphablend_table_tr_adr
	#define		__TABLE_ALPHABLEND_WORK1	OFFSET Gfont_alphablend_table_work1_adr
	#define		__TABLE_ALPHABLEND_WORK2	OFFSET Gfont_alphablend_table_work2_adr

	// ================================================================

	// �`�挳�s�N�Z���q���擾�i�d�a�w���W�X�^�Ɏ擾�j
	#define		__GET_SRC_R_EBX \
	__asm				xor		ebx, ebx \
	__asm				mov		bl, [esi+2]

	// �`�挳�s�N�Z���f���擾�i�d�a�w���W�X�^�Ɏ擾�j
	#define		__GET_SRC_G_EBX \
	__asm				xor		ebx, ebx \
	__asm				mov		bl, [esi+1]

	// �`�挳�s�N�Z���a���擾�i�d�a�w���W�X�^�Ɏ擾�j
	#define		__GET_SRC_B_EBX \
	__asm				xor		ebx, ebx \
	__asm				mov		bl, [esi]

	// �`�挳�s�N�Z���`���擾�i�d�a�w���W�X�^�Ɏ擾�j
	#define		__GET_SRC_A_EBX \
	__asm				xor		ebx, ebx \
	__asm				mov		bl, [esi+3]

	// ================================================================

	// �`�挳�s�N�Z���q���擾�i�d�`�w���W�X�^�Ɏ擾�j
	#define		__GET_SRC_R_EAX \
	__asm				xor		eax, eax \
	__asm				mov		al, [esi+2]

	// �`�挳�s�N�Z���f���擾�i�d�`�w���W�X�^�Ɏ擾�j
	#define		__GET_SRC_G_EAX \
	__asm				xor		eax, eax \
	__asm				mov		al, [esi+1]

	// �`�挳�s�N�Z���a���擾�i�d�`�w���W�X�^�Ɏ擾�j
	#define		__GET_SRC_B_EAX \
	__asm				xor		eax, eax \
	__asm				mov		al, [esi]

	// �`�挳�s�N�Z���`���擾�i�d�`�w���W�X�^�Ɏ擾�j
	#define		__GET_SRC_A_EAX \
	__asm				xor		eax, eax \
	__asm				mov		al, [esi+3]

	// ================================================================

	// �`���s�N�Z���q���擾�i�d�a�w���W�X�^�Ɏ擾�j
	#define		__GET_DST_R_EBX \
	__asm				xor		ebx, ebx \
	__asm				mov		bl, [edi+2]

	// �`���s�N�Z���f���擾�i�d�a�w���W�X�^�Ɏ擾�j
	#define		__GET_DST_G_EBX \
	__asm				xor		ebx, ebx \
	__asm				mov		bl, [edi+1]

	// �`���s�N�Z���a���擾�i�d�a�w���W�X�^�Ɏ擾�j
	#define		__GET_DST_B_EBX \
	__asm				xor		ebx, ebx \
	__asm				mov		bl, [edi]

	// �`���s�N�Z���`���擾�i�d�a�w���W�X�^�Ɏ擾�j
	#define		__GET_DST_A_EBX \
	__asm				xor		ebx, ebx \
	__asm				mov		bl, [edi+3]

	// ================================================================

	// �`���s�N�Z���q���擾�i�d�`�w���W�X�^�Ɏ擾�j
	#define		__GET_DST_R_EAX \
	__asm				xor		eax, eax \
	__asm				mov		al, [edi+2]

	// �`���s�N�Z���f���擾�i�d�`�w���W�X�^�Ɏ擾�j
	#define		__GET_DST_G_EAX \
	__asm				xor		eax, eax \
	__asm				mov		al, [edi+1]

	// �`���s�N�Z���a���擾�i�d�`�w���W�X�^�Ɏ擾�j
	#define		__GET_DST_B_EAX \
	__asm				xor		eax, eax \
	__asm				mov		al, [edi]

	// �`���s�N�Z���`���擾�i�d�`�w���W�X�^�Ɏ擾�j
	#define		__GET_DST_A_EAX \
	__asm				xor		eax, eax \
	__asm				mov		al, [edi+3]

	// ================================================================

	// �`���s�N�Z���q��ݒ�i�d�`�w���W�X�^��ݒ�j
	#define		__SET_DST_R_EAX \
	__asm				mov		[edi+2], al

	// �`���s�N�Z���f��ݒ�i�d�`�w���W�X�^��ݒ�j
	#define		__SET_DST_G_EAX \
	__asm				mov		[edi+1], al

	// �`���s�N�Z���a��ݒ�i�d�`�w���W�X�^��ݒ�j
	#define		__SET_DST_B_EAX \
	__asm				mov		[edi], al

	// �`���s�N�Z���`��ݒ�i�d�`�w���W�X�^��ݒ�j
	#define		__SET_DST_A_EAX \
	__asm				mov		[edi+3], al

	// ================================================================

	// �`���s�N�Z���q��ݒ�i�d�a�w���W�X�^��ݒ�j
	#define		__SET_DST_R_EBX \
	__asm				mov		[edi+2], bl

	// �`���s�N�Z���f��ݒ�i�d�a�w���W�X�^��ݒ�j
	#define		__SET_DST_G_EBX \
	__asm				mov		[edi+1], bl

	// �`���s�N�Z���a��ݒ�i�d�a�w���W�X�^��ݒ�j
	#define		__SET_DST_B_EBX \
	__asm				mov		[edi], bl

	// �`���s�N�Z���`��ݒ�i�d�a�w���W�X�^��ݒ�j
	#define		__SET_DST_A_EBX \
	__asm				mov		[edi+3], bl

	// ================================================================

	// �A���t�@�l���Q�T�T�ɐݒ�
	#define		__SET_DST_A_255 \
	__asm				mov		[edi+3], 255

	// ================================================================

	static int		alphablend_a;		// �A���t�@�����p�F�����x
	static int		alphablend_r;		// �A���t�@�����p�F�q
	static int		alphablend_g;		// �A���t�@�����p�F�f
	static int		alphablend_b;		// �A���t�@�����p�F�a

	#define		__ALPHABLEND_A		alphablend_a
	#define		__ALPHABLEND_R		alphablend_r
	#define		__ALPHABLEND_G		alphablend_g
	#define		__ALPHABLEND_B		alphablend_b

	// ================================================================

	// �A���t�@����
	#define		__SET_R_BLD			__SET_DST_R_EAX
	#define		__SET_G_BLD			__SET_DST_G_EAX
	#define		__SET_B_BLD			__SET_DST_B_EAX

	// �i�����x�j
	#define		__SET_A_BLD \
						__GET_SRC_A_EAX \
						__SET_DST_A_EAX

	// �����x �A���t�@����
	#define		__SET_BLD_TR_FUNC(COLOR) \
	__asm				mov		__ALPHABLEND_##COLOR, ebx \

	#define		__SET_R_BLD_TR		__SET_BLD_TR_FUNC(R)
	#define		__SET_G_BLD_TR		__SET_BLD_TR_FUNC(G)
	#define		__SET_B_BLD_TR		__SET_BLD_TR_FUNC(B)

	// �A���t�@��������
	#define		__READY_ALPHABLEND(COLOR) \
	__asm				mov		ebx, __ALPHABLEND_##COLOR \
	__asm				shl		ebx, 2 \
	__asm				add		ebx, eax \
	__asm				mov		ebx, [ebx] \
	__asm				mov		__ALPHABLEND_##COLOR, ebx

	// �A���t�@�����`��
	#define		__DRAW_ALPHABLEND(COLOR) \
						__GET_DST_##COLOR##_EAX \
	__asm				mul		ebx \
	__asm				add		eax, __ALPHABLEND_##COLOR \
	__asm				shr		eax, 8 \
	__asm				div		ecx \
						__SET_DST_##COLOR##_EAX \

	// �A���t�@�����Ȃ��`��
	#define		__DRAW_ALPHABLENDLESS \
	__asm				mov		ebx, __ALPHABLEND_R \
						__SET_DST_R_EBX \
	__asm				mov		ebx, __ALPHABLEND_G \
						__SET_DST_G_EBX \
	__asm				mov		ebx, __ALPHABLEND_B \
						__SET_DST_B_EBX \
						__SET_DST_A_EAX

	// �A���t�@�O����
	#define		__BEFOR_ALPHABLEND(fm) \
	__asm				push	ecx \
	__asm				mov		ecx, __ALPHABLEND_A \
	__asm				shl		ecx, 2 \
	__asm				mov		edx, ecx \
	__asm				add		ecx, __TABLE_ALPHABLEND_TR \
	__asm				mov		ecx, [ecx] \
						__GET_DST_A_EAX \
	__asm				shl		eax, 2 \
	__asm				add		ecx, eax \
	__asm				mov		ecx, [ecx] \
	__asm				and		ecx, ecx \
	__asm				jnz		__##fm##_befor_a_bld_tr_end \
	__asm				jmp		__##fm##_alphablend_skip \
	__asm			__##fm##_befor_a_bld_tr_end: \
	__asm				add		edx, __TABLE_ALPHABLEND_WORK1 \
	__asm				mov		edx, [edx] \
	__asm				add		edx, eax \
	__asm				mov		edx, [edx]

	// �A���t�@�㏈��
	#define		__AFTER_ALPHABLEND(fm) \
	__asm				mov		eax, __ALPHABLEND_A \
	__asm				mov		ebx, eax \
	__asm				xor		ebx, 0xff \
	__asm				jnz		__##fm##_after_alphablend_next \
						__DRAW_ALPHABLENDLESS \
	__asm				jmp		__##fm##_alphablend_skip \
	__asm			__##fm##_after_alphablend_next: \
						__GET_DST_A_EBX \
	__asm				and		ebx, ebx \
	__asm				jnz		__##fm##_after_alphablend_draw \
						__DRAW_ALPHABLENDLESS \
	__asm				jmp		__##fm##_alphablend_skip \
	__asm			__##fm##_after_alphablend_draw: \
	__asm				shl		eax, 2 \
	__asm				add		eax, __TABLE_ALPHABLEND_WORK2 \
	__asm				mov		eax, [eax] \
						__READY_ALPHABLEND(R) \
						__READY_ALPHABLEND(G) \
						__READY_ALPHABLEND(B) \
	__asm				mov		ebx, edx \
						__DRAW_ALPHABLEND(R) \
						__DRAW_ALPHABLEND(G) \
						__DRAW_ALPHABLEND(B) \
	__asm				mov		[edi+3], cl \
	__asm			__##fm##_alphablend_skip: \
	__asm				pop		ecx

	// ****************************************************************
	// �t�H���g�A���t�@�����R�s�[
	//		IN		BYTE *d_buf  : �]����|�C���^
	//				int d_xl     : �]����w�k
	//				int d_yl     : �]����x�k
	//				BYTE *s_buf  : �]�����|�C���^
	//				int s_xl     : �]�����w�k
	//				int s_yl     : �]�����x�k
	//				int x        : �]�����W
	//				int y        : �]�����W
	// ================================================================
	void Cfont_copy::alphablend_copy(BYTE *d_buf, int d_xl, int d_yl, BYTE *s_buf, int s_xl, int s_yl, int x, int y)
	{
		// �A���t�@�����e�[�u���쐬
		if (!Gfont_alphablend_table_make_flag)	{	make_alphablend_table();	}

		int cnt_x, cnt_y, d_free_x, d_free_y, s_free_x, s_free_y;

		if (x >= d_xl || y >= d_yl)	{	return;	}

		if (x < 0)	{	s_free_x = -x;	cnt_x = s_xl + x;	d_free_x = 0;	}
		else		{	s_free_x = 0;	cnt_x = s_xl;		d_free_x = x;	}
		if ((d_xl - d_free_x) < cnt_x)	{	cnt_x = d_xl - d_free_x;	}
		if (cnt_x <= 0)	{	return;	}

		if (y < 0)	{	s_free_y = -y;	cnt_y = s_yl + y;	d_free_y = 0;	}
		else		{	s_free_y = 0;	cnt_y = s_yl;		d_free_y = y;	}
		if ((d_yl - d_free_y) < cnt_y)	{	cnt_y = d_yl - d_free_y;	}
		if (cnt_y <= 0)	{	return;	}

		s_buf += (s_free_x + (s_free_y * s_xl)) * 4;
		d_buf += (d_free_x + (d_free_y * d_xl)) * 4;
		s_free_x = (s_xl - cnt_x) * 4;
		d_free_x = (d_xl - cnt_x) * 4;

		_asm{
			mov		edi, d_buf
			mov		esi, s_buf
			mov		ecx, cnt_y
		__loop_y:
			push	ecx
			mov		ecx, cnt_x
		__loop_x:
			xor		eax, eax
			mov		al, [esi+3]
			and		al, al
			jnz		__step_draw
			jmp		__step_no_draw			// �]�����̓����x���O�Ȃ̂ŃW�����v

		__step_draw:

			cmp		al, 255
			jne		__step_draw_else

			// �]�����̓����x�� 255 �̎��͂��̂܂܃R�s�[
			movsd
			jmp		__step_next

		__step_draw_else:

			// �]����̓����x�擾
			mov		bl, [edi+3]
			and		bl, bl
			jnz		__step_draw_main

			// �]����̓����x�� 0 �̎��͂��̂܂܃R�s�[
			movsd
			jmp		__step_next

		__step_draw_main:

			xor		ebx, ebx
			mov		bl, [esi+3]
			mov		__ALPHABLEND_A, ebx
			__BEFOR_ALPHABLEND(fm2)
			__GET_SRC_R_EBX __SET_R_BLD_TR
			__GET_SRC_G_EBX __SET_G_BLD_TR
			__GET_SRC_B_EBX __SET_B_BLD_TR
			__AFTER_ALPHABLEND(fm2)

		__step_no_draw:
			add		edi, 4
			add		esi, 4
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
	}

	// ****************************************************************
	// �t�H���g�A���t�@�����R�s�[�i�܂Ƃ��j
	//		IN		BYTE *d_buf  : �]����|�C���^
	//				int d_xl     : �]����w�k
	//				int d_yl     : �]����x�k
	//				BYTE *s_buf  : �]�����|�C���^
	//				int s_xl     : �]�����w�k
	//				int s_yl     : �]�����x�k
	//				int x        : �]�����W
	//				int y        : �]�����W
	// ================================================================
	void Cfont_copy::alphablend_copy_face(BYTE *d_buf, int d_xl, int d_yl, BYTE *s_buf, int s_xl, int s_yl, int x, int y, MOJI_FACE_TYPE face_type)
	{
		switch (face_type)	{
			case MOJI_FACE_TYPE_NONE:
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y);
				break;
			case MOJI_FACE_TYPE_4:
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y + 1);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 1, y);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 1, y + 1);
				break;
			case MOJI_FACE_TYPE_9:
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y + 1);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y + 2);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 1, y);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 1, y + 1);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 1, y + 2);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 2, y);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 2, y + 1);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 2, y + 2);
				break;
			case MOJI_FACE_TYPE_16:
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y + 1);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y + 2);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x, y + 3);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 1, y);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 1, y + 1);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 1, y + 2);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 1, y + 3);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 2, y);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 2, y + 1);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 2, y + 2);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 2, y + 3);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 3, y);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 3, y + 1);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 3, y + 2);
				alphablend_copy(d_buf, d_xl, d_yl, s_buf, s_xl, s_yl, x + 3, y + 3);
				break;
		}
	}
};
