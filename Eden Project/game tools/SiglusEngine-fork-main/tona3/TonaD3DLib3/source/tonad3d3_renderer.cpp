#include	"tonad3d3_pch.h"
#include	"tonad3d3_state.h"
#include	"tonad3d3_renderer.h"


namespace NT3
{


// ****************************************************************
// D3D ビュー初期化
// ================================================================
bool C_d3d_renderer::init()
{
	// 行列を初期化
	D3DXMatrixIdentity(&m_mat_view);
	D3DXMatrixIdentity(&m_mat_proj);

	m_shadow_map_tex = NULL;
	m_shadow_map_surface = NULL;
	m_shadow_map_depth_stencil_buffer = NULL;

	m_default_depth_stencil_buffer = NULL;
	m_default_back_buffer = NULL;

	m_light_bloom_tex = NULL;
	m_light_bloom_surface = NULL;

	m_fog_near = 0.0f;
	m_fog_far = 0.0f;

	m_render_default.initialize(this);
	m_pre_render_shadow_map.initialize(this);

	return true;
}

// ****************************************************************
// バックバッファを取得
// ================================================================
C_d3d_surface C_d3d_renderer::get_back_buffer()
{
	HRESULT hr;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("デバイスが準備されていません。"), _T("get_back_buffer"));
		return false;
	}

	// バックバッファのサーフェスを取得する
	IDirect3DSurface9* p_back_buffer = NULL;
	hr = G_d3d.device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &p_back_buffer);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("GetBackBuffer"), hr);
		return C_d3d_surface();
	}

	// GetBackBuffer は内部で参照カウンタを上げています。BIP で管理しないとリークしてしまいます。
	return BIP<IDirect3DSurface9>(p_back_buffer, false);
}

// ****************************************************************
// レンダーターゲットを設定
// ================================================================
bool C_d3d_renderer::set_render_target(IDirect3DSurface9* surface)
{
	HRESULT hr;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("デバイスが準備されていません。"), _T("set_render_target"));
		return false;
	}

	// レンダーターゲットを設定
	hr = G_d3d.device->SetRenderTarget(0, surface);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("SetRenderTarget"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// 深度ステンシルバッファを設定
// ================================================================
bool C_d3d_renderer::set_dpst_buffer(IDirect3DSurface9* surface)
{
	HRESULT hr;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("デバイスが準備されていません。"), _T("set_dpst_buffer"));
		return false;
	}

	// 深度視ステンシルバッファを設定
	hr = G_d3d.device->SetDepthStencilSurface(surface);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("SetDepthStencilSurface"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// 深度ステンシルバッファを取得
// ================================================================
C_d3d_surface C_d3d_renderer::get_dpst_buffer()
{
	HRESULT hr;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("デバイスが準備されていません。"), _T("get_dpst_buffer"));
		return false;
	}

	// 深度ステンシルバッファのサーフェスを取得しておく
	IDirect3DSurface9* p_dpst_buffer = NULL;
	hr = G_d3d.device->GetDepthStencilSurface(&p_dpst_buffer);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("GetDepthStencilSurface"), hr);
		return C_d3d_surface();
	}

	// GetDepthStencilSurface は内部で参照カウンタを上げています。BIP で管理しないとリークしてしまいます。
	return BIP<IDirect3DSurface9>(p_dpst_buffer, false);
}

// ****************************************************************
// レンダーターゲットをクリア
// ================================================================
bool C_d3d_renderer::clear_render_target(D3DCOLOR back_color)
{
	HRESULT hr;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("デバイスが準備されていません。"), _T("clear_render_target"));
		return false;
	}

	// バックバッファをクリア
	DWORD clear_target = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL;
	hr = G_d3d.device->Clear(0, NULL, clear_target, back_color, 1.0f, 0);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("Clear"), hr);
		return false;
	}

	return true;
}

