#include "tonad3d3_pch.h"
#include "tonad3d3_render_interface.h"
#include "tonad3d3_renderer.h"
#include "tonad3d3_debug_text.h"
#include "tonad3d3_camera.h"
#include "tonad3d3_light.h"
#include "tonad3d3_effect.h"
#include "tonad3d3_effect_source_creator_skinned_mesh.h"

namespace NT3
{


C_d3d_render_interface::C_d3d_render_interface()
{
	m_p_renderer = NULL;
}

C_d3d_render_interface::~C_d3d_render_interface()
{
	m_p_renderer = NULL;
}

bool C_d3d_render_interface::initialize(C_d3d_renderer* p_renderer)
{
	m_p_renderer = p_renderer;

	return true;
}

// デフォルトのビューポートを設定する
void C_d3d_render_interface::set_default_view_port(const C_rect& rect)
{
	m_default_view_port = rect;
}

// ****************************************************************
// スプライトリストをレンダー
// ================================================================
// レンダースプライトのリストを作成
// ブレンドありのものとなしのものを分けて扱う
//
// １．ブレンドなしのものをＺバッファの更新ありで描画
// ２．ブレンドありのものをＺバッファの更新なしで描画（Ｚバッファの使用自体はする）
//
// 本来は、子スプライトも含めて rp を計算してからリストに分ける必要がある
//
// メッシュはどうするよ？（検討中）
// ================================================================
bool C_d3d_render_interface::render_sprite_list(ARRAY<C_d3d_sprite *>& sprite_list)
{
	int sprite_cnt = (int)sprite_list.size();
	if (sprite_cnt == 0)
		return true;

	// パラメータが同じスプライトをまとめて描画する
	int top_sprite_no = 0;
	int total_vertex_cnt = 0;
	int total_primitive_cnt = 0;
	total_vertex_cnt += sprite_list[0]->get_vertex_cnt();		// １つ目のスプライトの頂点数を追加
	total_primitive_cnt += sprite_list[0]->get_primitive_cnt();	// １つ目のスプライトのプリミティブ数を追加
	for (int sprite_no = 1; ; sprite_no++)	{
		
		// 最後もしくはパラメータが変わったら描画
		bool is_draw = false;
		if (sprite_no >= sprite_cnt)	{
			is_draw = true;
		}
		else	{

			C_d3d_sprite* s1 = sprite_list[top_sprite_no];
			C_d3d_sprite* s2 = sprite_list[sprite_no];
			if (false
				|| s1->rp.d3_rect		// ▲片方でも３Ｄ矩形の場合、別々に書く
				|| s2->rp.d3_rect		// ▲カメラの設定が違うからです。３Ｄ矩形はカメラじゃなく set_d2_vertex_param で計算すべきだね。要修正。
				|| s1->type != s2->type
				|| s1->rp.disp != s2->rp.disp
				|| s1->rp.d2_rect != s2->rp.d2_rect

//				|| s1->rp.size != s2->rp.size
//				|| s1->rp.size_fit_to_texture != s2->rp.size_fit_to_texture
//				|| s1->rp.pos != s2->rp.pos
//				|| s1->rp.center != s2->rp.center
//				|| s1->rp.scale != s2->rp.scale
//				|| s1->rp.rotate != s2->rp.rotate
				|| s1->rp.dst_clip_use != s2->rp.dst_clip_use
				|| s1->rp.dst_clip != s2->rp.dst_clip
				|| s1->rp.tr != s2->rp.tr
				|| s1->rp.mono != s2->rp.mono
				|| s1->rp.reverse != s2->rp.reverse
				|| s1->rp.bright != s2->rp.bright
				|| s1->rp.dark != s2->rp.dark
				|| s1->rp.color_r != s2->rp.color_r
				|| s1->rp.color_g != s2->rp.color_g
				|| s1->rp.color_b != s2->rp.color_b
				|| s1->rp.color_rate != s2->rp.color_rate
				|| s1->rp.color_add_r != s2->rp.color_add_r
				|| s1->rp.color_add_g != s2->rp.color_add_g
				|| s1->rp.tone_curve_no != s2->rp.tone_curve_no
				|| s1->rp.tone_curve_sat != s2->rp.tone_curve_sat
				|| s1->rp.light_no != s2->rp.light_no
				|| s1->rp.culling != s2->rp.culling
				|| s1->rp.use_mask != s2->rp.use_mask
				|| s1->rp.use_fog != s2->rp.use_fog
				|| s1->rp.alpha_test != s2->rp.alpha_test
				|| s1->rp.alpha_blend != s2->rp.alpha_blend
				|| s1->rp.blend_type != s2->rp.blend_type
				|| s1->get_vertex_buffer() != s2->get_vertex_buffer()
				|| s1->get_index_buffer() != s2->get_index_buffer()
				|| s1->get_mesh() != s2->get_mesh()
				|| s1->get_frame_set() != s2->get_frame_set()
				|| s1->get_texture(0) != s2->get_texture(0)
				|| s1->get_texture(1) != s2->get_texture(1)
				|| s1->get_texture(2) != s2->get_texture(2)
				|| s1->get_texture(3) != s2->get_texture(3)
				|| s1->get_effect() != s2->get_effect()
				|| s1->get_effect_technique() != s2->get_effect_technique()
				|| s1->get_effect_constant_f(0)[0] != s2->get_effect_constant_f(0)[0]
				|| s1->get_effect_constant_f(0)[1] != s2->get_effect_constant_f(0)[1]
				|| s1->get_effect_constant_f(0)[2] != s2->get_effect_constant_f(0)[2]
				|| s1->get_effect_constant_f(0)[3] != s2->get_effect_constant_f(0)[3]
				|| s1->get_effect_constant_f(1)[0] != s2->get_effect_constant_f(1)[0]
				|| s1->get_effect_constant_f(1)[1] != s2->get_effect_constant_f(1)[1]
				|| s1->get_effect_constant_f(1)[2] != s2->get_effect_constant_f(1)[2]
				|| s1->get_effect_constant_f(1)[3] != s2->get_effect_constant_f(1)[3]
			)
			{
				is_draw = true;
			}
		}

		if (is_draw)	{

			// draw_first_no ～ (sprite_no - 1) までを描画
			C_d3d_sprite* p_sprite = sprite_list[top_sprite_no];

			// スプライトを描画
			if (!render_sprite(p_sprite, total_vertex_cnt, total_primitive_cnt))
				return false;

			// 次の描画開始位置を設定
			top_sprite_no = sprite_no;
			total_vertex_cnt = 0;
			total_primitive_cnt = 0;
		}

		// 最後まで描画したら終了
		if (sprite_no >= sprite_cnt)
			break;

		total_vertex_cnt += sprite_list[sprite_no]->get_vertex_cnt();
		total_primitive_cnt += sprite_list[sprite_no]->get_primitive_cnt();
	}

	return true;
}

// ****************************************************************
// スプライトをレンダー
// ================================================================
//		親スプライトとデータを合成します。
//		ビルボード、２Ｄスプライト、など特殊なパラメータの設定を行います。
//		テクスチャ、シェーダ、エフェクトなどの設定を行います。
//		最後にスプライトをレンダーします。
// ================================================================
bool C_d3d_render_interface::render_sprite(C_d3d_sprite* p_sprite, int vertex_cnt, int primitive_cnt)
{
	// 表示フラグがＯＦＦのスプライトを除外
	// 子供の描画も行わないことに注意
	if (!p_sprite->rp.disp)
		return true;	// 正常終了

	// ビルボードの場合はカメラにあわせてスプライトを回転させる
#if 0
	if (p_sprite->rp.billboard)	{

		// カメラに合わせてスプライトを回転
		D3DXVECTOR3 dir = m_camera.pint - m_camera.eye;
		dir = D3DXVECTOR3(dir.x, 0.0f, dir.z);							// xz 平面に投射
		float dot = D3DXVec3Dot(&dir, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));	// 内積
		float cos = dot / D3DXVec3Length(&dir);							// cosθ
		float theta = acos(cos);										// θ
		if (dir.x < 0.0f)	{
			theta = D3DX_PI * 2 - theta;
		}
		p_sprite->rp.rotate.y = theta;
	}
#endif

	// アルファテストの設定（有効、１以上が通過）
	if (p_sprite->rp.alpha_test)	{
		G_d3d.state.set_render_state(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテストあり
		G_d3d.state.set_render_state(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		G_d3d.state.set_render_state(D3DRS_ALPHAREF, 0x01);
	}
	else	{
		G_d3d.state.set_render_state(D3DRS_ALPHATESTENABLE, FALSE);		// アルファテストなし
	}

	// アルファブレンドの設定
	if (p_sprite->rp.alpha_blend)	{
		G_d3d.state.set_render_state(D3DRS_ALPHABLENDENABLE, TRUE);		// アルファブレンドあり
	}
	else	{
		G_d3d.state.set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);	// アルファブレンドなし
	}

	// カリングの設定
	if (p_sprite->rp.culling)	{
		G_d3d.state.set_render_state(D3DRS_CULLMODE, D3DCULL_CCW);		// カリングを行う
	}
	else	{
		G_d3d.state.set_render_state(D3DRS_CULLMODE, D3DCULL_NONE);		// カリングを行わない
	}

	// ブレンディング係数を設定
	render_sprite_set_blend_state(&p_sprite->rp);

	// ２Ｄ矩形の場合の特殊設定
	if (p_sprite->rp.d2_rect)	{

		// ２Ｄ矩形スプライトはＺソートを行わない
		G_d3d.state.set_render_state(D3DRS_ZENABLE, FALSE);				// Ｚソートを行わない
		G_d3d.state.set_render_state(D3DRS_ZWRITEENABLE, FALSE);		// Ｚバッファを更新しない

		// ２Ｄ矩形スプライトはテクスチャをループさせない
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);

		// ２Ｄ矩形はクリッピングをビューポートで設定できる
		if (p_sprite->rp.dst_clip_use)	{

			// ビューポートはデフォルトのクリップに収める（GeForce とかだと、画面外を指定すると完全に表示されないグラボがあります。）
			C_rect clip = p_sprite->rp.dst_clip;
			clip.left = std::max(clip.left, m_default_view_port.left);
			clip.top = std::max(clip.top, m_default_view_port.top);
			clip.right = std::min(clip.right, m_default_view_port.right);
			clip.bottom = std::min(clip.bottom, m_default_view_port.bottom);
			G_d3d.device.set_view_port(clip);
		}
		else	{
			G_d3d.device.set_view_port(m_default_view_port);
		}
	}

