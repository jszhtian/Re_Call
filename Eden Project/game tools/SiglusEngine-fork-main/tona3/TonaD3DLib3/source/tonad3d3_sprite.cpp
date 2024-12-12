#include	"tonad3d3_pch.h"
#include	"tonad3d3_mesh.h"
#include	"tonad3d3_sprite.h"

namespace NT3
{

// ****************************************************************
// スプライトレンダーパラメータ
// ================================================================
S_d3d_render_param::S_d3d_render_param()
{
	init();
}

// ****************************************************************
// スプライトレンダーパラメータ：初期化
// ================================================================
void S_d3d_render_param::init()
{
	disp = false;					// 表示フラグ
	d2_rect = false;				// ２Ｄ矩形（２Ｄの場合 true）
	d3_rect = false;				// ３Ｄ矩形（３Ｄの場合 true）
	d3_billboard = false;			// ３Ｄビルボード

	size = C_float2(0.0f, 0.0f);	// 大きさ
	size_fit_to_texture = -1;		// 大きさをテクスチャに合わせる

	// 頂点
	pos.x = 0.0f;					// 位置
	pos.y = 0.0f;					// 位置
	pos.z = 0.0f;					// 位置
	center.x = 0.0f;				// 中心
	center.y = 0.0f;				// 中心
	center.z = 0.0f;				// 中心
	scale.x = 1.0f;					// 拡大
	scale.y = 1.0f;					// 拡大
	scale.z = 1.0f;					// 拡大
	rotate.x = 0.0f;				// 回転
	rotate.y = 0.0f;				// 回転
	rotate.z = 0.0f;				// 回転
	quaternion.x = 0.0f;			// ★クォータニオン
	quaternion.y = 0.0f;			// ★クォータニオン
	quaternion.z = 0.0f;			// ★クォータニオン
	quaternion.w = 1.0f;			// ★クォータニオン
	dst_clip_use = false;			// クリッピング
	dst_clip = C_rect(0, 0, 0, 0);	// クリッピング矩形
	src_clip_use = false;			// クリッピング
	src_clip = C_rect(0, 0, 0, 0);	// クリッピング矩形

	// ピクセル
	tr = 255;						// 不透明度
	mono = 0;						// モノクロ度
	reverse = 0;					// 反転度
	bright = 0;						// 明度
	dark = 0;						// 暗度
	color_r = 0;					// 色変化
	color_g = 0;					// 色変化
	color_b = 0;					// 色変化
	color_rate = 0;					// 色変化
	color_add_r = 0;				// 色加算
	color_add_g = 0;				// 色加算
	color_add_b = 0;				// 色加算

	// ピクセル応用
	tone_curve_no = D3D_TONE_CURVE_NONE;	// トーンカーブ番号
	tone_curve_sat = 0;						// トーンカーブ彩度
	light_no = D3D_LIGHT_NONE;				// ライト番号
	mask_x = 0;
	mask_y = 0;

	// 合成その他
	culling = false;						// カリング
	alpha_test = false;						// アルファテスト
	alpha_blend = false;					// アルファブレンド
	use_mask = false;						// マスク
	use_fog = false;						// フォグを使う
	blend_type = E_d3d_blend_type_alpha;	// ブレンドタイプ

	// フィルター
	filter.color = C_argb(255, 255, 255, 255);	// フィルター色
}

// ****************************************************************
// D3D スプライト
// ================================================================
C_d3d_sprite::C_d3d_sprite()
{
	// ＰＯＤなデータを初期化する

	// タイプの初期化
	type = E_d3d_sprite_type_none;

	// プリミティブ関連
	m_vertex_offset = 0;
	m_vertex_cnt = 0;
	m_index_offset = 0;
	m_index_cnt = 0;
	m_primitive_cnt = 0;

	// エフェクト関連
	m_effect = NULL;
	m_effect_technique = NULL;
	m_effect_constant_f[0][0] = 0.0f;
	m_effect_constant_f[0][1] = 0.0f;
	m_effect_constant_f[0][2] = 0.0f;
	m_effect_constant_f[0][3] = 0.0f;
	m_effect_constant_f[1][0] = 0.0f;
	m_effect_constant_f[1][1] = 0.0f;
	m_effect_constant_f[1][2] = 0.0f;
	m_effect_constant_f[1][3] = 0.0f;
}

C_d3d_sprite::~C_d3d_sprite()
{
	// 解放
	release();
}

// ****************************************************************
// スプライトの初期化
// ================================================================
void C_d3d_sprite::init()
{
	// 解放
	release();

	// 初期化
	init_no_release();
}

// ****************************************************************
// スプライトの初期化（解放を行わない）
// ================================================================
void C_d3d_sprite::init_no_release()
{
	// タイプの初期化
	type = E_d3d_sprite_type_none;

	// レンダーパラメータ
	rp.init();

	// プリミティブ関連
	m_vertex_offset = 0;
	m_vertex_cnt = 0;
	m_index_offset = 0;
	m_index_cnt = 0;
	m_primitive_cnt = 0;

	// エフェクト関連
	m_effect = NULL;
	m_effect_technique = NULL;
	m_effect_constant_f[0][0] = 0.0f;
	m_effect_constant_f[0][1] = 0.0f;
	m_effect_constant_f[0][2] = 0.0f;
	m_effect_constant_f[0][3] = 0.0f;
	m_effect_constant_f[1][0] = 0.0f;
	m_effect_constant_f[1][1] = 0.0f;
	m_effect_constant_f[1][2] = 0.0f;
	m_effect_constant_f[1][3] = 0.0f;
}

// ****************************************************************
// スプライトの解放
// ================================================================
void C_d3d_sprite::release()
{
	// プリミティブ関連
	m_vertex_buffer.reset();
	m_index_buffer.reset();

	// メッシュ関連
	m_mesh.reset();
	m_frame_set.reset();

	// テクスチャ関連
	for (int t = 0; t < D3D_TEXTURE_CNT_MAX; t++)
		m_texture_list[t].reset();
}

// ****************************************************************
// ポリゴンスプライトの作成（初期化しない）
// ================================================================
bool C_d3d_sprite::create_polygon_no_init()
{
	// メンバの初期化
	type = E_d3d_sprite_type_polygon;

	return true;
}

// ****************************************************************
// メッシュスプライトの作成（初期化しない）
// ================================================================
bool C_d3d_sprite::create_mesh_no_init()
{
	// メンバの初期化
	type = E_d3d_sprite_type_mesh;

	return true;
}

// ****************************************************************
// フレームセットスプライトの作成（初期化しない）
// ================================================================
bool C_d3d_sprite::create_frame_set_no_init()
{
	// メンバの初期化
	type = E_d3d_sprite_type_frame_set;

	return true;
}

// ****************************************************************
// 位置を２Ｄ→３Ｄに変換
// ================================================================
//		update_d2_vertex_transform で使われる補助関数。
//		拡縮、回転、移動を計算します。
// ================================================================
D3DXVECTOR4 update_d2_vertex_sprite_to_client(D3DXVECTOR4 vertex_2d, const S_d3d_render_param* rp)
{
	// 中心のクライアント座標を求める
	D3DXVECTOR4 pos((float)(rp->pos.x), (float)(rp->pos.y), 0.0f, 1.0f);

	// 頂点のクライアント座標を求める
	D3DXVECTOR4 vertex(pos.x + vertex_2d.x, pos.y + vertex_2d.y, 0.0f, 1.0f);

	// 中心が原点に来るように移動する
	vertex.x -= pos.x;
	vertex.y -= pos.y;

	// 回転パラメータ
	float yaw   = rp->rotate.x;
	float pitch = rp->rotate.y;
	float roll  = rp->rotate.z;

	// ワールド座標変換行列を作成
	D3DXMATRIXA16 mat_world, mat_temp;
	D3DXMatrixIdentity(&mat_world);
	D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixScaling(&mat_temp, rp->scale.x, rp->scale.y, 1.0f));			// 拡縮
	D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixRotationYawPitchRoll(&mat_temp, yaw, pitch, roll));			// 回転
