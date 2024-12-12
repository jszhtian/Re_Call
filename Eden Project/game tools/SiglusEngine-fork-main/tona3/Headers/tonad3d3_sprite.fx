// An empty material, which simply transforms the vertex and sets the color to white.

//------------------------------------
// state block
//------------------------------------
stateblock default_state = stateblock_state
{
	ColorArg0[0] = Texture;
	ColorArg1[0] = Diffuse;
	ColorOp[0] = Modulate;
	AlphaArg0[0] = Texture;
	AlphaArg1[0] = Diffuse;
	AlphaOp[0] = Modulate;
};

//------------------------------------
// vertex shader
//------------------------------------

matrix	mat_world;
matrix	mat_view_proj;

float3	light_dir;
float4	light_diffuse;
float4	light_ambient;

struct VS_OUTPUT
{
	float4	position	: POSITION;
	float2	texture_uv	: TEXCOORD0;
	float4	diffuse		: COLOR0;
};

// vertex shader does nothing.
float4 vs_fixed(float4 local_pos: POSITION) : POSITION
{
	return local_pos;
}

// vertex shader basic
VS_OUTPUT vs_d2_basic(float4 v_pos: POSITION, float2 v_texture_uv : TEXCOORD0, float4 v_diffuse : COLOR0)
{
	VS_OUTPUT result;
	result.position = v_pos;
	result.position = mul(result.position, mat_world);
	result.position = mul(result.position, mat_view_proj);
	result.texture_uv = v_texture_uv;
	result.diffuse = v_diffuse;

	return result;
}

VS_OUTPUT vs_d3_basic(float4 v_pos: POSITION, float3 v_normal: NORMAL, float2 v_texture_uv : TEXCOORD0, float4 v_diffuse : COLOR0)
{
	VS_OUTPUT result;

	result.position = v_pos;
	result.position = mul(result.position, mat_world);
	result.position = mul(result.position, mat_view_proj);
	result.texture_uv = v_texture_uv;

	// normal
	float3 world_normal = normalize(mul(v_normal, (float3x3)mat_world));

	// set light
	result.diffuse.rgb = v_diffuse * light_diffuse * max(0, dot(world_normal, - light_dir)) + v_diffuse * light_ambient;
	result.diffuse.a = 1.0f;

	return result;
}

VS_OUTPUT vs_basic(float4 v_pos: POSITION, float2 v_texture_uv : TEXCOORD0, float4 v_diffuse : COLOR0)
{
	return vs_d2_basic(v_pos, v_texture_uv, v_diffuse);
}

//------------------------------------
// pixel shader
//------------------------------------

texture Tex00;		// texture 00
texture Tex01;		// texture 01
float4 mono = float4(0.2989f, 0.5886f, 0.1145f, 0.0f);	// mono
float4 c0;			// if sprite ... mono, reverse, bright, dark
float4 c1;			// if sprite ... r, g, b, rate
float4 c2;			// if sprite ... r, g, b
float4 c3;			// if sprite ... tonecurve_no, tonecurve_sat

//------------------------------------
// texture sampler
//------------------------------------

// texture sampler 00
sampler my_sampler_tex00 = sampler_state
{
	Texture = <Tex00>;

	// (d2 | billboard) ? BORDER : WRAP;
};

// texture sampler 01 for mask
sampler my_sampler_tex01_mask = sampler_state
{
	Texture = <Tex01>;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};

// texture sampler 01 for tonecurve
sampler my_sampler_tex01_tonecurve = sampler_state
{
	Texture = <Tex01>;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};

//------------------------------------
// pixel shader functions
//------------------------------------

float4 ps_func_mrbd(float4 color_org)
{
	float4 color = color_org;

	// reverse
	float4 reverse = 1.0f - color_org;
	color = lerp(color, reverse, c0.y);

	// mono
	float mono_y = dot(mono, color_org);
	color = lerp(color, mono_y, c0.x);

	// bright
	color = color + c0.z;

 	// dark
	color = color - c0.w;

	// turn back alpha
	color.a = color_org.a;

	// return color
	return color;
}

