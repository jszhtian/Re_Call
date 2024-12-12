#include	"tonasound3_pch.h"
#include	"tonasound3_omv.h"
#include	"tonasound3_omv_pimpl.h"

namespace NT3
{

// ****************************************************************
// インターフェース
// ================================================================
C_omv_player::C_omv_player() : pi(new C_omv_player_impl)
{
}

C_omv_player::~C_omv_player()
{
}

void C_omv_player::ready_table()
{
	C_omv_player_impl::ready_table();
}

bool C_omv_player::init_player()
{
	return pi->init_player();
}

bool C_omv_player::ready_omv(CTSTR& file_path, bool is_loop, bool is_onmemory)
{
	return pi->ready_omv(file_path, is_loop, is_onmemory);
}

void C_omv_player::end_loop()
{
	return pi->end_loop();
}

void C_omv_player::update_time_only(int now_time)
{
	pi->update_time_only(now_time);
}

bool C_omv_player::check_need_update(int now_time, int* ret_frame_no, bool by_force)
{
	return pi->check_need_update(now_time, ret_frame_no, by_force);
}

bool C_omv_player::get_video_for_time(int now_time, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag)
{
	return pi->get_video_for_time(now_time, p_dst_org, dst_pitch, alpha_only, by_force, ret_update_flag);
}

bool C_omv_player::get_video_for_frame(int frame_no, BYTE* p_dst_org, int dst_pitch, bool alpha_only, bool by_force, bool *ret_update_flag)
{
	return pi->get_video_for_frame(frame_no, p_dst_org, dst_pitch, alpha_only, by_force, ret_update_flag);
}

bool C_omv_player::release_player()
{
	return pi->release_player();
}

bool C_omv_player::is_playing()
{
	return pi->is_playing();
}

bool C_omv_player::is_ready()
{
	return pi->is_ready();
}

void C_omv_player::get_theora_info(S_omv_theora_info* info)
{
	pi->get_theora_info(info);		
}

void C_omv_player::get_theora_play_info(S_omv_theora_play_info* info)
{
	pi->get_theora_play_info(info);		
}

void C_omv_player::reset_theora_play_info()
{
	pi->reset_theora_play_info();		
}

int C_omv_player::get_theora_total_frame_cnt()
{
	return pi->get_theora_total_frame_cnt();		
}

int C_omv_player::get_theora_total_frame_time()
{
	return pi->get_theora_total_frame_time();		
}

TSTR C_omv_player::get_last_error()
{
	return pi->get_last_error();	
}

}