	// ３Ｄ矩形の場合の特殊設定
	else if (p_sprite->rp.d3_rect)	{

		// ３Ｄ矩形はＺソートを行う
		G_d3d.state.set_render_state(D3DRS_ZENABLE, TRUE);

		// 透過処理がある場合はＺバッファを更新しない
		if (!p_sprite->rp.alpha_blend)	{
			G_d3d.state.set_render_state(D3DRS_ZWRITEENABLE, TRUE);			// Ｚバッファを更新する
		}
		else	{
			G_d3d.state.set_render_state(D3DRS_ZWRITEENABLE, FALSE);		// Ｚバッファを更新しない
		}

		// ３Ｄ矩形スプライトはテクスチャをループさせない
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
	}

	// ビルボードの場合の特殊設定
	else if (p_sprite->rp.d3_billboard)	{

		// ３ＤスプライトはＺソートを行う
		G_d3d.state.set_render_state(D3DRS_ZENABLE, TRUE);

		// 透過処理がある場合はＺバッファを更新しない
		if (!p_sprite->rp.alpha_blend)	{
			G_d3d.state.set_render_state(D3DRS_ZWRITEENABLE, TRUE);			// Ｚバッファを更新する
		}
		else	{
			G_d3d.state.set_render_state(D3DRS_ZWRITEENABLE, FALSE);		// Ｚバッファを更新しない
		}

		// ビルボードはテクスチャをループさせない
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_BORDERCOLOR, 0x00000000);	// 境界の色は透明
	}

	// ３Ｄスプライトの場合の特殊設定
	else	{

		// ３ＤスプライトはＺソートを行う
		G_d3d.state.set_render_state(D3DRS_ZENABLE, TRUE);

		// 透過処理がある場合はＺバッファを更新しない
		if (!p_sprite->rp.alpha_blend)	{
			G_d3d.state.set_render_state(D3DRS_ZWRITEENABLE, TRUE);			// Ｚバッファを更新する
		}
		else	{
			G_d3d.state.set_render_state(D3DRS_ZWRITEENABLE, FALSE);		// Ｚバッファを更新しない
		}

		// ３Ｄの場合はテクスチャをループさせる → ループさせないようにしました（2010/10/22 ３Ｄワイプ対策）
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(0, D3DSAMP_BORDERCOLOR, 0x00000000);	// 境界の色は透明

		// カメラの反対側にいるスプライトを除外
		//D3DXVECTOR3 pnt = camera.pint - camera.eye;
		//D3DXVECTOR3 pos = rp.pos - camera.eye;
		//if (D3DXVec3Dot(&pnt, &pos) < 0)
		//	return false;
	}

	// ステージ０についてミップマップを使うかどうか
	// ミップマップの使えないテクスチャの場合はミップマップを使用しない
	if (p_sprite->get_texture(0) && p_sprite->get_texture(0)->is_mipmap_enable())	{
		G_d3d.state.set_sampler_state(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	}
	else	{
		G_d3d.state.set_sampler_state(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		G_d3d.state.set_sampler_state(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	}

	// ピクセルシェーダの初期化
	G_d3d.device->SetPixelShader(NULL);

	// テクスチャの初期化
	for (int t = 0; t < D3D_TEXTURE_CNT_MAX; t++)
		G_d3d.device->SetTexture(t, NULL);

	// スプライトタイプによって場合わけ
	if (false);

	// 「タイプ：ポリゴン」の描画（２Ｄ）
	else if (p_sprite->type == E_d3d_sprite_type_polygon && p_sprite->rp.d2_rect)	{

		if (!render_sprite_polygon_d2(p_sprite, &p_sprite->rp, vertex_cnt, primitive_cnt))
			return false;
	}
	// 「タイプ：ポリゴン」の描画（ビルボード）
	else if (p_sprite->type == E_d3d_sprite_type_polygon && p_sprite->rp.d3_billboard)	{

		// ▲ビルボードはいったん未対応にします。
#if 0
		// 頂点をストリームに流し込む
		Gd3d.device->SetStreamSource(0, p_sprite->get_vertex_buffer()->body(), 0, p_sprite->get_FVF_size());
		Gd3d.device->SetIndices(p_sprite->get_index_buffer()->body());	// インデックスバッファの設定
		Gd3d.device->SetFVF(p_sprite->get_FVF());							// FVF の設定

		// テクスチャを設定する
		for (int t = 0; t < D3D_TEXTURE_STAGE_MAX; t++)	{
			if (p_sprite->get_texture(t))
				Gd3d.device->SetTexture(t, p_sprite->get_texture(t)->body());
		}

		// 描画を行う
		Gd3d.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, p_sprite->get_vertex_count(), 0, p_sprite->get_index_count() / 3);
#endif
	}
	// 「タイプ：ポリゴン」の描画（３Ｄ）
	else if (p_sprite->type == E_d3d_sprite_type_polygon)	{

		if (!render_sprite_polygon_d3(p_sprite, &p_sprite->rp, vertex_cnt, primitive_cnt))
			return false;
	}

	// 「タイプ：メッシュ」の描画
	else if (p_sprite->type == E_d3d_sprite_type_mesh && p_sprite->get_mesh())	{

		// メッシュを描画する
		if (!render_sprite_mesh(p_sprite, &p_sprite->rp, p_sprite->get_mesh().get()))
			return false;
	}

	// 「タイプ：フレームセット」の描画
	else if (p_sprite->type == E_d3d_sprite_type_frame_set && p_sprite->get_frame_set())	{
		BSP<C_d3d_frame_set> frame_set = p_sprite->get_frame_set();

		/*
		// ワールド変換行列を計算
		D3DXMATRIX mat_world, mat_temp;

		// ワールド変換行列を設定
		float yaw = p_sprite->rp.rotate.y;
		float pitch = p_sprite->rp.rotate.x;
		float roll = p_sprite->rp.rotate.z;

		// ワールド座標変換行列を作成(S-R-T)
		D3DXMatrixIdentity(&mat_world);
		D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixScaling(&mat_temp, p_sprite->rp.scale.x, p_sprite->rp.scale.y, p_sprite->rp.scale.z));	// 拡縮
		D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixRotationYawPitchRoll(&mat_temp, yaw, pitch, roll));									// 回転
		D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixTranslation(&mat_temp, p_sprite->rp.pos.x, p_sprite->rp.pos.y, p_sprite->rp.pos.z));	// 移動

		//// （頂点シェーダを使わない場合）ワールド変換行列を設定
		//Gd3d.device->SetTransform(D3DTS_WORLD, &mat_world);

		// ワールド変換行列を元にフレーム変換行列を生成する
		frame_set->update_matrix(&mat_world);
		// アニメーションを進める
		frame_set->update_animation_controller();

		// ここの時点でワールド行列変換込みのスキンアニメーションを行わないアニメーション行列は算出済み？
		// プリレンダの場合どうするか？
		// フレームの描画順はどうなっているのか？
		*/

		// 全てのフレームを順に描画する
		if (!render_sprite_frame(p_sprite, &p_sprite->rp, frame_set->m_frame_root.get()))
			return false;
	}

	return true;
}

// ****************************************************************
// スプライト：ブレンドステートを設定
// ================================================================
bool C_d3d_render_interface::render_sprite_set_blend_state(const S_d3d_render_param* rp)
{
	if (rp->blend_type == E_d3d_blend_type_alpha)	{			// 透過

		// result = d * (1 - a) + s * a

		G_d3d.state.set_render_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		G_d3d.state.set_render_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		G_d3d.state.set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	}
	else if (rp->blend_type == E_d3d_blend_type_add)	{		// 加算

		// result = d + s * a

		G_d3d.state.set_render_state(D3DRS_DESTBLEND, D3DBLEND_ONE);
		G_d3d.state.set_render_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		G_d3d.state.set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	}
	else if (rp->blend_type == E_d3d_blend_type_sub)	{		// 減算

		// result = d - s * a

		G_d3d.state.set_render_state(D3DRS_DESTBLEND, D3DBLEND_ONE);
		G_d3d.state.set_render_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		G_d3d.state.set_render_state(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	}
	else if (rp->blend_type == E_d3d_blend_type_mul)	{		// 乗算

		// result = d * s（エフェクト内で a を適用させています）

		G_d3d.state.set_render_state(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		G_d3d.state.set_render_state(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		G_d3d.state.set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	}
	else if (rp->blend_type == E_d3d_blend_type_screen)	{	// スクリーン

		// result = 1 - (1 - d) * (1 - s)
		//        = (1 - s) * d + s
		//
		// （エフェクト内で a を適用させています）

		G_d3d.state.set_render_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		G_d3d.state.set_render_state(D3DRS_SRCBLEND, D3DBLEND_ONE);
		G_d3d.state.set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	}
	else	{												// 通常

		// result = d * (1 - a) + s * a

		G_d3d.state.set_render_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		G_d3d.state.set_render_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		G_d3d.state.set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	}

	return true;
}

// ****************************************************************
// エフェクトを開始（２Ｄ）
// ================================================================
bool C_d3d_render_interface::render_sprite_begin_effect_d2(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, ID3DXEffect** pp_effect)
{
	HRESULT hr;

	ID3DXEffect* p_effect = p_sprite->get_effect();
	D3DXHANDLE h_technique = NULL;
	UINT pass_cnt = 0;

	// エフェクトが指定されていない場合はデフォルトのエフェクトを使う
	if (p_effect == NULL)	{

		// エフェクトの制限
		if (rp->use_mask && rp->light_no >= 0)	{
			set_last_error(_T("エフェクトの組み合わせでエラーが発生しました。(1)"), _T("SetEffect"));
			return false;
		}
		if (!rp->d3_rect && rp->light_no >= 0)	{
			set_last_error(_T("エフェクトの組み合わせでエラーが発生しました。(2)"), _T("SetEffect"));
			return false;
		}

		// エフェクトをデフォルトに設定する
		p_effect = m_p_renderer->get_default_effect()->body();

		// テクニックを取得する
		h_technique = p_sprite->get_effect_technique();
		int tec_type[10] = {0};

		// テクニックが指定されていない場合は自動生成
		if (h_technique == NULL)	{

			// ２Ｄ
			tec_type[0] = 0;
			// ライト
			tec_type[1] = 0;
			// フォグ
			tec_type[2] = 0;
			// テクスチャ
			if (p_sprite->get_texture(0) && p_sprite->get_texture(0)->body())	{
				tec_type[3] = 1;
			}
			// diffuse
			if (rp->filter.color < 0xffffffff || rp->tr < 255)	{
				tec_type[4] = 1;
			}
			// mono, reverse, bright, dark
			if (rp->mono > 0 || rp->reverse > 0 || rp->bright > 0 || rp->dark > 0)	{
				tec_type[5] = 1;
			}
			// color, color_add
			if (rp->color_rate > 0 || rp->color_add_r > 0 || rp->color_add_g > 0 || rp->color_add_b > 0)	{
				tec_type[6] = 1;
			}
			// トーンカーブ
			if (rp->tone_curve_no >= 0)	{
				tec_type[7] = 1;
			}
			// マスク
			if (rp->use_mask == 1)	{
				tec_type[8] = 1;
			}
			// 乗算（d * s * a は Direct3D ではできないので、s = s * a を先に計算しておき、後で d * s とする。）
			if (rp->blend_type == E_d3d_blend_type_mul)	{
				tec_type[9] = 1;
			}
			// スクリーン
			if (rp->blend_type == E_d3d_blend_type_screen)	{
				tec_type[9] = 2;
			}

			// エフェクト効果が全くないならエフェクトを使わない（テクスチャは除く）
			if (tec_type[1] == 0 && tec_type[3] == 0 && tec_type[4] == 0 && tec_type[5] == 0 && tec_type[6] == 0 && tec_type[7] == 0 && tec_type[8] == 0 && tec_type[9] == 0)	{

				// エフェクトを取り消し
				p_effect = NULL;
			}
		}

		// 結局エフェクトを使う場合
		if (p_effect)	{

			// テクニックが指定されていない場合は自動生成
			if (h_technique == NULL)	{
				h_technique = m_p_renderer->get_default_effect()->m_sprite_technique[tec_type[0]][tec_type[1]][tec_type[2]][tec_type[3]][tec_type[4]][tec_type[5]][tec_type[6]][tec_type[7]][tec_type[8]][tec_type[9]];

				// エフェクトのテクスチャを設定する
				for (int t = 0; t < D3D_TEXTURE_CNT_MAX; t++)	{
					if (p_sprite->get_texture(t) && p_sprite->get_texture(t)->body())	{
						hr = p_effect->SetTexture(m_p_renderer->get_default_effect()->m_texture[t], p_sprite->get_texture(t)->body());
						if (FAILED(hr))	{
							set_last_error_d3d(_T("エフェクトのテクスチャの設定に失敗しました。"), _T("SetTexture"), hr);
							return false;
						}
					}
				}

				// エフェクトの定数を設定する
				if (rp->mono > 0 || rp->reverse > 0 || rp->bright > 0 || rp->dark > 0 || rp->color_rate > 0 || rp->color_add_r > 0 || rp->color_add_g > 0 || rp->color_add_b > 0)	{

					// mono, reverse, bright, dark を定数として送る
					float c0[4] = {(float)rp->mono / 255, (float)rp->reverse / 255, (float)rp->bright / 255, (float)rp->dark / 255};
					hr = p_effect->SetFloatArray(m_p_renderer->get_default_effect()->m_float_array[0], c0, 4);
					if (FAILED(hr))	{
						set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetFloatArray"), hr);
						return false;
					}

					// r, g, b, rate を定数として送る
					float c1[4] = {(float)rp->color_r / 255, (float)rp->color_g / 255, (float)rp->color_b / 255, (float)rp->color_rate / 255};
					hr = p_effect->SetFloatArray(m_p_renderer->get_default_effect()->m_float_array[1], c1, 4);
					if (FAILED(hr))	{
						set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetFloatArray"), hr);
						return false;
					}

					// r, g, b, を定数として送る
					float c2[4] = {(float)rp->color_add_r / 255, (float)rp->color_add_g / 255, (float)rp->color_add_b / 255};
					hr = p_effect->SetFloatArray(m_p_renderer->get_default_effect()->m_float_array[2], c2, 4);
					if (FAILED(hr))	{
						set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetFloatArray"), hr);
						return false;
					}
				}
				if (rp->tone_curve_no >= 0)	{

					// トーンカーブ情報を定数として送る
					float c3[4] = {((float)(BYTE)rp->tone_curve_no + 0.5f) / 255, (float)rp->tone_curve_sat / 255};
					hr = p_effect->SetFloatArray(m_p_renderer->get_default_effect()->m_float_array[3], c3, 4);
					if (FAILED(hr))	{
						set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetFloatArray"), hr);
						return false;
					}
				}
			}

			// テクニックが指定されている場合はそれに従う
			else	{

				// エフェクトのテクスチャを設定する
				for (int t = 0; t < D3D_TEXTURE_CNT_MAX; t++)	{
					if (p_sprite->get_texture(t) && p_sprite->get_texture(t)->body())	{
						hr = p_effect->SetTexture(m_p_renderer->get_default_effect()->m_texture[t], p_sprite->get_texture(t)->body());
						if (FAILED(hr))	{
							set_last_error_d3d(_T("エフェクトのテクスチャの設定に失敗しました。"), _T("SetTexture"), hr);
							return false;
						}
					}
				}

				// エフェクトの定数を送る
				hr = p_effect->SetFloatArray(m_p_renderer->get_default_effect()->m_float_array[0], p_sprite->get_effect_constant_f(0), 4);
				if (FAILED(hr))	{
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetFloatArray"), hr);
					return false;
				}
				hr = p_effect->SetFloatArray(m_p_renderer->get_default_effect()->m_float_array[1], p_sprite->get_effect_constant_f(1), 4);
				if (FAILED(hr))	{
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetFloatArray"), hr);
					return false;
				}
			}

			// エフェクトのテクニックを設定する
			hr = p_effect->SetTechnique(h_technique);
			if (FAILED(hr))	{
				set_last_error_d3d(_T("エフェクトテクニックの設定に失敗しました。"), _T("SetTechnique"), hr);
				return false;
			}

			// エフェクトを開始する
			hr = p_effect->Begin(&pass_cnt, 0);
			if (FAILED(hr))	{
				set_last_error_d3d(_T("エフェクトの開始に失敗しました。"), _T("Begin"), hr);
				return false;
			}

			// パスを実行する
			hr = p_effect->BeginPass(0);
			if (FAILED(hr))	{
				set_last_error_d3d(_T("エフェクトのパスの設定に失敗しました。"), _T("BeginPass"), hr);
				return false;
			}

			// 使用したエフェクトを返す
			*pp_effect = p_effect;
		}

		// エフェクトを使わない場合
		else	{

			// テクスチャを設定する
			for (int t = 0; t < D3D_TEXTURE_CNT_MAX; t++)	{
				if (p_sprite->get_texture(t) && p_sprite->get_texture(t)->body())	{
					hr = G_d3d.device->SetTexture(t, p_sprite->get_texture(t)->body());
					if (FAILED(hr))	{
						set_last_error_d3d(_T("エフェクトのテクスチャの設定に失敗しました。"), _T("SetTexture"), hr);
						return false;
					}
				}
			}
		}
	}

	// エフェクトが指定されている場合：▲未実装

	return true;
}

// ****************************************************************
// エフェクトを終了（２Ｄ）
// ================================================================
bool C_d3d_render_interface::render_sprite_end_effect_d2(C_d3d_sprite* p_sprite, ID3DXEffect* p_effect)
{
	HRESULT hr;

	if (p_effect == NULL)
		return true;

	hr = p_effect->EndPass();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("エフェクトのパスの終了に失敗しました。"), _T("EndPass"), hr);
		return false;
	}

	hr = p_effect->End();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("エフェクトの終了に失敗しました。"), _T("End"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// エフェクトを開始（３Ｄ）
// ================================================================
bool C_d3d_render_interface::render_sprite_begin_effect_d3(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, ID3DXEffect** pp_effect)
{
	HRESULT hr;
	ID3DXEffect* p_effect = m_p_renderer->get_default_effect()->body();

	// テクスチャを設定する
	int texture_cnt = 0;
	for (int t = 0; t < D3D_TEXTURE_CNT_MAX; t++)	{
		if (p_sprite->get_texture(t) && p_sprite->get_texture(t)->body())	{
			TSTR name = str_format(_T("Tex%02d"), t);
			hr = p_effect->SetTexture(TSTR_to_MBSTR(name).c_str(), p_sprite->get_texture(t)->body());
			if (FAILED(hr))	{
				set_last_error_d3d(_T("エフェクトのテクスチャの設定に失敗しました。"), _T("SetTexture"), hr);
				return false;
			}

			texture_cnt = t + 1;	// 最大値がテクスチャ数になる
		}
	}

	// エフェクトの設定続き
	if (!render_sprite_begin_effect_d3_sub(p_sprite, rp, NULL, texture_cnt, p_effect))
		return false;

	*pp_effect = p_effect;
	return true;
}

bool C_d3d_render_interface::render_sprite_begin_effect_d3_mesh(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, C_d3d_mtrl* p_mtrl, ID3DXEffect** pp_effect)
{
	HRESULT hr;
	ID3DXEffect* p_effect = m_p_renderer->get_default_effect()->body();

	// テクスチャを設定する
	int texture_cnt = 0;
	if (p_mtrl->texture && p_mtrl->texture->body())	{
		TSTR name = str_format(_T("Tex%02d"), texture_cnt);
		hr = p_effect->SetTexture(TSTR_to_MBSTR(name).c_str(), p_mtrl->texture->body());
		if (FAILED(hr))	{
			set_last_error_d3d(_T("エフェクトのテクスチャの設定に失敗しました。"), _T("SetTexture"), hr);
			return false;
		}

		p_sprite->set_texture(p_mtrl->texture);

		texture_cnt = 1;	// 最大値がテクスチャ数になる
	}
	//p_sprite->rp.light_no = 0;

	// マテリアルの色を設定する
	

	// エフェクトの設定続き
	if (!render_sprite_begin_effect_d3_sub(p_sprite, rp, p_mtrl, texture_cnt, p_effect))
		return false;

	*pp_effect = p_effect;
	return true;
}

bool C_d3d_render_interface::render_sprite_begin_effect_d3_sub(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, const C_d3d_mtrl* p_mtrl, int texture_cnt, ID3DXEffect* p_effect)
{
	HRESULT hr;

	// テクニックを取得する
	D3DXHANDLE h_technique = p_sprite->get_effect_technique();
	int tec_type[10] = {0};

	// テクニックが指定されていない場合は自動生成
	if (h_technique == NULL)	{

		// エフェクトの制限
		if (rp->use_mask && rp->light_no >= 0)	{
			set_last_error(_T("エフェクトの組み合わせでエラーが発生しました。(1)"), _T("SetEffect"));
			return false;
		}

		// ３Ｄ
		tec_type[0] = 1;
		// ライト
		if (p_sprite->rp.light_no >= 0)	{
			tec_type[1] = 1;
		}
		// フォグ
		if (m_p_renderer->m_fog_use && p_sprite->rp.use_fog)	{
			tec_type[2] = 1;
		}
		// テクスチャ
		if (p_sprite->get_texture(0) && p_sprite->get_texture(0)->body())	{
			tec_type[3] = 1;
		}
		// diffuse（ライトの都合上かならず設定する）
		tec_type[4] = 1;
		// mono, reverse, bright, dark
		if (rp->mono > 0 || rp->reverse > 0 || rp->bright > 0 || rp->dark > 0)	{
			tec_type[5] = 1;
		}
		// color, color_add
		if (rp->color_rate > 0 || rp->color_add_r > 0 || rp->color_add_g > 0 || rp->color_add_b > 0)	{
			tec_type[6] = 1;
		}
		// トーンカーブ
		if (rp->tone_curve_no >= 0)	{
			tec_type[7] = 1;
		}
		// マスク
		if (rp->use_mask == 1)	{
			tec_type[8] = 1;
		}
		// 乗算（d * s * a は Direct3D ではできないので、s = s * a を先に計算しておき、後で d * s とする。）
		if (rp->blend_type == E_d3d_blend_type_mul)	{
			tec_type[9] = 1;
		}
		// スクリーン
		if (rp->blend_type == E_d3d_blend_type_screen)	{
			tec_type[9] = 2;
		}

		// テクニックを取得する
		h_technique = m_p_renderer->get_default_effect()->m_sprite_technique[tec_type[0]][tec_type[1]][tec_type[2]][tec_type[3]][tec_type[4]][tec_type[5]][tec_type[6]][tec_type[7]][tec_type[8]][tec_type[9]];

		// テクニックを設定する
		hr = p_effect->SetTechnique(h_technique);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("エフェクトテクニックの設定に失敗しました。"), _T("SetTechnique"), hr);
			return false;
		}

		// エフェクトの定数を設定する
		if (rp->mono > 0 || rp->reverse > 0 || rp->bright > 0 || rp->dark > 0 || rp->color_rate > 0 || rp->color_add_r > 0 || rp->color_add_g > 0 || rp->color_add_b > 0)	{

			// mono, reverse, bright, dark を定数として送る
			float c0[4] = {(float)rp->mono / 255, (float)rp->reverse / 255, (float)rp->bright / 255, (float)rp->dark / 255};
			hr = p_effect->SetFloatArray("c0", c0, 4);
			if (FAILED(hr))	{
				set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetFloatArray"), hr);
				return false;
			}

			// r, g, b, rate を定数として送る
			float c1[4] = {(float)rp->color_r / 255, (float)rp->color_g / 255, (float)rp->color_b / 255, (float)rp->color_rate / 255};
			hr = p_effect->SetFloatArray("c1", c1, 4);
			if (FAILED(hr))	{
				set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetFloatArray"), hr);
				return false;
			}

			// r, g, b, を定数として送る
			float c2[4] = {(float)rp->color_add_r / 255, (float)rp->color_add_g / 255, (float)rp->color_add_b / 255};
			hr = p_effect->SetFloatArray("c2", c2, 4);
			if (FAILED(hr))	{
				set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetFloatArray"), hr);
				return false;
			}
		}
		if (rp->tone_curve_no >= 0)	{

			// トーンカーブ情報を定数として送る
			float c3[4] = {((float)(BYTE)rp->tone_curve_no + 0.5f) / 255, (float)rp->tone_curve_sat / 255};
			hr = p_effect->SetFloatArray("c3", c3, 4);
			if (FAILED(hr))	{
				set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetFloatArray"), hr);
				return false;
			}
		}
	}

	// テクニックが指定されている場合はそれに従う
	else	{

		// テクニックを設定する
		hr = p_effect->SetTechnique(h_technique);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("エフェクトテクニックの設定に失敗しました。"), _T("SetTechnique"), hr);
			return false;
		}

		// 定数を送る
		hr = p_effect->SetFloatArray("c0", p_sprite->get_effect_constant_f(0), 4);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetFloatArray"), hr);
			return false;
		}
		hr = p_effect->SetFloatArray("c1", p_sprite->get_effect_constant_f(1), 4);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetFloatArray"), hr);
			return false;
		}
	}

	D3DXMATRIX mat_temp, mat_world, mat_view, mat_proj, mat_view_proj;

	// ワールド座標変換行列を作成
	D3DXMatrixIdentity(&mat_world);
	D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixScaling(&mat_temp, rp->scale.x, rp->scale.y, rp->scale.z));					// 拡縮
	D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixRotationYawPitchRoll(&mat_temp, rp->rotate.y, rp->rotate.x, rp->rotate.z));	// 回転
	D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixTranslation(&mat_temp, rp->pos.x, rp->pos.y, rp->pos.z));						// 移動

	// ビューと射影の合成行列を求めておく
	mat_view = *m_p_renderer->get_view_matrix();
	mat_proj = *m_p_renderer->get_proj_matrix();
	D3DXMatrixMultiply(&mat_view_proj, m_p_renderer->get_view_matrix(), m_p_renderer->get_proj_matrix());

	// 行列を設定する
	p_effect->SetMatrix("g_mat_world", &mat_world);
	p_effect->SetMatrix("g_mat_view_proj", &mat_view_proj);

	// 行列とは別に設定する（ライトの計算に使う）
	float canera_dir[3] = {m_p_renderer->m_camera_dir.x, m_p_renderer->m_camera_dir.y, m_p_renderer->m_camera_dir.z};
	p_effect->SetFloatArray("g_camera_dir", canera_dir, 3);

	// マテリアルカラーを設定する
	if (p_mtrl)	{
		p_effect->SetFloatArray("g_mtrl_diffuse", (float *)&p_mtrl->mtrl.Diffuse, 4);
	}
	else	{
		float white[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		p_effect->SetFloatArray("g_mtrl_diffuse", white, 4);
	}

	// ライトを設定する
	if (p_sprite->rp.light_no >= 0)	{
		C_d3d_light light = G_d3d.state.get_light(p_sprite->rp.light_no);
		float light_pos[4] = {light.Position.x, light.Position.y, light.Position.z, 1.0f};	// x, y, z, use
		float light_dir[3] = {light.Direction.x, light.Direction.y, light.Direction.z};
		float light_diffuse[4] = {light.Diffuse.r, light.Diffuse.g, light.Diffuse.b, light.Diffuse.a};
		float light_ambient[4] = {light.Ambient.r, light.Ambient.g, light.Ambient.b, light.Ambient.a};
		//int camera_light_calc_type = light.camera_light_calc_type;
		//int ambient_light_calc_type = light.ambient_light_calc_type;

		p_effect->SetFloatArray("g_light_pos", light_pos, 4);
		//p_effect->SetFloatArray("g_light_dir", light_dir, 3);
		//p_effect->SetFloatArray("g_light_diffuse", light_diffuse, 4);
		p_effect->SetFloatArray("g_light_ambient", light_ambient, 4);
		//p_effect->SetInt("g_camera_light_calc_type", camera_light_calc_type);
		//p_effect->SetInt("g_ambient_light_calc_type", ambient_light_calc_type);
	}
	if (m_p_renderer->m_fog_use && p_sprite->rp.use_fog)	{
		const int TNM_TEXTURE_STAGE_FOG = 3;
		if (p_sprite->get_texture(TNM_TEXTURE_STAGE_FOG))	{
			float fog_tex_width = (float)p_sprite->get_texture(TNM_TEXTURE_STAGE_FOG)->get_width_ex();
			float fog_tex_height = (float)p_sprite->get_texture(TNM_TEXTURE_STAGE_FOG)->get_height_ex();
			float aspect = fog_tex_height / m_default_view_port.height();

			float fog_w = m_default_view_port.width() / fog_tex_width * aspect;
			float fog_h = m_default_view_port.height() / fog_tex_height;
			float fog_x = - m_p_renderer->m_fog_x / fog_tex_width * aspect - 0.5f / m_default_view_port.width();
			float fog_y = 0.5f / m_default_view_port.height();

			float fog_param[4] = {fog_x, fog_y, fog_w, fog_h};	// x, y, w, h
			float fog_range[4] = {1.0f, m_p_renderer->m_fog_near, m_p_renderer->m_fog_far, 0.0f};	// use, near, far, (keep)
			float camera_pos[3] = {m_p_renderer->m_camera_eye.x, m_p_renderer->m_camera_eye.y, m_p_renderer->m_camera_eye.z};

			p_effect->SetFloatArray("g_fog_param", fog_param, 4);
			p_effect->SetFloatArray("g_fog_range", fog_range, 4);
			p_effect->SetFloatArray("g_camera_pos", camera_pos, 3);
		}
	}

	// エフェクトを開始する
	UINT pass_cnt = 0;
	hr = p_effect->Begin(&pass_cnt, 0);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("エフェクトの開始に失敗しました。"), _T("Begin"), hr);
		return false;
	}

	// パスを実行する
	hr = p_effect->BeginPass(0);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("エフェクトのパスの実行に失敗しました。"), _T("BeginPass"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// エフェクトを開始（３Ｄ）余計な事はしない
// ================================================================
bool C_d3d_render_interface::render_sprite_begin_effect(C_d3d_sprite* p_sprite, ID3DXEffect* p_effect)
{
	HRESULT hr;

	// エフェクトを開始する
	UINT pass_cnt = 0;
	hr = p_effect->Begin(&pass_cnt, 0);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("エフェクトの開始に失敗しました。"), _T("Begin"), hr);
		return false;
	}

	// パスを実行する
	hr = p_effect->BeginPass(0);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("エフェクトのパスの実行に失敗しました。"), _T("BeginPass"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// エフェクトを終了（３Ｄ）
// ================================================================
bool C_d3d_render_interface::render_sprite_end_effect_d3(C_d3d_sprite* p_sprite, ID3DXEffect* p_effect)
{
	HRESULT hr;

	hr = p_effect->EndPass();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("エフェクトのパスの終了に失敗しました。"), _T("EndPass"), hr);
		return false;
	}

	hr = p_effect->End();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("エフェクトの終了に失敗しました。"), _T("End"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// スプライト：ポリゴンをレンダー
// ================================================================
bool C_d3d_render_interface::render_sprite_polygon_d2(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, int vertex_cnt, int primitive_cnt)
{
	HRESULT hr;

	if (!p_sprite->get_vertex_buffer() || !p_sprite->get_index_buffer())
		return true;	// エラーではない

#if 1	// 最新バージョン（インデックスバッファを拡張したバージョン）

	// エフェクトを開始する
	ID3DXEffect* p_effect = NULL;
	if (!render_sprite_begin_effect_d2(p_sprite, rp, &p_effect))
		return false;

	// 頂点をストリームに流し込む
	// SetStreamSource の第３引数は０以外を指定してはいけません。GeForce 7300 GS で動きません。（６Ｆスタジオマシンで確認）
	// プログラムのミスなのかぐらぼの仕様なのかバグなのか分かりません。
	G_d3d.device->SetStreamSource(0, p_sprite->get_vertex_buffer()->body(), 0, p_sprite->get_FVF_size());
	G_d3d.device->SetIndices(p_sprite->get_index_buffer()->body());		// インデックスバッファの設定
	G_d3d.device->SetFVF(p_sprite->get_FVF());							// FVF の設定

	// 描画を行う
	// DrawIndexedPrimitive の第２引数に０以外を指定してはいけません。Intel GMA 4500MHD で動きません。（吉田さん家庭用ＰＣで確認）
	// プログラムのミスなのかぐらぼの仕様なのかバグなのか分かりません。
	hr = G_d3d.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, p_sprite->get_vertex_offset(), vertex_cnt, p_sprite->get_index_offset(), primitive_cnt);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("DrawIndexedPrimitive"), hr);
		return false;
	}

	// エフェクトを終了する
	if (!render_sprite_end_effect_d2(p_sprite, p_effect))
		return false;

