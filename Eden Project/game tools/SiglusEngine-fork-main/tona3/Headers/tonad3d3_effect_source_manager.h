#pragma once

#include	"tonad3d3_d3d.h"
#include	"tonad3d3_mesh.h"

namespace NT3
{


class C_d3d_effect_data;
class C_d3d_effect_source_manager;

typedef std::map< TSTR, BSP<C_d3d_effect_data> > SHADER_MAP;

// データ(ID3DXEffectベタ書きでもOKだけど拡張時の事を考えてクラス化)
class C_d3d_effect_data {
	friend class C_d3d_effect_source_manager;
private:
	BIP<ID3DXEffect>	m_effect;
	// 自動解放用のカウンタ変数etc
	// key(削除用)
public:
	C_d3d_effect_data();
	~C_d3d_effect_data();
	ID3DXEffect* get_effect();

};


// マネージャ
class C_d3d_effect_source_manager {
public:
	C_d3d_effect_source_manager();
	~C_d3d_effect_source_manager();

	// 生成(失敗NULL)
	C_d3d_effect_data* create_effect_source(CTSTR& key, CMBSTR& data);
	// 検索(失敗NULL)
	C_d3d_effect_data* search_effect_source(CTSTR& key);

	// 解放
	void destroy_effect_source(CTSTR& key);
	// 全解放
	void destroy_effect_source_all();


	// デバッグ用？開発用？
	// ファイルから読み込み、それをマネージャに登録する
	C_d3d_effect_data* create_effect_source(CTSTR& key);

private:
	SHADER_MAP m_ShaderMap;
};

// 唯一のインスタンス
extern C_d3d_effect_source_manager G_d3d_effect_source_manager;



// ヘルパー
ID3DXEffect* create_effect_for_mesh_from_file(CTSTR& file_path, C_d3d_mesh* p_mesh);
ID3DXEffect* create_effect_for_mesh_from_mesh(C_d3d_mesh* p_mesh, C_d3d_mtrl* p_mtrl, ID3DXSkinInfo* p_skin_info);
ID3DXEffect* create_effect_for_shadow_map_from_mesh(C_d3d_mesh* p_mesh, C_d3d_mtrl* p_mtrl, ID3DXSkinInfo* p_skin_info);


}