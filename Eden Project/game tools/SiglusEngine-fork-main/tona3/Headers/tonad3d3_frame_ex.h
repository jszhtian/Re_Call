#pragma		once

#include	"tonad3d3_mesh.h"
#include	"tonad3d3_animation_controller.h"

namespace NT3 {

// D3DXを拡張した物はD3DXの命名規則に従う

// ****************************************************************
// D3D フレーム：フレーム構造体を拡張
// ================================================================
//	struct D3DXFRAME
//	{
//		LPSTR                   Name;					// フレームの名前
//		D3DXMATRIX              TransformationMatrix;	// 変換行列
//		LPD3DXMESHCONTAINER     pMeshContainer;			// メッシュコンテナ
//		struct _D3DXFRAME       *pFrameSibling;			// 兄弟フレーム
//		struct _D3DXFRAME       *pFrameFirstChild;		// 子供フレーム
//	};
// ================================================================
struct S_d3d_frame : public D3DXFRAME {
	D3DXMATRIX		CombinedTransformationMatrix;		// 合成した変換行列
														// スキンメッシュ変換を含まない行列
};

// ****************************************************************
// D3D フレーム：メッシュコンテナ構造体を拡張
// ================================================================
//	struct D3DXMESHCONTAINER
//	{
//		LPSTR                   Name;					// メッシュの名前
//		D3DXMESHDATA            MeshData;				// メッシュデータ
//		LPD3DXMATERIAL          pMaterials;				// マテリアル
//		LPD3DXEFFECTINSTANCE    pEffects;				// エフェクト
//		DWORD                   NumMaterials;			// マテリアル数
//		DWORD                  *pAdjacency;				// 隣接情報
//		LPD3DXSKININFO          pSkinInfo;				// スキン
// 
//		struct _D3DXMESHCONTAINER *pNextMeshContainer;
//	};
// ================================================================
struct S_d3d_skinned_mesh_container : public D3DXMESHCONTAINER {
	// スキンメッシュアニメーションをしない場合は D3DXMESHCONTAINER を使用する事(やっぱ使用する)
	LPD3DXMESH				pOrigMesh;					// オリジナルのメッシュ

	DWORD					NumInfl;					// １つの頂点に影響するボーンの最大数
	DWORD					NumAttributeGroups;			// 属性グループ数

	LPD3DXBUFFER			pBoneCombinationBuf;		// ボーンコンビネーションテーブルというボーンと頂点の対応表が配列として入る変数

	DWORD					NumPaletteEntries;			// スキンアニメブレンド行列の合成に使う
														// std::min<DWORD>(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());
	ARRAY<D3DXMATRIX>		SkinnedBlendMatrices;		// スキンアニメブレンド行列の配列


	// FVFを使わないようにする
	//LPDIRECT3DVERTEXDECLARATION9	pDecl;				// 頂点宣言
};

// ****************************************************************
// D3D フレーム階層
// ================================================================
//		ID3DXAllocateHierarchy は純粋仮想関数を持つクラスです。
//		これを継承して、以下の４つをオーバーロードするクラスを作る必要があります。
//			CreateFrame, CreateMeshContainer, DestroyFrame, DestroyMeshContainer 
// ================================================================
class C_d3d_allocate_hierarchy : public ID3DXAllocateHierarchy {
public:
	C_d3d_allocate_hierarchy()			{}
	virtual ~C_d3d_allocate_hierarchy()	{}

	virtual HRESULT __stdcall	CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame);
	virtual HRESULT __stdcall	CreateMeshContainer(
		LPCSTR Name, 
		CONST D3DXMESHDATA* pMeshData, 
		CONST D3DXMATERIAL* pMaterials, 
		CONST D3DXEFFECTINSTANCE* pEffectInstances, 
		DWORD NumMaterials, 
		CONST DWORD* pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER* ppNewMeshContainer
		);

	virtual HRESULT __stdcall	DestroyFrame(LPD3DXFRAME pFrameToFree);
	virtual HRESULT __stdcall	DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree); 

private:
	char*	allocate_name(const char* src);
	HRESULT	generate_skinned_mesh(S_d3d_skinned_mesh_container* p_mc);
	HRESULT destroy_skinned_mesh_container(S_d3d_skinned_mesh_container* p_mc);
};


