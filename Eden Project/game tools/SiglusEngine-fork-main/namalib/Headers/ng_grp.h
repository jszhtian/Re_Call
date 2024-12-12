// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°

#ifdef expand_hensuu_ng_data
#	define	NGDATA
#else
#	define	NGDATA		extern
#endif

NGDATA int *CTPCLR;
NGDATA int *CTPBETA;
NGDATA int *CTPMONO;
NGDATA int *CTPREV;
NGDATA int *CTPR, *CTPG, *CTPB;

NGDATA int ENs, ENa, ENh;
NGDATA int ECNT, EALPHA, EBETA;

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°

NGDATA int GRPF_reverse;
NGDATA int GRPF_mono;
NGDATA int GRPF_c_r;
NGDATA int GRPF_c_g;
NGDATA int GRPF_c_b;
NGDATA int GRPF_p_r;
NGDATA int GRPF_p_g;
NGDATA int GRPF_p_b;
NGDATA int GRPF_clr4;
NGDATA int GRPF_clrR;

NGDATA int GRPF_mono_pal;

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°

NGDATA UC *GRPF_d_buf;
NGDATA int GRPF_d_xl;
NGDATA int GRPF_d_yl;
NGDATA UC *GRPF_s_buf;
NGDATA int GRPF_s_xl;
NGDATA int GRPF_s_yl;
NGDATA int GRPF_cnt_y;

NGDATA int GRPF_cnt_x_org;

NGDATA UC *GRPF_t_adr_max;
NGDATA UC *GRPF_t_adr_org;
NGDATA UC *GRPF_t_adr;
NGDATA int GRPF_t_x;
NGDATA int GRPF_t_x_org;
NGDATA int GRPF_t_y;
NGDATA int GRPF_t_xl;
NGDATA int GRPF_t_yl;
NGDATA int GRPF_t_add;
NGDATA int GRPF_t_reverse;
NGDATA int GRPF_t_shift;

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°

NGDATA int RLINEF_dst_alpha;

NGDATA UC *RLINEF_s_buf;
NGDATA int RLINEF_s_xl;
NGDATA int RLINEF_s_yl;

NGDATA int RLINEF_xl;
NGDATA int RLINEF_yl;

NGDATA int RLINEF_xp;
NGDATA int RLINEF_yp;

NGDATA UC *RLINEF_dst;
NGDATA int RLINEF_dst_size;
NGDATA UC *RLINEF_dst_min;
NGDATA UC *RLINEF_dst_max;
NGDATA int RLINEF_d_buf_size;

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°

NGDATA int RGBFILTER_p_r;
NGDATA int RGBFILTER_p_g;
NGDATA int RGBFILTER_p_b;

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°

NGDATA int TCMAG_sxl;
NGDATA int TCMAG_syl;
NGDATA int TCMAG_dxl;
NGDATA int TCMAG_dyl;

NGDATA int TCMAG_x_alpha;
NGDATA int TCMAG_y_alpha;

NGDATA int TCMAG_x_count;
NGDATA int TCMAG_y_count;

NGDATA UC *TCMAG_dst;
NGDATA UC *TCMAG_src;

NGDATA int TCMAG_dst_add_adr;
NGDATA int TCMAG_src_add_adr;

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°

NGDATA UC *LINEF_d_buf;
NGDATA int LINEF_d_xl;
NGDATA int LINEF_d_yl;

NGDATA int LINEF_r;
NGDATA int LINEF_g;
NGDATA int LINEF_b;

NGDATA int LINEF_xl;
NGDATA int LINEF_yl;

NGDATA int LINEF_xp;
NGDATA int LINEF_yp;

NGDATA int LINEF_reverse;
NGDATA int LINEF_mono;
NGDATA int LINEF_c_r;
NGDATA int LINEF_c_g;
NGDATA int LINEF_c_b;
NGDATA int LINEF_p_r;
NGDATA int LINEF_p_g;
NGDATA int LINEF_p_b;
NGDATA int LINEF_clr;
NGDATA int LINEF_clrR;

