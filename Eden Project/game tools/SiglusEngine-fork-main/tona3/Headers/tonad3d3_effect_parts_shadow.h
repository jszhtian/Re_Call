#pragma once

#include "tonad3d3.h"


namespace NT3
{

// �����̊֐��͏����I�ɂ͉e�̎�ނɂ���Ĉ������ς���Ă��邩������Ȃ�

// ���_�V�F�[�_�ɂ�����e�����R�[�h�擾
MBSTR getShadowCodeVS(LIGHTING_TYPE lt, SHADING_TYPE st);

// �s�N�Z���V�F�[�_�ɂ�����e�����R�[�h�擾
MBSTR getShadowCodePS(LIGHTING_TYPE lt, SHADING_TYPE st, MBSTR shadow_color_code, MBSTR default_color_code);







}