float4 ps_func_rgb(float4 color_org)
{
	float4 color = color_org;

	// rgb
	color = lerp(color, c1, c1.w);

	// rgbrep
	color += c2;

	// turn back alpha
	color.a = color_org.a;

	// return color
	return color;
}

float4 ps_func_mul(float4 color_org)
{
	// mul
	float4 color = lerp(1.0f, color_org, color_org.a);

	// turn back alpha
	color.a = color_org.a;

	// return color
	return color;
}

float4 ps_func_tonecurve(float4 color_org)
{
	float2 tonecurve_pos;
	float4 tonecurve_color;

	float4 color = color_org;

	// tonecurve sat
	float mono_y = dot(mono, color_org);
	color = lerp(color, mono_y, c3.g);

	// tonecurve r
	tonecurve_pos = float2(color.r, c3.r);
	tonecurve_color = tex2D(my_sampler_tex01_tonecurve, tonecurve_pos);
	color.r = tonecurve_color.r;

	// tonecurve g
	tonecurve_pos = float2(color.g, c3.r);
	tonecurve_color = tex2D(my_sampler_tex01_tonecurve, tonecurve_pos);
	color.g = tonecurve_color.g;

	// tonecurve b
	tonecurve_pos = float2(color.b, c3.r);
	tonecurve_color = tex2D(my_sampler_tex01_tonecurve, tonecurve_pos);
	color.b = tonecurve_color.b;

	// turn back alpha
	color.a = color_org.a;

	// return color
	return color;
}

//------------------------------------
// pixel shader main
//------------------------------------

//------------------------------------
// tex0
//------------------------------------

// pixel shader / tex0
float4 ps_tex0(float4 diffuse : COLOR0) : COLOR0
{
	return diffuse;
}

// pixel shader / tex0, diffuse
float4 ps_tex0_diffuse(float4 diffuse : COLOR0) : COLOR0
{
	return diffuse;
}

// pixel shader / tex0, mrbd, rgb
float4 ps_tex0_mrbd_rgb(float4 diffuse : COLOR0) : COLOR0
{
	return ps_func_rgb(ps_func_mrbd(diffuse));
}

// pixel shader / tex0, diffuse, mrbd, rgb
float4 ps_tex0_diffuse_mrbd_rgb(float4 diffuse : COLOR0) : COLOR0
{
	return ps_func_rgb(ps_func_mrbd(diffuse));
}

//------------------------------------
// tex0 - mul
//------------------------------------

// pixel shader / tex0, mul
float4 ps_tex0_mul(float4 diffuse : COLOR0) : COLOR0
{
	return ps_func_mul(diffuse);
}

// pixel shader / tex0, diffuse, mul
float4 ps_tex0_diffuse_mul(float4 diffuse : COLOR0) : COLOR0
{
	return ps_func_mul(diffuse);
}

// pixel shader / tex0, mrbd, rgb, mul
float4 ps_tex0_mrbd_rgb_mul(float4 diffuse : COLOR0) : COLOR0
{
	return ps_func_mul(ps_func_rgb(ps_func_mrbd(diffuse)));
}

// pixel shader / tex0, diffuse, mrbd, rgb, mul
float4 ps_tex0_diffuse_mrbd_rgb_mul(float4 diffuse : COLOR0) : COLOR0
{
	return ps_func_mul(ps_func_rgb(ps_func_mrbd(diffuse)));
}

//------------------------------------
// tex1
//------------------------------------

// pixel shader / tex1
float4 ps_tex1(float2 texcoord0 : TEXCOORD0) : COLOR0
{
	return tex2D(my_sampler_tex00, texcoord0);
}

// pixel shader / tex1, diffuse
float4 ps_tex1_diffuse(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0) : COLOR0
{
	return tex2D(my_sampler_tex00, texcoord0) * diffuse;
}

// pixel shader / tex1, mrbd, rgb
float4 ps_tex1_mrbd_rgb(float2 texcoord0 : TEXCOORD0) : COLOR0
{
	return ps_func_rgb(ps_func_mrbd(tex2D(my_sampler_tex00, texcoord0)));
}

// pixel shader / tex1, diffuse, mrbd, rgb
float4 ps_tex1_diffuse_mrbd_rgb(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0) : COLOR0
{
	return ps_func_rgb(ps_func_mrbd(tex2D(my_sampler_tex00, texcoord0) * diffuse));
}

