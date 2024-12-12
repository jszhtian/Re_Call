#pragma once

#include "tonad3d3.h"
// 複数のライトに対応させる為のコード管理を行う


// MBSTR hogehogeVS(MBSTR power, MBSTR vso_hoge);
// if(vso_hoge == "") // 空なら何もしない

// max(0, dot(N, L))
// これはライトの方向ベクトルと法線ベクトルから輝度(brightness)を求める
namespace NT3
{


MBSTR get_brightness_code_vs();

MBSTR get_brightness_code_ps();











}