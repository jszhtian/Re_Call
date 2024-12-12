#include	"tonad3d3_pch.h"
#include	"tonad3d3_d3d.h"
#include	"tonad3d3_device.h"
#include	"tonad3d3_state.h"

namespace NT3
{

// ****************************************************************
// ステートの初期化
// ================================================================
bool C_d3d_state::init()
{
	// レンダーステートの初期化
	for (int d = 0; d < 256; d ++)
		render_state[d] = (DWORD)(-1);

	return true;
}

// ****************************************************************
// ステートにテンプレートを与える
// ================================================================
bool C_d3d_state::set_template()
{
	// 深度バッファ（有効）
	set_render_state(D3DRS_ZENABLE, D3DZB_TRUE);
	set_render_state(D3DRS_ZWRITEENABLE, TRUE);

	// 深度バッファの比較関数
	set_render_state(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// フィリング（面）
	set_render_state(D3DRS_FILLMODE, D3DFILL_SOLID);

	// シェーディング（グーロー）
	set_render_state(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	// アルファテスト（有効、１以上が通過）
	set_render_state(D3DRS_ALPHATESTENABLE, TRUE);
	set_render_state(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	set_render_state(D3DRS_ALPHAREF, 0x01);

	// アルファブレンド（有効）
	set_render_state(D3DRS_ALPHABLENDENABLE, TRUE);
	set_render_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	set_render_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	// アルファ値のアルファブレンド（有効）
	set_render_state(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	set_render_state(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	set_render_state(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
	set_render_state(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);

	// カリング（左回り）
	set_render_state(D3DRS_CULLMODE, D3DCULL_CCW);

	// ライティングモード（無効）
	set_render_state(D3DRS_LIGHTING, FALSE);

	// アンビエント光(0）
	set_render_state(D3DRS_AMBIENT, 0);

	// 法線を自動的に正規化
	set_render_state(D3DRS_NORMALIZENORMALS, TRUE);

	// マルチサンプリングの設定
	set_render_state(D3DRS_MULTISAMPLEANTIALIAS, G_d3d.device.pp()->MultiSampleType ? TRUE : FALSE);

	// テクスチャステージの設定
	G_d3d.device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);			// RGB = ARG1 * ARG2
	G_d3d.device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);			// ARG1 = テクスチャ
	G_d3d.device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);			// ARG2 = ディフューズ色
	G_d3d.device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);			// A = ARG1 * ARG2
	G_d3d.device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);			// ARG1 = テクスチャ
	G_d3d.device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);			// ARG2 = ディフューズ色
	G_d3d.device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);			// RGB = ARG1 * ARG2
	G_d3d.device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);			// ARG1 = テクスチャ
	G_d3d.device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);			// ARG2 = ディフューズ色
	G_d3d.device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);			// A = ARG1 * ARG2
	G_d3d.device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);			// ARG1 = テクスチャ
	G_d3d.device->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);			// ARG2 = ディフューズ色

	// テクスチャフィルタリング（リニア）
	for (int i = 0; i < 4; i++)	{
		G_d3d.device->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		G_d3d.device->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

		// ステージ０のミップマップのフィルタは D3DTEXF_LINEAR。
		// ステージ１はマスクワイプ／トーンカーブ専用。ミップマップを使用しない。
		if (i == 0)	{

			// 使えない場合は自動的に下げられることに注意
			set_sampler_state(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		}
		else	{

			// ミップマップを使用しない
			set_sampler_state(i, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		}
	}

	// テクスチャラッピングモード
	for (int i = 0; i < 8; i ++)	{
		G_d3d.state.set_sampler_state(i, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(i, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(i, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
	}

	// ライトの初期化
#if 0
	for (int l = 0; l < MAX_LIGHT; l++)	{
		ZeroMemory(&light[l], sizeof(light[l]));

		// デフォルト：白の真上からの直線光
		light[l].Type = D3DLIGHT_DIRECTIONAL;
		light[l].Position.x = 0.0f;
		light[l].Position.y = 0.0f;
		light[l].Position.z = 0.0f;
		light[l].Diffuse.a = 1.0f;
		light[l].Diffuse.r = 1.0f;
		light[l].Diffuse.g = 1.0f;
		light[l].Diffuse.b = 1.0f;
		light[l].Ambient.a = 1.0f;
		light[l].Ambient.r = 1.0f;
		light[l].Ambient.g = 1.0f;
		light[l].Ambient.b = 1.0f;

		D3DXVECTOR3 dir(0.0f, 1.0f, 0.0f);
		D3DXVec3Normalize(&dir, &dir);
		light[l].Direction = dir;
		G_d3d.device->SetLight(l, &light[l]);
	}
#endif

	// ライト有効無効の初期化
#if 0
	for (int l = 0; l < MAX_LIGHT; l++)	{
		G_d3d.device->LightEnable(l, FALSE);
		light_enable[l] = false;
	}
#endif

	return true;
}

// ****************************************************************
// サンプラーステートの設定
// ================================================================
void C_d3d_state::set_sampler_state(int stage, D3DSAMPLERSTATETYPE type, DWORD value)
{
	// グラボの対応をチェックしつつ決定。
	// ミップマップが自動生成できない環境では D3DTEXF_NONE にすることに注意。
	// Intel 82915 で NONE になる。

	// ミップフィルターの場合は使えるかを判定
	if (type == D3DSAMP_MIPFILTER)	{

		if (value == D3DTEXF_LINEAR)	{
			if (G_d3d.device.can_set_D3DPTFILTERCAPS_MIPFLINEAR_for_texture())	{
				G_d3d.device->SetSamplerState(stage, type, value);
			}
			else	{
				value = D3DTEXF_POINT;		// 使えないので D3DTEXF_POINT を試みる
			}
		}
		if (value == D3DTEXF_POINT)	{
			if (G_d3d.device.can_set_D3DPTFILTERCAPS_MIPFPOINT_for_texture())	{
				G_d3d.device->SetSamplerState(stage, type, value);
			}
			else	{
				value = D3DTEXF_NONE;		// 使えないので D3DTEXF_NONE を試みる
			}
		}
		if (value == D3DTEXF_NONE)	{
			G_d3d.device->SetSamplerState(stage, type, value);	// 必ず使える
		}
	}

	// アドレッシングモードの場合も使えるか判定
	else if (type == D3DSAMP_ADDRESSU || type == D3DSAMP_ADDRESSV || type == D3DSAMP_ADDRESSW)	{

		if (value == D3DTADDRESS_BORDER)	{
			if (G_d3d.device.device_caps().TextureAddressCaps & D3DPTADDRESSCAPS_BORDER)	{
				G_d3d.device->SetSamplerState(stage, type, value);
			}
			else	{
				value = D3DTADDRESS_CLAMP;	// 使えないので D3DTADDRESS_CLAMP を試みる
			}
		}
		if (value == D3DTADDRESS_CLAMP)	{
			G_d3d.device->SetSamplerState(stage, type, value);	// たぶん使える
		}
		if (value == D3DTADDRESS_MIRROR)	{
			G_d3d.device->SetSamplerState(stage, type, value);	// たぶん使える
		}
		if (value ==  D3DTADDRESS_WRAP)	{
			G_d3d.device->SetSamplerState(stage, type, value);	// たぶん使える
		}
	}

	// それ以外の場合は普通に使う
	else	{
		G_d3d.device->SetSamplerState(stage, type, value);
	}
}

// ****************************************************************
// レンダーステートの設定
// ================================================================
void C_d3d_state::set_render_state(D3DRENDERSTATETYPE type, DWORD value)
{
	// 値が変更された場合のみ処理を行う
	if (render_state[type] != value)	{

		// 値の設定
		render_state[type] = value;
		G_d3d.device->SetRenderState(type, value);
	}
}

// ****************************************************************
// レンダーステートの取得
// ================================================================
DWORD C_d3d_state::get_render_state(D3DRENDERSTATETYPE type)
{
	return render_state[type];
}

// ****************************************************************
// ライト有効無効の設定
// ================================================================
void C_d3d_state::set_light_enable(int light_no, bool on)
{
	// 値が変更された場合のみ処理を行う
	if (light_enable[light_no] != on)	{

		// 値の設定
		light_enable[light_no] = on;
		G_d3d.device->LightEnable(light_no, on ? TRUE : FALSE);
	}
}

// ****************************************************************
// ライト有効無効の設定
// ================================================================
bool C_d3d_state::get_light_enable(int light_no)
{
	return light_enable[light_no];
}

// ****************************************************************
// ライトの設定
// ================================================================
void C_d3d_state::set_light(int light_no, C_d3d_light* p_light)
{
	light[light_no] = *p_light;
}

C_d3d_light C_d3d_state::get_light(int light_no)
{
	return light[light_no];
}

}
