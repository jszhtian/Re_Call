#pragma once

#include "tonad3d3_render_interface.h"

namespace NT3
{



// shadow map:光源から見たシーンの深度値を書き込んだテクスチャ
// draw depth:深度値を書き込む(例:カメラから見た)
class C_d3d_pre_render_shadow_map : public C_d3d_render_interface {
public:
	C_d3d_pre_render_shadow_map(){};
	~C_d3d_pre_render_shadow_map(){};

	bool	render_sprite_list(ARRAY<C_d3d_sprite *>& sprite_list);

private:

	// エフェクト定数の設定
	bool	render_sprite_frame_per_mesh_set_effect_constant(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect, ID3DXSkinInfo* p_skin_info);

	// フレーム描画時のメッシュ毎の描画条件(プリレンダタイプがシャドウマップなら)
	bool	render_sprite_frame_per_mesh_draw_condition(C_d3d_mesh* p_mesh);

	// メッシュ毎のエフェクト生成
	ID3DXEffect*	render_sprite_frame_per_mesh_create_effect(C_d3d_mesh* p_mesh, C_d3d_mtrl* p_mtrl, ID3DXSkinInfo* p_skin_info);
};

















}