#include<string.h>
#include<stdio.h>
#include"image_di_impl.h"

/* true で全画面描画終了 */
static void MakeSlideTable(char* table, int dif, int x0, int x1, int x2) {
	int i;
	int fade = 32;
	if (x0 > x2) x0 = x2;
	if (x1 > x2) x1 = x2;
	for (i=0; i<x0; i++) {
		*table = fade;
		table += dif;
	}
	if (x0 < 0) fade += x0;
	for (; i<x1; i++) {
		*table = --fade;
		table += dif;
	}
	for (; i<x2; i++) {
		*table = 0;
		table += dif;
	}
}
bool BlockFadeData::MakeSlideCountTable(int count, int max_count) {
	/* x0 - x1 の範囲で徐々に変化させる */
	int xl = (table_size+32) * count / max_count;
	int xf = xl - 32;
	int dif = direction==UtoD ? 1 : -1;
	char* cur_table = direction==UtoD ? tables : tables+table_size-1;

	MakeSlideTable(cur_table, dif, xf, xl, table_size);

	if (count >= max_count) return true;
	return false;
}

bool BlockFadeData::MakeDiagCountTable(int count, int max_count) {
	DIR hdir;
	if (diag_dir == ULtoDR1 || diag_dir == ULtoDR2 || diag_dir == DLtoUR1 || diag_dir == DLtoUR2) hdir = UtoD;
	else hdir = DtoU;

	/* 元になる縦横の fade table を作る */

	char* htable = tables + ( (hdir == UtoD) ? 0 : blockwidth - 1);;
	int xl = (blockwidth+32) * count / max_count;
	int xf = xl - 32;
	MakeSlideTable(htable, hdir==UtoD ? 1 : -1, xf, xl, blockwidth);

	/* 対角線より上側 / 下側で使用するテーブルを変える */
	htable = tables;
	char* curtable = tables+blockwidth;

	int i,j;
	for (i=1; i<blockheight; i++) {
		int xd = i*blockwidth/blockheight;
		switch(diag_dir){
		case ULtoDR1: case DRtoUL2:
			for (j=0; j<xd; j++) *curtable++ = htable[xd];
			for (; j<blockwidth; j++) *curtable++ = htable[j];
			break;
		case DRtoUL1: case ULtoDR2:
			for (j=0; j<xd; j++) *curtable++ = htable[j];
			for (; j<blockwidth; j++) *curtable++ = htable[xd];
			break;
		case URtoDL1: case DLtoUR2:
			xd = blockwidth - 1 - xd;
			for (j=0; j<xd; j++) *curtable++ = htable[j];
			for (; j<blockwidth; j++) *curtable++ = htable[xd];
			break;
		case DLtoUR1: case URtoDL2:
			xd = blockwidth - 1 - xd;
			for (j=0; j<xd; j++) *curtable++ = htable[xd];
			for (; j<blockwidth; j++) *curtable++ = htable[j];
			break;
		}
	}
	if (diag_dir == DRtoUL1 || diag_dir == ULtoDR2) {
		for (j=0; j<blockwidth; j++) tables[j] = htable[0];
	} else if (diag_dir == DLtoUR1 || diag_dir == URtoDL2) {
		for (j=0; j<blockwidth; j++) tables[j] = htable[blockwidth-1];
	}
	if (count >= max_count) return true;
	else return false;
}
bool BlockFadeData::MakeDiagCountTable2(int count, int max_count) {
	DIR hdir;
	if (diag_dir >= ULtoDR2) { diag_dir = DDIR(diag_dir-4);}
	if (diag_dir == ULtoDR1 || diag_dir == URtoDL1) hdir = UtoD;
	else hdir = DtoU;

	/* 元になる横の fade table を作る */

	char* htable = tables + ( (hdir == UtoD) ? 0 : blockwidth*2 - 1);;
	int xl = (blockwidth*2+32) * count / max_count;
	int xf = xl - 32;
	MakeSlideTable(htable, hdir==UtoD ? 1 : -1, xf, xl, blockwidth*2);

	htable = tables;
	char* curtable = tables+blockwidth*2;

	int i,j;
	for (i=2; i<blockheight; i++) {
		int y0 = i*blockwidth/blockheight;
		if (diag_dir <= DRtoUL1) {
			for (j=0; j<blockwidth; j++) curtable[j] = htable[y0+j];
		} else {
			for (j=0; j<blockwidth; j++) curtable[blockwidth-1-j] = htable[y0+j];
		}
		curtable += blockwidth;
	}
	if (diag_dir <= DRtoUL1) {
		for (j=0; j<blockwidth; j++) tables[j+blockwidth] = tables[j];
	} else {
		for (j=0; j<blockwidth; j++) tables[j+blockwidth] = tables[blockwidth-1-j];
		for (j=0; j<blockwidth; j++) tables[j] = tables[j+blockwidth];
	}
	if (count >= max_count) return true;
	else return false;
}