//------------------------------------
// tex1 - mul
//------------------------------------

// pixel shader / tex1, mul
float4 ps_tex1_mul(float2 texcoord0 : TEXCOORD0) : COLOR0
{
	return ps_func_mul(tex2D(my_sampler_tex00, texcoord0));
}

// pixel shader / tex1, diffuse, mul
float4 ps_tex1_diffuse_mul(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0) : COLOR0
{
	return ps_func_mul(tex2D(my_sampler_tex00, texcoord0) * diffuse);
}

// pixel shader / tex1, mrbd, rgb, mul
float4 ps_tex1_mrbd_rgb_mul(float2 texcoord0 : TEXCOORD0) : COLOR0
{
	return ps_func_mul(ps_func_rgb(ps_func_mrbd(tex2D(my_sampler_tex00, texcoord0))));
}

// pixel shader / tex1, diffuse, mrbd, rgb, mul
float4 ps_tex1_diffuse_mrbd_rgb_mul(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0) : COLOR0
{
	return ps_func_mul(ps_func_rgb(ps_func_mrbd(tex2D(my_sampler_tex00, texcoord0) * diffuse)));
}

//------------------------------------
// tex2 - tonecurve
//------------------------------------

// pixel shader / tex2, tonecurve
float4 ps_tex2_tonecurve(float2 texcoord0 : TEXCOORD0) : COLOR0
{
	return ps_func_tonecurve(tex2D(my_sampler_tex00, texcoord0));
}

// pixel shader / tex2, diffuse, tonecurve
float4 ps_tex2_diffuse_tonecurve(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0) : COLOR0
{
	return ps_func_tonecurve(tex2D(my_sampler_tex00, texcoord0) * diffuse);
}

// pixel shader / tex2, mrbd, rgb, tonecurve
float4 ps_tex2_mrbd_rgb_tonecurve(float2 texcoord0 : TEXCOORD0) : COLOR0
{
	return ps_func_rgb(ps_func_mrbd(ps_func_tonecurve(tex2D(my_sampler_tex00, texcoord0))));
}

// pixel shader / tex2, diffuse, mrbd, rgb, tonecurve
float4 ps_tex2_diffuse_mrbd_rgb_tonecurve(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0) : COLOR0
{
	return ps_func_rgb(ps_func_mrbd(ps_func_tonecurve(tex2D(my_sampler_tex00, texcoord0) * diffuse)));
}

//------------------------------------
// tex2 - mul, tonecurve
//------------------------------------

// pixel shader / tex2, mul, tonecurve
float4 ps_tex2_mul_tonecurve(float2 texcoord0 : TEXCOORD0) : COLOR0
{
	return ps_func_mul(ps_func_tonecurve(tex2D(my_sampler_tex00, texcoord0)));
}

// pixel shader / tex2, diffuse, mul, tonecurve
float4 ps_tex2_diffuse_mul_tonecurve(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0) : COLOR0
{
	return ps_func_mul(ps_func_tonecurve(tex2D(my_sampler_tex00, texcoord0) * diffuse));
}

// pixel shader / tex2, mrbd, rgb, mul, tonecurve
float4 ps_tex2_mrbd_rgb_mul_tonecurve(float2 texcoord0 : TEXCOORD0) : COLOR0
{
	return ps_func_mul(ps_func_rgb(ps_func_mrbd(ps_func_tonecurve(tex2D(my_sampler_tex00, texcoord0)))));
}

// pixel shader / tex2, diffuse, mrbd, rgb, mul, tonecurve
float4 ps_tex2_diffuse_mrbd_rgb_mul_tonecurve(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0) : COLOR0
{
	return ps_func_mul(ps_func_rgb(ps_func_mrbd(ps_func_tonecurve(tex2D(my_sampler_tex00, texcoord0) * diffuse))));
}

//------------------------------------
// tex2 - mask
//------------------------------------