NGDATA int LINEF_pal;

NGDATA UC *LINEF_t_adr_max;
NGDATA UC *LINEF_t_adr_org;
NGDATA UC *LINEF_t_adr;
NGDATA int LINEF_t_x;
NGDATA int LINEF_t_x_org;
NGDATA int LINEF_t_y;
NGDATA int LINEF_t_xl;
NGDATA int LINEF_t_yl;
NGDATA int LINEF_t_x_add;
NGDATA int LINEF_t_y_add;
NGDATA int LINEF_t_add;
NGDATA int LINEF_t_reverse;
NGDATA int LINEF_t_shift;

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°

NGDATA UC *FILTER_d_buf;
NGDATA int FILTER_d_xl;
NGDATA int FILTER_d_yl;

NGDATA int FILTER_xl;
NGDATA int FILTER_yl;

NGDATA int FILTER_xp;
NGDATA int FILTER_yp;

NGDATA int FILTER_reverse;
NGDATA int FILTER_mono;
NGDATA int FILTER_c_r;
NGDATA int FILTER_c_g;
NGDATA int FILTER_c_b;
NGDATA int FILTER_p_r;
NGDATA int FILTER_p_g;
NGDATA int FILTER_p_b;
NGDATA int FILTER_clr;
NGDATA int FILTER_clr4;

NGDATA int FILTER_pal;

NGDATA UC *FILTER_t_adr_max;
NGDATA UC *FILTER_t_adr_org;
NGDATA UC *FILTER_t_adr;
NGDATA int FILTER_t_x;
NGDATA int FILTER_t_x_org;
NGDATA int FILTER_t_y;
NGDATA int FILTER_t_xl;
NGDATA int FILTER_t_yl;
NGDATA int FILTER_t_x_add;
NGDATA int FILTER_t_y_add;
NGDATA int FILTER_t_add;
NGDATA int FILTER_t_reverse;
NGDATA int FILTER_t_shift;

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°

NGDATA UC *IC_pal;

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°

NGDATA int ICMAG_sxl;
NGDATA int ICMAG_syl;
NGDATA int ICMAG_dxl;
NGDATA int ICMAG_dyl;

NGDATA int ICMAG_x_alpha;
NGDATA int ICMAG_y_alpha;

NGDATA int ICMAG_x_count;
NGDATA int ICMAG_y_count;

NGDATA UC *ICMAG_dst;
NGDATA UC *ICMAG_src;

NGDATA int ICMAG_dst_add_adr;
NGDATA int ICMAG_src_add_adr;

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// ng_sys.c
void ng_set_eria(int xl, int yl, int *x1, int *y1, int *x2, int *y2);

int magnify_param(
	int _sxlen, int _dxlen,
	int sx1, int sx2,
	int dx1, int dx2,
	int _sx1, int _sx2,
	int ex1, int ex2,
	int *ret_x_alpha, int *ret_sx_free,
	int *ret_dx_free, int *ret_x_count
);

int line_buffring(int *buf, int buf_max, int x1, int y1, int x2, int y2);

int y_line_buffring(int *buf, int buf_max, int x1, int y1, int x2, int y2, int mod);

