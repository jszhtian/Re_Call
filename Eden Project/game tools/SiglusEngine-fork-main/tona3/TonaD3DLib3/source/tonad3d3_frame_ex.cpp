#include	"tonad3d3_pch.h"
#include	"tonad3d3_frame_ex.h"

namespace NT3
{

#define		SAFE_DELETE(p)			do	{	if (p)	{	delete (p);		(p) = NULL;	}	} while(0)
#define		SAFE_DELETE_ARRAY(p)	do	{	if (p)	{	delete[] (p);	(p) = NULL;	}	} while(0)
#define		SAFE_RELEASE(p)			do	{	if (p)	{	(p)->Release(); (p) = NULL;	}	} while(0)

int get_size_from_d3ddecltype(BYTE type)
{
	int size = 0;
	switch(type) {
		case D3DDECLTYPE_FLOAT1		:	size = 4;	break;
		case D3DDECLTYPE_FLOAT2		:	size = 8;	break;  
		case D3DDECLTYPE_FLOAT3		:	size = 12;	break;
		case D3DDECLTYPE_FLOAT4		:	size = 16;	break;
		case D3DDECLTYPE_D3DCOLOR	:	size = 16;	break;
		case D3DDECLTYPE_UBYTE4		:	size = 4;	break;
		case D3DDECLTYPE_SHORT2		:	size = 2;	break;
		case D3DDECLTYPE_SHORT4		:	size = 4;	break;
		default						:	size = 0;	break;

	// The following types are valid only with vertex shaders >= 2.0
	//D3DDECLTYPE_UBYTE4N   =  8,  // Each of 4 bytes is normalized by dividing to 255.0
	//D3DDECLTYPE_SHORT2N   =  9,  // 2D signed short normalized (v[0]/32767.0,v[1]/32767.0,0,1)
	//D3DDECLTYPE_SHORT4N   = 10,  // 4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0)
	//D3DDECLTYPE_USHORT2N  = 11,  // 2D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,0,1)
	//D3DDECLTYPE_USHORT4N  = 12,  // 4D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,v[2]/65535.0,v[3]/65535.0)
	//D3DDECLTYPE_UDEC3     = 13,  // 3D unsigned 10 10 10 format expanded to (value, value, value, 1)
	//D3DDECLTYPE_DEC3N     = 14,  // 3D signed 10 10 10 format normalized and expanded to (v[0]/511.0, v[1]/511.0, v[2]/511.0, 1)
	//D3DDECLTYPE_FLOAT16_2 = 15,  // Two 16-bit floating point values, expanded to (value, value, 0, 1)
	//D3DDECLTYPE_FLOAT16_4 = 16,  // Four 16-bit floating point values
	//D3DDECLTYPE_UNUSED    = 17,  // When the type field in a decl is unused.
	}

	return size;
}

// ********************************************************************************************************************************
// D3D フレーム階層
// ================================================================================================================================

// ****************************************************************
// D3D フレーム階層：フレームを構築する
// ================================================================
HRESULT C_d3d_allocate_hierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	// フレームを作成
	S_d3d_frame* p_frame = new S_d3d_frame;
	// 全部0で初期化する
	ZeroMemory(p_frame, sizeof(S_d3d_frame));

	// 名前をコピーする(関数内部でnewしてる)
	p_frame->Name = allocate_name(Name);

	// 行列初期化
    D3DXMatrixIdentity(&p_frame->TransformationMatrix);			// 変換行列
    D3DXMatrixIdentity(&p_frame->CombinedTransformationMatrix);	// 合成済みの変換行列

	// 新しいフレームを戻り値に設定する
	*ppNewFrame = (D3DXFRAME *)p_frame;

	return D3D_OK;
}

// ****************************************************************
// D3D フレーム階層：フレームを解放する
// ================================================================
HRESULT C_d3d_allocate_hierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	//SAFE_DELETE(pFrameToFree->pMeshContainer);	// メッシュコンテナ
	//SAFE_DELETE_ARRAY(pFrameToFree->Name);		// 名前

	return D3D_OK;
}

