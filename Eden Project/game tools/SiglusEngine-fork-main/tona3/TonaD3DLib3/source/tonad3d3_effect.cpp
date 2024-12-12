#include	"tonad3d3_pch.h"
#include	"tonad3d3_d3d.h"
#include	"tonad3d3_effect.h"

namespace NT3
{

// ****************************************************************
// D3D �G�t�F�N�g
// ================================================================
C_d3d_effect::C_d3d_effect()
{
}

C_d3d_effect::C_d3d_effect(BIP<ID3DXEffect> effect)
{
	m_effect = effect;
}

C_d3d_effect::~C_d3d_effect()
{
}

// ****************************************************************
// D3D �G�t�F�N�g��������
// ================================================================
void C_d3d_effect::init()
{
	release();
}

// ****************************************************************
// D3D �G�t�F�N�g�����
// ================================================================
void C_d3d_effect::release()
{
	m_effect = NULL;
}

// ****************************************************************
// D3D �G�t�F�N�g���쐬
// ================================================================

// �o�b�t�@����
bool C_d3d_effect::load_effect_from_buffer(BYTE* buffer, int size)
{
	HRESULT hr;

	// �G�t�F�N�g��������
	init();

	// �G�t�F�N�g���쐬
	ID3DXEffect* p_effect = NULL;
	ID3DXBuffer* p_error = NULL;
	hr = D3DXCreateEffect(G_d3d.device.body(), buffer, (UINT)size, NULL, NULL, 0, NULL, &p_effect, &p_error);
	Cd3d_buffer error(p_error);
	if (FAILED(hr))	{
		set_last_error_d3d(MBSTR_to_TSTR(p_error ? (char *)p_error->GetBufferPointer() : ""), _T("CreateEffect"), hr);
		return false;
	}

	m_effect = BIP<ID3DXEffect>(p_effect, false);

	return true;
}

// ���\�[�X����
bool C_d3d_effect::load_effect_from_resource(LPCTSTR name, LPCTSTR type)
{
	HRESULT hr;

	// �G�t�F�N�g��������
	init();

	// ���\�[�X��ǂݍ���
	BUFFER effect_data;
	if (!resource_get_data(name, type, effect_data))
		return false;

	// �G�t�F�N�g���쐬
	ID3DXEffect* p_effect = NULL;
	ID3DXBuffer* p_error = NULL;
	hr = D3DXCreateEffect(G_d3d.device.body(), effect_data.get(), (UINT)effect_data.size(), NULL, NULL, 0, NULL, &p_effect, &p_error);
	if (FAILED(hr))	{
		Cd3d_buffer error(p_error);
		set_last_error_d3d(MBSTR_to_TSTR((char *)p_error->GetBufferPointer()), _T("CreateEffect"), hr);
		return false;
	}

	m_effect = BIP<ID3DXEffect>(p_effect, false);

	return true;
}

// ****************************************************************
// D3D �f�t�H���g�̃G�t�F�N�g���쐬
// ================================================================

// �쐬����
bool C_d3d_default_effect::create_default_effect()
{
	HRESULT hr;

	MBSTR effect_str;

	effect_str +=

		// �s��
		"matrix	g_mat_world;"
		"matrix	g_mat_view_proj;"

		// �}�e���A���J���[
		"float4	g_mtrl_diffuse;"

		// ���C�g
		//"int g_camera_light_calc_type;"
		//"int g_ambient_light_calc_type;"
		"float3	g_camera_pos;"
		"float3	g_camera_dir;"
		"float4	g_light_pos;"
		"float4	g_light_ambient;"

		// �t�H�O
		"float4 g_fog_param;"	// x=x, y=y, z=w, w=h
		"float4 g_fog_range;"	// x=use, y=near, z=far, w=(keep)

		// ���_�V�F�[�_�F�o�͍\����
		"struct VS_OUTPUT"
		"{"
		"	float4	position	: POSITION;"
		"	float2	texture_uv	: TEXCOORD0;"
		"	float4	diffuse		: COLOR0;"
		"};"
		"struct VS_OUTPUT_3D_FOG"
		"{"
		"	float4	position	: POSITION;"
		"	float2	texture_uv	: TEXCOORD0;"
		"	float4	world_pos	: TEXCOORD1;"
		"	float4	proj_pos	: TEXCOORD2;"
		"	float4	diffuse		: COLOR0;"
		"};"
		"struct VS_OUTPUT_3D_LIGHT"
		"{"
		"	float4	position	: POSITION;"
		"	float2	texture_uv	: TEXCOORD0;"
		"	float3	normal		: TEXCOORD1;"
		"	float4	world_pos	: TEXCOORD2;"
		"	float4	proj_pos	: TEXCOORD3;"
		"	float4	diffuse		: COLOR0;"
		"};"

		// ���_�V�F�[�_
		"VS_OUTPUT vs(float4 position: POSITION, float2 texture_uv : TEXCOORD0, float4 diffuse : COLOR0)"
		"{"
		"	VS_OUTPUT result;"
		"	result.position = position;"				// �ʒu
		"	result.texture_uv = texture_uv;"			// �e�N�X�`�����W
		"	result.diffuse = diffuse;"					// �f�B�q���[�Y�F
		""
		"	return result;"
		"}"

		// ���_�V�F�[�_�F�R�c
		"VS_OUTPUT vs_d3(float4 position: POSITION, float2 texture_uv : TEXCOORD0, float4 diffuse : COLOR0)"
		"{"
		"	VS_OUTPUT result;"
		"	position = mul(position, g_mat_world);"		// ���[���h�ϊ�
		"	position = mul(position, g_mat_view_proj);"	// �r���[���ˉe�ϊ�
		"	result.position = position;"				// �ŏI�I�Ȉʒu
		"	result.texture_uv = texture_uv;"			// �e�N�X�`�����W
		"	result.diffuse = diffuse;"					// �f�B�q���[�Y�F
		"	result.diffuse *= g_mtrl_diffuse;"			// �}�e���A���̃f�B�q���[�Y�F
		"	return result;"
		"}"

		// ���_�V�F�[�_�F�R�c�F�t�H�O
		"VS_OUTPUT_3D_FOG vs_d3_fog(float4 position: POSITION, float2 texture_uv : TEXCOORD0, float4 diffuse : COLOR0)"
		"{"
		"	VS_OUTPUT_3D_FOG result;"
		"	position = mul(position, g_mat_world);"		// ���[���h�ϊ�
		"	result.world_pos = position;"				// ���[���h�ϊ��ς݂̍��W�����ʂƂ��ēn��
		"	position = mul(position, g_mat_view_proj);"	// �r���[���ˉe�ϊ�
		"	result.proj_pos = position;"				// �r���[���ˉe�ϊ��̍��W�����ʂƂ��ēn��
		"	result.position = position;"				// �ŏI�I�Ȉʒu
		"	result.texture_uv = texture_uv;"			// �e�N�X�`�����W
		"	result.diffuse = diffuse;"					// �f�B�q���[�Y�F
		"	result.diffuse *= g_mtrl_diffuse;"			// �}�e���A���̃f�B���[�Y�F
		"	return result;"
		"}"

		// ���_�V�F�[�_�F�R�c�F���C�g
		"VS_OUTPUT_3D_LIGHT vs_d3_light(float4 position: POSITION, float3 normal: NORMAL, float2 texture_uv : TEXCOORD0, float4 diffuse : COLOR0)"
		"{"
		"	VS_OUTPUT_3D_LIGHT result;"
		"	position = mul(position, g_mat_world);"		// ���[���h�ϊ�
		"	result.world_pos = position;"				// ���[���h�ϊ��ς݂̍��W�����ʂƂ��ēn��
		"	position = mul(position, g_mat_view_proj);"	// �r���[���ˉe�ϊ�
		"	result.proj_pos = position;"				// �r���[���ˉe�ϊ��̍��W�����ʂƂ��ēn��
		"	result.position = position;"				// �ŏI�I�Ȉʒu
		"	result.normal = mul(normal, g_mat_world);"	// �@��
		"	result.texture_uv = texture_uv;"			// �e�N�X�`�����W
		"	result.diffuse = diffuse;"					// �f�B�q���[�Y�F
		"	result.diffuse *= g_mtrl_diffuse;"			// �}�e���A���̃f�B���[�Y�F
		"	return result;"
		"}"

		// �e�N�X�`��
		"texture Tex00;"
		"texture Tex01;"
		"texture Tex02;"
		"texture Tex03;"

		// �萔
		"float4 mono = float4(0.2989f, 0.5886f, 0.1145f, 0.0f);"
		"float4 c0;"
		"float4 c1;"
		"float4 c2;"
		"float4 c3;"

		// �T���v���[
		"sampler my_sampler_tex00 = sampler_state"
		"{"
		"	Texture = <Tex00>;"
		"};"
		"sampler my_sampler_tex01 = sampler_state"
		"{"
		"	Texture = <Tex01>;"
		"};"
		"sampler my_sampler_tex02 = sampler_state"
		"{"
		"	Texture = <Tex02>;"
		"};"
		"sampler my_sampler_tex00_ex = sampler_state"
		"{"
		"	Texture = <Tex00>;"
		"	MinFilter = POINT;"
		"	MagFilter = POINT;"
		"	MipFilter = NONE;"
		"};"
		"sampler my_sampler_mask = sampler_state"
		"{"
		"	Texture = <Tex01>;"
		"	AddressU = CLAMP;"
		"	AddressV = CLAMP;"
		"	AddressW = CLAMP;"
		"};"
		"sampler my_sampler_tonecurve = sampler_state"
		"{"
		"	Texture = <Tex02>;"
		"	AddressU = CLAMP;"		// ���� CLAMP �͏d�v�B�[���Q�Ƃ����Ƃ��΍�B
		"	AddressV = CLAMP;"
		"	AddressW = CLAMP;"
		"};"
		"sampler my_sampler_fog = sampler_state"
		"{"
		"	Texture = <Tex03>;"
		"	AddressU = WRAP;"
		"	AddressV = WRAP;"
		"	AddressW = WRAP;"
		"};"
		""
		"float4 tonecurve(float4 color, float mono_y)"
		"{"
		"	float2 tonecurve_pos;"
		"	float4 tonecurve_color;"
		"	color = lerp(color, mono_y, c3.g);"
		"	tonecurve_pos = float2(color.r, c3.r);"
		"	tonecurve_color = tex2D(my_sampler_tonecurve, tonecurve_pos);"
		"	color.r = tonecurve_color.r;"
		"	tonecurve_pos = float2(color.g, c3.r);"
		"	tonecurve_color = tex2D(my_sampler_tonecurve, tonecurve_pos);"
		"	color.g = tonecurve_color.g;"
		"	tonecurve_pos = float2(color.b, c3.r);"
		"	tonecurve_color = tex2D(my_sampler_tonecurve, tonecurve_pos);"
		"	color.b = tonecurve_color.b;"
		"	return color;"
		"}"

		// ���C�g
		"float4 calc_light(float4 color, float4 world_pos, float3 normal)"
		"{"
			// ���|�C���g���C�g
		"	float3 dir_point = g_light_pos - world_pos;"						// �_����̌����Ƌ���
		"	float distance_point = length(dir_point);"							// �_����̋���
		//"	float distance_solid = dot(dir_point, g_camera_dir);"				// �ʂ���̋���
		//"	float dir_solid = g_camera_dir * distance_solid;"					// �ʂ���̌����Ƌ���
		"	float light_power = dot(normalize(normal), normalize(dir_point));"	// �����ɂ�錸��
		"	light_power *= (1.0f - distance_point / 2000);"						// �����ɂ�錸��
		"	light_power = clamp(light_power, 0.0f, 1.0f);"
		//"	if (g_camera_light_calc_type == 1) {"
		//"		color += light_power;"
		//"	}"
		//"	else if (g_camera_light_calc_type == 2) {"
		"		color *= light_power;"
		//"	}"
			// ������
		//"	if (g_ambient_light_calc_type == 1) {"
		//"		color += g_light_ambient;"
		//"	}"
		//"	else if (g_ambient_light_calc_type == 2) {"
		"		color *= g_light_ambient;"
		//"	}"
			// ���f�B���N�V���i�����C�g
		//"	float power = dot(normalize(normal), - normalize(g_light_dir));"
		//"	power = clamp(power, 0.0f, 1.0f);"
		//"	color += g_light_diffuse * power;"
		"	return color;"
		"}"

		// �t�H�O
		"float4 calc_fog(float4 color, float4 world_pos, float4 proj_pos)"
		"{"
//		"	float fog_w = (1280.0f / 2048.0f);"						// �t�H�O���i���ۂɂ͋t���H�j
//		"	float fog_x = (1024.0f / 2048.0f);"						// �t�H�O�ʒu
		"	float3 camera_dir = g_camera_pos - world_pos;"			// �J��������̌����Ƌ���
		"	float camera_distance = length(camera_dir);"			// �J��������̋���
		"	float2 fog_uv = float2((proj_pos.x / proj_pos.w + 1.0f) / 2.0f * g_fog_param.z + g_fog_param.x, 1.0f - (proj_pos.y / proj_pos.w + 1.0f) / 2.0f) * g_fog_param.w + g_fog_param.y;"
		"	float4 fog_color = tex2D(my_sampler_fog, fog_uv);"		// �e�N�X�`���R�Ԃ��t�H�O�e�N�X�`���Ƃ��Č���
		"	float fog_rate = (1.0f - 0.0f) / (g_fog_range.z - g_fog_range.y) * (camera_distance - g_fog_range.y);"
		"	fog_rate = clamp(fog_rate, 0.0f, 1.0f);"
		"	color = lerp(color, fog_color, fog_rate);"
		"	return color;"
		"}"
		;

	// �s�N�Z���V�F�[�_
	for (int vtype = 0; vtype < 3; vtype++)	{
		// 0=base�A1=fog�A2=light

		for (int tex = 0; tex < 2; tex++)	{
			for (int diffuse = 0; diffuse < 2; diffuse++)	{
				for (int mrbd = 0; mrbd < 2; mrbd++)	{
					for (int rgb = 0; rgb < 2; rgb++)	{
						for (int tonecurve = 0; tonecurve < 2; tonecurve++)	{
							for (int mask = 0; mask < 2; mask++)	{
								for (int special = 0; special < 3; special++)	{

									if (mask && vtype != 0)		// �}�X�N���g���ꍇ�͊�{�̂�
										continue;

									MBSTR effect_name = MBSTR()
										+ (vtype == 0 ? "_v0" : "")
										+ (vtype == 1 ? "_v1" : "")
										+ (vtype == 2 ? "_v2" : "")
										+ (tex ? "_tex" : "")
										+ (diffuse ? "_diffuse" : "")
										+ (mrbd ? "_mrbd" : "")
										+ (rgb ? "_rgb" : "")
										+ (tonecurve ? "_tonecurve" : "")
										+ (mask ? "_mask" : "")
										+ (special == 1 ? "_mul" : special == 2 ? "_screen" : "")
										;

									effect_str += "float4 ps" + effect_name + "(float4 diffuse : COLOR0"
										+ (tex ? ", float2 texcoord0 : TEXCOORD0" : "")
										+ (mask ? ", float2 mask_uv : TEXCOORD1" : "")
										+ (vtype == 1 ? ", float4 world_pos : TEXCOORD1" : "")
										+ (vtype == 1 ? ", float4 proj_pos : TEXCOORD2" : "")
										+ (vtype == 2 ? ", float3 normal : TEXCOORD1" : "")
										+ (vtype == 2 ? ", float4 world_pos : TEXCOORD2" : "")
										+ (vtype == 2 ? ", float4 proj_pos : TEXCOORD3" : "")
										+ ") : COLOR0{";
									effect_str += "float4 color = diffuse;";

									// �e�N�X�`������
									if (tex)	{
										effect_str += "float2 texture_uv = float2(texcoord0.x, texcoord0.y);";
										effect_str += "color = color * tex2D(my_sampler_tex00, texture_uv);";
									}
									// �s�N�Z������
									if (mrbd || rgb || tonecurve || special || vtype)	{

										effect_str += "float4 color_org = color;";
										if (vtype == 2)	{
											effect_str += "if (g_light_pos.w > 0.5f) {";	// light.use
											effect_str += "	color = calc_light(color, world_pos, normal);";
											effect_str += "}";
										}
										if (vtype == 1 || vtype == 2)	{
											effect_str += "if (g_fog_range.x > 0.5) {";		// fog.use
											effect_str += "	color = calc_fog(color, world_pos, proj_pos);";
											effect_str += "}";
										}
										if (tonecurve || mrbd)	{
											effect_str += "	float mono_y = dot(mono, color);";
										}
										if (tonecurve)	{
											effect_str += "color = tonecurve(color, mono_y);";
										}
										if (mrbd)	{
											effect_str += "float4 reverse = 1.0f - color;";
											effect_str += "color = lerp(color, reverse, c0.y);";
											effect_str += "color = lerp(color, mono_y, c0.x);";
											effect_str += "color = color + c0.z;";
											effect_str += "color = color - c0.w;";
										}
										if (rgb)	{
											effect_str += "color = lerp(color, c1, c1.w);";
											effect_str += "color += c2;";
										}
										if (special == 1)	{
											effect_str += "color = lerp(1.0f, color, color_org.a);";
										}
										if (special == 2)	{
											effect_str += "color = lerp(0.0f, color, color_org.a);";
										}

										// �����ƕω���������Ă铧���x�����ɖ߂�
										effect_str += "color.a = color_org.a;";
									}

									// �}�X�N�͓����x���ς��̂ł����ŏ���
									if (mask)	{
										effect_str += "float4 mask_color = tex2D(my_sampler_mask, mask_uv);";		// �}�X�N�e�N�X�`���̐F���擾
										effect_str += "color *= mask_color;";										// �}�X�N�F����Z�B�����x����Z���邱�Ƃɒ��ӁB
									}

									effect_str += "return color;";
									effect_str += "}";
								}
							}
						}
					}
				}
			}
		}
	}

	// �e�N�j�b�N
	for (int d3 = 0; d3 < 2; d3++)	{
		for (int light = 0; light < 2; light++)	{
			for (int fog = 0; fog < 2; fog++)	{
				for (int tex = 0; tex < 2; tex++)	{
					for (int diffuse = 0; diffuse < 2; diffuse++)	{
						for (int mrbd = 0; mrbd < 2; mrbd++)	{
							for (int rgb = 0; rgb < 2; rgb++)	{
								for (int tonecurve = 0; tonecurve < 2; tonecurve++)	{
									for (int mask = 0; mask < 2; mask++)	{
										for (int special = 0; special < 3; special++)	{

											if (mask && (light || fog))
												continue;
											if (!d3 && (light || fog))
												continue;

											MBSTR vertex_name = MBSTR()
												+ (d3 ? "_d3" : "")
												+ (light ? "_light" : fog ? "_fog" : "")
												;

											MBSTR pixel_name = MBSTR()
												+ (light ? "_v2" : fog ? "_v1" : "_v0")
												+ (tex ? "_tex" : "")
												+ (diffuse ? "_diffuse" : "")
												+ (mrbd ? "_mrbd" : "")
												+ (rgb ? "_rgb" : "")
												+ (tonecurve ? "_tonecurve" : "")
												+ (mask ? "_mask" : "")
												+ (special == 1 ? "_mul" : special == 2 ? "_screen" : "")
												;

											effect_str += "technique tec_v" + vertex_name + "_p" + pixel_name + "{";
											effect_str += "pass p0 {";
											effect_str += "VertexShader = compile vs_2_0 vs" + vertex_name + "();";
											effect_str += "PixelShader = compile ps_2_0 ps" + pixel_name + "();";
											effect_str += "}";
											effect_str += "}";
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	// �s�N�Z���V�F�[�_�F�}�X�N���C�v
	effect_str +=

		// c0: wipe progress (0 - 1)
		// c1: fade effect (0 - 1) -> (256 - 2)
		"float4 ps_tex2_mask(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0, float2 texcoord1 : TEXCOORD1) : COLOR0"
		"{"
		"	float4 color;"
		"	float4 temp;"
		"	float4 tex = tex2D(my_sampler_tex00, texcoord0);"
		"	float4 mask = tex2D(my_sampler_mask, texcoord1);"
		""
		"	"
		"	float fade = lerp(256.0f, 2.0f, c0.r);"
		"	color = (c0 * fade + mask * (fade - 1.0f) - (fade - 1.0f)) * tex;"
		"	color.bgr = tex.bgr;"
		""
		"	return color;"
		"}"
		;

	// �e�N�j�b�N�F�}�X�N���C�v
	effect_str += 
		"technique tec_tex2_mask"
		"{"
		"	pass p0"
		"	{"
		"		PixelShader = compile ps_2_0 ps_tex2_mask();"
		"	}"
		"}"
		;
#if 0
	// �֐��FRGB�̍ő�l���擾
	effect_str += 
		"float get_rgb_max(float4 rgb)"
		"{"
		"	float max = rgb.r;"
		"	if(max < rgb.g) {"
		"		max = rgb.g;"
		"	}"
		"	if(max < rgb.b) {"
		"		max = rgb.b;"
		"	}"
		"	return max;"
		"}"
		;
#endif
	// �֐��FRGB�����m�N�������ċP�x���擾
	effect_str +=
		"float get_rgb_brightness(float4 rgb)"
		"{"
		"	return dot(mono, rgb);"
		"}"
		;

	// �s�N�Z���V�F�[�_�F�V�~
	// �Â��������炾�񂾂�\������Ă���
	effect_str +=
		// c0.xyz: fade_float(0.0 ~ 1.0)       static
		// c0.w  : wipe_prg_float(0.0 to 1.0)  dynamic
		"float4 ps_tex1_shimi(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0) : COLOR0"
		"{"
		"	float4 color;"
		"	float4 tex = tex2D(my_sampler_tex00, texcoord0);"
		"	color = tex;"
		//"	float avg = (color.r + color.g + color.b) / 3.0f;"
		// �P�x���~���l(����)���傫�����ɐ��`�ŃA���t�@�l���Z�o
		"	if(get_rgb_brightness(color) > c0.w) {"
		"		color.a = tex.a * (c0.x - lerp(c0.x, 0.0f, c0.w));"
		"	}"
		// �P�x���~���l(����)��菬�����Ȃ�����ȍ~�͂����ƃe�N�X�`���̃A���t�@�l�ŕ\��(color = tex�ő���ς�)
		""
		"	return color;"
		"}"
		;

	// �e�N�j�b�N�F�V�~
	effect_str += 
		"technique tec_tex1_shimi"
		"{"
		"	pass p0"
		"	{"
		"		PixelShader = compile ps_2_0 ps_tex1_shimi();"
		"	}"
		"}"
		;

	// �s�N�Z���V�F�[�_�F�V�~INV
	// ���邢�������炾�񂾂�\������Ă���
	effect_str +=
		// c0.xyz: fade_float(0.0 ~ 1.0)       static
		// c0.w  : wipe_prg_float(0.0 to 1.0)  dynamic
		"float4 ps_tex1_shimi_inv(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0) : COLOR0"
		"{"
		"	float4 color;"
		"	float4 tex = tex2D(my_sampler_tex00, texcoord0);"
		"	color = tex;"
		//"	float avg = (color.r + color.g + color.b) / 3.0f;"
		// �P�x���~���l(����)��菬�������ɐ��`�ŃA���t�@�l���Z�o
		"	if(get_rgb_brightness(color) < 1.0f - c0.w) {"
		"		color.a = tex.a * (c0.x - lerp(c0.x, 0.0f, c0.w));"
		"	}"
		// �P�x���~���l(����)���傫���Ȃ�����ȍ~�͂����ƃe�N�X�`���̃A���t�@�l�ŕ\��(color = tex�ő���ς�)
		""
		"	return color;"
		"}"
		;

	// �e�N�j�b�N�F�V�~INV
	effect_str += 
		"technique tec_tex1_shimi_inv"
		"{"
		"	pass p0"
		"	{"
		"		PixelShader = compile ps_2_0 ps_tex1_shimi_inv();"
		"	}"
		"}"
		;

	// �s�N�Z���V�F�[�_�F�N���X���X�^����
	effect_str +=
		// c0.x  : fraction_num(0.0 ~ )				static
		// c0.y  : wave_num(1.0 ~)					static
		// c0.z  : power(0~)						static
		// c0.w  : wipe_prg_float(0.0 to 1.0)		dynamic
		"float4 ps_tex2_raster_h(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0) : COLOR0"
		"{"
		"	float4 color;"
		"	float2 raster_uv;"
		"	float theta = c0.w;"
		"	float fraction_num = c0.x;"		// ������
		"	float tex_coord_for_sin = texcoord0.y * fraction_num;"
		"	modf(tex_coord_for_sin, tex_coord_for_sin);"
		"	tex_coord_for_sin -= fraction_num * 0.1f;"
		"	tex_coord_for_sin /= fraction_num;"
		"	float raster_power = 1.0f;"
		"	float st_tex_rate = 0.0f;"
		"	float ed_tex_rate = 0.0f;"
		"	if(c0.w < 0.5f) {"
		"		raster_power = 1.0f - lerp(1.0f, 0.0f, c0.w*2.0f);"	// 0�̂Ƃ�1�A0.5�̂Ƃ�0
		"		st_tex_rate = lerp(2.0f, 0.0f, c0.w*2.0f);"	// 0.5�̂Ƃ�0�A�ȍ~��0
		"		if(st_tex_rate > 1.0f) st_tex_rate = 1.0f;"
		"	}"
		"	else {"
		"		raster_power = 1.0f - lerp(0.0f, 2.0f, c0.w-0.5f);"	// 0�̂Ƃ�0�A1�̂Ƃ�2�Ȃ̂�0.5�̂Ƃ�1
		"		ed_tex_rate = lerp(0.0f, 4.0f, c0.w-0.5f);"	// 0�̂Ƃ�0�A1�̂Ƃ�2�Ȃ̂�0.5�̂Ƃ�1
		"		if(ed_tex_rate > 1.0f) ed_tex_rate = 1.0f;"
		"	}"
		"	float amplitude = pow(raster_power, 3);"	// �U��
		//"	raster_uv.x = texcoord0.x + sin(3.14*c0.w*c0.z +(tex_coord_for_sin) * 3.14 * c0.y) * amplitude;"
		"	raster_uv.x = texcoord0.x + sin(3.14*c0.w*c0.z +(tex_coord_for_sin) * 3.14 * c0.y) * (1.0f-(log10((1.0f-raster_power)*100.0f)+1)/3.0f);"
		"	raster_uv.y = texcoord0.y;"
		"	float4 tex0 = tex2D(my_sampler_tex00, raster_uv);"
		"	float4 tex1 = tex2D(my_sampler_tex01, raster_uv);"
		// �e�N�X�`���͈͊O�̎��ɋ����I�ɓ����ɂ���
		"	if(raster_uv.x < 0.0f || raster_uv.x > 1.0f || raster_uv.y < 0.0f || raster_uv.y > 1.0f) { "
		"		tex0 = float4(0,0,0,0);"
		"		tex1 = float4(0,0,0,0);"
		"	}"

		// tex0�����C�v��̉摜�ɂȂ�̂Œ���
		"	color = tex1 * st_tex_rate + tex0 * ed_tex_rate;"
		""
		"	return color;"
		"}"
		;

	// �e�N�j�b�N�F�N���X���X�^����
	effect_str += 
		"technique tec_tex2_raster_h"
		"{"
		"	pass p0"
		"	{"
		"		PixelShader = compile ps_2_0 ps_tex2_raster_h();"
		"	}"
		"}"
		;

	// �s�N�Z���V�F�[�_�F�N���X���X�^����
	effect_str +=
		// c0.x  : fraction_num(0.0 ~ )				static
		// c0.y  : wave_num(1.0 ~)					static
		// c0.z  : power(0~)						static
		// c0.w  : wipe_prg_float(0.0 to 1.0)		dynamic
		"float4 ps_tex2_raster_v(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0) : COLOR0"
		"{"
		"	float4 color;"
		"	float2 raster_uv;"
		"	float theta = c0.w;"
		"	float fraction_num = c0.x;"		// ������
		"	float tex_coord_for_sin = texcoord0.x * fraction_num;"
		"	modf(tex_coord_for_sin, tex_coord_for_sin);"
		"	tex_coord_for_sin -= 1 * fraction_num / 10;"
		"	tex_coord_for_sin /= fraction_num;"
		"	float raster_power = 1.0f;"
		"	float st_tex_rate = 0.0f;"
		"	float ed_tex_rate = 0.0f;"
		"	if(c0.w < 0.5f) {"
		"		raster_power = 1.0f - lerp(1.0f, 0.0f, c0.w*2.0f);"	// 0�̂Ƃ�1�A0.5�̂Ƃ�0
		"		st_tex_rate = lerp(2.0f, 0.0f, c0.w*2.0f);"	// 0.5�̂Ƃ�0�A�ȍ~��0
		"		if(st_tex_rate > 1.0f) st_tex_rate = 1.0f;"
		"	}"
		"	else {"
		"		raster_power = 1.0f - lerp(0.0f, 2.0f, c0.w-0.5f);"	// 0�̂Ƃ�0�A1�̂Ƃ�2�Ȃ̂�0.5�̂Ƃ�1
		"		ed_tex_rate = lerp(0.0f, 4.0f, c0.w-0.5f);"	// 0�̂Ƃ�0�A1�̂Ƃ�2�Ȃ̂�0.5�̂Ƃ�1
		"		if(ed_tex_rate > 1.0f) ed_tex_rate = 1.0f;"
		"	}"
		"	float amplitude = pow(raster_power, 3);"	// �U��
		"	raster_uv.x = texcoord0.x;"
		//"	raster_uv.y = texcoord0.y + sin(3.14*c0.w*c0.z +(tex_coord_for_sin) * 3.14 * c0.y) * amplitude;"
		"	raster_uv.y = texcoord0.y + sin(3.14*c0.w*c0.z +(tex_coord_for_sin) * 3.14 * c0.y) * (1.0f-(log10((1.0f-raster_power)*100.0f)+1)/3.0f);"

		"	float4 tex0 = tex2D(my_sampler_tex00, raster_uv);"
		"	float4 tex1 = tex2D(my_sampler_tex01, raster_uv);"
		// �e�N�X�`���͈͊O�̎��ɋ����I�ɓ����ɂ���
		"	if(raster_uv.x < 0.0f || raster_uv.x > 1.0f || raster_uv.y < 0.0f || raster_uv.y > 1.0f) { "
		"		tex0 = float4(0,0,0,0);"
		"		tex1 = float4(0,0,0,0);"
		"	}"

		// tex0�����C�v��̉摜�ɂȂ�̂Œ���
		"	color = tex1 * st_tex_rate + tex0 * ed_tex_rate;"
		""
		"	return color;"
		"}"
		;

	// �e�N�j�b�N�F�N���X���X�^����
	effect_str += 
		"technique tec_tex2_raster_v"
		"{"
		"	pass p0"
		"	{"
		"		PixelShader = compile ps_2_0 ps_tex2_raster_v();"
		"	}"
		"}"
		;


	// �s�N�Z���V�F�[�_�F���X�^����
	effect_str +=
		// c0.x  : fraction_num(0.0 ~ )				static
		// c0.y  : wave_num(1.0 ~)					static
		// c0.z  : power(0~)						static
		// c0.w  : wipe_prg_float(0.0 �� 1.0)		dynamic
		"float4 ps_tex1_raster_h(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0) : COLOR0"
		"{"
		"	float4 color;"
		"	float2 raster_uv;"
		"	float theta = c0.w;"
		"	float fraction_num = c0.x;"		// ������
		"	float tex_coord_for_sin = texcoord0.y * fraction_num;"
		"	modf(tex_coord_for_sin, tex_coord_for_sin);"
		"	tex_coord_for_sin -= fraction_num * 0.1f;"
		"	tex_coord_for_sin /= fraction_num;"
		"	float raster_power = 1.0f-c0.w;"
		"	float tex_rate = c0.w;"
		"	float amplitude = pow(raster_power, 3);"	// �U��
		//"	raster_uv.x = texcoord0.x + sin(3.14*c0.w*c0.z +(tex_coord_for_sin) * 3.14 * c0.y) * amplitude;"
		"	raster_uv.x = texcoord0.x + sin(3.14*c0.w*c0.z +(tex_coord_for_sin) * 3.14 * c0.y) * (1.0f-(log10((1.0f-raster_power)*100.0f)+1)/3.0f);"
		"	raster_uv.y = texcoord0.y;"
		"	float4 tex = tex2D(my_sampler_tex00, raster_uv);"
		// �e�N�X�`���͈͊O�̎��ɋ����I�ɓ����ɂ���
		"	if(raster_uv.x < 0.0f || raster_uv.x > 1.0f || raster_uv.y < 0.0f || raster_uv.y > 1.0f) { "
		"		tex = float4(0,0,0,0);"
		"	}"
		"	color = tex;"
		"	color.a = tex.a * tex_rate;"
		""
		"	return color;"
		"}"
		;

	// �e�N�j�b�N�F���X�^����
	effect_str += 
		"technique tec_tex1_raster_h"
		"{"
		"	pass p0"
		"	{"
		"		PixelShader = compile ps_2_0 ps_tex1_raster_h();"
		"	}"
		"}"
		;

	// �s�N�Z���V�F�[�_�F���X�^����
	effect_str +=
		// c0.x  : fraction_num(0.0 ~ )				static
		// c0.y  : wave_num(1.0 ~)					static
		// c0.z  : power(0~)						static
		// c0.w  : wipe_prg_float(0.0 �� 1.0)		dynamic
		"float4 ps_tex1_raster_v(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0) : COLOR0"
		"{"
		"	float4 color;"
		"	float2 raster_uv;"
		"	float theta = c0.w;"
		"	float fraction_num = c0.x;"		// ������
		"	float tex_coord_for_sin = texcoord0.x * fraction_num;"
		"	modf(tex_coord_for_sin, tex_coord_for_sin);"
		"	tex_coord_for_sin -= fraction_num * 0.1f;"
		"	tex_coord_for_sin /= fraction_num;"
		"	float raster_power = 1.0f-c0.w;"
		"	float tex_rate = c0.w;"
		"	float amplitude = pow(raster_power, 3);"	// �U��
		"	raster_uv.x = texcoord0.x;"
		//"	raster_uv.y = texcoord0.y + sin(3.14*c0.w*c0.z +(tex_coord_for_sin) * 3.14 * c0.y) * amplitude;"
		"	raster_uv.y = texcoord0.y + sin(3.14*c0.w*c0.z +(tex_coord_for_sin) * 3.14 * c0.y) * (1.0f-(log10((1.0f-raster_power)*100.0f)+1)/3.0f);"
		"	float4 tex = tex2D(my_sampler_tex00, raster_uv);"
		// �e�N�X�`���͈͊O�̎��ɋ����I�ɓ����ɂ���
		"	if(raster_uv.x < 0.0f || raster_uv.x > 1.0f || raster_uv.y < 0.0f || raster_uv.y > 1.0f) { "
		"		tex = float4(0,0,0,0);"
		"	}"
		"	color = tex;"
		"	color.a = tex.a * tex_rate;"
		""
		"	return color;"
		"}"
		;

	// �e�N�j�b�N�F���X�^����
	effect_str += 
		"technique tec_tex1_raster_v"
		"{"
		"	pass p0"
		"	{"
		"		PixelShader = compile ps_2_0 ps_tex1_raster_v();"
		"	}"
		"}"
		;

	// �s�N�Z���V�F�[�_�F�N���X�����u���[
	effect_str +=
		// c0.x  : texel_size_u()				static
		// c0.y  : texel_size_v()				static
		// c0.z  : texel_center_u(0.0~1.0)		static
		// c0.w  : texel_center_v(0.0~1.0)		static
		// c1.x  : blur_power()					dynamic
		// c1.y  : blur_coefficient()			static
		// c1.z  : ()						static
		// c1.w  : wipe_prg_float(0.0 �� 1.0)		dynamic
		"float4 ps_tex2_explosion_blur(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0) : COLOR0"
		"{"
		"	float4 Color0[5];"
		"	float4 Color1[5];"
		"	float2 CenterTexel = float2(c0.z, c0.w);"
		// �u���[�̒��S�ʒu �� ���݂̃e�N�Z���ʒu
		"	float2 dir = CenterTexel - texcoord0;"
		// �������v�Z����
		"	float len = length(dir);"
		// �����x�N�g���̐��K�����A�P�e�N�Z�����̒����ƂȂ�����x�N�g�����v�Z����
		"	dir = normalize(dir) * float2(c0.x, c0.x);"
		// BlurPower �Ń{�P��𒲐�����
		// ������ώZ���邱�Ƃɂ��A�����̒��S�ʒu�ɋ߂��قǃu���[�̉e�����������Ȃ�悤�ɂ���
		"	dir *= c1.x * len * c1.y;"

		"float4 FinColor;"
		// ��������
		"	Color0[0]  = tex2D( my_sampler_tex01, texcoord0              ) * 0.40f;"
		"	Color0[1]  = tex2D( my_sampler_tex01, texcoord0 + dir * 2.0f ) * 0.24f;"
		"	Color0[2]  = tex2D( my_sampler_tex01, texcoord0 + dir * 4.0f ) * 0.16f;"
		"	Color0[3]  = tex2D( my_sampler_tex01, texcoord0 + dir * 6.0f ) * 0.14f;"
		"	Color0[4]  = tex2D( my_sampler_tex01, texcoord0 + dir * 8.0f ) * 0.06f;"
		"	Color1[0]  = tex2D( my_sampler_tex00, texcoord0              ) * 0.40f;"
		"	Color1[1]  = tex2D( my_sampler_tex00, texcoord0 + dir * 2.0f ) * 0.24f;"
		"	Color1[2]  = tex2D( my_sampler_tex00, texcoord0 + dir * 4.0f ) * 0.16f;"
		"	Color1[3]  = tex2D( my_sampler_tex00, texcoord0 + dir * 6.0f ) * 0.14f;"
		"	Color1[4]  = tex2D( my_sampler_tex00, texcoord0 + dir * 8.0f ) * 0.06f;"
		"	FinColor = diffuse.a * (Color0[0] + Color0[1] + Color0[2] + Color0[3] + Color0[4]) + (1.0f - diffuse.a) * (Color1[0] + Color1[1] + Color1[2] + Color1[3] + Color1[4]);"

		"return FinColor;"
		"}"
		;
	// �e�N�j�b�N�F�N���X�����u���[
	effect_str += 
		"technique tec_tex2_explosion_blur"
		"{"
		"	pass p0"
		"	{"
		"		PixelShader = compile ps_2_0 ps_tex2_explosion_blur();"
		"	}"
		"}"
		;

	// �s�N�Z���V�F�[�_�F�����u���[
	effect_str +=
		// c0.x  : texel_size_u()				static
		// c0.y  : texel_size_v()				static
		// c0.z  : texel_center_u(0.0~1.0)		static
		// c0.w  : texel_center_v(0.0~1.0)		static
		// c1.x  : blur_power()					dynamic
		// c1.y  : blur_coefficient()			static
		// c1.z  : ()						static
		// c1.w  : wipe_prg_float(0.0 �� 1.0)		dynamic
		"float4 ps_tex1_explosion_blur(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0) : COLOR0"
		"{"
		"	float4 Color[10];"
		"	float2 CenterTexel = float2(c0.z, c0.w);"
		// �u���[�̒��S�ʒu �� ���݂̃e�N�Z���ʒu
		"	float2 dir = CenterTexel - texcoord0;"
		// �������v�Z����
		"	float len = length(dir);"
		// �����x�N�g���̐��K�����A�P�e�N�Z�����̒����ƂȂ�����x�N�g�����v�Z����
		"	dir = normalize(dir) * float2(c0.x, c0.x);"
		// BlurPower �Ń{�P��𒲐�����
		// ������ώZ���邱�Ƃɂ��A�����̒��S�ʒu�ɋ߂��قǃu���[�̉e�����������Ȃ�悤�ɂ���
		"	dir *= c1.x * len * c1.y;"

		"float4 FinColor;"
		// ��������
		"	Color[0]  = tex2D( my_sampler_tex00, texcoord0              ) * 0.19f;"
		"	Color[1]  = tex2D( my_sampler_tex00, texcoord0 + dir        ) * 0.17f;"
		"	Color[2]  = tex2D( my_sampler_tex00, texcoord0 + dir * 2.0f ) * 0.15f;"
		"	Color[3]  = tex2D( my_sampler_tex00, texcoord0 + dir * 3.0f ) * 0.13f;"
		"	Color[4]  = tex2D( my_sampler_tex00, texcoord0 + dir * 4.0f ) * 0.11f;"
		"	Color[5]  = tex2D( my_sampler_tex00, texcoord0 + dir * 5.0f ) * 0.09f;"
		"	Color[6]  = tex2D( my_sampler_tex00, texcoord0 + dir * 6.0f ) * 0.07f;"
		"	Color[7]  = tex2D( my_sampler_tex00, texcoord0 + dir * 7.0f ) * 0.05f;"
		"	Color[8]  = tex2D( my_sampler_tex00, texcoord0 + dir * 8.0f ) * 0.03f;"
		"	Color[9]  = tex2D( my_sampler_tex00, texcoord0 + dir * 9.0f ) * 0.01f;"
		"	FinColor = Color[0] + Color[1] + Color[2] + Color[3] + Color[4] + Color[5] + Color[6] + Color[7] + Color[8] + Color[9];"

		"	FinColor.a = diffuse.a;"

		"return FinColor;"
		"}"
		;
	// �e�N�j�b�N�F�����u���[
	effect_str += 
		"technique tec_tex1_explosion_blur"
		"{"
		"	pass p0"
		"	{"
		"		PixelShader = compile ps_2_0 ps_tex1_explosion_blur();"
		"	}"
		"}"
		;

	// �s�N�Z���V�F�[�_�F���U�C�N
	effect_str +=
		// c0.x  : cut_per_texel_size()			static
		// c0.y  : tex_rate_for_square()		static
		// c0.z  : 
		// c0.w  : 
		// c1.x  : 
		// c1.y  : 
		// c1.z  : 
		// c1.w  : 
		// u�����cut_per_texel_size��ݒ肵��v��tex_rate_for_square���|���Đ����`�ɂȂ�悤��������
		"float4 ps_tex1_mosaic(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0) : COLOR0"
		"{"
		"	float4 Color;"
		//"	float grid = c0.x;"
		//"	float2 tc = floor(grid * texcoord0 + 0.5f) / grid;"
		//"	Color = tex2D( my_sampler_tex00, tc );"
		//"	Color.a = diffuse.a;"
		"	float cut_per_texel_u = c0.x;"
		"	float cut_per_texel_u_inv = 1.0f / cut_per_texel_u;"
		"	float cut_per_texel_v = c0.x * c0.y;"
		"	float cut_per_texel_v_inv = 1.0f / cut_per_texel_v;"
		"	float2 tc;"
		"	tc.x = floor(cut_per_texel_u_inv * texcoord0.x) * cut_per_texel_u;"
		"	tc.y = floor(cut_per_texel_v_inv * texcoord0.y) * cut_per_texel_v;"
		"	Color = tex2D( my_sampler_tex00_ex, tc );"
		"	Color.a = diffuse.a;"

		"	return Color;"
		"}"
		;
	// �e�N�j�b�N�F���U�C�N
	effect_str += 
		"technique tec_tex1_mosaic"
		"{"
		"	pass p0"
		"	{"
		"		PixelShader = compile ps_2_0 ps_tex1_mosaic();"
		"	}"
		"}"
		;

#if 0

	// �t�@�C���ɕۑ�
	C_file::write_full_text(_T("shader.hlsl"), effect_str);

#endif

#if 0

	// �t�@�C������G�t�F�N�g�ǂݍ���
	ID3DXEffect* p_effect = NULL;
	ID3DXBuffer* p_error = NULL;
	hr = D3DXCreateEffectFromFile(G_d3d.device.body(), _T("shader.cfx"), NULL, NULL, 0, NULL, &p_effect, &p_error);
	if (FAILED(hr))	{
		Cd3d_buffer error(p_error);
		set_last_error_d3d(p_error ? MBSTR_to_TSTR((char *)p_error->GetBufferPointer()) : _T(""), _T("CreateEffect"), hr);
		return false;
	}

	m_effect = BIP<ID3DXEffect>(p_effect, false);

#elif 1

	// �����񂩂�G�t�F�N�g���쐬
	ID3DXEffect* p_effect = NULL;
	ID3DXBuffer* p_error = NULL;
	hr = D3DXCreateEffect(G_d3d.device.body(), effect_str.c_str(), (UINT)effect_str.size(), NULL, NULL, 0, NULL, &p_effect, &p_error);
	if (FAILED(hr))	{
		Cd3d_buffer error(p_error);
		set_last_error_d3d(p_error ? MBSTR_to_TSTR((char *)p_error->GetBufferPointer()) : _T(""), _T("CreateEffect"), hr);
		return false;
	}

	m_effect = BIP<ID3DXEffect>(p_effect, false);

#endif

	if (!set_effect_handle())
		return false;

	return true;
}

// �o�b�t�@����
bool C_d3d_default_effect::load_effect_from_buffer(BYTE* buffer, int size)
{
	if (!C_d3d_effect::load_effect_from_buffer(buffer, size))
		return false;

	if (!set_effect_handle())
		return false;

	return true;
}

// ���\�[�X����
bool C_d3d_default_effect::load_effect_from_resource(LPCTSTR name, LPCTSTR type)
{
	if (!C_d3d_effect::load_effect_from_resource(name, type))
		return false;

	if (!set_effect_handle())
		return false;

	return true;
}

// �G�t�F�N�g�̃n���h����ݒ�
bool C_d3d_default_effect::set_effect_handle()
{
	D3DXEFFECT_DESC desc;
	m_effect->GetDesc(&desc);
	for (int i = 0; i < (int)desc.Parameters; i++)	{
		D3DXPARAMETER_DESC param_desc;
		D3DXHANDLE h_param = m_effect->GetParameter(NULL, i);
		m_effect->GetParameterDesc(h_param, &param_desc);

		if (false);
		else if (param_desc.Class == D3DXPC_OBJECT && param_desc.Type == D3DXPT_TEXTURE)	{

			if (false);
			else if (strcmp(param_desc.Name, "Tex00") == 0)	{
				m_texture[0] = h_param;
			}
			else if (strcmp(param_desc.Name, "Tex01") == 0)	{
				m_texture[1] = h_param;
			}
			else if (strcmp(param_desc.Name, "Tex02") == 0)	{
				m_texture[2] = h_param;
			}
			else if (strcmp(param_desc.Name, "Tex03") == 0)	{
				m_texture[3] = h_param;
			}
		}
		else if (param_desc.Class == D3DXPC_VECTOR && param_desc.Type == D3DXPT_FLOAT)	{

			if (false);
			else if (strcmp(param_desc.Name, "c0") == 0)	{
				m_float_array[0] = h_param;
			}
			else if (strcmp(param_desc.Name, "c1") == 0)	{
				m_float_array[1] = h_param;
			}
			else if (strcmp(param_desc.Name, "c2") == 0)	{
				m_float_array[2] = h_param;
			}
			else if (strcmp(param_desc.Name, "c3") == 0)	{
				m_float_array[3] = h_param;
			}
		}
	}

	// �G�t�F�N�g�̃n���h�����擾���Ă����܂��B
	for (int d3 = 0; d3 < 2; d3 ++)	{
		for (int light = 0; light < 2; light ++)	{
			for (int fog = 0; fog < 2; fog ++)	{
				for (int tex = 0; tex < 2; tex ++)	{
					for (int diffuse = 0; diffuse < 2; diffuse ++)	{
						for (int mrbd = 0; mrbd < 2; mrbd ++)	{
							for (int rgb = 0; rgb < 2; rgb ++)	{
								for (int tonecurve = 0; tonecurve < 2; tonecurve ++)	{
									for (int mask = 0; mask < 2; mask ++)	{
										for (int special = 0; special < 3; special ++)	{

											if (mask && (light || fog))
												continue;
											if (!d3 && (light || fog))
												continue;

											TSTR name;
											name += _T("tec_v");
											if (light)			name += _T("_d3_light");
											else if (fog)		name += _T("_d3_fog");
											else if (d3)		name += _T("_d3");

											name += _T("_p");
											if (light)			name += _T("_v2");
											else if (fog)		name += _T("_v1");
											else				name += _T("_v0");

											if (tex)			name += _T("_tex");
											if (diffuse)		name += _T("_diffuse");
											if (mrbd)			name += _T("_mrbd");
											if (rgb)			name += _T("_rgb");
											if (tonecurve)		name += _T("_tonecurve");
											if (mask)			name += _T("_mask");
											if (special == 1)	name += _T("_mul");
											if (special == 2)	name += _T("_screen");

											m_sprite_technique[d3][light][fog][tex][diffuse][mrbd][rgb][tonecurve][mask][special] = m_effect->GetTechniqueByName(TSTR_to_MBSTR(name).c_str());
											if (m_sprite_technique[d3][light][fog][tex][diffuse][mrbd][rgb][tonecurve][mask][special] == NULL)	{
												set_last_error(_T("�G�t�F�N�g ") + name + _T(" ��������܂���ł����B"), _T("GetTechniqueByName"));
												return false;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	m_mask_technique = m_effect->GetTechniqueByName("tec_tex2_mask");
	m_shimi_technique = m_effect->GetTechniqueByName("tec_tex1_shimi");
	m_shimi_inv_technique = m_effect->GetTechniqueByName("tec_tex1_shimi_inv");
	m_cross_raster_horizontal_technique = m_effect->GetTechniqueByName("tec_tex2_raster_h");
	m_cross_raster_vertical_technique = m_effect->GetTechniqueByName("tec_tex2_raster_v");
	m_raster_horizontal_technique = m_effect->GetTechniqueByName("tec_tex1_raster_h");
	m_raster_vertical_technique = m_effect->GetTechniqueByName("tec_tex1_raster_v");
	m_cross_explosion_blur_technique = m_effect->GetTechniqueByName("tec_tex2_explosion_blur");
	m_explosion_blur_technique = m_effect->GetTechniqueByName("tec_tex1_explosion_blur");
	m_cross_mosaic_technique = m_effect->GetTechniqueByName("tec_tex2_mosaic");
	m_mosaic_technique = m_effect->GetTechniqueByName("tec_tex1_mosaic");

	return true;
}

bool C_d3d_default_effect::on_lost_device()
{
	HRESULT hr;

	hr = m_effect->OnLostDevice();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("OnLostDevice"), hr);
		return false;
	}

	return true;
}

bool C_d3d_default_effect::on_reset_device()
{
	HRESULT hr;

	hr = m_effect->OnResetDevice();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("OnResetDevice"), hr);
		return false;
	}

	// �n���h�����Đݒ肷��
	set_effect_handle();

	return true;
}

}