#elif 0	// Intel GMA 4500MHD で動かなかったバージョン（吉田さん家庭用ＰＣで確認）

	// 頂点をストリームに流し込む
	Gd3d.device->SetStreamSource(0, p_sprite->get_vertex_buffer()->body(), 0, p_sprite->get_FVF_size());
	Gd3d.device->SetIndices(p_sprite->get_index_buffer()->body());	// インデックスバッファの設定
	Gd3d.device->SetFVF(p_sprite->get_FVF());						// FVF の設定

	// エフェクトを開始する
	ID3DXEffect* p_effect = NULL;
	if (!render_sprite_begin_effect_d2(p_sprite, rp, &p_effect))
		return false;

	// 描画を行う
	hr = Gd3d.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, p_sprite->get_vertex_offset(), 0, p_sprite->get_vertex_count(), 0, p_sprite->get_index_count() / 3);
	if (FAILED(hr))
		return set_my_last_error_d3d(_T("頂点の描画に失敗しました。"), hr);

	// エフェクトを終了する
	if (!render_sprite_end_effect_d2(p_sprite, p_effect))
		return false;

#else	// GeForce 7300 GS で動かなかったバージョン（６Ｆスタジオマシンで確認）

	// 頂点をストリームに流し込む
	Gd3d.device->SetStreamSource(0, p_sprite->get_vertex_buffer()->body(), p_sprite->get_vertex_offset() * p_sprite->get_FVF_size(), p_sprite->get_FVF_size());
	Gd3d.device->SetIndices(p_sprite->get_index_buffer()->body());	// インデックスバッファの設定
	Gd3d.device->SetFVF(p_sprite->get_FVF());						// FVF の設定

	// エフェクトを開始する
	ID3DXEffect* p_effect = NULL;
	if (!render_sprite_begin_effect_d2(p_sprite, rp, &p_effect))
		return false;

	// 描画を行う
	hr = Gd3d.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, p_sprite->get_vertex_count(), 0, p_sprite->get_index_count() / 3);
	if (FAILED(hr))
		return set_my_last_error_d3d(_T("頂点の描画に失敗しました。"), hr);

	// エフェクトを終了する
	if (!render_sprite_end_effect_d2(p_sprite, p_effect))
		return false;