// ****************************************************************
// D3D フレーム階層：メッシュコンテナを構築する
// ================================================================
HRESULT C_d3d_allocate_hierarchy::CreateMeshContainer(
		LPCSTR Name, 
		CONST D3DXMESHDATA *pMeshData, 
		CONST D3DXMATERIAL *pMaterials, 
		CONST D3DXEFFECTINSTANCE *pEffectInstances, 
		DWORD NumMaterials, 
		CONST DWORD *pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer
		)
{
	HRESULT hr;

	// 分かりやすくメッシュデータへのポインタを取得しておく
	ID3DXMesh* p_mesh = pMeshData->pMesh;

	// ▲通常のメッシュ以外は未対応
	if (pMeshData->Type != D3DXMESHTYPE_MESH)	{
		set_last_error(_T("プログレッシブメッシュやパッチメッシュには対応していません。"), _T("CreateMeshContainer"));
		return E_FAIL;
	}

	// ▲FVF が設定されていないメッシュは未対応
	if (p_mesh->GetFVF() == 0)	{
		set_last_error(_T("FVF が設定されていないメッシュは未対応です。"), _T("CreateMeshContainer"));
		return E_FAIL;
	}

	// 新しいメッシュコンテナを作る
	/*
	D3DXMESHCONTAINER* p_mc = NULL;
	// スキンメッシュの場合の処理
	if(pSkinInfo) {
		p_mc = new S_d3d_skinned_mesh_container;
		ZeroMemory(p_mc, sizeof(S_d3d_skinned_mesh_container));
	}
	// スキンメッシュ以外の場合の処理
	else {
		p_mc = new D3DXMESHCONTAINER;
		ZeroMemory(p_mc, sizeof(D3DXMESHCONTAINER));
	}
	*/
	S_d3d_skinned_mesh_container* p_mc = NULL;
	p_mc = new S_d3d_skinned_mesh_container;
	ZeroMemory(p_mc, sizeof(S_d3d_skinned_mesh_container));

	// 名前をコピー
	p_mc->Name = allocate_name(Name);



	// メッシュ情報を取得する(FVFを使わない版)
	D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
	LPD3DVERTEXELEMENT9 pDeclCur;
	hr = p_mesh->GetDeclaration(pDecl);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("GetDeclaration に失敗しました。"), _T("GetDeclaration"), hr);
		return hr;
	}
	pDeclCur = pDecl;
	// メッシュ情報を実際に調べる
	bool have_normal = false;		// 法線ベクトル情報
	bool have_tangent = false;		// 接ベクトル情報
									// 接ベクトルはテクスチャが張られたモデルに関する法線ベクトルを計算する
									// つまり、テクスチャを張らないモデルでは動かない
	bool have_binormal = false;		// 従法線ベクトル情報
	bool have_texcoord = false;
	while (pDeclCur->Stream != 0xff) {
		// 法線ベクトル情報の有無
		if ((pDeclCur->Usage == D3DDECLUSAGE_NORMAL) && (pDeclCur->UsageIndex == 0))
			have_normal = true;
		// 接ベクトル情報の有無
		if ((pDeclCur->Usage == D3DDECLUSAGE_TANGENT) && (pDeclCur->UsageIndex == 0))
			have_tangent = true;
		// 従法線ベクトル情報の有無
		if ((pDeclCur->Usage == D3DDECLUSAGE_BINORMAL) && (pDeclCur->UsageIndex == 0))
			have_binormal = true;
		// の有無
		if ((pDeclCur->Usage == D3DDECLUSAGE_TEXCOORD) && (pDeclCur->UsageIndex == 0))
			have_texcoord = true;

		// インデックスを進める
		pDeclCur++;
	}
	// いずれかの情報が欠けていれば追加する
	if(!have_normal || !have_tangent || !have_binormal || !have_texcoord) {
		p_mc->MeshData.Type = D3DXMESHTYPE_MESH;

		// 新しい頂点フォーマット
		//D3DVERTEXELEMENT9 new_decl[MAX_FVF_DECL_SIZE];
		ARRAY<D3DVERTEXELEMENT9> new_decl;
		// まずは既存の情報を追加する
		pDeclCur = pDecl;
		WORD last_offset;
		BYTE last_type;
		while (pDeclCur->Stream != 0xff) {
			new_decl.push_back(*pDeclCur);
			last_offset = pDeclCur->Offset;
			last_type = pDeclCur->Type;
			// インデックスを進める
			pDeclCur++;
		}
		// 頂点フォーマットの追加
		if(!have_normal) {
			D3DVERTEXELEMENT9 add_decl;
			//add_decl = D3DVERTEXELEMENT9(0, last_offset + get_size_from_d3ddecltype(last_type), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0);
			add_decl.Stream = 0;
			add_decl.Offset = last_offset + get_size_from_d3ddecltype(last_type);
			add_decl.Type = D3DDECLTYPE_FLOAT3;
			add_decl.Method = D3DDECLMETHOD_DEFAULT;
			add_decl.Usage = D3DDECLUSAGE_NORMAL;
			add_decl.UsageIndex = 0;
			last_offset = last_offset + get_size_from_d3ddecltype(last_type);
			last_type = D3DDECLTYPE_FLOAT3;
			new_decl.push_back(add_decl);
		}
		if(!have_tangent) {
			D3DVERTEXELEMENT9 add_decl;
			//add_decl = D3DVERTEXELEMENT9(0, last_offset + get_size_from_d3ddecltype(last_type), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0);
			add_decl.Stream = 0;
			add_decl.Offset = last_offset + get_size_from_d3ddecltype(last_type);
			add_decl.Type = D3DDECLTYPE_FLOAT3;
			add_decl.Method = D3DDECLMETHOD_DEFAULT;
			add_decl.Usage = D3DDECLUSAGE_TANGENT;
			add_decl.UsageIndex = 0;
			last_offset = last_offset + get_size_from_d3ddecltype(last_type);
			last_type = D3DDECLTYPE_FLOAT3;
			new_decl.push_back(add_decl);
		}
		if(!have_binormal) {
			D3DVERTEXELEMENT9 add_decl;
			//add_decl = D3DVERTEXELEMENT9(0, last_offset + get_size_from_d3ddecltype(last_type), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0);
			add_decl.Stream = 0;
			add_decl.Offset = last_offset + get_size_from_d3ddecltype(last_type);
			add_decl.Type = D3DDECLTYPE_FLOAT3;
			add_decl.Method = D3DDECLMETHOD_DEFAULT;
			add_decl.Usage = D3DDECLUSAGE_BINORMAL;
			add_decl.UsageIndex = 0;
			last_offset = last_offset + get_size_from_d3ddecltype(last_type);
			last_type = D3DDECLTYPE_FLOAT3;
			new_decl.push_back(add_decl);
		}
		if(!have_texcoord) {
			D3DVERTEXELEMENT9 add_decl;
			//add_decl = D3DVERTEXELEMENT9(0, last_offset + get_size_from_d3ddecltype(last_type), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0);
			add_decl.Stream = 0;
			add_decl.Offset = last_offset + get_size_from_d3ddecltype(last_type);
			add_decl.Type = D3DDECLTYPE_FLOAT2;
			add_decl.Method = D3DDECLMETHOD_DEFAULT;
			add_decl.Usage = D3DDECLUSAGE_TEXCOORD;
			add_decl.UsageIndex = 0;
			last_offset = last_offset + get_size_from_d3ddecltype(last_type);
			last_type = D3DDECLTYPE_FLOAT2;
			new_decl.push_back(add_decl);
		}
		// #define D3DDECL_END() {0xFF,0,D3DDECLTYPE_UNUSED,0,0,0}
		D3DVERTEXELEMENT9 add_decl;
		//add_decl = D3DVERTEXELEMENT9(0, last_offset + get_size_from_d3ddecltype(last_type), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0);
		add_decl.Stream = 0xFF;
		add_decl.Offset = 0;
		add_decl.Type = D3DDECLTYPE_UNUSED;
		add_decl.Method = 0;
		add_decl.Usage = 0;
		add_decl.UsageIndex = 0;
		new_decl.push_back(add_decl);

		// 新しい頂点フォーマットのメッシュを生成する
		hr = p_mesh->CloneMesh(p_mesh->GetOptions(), new_decl.get(), G_d3d.device.body(), &p_mc->MeshData.pMesh);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("メッシュの複製に失敗しました。"), _T("CloneMesh"), hr);
			return E_FAIL;
		}

		// メッシュへのポインタを再取得
		p_mesh = p_mc->MeshData.pMesh;

		// 法線ベクトルを算出する
		hr = D3DXComputeNormals(p_mesh, NULL);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("D3DXComputeNormals"), hr);
			return E_FAIL;
		}
		// 接ベクトルと従法線ベクトルを算出する
		hr = D3DXComputeTangent(p_mesh, 0, 0, 0, TRUE, NULL);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("D3DXComputeTangent"), hr);
			return E_FAIL;
		}
	}
	// 全ての情報があればそのままコピーする
	else {
		// メッシュ（へのポインタ）をコピーする
		p_mc->MeshData.Type = D3DXMESHTYPE_MESH;
		p_mc->MeshData.pMesh = p_mesh;
		p_mesh->AddRef();	// 参照カウンタを増やす！
	}
	//// 頂点デクラレーションの生成
	//D3DVERTEXELEMENT9 pFinDecl[MAX_FVF_DECL_SIZE];
	//hr = p_mesh->GetDeclaration(pFinDecl);
	//if (FAILED(hr))	{
	//	set_last_error_d3d(_T("GetDeclaration に失敗しました。"), _T("GetDeclaration"), hr);
	//	return hr;
	//}
	//G_d3d.device.body()->CreateVertexDeclaration(pFinDecl, &p_mc->pDecl);
	//if (FAILED(hr))	{
	//	set_last_error_d3d(_T("CreateVertexDeclaration に失敗しました。"), _T("CreateVertexDeclaration"), hr);
	//	return hr;
	//}

	//// メッシュ情報を更新し反映させる
	//hr = p_mesh->UpdateSemantics(pDecl);
	//if (FAILED(hr))	{
	//	set_last_error_d3d(_T("UpdateSemantics に失敗しました。"), _T("UpdateSemantics"), hr);
	//	return hr;
	//}



	//// メッシュデータ
	//// 法線がある場合はそのままコピーする
	//if (p_mesh->GetFVF() & D3DFVF_NORMAL)	{
	//	// 接ベクトルを算出する(主にバンプマップ用)
	//	//hr = D3DXComputeTangent(p_mesh, 0, 0, 0, TRUE, NULL);
	//	//if(FAILED(hr)) { 
	//	//	set_last_error_d3d(_T("D3DXComputeTangent"), hr);
	//	//}

	//	// メッシュ（へのポインタ）をコピーする
	//	p_mc->MeshData.Type = D3DXMESHTYPE_MESH;
	//	p_mc->MeshData.pMesh = p_mesh;
	//	p_mesh->AddRef();	// 参照カウンタを増やす！
	//}
	//// 法線がない場合は法線を計算する
	//else	{
	//	p_mc->MeshData.Type = D3DXMESHTYPE_MESH;

	//	// メッシュに法線がなかったので追加する
	//	hr = p_mesh->CloneMeshFVF(p_mesh->GetOptions(), p_mesh->GetFVF() | D3DFVF_NORMAL, G_d3d.device.body(), &p_mc->MeshData.pMesh);
	//	if (FAILED(hr))	{
	//		set_last_error_d3d(_T("メッシュの複製に失敗しました。"), _T("CloneMeshFVF"), hr);
	//		return E_FAIL;
	//	}

	//	// メッシュへのポインタを再取得
	//	p_mesh = p_mc->MeshData.pMesh;

	//	// 実際に法線を計算する
	//	D3DXComputeNormals(p_mesh, NULL);

	//	// 接ベクトルを算出する(主にバンプマップ用)
	//	//hr = D3DXComputeTangent(p_mesh, 0, 0, 0, TRUE, NULL);
	//	//if(FAILED(hr)) { 
	//	//	set_last_error_d3d(_T("D3DXComputeTangent"), hr);
	//	//}
	//}


	// 面の数を取得しておく
	int face_cnt = p_mesh->GetNumFaces();

	// マテリアル数の設定(最低1個は確保する)
	p_mc->NumMaterials = std::max<DWORD>(1, NumMaterials);
	// マテリアル配列の確保
	p_mc->pMaterials = new D3DXMATERIAL[p_mc->NumMaterials];

	// 隣接性バッファの確保
	p_mc->pAdjacency = new DWORD[face_cnt * 3];
	// 隣接性バッファをコピーする
	memcpy(p_mc->pAdjacency, pAdjacency, sizeof(DWORD) * face_cnt * 3);

	// マテリアルがあれば
	if(NumMaterials > 0) {
		// マテリアル配列をコピーする
		memcpy(p_mc->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for(int i = 0; i < (int)NumMaterials; i++) {
			if(pMaterials[i].pTextureFilename) {
				// テクスチャ生成は C_d3d_frame::create_frame とかで名前から行うので名前をコピーしておく
				p_mc->pMaterials[i].pTextureFilename = allocate_name(pMaterials[i].pTextureFilename);
			}
			// Ｘファイルではアンビエントは定義できないので、ディフューズをコピーしておく/*要修正*/
			p_mc->pMaterials[i].MatD3D.Ambient = p_mc->pMaterials[i].MatD3D.Diffuse;
		}
	}
	// マテリアルがなければ
	else {
		// 適当な値を入れる

	}


	// エフェクト
	/*
	p_mc->pEffects = new D3DXEFFECTINSTANCE;
	ZeroMemory(p_mc->pEffects, sizeof(D3DXEFFECTINSTANCE));
	if (pEffectInstances->pEffectFilename)	{
		dst->pEffects->pEffectFilename = new char[strlen(pEffectInstances->pEffectFilename) + 1];
		strcpy_s(dst->pEffects->pEffectFilename, strlen(pEffectInstances->pEffectFilename) + 1, pEffectInstances->pEffectFilename);
	}
	dst->pEffects->NumDefaults = pEffectInstances->NumDefaults;
	dst->pEffects->pDefaults = new D3DXEFFECTDEFAULT[pEffectInstances->NumDefaults];
	for (int i = 0; i < (int)pEffectInstances->NumDefaults; i++)	{
		D3DXEFFECTDEFAULT* dst_default = &dst->pEffects->pDefaults[i];
		D3DXEFFECTDEFAULT* src_default = &pEffectInstances->pDefaults[i];

		dst_default->pParamName = new char[strlen(src_default->pParamName) + 1];
		strcpy_s(dst_default->pParamName, strlen(src_default->pParamName) + 1, src_default->pParamName);
		dst_default->NumBytes = src_default->NumBytes;
		dst_default->Type = src_default->Type;
		if (src_default->Type <= D3DXEDT_DWORD)	{
			dst_default->pValue = new BYTE[src_default->NumBytes];
			memcpy(dst_default->pValue, src_default->pValue, src_default->NumBytes);
		}
	}
	*/

	// 次のメッシュコンテナ(どのサンプルを見てもDrawFrameで使っているのに(NULLチェックではじかれる)初期化では何もしていない)
	// ZeroMemoryしてるけどコメントを残すためにNULL代入しておく
	p_mc->pNextMeshContainer = NULL;

	// スキン
	if (pSkinInfo)	{

		// スキン（へのポインタ）をコピー
		p_mc->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();			// 参照カウンタを増やす！

		/*
		// ダウンキャストを行う(Cキャストを使っているのはコンパイルが通らなかったから)
		// classじゃないせいかコンパイルオプションなのかは不明(そのうちテストする)
		S_d3d_skinned_mesh_container* p_smc = (S_d3d_skinned_mesh_container*)p_mc;

		// 元のメッシュ（へのポインタ）をコピー
		p_smc->pOrigMesh = p_mesh;
		p_mesh->AddRef();				// 参照カウンタを増やす！

		// スキンメッシュを一般化する？
		hr = generate_skinned_mesh(p_smc);
		*/

		// 元のメッシュ（へのポインタ）をコピー
		p_mc->pOrigMesh = p_mesh;
		p_mesh->AddRef();				// 参照カウンタを増やす！
		// スキンメッシュを一般化する？
		hr = generate_skinned_mesh(p_mc);

		if(FAILED(hr)) {
			return hr;
		}
	}

	// ローカルに生成したメッシュコンテナを戻り値に設定する
	*ppNewMeshContainer = p_mc;

	return D3D_OK;
}