// pixel shader / tex2 - mask
float4 ps_tex2_mask(float4 diffuse : COLOR0, float2 texcoord0 : TEXCOORD0, float2 texcoord1 : TEXCOORD1) : COLOR0
{
	float4 color;
	float4 temp;
	float4 tex = tex2D(my_sampler_tex00, texcoord0);
	float4 mask = tex2D(my_sampler_tex01_mask, texcoord1);

	// c0: wipe progress (0 - 1)
	// c1: fade effect (0 - 1) -> (256 - 2)
	
	float fade = lerp(256.0f, 2.0f, c0.r);
	color = (c0 * fade + mask * (fade - 1.0f) - (fade - 1.0f)) * tex;
	color.bgr = tex.bgr;

	return color;
}



//------------------------------------
// technique
//------------------------------------

//------------------------------------
// tex0
//------------------------------------
technique tec_tex0
{
	pass p0
	{
		StateBlock = (default_state);
		VertexShader = compile vs_2_0 vs_d2_basic();
		PixelShader = compile ps_2_0 ps_tex0();
	}
}

technique tec_tex0_diffuse
{
	pass p0
	{
		StateBlock = (default_state);
		VertexShader = compile vs_2_0 vs_d2_basic();
		PixelShader = compile ps_2_0 ps_tex0_diffuse();
	}
}

technique tec_tex0_mrbd_rgb
{
    pass p0
    {
		StateBlock = (default_state);
		VertexShader = compile vs_2_0 vs_basic();
		PixelShader = compile ps_2_0 ps_tex0_mrbd_rgb();
    }
}

technique tec_tex0_diffuse_mrbd_rgb
{
    pass p0
    {
		StateBlock = (default_state);
		VertexShader = compile vs_2_0 vs_basic();
		PixelShader = compile ps_2_0 ps_tex0_diffuse_mrbd_rgb();
    }
}

//------------------------------------
// tex0 - mul
//------------------------------------
technique tec_tex0_mul
{
    pass p0
    {
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex0_mul();
    }
}

technique tec_tex0_diffuse_mul
{
    pass p0
    {
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex0_diffuse_mul();
    }
}

technique tec_tex0_mrbd_rgb_mul
{
    pass p0
    {
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex0_mrbd_rgb_mul();
    }
}

technique tec_tex0_diffuse_mrbd_rgb_mul
{
    pass p0
    {
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex0_diffuse_mrbd_rgb_mul();
    }
}

//------------------------------------
// tex1
//------------------------------------
technique tec_tex1
{
	pass p0
	{
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex1();
	}
}

technique tec_tex1_diffuse
{
	pass p0
	{
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex1_diffuse();
	}
}

technique tec_tex1_mrbd_rgb
{
	pass p0
	{
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex1_mrbd_rgb();
	}
}

technique tec_tex1_diffuse_mrbd_rgb
{
	pass p0
	{
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex1_diffuse_mrbd_rgb();
	}
}

//------------------------------------
// tex1 - mul
//------------------------------------
technique tec_tex1_mul
{
    pass p0
    {
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex1_mul();
    }
}

technique tec_tex1_diffuse_mul
{
    pass p0
    {
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex1_diffuse_mul();
    }
}

technique tec_tex1_mrbd_rgb_mul
{
    pass p0
    {
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex1_mrbd_rgb_mul();
    }
}

technique tec_tex1_diffuse_mrbd_rgb_mul
{
    pass p0
    {
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex1_diffuse_mrbd_rgb_mul();
    }
}

//------------------------------------
// tex2 - tonecurve
//------------------------------------
technique tec_tex2_tonecurve
{
	pass p0
	{
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex2_tonecurve();
	}
}

technique tec_tex2_diffuse_tonecurve
{
	pass p0
	{
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex2_diffuse_tonecurve();
	}
}

technique tec_tex2_mrbd_rgb_tonecurve
{
	pass p0
	{
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex2_mrbd_rgb_tonecurve();
	}
}

technique tec_tex2_diffuse_mrbd_rgb_tonecurve
{
	pass p0
	{
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex2_diffuse_mrbd_rgb_tonecurve();
	}
}

//------------------------------------
// tex2 - tonecurve, mul
//------------------------------------
technique tec_tex2_tonecurve_mul
{
    pass p0
    {
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex2_mul_tonecurve();
    }
}