#endif

	return true;
}

// ****************************************************************
// スプライト：ポリゴンをレンダー（３Ｄ）
// ================================================================
bool C_d3d_render_interface::render_sprite_polygon_d3(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, int vertex_cnt, int primitive_cnt)
{
	HRESULT hr;

	// 頂点をストリームに流し込む
	// SetStreamSource の第３引数は０以外を指定してはいけません。GeForce 7300 GS で動きません。（６Ｆスタジオマシンで確認）
	// プログラムのミスなのかぐらぼの仕様なのかバグなのか分かりません。
	if (p_sprite && p_sprite->get_vertex_buffer())	{
		G_d3d.device->SetStreamSource(0, p_sprite->get_vertex_buffer()->body(), 0, p_sprite->get_FVF_size());
		G_d3d.device->SetIndices(p_sprite->get_index_buffer()->body());		// インデックスバッファの設定
		G_d3d.device->SetFVF(p_sprite->get_FVF());							// FVF の設定

		// エフェクトを開始する
		ID3DXEffect* p_effect = NULL;
		if (!render_sprite_begin_effect_d3(p_sprite, rp, &p_effect))
			return false;

		// 描画を行う
		// DrawIndexedPrimitive の第２引数に０以外を指定してはいけません。Intel GMA 4500MHD で動きません。（吉田さん家庭用ＰＣで確認）
		// プログラムのミスなのかぐらぼの仕様なのかバグなのか分かりません。
		hr = G_d3d.device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, p_sprite->get_vertex_offset(), vertex_cnt, p_sprite->get_index_offset(), primitive_cnt);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("DrawIndexedPrimitive"), hr);
			return false;
		}

		// エフェクトを終了する
		if (!render_sprite_end_effect_d3(p_sprite, p_effect))
			return false;
	}

	return true;
}

