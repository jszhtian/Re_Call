#include	"pch.h"




/*========================================================================================
	ボックス表示
void true_color_box(
	UC *d_buf, int d_xl, int d_yl,
	int x1, int y1, int x2, int y2,
	UC r, UC g, UC b,
	int ex1, int ey1, int ex2, int ey2,
	int screen_mod,
	UC *tile, UC t_shift, int t_add, int t_xl, int t_yl, int t_repx, int t_repy, int t_reverse,
	UC mono,
	UC reverse,
	UC c_r, UC c_g, UC c_b, UC c_rate,
	int p_r, int p_g, int p_b,
	int bright,
	UC clr,
	int disp_mod
);
		IN		int (x1 - y2) : 終始点
				true_color_grp_disp参照
		OUT		なし
----------------------------------------------------------------------------------------*/
void true_color_box(
	UC *d_buf, int d_xl, int d_yl,
	int x1, int y1, int x2, int y2,
	UC r, UC g, UC b,
	int ex1, int ey1, int ex2, int ey2,
	int screen_mod,
	UC *tile, UC t_shift, int t_add, int t_xl, int t_yl, int t_repx, int t_repy, int t_reverse,
	UC mono,
	UC reverse,
	UC c_r, UC c_g, UC c_b, UC c_rate,
	int p_r, int p_g, int p_b,
	int bright,
	UC clr,
	int disp_mod
){
	int tmp;
	if(y1 > y2){
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}
	true_color_line(
		d_buf, d_xl, d_yl,
		x1, y1, x2, y1,
		r, g, b,
		ex1, ey1, ex2, ey2,
		screen_mod,
		tile, t_shift, t_add, t_xl, t_yl, t_repx, t_repy, t_reverse,
		mono,
		reverse,
		c_r, c_g, c_b, c_rate,
		p_r, p_g, p_b,
		bright,
		clr,
		disp_mod
	);
	if(y1 != y2){
		true_color_line(
			d_buf, d_xl, d_yl,
			x1, y2, x2, y2,
			r, g, b,
			ex1, ey1, ex2, ey2,
			screen_mod,
			tile, t_shift, t_add, t_xl, t_yl, t_repx, t_repy, t_reverse,
			mono,
			reverse,
			c_r, c_g, c_b, c_rate,
			p_r, p_g, p_b,
			bright,
			clr,
			disp_mod
		);
		if((y1+1) != y2){
			true_color_line(
				d_buf, d_xl, d_yl,
				x1, y1+1, x1, y2-1,
				r, g, b,
				ex1, ey1, ex2, ey2,
				screen_mod,
				tile, t_shift, t_add, t_xl, t_yl, t_repx, t_repy, t_reverse,
				mono,
				reverse,
				c_r, c_g, c_b, c_rate,
				p_r, p_g, p_b,
				bright,
				clr,
				disp_mod
			);
			if((y1+1) != (y2-1)){
				true_color_line(
					d_buf, d_xl, d_yl,
					x2, y1+1, x2, y2-1,
					r, g, b,
					ex1, ey1, ex2, ey2,
					screen_mod,
					tile, t_shift, t_add, t_xl, t_yl, t_repx, t_repy, t_reverse,
					mono,
					reverse,
					c_r, c_g, c_b, c_rate,
					p_r, p_g, p_b,
					bright,
					clr,
					disp_mod
				);
			}
		}
	}
}