//	D3DXMatrixMultiply(&mat_world, &mat_world, D3DXMatrixTranslation(&mat_temp, rp->pos.x, rp->pos.y, rp->pos.z));		// 移動

	// ワールド変換座標を適用
	D3DXVec4Transform(&vertex, &vertex, &mat_world);

	// 位置を元に戻す
	vertex.x += pos.x;
	vertex.y += pos.y;

	return vertex;
}

// ****************************************************************
// ２Ｄスプライトの設定
// ================================================================
//		頂点バッファの位置やインデックスの位置、プリミティブ数などを指定します。
//		また、rp から計算された情報を、頂点バッファに実際に書き込みます。
// ================================================================
//		rp に頼らず座標データを直に指定したい場合は、p_vertex_pos を指定して下さい。通常は NULL です。
//		rp に頼らずテクスチャ座標データを直に指定したい場合は、p_texture_uv を指定して下さい。通常は NULL です。
// ================================================================
bool C_d3d_sprite::set_d2_vertex_param(int vertex_offset, int vertex_cnt, int index_offset, int index_cnt, int primitive_cnt
									   , BYTE* p_vertex_buffer, D3DXVECTOR4* p_vertex_pos, D3DXVECTOR2* p_texture_uv)
{
	// 頂点バッファのオフセットを指定
	m_vertex_offset = vertex_offset;
	m_index_offset = index_offset;
	m_vertex_cnt = vertex_cnt;
	m_index_cnt = index_cnt;
	m_primitive_cnt = primitive_cnt;

	// 書き込み先の頂点バッファが指定されてないときはここで終了
	if (!p_vertex_buffer)
		return true;

	// 頂点バッファが設定されていなければ失敗（FVF などが取得できないため）
	if (!m_vertex_buffer)
		return false;

	// 各頂点を書き込む位置を取得する
	DWORD FVF = m_vertex_buffer->get_FVF();
	int FVF_size = m_vertex_buffer->get_FVF_size();
	BYTE* vertex[4] = {(BYTE *)p_vertex_buffer + m_vertex_offset * FVF_size
		, (BYTE *)p_vertex_buffer + (m_vertex_offset + 1) * FVF_size
		, (BYTE *)p_vertex_buffer + (m_vertex_offset + 2) * FVF_size
		, (BYTE *)p_vertex_buffer + (m_vertex_offset + 3) * FVF_size};
	int offset = 0;

	// 頂点座標
	D3DXVECTOR4 vertex_pos[4];
	float uv_left = 0.0f, uv_top = 0.0f, uv_right = 1.0f, uv_bottom = 1.0f;

	// 位置情報がない場合は rp から導き出す
	if (p_vertex_pos == NULL)	{

		// 中心座標とサイズ
		C_float2 size = rp.size;
		C_float3 center = rp.center;

		// 大きさをテクスチャサイズに合わせる
		if (0 <= rp.size_fit_to_texture && rp.size_fit_to_texture < D3D_TEXTURE_CNT_MAX)	{
			if (m_texture_list[rp.size_fit_to_texture])	{
				size.x = (float)m_texture_list[rp.size_fit_to_texture]->get_width();
				size.y = (float)m_texture_list[rp.size_fit_to_texture]->get_height();

				// 中心座標をあわせる
				center.x += m_texture_list[rp.size_fit_to_texture]->get_center().x;
				center.y += m_texture_list[rp.size_fit_to_texture]->get_center().y;
			}
		}

		// 転送元矩形を設定する
		float src_clip_left = 0.0f - center.x;
		float src_clip_top = 0.0f - center.y;
		float src_clip_right = size.x - center.x;
		float src_clip_bottom = size.y - center.y;

		// 転送元矩形の補正を行う
		if (rp.src_clip_use)	{

			src_clip_left = std::max(src_clip_left, (float)rp.src_clip.left);
			src_clip_top = std::max(src_clip_top, (float)rp.src_clip.top);
			src_clip_right = std::min(src_clip_right, (float)rp.src_clip.right);
			src_clip_bottom = std::min(src_clip_bottom, (float)rp.src_clip.bottom);

			// 割合からテクスチャ座標を補正する
			uv_left = (src_clip_left + center.x) / size.x;
			uv_top = (src_clip_top + center.y) / size.y;
			uv_right = (src_clip_right + center.x) / size.x;
			uv_bottom = (src_clip_bottom + center.y) / size.y;
		}

		// 回転を行わない場合（３Ｄの場合は頂点シェーダで回転を行う）
		if (fabs(rp.rotate.z) < FLT_EPSILON)	{

			// ２Ｄの場合
			if ((FVF & D3DFVF_POSITION_MASK) == D3DFVF_XYZRHW)	{

				// 左上と右下だけを計算する
				vertex_pos[0] = D3DXVECTOR4(src_clip_left, src_clip_top, 0.0f - center.z, 1.0f);
				vertex_pos[3] = D3DXVECTOR4(src_clip_right, src_clip_bottom, 0.0f - center.z, 1.0f);

				// 拡縮を適用する
				vertex_pos[0] = update_d2_vertex_sprite_to_client(vertex_pos[0], &rp);
				vertex_pos[3] = update_d2_vertex_sprite_to_client(vertex_pos[3], &rp);

				// 右上と左下にもコピーする
				vertex_pos[1].x = vertex_pos[3].x;
				vertex_pos[1].y = vertex_pos[0].y;
				vertex_pos[2].x = vertex_pos[0].x;
				vertex_pos[2].y = vertex_pos[3].y;
			}
			// ３Ｄの場合
			else	{

				// ４頂点を計算する
				vertex_pos[0] = D3DXVECTOR4(src_clip_left, src_clip_top, 0.0f - center.z, 1.0f);
				vertex_pos[1] = D3DXVECTOR4(src_clip_right, src_clip_top, 0.0f - center.z, 1.0f);
				vertex_pos[2] = D3DXVECTOR4(src_clip_left,src_clip_bottom, 0.0f - center.z, 1.0f);
				vertex_pos[3] = D3DXVECTOR4(src_clip_right, src_clip_bottom, 0.0f - center.z, 1.0f);
			}
		}
		// 回転を行う場合
		else	{

			// ４頂点を計算する
			vertex_pos[0] = D3DXVECTOR4(src_clip_left, src_clip_top, 0.0f - center.z, 1.0f);
			vertex_pos[1] = D3DXVECTOR4(src_clip_right, src_clip_top, 0.0f - center.z, 1.0f);
			vertex_pos[2] = D3DXVECTOR4(src_clip_left, src_clip_bottom, 0.0f - center.z, 1.0f);
			vertex_pos[3] = D3DXVECTOR4(src_clip_right, src_clip_bottom, 0.0f - center.z, 1.0f);

			// ２Ｄの場合は変形
			if ((FVF & D3DFVF_POSITION_MASK) == D3DFVF_XYZRHW)	{
				vertex_pos[0] = update_d2_vertex_sprite_to_client(vertex_pos[0], &rp);
				vertex_pos[1] = update_d2_vertex_sprite_to_client(vertex_pos[1], &rp);
				vertex_pos[2] = update_d2_vertex_sprite_to_client(vertex_pos[2], &rp);
				vertex_pos[3] = update_d2_vertex_sprite_to_client(vertex_pos[3], &rp);
			}
		}

		// xy 座標を 0.5px 左上にずらす。これ重要。なければ整数時にぼけてしまう。
		// z 座標は使わないので適当に 0.5f にしておく。（0.0f や 1.0f では表示されないグラボがある）
		if ((FVF & D3DFVF_POSITION_MASK) == D3DFVF_XYZRHW)	{
			vertex_pos[0].x -= 0.5f;	vertex_pos[0].y -= 0.5f;	vertex_pos[0].z = 0.5f;		
			vertex_pos[1].x -= 0.5f;	vertex_pos[1].y -= 0.5f;	vertex_pos[1].z = 0.5f;		
			vertex_pos[2].x -= 0.5f;	vertex_pos[2].y -= 0.5f;	vertex_pos[2].z = 0.5f;		
			vertex_pos[3].x -= 0.5f;	vertex_pos[3].y -= 0.5f;	vertex_pos[3].z = 0.5f;		
		}

		// w は 1.0 固定
		vertex_pos[0].w = 1.0f;
		vertex_pos[1].w = 1.0f;
		vertex_pos[2].w = 1.0f;
		vertex_pos[3].w = 1.0f;

		// 作った頂点情報をセットする
		p_vertex_pos = vertex_pos;
	}

	// 位置
	if ((FVF & D3DFVF_POSITION_MASK) == D3DFVF_XYZRHW)	{

		*(D3DXVECTOR3 *)(vertex[0] + offset) = D3DXVECTOR3(p_vertex_pos[0].x, p_vertex_pos[0].y, p_vertex_pos[0].z);
		*(D3DXVECTOR3 *)(vertex[1] + offset) = D3DXVECTOR3(p_vertex_pos[1].x, p_vertex_pos[1].y, p_vertex_pos[1].z);
		*(D3DXVECTOR3 *)(vertex[2] + offset) = D3DXVECTOR3(p_vertex_pos[2].x, p_vertex_pos[2].y, p_vertex_pos[2].z);
		*(D3DXVECTOR3 *)(vertex[3] + offset) = D3DXVECTOR3(p_vertex_pos[3].x, p_vertex_pos[3].y, p_vertex_pos[3].z);
		offset += sizeof(D3DXVECTOR3);
		*(FLOAT *)(vertex[0] + offset) = p_vertex_pos[0].w;
		*(FLOAT *)(vertex[1] + offset) = p_vertex_pos[1].w;
		*(FLOAT *)(vertex[2] + offset) = p_vertex_pos[2].w;
		*(FLOAT *)(vertex[3] + offset) = p_vertex_pos[3].w;
		offset += sizeof(FLOAT);
	}
	else if ((FVF & D3DFVF_POSITION_MASK) == D3DFVF_XYZ)	{

		*(D3DXVECTOR3 *)(vertex[0] + offset) = D3DXVECTOR3(p_vertex_pos[0].x, p_vertex_pos[0].y, p_vertex_pos[0].z);
		*(D3DXVECTOR3 *)(vertex[1] + offset) = D3DXVECTOR3(p_vertex_pos[1].x, p_vertex_pos[1].y, p_vertex_pos[1].z);
		*(D3DXVECTOR3 *)(vertex[2] + offset) = D3DXVECTOR3(p_vertex_pos[2].x, p_vertex_pos[2].y, p_vertex_pos[2].z);
		*(D3DXVECTOR3 *)(vertex[3] + offset) = D3DXVECTOR3(p_vertex_pos[3].x, p_vertex_pos[3].y, p_vertex_pos[3].z);
		offset += sizeof(D3DXVECTOR3);
	}

	// 法線
	if (FVF & D3DFVF_NORMAL)	{

		*(D3DXVECTOR3 *)(vertex[0] + offset) = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		*(D3DXVECTOR3 *)(vertex[1] + offset) = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		*(D3DXVECTOR3 *)(vertex[2] + offset) = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		*(D3DXVECTOR3 *)(vertex[3] + offset) = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		offset += sizeof(D3DXVECTOR3);
	}

	// ディフューズ色
	if (FVF & D3DFVF_DIFFUSE)	{
		*(D3DCOLOR *)(vertex[0] + offset) = D3DCOLOR_ARGB(rp.tr * rp.filter.color.a / 255, rp.filter.color.r, rp.filter.color.g, rp.filter.color.b);
		*(D3DCOLOR *)(vertex[1] + offset) = D3DCOLOR_ARGB(rp.tr * rp.filter.color.a / 255, rp.filter.color.r, rp.filter.color.g, rp.filter.color.b);
		*(D3DCOLOR *)(vertex[2] + offset) = D3DCOLOR_ARGB(rp.tr * rp.filter.color.a / 255, rp.filter.color.r, rp.filter.color.g, rp.filter.color.b);
		*(D3DCOLOR *)(vertex[3] + offset) = D3DCOLOR_ARGB(rp.tr * rp.filter.color.a / 255, rp.filter.color.r, rp.filter.color.g, rp.filter.color.b);
		offset += sizeof(D3DCOLOR);
	}

	// テクスチャ（２枚まで対応）
	int tex_cnt = 0;
	if (false);
	else if ((FVF & D3DFVF_TEXCOUNT_MASK) == D3DFVF_TEX1)	tex_cnt = 1;
	else if ((FVF & D3DFVF_TEXCOUNT_MASK) == D3DFVF_TEX2)	tex_cnt = 2;
	//else if ((FVF & D3DFVF_TEXCOUNT_MASK) == D3DFVF_TEX3)	tex_cnt = 3;
	//else if ((FVF & D3DFVF_TEXCOUNT_MASK) == D3DFVF_TEX4)	tex_cnt = 4;
	//else if ((FVF & D3DFVF_TEXCOUNT_MASK) == D3DFVF_TEX5)	tex_cnt = 5;
	//else if ((FVF & D3DFVF_TEXCOUNT_MASK) == D3DFVF_TEX6)	tex_cnt = 6;
	//else if ((FVF & D3DFVF_TEXCOUNT_MASK) == D3DFVF_TEX7)	tex_cnt = 7;
	//else if ((FVF & D3DFVF_TEXCOUNT_MASK) == D3DFVF_TEX8)	tex_cnt = 8;

	// デフォルト設定を行うテクスチャの個数
	int default_set_tex_cnt = tex_cnt;
	if ((FVF & D3DFVF_POSITION_MASK) == D3DFVF_XYZRHW && rp.use_mask)	{
		default_set_tex_cnt = 1;	// テクスチャ１番をマスクとして使う場合は０番のみデフォルト設定
	}

	// テクスチャのデフォルト設定をまず行う
	for (int t = 0; t < default_set_tex_cnt; t++)	{
		D3DXVECTOR2 texture_uv[4];

		// p_texture_uv が指定されている場合は与えられた値を使う
		if (p_texture_uv)	{

			// テクスチャ座標を求める
			if (m_texture_list[t])	{

				// テクスチャの実領域を考えてテクスチャ座標を計算する
				// 例えばテクスチャ幅が 800 の場合、実際は 1024 に補正されている可能性があります。
				// その場合、テクスチャ座標を 800/1024 倍する必要があるのです。
				float x_rep = (float)m_texture_list[t]->get_width() / (float)m_texture_list[t]->get_width_ex();
				float y_rep = (float)m_texture_list[t]->get_height() / (float)m_texture_list[t]->get_height_ex();
				texture_uv[0].x = p_texture_uv[0].x * x_rep;
				texture_uv[0].y = p_texture_uv[0].y * y_rep;
				texture_uv[1].x = p_texture_uv[1].x * x_rep;
				texture_uv[1].y = p_texture_uv[1].y * y_rep;
				texture_uv[2].x = p_texture_uv[2].x * x_rep;
				texture_uv[2].y = p_texture_uv[2].y * y_rep;
				texture_uv[3].x = p_texture_uv[3].x * x_rep;
				texture_uv[3].y = p_texture_uv[3].y * y_rep;
			}
		}

		// p_texture_uv が指定されていない場合は計算した値を使う
		else	{

			// テクスチャの実領域を考えてテクスチャ座標を計算する
			// 例えばテクスチャ幅が 800 の場合、実際は 1024 に補正されている可能性があります。
			// その場合、テクスチャ座標を 800/1024 倍する必要があるのです。
			float uv_l = uv_left, uv_t = uv_top, uv_r = uv_right, uv_b = uv_bottom;
			if (m_texture_list[t])	{
				float x_rep = (float)m_texture_list[t]->get_width() / (float)m_texture_list[t]->get_width_ex();
				float y_rep = (float)m_texture_list[t]->get_height() / (float)m_texture_list[t]->get_height_ex();
				uv_l = uv_l * x_rep;
				uv_t = uv_t * y_rep;
				uv_r = uv_r * x_rep;
				uv_b = uv_b * y_rep;
			}

			texture_uv[0] = D3DXVECTOR2(uv_l, uv_t);
			texture_uv[1] = D3DXVECTOR2(uv_r, uv_t);
			texture_uv[2] = D3DXVECTOR2(uv_l, uv_b);
			texture_uv[3] = D3DXVECTOR2(uv_r, uv_b);
		}

		*(D3DXVECTOR2 *)(vertex[0] + offset) = texture_uv[0];
		*(D3DXVECTOR2 *)(vertex[1] + offset) = texture_uv[1];
		*(D3DXVECTOR2 *)(vertex[2] + offset) = texture_uv[2];
		*(D3DXVECTOR2 *)(vertex[3] + offset) = texture_uv[3];
		offset += sizeof(D3DXVECTOR2);
	}

	// テクスチャ１番をマスクとして使う場合
	if ((FVF & D3DFVF_POSITION_MASK) == D3DFVF_XYZRHW && rp.use_mask)	{
		C_d3d_texture* p_mask_texture = m_texture_list[1].get();
		float center_x = (float)p_mask_texture->get_center().x;
		float center_y = (float)p_mask_texture->get_center().y;
		float width_ex = (float)p_mask_texture->get_width_ex();
		float height_ex = (float)p_mask_texture->get_height_ex();
		for (int i = 0; i < 4; i++)	{
			float u = linear(p_vertex_pos[i].x + 0.5f - rp.mask_x, - center_x, 0.0f, - center_x + width_ex, 1.0f);
			float v = linear(p_vertex_pos[i].y + 0.5f - rp.mask_y, - center_y, 0.0f, - center_y + height_ex, 1.0f);
			*(D3DXVECTOR2 *)(vertex[i] + offset) = D3DXVECTOR2(u, v);
		}
		offset += sizeof(D3DXVECTOR2);
	}

	return true;
}