// ****************************************************************
// メッシュ：フレーム階層
// ================================================================
HRESULT C_d3d_allocate_hierarchy::generate_skinned_mesh(S_d3d_skinned_mesh_container *p_mc)
{
	HRESULT hr = S_OK;

	// スキンがない場合何もしない
	if (!p_mc->pSkinInfo)
		return E_FAIL;	//return S_OK;

	// メッシュとボーンコンビネーションバッファは必要ないので解放する(解放というか初期化？作ってないっぽい)
	SAFE_RELEASE(p_mc->MeshData.pMesh);
	SAFE_RELEASE(p_mc->pBoneCombinationBuf);


	// 行列パレット スキニングに使用可能なボーン行列の数を決める
	UINT MaxMatrices = 26; 
	DWORD NumBones = p_mc->pSkinInfo->GetNumBones();
	p_mc->NumPaletteEntries = std::min<DWORD>(MaxMatrices, NumBones);

	// メッシュを受け取り、頂点単位のブレンドの重みとインデックスとボーンの組み合わせテーブルを
	// 適用した新しいメッシュを返します。このテーブルは、ボーン パレットとメッシュのサブセットの関係を表します。
	hr = p_mc->pSkinInfo->ConvertToIndexedBlendedMesh(
							p_mc->pOrigMesh,							// 入力メッシュ
							D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE, // 現在は使われていません
							p_mc->NumPaletteEntries,					// 行列パレット スキニングに使用可能なボーン行列の数
							p_mc->pAdjacency,							// 入力メッシュの隣接性情報
							NULL,										// 出力メッシュの隣接性情報
							NULL,										// 各面の新しいインデックスを格納する転送先バッファへのポインタ。NULL でもかまいません。
							NULL,										// 各頂点の新しいインデックスを格納する、ID3DXBuffer インターフェイスへのポインタのアドレス。NULL でもかまいません。
							&p_mc->NumInfl,								// 1つの頂点に影響するボーンの最大数
							&p_mc->NumAttributeGroups,					// ボーンコンビネーションの数
							&p_mc->pBoneCombinationBuf,					// ボーンコンビネーションテーブルというボーンと頂点の対応表の配列
							&p_mc->MeshData.pMesh						// スキンメッシュ用に最適化される出力メッシュ
						   );
	if (FAILED(hr))	{
		set_last_error_d3d(_T("ConvertToIndexedBlendedMesh に失敗しました。"), _T("ConvertToIndexedBlendedMesh"), hr);
		return hr;
	}



	//// FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
	//// D3DFVF_TEX1 が必須なのはおかしいかも/*要修正*/
	//DWORD NewFVF = (p_mc->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
	//if (NewFVF != p_mc->MeshData.pMesh->GetFVF()) {
	//	LPD3DXMESH temp_mesh = NULL;
	//	hr = p_mc->MeshData.pMesh->CloneMeshFVF(p_mc->MeshData.pMesh->GetOptions(), NewFVF, G_d3d.device.body(), &temp_mesh);
	//	if(FAILED(hr)) {
	//		set_last_error_d3d(_T("CloneMeshFVF に失敗しました。"), _T("CloneMeshFVF"), hr);
	//		return hr;
	//	}
	//	else {
	//		p_mc->MeshData.pMesh->Release();
	//		p_mc->MeshData.pMesh = temp_mesh;
	//		temp_mesh = NULL;
	//	}
	//}
	//DWORD fvf = p_mc->MeshData.pMesh->GetFVF();

	

	// Geforce3より後はUBYTE4に対応してるっぽい(D3DCOLORへの変換不要)ので下のソースはコメントアウトしておく
	/*
    // Interpret the UBYTE4 as a D3DCOLOR.
    // The GeForce3 doesn't support the UBYTE4 decl type.  So, we convert any
    // blend indices to a D3DCOLOR semantic, and later in the shader convert
    // it back using the D3DCOLORtoUBYTE4() intrinsic.  Note that we don't
    // convert any data, just the declaration.
	D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
	LPD3DVERTEXELEMENT9 pDeclCur;
	hr = p_mc->MeshData.pMesh->GetDeclaration(pDecl);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("GetDeclaration に失敗しました。"), _T("GetDeclaration"), hr);
		return hr;
	}

	// the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
	//   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
	//          this is more of a "cast" operation
	pDeclCur = pDecl;
	while (pDeclCur->Stream != 0xff) {
		if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
			pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
		pDeclCur++;
	}

	hr = p_mc->MeshData.pMesh->UpdateSemantics(pDecl);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("UpdateSemantics に失敗しました。"), _T("UpdateSemantics"), hr);
		return hr;
	}
	*/

	// allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
	if( NumBones > 0 ) {
		// 行列リストを確保する
		p_mc->SkinnedBlendMatrices.reserve(NumBones);
		p_mc->SkinnedBlendMatrices.resize(NumBones);
	}


	/*
	// If the device can only do 2 matrix blends, ConvertToBlendedMesh cannot approximate all meshes to it
	// Thus we split the mesh in two parts: The part that uses at most 2 matrices and the rest. The first is
	// drawn using the device's HW vertex processing and the rest is drawn using SW vertex processing.
	D3DXBONECOMBINATION* bone_combination = (D3DXBONECOMBINATION*)p_mc->pBoneCombinationBuf->GetBufferPointer();

	// １つの頂点に影響するボーンの最大数を調べ、
	// デバイスの能力を超えていないかどうかを判定する
	int group_no = 0;
	for (group_no = 0; group_no < (int)p_mc->NumAttributeGroups; group_no ++)	{

		// 影響するボーンの数を調べる
		DWORD infl_cnt = 0;
		for (DWORD infl_no = 0; infl_no < p_mc->NumInfl; infl_no++)	{
			if (bone_combination[group_no].BoneId[infl_no] != UINT_MAX)
				++infl_cnt;
		}

		// デバイスの能力を超えたボーン数が見つかれば終了
		//if (infl_cnt > G_d3d.device.device_caps().MaxVertexBlendMatrices)
		//	break;
	}

	// デバイスの能力を超えたボーン数が見つかった場合、
	// メッシュをソフトウェア描画にする
	if (group_no < (int)p_mc->NumAttributeGroups)	{
		ID3DXMesh* p_mesh_tmp = NULL;
		hr = p_mc->MeshData.pMesh->CloneMeshFVF(D3DXMESH_SOFTWAREPROCESSING | p_mc->MeshData.pMesh->GetOptions(), p_mc->MeshData.pMesh->GetFVF(), G_d3d.device.body(), &p_mesh_tmp);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("CloneMeshFVF に失敗しました。"), _T("CloneMeshFVF"), hr);
			return hr;
		}

		// 新しいメッシュに入れ替える
		p_mc->MeshData.pMesh->Release();
		p_mc->MeshData.pMesh = p_mesh_tmp;
		p_mesh_tmp = NULL;
	}
	*/

	return S_OK;
}

