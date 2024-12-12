#include	"tonasound3_pch.h"
#include	"tonasound3_omv.h"
#include	"tonasound3_omv_pimpl.h"

namespace NT3
{

#define		OMV_DEBUG_INFO_BOX_USE		0		// ���f�o�b�O�p���b�Z�[�W�{�b�N�X�̕\���̗L���i�����[�X���� 0 �ɂ��鎖�j

int C_omv_player_impl::yuv2rgb_table[256][256][256];

// ****************************************************************
// omv �v���C���[�F�R���X�g���N�^
// ================================================================
C_omv_player_impl::C_omv_player_impl()
{
	// �t�H�[�}�b�g
	format_player();
}

// ****************************************************************
// omv �v���C���[�F�f�X�g���N�^
// ================================================================
C_omv_player_impl::~C_omv_player_impl()
{
	// ���
	release_player();
}

// ****************************************************************
// omv �v���C���[���t�H�[�}�b�g����
// ================================================================
void C_omv_player_impl::format_player()
{
	ZeroMemory(&m_omv_header, sizeof(m_omv_header));

	m_state_flag = 0;

	ZeroMemory(&m_theora_info, sizeof(m_theora_info));
	ZeroMemory(&m_theora_play_info, sizeof(m_theora_play_info));

	m_loop_flag = false;
	m_end_of_theora = false;
	m_seek_top = 0;
	m_theora_total_page_cnt = 0;
	m_theora_total_packet_cnt = 0;
	m_theora_now_page_no = -1;
	m_theora_now_frame_no = -1;

	m_theora_onmemory_buffer_p = NULL;

	ZeroMemory(&m_theora_stream, sizeof(m_theora_stream));

	ZeroMemory(&m_theora_header, sizeof(m_theora_header));
	th_info_init(&m_theora_header);

	ZeroMemory(&m_theora_comment, sizeof(m_theora_comment));
	th_comment_init(&m_theora_comment);

	m_theora_decoder = NULL;
}

// ****************************************************************
// omv �v���C���[������������
// ================================================================
bool C_omv_player_impl::init_player()
{
	// ���
	release_player();

	// �t�H�[�}�b�g
	format_player();

	return true;
}

// ****************************************************************
// omv �v���C���[���������
// ================================================================
bool C_omv_player_impl::release_player()
{
	ogg_stream_clear(&m_theora_stream);
	th_info_clear(&m_theora_header);
	th_comment_clear(&m_theora_comment);
	if (m_theora_decoder != NULL)	{
		th_decode_free(m_theora_decoder);
	}

	m_theora_page_list.clear();
	m_theora_packet_list.clear();
	m_theora_onmemory_buffer.clear();
	m_theora_onmemory_buffer_p = NULL;

	// �t�@�C�������
	m_file.close();

	return true;
}

// ****************************************************************
// �G���[��ݒ�
// ================================================================
void C_omv_player_impl::set_error(CTSTR& str)
{
	m_last_error = str;
}

// ****************************************************************
// �G���[��ݒ�i�O�ɒǉ��j
// ================================================================
void C_omv_player_impl::set_error_befor(CTSTR& str, bool return_separate)
{
	if (return_separate)	{
		m_last_error = str + _T("\n") + m_last_error;
	}
	else	{
		m_last_error = str + m_last_error;
	}
}

// ****************************************************************
// �G���[��ݒ�i��ɒǉ��j
// ================================================================
void C_omv_player_impl::set_error_after(CTSTR& str, bool return_separate)
{
	if (return_separate)	{
		m_last_error = m_last_error + _T("\n") + str;
	}
	else	{
		m_last_error = m_last_error + str;
	}
}

// ****************************************************************
// �G���[���擾
// ================================================================
TSTR C_omv_player_impl::get_last_error()
{
	return m_last_error;
}

// ****************************************************************
// �Đ����H
// ================================================================
bool C_omv_player_impl::is_playing()
{
	return (m_state_flag == 0 || m_end_of_theora) ? false : true;
}

// ****************************************************************
// �����ł��Ă�H
// ================================================================
bool C_omv_player_impl::is_ready()
{
	return (m_state_flag == 0) ? false : true;
}

// ****************************************************************
// �V�I���̏����擾
// ================================================================
void C_omv_player_impl::get_theora_info(S_omv_theora_info* info)
{
	// �R�s�[
	*info = m_theora_info;
}

// ****************************************************************
// �V�I���̍Đ������擾
// ================================================================
void C_omv_player_impl::get_theora_play_info(S_omv_theora_play_info* info)
{
	// �R�s�[
	*info = m_theora_play_info;

	// ���݂̃t���[���ԍ�
	info->now_frame_no = (m_theora_now_frame_no < 0) ? 0 : m_theora_now_frame_no;

	// ���݂̃t���[���J�n����
	// ���݂̃t���[���I������
	// ���݂̃L�[�t���[���ԍ�
	if (0 <= info->now_frame_no && info->now_frame_no < m_theora_total_packet_cnt)	{
		S_omv_theora_packet *tpacket = &m_theora_packet_list[info->now_frame_no];
		info->now_frame_time_start = tpacket->frame_time_start;
		info->now_frame_time_end = tpacket->frame_time_end;
		info->now_key_frame_no = tpacket->key_frame_packet_no;
	}
	else	{
		info->now_frame_time_start = 0;
		info->now_frame_time_end = 0;
		info->now_key_frame_no = 0;
	}
}

// ****************************************************************
// �V�I���̍Đ��������Z�b�g����
// ================================================================
void C_omv_player_impl::reset_theora_play_info()
{
	m_theora_play_info.packet_ok_cnt = 0;				// �\���ɐ��������p�P�b�g��
	m_theora_play_info.packet_dropped_cnt = 0;			// �h���b�v�����p�P�b�g��
	m_theora_play_info.total_decode_frame_time = 0;		// �f�R�[�h�������t���[������
	m_theora_play_info.buffering_cnt = 0;				// �o�b�t�@�����O������
}

// ****************************************************************
// �V�I���̑��t���[�������擾
// ================================================================
int C_omv_player_impl::get_theora_total_frame_cnt()
{
	return m_theora_info.frame_cnt;
}

// ****************************************************************
// �V�I���̑��t���[�����Ԃ��擾
// ================================================================
int C_omv_player_impl::get_theora_total_frame_time()
{
	return m_theora_info.frame_time;
}

// ****************************************************************
// omv �̍Đ��̏���������
// ================================================================

bool C_omv_player_impl::ready_omv(CTSTR& file_path, bool is_loop, bool is_onmemory)
{
	if (!ready_omv_func(file_path, is_loop, is_onmemory))	{
#if OMV_DEBUG_INFO_BOX_USE
		info_box(m_last_error);		// �f�o�b�O�p���b�Z�[�W�{�b�N�X
#endif
		return false;
	}
	return true;
}

bool C_omv_player_impl::ready_omv_func(CTSTR& file_path, bool is_loop, bool is_onmemory)
{
	ogg_packet pkt;

	// �t�@�C�����J��
	if (!m_file.open(file_path, _T("rb")))	{
		set_error(_T("omv �t�@�C�����J���܂���ł����B\n") + get_last_error_msg());
		return false;
	}

	// ���[�v�t���O��ݒ�
	m_loop_flag = is_loop;

	// omv �w�b�_��ǂݍ���
	m_file.read(&m_omv_header, sizeof(m_omv_header));
	m_seek_top = sizeof(m_omv_header);

	// �o�[�W��������
	if (m_omv_header.major_version != omv_major_version || m_omv_header.minor_version != omv_minor_version)	{
		set_error(str_format(_T("omv �o�[�W�������Ⴂ�܂��B\nFileVersion : %d.%d\n\nSystemVersion : %d.%d\n"), m_omv_header.major_version, m_omv_header.minor_version, omv_major_version, omv_minor_version) + get_last_error_msg());
		return false;
	}

	// �V�I���̃y�[�W���X�g��ǂݍ���
	if (m_omv_header.theora_page_cnt > 0)	{
		m_theora_page_list.resize(m_omv_header.theora_page_cnt);
		int read_size = sizeof(S_omv_theora_page) * m_omv_header.theora_page_cnt;
		m_file.read(m_theora_page_list.get(), read_size);
		m_seek_top += read_size;
		m_theora_total_page_cnt = m_omv_header.theora_page_cnt;
	}

	// �V�I���̃p�P�b�g���X�g��ǂݍ���
	if (m_omv_header.theora_packet_cnt > 0)	{
		m_theora_packet_list.resize(m_omv_header.theora_packet_cnt);
		int read_size = sizeof(S_omv_theora_packet) * m_omv_header.theora_packet_cnt;
		m_file.read(m_theora_packet_list.get(), read_size);
		m_seek_top += read_size;
		m_theora_total_packet_cnt = m_omv_header.theora_packet_cnt;
	}

	// �V�I���̃X�g���[��������������
	ogg_stream_init(&m_theora_stream, m_omv_header.theora_serial_no);

	// �V�I���̃Z�b�g�A�b�v�h�m�e�n������������
	th_setup_info *theora_setupinfo = NULL;

	// �V�I���̃w�b�_�[�̉��
	if (read_page_for_theora(m_omv_header.theora_header_page_no) < 0)	{
		set_error_befor(_T("omv �V�I���̃w�b�_�[���ǂݍ��߂܂���ł����B"), true);
		return false;
	}
	if (ogg_stream_packetout(&m_theora_stream, &pkt) <= 0)	{
		set_error(_T("omv �V�I���̃w�b�_�[����͂ł��܂���ł����B�i�p�P�b�g�G���[�j\n") + get_last_error_msg());
		return false;
	}
	if (th_decode_headerin(&m_theora_header, &m_theora_comment, &theora_setupinfo, &pkt) < 0)	{
		set_error(_T("omv �V�I���̃w�b�_�[����͂ł��܂���ł����B") + get_last_error_msg());
		return false;
	}
	empty_stream_for_theora();		// �p�P�b�g����ɂ���i�����Ԃ�K�v�Ȃ��j

	// �V�I���̃T�u�w�b�_�[�̉��
	if (read_page_for_theora(m_omv_header.theora_subheader_page_no) < 0)	{
		set_error_befor(_T("omv �V�I���̃T�u�w�b�_�[���ǂݍ��߂܂���ł����B"), true);
		return false;
	}
	if (ogg_stream_packetout(&m_theora_stream, &pkt) <= 0)	{
		set_error(_T("omv �V�I���̃T�u�w�b�_�[�P����͂ł��܂���ł����B�i�p�P�b�g�G���[�j\n") + get_last_error_msg());
		return false;
	}
	if (th_decode_headerin(&m_theora_header, &m_theora_comment, &theora_setupinfo, &pkt) < 0)	{
		set_error(_T("omv �V�I���̃T�u�w�b�_�[�P����͂ł��܂���ł����B\n") + get_last_error_msg());
		return false;
	}
	if (ogg_stream_packetout(&m_theora_stream, &pkt) <= 0)	{
		set_error(_T("omv �V�I���̃T�u�w�b�_�[�Q����͂ł��܂���ł����B�i�p�P�b�g�G���[�j\n") + get_last_error_msg());
		return false;
	}
	if (th_decode_headerin(&m_theora_header, &m_theora_comment, &theora_setupinfo, &pkt) < 0)	{
		set_error(_T("omv �V�I���̃T�u�w�b�_�[�Q����͂ł��܂���ł����B\n") + get_last_error_msg());
		return false;
	}
	empty_stream_for_theora();		// �p�P�b�g����ɂ���i�����Ԃ�K�v�Ȃ��j

	// �V�I���̃f�R�[�_�[���m�ۂ���
	m_theora_decoder = th_decode_alloc(&m_theora_header, theora_setupinfo);
	if (m_theora_decoder == NULL)	{
		set_error(_T("omv �V�I���̃f�R�[�_�[�̊m�ۂɎ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	// �V�I���̃f�R�[�_�[�� pre-processing level ��ݒ肵�܂��B
	int pp_level = 0;		// �Đ��x�����Ȃ������߂� 0 ��ݒ肵�܂��B
	th_decode_ctl(m_theora_decoder, TH_DECCTL_SET_PPLEVEL, &pp_level, sizeof(pp_level));

	// �V�I���̃Z�b�g�A�b�v�h�m�e�n���J������
	th_setup_free(theora_setupinfo);
	theora_setupinfo = NULL;

	// �V�I���̏����擾
	{
		m_theora_info.type = m_omv_header.theora_type;				// �V�I���̌`���i0=�q�f�a 1=�q�f�a�`�j
		m_theora_info.size = m_omv_header.theora_size;				// �V�I���̃T�C�Y
		m_theora_info.center = m_omv_header.theora_center;			// �V�I���̒��S���W
		m_theora_info.frame_cnt = m_omv_header.theora_packet_cnt;	// �V�I���̑��t���[����

		// �V�I���̑��t���[������
		if (m_theora_total_packet_cnt > 0)	{
			// �V�I���̍Ō�̃t���[�����Ԃ��擾
			S_omv_theora_packet *tpacket = &m_theora_packet_list[m_theora_total_packet_cnt - 1];
			m_theora_info.frame_time = tpacket->frame_time_end;
		}
	}

	// �I��������
	if (is_onmemory)	{
		int cnt = (int)m_theora_page_list.size();
		if (cnt > 0)	{
			S_omv_theora_page *tpage = &m_theora_page_list[0];
			int read_offset = tpage->seek_offset;
			int read_size = 0;
			for (int i = 0; i < cnt; i++)	{
				read_size += tpage->page_size;
				tpage++;
			}
			if (read_size > 0)	{
				m_theora_onmemory_buffer.resize(read_size);
				m_theora_onmemory_buffer_p = m_theora_onmemory_buffer.get();
				if (m_theora_onmemory_buffer_p)	{
					m_file.seek(read_offset + m_seek_top, SEEK_SET);
					m_file.read(m_theora_onmemory_buffer_p, read_size);
				}
			}
		}
	}

	// ��������
	m_state_flag = 1;

	return true;
}

// ****************************************************************
// �V�I���̃y�[�W��ǂݍ���
// ================================================================

int C_omv_player_impl::read_page_for_theora(int page_no)
{
	int res = read_page_for_theora_func(page_no);
	if (res < 0)	{
		set_error_befor(_T("omv �y�[�W�̓ǂݍ��݃G���["), true);
		set_error_after(_T("�y�[�W�ԍ� ") + tostr(page_no), true);
		set_error_after(get_last_error_msg(), true);
	}
	return res;
}

int C_omv_player_impl::read_page_for_theora_func(int page_no)
{
	m_theora_play_info.buffering_cnt++;

	// �d�q�q�F�ُ�ȃy�[�W���w�肵�܂���
	if (page_no < 0)	{
		set_error(_T("omv �ُ�ȃy�[�W���w�肵�܂����B�i�A���_�[�j"));
		return -1;
	}

	// �d�q�q�F�ُ�ȃy�[�W���w�肵�܂���
	if (page_no >= m_theora_total_page_cnt)	{
		set_error(_T("omv �ُ�ȃy�[�W���w�肵�܂����B�i�I�[�o�[�j"));
		return -2;
	}

	S_omv_theora_page *tpage = &m_theora_page_list[page_no];

	// �d�q�q�F�T�C�Y���O�̃y�[�W�ł�
	if (tpage->page_size <= 0)	{
		set_error(_T("omv �T�C�Y���O�̃y�[�W�ł��B"));
		return -3;
	}

	// �o�b�t�@�m��
	ogg_sync_state oggsyncstate;
	ogg_sync_init(&oggsyncstate);
	char *syncbuf = ogg_sync_buffer(&oggsyncstate, tpage->page_size);

	// �d�q�q�F�o�b�t�@���m�ۂł��܂���ł���
	if (syncbuf == NULL)	{
		set_error(_T("omv �o�b�t�@���m�ۂł��܂���ł����B"));
		return -4;
	}

	// �ǂݍ���
	int read_size;
	if (m_theora_onmemory_buffer_p)	{
		int buffer_size = (int)m_theora_onmemory_buffer.size();
		if (tpage->seek_offset < buffer_size)	{
			read_size = buffer_size - tpage->seek_offset;
			if (tpage->page_size < read_size)	{
				read_size = tpage->page_size;
			}
		}
		else	{
			read_size = 0;
		}
		memcpy(syncbuf, m_theora_onmemory_buffer_p + tpage->seek_offset, read_size);
	}
	else	{
		m_file.seek(tpage->seek_offset + m_seek_top, SEEK_SET);
		read_size = (int)m_file.read(syncbuf, tpage->page_size);
	}

	// �d�q�q�F�v�����̃T�C�Y���ǂݍ��߂܂���ł���
	if (tpage->page_size != read_size)	{
		ogg_sync_clear(&oggsyncstate);					// �o�b�t�@���N���A����
		set_error(_T("omv �v�����̃T�C�Y���ǂݍ��߂܂���ł����B"));
		return -5;
	}

	// �o�b�t�@���n�f�f�ɏ���������
	if (ogg_sync_wrote(&oggsyncstate, read_size) < 0)	{
		ogg_sync_clear(&oggsyncstate);					// �o�b�t�@���N���A����
		set_error(_T("omv �o�b�t�@�����Ɏ��s���܂����B"));
		return -6;
	}

	// �y�[�W���擾����
	ogg_page page_data;
	if (ogg_sync_pageout(&oggsyncstate, &page_data) < 0)	{
		ogg_sync_clear(&oggsyncstate);					// �o�b�t�@���N���A����
		set_error(_T("omv �y�[�W�̎擾�Ɏ��s���܂����B"));
		return -7;
	}

	// �X�g���[���ɗ�������
	if (ogg_stream_pagein(&m_theora_stream, &page_data) < 0)	{
		ogg_sync_clear(&oggsyncstate);					// �o�b�t�@���N���A����
		set_error(_T("omv �X�g���[���̐ݒ�Ɏ��s���܂����B"));
		return -8;
	}

	// �o�b�t�@���N���A����
	ogg_sync_clear(&oggsyncstate);

	return (tpage->is_eos) ? 1 : 0;		// 1 = �d�n�e�ł�
}

// ****************************************************************
// �V�I���̃X�g���[������ɂ���
// ================================================================
void C_omv_player_impl::empty_stream_for_theora(void)
{
	// �� ogg_stream_clear / ogg_stream_reset ���g�p���Ă͂Ȃ�Ȃ�
	// �O�̃y�[�W����̑�������������ׂɂ́A�O�̃y�[�W�̃p�P�b�g���P�����o���K�v������܂��B
	// �������Ȃ���΁A���̃y�[�W�̃p�P�b�g������Ɏ擾�ł��Ȃ���������܂��B
	ogg_packet pkt;
	while (1)	{
		if (ogg_stream_packetout(&m_theora_stream, &pkt) <= 0)	{	break;	}
	}
}

// ****************************************************************
// �v���C���[�̃G���[���`�F�b�N����
// ================================================================
bool C_omv_player_impl::check_player_error()
{
	if (m_state_flag != 1)	{
		set_error(_T("omv �V�I���̍Đ��̏������ł��Ă��܂���B\n") + get_last_error_msg());
		return false;
	}

	if (m_theora_decoder == NULL)	{
		set_error(_T("omv �V�I���̃f�R�[�_�[�̏������ł��Ă��܂���B\n") + get_last_error_msg());
		return false;
	}

	if (m_theora_total_page_cnt <= 0)	{
		set_error(_T("omv �V�I���̃y�[�W������܂���B\n") + get_last_error_msg());
		return false;
	}

	if (m_theora_total_packet_cnt <= 0)	{
		set_error(_T("omv �V�I���̃p�P�b�g������܂���B\n") + get_last_error_msg());
		return false;
	}

	return true;
}

// ****************************************************************
// ���Ԃ��X�V����
// ================================================================
//		m_end_of_theora ���ς�邾���ł�
// ================================================================
void C_omv_player_impl::update_time_only(int now_time)
{
	if (now_time < m_theora_info.frame_time)	{
		m_end_of_theora = false;
	}
	else	{
		m_end_of_theora = true;
	}
}

// ****************************************************************
// ���Ԃ��t���[���ɂ���
// ================================================================
int C_omv_player_impl::time_to_frame(int now_time, bool set_m_end_of_theora)
{
	int frame_no = -1;

	// ���Ԃ��͈͊O�̏ꍇ�̃t���[��
	if (now_time <= 0)	{
		now_time = 0;
		frame_no = 0;
		m_end_of_theora = false;	// 2012/06/01 �ǉ�
	}
	else if (now_time >= m_theora_info.frame_time)	{
		if (m_loop_flag)	{
			now_time %= m_theora_info.frame_time;
		}
		else	{
			// �t���[���I��
			frame_no = m_theora_total_packet_cnt - 1;
			if (set_m_end_of_theora)	{
				m_end_of_theora = true;
			}
		}
	}
	else	{
		m_end_of_theora = false;	// 2012/06/01 �ǉ�
	}

	if (frame_no != -1)	{
		return frame_no;
	}

	// ���݂̃t���[���ʒu����Z�o����
	S_omv_theora_packet *tpacket;
	int i;

	// ���Ԃ���t���[���ԍ����Z�o
	if (0 <= m_theora_now_frame_no && m_theora_now_frame_no < m_theora_total_packet_cnt)	{
		tpacket = &m_theora_packet_list[m_theora_now_frame_no];
		for (i = m_theora_now_frame_no; i < m_theora_total_packet_cnt; i++)	{
			if (now_time <= tpacket->frame_time_end)	{
				if (now_time >= tpacket->frame_time_start)	{
					frame_no = i;
				}
				break;
			}
			tpacket++;
		}
	}

	if (frame_no != -1)	{
		return frame_no;
	}

	// �S�Ẵt���[���ʒu����Z�o����
	tpacket = &m_theora_packet_list[0];
	for (int i = 0; i < m_theora_total_packet_cnt; i++)	{
		if (now_time <= tpacket->frame_time_end)	{
			frame_no = i;
			break;
		}
		tpacket++;
	}
	return frame_no;
}

// ****************************************************************
// �r�f�I���擾����i���Ԏw��j
// ================================================================

bool C_omv_player_impl::check_need_update(int now_time, int* ret_frame_no, bool by_force)
{
	if (!check_player_error())	{
		return false;	// �X�V���Ȃ�
	}
	int frame_no = time_to_frame(now_time, true);

	if (!by_force && frame_no == m_theora_now_frame_no)	{
		return false;	// �X�V���Ȃ�
	}

	if (ret_frame_no)	*ret_frame_no = frame_no;
	return true;	// �X�V����
}

bool C_omv_player_impl::get_video_for_time(int now_time, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag)
{
	if (!get_video_for_time_func(now_time, p_dst_org, dst_pitch, alpha_only, by_force, ret_update_flag))	{
#if OMV_DEBUG_INFO_BOX_USE
		info_box(m_last_error);		// �f�o�b�O�p���b�Z�[�W�{�b�N�X
#endif
		return false;
	}
	return true;
}

bool C_omv_player_impl::get_video_for_time_func(int now_time, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag)
{
	if (ret_update_flag)	{	*ret_update_flag = false;	}		// �X�V�t���O���N���A�ɂ���i�n�e�e�ɂ���j
	if (!check_player_error())	{
		return false;
	}
	int frame_no = time_to_frame(now_time, true);
	return get_video_for_frame_funcfunc(frame_no, p_dst_org, dst_pitch, alpha_only, by_force, ret_update_flag);
}

// ****************************************************************
// �r�f�I���擾����i�t���[���w��j
// ================================================================

bool C_omv_player_impl::get_video_for_frame(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag)
{
	if (!get_video_for_frame_func(frame_no, p_dst_org, dst_pitch, alpha_only, by_force, ret_update_flag))	{
#if OMV_DEBUG_INFO_BOX_USE
		info_box(m_last_error);		// �f�o�b�O�p���b�Z�[�W�{�b�N�X
#endif
		return false;
	}
	return true;
}

bool C_omv_player_impl::get_video_for_frame_func(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag)
{
	if (ret_update_flag)	{	*ret_update_flag = false;	}		// �X�V�t���O���N���A�ɂ���i�n�e�e�ɂ���j
	if (!check_player_error())	{
		return false;
	}

	// �t���[���␳
	if (frame_no < 0)	{
		frame_no = 0;
	}
	else if (frame_no >= m_theora_total_packet_cnt)	{
		frame_no = m_theora_total_packet_cnt - 1;
	}

	// �t���[���I��
	if (frame_no == (m_theora_total_packet_cnt - 1))	{
		if (!m_loop_flag)	{
			m_end_of_theora = true;
		}
	}

	return get_video_for_frame_funcfunc(frame_no, p_dst_org, dst_pitch, alpha_only, by_force, ret_update_flag);
}

bool C_omv_player_impl::get_video_for_frame_funcfunc(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag)
{
	if (!get_video_for_frame_funcfuncfunc(frame_no, p_dst_org, dst_pitch, alpha_only, by_force, ret_update_flag))	{
		if (ret_update_flag)	{	*ret_update_flag = false;	}		// �X�V�t���O���n�e�e�ɂ���
		set_error_befor(_T("�t���[���ԍ� ") + tostr(frame_no), true);
		set_error_befor(_T("omv �t���[�����擾�ł��܂���ł����B"), true);
		return false;
	}
	return true;
}

bool C_omv_player_impl::get_video_for_frame_funcfuncfunc(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag)
{
	// ���݂̃t���[��
	if (!by_force && frame_no == m_theora_now_frame_no)	{
		return true;
	}

	if (ret_update_flag)	{	*ret_update_flag = true;	}		// �X�V�t���O���n�m�ɂ���

	S_omv_theora_page *tpage;
	S_omv_theora_packet *tpacket;

	// �����ǂݍ���
	if (m_theora_now_page_no == -1 || m_theora_now_frame_no == -1)	{
		by_force = true;
	}
	else if (frame_no < m_theora_now_frame_no)	{
		by_force = true;

		// �h���b�v�p�P�b�g
		m_theora_play_info.packet_dropped_cnt += ((m_theora_total_packet_cnt - m_theora_now_frame_no) - 1) + frame_no;
	}
	else	{

		// �h���b�v�p�P�b�g
		m_theora_play_info.packet_dropped_cnt += (frame_no - m_theora_now_frame_no) - 1;

		// �V�[�N����
		S_omv_theora_packet *tpacket_now = &m_theora_packet_list[m_theora_now_frame_no];
		tpacket = &m_theora_packet_list[frame_no];
		if (tpacket_now->key_frame_packet_no != tpacket->key_frame_packet_no)	{
			tpage = &m_theora_page_list[tpacket->key_frame_page_no];
			if (tpacket_now->own_page_no < tpage->seek_page_no)	{
				by_force = true;	// �V�[�N������������
			}
		}
	}

	// �n�j�J�E���g��i�߂�
	m_theora_play_info.packet_ok_cnt++;

	// ��͊J�n
	ogg_packet pkt;
	int res, read_page_no, decode_packet_no;
	bool eos_flag = false;
	tpacket = &m_theora_packet_list[frame_no];
	int key_frame_packet_no = tpacket->key_frame_packet_no;
	int skip_for_key_frame_no = -1;
	bool video_update_flag = true;

	// �����I�ɓǂݍ���
	if (by_force)	{

		// �X�g���[��������
//		ogg_stream_clear(&m_theora_stream);
//		ogg_stream_init(&m_theora_stream, m_omv_header.theora_serial_no);
		ogg_stream_reset(&m_theora_stream);

		// �L�[�t���[���̃y�[�W��ǂݍ���
		tpage = &m_theora_page_list[tpacket->key_frame_page_no];
		read_page_no = tpage->seek_page_no;
		int back_page_cnt = (tpage->own_page_no - tpage->seek_page_no) + 1;
		while (1)	{
			res = read_page_for_theora(read_page_no);
			if (res < 0)	{
				set_error_befor(_T("omv �L�[�t���[���̃y�[�W���ǂݍ��߂܂���ł����B"), true);
				return false;
			}
			if (read_page_no == tpage->own_page_no)	{	break;	}	// ��͂���y�[�W�ɂ��܂����B
			empty_stream_for_theora();								// �p�P�b�g����ɂ���
			read_page_no++;
		}
		skip_for_key_frame_no = key_frame_packet_no;		// ���̕ϐ��͂܂��g���܂�
		if (tpage->is_eos)	{	eos_flag = true;	}		// ���̕ϐ��͂܂��g���܂��i�d�n�r�j
		decode_packet_no = tpage->top_packet_no;			// ���̕ϐ��͂܂��g���܂�
		read_page_no++;										// ���̕ϐ��͂܂��g���܂��i + 1 = ���݂̃y�[�W�͊��ɓǂݍ��܂�Ă���̂ŁA���֐i�߂�j
		tpage++;											// ���̕ϐ��͂܂��g���܂��i + 1 = ���݂̃y�[�W�͊��ɓǂݍ��܂�Ă���̂ŁA���֐i�߂�j
	}
	// ���݂̃X�g���[�����𗘗p���܂�
	else	{
		tpacket = &m_theora_packet_list[m_theora_now_frame_no];
		tpage = &m_theora_page_list[tpacket->own_page_no];
		if (tpacket->key_frame_packet_no != key_frame_packet_no)	{
			// �L�[�t���[�����ς�����̂ŃL�[�t���[���܂ŃX�L�b�v����
			skip_for_key_frame_no = key_frame_packet_no;	// ���̕ϐ��͂܂��g���܂�
		}
		if (tpage->is_eos)	{	eos_flag = true;	}		// ���̕ϐ��͂܂��g���܂��i�d�n�r�j
		decode_packet_no = m_theora_now_frame_no + 1;		// ���̕ϐ��͂܂��g���܂��i + 1 = ���݂̃t���[���͊��ɏ�������Ă���̂ŁA���֐i�߂�j
		read_page_no = tpacket->own_page_no + 1;			// ���̕ϐ��͂܂��g���܂��i + 1 = ���݂̃y�[�W�͊��ɓǂݍ��܂�Ă���̂ŁA���֐i�߂�j
		tpage++;											// ���̕ϐ��͂܂��g���܂��i + 1 = ���݂̃y�[�W�͊��ɓǂݍ��܂�Ă���̂ŁA���֐i�߂�j
	}

	// �w��̃L�[�t���[���܂ŃX�L�b�v
	if (skip_for_key_frame_no != -1 && decode_packet_no != skip_for_key_frame_no)	{
		tpacket = &m_theora_packet_list[decode_packet_no];
		while (1)	{
			while (1)	{
				if (decode_packet_no == skip_for_key_frame_no)	{	break;	}
				if (ogg_stream_packetout(&m_theora_stream, &pkt) <= 0)	{	break;	}
				tpacket++;
				decode_packet_no++;
			}
			if (decode_packet_no == skip_for_key_frame_no)	{	break;	}

			res = read_page_for_theora(read_page_no);
			if (res < 0)	{
				set_error_befor(_T("omv �w��̃L�[�t���[���܂ŃX�L�b�v�ł��܂���ł����B"), true);
				return false;
			}
			if (tpage->is_eos)	{	eos_flag = true;	}		// �d�n�r
			read_page_no++;
			tpage++;
		}
	}

	// ��͂���
	bool loop_out = false;
	ogg_int64_t granulepos = 0;
	while (1)	{
		while (1)	{

			// �p�P�b�g�̎擾
			res = ogg_stream_packetout(&m_theora_stream, &pkt);
			if (res <= 0)	{
				loop_out = eos_flag;	// �d�n�r�̏ꍇ�A�����ŏI��
				break;
			}

			// �����낱��c
			//if (pkt.granulepos >= 0)	{
			//	th_decode_ctl(m_theora_decoder, TH_DECCTL_SET_GRANPOS, &pkt.granulepos, sizeof(pkt.granulepos));
			//}

			// �t���[���̓W�J
			res = th_decode_packetin(m_theora_decoder, &pkt, &granulepos);
			if (res < 0)	{
				if (res != TH_DUPFRAME)	{		// TH_DUPFRAME = �W�J����K�v�������t���[��
					set_error(_T("omv �t���[���̓W�J�Ɏ��s���܂����B\n") + get_last_error_msg());
					return false;
				}
				video_update_flag = false;
				loop_out = true;
				break;
			}

			// �ړI�̃t���[���ɒB���܂���
			if (decode_packet_no == frame_no)	{
				loop_out = true;
				break;
			}

			decode_packet_no++;
		}

		if (loop_out)
			break;

		res = read_page_for_theora(read_page_no);
		if (res < 0)	{
			set_error_befor(_T("omv ��͂ł��܂���ł����B"), true);
			return false;
		}
		if (tpage->is_eos)	{	eos_flag = true;	}		// �d�n�r
		read_page_no++;
		tpage++;
	}

	// �f�R�[�h�������t���[�����Ԃɉ��Z����
	tpacket = &m_theora_packet_list[frame_no];
	m_theora_play_info.total_decode_frame_time += tpacket->frame_time_end - tpacket->frame_time_start + 1;

	// �r�f�I���X�V����
	m_theora_now_page_no = read_page_no;
	m_theora_now_frame_no = frame_no;

	if (alpha_only)	{
		video_write_only_alpha(p_dst_org, dst_pitch);
	}
	else	{
		video_write(p_dst_org, dst_pitch);
	}

	return true;
}

#if 0
struct S_omv_thread_data
{
	BYTE*	p_src_y;
	BYTE*	p_src_u;
	BYTE*	p_src_v;
	BYTE*	p_src_a;
	int		src_w;
	int		src_h;
	int		src_y_stride;
	int		src_u_stride;
	int		src_v_stride;
	int		src_a_stride;
	BYTE*	p_dst;
	int		dst_pitch;
};
DWORD WINAPI omv_thread_func(LPVOID pv_ref)
{
	S_omv_thread_data* p_td = (S_omv_thread_data *)pv_ref;

	BYTE* p_src_y = p_td->p_src_y;
	BYTE* p_src_u = p_td->p_src_u;
	BYTE* p_src_v = p_td->p_src_v;
	BYTE* p_src_a = p_td->p_src_a;
	int src_w = p_td->src_w;
	int src_h = p_td->src_h;
	int diff_y = p_td->src_y_stride - p_td->src_w;
	int diff_u = p_td->src_u_stride - p_td->src_w;
	int diff_v = p_td->src_v_stride - p_td->src_w;
	int diff_a = p_td->src_a_stride - p_td->src_w;
	BYTE* p_dst = p_td->p_dst;
	BYTE* p_dst_goal;
	int diff_dst = p_td->dst_pitch - p_td->src_w * 4;
	int i;

	for (i = 0; i < src_h; i++)	{
		p_dst_goal = p_dst + src_w * 4;
		while (p_dst < p_dst_goal)	{ 
			*p_dst++ = *p_src_y++;
			*p_dst++ = *p_src_u++;
			*p_dst++ = *p_src_v++;
			*p_dst++ = *p_src_a++;
		}
		p_src_y += diff_y;
		p_src_u += diff_u;
		p_src_v += diff_v;
		p_src_a += diff_a;
		p_dst += diff_dst;
	}

	// �X���b�h�I��
	ExitThread(true);

	return 0;
}
#endif

// ****************************************************************
// ���[�v���I������
// ================================================================
void C_omv_player_impl::end_loop()
{
	m_loop_flag = false;
}

// ****************************************************************
// �ϊ��e�[�u������������
// ================================================================
void C_omv_player_impl::ready_table()
{
	// �����͂P�x�����ł���
	static bool create_table_flag = false;
	if (!create_table_flag)	{
		BYTE r, g, b;
		for (int y = 0; y < 256; y++)	{
			for (int u = 0; u < 256; u++)	{
				for (int v = 0; v < 256; v++)	{
					r = limit(0, (int)(y                       + 1.40200 * (v - 128)), 255);
					g = limit(0, (int)(y - 0.34414 * (u - 128) - 0.71414 * (v - 128)), 255);
					b = limit(0, (int)(y + 1.77200 * (u - 128)                      ), 255);
					yuv2rgb_table[y][u][v] = (255 << 24) | (r << 16) | (g << 8) | b;
				}
			}
		}
		create_table_flag = true;
	}
}

// ****************************************************************
// �r�f�I�f�[�^����������
// ================================================================
void C_omv_player_impl::video_write(BYTE* p_dst_org, int dst_pitch)
{
	th_ycbcr_buffer yuv;
	th_decode_ycbcr_out(m_theora_decoder, yuv);

	// �ϊ��e�[�u������������
	ready_table();

	// RGB �̏ꍇ
	if (m_theora_info.type == OMV_THEORA_TYPE_RGB)	{

		int pic_w = m_theora_info.size.cx;
		int pic_h = m_theora_info.size.cy;

		int diff_y = yuv[0].stride - pic_w;
		int diff_u = yuv[1].stride - pic_w;
		int diff_v = yuv[2].stride - pic_w;
		int diff_dst = dst_pitch - pic_w * sizeof(C_argb);
		int i;

		BYTE* p_dst;
		BYTE* p_dst_goal;
		BYTE* p_src_y;
		BYTE* p_src_u;
		BYTE* p_src_v;

		// yuv �̃\�[�X��ݒ�
		p_src_y = yuv[0].data;
		p_src_u = yuv[1].data;
		p_src_v = yuv[2].data;
		p_dst = p_dst_org;

		for (i = 0; i < pic_h; i++)	{
			p_dst_goal = p_dst + pic_w * sizeof(C_argb);
			while (p_dst < p_dst_goal)	{

				// RGB => RGBA
				*p_dst++ = *p_src_y++;
				*p_dst++ = *p_src_u++;
				*p_dst++ = *p_src_v++;
				*p_dst++ = 255;
			}
			p_dst += diff_dst;
			p_src_y += diff_y;
			p_src_u += diff_u;
			p_src_v += diff_v;
		}
	}
	// YUV �̏ꍇ
	if (m_theora_info.type == OMV_THEORA_TYPE_YUV)	{

		int cy, cu, cv;

		int pic_w = m_theora_info.size.cx;
		int pic_h = m_theora_info.size.cy;

		int diff_y = yuv[0].stride - pic_w;
		int diff_u = yuv[1].stride - pic_w;
		int diff_v = yuv[2].stride - pic_w;
		int diff_dst = dst_pitch - pic_w * sizeof(C_argb);
		int i;

		BYTE* p_dst;
		BYTE* p_dst_goal;
		BYTE* p_src_y;
		BYTE* p_src_u;
		BYTE* p_src_v;

		// yuv �̃\�[�X��ݒ�
		p_src_y = yuv[0].data;
		p_src_u = yuv[1].data;
		p_src_v = yuv[2].data;
		p_dst = p_dst_org;

		for (i = 0; i < pic_h; i++)	{
			p_dst_goal = p_dst + pic_w * sizeof(C_argb);
			while (p_dst < p_dst_goal)	{

				// YUV => RGBA
				cy = *p_src_y++;
				cu = *p_src_u++;
				cv = *p_src_v++;
				*(int *)p_dst = yuv2rgb_table[cy][cu][cv];
				p_dst += sizeof(int);
			}
			p_dst = p_dst + diff_dst;
			p_src_y += diff_y;
			p_src_u += diff_u;
			p_src_v += diff_v;
		}
	}
	// RGBA �̏ꍇ
	// RGB ���ꂼ��̃v���[���̉��ɁA1/3 ���� A �������Ă��܂�
	if (m_theora_info.type == OMV_THEORA_TYPE_RGBA)	{

		int pic_w = m_theora_info.size.cx;
		int pic_h = m_theora_info.size.cy;
		int alpha_h = (pic_h + 2) / 3;
		int alpha_h_2 = alpha_h * 2;

		int diff_y = yuv[0].stride - pic_w;
		int diff_u = yuv[1].stride - pic_w;
		int diff_v = yuv[2].stride - pic_w;
		int diff_dst = dst_pitch - pic_w * sizeof(C_argb);
		int i;

		BYTE* p_dst;
		BYTE* p_dst_goal;
		BYTE* p_src_y;
		BYTE* p_src_u;
		BYTE* p_src_v;
		BYTE* p_src_a;

		// yuv �̃\�[�X��ݒ�
		p_src_y = yuv[0].data;
		p_src_u = yuv[1].data;
		p_src_v = yuv[2].data;
		p_dst = p_dst_org;

		// ��R���̂P��`��
		p_src_a = yuv[0].data + yuv[0].stride * pic_h;
		for (i = 0; i < alpha_h; i++)	{
			p_dst_goal = p_dst + pic_w * sizeof(C_argb);
			while (p_dst < p_dst_goal)	{ 
				*p_dst++ = *p_src_y++;
				*p_dst++ = *p_src_u++;
				*p_dst++ = *p_src_v++;
				*p_dst++ = *p_src_a++;
			}
			p_dst += diff_dst;
			p_src_y += diff_y;
			p_src_u += diff_u;
			p_src_v += diff_v;
			p_src_a += diff_y;	// y �v���[���̉��Ƀ�
		}
		// ���R���̂P��`��
		p_src_a = yuv[1].data + yuv[1].stride * pic_h;
		for (i = alpha_h; i < alpha_h_2; i++)	{
			p_dst_goal = p_dst + pic_w * sizeof(C_argb);
			while (p_dst < p_dst_goal)	{ 
				*p_dst++ = *p_src_y++;
				*p_dst++ = *p_src_u++;
				*p_dst++ = *p_src_v++;
				*p_dst++ = *p_src_a++;
			}
			p_dst += diff_dst;
			p_src_y += diff_y;
			p_src_u += diff_u;
			p_src_v += diff_v;
			p_src_a += diff_u;	// u �v���[���̉��Ƀ�
		}
		// ���R���̂P��`��
		p_src_a = yuv[2].data + yuv[2].stride * pic_h;
		for (i = alpha_h_2; i < pic_h; i++)	{
			p_dst_goal = p_dst + pic_w * sizeof(C_argb);
			while (p_dst < p_dst_goal)	{ 
				*p_dst++ = *p_src_y++;
				*p_dst++ = *p_src_u++;
				*p_dst++ = *p_src_v++;
				*p_dst++ = *p_src_a++;
			}
			p_dst += diff_dst;
			p_src_y += diff_y;
			p_src_u += diff_u;
			p_src_v += diff_v;
			p_src_a += diff_v;	// v �v���[���̉��Ƀ�
		}
	}
}

// ****************************************************************
// �r�f�I�f�[�^���������ށi�`�̂݁j
// ================================================================
void C_omv_player_impl::video_write_only_alpha(BYTE* p_dst_org, int dst_pitch)
{
	th_ycbcr_buffer yuv;
	th_decode_ycbcr_out(m_theora_decoder, yuv);

	// �ϊ��e�[�u������������
	ready_table();

	// RGB / YUV �̏ꍇ
	if (m_theora_info.type == OMV_THEORA_TYPE_RGB || m_theora_info.type == OMV_THEORA_TYPE_YUV)	{

		int pic_w = m_theora_info.size.cx;
		int pic_h = m_theora_info.size.cy;

		int diff_y = yuv[0].stride - pic_w;
		int diff_u = yuv[1].stride - pic_w;
		int diff_v = yuv[2].stride - pic_w;
		int diff_dst = dst_pitch - pic_w * 4;
		int i;

		BYTE* p_dst;
		BYTE* p_dst_goal;

		// yuv �̃\�[�X��ݒ�
		p_dst = p_dst_org;

		for (i = 0; i < pic_h; i++)	{
			p_dst_goal = p_dst + pic_w * 4;
			while (p_dst < p_dst_goal)	{

				// RGB => RGBA�i���_���炷��Ɛ^�����ɂȂ�j
				*p_dst++ = 255;
				*p_dst++ = 255;
				*p_dst++ = 255;
				*p_dst++ = 255;
			}
			p_dst += diff_dst;
		}
	}
	// RGBA �̏ꍇ
	// RGB ���ꂼ��̃v���[���̉��ɁA1/3 ���� A �������Ă��܂�
	else if (m_theora_info.type == OMV_THEORA_TYPE_RGBA)	{

		int pic_w = m_theora_info.size.cx;
		int pic_h = m_theora_info.size.cy;
		int alpha_h = (pic_h + 2) / 3;
		int alpha_h_2 = alpha_h * 2;

		int diff_y = yuv[0].stride - pic_w;
		int diff_u = yuv[1].stride - pic_w;
		int diff_v = yuv[2].stride - pic_w;
		int diff_dst = dst_pitch - pic_w * 4;
		int i;

		BYTE* p_dst;
		BYTE* p_dst_goal;
		BYTE* p_src_a;

		// yuv �̃\�[�X��ݒ�
		p_dst = p_dst_org;

		// ��R���̂P��`��
		p_src_a = yuv[0].data + yuv[0].stride * pic_h;
		for (i = 0; i < alpha_h; i++)	{
			p_dst_goal = p_dst + pic_w * 4;
			while (p_dst < p_dst_goal)	{ 
				*p_dst++ = *p_src_a;
				*p_dst++ = *p_src_a;
				*p_dst++ = *p_src_a++;
				*p_dst++ = 255;
			}
			p_dst += diff_dst;
			p_src_a += diff_y;	// y �v���[���̉��Ƀ�
		}
		// ���R���̂P��`��
		p_src_a = yuv[1].data + yuv[1].stride * pic_h;
		for (i = alpha_h; i < alpha_h_2; i++)	{
			p_dst_goal = p_dst + pic_w * 4;
			while (p_dst < p_dst_goal)	{ 
				*p_dst++ = *p_src_a;
				*p_dst++ = *p_src_a;
				*p_dst++ = *p_src_a++;
				*p_dst++ = 255;
			}
			p_dst += diff_dst;
			p_src_a += diff_u;	// u �v���[���̉��Ƀ�
		}
		// ���R���̂P��`��
		p_src_a = yuv[2].data + yuv[2].stride * pic_h;
		for (i = alpha_h_2; i < pic_h; i++)	{
			p_dst_goal = p_dst + pic_w * 4;
			while (p_dst < p_dst_goal)	{ 
				*p_dst++ = *p_src_a;
				*p_dst++ = *p_src_a;
				*p_dst++ = *p_src_a++;
				*p_dst++ = 255;
			}
			p_dst += diff_dst;
			p_src_a += diff_v;	// v �v���[���̉��Ƀ�
		}
	}
}

}
