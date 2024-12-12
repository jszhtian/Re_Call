#pragma once

#include "tonad3d3.h"


namespace NT3
{

// ここの関数は将来的には影の種類によって引数が変わってくるかもしれない

// 頂点シェーダにおける影生成コード取得
MBSTR getShadowCodeVS(LIGHTING_TYPE lt, SHADING_TYPE st);

// ピクセルシェーダにおける影生成コード取得
MBSTR getShadowCodePS(LIGHTING_TYPE lt, SHADING_TYPE st, MBSTR shadow_color_code, MBSTR default_color_code);







}