#if 0 /* obsolete */
static void CalcDifTable(char* table, char* old_table, const FadeTableOrig** dif_table, int table_size, int& ret_min_count, int& ret_max_count) {
	int i;
	int min_flag = 1;
	int min_count = 0; int max_count = 0;
	for (i=0; i<table_size; i++) {
		dif_table[i] = FadeTableOrig::DifTable(old_table[i], table[i]);
		old_table[i] = table[i];
		if (dif_table[i] == 0) {
			if (min_flag == 1) min_count = i;
		} else {
			max_count = i;
			min_flag = 0;
		}
	}
	ret_min_count = min_count;
	ret_max_count = max_count;
}
#endif /* obsolete */

BlockFadeData::BlockFadeData(int _blocksize_x, int _blocksize_y, int _x0, int _y0, int _width, int _height) {
	diftables = 0; tables = 0; old_tables = 0;
	width = 0; height = 0; blocksize_x = 1; blocksize_y = 1;
	blockwidth = 0; blockheight = 0; table_size = 0;
	next = 0;
	/* 引数の調整 */
	if (_blocksize_x <= 0) return;
	if (_blocksize_y <= 0) return;
	if (_width <= 0) return;
	if (_height <= 0) return;
	/* instance の初期化 */
	x0 = _x0;
	y0 = _y0;
	width = _width;
	height = _height;
	blocksize_x = _blocksize_x;
	blocksize_y = _blocksize_y;
	blockwidth =  (_width +_blocksize_x-1) / _blocksize_x;
	blockheight = (_height+_blocksize_y-1) / _blocksize_y;
	table_size = blockwidth * blockheight;
	/* table の初期化 */
	tables = new char[table_size];
	old_tables = new char[table_size];
	diftables = new const FadeTableOrig*[table_size];
	memset(old_tables, 0, table_size);
	return;
}
BlockFadeData::~BlockFadeData() {
	if (diftables) delete[] diftables;
	if (tables) delete[] tables;
	if (old_tables) delete[] old_tables;
}

void CalcDifTable(BlockFadeData* instance) {
	if (instance == 0) return;
	int w = instance->blockwidth;
	int h = instance->blockheight;
	char* tables = instance->tables;
	char* old_tables = instance->old_tables;
	const FadeTableOrig** diftables = instance->diftables;
	int i,j; int k=0;
	int min_x = w, max_x = 0, min_y = -1, max_y = 0;
	int min_y_flag = 1;
	for (i=0; i<h; i++) {
		int min_count = -1, max_count = 0, min_flag = 1;
		/* fade table の計算と、有効な table の得られた j の範囲の算出 */
		for (j=0; j<w; j++) {
			diftables[k] = FadeTableOrig::DifTable(old_tables[k], tables[k]);
			old_tables[k] = tables[k];
			if (diftables[k] == 0) {
				if (min_flag == 1) min_count = i;
			} else {
				max_count = j;
				min_flag = 0;
			}
			k++;
		}
		min_count++;
		/* {min|max}_y の更新 */
		if (min_count == w) { /* x 軸の描画なし */
			if (min_y_flag == 1) min_y = i;
		} else {
			max_y = i;
			min_y_flag = 0;
			/* x 軸の min / max の算出 */
			if (min_x > min_count) min_x = min_count;
			if (max_x < max_count) max_x = max_count;
		}
	}
	min_y++;
	/* 描画範囲： min_x <= x <= max_x, min_y <= y <= max_y */
	instance->max_x = max_x;
	instance->min_x = min_x;
	instance->max_y = max_y;
	instance->min_y = min_y;
}