// バッファのみ
bool C_d3d_renderer::clear_render_target_buffer(D3DCOLOR back_color)
{
	HRESULT hr;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("デバイスが準備されていません。"), _T("clear_render_target_buffer"));
		return false;
	}

	// バックバッファをクリア
	DWORD clear_target = D3DCLEAR_TARGET;
	hr = G_d3d.device->Clear(0, NULL, clear_target, back_color, 1.0f, 0);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("Clear"), hr);
		return false;
	}

	return true;
}

// Ｚバッファのみ
bool C_d3d_renderer::clear_render_target_z_buffer()
{
	HRESULT hr;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("デバイスが準備されていません。"), _T("clear_render_target_z_buffer"));
		return false;
	}

	// バックバッファをクリア
	DWORD clear_target = D3DCLEAR_ZBUFFER;
	hr = G_d3d.device->Clear(0, NULL, clear_target, 0, 1.0f, 0);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("Clear"), hr);
		return false;
	}

	return true;
}

// ステンシルバッファのみ
bool C_d3d_renderer::clear_render_target_stencil()
{
	HRESULT hr;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("デバイスが準備されていません。"), _T("clear_render_target_stencil"));
		return false;
	}

	// バックバッファをクリア
	DWORD clear_target = D3DCLEAR_STENCIL;
	hr = G_d3d.device->Clear(0, NULL, clear_target, 0, 1.0f, 0);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("Clear"), hr);
		return false;
	}

	return true;
}

// ****************************************************************
// アップデート
// ================================================================
bool C_d3d_renderer::update()
{
	ARRAY<C_d3d_sprite *>::iterator it = sprite_list.begin();
	for(; it != sprite_list.end(); ++it) {
		C_d3d_sprite* p_sprite = *it;

		// 「タイプ：フレームセット」の描画
		if (p_sprite->type == E_d3d_sprite_type_frame_set && p_sprite->get_frame_set())	{
			BSP<C_d3d_frame_set> frame_set = p_sprite->get_frame_set();

			// ワールド変換行列を計算
			D3DXMATRIX mat_world, mat_temp;

			// ワールド変換行列を設定
			float yaw = p_sprite->rp.rotate.y;
			float pitch = p_sprite->rp.rotate.x;
			float roll = p_sprite->rp.rotate.z;

			// ワールド座標変換行列を作成(S-R-T)
			D3DXMatrixIdentity(&mat_world);
			D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixScaling(&mat_temp, p_sprite->rp.scale.x, p_sprite->rp.scale.y, p_sprite->rp.scale.z));		// 拡縮
			D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixRotationQuaternion(&mat_temp, &p_sprite->rp.quaternion));									// 回転
			//D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixRotationYawPitchRoll(&mat_temp, yaw, pitch, roll));										// 回転
			D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixTranslation(&mat_temp, p_sprite->rp.pos.x, p_sprite->rp.pos.y, p_sprite->rp.pos.z));		// 移動

			//// （頂点シェーダを使わない場合）ワールド変換行列を設定
			//Gd3d.device->SetTransform(D3DTS_WORLD, &mat_world);

			// アニメーションを進める(D3DXFRAME::TransformationMatrix をキーフレームに合わせて更新している)
			frame_set->update_animation_controller();

			// ワールド変換行列を元にフレーム変換行列を生成する
			// mat_world と D3DXFRAME::TransformationMatrix から 合成行列 S_d3d_frame::CombinedTransformationMatrix を算出
			frame_set->update_matrix(&mat_world);

			// フレームセットのアップデート(うまくいかなかったので保留)
			//if(!update_frame_set(p_sprite, &p_sprite->rp, frame_set->m_frame_root.get()))
			//	return false;
		}
	}

	return true;
}
// ****************************************************************
// フレームセットのアップデート
// ================================================================
bool C_d3d_renderer::update_frame_set(C_d3d_sprite* p_sprite, const S_d3d_render_param* rp, C_d3d_frame* frame)
{
	// メッシュがあればメッシュを描画！(多分pNextMeshContainerを実装しないとsizeは常に1？)
	for (int mesh_no = 0; mesh_no < (int)frame->m_mesh_list.size(); mesh_no ++)	{
		BSP<C_d3d_mesh> mesh = frame->m_mesh_list[mesh_no];
		S_d3d_skinned_mesh_container* p_mc = (S_d3d_skinned_mesh_container *)frame->body()->pMeshContainer;

		// スキンがない場合
		if (!p_mc->pSkinInfo)	{
			// 何もしない
		}
		// スキンがある場合
		else	{
			ID3DXSkinInfo* skin = p_mc->pSkinInfo;
			D3DXBONECOMBINATION* p_bone_conbination = (D3DXBONECOMBINATION *)p_mc->pBoneCombinationBuf->GetBufferPointer();
			UINT matrix_index;

			for (int i = 0; i < (int)p_mc->NumAttributeGroups; i++)	{

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
				// 他は特に何もしない
			}
		}

	}
	// 兄弟フレームを描画
	if (frame->m_sibling)	{
		if (!update_frame_set(p_sprite, rp, frame->m_sibling.get()))
			return false;
	}
	// 子フレームを描画
	if (frame->m_first_child)	{
		if (!update_frame_set(p_sprite, rp, frame->m_first_child.get()))
			return false;
	}

	return true;
}

