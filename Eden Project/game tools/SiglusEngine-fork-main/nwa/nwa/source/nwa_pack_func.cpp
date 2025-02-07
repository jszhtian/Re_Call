#include	"pch.h"
#include	"nwa_pack.h"

// ****************************************************************
// ＮＷＡ圧縮 圧縮関数 本体
// ================================================================
DWORD Cnwa_pack::_pack_func_pack(BYTE* src_p, DWORD src_size)
{
	_pack.src_p = src_p;								// ソースデータポインタ
	_pack.src_end = (UINT_PTR)(src_p + src_size);		// ソースデータ終了位置

	_pack.dst.resize(src_size * 2);						// 出力先バッファ（安全の為ソースの２倍確保）
	_pack.dst_p = &_pack.dst[0];						// 出力先バッファポインタ
	ZeroMemory(_pack.dst_p, _pack.dst.size());			// クリア（必須！）

	_pack.dst_bit_size = 0;								// 出力先バッファのビットサイズ
	_pack.zero_cnt = 0;									// ＺＥＲＯの数
	_pack.transition = 0;								// サンプル値の変移

	if (_header.bits_per_sample == 16)	{
		if (_header.channels == 1)		{	_pack_func_pack16m();	}
		else if (_header.channels == 2)	{	_pack_func_pack16s();	}
	}

//	DWORD ret_size = sizeof(NWA_HEADER_STRUCT) + (_pack.dst_bit_size / 8) - 4;		// 間違った旧版ステレオ（ヘッダサイズは余計でした）-4は頭のサンプル分
//	DWORD ret_size = sizeof(NWA_HEADER_STRUCT) + (_pack.dst_bit_size / 8) - 2;		// 間違った旧版モノラル（ヘッダサイズは余計でした）-2は頭のサンプル分

	DWORD ret_size = (_pack.dst_bit_size / 8);
	if (_pack.dst_bit_size & 0x07)	{	ret_size++;		}
	return ret_size;
}

// ****************************************************************
// ＮＷＡ圧縮 圧縮関数 １６ビット・モノラル
// ================================================================
void Cnwa_pack::_pack_func_pack16m()
{
	// 初期サンプル値
	int nowsmp = *(short*)_pack.src_p;		_pack.src_p += 2;

	short* p = (short*)(_pack.dst_p + (_pack.dst_bit_size / 8));
	*p = nowsmp;	_pack.dst_bit_size += (2 * 8);

	// ＺＥＲＯ
	if (_header.zero_mod)	{	_pack.zero_cnt = 1;		}
	else					{	_pack.zero_cnt = 0;	_pack_func_set_bit_zero_one();	}

	while (1)	{
		if ((UINT_PTR)_pack.src_p == _pack.src_end)	{	break;	}
		_pack_func_pack16(&nowsmp);
	}
}

// ****************************************************************
// ＮＷＡ圧縮 圧縮関数 １６ビット・ステレオ
// ================================================================
void Cnwa_pack::_pack_func_pack16s()
{
	// 初期サンプル値
	int nowsmp_l = *(short*)_pack.src_p;	_pack.src_p += 2;
	int nowsmp_r = *(short*)_pack.src_p;	_pack.src_p += 2;

	short* p = (short*)(_pack.dst_p + (_pack.dst_bit_size / 8));
	*p++ = nowsmp_l;	_pack.dst_bit_size += (2 * 8);
	*p   = nowsmp_r;	_pack.dst_bit_size += (2 * 8);

	// ＺＥＲＯ
	if (_header.zero_mod)	{	_pack.zero_cnt = 2;		}
	else					{	_pack.zero_cnt = 0;	_pack_func_set_bit_zero_one();	_pack_func_set_bit_zero_one();	}

	while (1)	{
		if ((UINT_PTR)_pack.src_p == _pack.src_end)	{	break;	}
		_pack_func_pack16(&nowsmp_l);
		_pack_func_pack16(&nowsmp_r);
	}
}

// ****************************************************************
// ＮＷＡ圧縮 圧縮関数 １６ビット
// ================================================================
void Cnwa_pack::_pack_func_pack16(int *nowsmp)
{
	int smp = *(short*)_pack.src_p;

	if (*nowsmp > smp)	{
		_pack.transition = *nowsmp - smp;
		if (!_pack_func_zero_proc())	{
			(this->*_pack_func_dat_set[_header.pack_mod])(1);
			*nowsmp -= _pack.transition;
		}
	}
	else if(*nowsmp < smp){
		_pack.transition = smp - *nowsmp;
		if (!_pack_func_zero_proc())	{
			(this->*_pack_func_dat_set[_header.pack_mod])(0);
			*nowsmp += _pack.transition;
		}
	}
	else	{	_pack_func_zero_set();	}

	_pack.src_p += 2;
}