// ****************************************************************
// D3D フレーム階層：メッシュコンテナを解放する
// ================================================================
HRESULT C_d3d_allocate_hierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	// スキンメッシュの場合
	if (pMeshContainerToFree->pSkinInfo) {
		destroy_skinned_mesh_container((S_d3d_skinned_mesh_container*)pMeshContainerToFree);
	}

	// 名前
	SAFE_DELETE_ARRAY(pMeshContainerToFree->Name);

	// 頂点デクラレーション
	//SAFE_RELEASE(((S_d3d_skinned_mesh_container*)pMeshContainerToFree)->pDecl);

	// メッシュ
	SAFE_RELEASE(pMeshContainerToFree->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainerToFree->MeshData.pPMesh);
	SAFE_RELEASE(pMeshContainerToFree->MeshData.pPatchMesh);

	// マテリアル
	for (int i = 0; i < (int)pMeshContainerToFree->NumMaterials; i++) {
		SAFE_DELETE_ARRAY(pMeshContainerToFree->pMaterials[i].pTextureFilename);
	}
	SAFE_DELETE_ARRAY(pMeshContainerToFree->pMaterials);
	SAFE_DELETE_ARRAY(pMeshContainerToFree->pAdjacency);

	// エフェクト
	/*
	for (int i = 0; i < (int)pMeshContainerToFree->pEffects->NumDefaults; i++)	{
		SAFE_RELEASE_ARRAY(pMeshContainerToFree->pEffects->pDefaults[i].pParamName);
		SAFE_RELEASE_ARRAY(pMeshContainerToFree->pEffects->pDefaults[i].pValue);
	}
	SAFE_RELEASE_ARRAY(pMeshContainerToFree->pEffects->pDefaults);
	SAFE_RELEASE_ARRAY(pMeshContainerToFree->pEffects->pEffectFilename);
	SAFE_RELEASE(pMeshContainerToFree->pEffects);
	*/

	// 元ソースにはないから様子見
	//SAFE_DELETE(pMeshContainerToFree);
	
	return D3D_OK;
}