// ****************************************************************
// レンダー
// ================================================================
bool C_d3d_renderer::render()
{
	HRESULT hr;

	if (sprite_list.empty())
		return true;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("Direct3D デバイスが準備されていません。"), _T("render"));
		return false;
	}

	bool ret_value = false;

	// レンダリング開始
	hr = G_d3d.device->BeginScene();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("BeginScene"), hr);
	}
	else	{

		//// ブライトパスを使用する場合(主に輝度描画)
		//if(use_bright_path) {
		//	DWORD RTs = G_d3d.device.device_caps().NumSimultaneousRTs;	// レンダーターゲットの最大数
		//	// マルチレンダーターゲットが使えない場合
		//	if(RTs <= 1) {
		//		// カラー情報、輝度情報を順番に描画
		//		// カラー情報の描画
		//		// レンダーターゲットの変更
		//		// 輝度情報の変更
		//		// レンダーターゲットの変更？
		//	}
		//	// マルチレンダーターゲットが使える場合
		//	else {
		//		// カラー情報をRT0、輝度情報をRT1に同時描画
		//		// レンダーターゲットの設定
		//		/// カラー情報、輝度情報の描画
		//	}
		//}

		// スプライトリストをレンダー
		if (m_render_default.render_sprite_list(sprite_list))	{

			// 処理成功！
			ret_value = true;
		}

		// レンダリング終了
		G_d3d.device->EndScene();
	}

	return ret_value;
}

// ****************************************************************
// プリレンダー
// ================================================================
bool C_d3d_renderer::pre_render()
{
	HRESULT hr;

	if (sprite_list.empty())
		return true;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("Direct3D デバイスが準備されていません。"), _T("render"));
		return false;
	}

	bool ret_value = false;

	// レンダリング開始
	hr = G_d3d.device->BeginScene();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("BeginScene"), hr);
	}
	else	{
		// RT変更処理などを無条件で行っているので修正する

		// レンダーターゲットなどを保存
		if(!G_d3d.device.get_render_target(0, &m_default_depth_stencil_buffer))
			return false;
		if(!G_d3d.device.get_depth_stencil_surface(&m_default_back_buffer))
			return false;
		if(!G_d3d.device.get_viewport(&m_default_viewport))
			return false;

		// レンダーターゲットなどをシャドウマップ用に切り替え
		if(!G_d3d.device.set_render_target(0, m_shadow_map_surface))
			return false;
		if(!G_d3d.device.set_depth_stencil_surface(m_shadow_map_depth_stencil_buffer))
			return false;
		if(!G_d3d.device.set_viewport(&m_shadow_map_viewport))
			return false;
		// シャドウマップのクリア
		hr = G_d3d.device.body()->Clear(
						0L,
						NULL,
						D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						0xFFFFFFFF,
						1.0f,
						0L
						);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("Clear"), hr);
			return false;
		}

		// シャドウマップのプリレンダ
		if (m_pre_render_shadow_map.render_sprite_list(sprite_list))	{
			// 処理成功！
			ret_value = true;
		}

		// 今の所はこの時点でレンダーターゲットなどを元に戻しておく
		if(!G_d3d.device.set_render_target(0, m_default_depth_stencil_buffer))
			return false;
		if(!G_d3d.device.set_depth_stencil_surface(m_default_back_buffer))
			return false;
		if(!G_d3d.device.set_viewport(&m_default_viewport))
			return false;

		m_default_depth_stencil_buffer->Release();
		m_default_back_buffer->Release();

		// シャドウマップのクリア
		hr = G_d3d.device.body()->Clear(
						0L,
						NULL,
						D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						0x00404080,
						1.0f,
						0L
						);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("Clear"), hr);
			return false;
		}

		// レンダリング終了
		G_d3d.device->EndScene();
	}

	return ret_value;
}
// ****************************************************************
// ポストレンダー
// ================================================================
bool C_d3d_renderer::post_render()
{
	HRESULT hr;

	if (sprite_list.empty())
		return true;

	if (G_d3d.device.body() == NULL)	{
		set_last_error(_T("Direct3D デバイスが準備されていません。"), _T("render"));
		return false;
	}

	bool ret_value = false;

	// レンダリング開始
	hr = G_d3d.device->BeginScene();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("BeginScene"), hr);
	}
	else	{

		// スプライトリストをレンダー
		//if (render_sprite_list(sprite_list))	{

		//	// 処理成功！
		//	ret_value = true;
		//}

		// レンダリング終了
		G_d3d.device->EndScene();
	}

	return ret_value;
}