// ****************************************************************
// ＮＷＡ圧縮 圧縮関数 ビットフラグ設定
// ================================================================
void Cnwa_pack::_pack_func_set_bit(DWORD add_bit_cnt, DWORD set_bit_dat)
{
	WORD* p = (WORD*)(_pack.dst_p + (_pack.dst_bit_size / 8));
	*p |= (WORD)(set_bit_dat << (_pack.dst_bit_size & 0x07));
	_pack.dst_bit_size += add_bit_cnt;
}

// ****************************************************************
// ＮＷＡ圧縮 圧縮関数 ＺＥＲＯプロセス
// ================================================================
bool Cnwa_pack::_pack_func_zero_proc()
{
	if (_pack_func_zero_check())	{
		_pack_func_zero_set();
		return true;
	}
	else	{
		if (_pack.zero_cnt)	{	_pack_func_set_bit_zero();	}
		return false;
	}
}

// ****************************************************************
// ＮＷＡ圧縮 圧縮関数 ＺＥＲＯ判定
// ================================================================
bool Cnwa_pack::_pack_func_zero_check()
{
	     if (_header.pack_mod == 0 && _pack.transition < 64)	{	return true;	}
	else if (_header.pack_mod == 1 && _pack.transition < 32)	{	return true;	}
	else if (_header.pack_mod == 2 && _pack.transition < 16)	{	return true;	}
	else if (_header.pack_mod == 3 && _pack.transition <  8)	{	return true;	}
	else if (_header.pack_mod == 4 && _pack.transition <  4)	{	return true;	}
	else if (_header.pack_mod == 5 && _pack.transition <  2)	{	return true;	}
	return false;
}

// ****************************************************************
// ＮＷＡ圧縮 圧縮関数 ＺＥＲＯ設定
// ================================================================
void Cnwa_pack::_pack_func_zero_set()
{
	if (_header.zero_mod)	{
		_pack.zero_cnt++;
		if (_pack.zero_cnt == 256)	{
			_pack_func_set_bit_zero();
		}
	}
	else	{
		_pack_func_set_bit_zero_one();
	}
}

// ****************************************************************
// ＮＷＡ圧縮 圧縮関数 ＺＥＲＯフラグ１つを設定
// ================================================================
void Cnwa_pack::_pack_func_set_bit_zero_one()
{
	_pack_func_set_bit(3, 0);
}

// ****************************************************************
// ＮＷＡ圧縮 圧縮関数 ＺＥＲＯフラグを設定
// ================================================================
void Cnwa_pack::_pack_func_set_bit_zero()
{
	_pack_func_set_bit(3, 0);

	if (_pack.zero_cnt < 2)	{
		_pack_func_set_bit(1, 0);
	}
	else if (_pack.zero_cnt < 4)	{
		_pack_func_set_bit(1, 1);
		_pack_func_set_bit(2, _pack.zero_cnt - 1);
	}
	else	{
		_pack_func_set_bit(1, 1);
		_pack_func_set_bit(2, 3);
		_pack_func_set_bit(8, _pack.zero_cnt - 1);
	}

	_pack.zero_cnt = 0;
}

// ****************************************************************
// ＮＷＡ圧縮 圧縮関数 データセット０
// ================================================================

#define		DAT_MOD_SET1			_pack_func_set_bit(3, 1);
#define		DAT_MOD_SET2			_pack_func_set_bit(3, 2);
#define		DAT_MOD_SET3			_pack_func_set_bit(3, 3);
#define		DAT_MOD_SET4			_pack_func_set_bit(3, 4);
#define		DAT_MOD_SET5			_pack_func_set_bit(3, 5);
#define		DAT_MOD_SET6			_pack_func_set_bit(3, 6);
#define		DAT_MOD_SET7			_pack_func_set_bit(3, 7);	_pack_func_set_bit(1, 0);