// ****************************************************************
// D3D フレーム階層：スキンメッシュコンテナ用のデータのみを解放する
// ================================================================
HRESULT C_d3d_allocate_hierarchy::destroy_skinned_mesh_container(S_d3d_skinned_mesh_container* p_mc)
{
	SAFE_RELEASE(p_mc->pOrigMesh);
	SAFE_RELEASE(p_mc->pBoneCombinationBuf);
	p_mc->SkinnedBlendMatrices.clear();

	SAFE_RELEASE(p_mc->pSkinInfo);

	return D3D_OK;
}

// ****************************************************************
// D3D フレーム階層：名前用の領域を確保
// ================================================================
char* C_d3d_allocate_hierarchy::allocate_name(const char* src)
{
	if (!src)
		return NULL;

	int len = (int)strlen(src);

	char* dst = new char[len + 1];
	strcpy_s(dst, len + 1, src);
	return dst;
}






// ********************************************************************************************************************************
// フレーム
// ================================================================================================================================

// ****************************************************************
// フレームを作成する
// ================================================================
bool C_d3d_frame::create_frame(C_d3d_frame* root_frame, S_d3d_frame* p_frame, ARRAY< BSP<C_d3d_mesh> >& mesh_list_ret, ARRAY<TSTR>& texture_name_list_ret)
{
	// フレームへのポインタを記憶しておく
	m_frame = p_frame;

	// メッシュがある場合！
	S_d3d_skinned_mesh_container* p_mc = (S_d3d_skinned_mesh_container*)p_frame->pMeshContainer;
	while (p_mc)	{

		// 新しくメッシュを作成
		BSP<C_d3d_mesh> mesh(new C_d3d_mesh(p_mc->MeshData.pMesh));

		// マテリアルを設定
		for (int i = 0; i < (int)p_mc->NumMaterials; i++)	{

			// マテリアルを追加
			C_d3d_mtrl mtrl;
			mtrl.mtrl = p_mc->pMaterials[i].MatD3D;
			mesh->add_material(mtrl);

			// テクスチャ名リストにテクスチャ名を追加
			MBSTR name = p_mc->pMaterials[i].pTextureFilename ? p_mc->pMaterials[i].pTextureFilename : "";
			texture_name_list_ret.push_back(MBSTR_to_TSTR(name));
		}

		// スキン
		if (m_frame->pMeshContainer->pSkinInfo)	{
			ID3DXSkinInfo* skin = m_frame->pMeshContainer->pSkinInfo;

			// ボーンオフセット行列のコピー
			m_offset_matrix_list.resize(skin->GetNumBones());
			for (int i = 0; i < (int)skin->GetNumBones(); i++)
				memcpy(&m_offset_matrix_list[i], skin->GetBoneOffsetMatrix(i), sizeof(D3DXMATRIX));

			// ボーン行列のコピー
			m_root_matrix_list.resize(skin->GetNumBones());
			for (int i = 0; i < (int)skin->GetNumBones(); i++)	{
				S_d3d_frame* p_parent_frame = (S_d3d_frame*)D3DXFrameFind(root_frame->m_frame, skin->GetBoneName(i));
				m_root_matrix_list[i] = &p_parent_frame->CombinedTransformationMatrix;
			}
		}

		// メッシュリストにメッシュを追加
		m_mesh_list.push_back(mesh);
		mesh_list_ret.push_back(mesh);

		// 次のメッシュへ
		p_mc = (S_d3d_skinned_mesh_container *)p_mc->pNextMeshContainer;
	}

	// 兄弟がいる場合
	if (p_frame->pFrameSibling)	{
		m_sibling = BSP<C_d3d_frame>(new C_d3d_frame);
		m_sibling->create_frame(root_frame, (S_d3d_frame *)p_frame->pFrameSibling, mesh_list_ret, texture_name_list_ret);
	}

	// 子供がいる場合
	if (p_frame->pFrameFirstChild)	{
		m_first_child = BSP<C_d3d_frame>(new C_d3d_frame);
		m_first_child->create_frame(root_frame, (S_d3d_frame *)p_frame->pFrameFirstChild, mesh_list_ret, texture_name_list_ret);
	}



	return true;
}

// ****************************************************************
// ローカルの変換行列を更新する
// ================================================================
bool C_d3d_frame::update_matrix(const D3DXMATRIX* parent_matrix)
{
	if (!m_frame)
		return false;

	// 描画リストに追加する
	//if(m_frame->pMeshContainer) {
	//	m_draw_list.push_back(m_frame->pMeshContainer);
	//}

	// ワールド変換行列を親と合成
	if (parent_matrix) {
		// 親フレームの変換行列があれば合成する
		D3DXMatrixMultiply(&m_frame->CombinedTransformationMatrix, &m_frame->TransformationMatrix, parent_matrix);
	}
	else {
		// 親フレームの変換行列がなければこのフレームの変換行列にする
		m_frame->CombinedTransformationMatrix = m_frame->TransformationMatrix;
	}

	// 兄弟がいる場合
	if (m_sibling)
		m_sibling->update_matrix(parent_matrix);

	// 子供がいる場合
	if (m_first_child)
		m_first_child->update_matrix(&m_frame->CombinedTransformationMatrix);



	return true;
}







// ********************************************************************************************************************************
// フレームセット
// ================================================================================================================================

// ****************************************************************
// フレーム
// ================================================================
C_d3d_frame_set::C_d3d_frame_set()
{
	init();
}

C_d3d_frame_set::~C_d3d_frame_set()
{
	release();
}

void C_d3d_frame_set::init()
{
	release();
}

void C_d3d_frame_set::release()
{
	if (m_frame_root)	{
		D3DXFrameDestroy(m_frame_root->body(), &m_allocate_hierarchy);
		m_frame_root.reset();
	}

//	animation_controller = NULL;
}


// ****************************************************************
// Ｘファイルからメッシュを読み込む
// ================================================================
bool C_d3d_frame_set::load_x(CTSTR& file_path, ARRAY< BSP<C_d3d_mesh> >& mesh_list, ARRAY<TSTR>& texture_name_list)
{
	HRESULT hr;

	init();

	ID3DXAnimationController* p_animation_controller = NULL;
	S_d3d_frame* p_frame_root = NULL;

	// Ｘファイルから階層を読み込んでみる！
	hr = D3DXLoadMeshHierarchyFromX(file_path.c_str(), D3DXMESH_MANAGED, G_d3d.device.body(), &m_allocate_hierarchy, NULL, (D3DXFRAME **)&p_frame_root, &p_animation_controller);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("メッシュに階層がありませんでした。"), _T("D3DXLoadMeshHierarchyFromX"), hr);
		return false;
	}

	if (p_animation_controller) {
		m_animation_controller.set_animation_controller(p_animation_controller);
//		animation_controller = BIP<ID3DXAnimationController>(p_animation_controller, false);
	}

	// 階層が読めた場合の処理

	// 各フレームを構築していく
	m_frame_root = BSP<C_d3d_frame>(new C_d3d_frame);
	m_frame_root->create_frame(m_frame_root.get(), p_frame_root, mesh_list, texture_name_list);

	// 全メッシュに一括設定の時に使うリストに登録/*要修正*/
	for(int i = 0; i < (int)mesh_list.size(); i++) {
		m_mesh_list_for_parm_set.push_back(mesh_list[i].get());
	}

	return true;
}

// ****************************************************************
// アニメーションを進める
// ================================================================
//bool C_d3d_frame_set::advance_animation(double past_time)
//{
//	if (!animation_controller)
//		return false;
//
//	// アニメーションを進める
//	animation_controller->AdvanceTime(past_time, NULL);
//
//	return true;
//}