void box_magnify(
	int sx1, int sy1, int sx2, int sy2,
	int dx1, int dy1, int dx2, int dy2,
	int *x1, int *y1, int *x2, int *y2
);

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// ng_tc_line.c
void true_color_line(
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

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// ng_tc_boxfill.c
void true_color_boxfill(
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

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// ng_tc_box.c
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

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// ng_tc_filterline.c
void true_color_filterline(
	UC *d_buf, int d_xl, int d_yl,
	int x1, int y1, int x2, int y2,
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

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// ng_tc_filterboxfill.c
void true_color_filterboxfill(
	UC *d_buf, int d_xl, int d_yl,
	int x1, int y1, int x2, int y2,
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

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// ng_tc_filterbox.c
void true_color_filterbox(
	UC *d_buf, int d_xl, int d_yl,
	int x1, int y1, int x2, int y2,
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

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// ng_tcgd.c
void true_color_grp_disp(
	UC *d_buf, int d_xl, int d_yl,
	UC *s_buf, int s_xl, int s_yl, int s_xc, int s_yc, int type,
	int x, int y,
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

void true_color_grp_disp_eazy(
	UC *d_buf, int d_xl, int d_yl,
	UC *s_buf, int s_xl, int s_yl, int s_xc, int s_yc, int type,
	int x, int y,
	int ex1, int ey1, int ex2, int ey2,
	UC clr
);

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// ng_tcmagnify.c
void true_color_magnify(
	UC *dst_p, int dst_xl, int dst_yl,
	int dx1, int dy1, int dx2, int dy2,
	UC *src_p, int src_xl, int src_yl, int type,
	int sx1, int sy1, int sx2, int sy2,
	int srx, int sry, int sxl, int syl,
	int ex1, int ey1, int ex2, int ey2,
	UC mono,
	UC reverse,
	UC c_r, UC c_g, UC c_b, UC c_rate,
	int p_r, int p_g, int p_b,
	int bright,
	UC clr,
	int disp_mod
);

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// ng_tcrotate.c
void true_color_rotate(
	UC *sp, int sxl, int syl, int sx1, int sy1, int sx2, int sy2, int scx, int scy, int src_type,
	UC *dp, int dxl, int dyl, int dcx, int dcy, int dex1, int dey1, int dex2, int dey2,
	int r, int x_scale, int y_scale,
	UC mono,
	UC reverse,
	UC c_r, UC c_g, UC c_b, UC c_rate,
	int p_r, int p_g, int p_b,
	int bright,
	UC clr,
	int disp_mod,
	int *ret_redisp_x1, int *ret_redisp_y1, int *ret_redisp_x2, int *ret_redisp_y2, int redisp_eria_get_only
);

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// ng_tcgdex_xchg.c
void true_color_grp_disp_xchg(
	UC *dst_p, int dst_xl, int dst_yl, int x, int y,
	UC *src_p, int src_xl, int src_yl, int x1, int y1, int x2, int y2
);

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// ng_tcgdex_mozaiku.c
void true_color_grp_disp_mozaiku(
	UC *dst_p, int dst_xl, int dst_yl, int dx, int dy,
	UC *src_p, int src_xl, int src_yl, int sx1, int sy1, int sx2, int sy2,
	UC rate, UC clr
);

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// ng_tcgdex_eazymask.c
void true_color_grp_disp_eazymask(
	UC *d_buf, int d_xl, int d_yl,
	UC *s_buf, int s_xl, int s_yl, int s_xc, int s_yc,
	int x, int y,
	int ex1, int ey1, int ex2, int ey2,
	UC r, UC g, UC b,
	UC clr
);

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// ng_tcgdex_andor.c
void true_color_grp_disp_andor(
	UC *d_buf, int d_xl, int d_yl,
	UC *s_buf, int s_xl, int s_yl, int s_xc, int s_yc,
	int x, int y,
	int ex1, int ey1, int ex2, int ey2,
	int andor_mod
);

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// ng_index.c
void index_color_grp_disp(
	UC *d_buf, int d_xl, int d_yl,
	UC *s_buf, int s_xl, int s_yl, int s_xc, int s_yc,
	UC *pal, int palcnt,
	int x, int y,
	int clr,
	int ex1, int ey1, int ex2, int ey2
);

// Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
// ng_index_magnify.c
void index_color_magnify(
	UC *dst_p, int dst_xl, int dst_yl,
	int dx1, int dy1, int dx2, int dy2,
	UC *src_p, int src_xl, int src_yl,
	UC *pal, int palcnt,
	int sx1, int sy1, int sx2, int sy2,
	int srx, int sry, int sxl, int syl,
	int ex1, int ey1, int ex2, int ey2,
	UC clr
);