// ****************************************************************
// スプライト：メッシュをレンダー
// ================================================================
bool C_d3d_render_interface::render_sprite_mesh(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, C_d3d_mesh* mesh)
{
	// マテリアルを全て描画する

	// アルファブレンドを行うものと行わないものに分ける
	std::vector<int> alpha_list, no_alpha_list;
	for (int m = 0; m < mesh->get_mtrl_cnt(); m++)	{
		// メッシュの表示フラグが立っている場合
		if (mesh->get_mtrl(m)->disp)	{
			if (mesh->get_mtrl(m)->alpha_blend)		alpha_list.push_back(m);
			else									no_alpha_list.push_back(m);
		}
	}

	// アルファブレンドを行わないものを描画
	G_d3d.state.set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);
	for (int i = 0; i < (int)no_alpha_list.size(); i++)	{
		int mtrl_no = no_alpha_list[i];

		// マテリアルを描画
		if (!render_sprite_mesh_mtrl(p_sprite, rp, mesh, mtrl_no))
			return false;
	}

	// アルファブレンドを行うものを描画
	G_d3d.state.set_render_state(D3DRS_ALPHABLENDENABLE, TRUE);
	for (int i = 0; i < (int)alpha_list.size(); i++)	{
		int mtrl_no = alpha_list[i];

		// マテリアルを描画
		if (!render_sprite_mesh_mtrl(p_sprite, rp, mesh, mtrl_no))
			return false;
	}

	return true;
}

// ****************************************************************
// スプライト：メッシュのマテリアルをレンダー
// ================================================================
bool C_d3d_render_interface::render_sprite_mesh_mtrl(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, C_d3d_mesh* mesh, int mtrl_no)
{
	C_d3d_mtrl* mtrl = mesh->get_mtrl(mtrl_no);
	if (!mtrl)
		return false;

	// エフェクトを開始する
	ID3DXEffect* p_effect = NULL;
	if (!render_sprite_begin_effect_d3_mesh(p_sprite, rp, mtrl, &p_effect))
		return false;

	// このマテリアルのメッシュを描画
	(*mesh)->DrawSubset(mtrl_no);

	// エフェクトを終了する
	if (!render_sprite_end_effect_d3(p_sprite, p_effect))
		return false;

	return true;
}