// ****************************************************************
// 変換行列を更新する
// ================================================================
bool C_d3d_frame_set::update_matrix(const D3DXMATRIX* matrix)
{
	if (!m_frame_root)
		return false;

	// 変換行列を更新する
	m_frame_root->update_matrix(matrix);

	return true;
}

// ****************************************************************
// アニメーションを進める
// ================================================================
void C_d3d_frame_set::update_animation_controller()
{
	m_animation_controller.update();
}

// ****************************************************************
// フレーム名を使ってこのクラスのフレームセット内からフレームを検索する
// ================================================================
C_d3d_frame* C_d3d_frame_set::search_frame_by_name(MBSTR& frame_name)
{
	// ルートから検索
	C_d3d_frame* p_frame = m_frame_root.get();
	return search_frame_hierarchy_by_name(frame_name, p_frame);
}
// ****************************************************************
// フレーム名を使って引数のフレームセット内からフレームを検索する
// ================================================================
C_d3d_frame* C_d3d_frame_set::search_frame_by_name(MBSTR& frame_name, C_d3d_frame_set* p_frame_set)
{
	// 引数のルートから検索
	C_d3d_frame* p_frame = p_frame_set->m_frame_root.get();
	return search_frame_hierarchy_by_name(frame_name, p_frame);
}
// ****************************************************************
// フレーム名を使ってフレームセット内からフレームを検索する
// ================================================================
C_d3d_frame* C_d3d_frame_set::search_frame_hierarchy_by_name(MBSTR& frame_name, C_d3d_frame* p_frame)
{
	C_d3d_frame* return_frame = NULL;

	if(strcmp(p_frame->body()->Name, frame_name.c_str()) == 0)
		return p_frame;	// 見つかった

	if(p_frame->m_sibling) {
		return_frame = search_frame_hierarchy_by_name(frame_name, p_frame->m_sibling.get());
		if(return_frame)
			return return_frame;
	}

	if(p_frame->m_first_child) {
		return_frame = search_frame_hierarchy_by_name(frame_name, p_frame->m_first_child.get());
		if(return_frame)
			return return_frame;
	}

	return return_frame;
}

// ****************************************************************
// パラメータの設定(対象フレームのみ)
// ================================================================
void C_d3d_frame_set::set_parameter_to_target(C_d3d_frame* p_frame, C_d3d_frame_hierarchy_parameter_setter* p_setter)
{
	if(!p_frame)
		return;

	// 対象フレームに設定
	p_setter->set_up_parameter_to_frame(p_frame);
}
// ****************************************************************
// パラメータの設定(対象フレームと対象フレームの子供以下全て)
// ================================================================
void C_d3d_frame_set::set_parameter_to_target_and_target_below_child(C_d3d_frame* p_frame, C_d3d_frame_hierarchy_parameter_setter* p_setter)
{
	if(!p_frame)
		return;

	// 対象フレームに設定
	p_setter->set_up_parameter_to_frame(p_frame);

	// 対象フレームの子以下に設定していく
	if(p_frame->m_first_child) {
		set_parameter_hierarchy(p_frame->m_first_child.get(), p_setter);
	}

	return;
}
// ****************************************************************
// パラメータの設定(対象フレームの子供以下全て。ただし対象フレームを除く)
// ================================================================
void C_d3d_frame_set::set_parameter_to_target_below_child(C_d3d_frame* p_frame, C_d3d_frame_hierarchy_parameter_setter* p_setter)
{
	if(!p_frame)
		return;

	// 対象フレームの子以下に設定していく
	if(p_frame->m_first_child) {
		set_parameter_hierarchy(p_frame->m_first_child.get(), p_setter);
	}

	return;
}
// ****************************************************************
// パラメータの設定
// ================================================================
void C_d3d_frame_set::set_parameter_hierarchy(C_d3d_frame* p_frame, C_d3d_frame_hierarchy_parameter_setter* p_setter)
{
	if(!p_frame)
		return;

	p_setter->set_up_parameter_to_frame(p_frame);

	if(p_frame->m_sibling) {
		set_parameter_hierarchy(p_frame->m_sibling.get(), p_setter);
	}
	if(p_frame->m_first_child) {
		set_parameter_hierarchy(p_frame->m_first_child.get(), p_setter);
	}

	return;
}





// ********************************************************************************************************************************
// 階層構造に対するパラメータ設定用クラスなど
// ================================================================================================================================
// 設定用(フレームに対する)のパラメータをこのクラスにセットする
void C_d3d_frame_hierarchy_parameter_setter::set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, int data)
{
	// 登録済みの場合
	if(int_map.find(type) != int_map.end()) {
		// 削除する
		int_map.erase(type);
	}
	// 問答無用で上書きする(存在したら警告出すぐらいにはするかも)
	int_map.insert(std::make_pair(type, data));
}
void C_d3d_frame_hierarchy_parameter_setter::set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, bool data)
{
	// 登録済みの場合
	if(bool_map.find(type) != bool_map.end()) {
		// 削除する
		bool_map.erase(type);
	}
	// 問答無用で上書きする(存在したら警告出すぐらいにはするかも)
	bool_map.insert(std::make_pair(type, data));
}
void C_d3d_frame_hierarchy_parameter_setter::set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, float data)
{
	// 登録済みの場合
	if(float_map.find(type) != float_map.end()) {
		// 削除する
		float_map.erase(type);
	}
	// 問答無用で上書きする(存在したら警告出すぐらいにはするかも)
	float_map.insert(std::make_pair(type, data));
}
void C_d3d_frame_hierarchy_parameter_setter::set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, const D3DXVECTOR4& data)
{
	// 登録済みの場合
	if(vector4_map.find(type) != vector4_map.end()) {
		// 削除する
		vector4_map.erase(type);
	}
	// 問答無用で上書きする(存在したら警告出すぐらいにはするかも)
	vector4_map.insert(std::make_pair(type, data));
}
void C_d3d_frame_hierarchy_parameter_setter::set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, const ARRAY<int>& data)
{
	// 登録済みの場合
	if(int_array_map.find(type) != int_array_map.end()) {
		// 削除する
		int_array_map.erase(type);
	}
	// 問答無用で上書きする(存在したら警告出すぐらいにはするかも)
	int_array_map.insert(std::make_pair(type, data));
}

// フレームにパラメータを設定する(このクラスにセットされたものを)
void C_d3d_frame_hierarchy_parameter_setter::set_up_parameter_to_frame(C_d3d_frame* p_frame)
{
	FRAME_HIERARCHY_PARAMETER_TYPE_INT_MAP::iterator iit;

	// MP__LIGHTING_TYPE
	iit = int_map.find(MP__LIGHTING_TYPE);
	if(iit != int_map.end()) {
		for(int i = 0; i < (int)p_frame->m_mesh_list.size(); i++) {
			p_frame->m_mesh_list[i].get()->get_mesh_param_ptr()->lighting_type = (LIGHTING_TYPE)iit->second;
		}
	}

	// MP__SHADING_TYPE
	iit = int_map.find(MP__SHADING_TYPE);
	if(iit != int_map.end()) {
		for(int i = 0; i < (int)p_frame->m_mesh_list.size(); i++) {
			p_frame->m_mesh_list[i].get()->get_mesh_param_ptr()->shading_type = (SHADING_TYPE)iit->second;
		}
	}

}






#if 0// バックアップ