technique tec_tex2_diffuse_tonecurve_mul
{
    pass p0
    {
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex2_diffuse_mul_tonecurve();
    }
}

technique tec_tex2_mrbd_rgb_tonecurve_mul
{
    pass p0
    {
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex2_mrbd_rgb_mul_tonecurve();
    }
}

technique tec_tex2_diffuse_mrbd_rgb_tonecurve_mul
{
    pass p0
    {
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex2_diffuse_mrbd_rgb_mul_tonecurve();
    }
}

//------------------------------------
// tex2 - mask
//------------------------------------
technique tec_tex2_mask
{
	pass p0
	{
		StateBlock = (default_state);
		PixelShader = compile ps_2_0 ps_tex2_mask();
	}
}

//------------------------------------
// tex0 - d3
//------------------------------------
technique tec_tex0_d3
{
	pass p0
	{
		StateBlock = (default_state);
		VertexShader = compile vs_2_0 vs_d3_basic();
		PixelShader = compile ps_2_0 ps_tex0();
	}
}

technique tec_tex0_diffuse_d3
{
	pass p0
	{
		StateBlock = (default_state);
		VertexShader = compile vs_2_0 vs_d3_basic();
		PixelShader = compile ps_2_0 ps_tex0_diffuse();
	}
}

technique tec_tex0_mrbd_rgb_d3
{
    pass p0
    {
		StateBlock = (default_state);
		VertexShader = compile vs_2_0 vs_d3_basic();
		PixelShader = compile ps_2_0 ps_tex0_mrbd_rgb();
    }
}

technique tec_tex0_diffuse_mrbd_rgb_d3
{
    pass p0
    {
		StateBlock = (default_state);
		VertexShader = compile vs_2_0 vs_d3_basic();
		PixelShader = compile ps_2_0 ps_tex0_diffuse_mrbd_rgb();
    }
}


//------------------------------------
// tex1 - d3
//------------------------------------
technique tec_tex1_d3
{
	pass p0
	{
		StateBlock = (default_state);
		VertexShader = compile vs_2_0 vs_d3_basic();
		PixelShader = compile ps_2_0 ps_tex1();
	}
}

technique tec_tex1_diffuse_d3
{
	pass p0
	{
		StateBlock = (default_state);
		VertexShader = compile vs_2_0 vs_d3_basic();
		PixelShader = compile ps_2_0 ps_tex1_diffuse();
	}
}

technique tec_tex1_mrbd_rgb_d3
{
	pass p0
	{
		StateBlock = (default_state);
		VertexShader = compile vs_2_0 vs_d3_basic();
		PixelShader = compile ps_2_0 ps_tex1_mrbd_rgb();
	}
}

technique tec_tex1_diffuse_mrbd_rgb_d3
{
	pass p0
	{
		StateBlock = (default_state);
		VertexShader = compile vs_2_0 vs_d3_basic();
		PixelShader = compile ps_2_0 ps_tex1_diffuse_mrbd_rgb();
	}
}

//------------------------------------
// tex1 - mul - d3
//------------------------------------
technique tec_tex1_mul_d3
{
    pass p0
    {
		StateBlock = (default_state);
		VertexShader = compile vs_2_0 vs_d3_basic();
		PixelShader = compile ps_2_0 ps_tex1_mul();
    }
}

technique tec_tex1_diffuse_mul_d3
{
    pass p0
    {
		StateBlock = (default_state);
		VertexShader = compile vs_2_0 vs_d3_basic();
		PixelShader = compile ps_2_0 ps_tex1_diffuse_mul();
    }
}

technique tec_tex1_mrbd_rgb_mul_d3
{
    pass p0
    {
		StateBlock = (default_state);
		VertexShader = compile vs_2_0 vs_d3_basic();
		PixelShader = compile ps_2_0 ps_tex1_mrbd_rgb_mul();
    }
}

technique tec_tex1_diffuse_mrbd_rgb_mul_d3
{
    pass p0
    {
		StateBlock = (default_state);
		VertexShader = compile vs_2_0 vs_d3_basic();
		PixelShader = compile ps_2_0 ps_tex1_diffuse_mrbd_rgb_mul();
    }
}