// ****************************************************************
// スプライト：フレームのメッシュサブセットを描画
// ================================================================
bool C_d3d_render_interface::render_sprite_frame_mesh_subset(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no)
{
	//HRESULT hr;

	// 使いやすいように一時変数に代入
	BSP<C_d3d_mesh>					mesh = p_frame->m_mesh_list[mesh_no];
	S_d3d_skinned_mesh_container*	p_mc = (S_d3d_skinned_mesh_container *)p_frame->body()->pMeshContainer;
	C_d3d_mtrl*						mtrl = mesh->get_mtrl(mtrl_no);
	
	// マテリアルがない場合(エラー処理行う？)
	if (!mtrl)
		return false;

	if (!render_sprite_mesh_mtrl(p_sprite, p_rp, mesh.get(), mtrl_no))
		return false;

	/*
	// ここらは本来パラメータから決めるべき
	// 固定パイプラインで頂点ブレンディングを行わない
	G_d3d.state.set_render_state(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	G_d3d.state.set_render_state(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
	// Zバッファを有効にする
	G_d3d.state.set_render_state(D3DRS_ZENABLE, TRUE);
	G_d3d.state.set_render_state(D3DRS_ZWRITEENABLE, TRUE);
	// アルファブレンディングを無効にする
	G_d3d.state.set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);
	// 反時計回りにカリングする
	G_d3d.state.set_render_state(D3DRS_CULLMODE, D3DCULL_CCW);
	*/

	// エフェクトを生成もしくは取得する
	//ID3DXEffect* p_effect = render_sprite_frame_per_mesh_create_effect(mesh.get(), mtrl, p_mc->pSkinInfo);
	//if (!p_effect)
	//	return false;

	// エフェクトの定数を設定する
	//if (!render_sprite_frame_per_mesh_set_effect_constant(p_sprite, p_rp, p_frame, mesh_no, mtrl_no, subset_no, p_effect, p_mc->pSkinInfo))
	//	return false;

#if 0
	DWORD fvf = (*mesh)->GetFVF();
	D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
	(*mesh)->GetDeclaration(pDecl);

	OutputDebugStringA("***");
	OutputDebugStringA(p_mc->Name);
	OutputDebugStringA("***\n");
#endif

	/*

	// エフェクトを開始する
	UINT pass_max = 0;
	hr = p_effect->Begin(&pass_max, 0);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("エフェクトの開始に失敗しました。"), _T("Begin"), hr);
		return false;
	}

	// パスの回数だけ回す
	for (UINT pass = 0; pass < pass_max; pass++) {
		// パスを開始する
		hr = p_effect->BeginPass(pass);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("エフェクトのパスの実行に失敗しました。"), _T("BeginPass"), hr);
			return false;
		}

		//D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];;
		//(*mesh)->GetDeclaration(pDecl);
		//LPDIRECT3DVERTEXDECLARATION9	m_pDecl;// 頂点宣言
		//G_d3d.device.body()->CreateVertexDeclaration(pDecl, &m_pDecl);
		//G_d3d.device.body()->SetVertexDeclaration(m_pDecl);

		// このマテリアルのメッシュを描画
		hr = (*mesh)->DrawSubset(subset_no);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("DrawSubset"), hr);
			return false;
		}

		// パスを終了する
		hr = p_effect->EndPass();
		if (FAILED(hr))	{
			set_last_error_d3d(_T("エフェクトのパスの終了に失敗しました。"), _T("EndPass"), hr);
			return false;
		}
	}

	// エフェクトを終了する
	hr = p_effect->End();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("エフェクトの終了に失敗しました。"), _T("End"), hr);
		return false;
	}
	*/

	return true;
}

