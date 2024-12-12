#pragma		once

namespace NT3
{

const int omv_major_version = 1;
const int omv_minor_version = 1;

// ****************************************************************
// omv �w�b�_�\����
// ================================================================

const int OMV_THEORA_TYPE_RGB = 0;
const int OMV_THEORA_TYPE_RGBA = 1;
const int OMV_THEORA_TYPE_YUV = 2;

struct S_omv_header
{
	int			header_size;				// �w�b�_�[�T�C�Y

	BYTE		major_version;				// �o�[�W����
	BYTE		minor_version;				// �o�[�W����
	BYTE		dummy[2];					// �_�~�[�i�S�o�C�g�����j

	BYTE		keep00[32];					// 32

	int			theora_type;				// �V�I���̌`���i0=�q�f�a 1=�q�f�a�`�j
	C_size		theora_size;				// �V�I���̃T�C�Y
	C_point		theora_center;				// �V�I���̒��S���W
	int			theora_us_per_frame;		// �V�I���̂P�t���[���̃}�C�N���b
	int			theora_serial_no;			// �V�I���̃V���A���ԍ�
	int			theora_header_page_no;		// �V�I���̃w�b�_�[�̃y�[�W�ԍ�
	int			theora_subheader_page_no;	// �V�I���̃T�u�w�b�_�[�̃y�[�W�ԍ�
	int			theora_page_cnt;			// �V�I���̃y�[�W��
	int			theora_packet_cnt;			// �V�I���̃p�P�b�g��

	BYTE		keep01[32];					// 32

	int			vorbis_serial_no;			// ���H�[�r�X�̃V���A���ԍ�
	int			vorbis_header_page_no;		// ���H�[�r�X�̃w�b�_�[�̃y�[�W�ԍ��i�����g�p�j
	int			vorbis_subheader_page_no;	// ���H�[�r�X�̃T�u�w�b�_�[�̃y�[�W�ԍ��i�����g�p�j
	int			vorbis_page_cnt;			// ���H�[�r�X�̃y�[�W���i�����g�p�j
	int			vorbis_packet_cnt;			// ���H�[�r�X�̃p�P�b�g���i�����g�p�j

	BYTE		keep02[32];					// 32
};

// ****************************************************************
// omv �y�[�W�\����
// ================================================================

const int OMV_PAGE_TYPE_HEADER = 0;
const int OMV_PAGE_TYPE_SUBHEADER = 1;
const int OMV_PAGE_TYPE_DATA = 2;
const int OMV_PAGE_TYPE_EOS = 3;

struct S_omv_theora_page
{
	int			own_page_no;				// ���g�̃y�[�W�ԍ�
	bool		is_eos;						// �d�n�r�ł�
	bool		is_key_page;				// �L�[�y�[�W�ł��ifalse = �f�[�^�͑O�̃y�[�W���瑱���Ă���j
	int			page_size;					// �y�[�W�T�C�Y
	int			seek_offset;				// �V�[�N�I�t�Z�b�g
	int			seek_page_no;				// �V�[�N�y�[�W�ԍ�
	int			packet_cnt;					// �p�P�b�g��
	int			top_packet_no;				// �擪�p�P�b�g�ԍ�
};

// ****************************************************************
// omv �y�[�W�\����
// ================================================================
struct S_omv_theora_packet
{
	int		own_packet_no;					// ���g�̃p�P�b�g�ԍ�
	int		own_page_no;					// ���g�̃y�[�W�ԍ�
	int		own_packet_no_in_page;			// ���g�̃y�[�W���̃p�P�b�g�ԍ�
	bool	is_key_frame;					// �L�[�t���[���ł�
	int		key_frame_packet_no;			// �L�[�t���[���̃p�P�b�g�ԍ�
	int		key_frame_page_no;				// �L�[�t���[���̃y�[�W�ԍ�
	int		frame_time_start;				// �t���[���̊J�n����
	int		frame_time_end;					// �t���[���̏I������
};

// ****************************************************************
// omv �V�I���̏��
// ================================================================
struct S_omv_theora_info
{
	int			type;						// �V�I���̌`���i0=�q�f�a 1=�q�f�a�`�j
	C_size		size;						// �V�I���̃T�C�Y
	C_point		center;						// �V�I���̒��S���W
	int			frame_cnt;					// �V�I���̑��t���[����
	int			frame_time;					// �V�I���̑��t���[������
};

// ****************************************************************
// omv �V�I���̃v���C���
// ================================================================
struct S_omv_theora_play_info
{
	int			now_frame_no;				// ���݂̃t���[���ԍ�
	int			now_frame_time_start;		// ���݂̃t���[���J�n����
	int			now_frame_time_end;			// ���݂̃t���[���I������
	int			now_key_frame_no;			// ���݂̃L�[�t���[���ԍ�

	int			packet_ok_cnt;				// �\���ɐ��������p�P�b�g��
	int			packet_dropped_cnt;			// �h���b�v�����p�P�b�g��
	int			total_decode_frame_time;	// �f�R�[�h�������t���[������
	int			buffering_cnt;				// �o�b�t�@�����O������
};

// ****************************************************************
// omv �v���C���[
// ================================================================
class	C_omv_player_impl;

class C_omv_player
{

public:

private:
	BSP<C_omv_player_impl>	pi;		

public:
	C_omv_player();
	~C_omv_player();

	static void		ready_table();		// static �Ȋ֐��B�N�����ɌĂ�ł������Ƃŏ���Đ����̒x���𖳂����B

	bool	init_player();				// �v���C���[������������
	bool	release_player();			// �v���C���[���������

	bool	is_playing();				// �Đ����H�i�����ł��Ă��Ȃ��A�������̓����V���b�g�Đ��̏ꍇ�ōĐ����������Ă���ꍇ�� false ���Ԃ�j
	bool	is_ready();					// �����ł��Ă�H

	bool	ready_omv(CTSTR& file_path, bool is_loop, bool is_onmemory);		// omv ����������
	void	end_loop();															// ���[�v���I������

	void	update_time_only(int now_time);
	bool	check_need_update(int now_time, int* ret_frame_no, bool by_force);
	bool	get_video_for_time(int now_time, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag);	// ���Ԃ��w�肵�ăr�f�I���擾����
	bool	get_video_for_frame(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag);	// �t���[�����w�肵�ăr�f�I���擾����

	void	get_theora_info(S_omv_theora_info* info);							// �V�I���̏����擾����
	void	get_theora_play_info(S_omv_theora_play_info* info);					// �V�I���̍Đ��Ɋւ�������擾����
	void	reset_theora_play_info();											// �V�I���̍Đ��Ɋւ���������Z�b�g����

	int		get_theora_total_frame_cnt();										// �V�I���̑��t���[�������擾
	int		get_theora_total_frame_time();										// �V�I���̑��t���[�����Ԃ��擾

	TSTR	get_last_error();													// �G���[���擾����

private:

};

}


