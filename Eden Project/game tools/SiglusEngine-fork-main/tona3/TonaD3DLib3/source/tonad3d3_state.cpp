#include	"tonad3d3_pch.h"
#include	"tonad3d3_d3d.h"
#include	"tonad3d3_device.h"
#include	"tonad3d3_state.h"

namespace NT3
{

// ****************************************************************
// �X�e�[�g�̏�����
// ================================================================
bool C_d3d_state::init()
{
	// �����_�[�X�e�[�g�̏�����
	for (int d = 0; d < 256; d ++)
		render_state[d] = (DWORD)(-1);

	return true;
}

// ****************************************************************
// �X�e�[�g�Ƀe���v���[�g��^����
// ================================================================
bool C_d3d_state::set_template()
{
	// �[�x�o�b�t�@�i�L���j
	set_render_state(D3DRS_ZENABLE, D3DZB_TRUE);
	set_render_state(D3DRS_ZWRITEENABLE, TRUE);

	// �[�x�o�b�t�@�̔�r�֐�
	set_render_state(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// �t�B�����O�i�ʁj
	set_render_state(D3DRS_FILLMODE, D3DFILL_SOLID);

	// �V�F�[�f�B���O�i�O�[���[�j
	set_render_state(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	// �A���t�@�e�X�g�i�L���A�P�ȏオ�ʉ߁j
	set_render_state(D3DRS_ALPHATESTENABLE, TRUE);
	set_render_state(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	set_render_state(D3DRS_ALPHAREF, 0x01);

	// �A���t�@�u�����h�i�L���j
	set_render_state(D3DRS_ALPHABLENDENABLE, TRUE);
	set_render_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	set_render_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	// �A���t�@�l�̃A���t�@�u�����h�i�L���j
	set_render_state(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	set_render_state(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	set_render_state(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
	set_render_state(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);

	// �J�����O�i�����j
	set_render_state(D3DRS_CULLMODE, D3DCULL_CCW);

	// ���C�e�B���O���[�h�i�����j
	set_render_state(D3DRS_LIGHTING, FALSE);

	// �A���r�G���g��(0�j
	set_render_state(D3DRS_AMBIENT, 0);

	// �@���������I�ɐ��K��
	set_render_state(D3DRS_NORMALIZENORMALS, TRUE);

	// �}���`�T���v�����O�̐ݒ�
	set_render_state(D3DRS_MULTISAMPLEANTIALIAS, G_d3d.device.pp()->MultiSampleType ? TRUE : FALSE);

	// �e�N�X�`���X�e�[�W�̐ݒ�
	G_d3d.device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);			// RGB = ARG1 * ARG2
	G_d3d.device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);			// ARG1 = �e�N�X�`��
	G_d3d.device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);			// ARG2 = �f�B�t���[�Y�F
	G_d3d.device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);			// A = ARG1 * ARG2
	G_d3d.device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);			// ARG1 = �e�N�X�`��
	G_d3d.device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);			// ARG2 = �f�B�t���[�Y�F
	G_d3d.device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);			// RGB = ARG1 * ARG2
	G_d3d.device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);			// ARG1 = �e�N�X�`��
	G_d3d.device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);			// ARG2 = �f�B�t���[�Y�F
	G_d3d.device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);			// A = ARG1 * ARG2
	G_d3d.device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);			// ARG1 = �e�N�X�`��
	G_d3d.device->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);			// ARG2 = �f�B�t���[�Y�F

	// �e�N�X�`���t�B���^�����O�i���j�A�j
	for (int i = 0; i < 4; i++)	{
		G_d3d.device->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		G_d3d.device->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

		// �X�e�[�W�O�̃~�b�v�}�b�v�̃t�B���^�� D3DTEXF_LINEAR�B
		// �X�e�[�W�P�̓}�X�N���C�v�^�g�[���J�[�u��p�B�~�b�v�}�b�v���g�p���Ȃ��B
		if (i == 0)	{

			// �g���Ȃ��ꍇ�͎����I�ɉ������邱�Ƃɒ���
			set_sampler_state(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		}
		else	{

			// �~�b�v�}�b�v���g�p���Ȃ�
			set_sampler_state(i, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		}
	}

	// �e�N�X�`�����b�s���O���[�h
	for (int i = 0; i < 8; i ++)	{
		G_d3d.state.set_sampler_state(i, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(i, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		G_d3d.state.set_sampler_state(i, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
	}

	// ���C�g�̏�����
#if 0
	for (int l = 0; l < MAX_LIGHT; l++)	{
		ZeroMemory(&light[l], sizeof(light[l]));

		// �f�t�H���g�F���̐^�ォ��̒�����
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

	// ���C�g�L�������̏�����
#if 0
	for (int l = 0; l < MAX_LIGHT; l++)	{
		G_d3d.device->LightEnable(l, FALSE);
		light_enable[l] = false;
	}
#endif

	return true;
}

// ****************************************************************
// �T���v���[�X�e�[�g�̐ݒ�
// ================================================================
void C_d3d_state::set_sampler_state(int stage, D3DSAMPLERSTATETYPE type, DWORD value)
{
	// �O���{�̑Ή����`�F�b�N������B
	// �~�b�v�}�b�v�����������ł��Ȃ����ł� D3DTEXF_NONE �ɂ��邱�Ƃɒ��ӁB
	// Intel 82915 �� NONE �ɂȂ�B

	// �~�b�v�t�B���^�[�̏ꍇ�͎g���邩�𔻒�
	if (type == D3DSAMP_MIPFILTER)	{

		if (value == D3DTEXF_LINEAR)	{
			if (G_d3d.device.can_set_D3DPTFILTERCAPS_MIPFLINEAR_for_texture())	{
				G_d3d.device->SetSamplerState(stage, type, value);
			}
			else	{
				value = D3DTEXF_POINT;		// �g���Ȃ��̂� D3DTEXF_POINT �����݂�
			}
		}
		if (value == D3DTEXF_POINT)	{
			if (G_d3d.device.can_set_D3DPTFILTERCAPS_MIPFPOINT_for_texture())	{
				G_d3d.device->SetSamplerState(stage, type, value);
			}
			else	{
				value = D3DTEXF_NONE;		// �g���Ȃ��̂� D3DTEXF_NONE �����݂�
			}
		}
		if (value == D3DTEXF_NONE)	{
			G_d3d.device->SetSamplerState(stage, type, value);	// �K���g����
		}
	}

	// �A�h���b�V���O���[�h�̏ꍇ���g���邩����
	else if (type == D3DSAMP_ADDRESSU || type == D3DSAMP_ADDRESSV || type == D3DSAMP_ADDRESSW)	{

		if (value == D3DTADDRESS_BORDER)	{
			if (G_d3d.device.device_caps().TextureAddressCaps & D3DPTADDRESSCAPS_BORDER)	{
				G_d3d.device->SetSamplerState(stage, type, value);
			}
			else	{
				value = D3DTADDRESS_CLAMP;	// �g���Ȃ��̂� D3DTADDRESS_CLAMP �����݂�
			}
		}
		if (value == D3DTADDRESS_CLAMP)	{
			G_d3d.device->SetSamplerState(stage, type, value);	// ���Ԃ�g����
		}
		if (value == D3DTADDRESS_MIRROR)	{
			G_d3d.device->SetSamplerState(stage, type, value);	// ���Ԃ�g����
		}
		if (value ==  D3DTADDRESS_WRAP)	{
			G_d3d.device->SetSamplerState(stage, type, value);	// ���Ԃ�g����
		}
	}

	// ����ȊO�̏ꍇ�͕��ʂɎg��
	else	{
		G_d3d.device->SetSamplerState(stage, type, value);
	}
}

// ****************************************************************
// �����_�[�X�e�[�g�̐ݒ�
// ================================================================
void C_d3d_state::set_render_state(D3DRENDERSTATETYPE type, DWORD value)
{
	// �l���ύX���ꂽ�ꍇ�̂ݏ������s��
	if (render_state[type] != value)	{

		// �l�̐ݒ�
		render_state[type] = value;
		G_d3d.device->SetRenderState(type, value);
	}
}

// ****************************************************************
// �����_�[�X�e�[�g�̎擾
// ================================================================
DWORD C_d3d_state::get_render_state(D3DRENDERSTATETYPE type)
{
	return render_state[type];
}

// ****************************************************************
// ���C�g�L�������̐ݒ�
// ================================================================
void C_d3d_state::set_light_enable(int light_no, bool on)
{
	// �l���ύX���ꂽ�ꍇ�̂ݏ������s��
	if (light_enable[light_no] != on)	{

		// �l�̐ݒ�
		light_enable[light_no] = on;
		G_d3d.device->LightEnable(light_no, on ? TRUE : FALSE);
	}
}

// ****************************************************************
// ���C�g�L�������̐ݒ�
// ================================================================
bool C_d3d_state::get_light_enable(int light_no)
{
	return light_enable[light_no];
}

// ****************************************************************
// ���C�g�̐ݒ�
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