// ****************************************************************
// アニメーションを進める
// ================================================================
//void C_d3d_sprite::advance_animation(double past_time)
//{
//	if (m_frame_set)
//		m_frame_set->advance_animation(past_time);
//}

// ****************************************************************
// テクスチャをクリア
// ================================================================
void C_d3d_sprite::clear_texture(int stage)
{
	if (0 <= stage && stage < D3D_TEXTURE_CNT_MAX)
		m_texture_list[stage].reset();
}

// ****************************************************************
// テクスチャを設定
// ================================================================
void C_d3d_sprite::set_texture(BSP<C_d3d_texture> texture, int stage)
{
	if (0 <= stage && stage < D3D_TEXTURE_CNT_MAX)
		m_texture_list[stage] = texture;
}

// ****************************************************************
// テクスチャを取得
// ================================================================
BSP<C_d3d_texture> C_d3d_sprite::get_texture(int stage)
{
	if (0 <= stage && stage < D3D_TEXTURE_CNT_MAX)
		return m_texture_list[stage];

	return BSP<C_d3d_texture>();
}

// ****************************************************************
// エフェクトの設定と取得
// ================================================================
void C_d3d_sprite::set_effect(ID3DXEffect* effect)
{
	m_effect = effect;
}

LPD3DXEFFECT C_d3d_sprite::get_effect()
{
	return m_effect;
}

// ****************************************************************
// エフェクトのテクニックの設定と取得
// ================================================================
void C_d3d_sprite::set_effect_technique(D3DXHANDLE technique)
{
	m_effect_technique = technique;
}

D3DXHANDLE C_d3d_sprite::get_effect_technique()
{
	return m_effect_technique;
}

// ****************************************************************
// エフェクトの定数の設定と取得
// ================================================================
void C_d3d_sprite::set_effect_constant_f(int idx, float f0, float f1, float f2, float f3)
{
	m_effect_constant_f[idx][0] = f0;
	m_effect_constant_f[idx][1] = f1;
	m_effect_constant_f[idx][2] = f2;
	m_effect_constant_f[idx][3] = f3;
}

float* C_d3d_sprite::get_effect_constant_f(int idx)
{
	return m_effect_constant_f[idx];
}



}
