#pragma once



#include	"tonad3d3_d3d.h"
#include	"tonad3d3_sprite.h"

namespace NT3
{


class C_d3d_renderer;



class C_d3d_render_interface
{
protected:
	C_d3d_render_interface();

public:
	virtual ~C_d3d_render_interface();

	virtual bool	initialize(C_d3d_renderer* p_renderer);
	virtual bool	render_sprite_list(ARRAY<C_d3d_sprite *>& sprite_list);

	// デフォルトのビューポートを設定する
	void			set_default_view_port(const C_rect& rect);

protected:
	virtual bool	render_sprite(C_d3d_sprite* sprite, int vertex_cnt, int primitive_cnt);
	virtual bool	render_sprite_set_blend_state(const S_d3d_render_param* rp);
	virtual bool	render_sprite_begin_effect_d2(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, ID3DXEffect** pp_effect);
	virtual bool	render_sprite_begin_effect_d3(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, ID3DXEffect** pp_effect);
	virtual bool	render_sprite_begin_effect_d3_mesh(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_mtrl* p_mtrl, ID3DXEffect** pp_effect);
	virtual bool	render_sprite_begin_effect_d3_sub(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, const C_d3d_mtrl* p_mtrl, int texture_cnt, ID3DXEffect* p_effect);
	// 他のbegin系は余計な事をやっているので純粋にBegin()とBeginPass()だけを呼ぶ関数
	virtual bool	render_sprite_begin_effect(C_d3d_sprite* p_sprite, ID3DXEffect* p_effect);
	virtual bool	render_sprite_end_effect_d2(C_d3d_sprite* p_sprite, ID3DXEffect* p_effect);
	virtual bool	render_sprite_end_effect_d3(C_d3d_sprite* p_sprite, ID3DXEffect* p_effect);
	
	virtual bool	render_sprite_polygon_d2(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, int vertex_cnt, int primitive_cnt);
	virtual bool	render_sprite_polygon_d3(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, int vertex_cnt, int primitive_cnt);
	
	virtual bool	render_sprite_mesh(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_mesh* p_mesh);
	virtual bool	render_sprite_mesh_mtrl(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_mesh* p_mesh, int mtrl_no);
	
	virtual bool	render_sprite_frame(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame);
	virtual bool	render_sprite_frame_mesh_subset(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no);
	
	virtual bool	render_sprite_frame_per_mesh_set_effect_constant_common(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect);
	virtual bool	render_sprite_frame_per_mesh_set_effect_constant_mesh(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect);
	virtual bool	render_sprite_frame_per_mesh_set_effect_constant_skinned_mesh(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect);

	// エフェクト定数の設定
	virtual bool	render_sprite_frame_per_mesh_set_effect_constant(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect, ID3DXSkinInfo* p_skin_info);

	// フレーム描画時のメッシュ毎の描画条件(デフォルトは常にtrue)
	virtual bool	render_sprite_frame_per_mesh_draw_condition(C_d3d_mesh* p_mesh);

	// フレーム描画時のメッシュ毎のエフェクト生成
	virtual ID3DXEffect*	render_sprite_frame_per_mesh_create_effect(C_d3d_mesh* p_mesh, C_d3d_mtrl* p_mtrl, ID3DXSkinInfo* p_skin_info);


protected:
	C_rect			m_default_view_port;
	C_d3d_renderer*	m_p_renderer;

};

















}