// ****************************************************************
// D3D フレーム階層：メッシュコンテナを構築する
// ================================================================
HRESULT C_d3d_allocate_hierarchy::CreateMeshContainer(
		LPCSTR Name, 
		CONST D3DXMESHDATA *pMeshData, 
		CONST D3DXMATERIAL *pMaterials, 
		CONST D3DXEFFECTINSTANCE *pEffectInstances, 
		DWORD NumMaterials, 
		CONST DWORD *pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer
		)
{
	HRESULT hr;

	// 分かりやすくメッシュデータへのポインタを取得しておく
	ID3DXMesh* p_mesh = pMeshData->pMesh;

	// ▲通常のメッシュ以外は未対応
	if (pMeshData->Type != D3DXMESHTYPE_MESH)	{
		set_last_error(_T("プログレッシブメッシュやパッチメッシュには対応していません。"), _T("CreateMeshContainer"));
		return E_FAIL;
	}

	// ▲FVF が設定されていないメッシュは未対応
	if (p_mesh->GetFVF() == 0)	{
		set_last_error(_T("FVF が設定されていないメッシュは未対応です。"), _T("CreateMeshContainer"));
		return E_FAIL;
	}

	// 新しいメッシュコンテナを作る
	/*
	D3DXMESHCONTAINER* p_mc = NULL;
	// スキンメッシュの場合の処理
	if(pSkinInfo) {
		p_mc = new S_d3d_skinned_mesh_container;
		ZeroMemory(p_mc, sizeof(S_d3d_skinned_mesh_container));
	}
	// スキンメッシュ以外の場合の処理
	else {
		p_mc = new D3DXMESHCONTAINER;
		ZeroMemory(p_mc, sizeof(D3DXMESHCONTAINER));
	}
	*/
	S_d3d_skinned_mesh_container* p_mc = NULL;
	p_mc = new S_d3d_skinned_mesh_container;
	ZeroMemory(p_mc, sizeof(S_d3d_skinned_mesh_container));

	// 名前をコピー
	p_mc->Name = allocate_name(Name);

	// メッシュデータ

	// 法線がある場合はそのままコピーする
	if (p_mesh->GetFVF() & D3DFVF_NORMAL)	{
		// 接ベクトルを算出する(主にバンプマップ用)
		//hr = D3DXComputeTangent(p_mesh, 0, 0, 0, TRUE, NULL);
		//if(FAILED(hr)) { 
		//	set_last_error_d3d(_T("D3DXComputeTangent"), hr);
		//}

		// メッシュ（へのポインタ）をコピーする
		p_mc->MeshData.Type = D3DXMESHTYPE_MESH;
		p_mc->MeshData.pMesh = p_mesh;
		p_mesh->AddRef();	// 参照カウンタを増やす！
	}
	// 法線がない場合は法線を計算する
	else	{
		p_mc->MeshData.Type = D3DXMESHTYPE_MESH;

		// メッシュに法線がなかったので追加する
		hr = p_mesh->CloneMeshFVF(p_mesh->GetOptions(), p_mesh->GetFVF() | D3DFVF_NORMAL, G_d3d.device.body(), &p_mc->MeshData.pMesh);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("メッシュの複製に失敗しました。"), _T("CloneMeshFVF"), hr);
			return E_FAIL;
		}

		// メッシュへのポインタを再取得
		p_mesh = p_mc->MeshData.pMesh;

		// 実際に法線を計算する
		D3DXComputeNormals(p_mesh, NULL);

		// 接ベクトルを算出する(主にバンプマップ用)
		//hr = D3DXComputeTangent(p_mesh, 0, 0, 0, TRUE, NULL);
		//if(FAILED(hr)) { 
		//	set_last_error_d3d(_T("D3DXComputeTangent"), hr);
		//}

	}

	// 面の数を取得しておく
	int face_cnt = p_mesh->GetNumFaces();

	// マテリアル数の設定(最低1個は確保する)
	p_mc->NumMaterials = std::max<DWORD>(1, NumMaterials);
	// マテリアル配列の確保
	p_mc->pMaterials = new D3DXMATERIAL[p_mc->NumMaterials];

	// 隣接性バッファの確保
	p_mc->pAdjacency = new DWORD[face_cnt * 3];
	// 隣接性バッファをコピーする
	memcpy(p_mc->pAdjacency, pAdjacency, sizeof(DWORD) * face_cnt * 3);

	// マテリアルがあれば
	if(NumMaterials > 0) {
		// マテリアル配列をコピーする
		memcpy(p_mc->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for(int i = 0; i < (int)NumMaterials; i++) {
			if(pMaterials[i].pTextureFilename) {
				// テクスチャ生成は C_d3d_frame::create_frame とかで名前から行うので名前をコピーしておく
				p_mc->pMaterials[i].pTextureFilename = allocate_name(pMaterials[i].pTextureFilename);
			}
			// Ｘファイルではアンビエントは定義できないので、ディフューズをコピーしておく/*要修正*/
			p_mc->pMaterials[i].MatD3D.Ambient = p_mc->pMaterials[i].MatD3D.Diffuse;
		}
	}
	// マテリアルがなければ
	else {
		// 適当な値を入れる

	}


	// エフェクト
	/*
	p_mc->pEffects = new D3DXEFFECTINSTANCE;
	ZeroMemory(p_mc->pEffects, sizeof(D3DXEFFECTINSTANCE));
	if (pEffectInstances->pEffectFilename)	{
		dst->pEffects->pEffectFilename = new char[strlen(pEffectInstances->pEffectFilename) + 1];
		strcpy_s(dst->pEffects->pEffectFilename, strlen(pEffectInstances->pEffectFilename) + 1, pEffectInstances->pEffectFilename);
	}
	dst->pEffects->NumDefaults = pEffectInstances->NumDefaults;
	dst->pEffects->pDefaults = new D3DXEFFECTDEFAULT[pEffectInstances->NumDefaults];
	for (int i = 0; i < (int)pEffectInstances->NumDefaults; i++)	{
		D3DXEFFECTDEFAULT* dst_default = &dst->pEffects->pDefaults[i];
		D3DXEFFECTDEFAULT* src_default = &pEffectInstances->pDefaults[i];

		dst_default->pParamName = new char[strlen(src_default->pParamName) + 1];
		strcpy_s(dst_default->pParamName, strlen(src_default->pParamName) + 1, src_default->pParamName);
		dst_default->NumBytes = src_default->NumBytes;
		dst_default->Type = src_default->Type;
		if (src_default->Type <= D3DXEDT_DWORD)	{
			dst_default->pValue = new BYTE[src_default->NumBytes];
			memcpy(dst_default->pValue, src_default->pValue, src_default->NumBytes);
		}
	}
	*/

	// 次のメッシュコンテナ(どのサンプルを見てもDrawFrameで使っているのに初期化では何もしていない)
	// ZeroMemoryしてるけどコメントを残すためにNULL代入しておく
	p_mc->pNextMeshContainer = NULL;

	// スキン
	if (pSkinInfo)	{

		// スキン（へのポインタ）をコピー
		p_mc->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();			// 参照カウンタを増やす！

		/*
		// ダウンキャストを行う(Cキャストを使っているのはコンパイルが通らなかったから)
		// classじゃないせいかコンパイルオプションなのかは不明(そのうちテストする)
		S_d3d_skinned_mesh_container* p_smc = (S_d3d_skinned_mesh_container*)p_mc;

		// 元のメッシュ（へのポインタ）をコピー
		p_smc->pOrigMesh = p_mesh;
		p_mesh->AddRef();				// 参照カウンタを増やす！

		// スキンメッシュを一般化する？
		hr = generate_skinned_mesh(p_smc);
		*/

		// 元のメッシュ（へのポインタ）をコピー
		p_mc->pOrigMesh = p_mesh;
		p_mesh->AddRef();				// 参照カウンタを増やす！
		// スキンメッシュを一般化する？
		hr = generate_skinned_mesh(p_mc);

		if(FAILED(hr)) {
			return hr;
		}
	}

	// ローカルに生成したメッシュコンテナを戻り値に設定する
	*ppNewMeshContainer = p_mc;

	return D3D_OK;
}


