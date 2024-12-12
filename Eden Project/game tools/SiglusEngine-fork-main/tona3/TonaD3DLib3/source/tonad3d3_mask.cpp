#include	"tonad3d3_pch.h"
#include	"tonad3d3_texture.h"
#include	"tonad3d3_mask.h"

// ****************************************************************
// Ç∆Ç»ÇR
// ================================================================
namespace NT3
{
	// ****************************************************************
	// D3D É}ÉXÉN
	// ================================================================
	C_d3d_mask::C_d3d_mask()
	{
		m_param.x = 0;
		m_param.y = 0;
	}

	C_d3d_mask::~C_d3d_mask()
	{
	}

	void C_d3d_mask::init()
	{
		m_param.x = 0;
		m_param.y = 0;

		m_texture.reset();
	}
}