void Cnwa_pack::_pack_func_dat_set0(DWORD sign)
{
		 if (_pack.transition <  128)	{	DAT_MOD_SET1	_pack.transition &= 0xffffffe0;		_pack_func_set_bit(3, (_pack.transition >> 5) | (sign << 2));	}
	else if (_pack.transition <  256)	{	DAT_MOD_SET2	_pack.transition &= 0xffffffc0;		_pack_func_set_bit(3, (_pack.transition >> 6) | (sign << 2));	}
	else if (_pack.transition <  512)	{	DAT_MOD_SET3	_pack.transition &= 0xffffff80;		_pack_func_set_bit(3, (_pack.transition >> 7) | (sign << 2));	}
	else if (_pack.transition < 1024)	{	DAT_MOD_SET4	_pack.transition &= 0xffffff00;		_pack_func_set_bit(3, (_pack.transition >> 8) | (sign << 2));	}
	else if (_pack.transition < 2048)	{	DAT_MOD_SET5	_pack.transition &= 0xfffffe00;		_pack_func_set_bit(3, (_pack.transition >> 9) | (sign << 2));	}
	else if (_pack.transition < 4096)	{	DAT_MOD_SET6	_pack.transition &= 0xfffffc00;		_pack_func_set_bit(3, (_pack.transition >>10) | (sign << 2));	}
	else								{	DAT_MOD_SET7	_pack.transition &= 0xfffff800;		_pack_func_set_bit(6, (_pack.transition >>11) | (sign << 5));	}
}

// ****************************************************************
// ＮＷＡ圧縮 圧縮関数 データセット１
// ================================================================
void Cnwa_pack::_pack_func_dat_set1(DWORD sign)
{
		 if (_pack.transition <  128)	{	DAT_MOD_SET1	_pack.transition &= 0xfffffff0;		_pack_func_set_bit(4, (_pack.transition >> 4) | (sign << 3));	}
	else if (_pack.transition <  256)	{	DAT_MOD_SET2	_pack.transition &= 0xffffffe0;		_pack_func_set_bit(4, (_pack.transition >> 5) | (sign << 3));	}
	else if (_pack.transition <  512)	{	DAT_MOD_SET3	_pack.transition &= 0xffffffc0;		_pack_func_set_bit(4, (_pack.transition >> 6) | (sign << 3));	}
	else if (_pack.transition < 1024)	{	DAT_MOD_SET4	_pack.transition &= 0xffffff80;		_pack_func_set_bit(4, (_pack.transition >> 7) | (sign << 3));	}
	else if (_pack.transition < 2048)	{	DAT_MOD_SET5	_pack.transition &= 0xffffff00;		_pack_func_set_bit(4, (_pack.transition >> 8) | (sign << 3));	}
	else if (_pack.transition < 4096)	{	DAT_MOD_SET6	_pack.transition &= 0xfffffe00;		_pack_func_set_bit(4, (_pack.transition >> 9) | (sign << 3));	}
	else								{	DAT_MOD_SET7	_pack.transition &= 0xfffffc00;		_pack_func_set_bit(7, (_pack.transition >>10) | (sign << 6));	}
}

// ****************************************************************
// ＮＷＡ圧縮 圧縮関数 データセット２
// ================================================================
void Cnwa_pack::_pack_func_dat_set2(DWORD sign)
{
		 if (_pack.transition <  128)	{	DAT_MOD_SET1	_pack.transition &= 0xfffffff8;		_pack_func_set_bit(5, (_pack.transition >> 3) | (sign << 4));	}
	else if (_pack.transition <  256)	{	DAT_MOD_SET2	_pack.transition &= 0xfffffff0;		_pack_func_set_bit(5, (_pack.transition >> 4) | (sign << 4));	}
	else if (_pack.transition <  512)	{	DAT_MOD_SET3	_pack.transition &= 0xffffffe0;		_pack_func_set_bit(5, (_pack.transition >> 5) | (sign << 4));	}
	else if (_pack.transition < 1024)	{	DAT_MOD_SET4	_pack.transition &= 0xffffffc0;		_pack_func_set_bit(5, (_pack.transition >> 6) | (sign << 4));	}
	else if (_pack.transition < 2048)	{	DAT_MOD_SET5	_pack.transition &= 0xffffff80;		_pack_func_set_bit(5, (_pack.transition >> 7) | (sign << 4));	}
	else if (_pack.transition < 4096)	{	DAT_MOD_SET6	_pack.transition &= 0xffffff00;		_pack_func_set_bit(5, (_pack.transition >> 8) | (sign << 4));	}
	else								{	DAT_MOD_SET7	_pack.transition &= 0xfffffe00;		_pack_func_set_bit(8, (_pack.transition >> 9) | (sign << 7));	}
}

