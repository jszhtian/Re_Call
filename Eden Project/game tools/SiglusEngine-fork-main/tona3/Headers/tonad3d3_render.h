#pragma once

#include "tonad3d3_render_interface.h"

namespace NT3
{




class C_d3d_render_default : public C_d3d_render_interface {
public:
	C_d3d_render_default(){};
	~C_d3d_render_default(){};
private:
};



// とりあえず仮で作成する輝度描画用クラス群
// C_d3d_render_defaultでデフォルト(カラー情報)描画後にRTを切り替えてから使用される予定
class C_d3d_render_bright : public C_d3d_render_interface {
public:
	C_d3d_render_bright(){};
	~C_d3d_render_bright(){};
private:
};

// マルチレンダーターゲットを利用してデフォルト(カラー情報)描画と輝度描画を同時に行う
class C_d3d_render_multi_default_bright : public C_d3d_render_interface {
public:
	C_d3d_render_multi_default_bright(){};
	~C_d3d_render_multi_default_bright(){};
private:
};









}