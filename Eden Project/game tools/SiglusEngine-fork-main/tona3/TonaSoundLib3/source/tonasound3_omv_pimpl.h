#pragma		once

#include	<vorbis/codec.h>
#include	<theora/codec.h>
#include	<theora/theora.h>
#include	<theora/theoradec.h>

namespace NT3
{


// ****************************************************************
// C_omv_player ���́iPimpl �C�f�B�I���j
//		omv �������e��\���̂͋���ł��B
//		�w�b�_�Ɏ��̂�u�������Ȃ��̂ŁAPimpl �C�f�B�I�����g���ĉB�����܂��B
// ================================================================
class C_omv_player_impl
{

public:

private:
	TSTR						m_last_error;					// �G���[������

	S_omv_header				m_omv_header;					// omv �w�b�_�[

	int							m_state_flag;					// ���

	S_omv_theora_info			m_theora_info;					// �V�I���̏��
	S_omv_theora_play_info		m_theora_play_info;				// �V�I���̍Đ��Ɋւ�����

	C_file						m_file;							// �t�@�C���N���X

	bool						m_loop_flag;					// ���[�v�t���O
	bool						m_end_of_theora;				// �V�I���̍Đ��I���t���O
	int							m_seek_top;						// �t�@�C���V�[�N�̐擪�I�t�Z�b�g

	int							m_theora_total_page_cnt;		// �V�I���̑��y�[�W��
	int							m_theora_total_packet_cnt;		// �V�I���̑��p�P�b�g��
	int							m_theora_now_page_no;			// �V�I���̌��݂̃y�[�W�ԍ�
	int							m_theora_now_frame_no;			// �V�I���̌��݂̃t���[���ԍ�

	ARRAY<S_omv_theora_page>	m_theora_page_list;				// �y�[�W���X�g
	ARRAY<S_omv_theora_packet>	m_theora_packet_list;			// �V�I���̃p�P�b�g���X�g
	BUFFER						m_theora_onmemory_buffer;		// �I���������o�b�t�@�[
	BYTE*						m_theora_onmemory_buffer_p;		// �I���������o�b�t�@�[�|�C���^

	ogg_stream_state			m_theora_stream;				// �V�I���̃X�g���[��
	th_info						m_theora_header;				// �V�I���̃w�b�_�[
	th_comment					m_theora_comment;				// �V�I���̃R�����g
	th_dec_ctx*					m_theora_decoder;				// �V�I���̃G���R�[�_�[

public:
	C_omv_player_impl();
	~C_omv_player_impl();

	static void		ready_table();		// static �Ȋ֐��B�N�����ɌĂ�ł������Ƃŏ���Đ����̒x���𖳂����B

	bool	init_player();
	bool	release_player();

	TSTR	get_last_error();
	bool	is_playing();
	bool	is_ready();

	void	get_theora_info(S_omv_theora_info* info);
	void	get_theora_play_info(S_omv_theora_play_info* info);
	void	reset_theora_play_info();
	int		get_theora_total_frame_cnt();
	int		get_theora_total_frame_time();

	void	update_time_only(int now_time);
	bool	ready_omv(CTSTR& file_path, bool is_loop, bool is_onmemory);
	bool	check_need_update(int now_time, int* ret_frame_no, bool by_force);
	bool	get_video_for_time(int now_time, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag);
	bool	get_video_for_frame(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag);
	void	end_loop();

private:
	void	format_player();

	void	set_error(CTSTR& str);
	void	set_error_befor(CTSTR& str, bool return_separate);
	void	set_error_after(CTSTR& str, bool return_separate);

	int		read_page_for_theora(int page_no);
	int		read_page_for_theora_func(int page_no);
	void	empty_stream_for_theora(void);
	bool	check_player_error();
	int		time_to_frame(int now_time, bool set_m_end_of_theora);
	bool	ready_omv_func(CTSTR& file_path, bool is_loop, bool is_onmemory);
	bool	get_video_for_time_func(int now_time, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag);
	bool	get_video_for_frame_func(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag);
	bool	get_video_for_frame_funcfunc(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag);
	bool	get_video_for_frame_funcfuncfunc(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag);
	void	video_write(BYTE* p_dst_org, int dst_pitch);
	void	video_write_only_alpha(BYTE* p_dst_org, int dst_pitch);

	static int yuv2rgb_table[256][256][256];
};

}