bool C_d3d_render_interface::render_sprite_frame_per_mesh_set_effect_constant_common(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect)
{
	HRESULT hr;

	// 使いやすいように一時変数に代入
	BSP<C_d3d_mesh>					mesh = p_frame->m_mesh_list[mesh_no];
	S_d3d_skinned_mesh_container*	p_mc = (S_d3d_skinned_mesh_container *)p_frame->body()->pMeshContainer;
	C_d3d_mtrl*						mtrl = mesh->get_mtrl(mtrl_no);
	S_d3d_mesh_param*				p_mp = mesh->get_mesh_param_ptr();
	DWORD							fvf = mesh->body()->GetFVF();

	// テクニックの設定
	hr = p_effect->SetTechnique("tech");
	if(FAILED(hr)) {
		set_last_error_d3d(_T("エフェクトテクニックの設定に失敗しました。"), _T("SetTechnique"), hr);
		return false;
	}

	// ビュー変換行列の設定
	hr = p_effect->SetMatrix("mView", m_p_renderer->get_view_matrix());
	if(FAILED(hr)) {
		set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetMatrix"), hr);
		return false;
	}

	// 射影変換行列の設定
	hr = p_effect->SetMatrix("mProj", m_p_renderer->get_proj_matrix());
	if(FAILED(hr)) {
		set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetMatrix"), hr);
		return false;
	}

	// カメラの設定
	{
		// ワールドでの座標(基底クラスのままで問題なし)
		hr = p_effect->SetVector("g_CameraWorldPos", &m_p_renderer->get_camera()->get_camera_world_pos());
		if(FAILED(hr)) {
			set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVector"), hr);
			return false;
		}
		// ローカルでの座標(基底クラスのままで問題なし)
		hr = p_effect->SetVector("g_CameraLocalPos", &m_p_renderer->get_camera()->get_camera_local_pos(p_frame->body()->CombinedTransformationMatrix));
		if(FAILED(hr)) {
			set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVector"), hr);
			return false;
		}
	}

	// ライトの設定
	{
		/*
		// ディレクショナル
		{
			ARRAY<D3DXVECTOR4> w_dir;
			ARRAY<D3DXVECTOR4> l_dir;
			ARRAY<D3DXVECTOR4> diffuse;
			ARRAY<D3DXVECTOR4> ambient;
			ARRAY<D3DXVECTOR4> specular;

			int light_num = 0;
			for(int i = 0; i < (int)p_mp->directional_light_id_list.size(); i++) {
				// 設定可能数を越えたらループを抜ける
				if(i >= C_d3d_light_manager::OBJ_DIRECTIONAL_LIGHT_MAX)
					break;
				C_d3d_directional_light* p_light = dynamic_cast<C_d3d_directional_light*>(G_d3d_light_manager.search(p_mp->directional_light_id_list[i]).get());

				// ライトのデータを追加していく
				w_dir.push_back(p_light->get_light_world_dir());
				l_dir.push_back(p_light->get_light_local_dir(p_frame->body()->CombinedTransformationMatrix));

				diffuse.push_back(p_light->get_diffuse());
				ambient.push_back(p_light->get_ambient());
				specular.push_back(p_light->get_specular());

				// ライトの数をカウント
				light_num++;
			}

			if(light_num > 0) {
				// 実際のライトの数
				hr = p_effect->SetInt("g_DLightNum", light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetInt"), hr);
					return false;
				}
				// ワールドでの方向ベクトル
				hr = p_effect->SetVectorArray("g_DLightWorldDir", w_dir.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}
				// ローカルでの方向ベクトル
				hr = p_effect->SetVectorArray("g_DLightLocalDir", l_dir.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}

				// ディフューズ
				hr = p_effect->SetVectorArray("g_DLightDiffuse", diffuse.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}
				// アンビエント
				hr = p_effect->SetVectorArray("g_DLightAmbient", ambient.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}
				// スペキュラ
				hr = p_effect->SetVectorArray("g_DLightSpecular", specular.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}
			}
		}




		// ポイント
		{
			ARRAY<D3DXVECTOR4> w_pos;
			ARRAY<D3DXVECTOR4> l_pos;
			ARRAY<D3DXVECTOR4> atten012_range;
			ARRAY<D3DXVECTOR4> diffuse;
			ARRAY<D3DXVECTOR4> ambient;
			ARRAY<D3DXVECTOR4> specular;

			int light_num = 0;
			for(int i = 0; i < (int)p_mp->point_light_id_list.size(); i++) {
				// 設定可能数を越えたらループを抜ける
				if(i >= C_d3d_light_manager::OBJ_POINT_LIGHT_MAX)
					break;
				C_d3d_point_light* p_light = dynamic_cast<C_d3d_point_light*>(G_d3d_light_manager.search(p_mp->point_light_id_list[i]).get());

				// ライトのデータを追加していく
				w_pos.push_back(p_light->get_light_world_pos());
				l_pos.push_back(p_light->get_light_local_pos(p_frame->body()->CombinedTransformationMatrix,  *m_p_renderer->get_view_matrix()));
				atten012_range.push_back(p_light->get_light_attenuations_range());

				diffuse.push_back(p_light->get_diffuse());
				ambient.push_back(p_light->get_ambient());
				specular.push_back(p_light->get_specular());

				// ライトの数をカウント
				light_num++;
			}


			if(light_num > 0) {
				// 実際のライトの数
				hr = p_effect->SetInt("g_PLightNum", light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetInt"), hr);
					return false;
				}				
				// ワールドでの座標
				hr = p_effect->SetVectorArray("g_PLightWorldPos", w_pos.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}
				// ローカルでの座標
				hr = p_effect->SetVectorArray("g_PLightLocalPos", l_pos.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}
				// 距離減衰パラメータ
				hr = p_effect->SetVectorArray("g_PLightAttenuationsRange", atten012_range.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}

				// ディフューズ
				hr = p_effect->SetVectorArray("g_PLightDiffuse", diffuse.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}
				// アンビエント
				hr = p_effect->SetVectorArray("g_PLightAmbient", ambient.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}
				// スペキュラ
				hr = p_effect->SetVectorArray("g_PLightSpecular", specular.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}
			}
		}



		// スポット
		{
			ARRAY<D3DXVECTOR4> w_pos;
			ARRAY<D3DXVECTOR4> l_pos;
			ARRAY<D3DXVECTOR4> w_dir;
			ARRAY<D3DXVECTOR4> l_dir;
			ARRAY<D3DXVECTOR4> atten012_range;
			ARRAY<D3DXVECTOR4> cos_theta_phi_falloff;
			ARRAY<D3DXVECTOR4> diffuse;
			ARRAY<D3DXVECTOR4> ambient;
			ARRAY<D3DXVECTOR4> specular;

			int light_num = 0;
			for(int i = 0; i < (int)p_mp->spot_light_id_list.size(); i++) {
				// 設定可能数を越えたらループを抜ける
				if(i >= C_d3d_light_manager::OBJ_SPOT_LIGHT_MAX)
					break;
				C_d3d_spot_light* p_light = dynamic_cast<C_d3d_spot_light*>(G_d3d_light_manager.search(p_mp->spot_light_id_list[i]).get());

				// ライトのデータを追加していく
				w_pos.push_back(p_light->get_light_world_pos());
				l_pos.push_back(p_light->get_light_local_pos(p_frame->body()->CombinedTransformationMatrix,  *m_p_renderer->get_view_matrix()));
				w_dir.push_back(p_light->get_light_world_dir());
				l_dir.push_back(p_light->get_light_local_dir(p_frame->body()->CombinedTransformationMatrix));
				atten012_range.push_back(p_light->get_light_attenuations_range());
				cos_theta_phi_falloff.push_back(p_light->get_light_cos_theta_phi_falloff());	// cos(theta/2),cos(phi/2)を計算して渡す

				diffuse.push_back(p_light->get_diffuse());
				ambient.push_back(p_light->get_ambient());
				specular.push_back(p_light->get_specular());

				// ライトの数をカウント
				light_num++;
			}

			if(light_num > 0) {
				// 実際のライトの数
				hr = p_effect->SetInt("g_SLightNum", light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetInt"), hr);
					return false;
				}
				// ワールドでの座標
				hr = p_effect->SetVectorArray("g_SLightWorldPos", w_pos.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}
				// ローカルでの座標
				hr = p_effect->SetVectorArray("g_SLightLocalPos", l_pos.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}
				// ワールドでの方向ベクトル
				hr = p_effect->SetVectorArray("g_SLightWorldDir", w_dir.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}
				// ローカルでの方向ベクトル
				hr = p_effect->SetVectorArray("g_SLightLocalDir", l_dir.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}
				// 距離減衰パラメータ
				hr = p_effect->SetVectorArray("g_SLightAttenuationsRange", atten012_range.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}
				// スポットライトパラメータ(thetaとphiは予め割ったものを渡す)
				hr = p_effect->SetVectorArray("g_SLightThetaPhiFalloff", cos_theta_phi_falloff.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}

				// ディフューズ
				hr = p_effect->SetVectorArray("g_SLightDiffuse", diffuse.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}
				// アンビエント
				hr = p_effect->SetVectorArray("g_SLightAmbient", ambient.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}
				// スペキュラ
				hr = p_effect->SetVectorArray("g_SLightSpecular", specular.get(), light_num);
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVectorArray"), hr);
					return false;
				}
			}
		}




		*/



		// 従来のライティング
		ARRAY< BSP<C_d3d_light_interface> >::iterator it = m_p_renderer->get_light_list()->begin();
		for(; it != m_p_renderer->get_light_list()->end(); ++it) {
			// 種類の判定
			LIGHT_TYPE light_type = it->get()->get_type();

			if(false);
			// ディレクショナルライト(並行光源)の場合
			else if(light_type == LIGHT_TYPE_DIRECTIONAL) {
				C_d3d_directional_light* light = (C_d3d_directional_light*)(it->get());

				// ワールドでの方向ベクトル
				hr = p_effect->SetVector("g_LightWorldDir", &light->get_light_world_dir());
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVector"), hr);
					return false;
				}
				// ローカルでの方向ベクトル
				hr = p_effect->SetVector("g_LightLocalDir", &light->get_light_local_dir(p_frame->body()->CombinedTransformationMatrix));
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVector"), hr);
					return false;
				}
			}
			// ポイントライト(点光源)の場合
			else if(light_type == LIGHT_TYPE_POINT) {
			}
			// スポットライトの場合
			else if(light_type == LIGHT_TYPE_SPOT) {
			}
		}
	}



	// テクスチャの設定
	if(p_mp->use_mesh_tex && mtrl->texture) {
		hr = p_effect->SetTexture("meshtexture", mtrl->texture->body());
		if(FAILED(hr)) {
			set_last_error_d3d(_T("エフェクトのテクスチャの設定に失敗しました。"), _T("SetTexture"), hr);
			return false;
		}
	}

	// マテリアルの設定
	{
		D3DXVECTOR4 v;

		// ディフューズの設定
		v = D3DXVECTOR4(mtrl->mtrl.Diffuse.r, mtrl->mtrl.Diffuse.g, mtrl->mtrl.Diffuse.b, mtrl->mtrl.Diffuse.a);
		hr = p_effect->SetVector("g_MaterialDiffuse", &v);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVector"), hr);
			return false;
		}
		// アンビエントの設定
		v = D3DXVECTOR4(mtrl->mtrl.Ambient.r, mtrl->mtrl.Ambient.g, mtrl->mtrl.Ambient.b, mtrl->mtrl.Ambient.a);
		hr = p_effect->SetVector("g_MaterialAmbient", &v);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVector"), hr);
			return false;
		}
		// スペキュラの設定
		v = D3DXVECTOR4(mtrl->mtrl.Specular.r, mtrl->mtrl.Specular.g, mtrl->mtrl.Specular.b, mtrl->mtrl.Specular.a);
		hr = p_effect->SetVector("g_MaterialSpecular", &v);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVector"), hr);
			return false;
		}
		// エミッシブの設定
		v = D3DXVECTOR4(mtrl->mtrl.Emissive.r, mtrl->mtrl.Emissive.g, mtrl->mtrl.Emissive.b, mtrl->mtrl.Emissive.a);
		hr = p_effect->SetVector("g_MaterialEmissive", &v);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVector"), hr);
			return false;
		}
		// パワーの設定(スペキュラ)
		hr = p_effect->SetFloat("g_MaterialSpecularPower", mtrl->mtrl.Power);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetFloat"), hr);
			return false;
		}

	}

	// Siglus用のカラー設定
	{
		D3DXVECTOR4 v;

		// 頂点カラーの乗算率の設定
		if(p_mp->use_mul_vertex_color) {
			hr = p_effect->SetFloat("g_VertexColorRate", p_mp->mul_vertex_color_rate);
			if(FAILED(hr)) {
				set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetFloat"), hr);
				return false;
			}			
		}
		// MRBD
		if(p_mp->use_mrbd) {
			hr = p_effect->SetVector("g_C0", &p_mp->mrbd);
			if(FAILED(hr)) {
				set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVector"), hr);
				return false;
			}
		}
		// RGBRate, AddRGB
		if(p_mp->use_rgb) {
			hr = p_effect->SetVector("g_C1", &p_mp->rgb_rate);
			if(FAILED(hr)) {
				set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVector"), hr);
				return false;
			}
			hr = p_effect->SetVector("g_C2", &p_mp->add_rgb);
			if(FAILED(hr)) {
				set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVector"), hr);
				return false;
			}
		}
	}

	// シャドウマップ関連
	{
		if(p_mp->shading_type == SHADING_TYPE_DEPTH_BUFFER) {
			// シャドウマップの設定
			if(m_p_renderer->get_shadow_map_tex()) {
				hr = p_effect->SetTexture("shadowtexture", m_p_renderer->get_shadow_map_tex());
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトのテクスチャの設定に失敗しました。"), _T("SetTexture"), hr);
					return false;
				}
			}

			// 深度バッファシャドウのバイアス値の設定
			hr = p_effect->SetFloat("g_DBSBias", p_mp->depth_buffer_shadow_bias);
			if(FAILED(hr)) {
				set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetFloat"), hr);
				return false;
			}			
			// ライトから見た射影空間への変換行列
			D3DXMATRIX light_view_proj = (*m_p_renderer->get_light_view_matrix()) * (*m_p_renderer->get_light_proj_matrix());
			hr = p_effect->SetMatrix("mLightVP", &light_view_proj);
			if(FAILED(hr)) {
				set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetMatrix"), hr);
				return false;
			}
			// バイアス行列
			hr = p_effect->SetMatrix("mScaleBias", m_p_renderer->get_scale_bias_matrix());
			if(FAILED(hr)) {
				set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetMatrix"), hr);
				return false;
			}
		}
	}

	// トゥーン関連
	{
		if(p_mp->lighting_type == LIGHTING_TYPE_TOON) {
			// トゥーンテクスチャの設定
			if(p_mp->toon_tex && p_mp->toon_tex->body()) {
				hr = p_effect->SetTexture("toontexture", p_mp->toon_tex->body());
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトのテクスチャの設定に失敗しました。"), _T("SetTexture"), hr);
					return false;
				}
			}
		}
	}

	// バンプ、視差関連
	{
		if(p_mp->lighting_type == LIGHTING_TYPE_BUMP || p_mp->lighting_type == LIGHTING_TYPE_PARALLAX) {
			// 法線マップの設定
			if(p_mp->normal_tex && p_mp->normal_tex->body()) {
				hr = p_effect->SetTexture("normaltexture", p_mp->normal_tex->body());
				if(FAILED(hr)) {
					set_last_error_d3d(_T("エフェクトのテクスチャの設定に失敗しました。"), _T("SetTexture"), hr);
					return false;
				}
			}
		}
	}

	// リムライト関連
	{
		if(p_mp->shader_option & SHADER_OPTION_RIM_LIGHT) {
			// エミッシブの設定
			hr = p_effect->SetVector("g_RimLightColor", &p_mp->rim_light_color);
			if(FAILED(hr)) {
				set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetVector"), hr);
				return false;
			}
			// パワーの設定(スペキュラ)
			hr = p_effect->SetFloat("g_RimLightPower", p_mp->rim_light_power);
			if(FAILED(hr)) {
				set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetFloat"), hr);
				return false;
			}
		}
	}



	//if(g_UseGauss)	p_effect->SetTexture("shadowtexture", g_pXYMap);
	//else			p_effect->SetTexture("shadowtexture", g_pShadowTex);

	//p_effect->SetMatrix("mLightVP", &g_MatLightVP);
	//p_effect->SetMatrix("mLightWVP", &(g_MatLightVP * mScaleBias));
	//p_effect->SetMatrix("mScaleBias", &mScaleBias);	

	//D3DXVECTOR4 cp;
	//cp.x = g_Camera.m_Eye.x;
	//cp.y = g_Camera.m_Eye.y;
	//cp.z = g_Camera.m_Eye.z;
	//cp.w = 1.0f;
	//g_pEffect->SetVector("g_CameraLocalPos", &CalcCameraLocalPos(frame->CombinedTransformationMatrix, g_MatView));
	//p_effect->SetVector("g_CameraLocalPos", &cp);
	//p_effect->SetVector("g_CameraWorldPos", &cp);
	
	//D3DXVECTOR4 ld;
	//ld.x = g_Light.Direction.x;
	//ld.y = g_Light.Direction.y;
	//ld.z = g_Light.Direction.z;
	//ld.w = 1.0f;
	//g_pEffect->SetVector("g_LightLocalDir", &D3DXVECTOR4(0,0,1,1));
	//g_pEffect->SetVector("g_LightLocalDir", &CalcLightLocalDir(frame->CombinedTransformationMatrix, ld));
	//p_effect->SetVector("g_LightLocalDir", &ld);
	//p_effect->SetVector("g_LightWorldDir", &ld);

	//EffectSetMaterial(p_effect, (D3DMATERIAL9*)&meshContainer->pMaterials[pBoneCombination[i].AttribId]);


	return true;
}

