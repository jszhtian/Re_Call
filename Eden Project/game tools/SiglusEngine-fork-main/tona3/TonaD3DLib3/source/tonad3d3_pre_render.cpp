#include "tonad3d3_pch.h"
#include "tonad3d3_pre_render.h"
#include "tonad3d3_renderer.h"

namespace NT3
{


bool C_d3d_pre_render_shadow_map::render_sprite_list(ARRAY<C_d3d_sprite *>& sprite_list)
{
	ARRAY<C_d3d_sprite *>::iterator it = sprite_list.begin();
	for(; it != sprite_list.end(); ++it) {
		C_d3d_sprite* p_sprite = *it;

		// 「タイプ：フレームセット」の描画
		if (p_sprite->type == E_d3d_sprite_type_frame_set && p_sprite->get_frame_set())	{
			BSP<C_d3d_frame_set> frame_set = p_sprite->get_frame_set();

			// 実際にプリレンダを行う
			// 全てのフレームを順に描画する
			if (!render_sprite_frame(p_sprite, &p_sprite->rp, frame_set->m_frame_root.get()))
				return false;
		}
	}

	return true;
}




bool C_d3d_pre_render_shadow_map::render_sprite_frame_per_mesh_set_effect_constant(C_d3d_sprite* p_sprite, const S_d3d_render_param* p_rp, C_d3d_frame* p_frame, int mesh_no, int mtrl_no, int subset_no, ID3DXEffect* p_effect, ID3DXSkinInfo* p_skin_info)
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

	// スキンメッシュの場合
	if(p_mc->pSkinInfo) {
		// ワールド変換行列配列の設定
		hr = p_effect->SetMatrixArray("mWorldMatrixArray", p_mc->SkinnedBlendMatrices.get(), p_mc->NumPaletteEntries);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetMatrixArray"), hr);
			return false;
		}
		// ボーン数の設定
		hr = p_effect->SetInt("CurNumBones", p_mc->NumInfl);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetInt"), hr);
			return false;
		}
	}
	// 非スキンメッシュの場合
	else {
		// ワールド変換行列配列の設定
		hr = p_effect->SetMatrixArray("mWorldMatrixArray", &p_frame->body()->CombinedTransformationMatrix, 1);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetMatrixArray"), hr);
			return false;
		}
		// ボーン数の設定
		hr = p_effect->SetInt("CurNumBones", 1);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetInt"), hr);
			return false;
		}
	}

	// シャドウマップ関連
	{
		// ライトから見た射影空間への変換行列
		D3DXMATRIX light_view_proj = (*m_p_renderer->get_light_view_matrix()) * (*m_p_renderer->get_light_proj_matrix());
		hr = p_effect->SetMatrix("mLightVP", &light_view_proj);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("エフェクトの定数の設定に失敗しました。"), _T("SetMatrix"), hr);
			return false;
		}
	}


	return true;
}


// フレーム描画時のメッシュ毎の描画条件(プリレンダタイプがシャドウマップなら)
bool C_d3d_pre_render_shadow_map::render_sprite_frame_per_mesh_draw_condition(C_d3d_mesh* p_mesh)
{
	S_d3d_mesh_param* p_mp = p_mesh->get_mesh_param_ptr();

	if(p_mp->pre_render_type & S_d3d_mesh_param::SHADOW_MAP) {
		return true;
	}

	return false;
}

// メッシュ毎のエフェクト生成
ID3DXEffect* C_d3d_pre_render_shadow_map::render_sprite_frame_per_mesh_create_effect(C_d3d_mesh* p_mesh, C_d3d_mtrl* p_mtrl, ID3DXSkinInfo* p_skin_info)
{
	return create_effect_for_shadow_map_from_mesh(p_mesh, p_mtrl, p_skin_info);
}












}