// ****************************************************************
// デバッグ用シャドウマップの描画
// ================================================================
bool C_d3d_renderer::debug_render_shadow_map(int render_size, int x, int y)
{
	if(!m_shadow_map_tex)
		return true;	// 失敗ではないのでtrue

	typedef struct {
		FLOAT       p[4];
		FLOAT       tu, tv;
	} TVERTEX;

	G_d3d.device->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
	G_d3d.device->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
	G_d3d.device->SetTextureStageState(1,D3DTSS_COLOROP,    D3DTOP_DISABLE);

	TVERTEX Vertex[4] = {
		// x  y  z rhw tu tv
		{	(float)x,				(float)y,0, 1, 0, 0,},
		{	(float)x+render_size,	(float)y,0, 1, 1, 0,},
		{	(float)x+render_size,	(float)y+render_size,0, 1, 1, 1,},
		{	(float)x,				(float)y+render_size,0, 1, 0, 1,},
	};

	G_d3d.device->SetTexture( 0, m_shadow_map_tex );
	G_d3d.device->SetVertexShader(NULL);
	G_d3d.device->SetPixelShader(NULL);
	G_d3d.device->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
	G_d3d.device->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TVERTEX ) );
	G_d3d.device->SetTexture( 0, NULL );

	return true;
}

bool C_d3d_renderer::debug_render_light_bloom_tex(int render_size, int x, int y)
{
	if(!m_light_bloom_tex)
		return true;	// 失敗ではないのでtrue

	typedef struct {
		FLOAT       p[4];
		FLOAT       tu, tv;
	} TVERTEX;

	G_d3d.device->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
	G_d3d.device->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
	G_d3d.device->SetTextureStageState(1,D3DTSS_COLOROP,    D3DTOP_DISABLE);

	TVERTEX Vertex[4] = {
		// x  y  z rhw tu tv
		{	(float)x,				(float)y,0, 1, 0, 0,},
		{	(float)x+render_size,	(float)y,0, 1, 1, 0,},
		{	(float)x+render_size,	(float)y+render_size,0, 1, 1, 1,},
		{	(float)x,				(float)y+render_size,0, 1, 0, 1,},
	};

	G_d3d.device->SetTexture( 0, m_light_bloom_tex );
	G_d3d.device->SetVertexShader(NULL);
	G_d3d.device->SetPixelShader(NULL);
	G_d3d.device->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
	G_d3d.device->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TVERTEX ) );
	G_d3d.device->SetTexture( 0, NULL );

	return true;
}

}