// ****************************************************************
// フレーム
// ================================================================
class C_d3d_frame {
public:
	bool	create_frame(C_d3d_frame* root_frame, S_d3d_frame* p_frame, ARRAY< BSP<C_d3d_mesh> >& mesh_list, ARRAY<TSTR>& texture_name_list);
	bool	update_matrix(const D3DXMATRIX* parent_matrix);

	S_d3d_frame*	body()			{	return m_frame;	}
	S_d3d_frame*	operator -> ()	{	return m_frame;	}

	ARRAY< BSP<C_d3d_mesh> >		m_mesh_list;			// メッシュ
														// フレーム以下に複数メッシュがぶら下がらない限りARRAYじゃなくて良い気がする
	BSP<C_d3d_frame>			m_sibling;				// 兄弟フレーム
	BSP<C_d3d_frame>			m_first_child;			// 子フレーム
	ARRAY<D3DXMATRIX>			m_offset_matrix_list;	// オフセット行列
	ARRAY<D3DXMATRIX*>			m_root_matrix_list;		// ルート行列

private:
	S_d3d_frame*				m_frame;				// オリジナルのフレーム
};

// ****************************************************************
// フレームセット
// ================================================================
class C_d3d_frame_hierarchy_parameter_setter;
class C_d3d_frame_set {
public:
	C_d3d_frame_set();
	~C_d3d_frame_set();

	void	init();		// 初期化
	void	release();	// 解放

	bool	load_x(CTSTR& file_path, ARRAY< BSP<C_d3d_mesh> >& mesh_list, ARRAY<TSTR>& texture_name_list);
	//bool	advance_animation(double past_time);
	bool	update_matrix(const D3DXMATRIX* matrix);
	void	update_animation_controller();

	BSP<C_d3d_frame>				m_frame_root;				// ルートのフレーム
	C_d3d_allocate_hierarchy		m_allocate_hierarchy;		// フレーム階層
	//BIP<ID3DXAnimationController>	m_animation_controller;	// アニメーションコントローラ
	C_d3d_animation_controller		m_animation_controller;	// アニメーションコントローラ


	// 全メッシュに一括設定の時に使うリスト/*要修正*/
	ARRAY<C_d3d_mesh*>				m_mesh_list_for_parm_set;



	// フレーム名を使ってこのクラスのフレームセット内からフレームを検索する
	C_d3d_frame* search_frame_by_name(MBSTR& frame_name);
	// フレーム名を使って引数のフレームセット内からフレームを検索する
	static C_d3d_frame* search_frame_by_name(MBSTR& frame_name, C_d3d_frame_set* p_frame_set);

	// パラメータの設定(対象フレームのみ)
	static void set_parameter_to_target(C_d3d_frame* p_frame, C_d3d_frame_hierarchy_parameter_setter* p_setter);
	// パラメータの設定(対象フレームと対象フレームの子供以下全て)
	static void set_parameter_to_target_and_target_below_child(C_d3d_frame* p_frame, C_d3d_frame_hierarchy_parameter_setter* p_setter);
	// パラメータの設定(対象フレームの子供以下全て。ただし対象フレームを除く)
	static void set_parameter_to_target_below_child(C_d3d_frame* p_frame, C_d3d_frame_hierarchy_parameter_setter* p_setter);
	// ☆現時点では対象フレームの兄弟を含んだ何かからスタートするものは実装していない

private:
	// フレーム名を使ってフレームセット内からフレームを検索する
	static C_d3d_frame* search_frame_hierarchy_by_name(MBSTR& frame_name, C_d3d_frame* p_frame);

	// パラメータの設定
	static void set_parameter_hierarchy(C_d3d_frame* p_frame, C_d3d_frame_hierarchy_parameter_setter* p_setter);
};

// サポート関数


// 
// 階層構造を把握しやすくする構造体
// 