// スキンじゃないメッシュの場合
bool C_d3d_render_interface::render_sprite_frame_per_mesh_set_effect_constant_mesh(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect)
{
	HRESULT hr;

	// 使いやすいように一時変数に代入
	BSP<C_d3d_mesh>					mesh = p_frame->m_mesh_list[mesh_no];
	S_d3d_skinned_mesh_container*	p_mc = (S_d3d_skinned_mesh_container *)p_frame->body()->pMeshContainer;
	C_d3d_mtrl*						mtrl = mesh->get_mtrl(mtrl_no);
	S_d3d_mesh_param*				p_mp = mesh->get_mesh_param_ptr();
	DWORD							fvf = mesh->body()->GetFVF();

	// ワールド変換行列配列の設定
	hr = p_effect->SetMatrixArray("mWorldMatrixArray", &p_frame->body()->CombinedTransformationMatrix, 1);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetMatrixArray"), hr);
		return false;
	}

	// ワールド変換行列の設定
	hr = p_effect->SetMatrix("mWorld", &p_frame->body()->CombinedTransformationMatrix);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetMatrix"), hr);
		return false;
	}
	// ローカル変換行列の設定(アニメーション後のローカル座標系に変換する行列)
	// TransformationMatrixじゃなくてIDENTITY_MATRIXかも？/*要修正*/
	hr = p_effect->SetMatrix("mLocal", &p_frame->body()->TransformationMatrix);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetMatrix"), hr);
		return false;
	}

	// ボーン数の設定
	hr = p_effect->SetInt("CurNumBones", 1);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetInt"), hr);
		return false;
	}

	return true;
}
// スキンメッシュの場合
bool C_d3d_render_interface::render_sprite_frame_per_mesh_set_effect_constant_skinned_mesh(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect)
{
	HRESULT hr;

	// 使いやすいように一時変数に代入
	BSP<C_d3d_mesh>					mesh = p_frame->m_mesh_list[mesh_no];
	S_d3d_skinned_mesh_container*	p_mc = (S_d3d_skinned_mesh_container *)p_frame->body()->pMeshContainer;
	C_d3d_mtrl*						mtrl = mesh->get_mtrl(mtrl_no);
	S_d3d_mesh_param*				p_mp = mesh->get_mesh_param_ptr();
	DWORD							fvf = mesh->body()->GetFVF();

	// ワールド変換行列配列の設定
	hr = p_effect->SetMatrixArray("mWorldMatrixArray", p_mc->SkinnedBlendMatrices.get(), p_mc->NumPaletteEntries);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetMatrixArray"), hr);
		return false;
	}

	// ワールド変換行列の設定
	hr = p_effect->SetMatrix("mWorld", &p_frame->body()->CombinedTransformationMatrix);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetMatrix"), hr);
		return false;
	}
	// ローカル変換行列の設定(アニメーション後のローカル座標系に変換する行列)
	hr = p_effect->SetMatrix("mLocal", &p_frame->body()->TransformationMatrix);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetMatrix"), hr);
		return false;
	}

	// ボーン数の設定
	hr = p_effect->SetInt("CurNumBones", p_mc->NumInfl);
	if(FAILED(hr)) {
		set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetInt"), hr);
		return false;
	}

	return true;
}

// エフェクト定数の設定
bool C_d3d_render_interface::render_sprite_frame_per_mesh_set_effect_constant(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect, ID3DXSkinInfo* p_skin_info)
{
	// スキンメッシュ、非スキンメッシュ共通のエフェクト定数設定
	if (!render_sprite_frame_per_mesh_set_effect_constant_common(p_sprite, p_rp, p_frame, mesh_no, mtrl_no, subset_no, p_effect))
		return false;

	// エフェクトを開始する(定数の設定した後にエフェクトを開始)
	if (!(p_skin_info
			? render_sprite_frame_per_mesh_set_effect_constant_skinned_mesh(p_sprite, p_rp, p_frame, mesh_no, mtrl_no, subset_no, p_effect)
			: render_sprite_frame_per_mesh_set_effect_constant_mesh(p_sprite, p_rp, p_frame, mesh_no, mtrl_no, subset_no, p_effect)
			))
		return false;

	return true;
}

// ****************************************************************
// スプライト：フレームをレンダー
// ================================================================
bool C_d3d_render_interface::render_sprite_frame(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, C_d3d_frame* frame)
{
	LPSTR fname = frame->body()->Name;

	// メッシュがあればメッシュを描画！(多分pNextMeshContainerを実装しないとsizeは常に1？)
	for (int mesh_no = 0; mesh_no < (int)frame->m_mesh_list.size(); mesh_no ++)	{
		BSP<C_d3d_mesh> mesh = frame->m_mesh_list[mesh_no];
		S_d3d_skinned_mesh_container* p_mc = (S_d3d_skinned_mesh_container *)frame->body()->pMeshContainer;

		// このメッシュの描画条件をチェック
		if (render_sprite_frame_per_mesh_draw_condition(mesh.get())) {

			// スキンがない場合
			if (!p_mc->pSkinInfo)	{

				for (int m = 0; m < (int)mesh->get_mtrl_cnt(); m++)	{

					// マテリアルを描画
					if (!render_sprite_frame_mesh_subset(p_sprite, rp, frame, mesh_no, m, m))
						return false;
				}
			}

			// スキンがある場合
			else	{
				ID3DXSkinInfo* skin = p_mc->pSkinInfo;
				D3DXBONECOMBINATION* p_bone_conbination = (D3DXBONECOMBINATION *)p_mc->pBoneCombinationBuf->GetBufferPointer();
				UINT matrix_index;

				for (int i = 0; i < (int)p_mc->NumAttributeGroups; i++)	{

					// C_d3d_renderer::update()に移行予定だったけどうまくいかなかったので保留
					// 変換用行列配列の算出
					for(int j = 0; j < (int)p_mc->NumPaletteEntries; j++) {
						matrix_index = p_bone_conbination[i].BoneId[j];
						if(matrix_index != UINT_MAX) {
							D3DXMatrixMultiply(&p_mc->SkinnedBlendMatrices[j],
												&frame->m_offset_matrix_list[matrix_index],
												frame->m_root_matrix_list[matrix_index]);
//							G_debug_text.printf(_T("*****%s*****\n"), MBSTR_to_TSTR(frame->body()->Name).c_str());
//							draw_matrix_data(_T("SBM"), &p_mc->SkinnedBlendMatrices[j]);
//							draw_matrix_data(_T("OFFSET"), &frame->m_offset_matrix_list[matrix_index]);
//							draw_matrix_data(_T("ROOT"), frame->m_root_matrix_list[matrix_index]);
						}
					}

					/*
					DWORD blend_cnt = 0;
					// 頂点ブレンディングの回数を求める
					for (int j = 0; j < (int)p_mc->NumInfl; j++)	{
						if (p_bone_conbination[i].BoneId[j] != UINT_MAX)
							blend_cnt = j;	// なぜか "="。サンプルもそう。うーん。
					}

					// 頂点ブレンディングの回数がスペックをオーバーしていないかどうか調べる
					if (G_d3d.device.device_caps().MaxVertexBlendMatrices <= blend_cnt)	{
						set_last_error(_T("MaxVertexBlendMatrices が足りません。スキンメッシュを描画できませんでした。"), _T("render_sprite_frame"));
						return false;
					}

					// 頂点ブレンディングの回数を設定する
					G_d3d.state.set_render_state(D3DRS_VERTEXBLEND, blend_cnt);

					// 頂点ブレンディングの行列の求めて設定する
					for (int j = 0; j < (int)p_mc->NumInfl; j++)	{
						DWORD matrix_index = p_bone_conbination[i].BoneId[j];
						if (matrix_index != UINT_MAX)	{
							D3DXMATRIX mat_temp;
							D3DXMatrixMultiply(&mat_temp, &frame->m_offset_matrix_list[matrix_index], frame->m_root_matrix_list[matrix_index]);
							G_d3d.device->SetTransform(D3DTS_WORLDMATRIX(j), &mat_temp);
						}
					}
					*/

					// この属性グループで使うマテリアルを取得
					int mtrl_no = p_bone_conbination[i].AttribId;

					// マテリアルを描画
					//if (!render_sprite_mesh_mtrl(p_sprite, rp, mesh.get(), mtrl_no, p_mc))
					if (!render_sprite_frame_mesh_subset(p_sprite, rp, frame, mesh_no, mtrl_no, i))
						return false;
				}
			}
		}
	}
	// 兄弟フレームを描画
	if (frame->m_sibling)	{
		if (!render_sprite_frame(p_sprite, rp, frame->m_sibling.get()))
			return false;
	}
	// 子フレームを描画
	if (frame->m_first_child)	{
		if (!render_sprite_frame(p_sprite, rp, frame->m_first_child.get()))
			return false;
	}



	return true;
}

// フレーム描画時のメッシュ毎の描画条件(デフォルトは常にtrue)
bool C_d3d_render_interface::render_sprite_frame_per_mesh_draw_condition(C_d3d_mesh* p_mesh)
{
	return true;
}

// メッシュ毎のエフェクト生成
ID3DXEffect* C_d3d_render_interface::render_sprite_frame_per_mesh_create_effect(C_d3d_mesh* p_mesh, C_d3d_mtrl* p_mtrl, ID3DXSkinInfo* p_skin_info)
{
	return create_effect_for_mesh_from_mesh(p_mesh, p_mtrl, p_skin_info);
}








#if 0
static float get_val_from_matrix_index(int i, D3DXMATRIX* matrix)
{
	float ret = 0.0f;

	if(false);
	else if(i == 0) ret = matrix->_11;
	else if(i == 1) ret = matrix->_12;
	else if(i == 2) ret = matrix->_13;
	else if(i == 3) ret = matrix->_14;
	else if(i == 4) ret = matrix->_21;
	else if(i == 5) ret = matrix->_22;
	else if(i == 6) ret = matrix->_23;
	else if(i == 7) ret = matrix->_24;
	else if(i == 8) ret = matrix->_31;
	else if(i == 9) ret = matrix->_32;
	else if(i == 10) ret = matrix->_33;
	else if(i == 11) ret = matrix->_34;
	else if(i == 12) ret = matrix->_41;
	else if(i == 13) ret = matrix->_42;
	else if(i == 14) ret = matrix->_43;
	else if(i == 15) ret = matrix->_44;

	return ret;
}
static void draw_matrix_data(CTSTR& matrix_name, D3DXMATRIX* matrix)
{
	G_debug_text.printf(_T("%s\n"), matrix_name.c_str());
	for(int i = 0; i < 16; i++) {
		G_debug_text.printf(_T("%.2f\t"), get_val_from_matrix_index(i, matrix));
		if((i + 1) % 4 == 0) {
			G_debug_text.printf(_T("\n"));

		}
	}
}
#endif








}