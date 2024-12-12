#pragma		once

namespace NT3
{

// ****************************************************************
// D3D ライト
// ================================================================
const int	D3D_AMBIENT_TYPE_NONE = 0;
const int	D3D_AMBIENT_TYPE_PLUS = 1;
const int	D3D_AMBIENT_TYPE_MULTIPLE = 2;

class C_d3d_light
{
public:
    D3DLIGHTTYPE    Type;            /* Type of light source */
    D3DCOLORVALUE   Diffuse;         /* Diffuse color of light */
    D3DCOLORVALUE   Specular;        /* Specular color of light */
    D3DCOLORVALUE   Ambient;         /* Ambient color of light */
    D3DVECTOR       Position;         /* Position in world space */
    D3DVECTOR       Direction;        /* Direction in world space */
    float           Range;            /* Cutoff range */
    float           Falloff;          /* Falloff */
    float           Attenuation0;     /* Constant attenuation */
    float           Attenuation1;     /* Linear attenuation */
    float           Attenuation2;     /* Quadratic attenuation */
    float           Theta;            /* Inner angle of spotlight cone */
    float           Phi;              /* Outer angle of spotlight cone */

	int				camera_light_calc_type;			// カメラ光の計算タイプ
	int				ambient_light_calc_type;		// アンビエント光の計算タイプ
};

// ****************************************************************
// D3D ステート
// ================================================================
class C_d3d_state
{
public:

	bool		init();
	bool		set_template();
	void		set_render_state(D3DRENDERSTATETYPE type, DWORD value);
	DWORD		get_render_state(D3DRENDERSTATETYPE type);
	void		set_sampler_state(int stage, D3DSAMPLERSTATETYPE type, DWORD value);

	void		set_light_enable(int light_no, bool on);
	bool		get_light_enable(int light_no);
	void		set_light(int light_no, C_d3d_light* p_light);
	C_d3d_light	get_light(int lignt_no);

	enum	{
		MAX_RENDER_STATE = 256,
		MAX_LIGHT = 8,
		MAX_SHADER = 8,
	};

private:
	DWORD						render_state[MAX_RENDER_STATE];	// レンダーステート
	C_d3d_light					light[MAX_LIGHT];				// ライト
	bool						light_enable[MAX_LIGHT];		// ライト有効無効
};

}
