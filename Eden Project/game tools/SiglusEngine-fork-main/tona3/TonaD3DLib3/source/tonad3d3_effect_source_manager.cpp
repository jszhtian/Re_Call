#include "tonad3d3_pch.h"
#include "tonad3d3_effect_source_manager.h"
#include "tonad3d3_effect_source_creator.h"
#include "tonad3d3_effect_source_creator_skinned_mesh.h"
#include "tonad3d3_effect_source_creator_shadow_map.h"

namespace NT3
{

// 唯一のインスタンス
C_d3d_effect_source_manager G_d3d_effect_source_manager;



// データ
C_d3d_effect_data::C_d3d_effect_data()
{
}

C_d3d_effect_data::~C_d3d_effect_data()
{
}

ID3DXEffect* C_d3d_effect_data::get_effect()
{
	return m_effect.get();
}



// マネージャ
C_d3d_effect_source_manager::C_d3d_effect_source_manager()
{
	m_ShaderMap.clear();
}

C_d3d_effect_source_manager::~C_d3d_effect_source_manager()
{
	destroy_effect_source_all();
}

// 生成(失敗NULL)
C_d3d_effect_data* C_d3d_effect_source_manager::create_effect_source(CTSTR& key, CMBSTR& data)
{
	HRESULT hr;

	// 既に登録されているかを確認する
	SHADER_MAP::iterator it = m_ShaderMap.find(key);
	if(it != m_ShaderMap.end()) {
		// 登録済み
		return it->second.get();
	}

	// 登録されていないので新規登録
	BSP<C_d3d_effect_data> sd = BSP<C_d3d_effect_data>(new C_d3d_effect_data());
	DWORD flag = 0;
	ID3DXBuffer* p_error = NULL;
	ID3DXEffect* p_effect = NULL;

#if 1
	// メモリから読み込む
	hr = D3DXCreateEffect(G_d3d.device.body(), data.c_str(), (UINT)data.size(), 0, 0, flag, 0, &p_effect, &p_error);
#else
	// ファイルから読み込む
	hr = D3DXCreateEffectFromFile(G_d3d.device.body(), key.c_str(), 0, 0, flag, 0, &p_effect, &err);
#endif
	if(FAILED(hr)) {
		set_last_error_d3d(MBSTR_to_TSTR(p_error ? (char*)p_error->GetBufferPointer() : ""), _T("D3DXCreateEffect"), hr);
		MessageBox(NULL, get_last_error_msg().c_str(), NULL, MB_OK);
		return NULL;
	}

	// 実際に登録
	sd->m_effect = BIP<ID3DXEffect>(p_effect, false);
	m_ShaderMap.insert(std::make_pair(key, sd));

	return sd.get();
}
// 検索(失敗NULL)
C_d3d_effect_data* C_d3d_effect_source_manager::search_effect_source(CTSTR& key)
{
	SHADER_MAP::iterator it;

	// 既に登録されているかを確認する
	it = m_ShaderMap.find(key);
	if(it != m_ShaderMap.end()){
		// 登録済み
		return it->second.get();
	}

	// 登録していない
	return NULL;
}
// 解放
void C_d3d_effect_source_manager::destroy_effect_source(CTSTR& key)
{
	SHADER_MAP::iterator it;

	// 既に登録されているかを確認する
	it = m_ShaderMap.find(key);
	if(it != m_ShaderMap.end()){
		// 登録済み
//		SAFE_DELETE(it->second);
		m_ShaderMap.erase(it);
	}
}
// 全解放
void C_d3d_effect_source_manager::destroy_effect_source_all()
{
	SHADER_MAP::iterator it;

	// 全削除
	for(it = m_ShaderMap.begin(); it != m_ShaderMap.end(); ++it) {
//		SAFE_DELETE(it->second);
	}
	m_ShaderMap.clear();
}

// デバッグ用？開発用？
C_d3d_effect_data* C_d3d_effect_source_manager::create_effect_source(CTSTR& key)
{
	HRESULT hr;

	// 既に登録されているかを確認する
	SHADER_MAP::iterator it = m_ShaderMap.find(key);
	if(it != m_ShaderMap.end()){
		// 登録済み
		return it->second.get();
	}

	// 登録されていないので新規登録
	BSP<C_d3d_effect_data> sd = BSP<C_d3d_effect_data>(new C_d3d_effect_data());
	DWORD flag = 0;
	ID3DXBuffer* p_error = NULL;
	ID3DXEffect* p_effect = NULL;

	// ファイルから読み込む
	hr = D3DXCreateEffectFromFile(G_d3d.device.body(), key.c_str(), 0, 0, flag, 0, &p_effect, &p_error);
	if(FAILED(hr)) {
		set_last_error_d3d(MBSTR_to_TSTR(p_error ? (char*)p_error->GetBufferPointer() : ""), _T("D3DXCreateEffectFromFile"), hr);
		return NULL;
	}

	// 実際に登録
	sd->m_effect = BIP<ID3DXEffect>(p_effect, false);
	m_ShaderMap.insert(std::make_pair(key, sd));

	return sd.get();
}





// 以下ヘルパー関数群
// エフェクトコンパイル・生成時にマネージャに登録してくれる
ID3DXEffect* create_effect_for_mesh_from_file(CTSTR& file_path, C_d3d_mesh* p_mesh)
{
	S_d3d_mesh_param* p_mp = p_mesh->get_mesh_param_ptr();
	ID3DXEffect*					p_effect = NULL;

	// 前フレームのキーと比較
	if(p_mp->effect_old_key == file_path) {
		// 前のフレームと同じ
		p_effect = p_mp->p_effect;
	}
	else {
		// 前のフレームと異なっている
		p_mp->effect_old_key = file_path;

		// キー(ファイル名)よるエフェクトソース検索
		C_d3d_effect_data* p_effect_data = G_d3d_effect_source_manager.search_effect_source(file_path);
		// 見つかった
		if(p_effect_data) {
			p_mp->p_effect = p_effect_data->get_effect();
			p_effect = p_mp->p_effect;
		}
		// 見つからない
		else {
			// コンパイル(生成)してマネージャに登録
			p_effect_data = G_d3d_effect_source_manager.create_effect_source(file_path);
			if(p_effect_data) {
				p_mp->p_effect = p_effect_data->get_effect();
				p_effect = p_mp->p_effect;
			}
			else {
				// コンパイル失敗
				p_effect = NULL;
			}
		}
	}

	return p_effect;
}


ID3DXEffect* create_effect_for_mesh_from_mesh(C_d3d_mesh* p_mesh, C_d3d_mtrl* p_mtrl, ID3DXSkinInfo* p_skin_info)
{
	DWORD fvf = p_mesh->body()->GetFVF();
	S_d3d_mesh_param* p_mp = p_mesh->get_mesh_param_ptr();
	C_d3d_effect_semantics_info		si;
	C_d3d_effect_texture_info		ti;
	C_d3d_effect_func_info			fi;
	ID3DXEffect*					p_effect = NULL;

	// 将来的にメモリ展開時(リリース)にはギリギリまで短縮するかも
	TSTR effect_file_name = _T("effect\\");

	// スキンメッシュかどうか
	if(p_skin_info)
		effect_file_name += _T("skn");
	else
		effect_file_name += _T("frm");

	// 頂点デクラレーションによる判定
	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	p_mesh->body()->GetDeclaration(decl);
	LPD3DVERTEXELEMENT9 decl_cur = decl;
	// 現状の仕様だと見つかった物からファイル名に+しているから順番が違うだけでも別ファイル扱いになる
	while (decl_cur->Stream != 0xff) {
		if(decl_cur->Usage == D3DDECLUSAGE_POSITION) {
			si.typeAry.push_back(C_d3d_effect_semantics_info::POSITION);
			effect_file_name += _T("_p");
		}
		if(decl_cur->Usage == D3DDECLUSAGE_BLENDWEIGHT) {
			si.typeAry.push_back(C_d3d_effect_semantics_info::BLENDWEIGHT);
			effect_file_name += _T("_bw");
		}
		if(decl_cur->Usage == D3DDECLUSAGE_BLENDINDICES) {
			si.typeAry.push_back(C_d3d_effect_semantics_info::BLENDINDICES);
			effect_file_name += _T("_bi");
		}
		if(decl_cur->Usage == D3DDECLUSAGE_NORMAL) {
			si.typeAry.push_back(C_d3d_effect_semantics_info::NORMAL);
			effect_file_name += _T("_n");
		}
		if(decl_cur->Usage == D3DDECLUSAGE_TEXCOORD) {
			si.typeAry.push_back(C_d3d_effect_semantics_info::TEXCOORD);
			effect_file_name += _T("_uv");
		}
		if(decl_cur->Usage == D3DDECLUSAGE_TANGENT) {
			si.typeAry.push_back(C_d3d_effect_semantics_info::TANGENT);
			effect_file_name += _T("_t");
		}
		if(decl_cur->Usage == D3DDECLUSAGE_BINORMAL) {
			si.typeAry.push_back(C_d3d_effect_semantics_info::BINORMAL);
			effect_file_name += _T("_bn");
		}
		if(decl_cur->Usage == D3DDECLUSAGE_COLOR) {
			si.typeAry.push_back(C_d3d_effect_semantics_info::COLOR);
			effect_file_name += _T("_c");
		}

		decl_cur++;
	}

	/*
	// FVFによる判定
	if(fvf & D3DFVF_XYZ) {
		si.typeAry.push_back(C_d3d_effect_semantics_info::POSITION);
		effect_file_name += _T("_p");
	}
	if(fvf & D3DFVF_XYZB1 || fvf & D3DFVF_XYZB2 || fvf & D3DFVF_XYZB3 || fvf & D3DFVF_XYZB4 || fvf & D3DFVF_XYZB5) {
		// これは仮ソース
		if(fvf & D3DFVF_XYZ)
			effect_file_name = effect_file_name.substr(0, effect_file_name.size() - 2);

		// 頂点ブレンディングを行う(スキンメッシュ？)場合
		si.typeAry.push_back(C_d3d_effect_semantics_info::POSITION);
		si.typeAry.push_back(C_d3d_effect_semantics_info::BLENDINDICES);
		si.typeAry.push_back(C_d3d_effect_semantics_info::BLENDWEIGHT);
		effect_file_name += _T("_b");
	}
	if(fvf & D3DFVF_NORMAL) {
		si.typeAry.push_back(C_d3d_effect_semantics_info::NORMAL);
		effect_file_name += _T("_n");
	}
	if(fvf & D3DFVF_DIFFUSE || fvf & D3DFVF_SPECULAR) {
		// スペキュラ条件いらない？
		si.typeAry.push_back(C_d3d_effect_semantics_info::COLOR);
		effect_file_name += _T("_c");
	}
	if(fvf & D3DFVF_TEX1) {
		si.typeAry.push_back(C_d3d_effect_semantics_info::TEXCOORD);
		effect_file_name += _T("_uv");
	}
	*/

	if(p_mp->use_mesh_tex && p_mtrl->texture) {
		ti.addSemanticsName("MESHTEXTURE");
		effect_file_name += _T("_mt");
	}
	if(p_mp->shading_type == SHADING_TYPE_DEPTH_BUFFER) {	// 条件にシャドウテクスチャがあるかも追加する？
		// 影テクスチャ
		ti.addSemanticsName("SHADOWTEXTURE");
		effect_file_name += _T("_st");
	}
	if(p_mp->lighting_type == LIGHTING_TYPE_TOON &&
		p_mp->toon_tex && p_mp->toon_tex->body()) {	// 条件にトゥーンテクスチャがあるかも追加する？
		// トゥーンテクスチャ
		ti.addSemanticsName("TOONTEXTURE");
		effect_file_name += _T("_tt");
	}
	if((p_mp->lighting_type == LIGHTING_TYPE_BUMP || p_mp->lighting_type == LIGHTING_TYPE_PARALLAX) &&
		p_mp->normal_tex && p_mp->normal_tex->body()) {	// 条件に法線テクスチャがあるかも追加する？
		// 法線テクスチャ
		ti.addSemanticsName("NORMALTEXTURE");
		effect_file_name += _T("_nt");
	}
	
	if(p_mp->use_mul_vertex_color) {
		fi.typeAry.push_back(C_d3d_effect_func_info::VERTEX);
		effect_file_name += _T("_vc");
	}
	if(p_mp->use_mrbd) {
		fi.typeAry.push_back(C_d3d_effect_func_info::MRBD);
		effect_file_name += _T("_mrbd");
	}
	if(p_mp->use_rgb) {
		fi.typeAry.push_back(C_d3d_effect_func_info::RGB);
		effect_file_name += _T("_rgb");
	}

	if(false);
	else if(p_mp->lighting_type == LIGHTING_TYPE_LAMBERT)			effect_file_name += _T("_lmbt");
	else if(p_mp->lighting_type == LIGHTING_TYPE_BLINN_PHONG)		effect_file_name += _T("_blph");
	else if(p_mp->lighting_type == LIGHTING_TYPE_PP_BLINN_PHONG)	effect_file_name += _T("_ppbp");
	else if(p_mp->lighting_type == LIGHTING_TYPE_PP_HALF_LAMBERT)	effect_file_name += _T("_pphl");
	else if(p_mp->lighting_type == LIGHTING_TYPE_TOON)				effect_file_name += _T("_toon");
	else if(p_mp->lighting_type == LIGHTING_TYPE_FFP)				effect_file_name += _T("_ffp");
	else if(p_mp->lighting_type == LIGHTING_TYPE_PP_FFP)			effect_file_name += _T("_ppfp");
	else if(p_mp->lighting_type == LIGHTING_TYPE_BUMP)				effect_file_name += _T("_bump");
	else if(p_mp->lighting_type == LIGHTING_TYPE_PARALLAX)			effect_file_name += _T("_para");
	else if(p_mp->lighting_type == LIGHTING_TYPE_NONE)				effect_file_name += _T("_nolt");

	if(false);
	else if(p_mp->shading_type == SHADING_TYPE_DEPTH_BUFFER)		effect_file_name += _T("_dpbs");
	else if(p_mp->shading_type == SHADING_TYPE_NONE)				effect_file_name += _T("_nost");

	if(p_mp->shader_option) {
		if(p_mp->shader_option & SHADER_OPTION_RIM_LIGHT)			effect_file_name += _T("_rmlt");
	}

	effect_file_name += _T(".fx");

	// 前フレームのキーと比較
	if(p_mp->effect_old_key == effect_file_name) {
		// 前のフレームと同じ
		p_effect = p_mp->p_effect;
	}
	else {
		// 前のフレームと異なっている
		p_mp->effect_old_key = effect_file_name;

		// キー(ファイル名)よるエフェクトソース検索
		C_d3d_effect_data* p_effect_data = G_d3d_effect_source_manager.search_effect_source(effect_file_name);
		// 見つかった
		if(p_effect_data) {
			p_mp->p_effect = p_effect_data->get_effect();
			p_effect = p_mp->p_effect;
		}
		// 見つからない
		else {
			MBSTR effect_source;
			// 生成する
			C_d3d_effect_source_creator_skinned_mesh creator;
			creator.createShader(effect_source, si, ti, fi, p_mp->lighting_type, p_mp->shading_type, p_mp->shader_option);

			// コンパイル(生成)してマネージャに登録
			p_effect_data = G_d3d_effect_source_manager.create_effect_source(effect_file_name, effect_source);
			if(p_effect_data) {
				p_mp->p_effect = p_effect_data->get_effect();
				p_effect = p_mp->p_effect;
			}
			else {
				// コンパイル失敗
				p_effect = NULL;
			}
#ifdef _DEBUG
			// リリースなら書き出さない方が良いかも
			// ファイルに書き出す(エラー修正の為に失敗でも書き出す)
			std::ofstream ofs(effect_file_name.c_str());
			ofs.write(effect_source.c_str(), (std::streamsize)effect_source.size());
			ofs.close();
#endif

		}
	}

	return p_effect;
}

ID3DXEffect* create_effect_for_shadow_map_from_mesh(C_d3d_mesh* p_mesh, C_d3d_mtrl* p_mtrl, ID3DXSkinInfo* p_skin_info)
{
	DWORD fvf = p_mesh->body()->GetFVF();
	S_d3d_mesh_param* p_mp = p_mesh->get_mesh_param_ptr();
	C_d3d_effect_semantics_info		si;
	C_d3d_effect_texture_info		ti;
	C_d3d_effect_func_info			fi;
	ID3DXEffect*					p_effect = NULL;

	// 将来的にメモリ展開時(リリース)にはギリギリまで短縮するかも
	TSTR effect_file_name = _T("effect\\");

	effect_file_name += _T("shadow_map");

	// スキンメッシュかどうか
	if(p_skin_info)
		effect_file_name += _T("_skn");
	else
		effect_file_name += _T("_frm");

	// 頂点デクラレーションによる判定
	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	p_mesh->body()->GetDeclaration(decl);
	LPD3DVERTEXELEMENT9 decl_cur = decl;
	while (decl_cur->Stream != 0xff) {
		if(decl_cur->Usage == D3DDECLUSAGE_POSITION) {
			si.typeAry.push_back(C_d3d_effect_semantics_info::POSITION);
			effect_file_name += _T("_p");
		}
		if(decl_cur->Usage == D3DDECLUSAGE_BLENDWEIGHT) {
			si.typeAry.push_back(C_d3d_effect_semantics_info::BLENDWEIGHT);
			effect_file_name += _T("_bw");
		}
		if(decl_cur->Usage == D3DDECLUSAGE_BLENDINDICES) {
			si.typeAry.push_back(C_d3d_effect_semantics_info::BLENDINDICES);
			effect_file_name += _T("_bi");
		}
		if(decl_cur->Usage == D3DDECLUSAGE_NORMAL) {
			si.typeAry.push_back(C_d3d_effect_semantics_info::NORMAL);
			effect_file_name += _T("_n");
		}

		decl_cur++;
	}

	effect_file_name += _T(".fx");

	// 前フレームのキーと比較
	if(p_mp->shadow_map_effect_old_key == effect_file_name) {
		// 前のフレームと同じ
		p_effect = p_mp->p_shadow_map_effect;
	}
	else {
		// 前のフレームと異なっている
		p_mp->shadow_map_effect_old_key = effect_file_name;

		// キー(ファイル名)よるエフェクトソース検索
		C_d3d_effect_data* p_effect_data = G_d3d_effect_source_manager.search_effect_source(effect_file_name);
		// 見つかった
		if(p_effect_data) {
			p_mp->p_shadow_map_effect = p_effect_data->get_effect();
			p_effect = p_mp->p_shadow_map_effect;
		}
		// 見つからない
		else {
			MBSTR effect_source;
			// 生成する
			C_d3d_effect_source_creator_shadow_map creator;
			creator.createShader(effect_source, si, ti, fi, LIGHTING_TYPE_NONE, SHADING_TYPE_NONE, SHADER_OPTION_NONE);

			// コンパイル(生成)してマネージャに登録
			p_effect_data = G_d3d_effect_source_manager.create_effect_source(effect_file_name, effect_source);
			if(p_effect_data) {
				p_mp->p_shadow_map_effect = p_effect_data->get_effect();
				p_effect = p_mp->p_shadow_map_effect;
			}
			else {
				// コンパイル失敗
				p_effect = NULL;
			}
#ifdef _DEBUG
			// リリースなら書き出さない方が良いかも
			// ファイルに書き出す(エラー修正の為に失敗でも書き出す)
			std::ofstream ofs(effect_file_name.c_str());
			ofs.write(effect_source.c_str(), (std::streamsize)effect_source.size());
			ofs.close();
#endif

		}
	}

	return p_effect;
}



















}