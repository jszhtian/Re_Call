#pragma once

#include	"tonad3d3_d3d.h"

namespace NT3
{

// ****************************************************************
// アニメーションコントローラ
// ================================================================
class C_d3d_animation_controller {
private:
	//ID3DXAnimationController*	m_pAnimCont;
	BIP<ID3DXAnimationController>	m_pAnimCont;
	UINT						m_AnimNo;
	UINT						m_AnimTrackNo;
	float 						m_AnimTimeHold;			// 前の時間
	float 						m_AnimTime;				// 現在のアニメーション時間
	float 						m_AnimShiftTime;		// アニメーション変更時間
	bool						m_IsAnim;				// アニメーションするか

public:
	C_d3d_animation_controller();
	~C_d3d_animation_controller();

	void update();
	void change_animation_no(UINT no);

	bool copy(C_d3d_animation_controller& source);

	void set_animation_controller(ID3DXAnimationController* p_ctrl);

	ID3DXAnimationController* get_animation_controller();
	UINT get_animation_no();
	bool get_is_animation();

	void release_animation_controller();

};


}