// ****************************************************************
// メッシュ：フレーム階層
// ================================================================
HRESULT C_d3d_allocate_hierarchy::generate_skinned_mesh(S_d3d_skinned_mesh_container *p_mc)
{
	HRESULT hr = S_OK;

	// スキンがない場合何もしない
	if (!p_mc->pSkinInfo)
		return E_FAIL;	//return S_OK;

	// メッシュとボーンコンビネーションバッファは必要ないので解放する(解放というか初期化？作ってないっぽい)
	SAFE_RELEASE(p_mc->MeshData.pMesh);
	SAFE_RELEASE(p_mc->pBoneCombinationBuf);


	// 行列パレット スキニングに使用可能なボーン行列の数を決める
	UINT MaxMatrices = 26; 
	p_mc->NumPaletteEntries = std::min<DWORD>(MaxMatrices, p_mc->pSkinInfo->GetNumBones());

	// メッシュを受け取り、頂点単位のブレンドの重みとインデックスとボーンの組み合わせテーブルを
	// 適用した新しいメッシュを返します。このテーブルは、ボーン パレットとメッシュのサブセットの関係を表します。
	hr = p_mc->pSkinInfo->ConvertToIndexedBlendedMesh(
							p_mc->pOrigMesh,							// 入力メッシュ
							D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE, // 現在は使われていません
							p_mc->NumPaletteEntries,					// 行列パレット スキニングに使用可能なボーン行列の数
							p_mc->pAdjacency,							// 入力メッシュの隣接性情報
							NULL,										// 出力メッシュの隣接性情報
							NULL,										// 各面の新しいインデックスを格納する転送先バッファへのポインタ。NULL でもかまいません。
							NULL,										// 各頂点の新しいインデックスを格納する、ID3DXBuffer インターフェイスへのポインタのアドレス。NULL でもかまいません。
							&p_mc->NumInfl,								// 1つの頂点に影響するボーンの最大数
							&p_mc->NumAttributeGroups,					// ボーンコンビネーションの数
							&p_mc->pBoneCombinationBuf,					// ボーンコンビネーションテーブルというボーンと頂点の対応表の配列
							&p_mc->MeshData.pMesh						// スキンメッシュ用に最適化される出力メッシュ
						   );
	if (FAILED(hr))	{
		set_last_error_d3d(_T("ConvertToIndexedBlendedMesh に失敗しました。"), _T("ConvertToIndexedBlendedMesh"), hr);
		return hr;
	}


	// FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
	// D3DFVF_TEX1 が必須なのはおかしいかも/*要修正*/
	DWORD NewFVF = (p_mc->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
	if (NewFVF != p_mc->MeshData.pMesh->GetFVF()) {
		LPD3DXMESH temp_mesh = NULL;
		hr = p_mc->MeshData.pMesh->CloneMeshFVF(p_mc->MeshData.pMesh->GetOptions(), NewFVF, G_d3d.device.body(), &temp_mesh);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("CloneMeshFVF に失敗しました。"), _T("CloneMeshFVF"), hr);
			return hr;
		}
		else {
			p_mc->MeshData.pMesh->Release();
			p_mc->MeshData.pMesh = temp_mesh;
			temp_mesh = NULL;
		}
	}
	DWORD fvf = p_mc->MeshData.pMesh->GetFVF();

	

	// Geforce3より後はUBYTE4に対応してるっぽい(D3DCOLORへの変換不要)ので下のソースはコメントアウトしておく
	/*
    // Interpret the UBYTE4 as a D3DCOLOR.
    // The GeForce3 doesn't support the UBYTE4 decl type.  So, we convert any
    // blend indices to a D3DCOLOR semantic, and later in the shader convert
    // it back using the D3DCOLORtoUBYTE4() intrinsic.  Note that we don't
    // convert any data, just the declaration.
	D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
	LPD3DVERTEXELEMENT9 pDeclCur;
	hr = p_mc->MeshData.pMesh->GetDeclaration(pDecl);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("GetDeclaration に失敗しました。"), _T("GetDeclaration"), hr);
		return hr;
	}

	// the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
	//   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
	//          this is more of a "cast" operation
	pDeclCur = pDecl;
	while (pDeclCur->Stream != 0xff) {
		if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
			pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
		pDeclCur++;
	}

	hr = p_mc->MeshData.pMesh->UpdateSemantics(pDecl);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("UpdateSemantics に失敗しました。"), _T("UpdateSemantics"), hr);
		return hr;
	}
	*/

	// allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
	if( p_mc->pSkinInfo->GetNumBones() > 0 ) {
		// 行列リストを確保する
		p_mc->SkinnedBlendMatrices.reserve(p_mc->pSkinInfo->GetNumBones());
		p_mc->SkinnedBlendMatrices.resize(p_mc->pSkinInfo->GetNumBones());
	}


	/*
	// If the device can only do 2 matrix blends, ConvertToBlendedMesh cannot approximate all meshes to it
	// Thus we split the mesh in two parts: The part that uses at most 2 matrices and the rest. The first is
	// drawn using the device's HW vertex processing and the rest is drawn using SW vertex processing.
	D3DXBONECOMBINATION* bone_combination = (D3DXBONECOMBINATION*)p_mc->pBoneCombinationBuf->GetBufferPointer();

	// １つの頂点に影響するボーンの最大数を調べ、
	// デバイスの能力を超えていないかどうかを判定する
	int group_no = 0;
	for (group_no = 0; group_no < (int)p_mc->NumAttributeGroups; group_no ++)	{

		// 影響するボーンの数を調べる
		DWORD infl_cnt = 0;
		for (DWORD infl_no = 0; infl_no < p_mc->NumInfl; infl_no++)	{
			if (bone_combination[group_no].BoneId[infl_no] != UINT_MAX)
				++infl_cnt;
		}

		// デバイスの能力を超えたボーン数が見つかれば終了
		//if (infl_cnt > G_d3d.device.device_caps().MaxVertexBlendMatrices)
		//	break;
	}

	// デバイスの能力を超えたボーン数が見つかった場合、
	// メッシュをソフトウェア描画にする
	if (group_no < (int)p_mc->NumAttributeGroups)	{
		ID3DXMesh* p_mesh_tmp = NULL;
		hr = p_mc->MeshData.pMesh->CloneMeshFVF(D3DXMESH_SOFTWAREPROCESSING | p_mc->MeshData.pMesh->GetOptions(), p_mc->MeshData.pMesh->GetFVF(), G_d3d.device.body(), &p_mesh_tmp);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("CloneMeshFVF に失敗しました。"), _T("CloneMeshFVF"), hr);
			return hr;
		}

		// 新しいメッシュに入れ替える
		p_mc->MeshData.pMesh->Release();
		p_mc->MeshData.pMesh = p_mesh_tmp;
		p_mesh_tmp = NULL;
	}
	*/

	return S_OK;
}

#endif



}