// メッシュにセット用の関数持たせる？関数ポインタ用
// C_d3d_frame::set_data(data, func)				// 自分が所持するメッシュにセット
// C_d3d_frame::set_data_all(data, func)			// 自分以下のフレーム全てが所持するメッシュにセット
// set_data_all()みたいな関数の中で階層構造を辿っていく処理をする
// 10種類データをセットするときに10回階層構造を辿るのは無駄が多い気がする
// C_d3d_frame::set_data_all(Cdatalist dl) みたいなのを作って1回辿る時に全部セットする？
// Cdatalistはセットするかもしれないデータ全て(メッシュパラメータとか)と、それに1個1個対応した設定したかフラグを持つ
// 



// ****************************************************************
// D3Dフレーム階層パラメータの種類
// ================================================================
enum FRAME_HIERARCHY_PARAMETER_TYPE {
	MP__LIGHTING_TYPE,
	MP__SHADING_TYPE,
};

typedef std::map<FRAME_HIERARCHY_PARAMETER_TYPE, int>			FRAME_HIERARCHY_PARAMETER_TYPE_INT_MAP;
typedef std::map<FRAME_HIERARCHY_PARAMETER_TYPE, bool>			FRAME_HIERARCHY_PARAMETER_TYPE_BOOL_MAP;
typedef std::map<FRAME_HIERARCHY_PARAMETER_TYPE, float>			FRAME_HIERARCHY_PARAMETER_TYPE_FLOAT_MAP;
typedef std::map<FRAME_HIERARCHY_PARAMETER_TYPE, D3DXVECTOR4>	FRAME_HIERARCHY_PARAMETER_TYPE_VECTOR4_MAP;
typedef std::map<FRAME_HIERARCHY_PARAMETER_TYPE, ARRAY<int> >	FRAME_HIERARCHY_PARAMETER_TYPE_INT_ARRAY_MAP;
//#define FRAME_HIERARCHY_PARAMETER_TYPE_INT_MAP			std::map<FRAME_HIERARCHY_PARAMETER_TYPE, int>
//#define FRAME_HIERARCHY_PARAMETER_TYPE_BOOL_MAP			std::map<FRAME_HIERARCHY_PARAMETER_TYPE, bool>
//#define FRAME_HIERARCHY_PARAMETER_TYPE_FLOAT_MAP		std::map<FRAME_HIERARCHY_PARAMETER_TYPE, float>
//#define FRAME_HIERARCHY_PARAMETER_TYPE_VECTOR4_MAP		std::map<FRAME_HIERARCHY_PARAMETER_TYPE, D3DXVECTOR4>
//#define FRAME_HIERARCHY_PARAMETER_TYPE_INT_ARRAY_MAP	std::map<FRAME_HIERARCHY_PARAMETER_TYPE, ARRAY<int>>
// ****************************************************************
// D3Dフレーム階層パラメータ設定用クラス
// ================================================================
class C_d3d_frame_hierarchy_parameter_setter {
	friend C_d3d_frame_set;
private:
	// 列挙型はintとして処理する
	FRAME_HIERARCHY_PARAMETER_TYPE_INT_MAP			int_map;
	FRAME_HIERARCHY_PARAMETER_TYPE_BOOL_MAP			bool_map;
	FRAME_HIERARCHY_PARAMETER_TYPE_FLOAT_MAP		float_map;
	FRAME_HIERARCHY_PARAMETER_TYPE_VECTOR4_MAP		vector4_map;
	FRAME_HIERARCHY_PARAMETER_TYPE_INT_ARRAY_MAP	int_array_map;
public:
	// 設定用(フレームに対する)のパラメータをこのクラスにセットする
	void set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, int data);
	void set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, bool data);
	void set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, float data);
	void set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, const D3DXVECTOR4& data);
	void set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, const ARRAY<int>& data);
private:
	// 実際にフレームにパラメータを設定する(このクラスにセットされたものを)
	// C_d3d_frame_set以外からは呼び出せないようにした(破綻するようならまた考える)
	void set_up_parameter_to_frame(C_d3d_frame* p_frame);
};

// 使い方の例
// ルートフレーム以下すべてにパラメータを設定する場合
// C_d3d_frame_hierarchy_parameter_setter setter;
// setter.set_parameter_for_set_up(MP__LIGHTING_TYPE, (int)LT_LAMBERT);
// setter.set_parameter_for_set_up(てきとう);
// C_d3d_frame_set fs;
// fs.set_parameter_to_target_and_target_below_child(m_frame_root.get(), &setter);


}