// ****************************************************************
// ＮＷＡ圧縮 圧縮関数 データセット３
// ================================================================
void Cnwa_pack::_pack_func_dat_set3(DWORD sign)
{
		 if (_pack.transition <  128)	{	DAT_MOD_SET1	_pack.transition &= 0xfffffffc;		_pack_func_set_bit(6, (_pack.transition >> 2) | (sign << 5));	}
	else if (_pack.transition <  256)	{	DAT_MOD_SET2	_pack.transition &= 0xfffffff8;		_pack_func_set_bit(6, (_pack.transition >> 3) | (sign << 5));	}
	else if (_pack.transition <  512)	{	DAT_MOD_SET3	_pack.transition &= 0xfffffff0;		_pack_func_set_bit(6, (_pack.transition >> 4) | (sign << 5));	}
	else if (_pack.transition < 1024)	{	DAT_MOD_SET4	_pack.transition &= 0xffffffe0;		_pack_func_set_bit(6, (_pack.transition >> 5) | (sign << 5));	}
	else if (_pack.transition < 2048)	{	DAT_MOD_SET5	_pack.transition &= 0xffffffc0;		_pack_func_set_bit(6, (_pack.transition >> 6) | (sign << 5));	}
	else if (_pack.transition < 4096)	{	DAT_MOD_SET6	_pack.transition &= 0xffffff80;		_pack_func_set_bit(6, (_pack.transition >> 7) | (sign << 5));	}
	else								{	DAT_MOD_SET7	_pack.transition &= 0xfffffe00;		_pack_func_set_bit(8, (_pack.transition >> 9) | (sign << 7));	}
}

// ****************************************************************
// ＮＷＡ圧縮 圧縮関数 データセット４
// ================================================================
void Cnwa_pack::_pack_func_dat_set4(DWORD sign)
{
		 if (_pack.transition <  256)	{	DAT_MOD_SET1	_pack.transition &= 0xfffffffc;		_pack_func_set_bit(7, (_pack.transition >> 2) | (sign << 6));	}
	else if (_pack.transition <  512)	{	DAT_MOD_SET2	_pack.transition &= 0xfffffff8;		_pack_func_set_bit(7, (_pack.transition >> 3) | (sign << 6));	}
	else if (_pack.transition < 1024)	{	DAT_MOD_SET3	_pack.transition &= 0xfffffff0;		_pack_func_set_bit(7, (_pack.transition >> 4) | (sign << 6));	}
	else if (_pack.transition < 2048)	{	DAT_MOD_SET4	_pack.transition &= 0xffffffe0;		_pack_func_set_bit(7, (_pack.transition >> 5) | (sign << 6));	}
	else if (_pack.transition < 4096)	{	DAT_MOD_SET5	_pack.transition &= 0xffffffc0;		_pack_func_set_bit(7, (_pack.transition >> 6) | (sign << 6));	}
	else if (_pack.transition < 8192)	{	DAT_MOD_SET6	_pack.transition &= 0xffffff80;		_pack_func_set_bit(7, (_pack.transition >> 7) | (sign << 6));	}
	else								{	DAT_MOD_SET7	_pack.transition &= 0xfffffe00;		_pack_func_set_bit(8, (_pack.transition >> 9) | (sign << 7));	}
}

// ****************************************************************
// ＮＷＡ圧縮 圧縮関数 データセット５
// ================================================================
void Cnwa_pack::_pack_func_dat_set5(DWORD sign)
{
		 if (_pack.transition <   512)	{	DAT_MOD_SET1	_pack.transition &= 0xfffffffc;		_pack_func_set_bit(8, (_pack.transition >> 2) | (sign << 7));	}
	else if (_pack.transition <  1024)	{	DAT_MOD_SET2	_pack.transition &= 0xfffffff8;		_pack_func_set_bit(8, (_pack.transition >> 3) | (sign << 7));	}
	else if (_pack.transition <  2048)	{	DAT_MOD_SET3	_pack.transition &= 0xfffffff0;		_pack_func_set_bit(8, (_pack.transition >> 4) | (sign << 7));	}
	else if (_pack.transition <  4096)	{	DAT_MOD_SET4	_pack.transition &= 0xffffffe0;		_pack_func_set_bit(8, (_pack.transition >> 5) | (sign << 7));	}
	else if (_pack.transition <  8192)	{	DAT_MOD_SET5	_pack.transition &= 0xffffffc0;		_pack_func_set_bit(8, (_pack.transition >> 6) | (sign << 7));	}
	else if (_pack.transition < 16384)	{	DAT_MOD_SET6	_pack.transition &= 0xffffff80;		_pack_func_set_bit(8, (_pack.transition >> 7) | (sign << 7));	}
	else								{	DAT_MOD_SET7	_pack.transition &= 0xfffffe00;		_pack_func_set_bit(8, (_pack.transition >> 9) | (sign << 7));	}
}



