#pragma		once

#include	"tona3_array.h"

namespace NT3
{

// ****************************************************************
// スワップ
// ================================================================

/*
template <typename TYPE_0, typename TYPE_1>
inline void swap(TYPE_0* a, TYPE_1* b)
{
	TYPE_0 temp = *a;
	*a = *b;
	*b = temp;
}
*/

// ****************************************************************
// 範囲制限
// ================================================================
template <typename TYPE>
inline TYPE limit(TYPE min, TYPE value, TYPE max)
{
	if (false);
	else if (value < min)	return min;
	else if (value > max)	return max;
	else					return value;
}

template <typename TYPE>
inline TYPE limit_loop(TYPE min, TYPE value, TYPE max)
{
	if (max == min)
		return max;

	if (max < min)
		swap(&max, &min);

	int range = max - min;

	while (value < min)		value += range;
	while (value > max)		value -= range;

	return value;
}

// ****************************************************************
// 線形変換
// ================================================================
template <typename TYPE_IN, typename TYPE_OUT>
inline TYPE_OUT linear(TYPE_IN in, TYPE_IN in1, TYPE_OUT out1, TYPE_IN in2, TYPE_OUT out2)
{
	if (in1 == in2)
		return out2;

	// in1 != in2 を前提にしています！
	return (out2 - out1) * (in - in1) / (in2 - in1) + out1;
}

template <typename TYPE_IN, typename TYPE_OUT>
inline TYPE_OUT linear_limit(TYPE_IN in, TYPE_IN in1, TYPE_OUT out1, TYPE_IN in2, TYPE_OUT out2)
{
	if (in1 == in2)
		return out2;
	else if (in1 < in2)
		in = limit(in1, in, in2);
	else if (in1 > in2)
		in = limit(in2, in, in1);

	// in1 != in2 を前提にしています！
	return (out2 - out1) * (in - in1) / (in2 - in1) + out1;
}

// ****************************************************************
// 数値関連
// ================================================================

// 桁数を取得
int get_keta(int num);
int get_keta(int num, int* digit, int digit_cnt);

// 符号を取得
int get_sign(int num);

// ****************************************************************
// スプライン
// ================================================================
class Cspline
{
public:
	Cspline();
	void	init(double* x_array, double* y_array, int vertex_cnt);
	double	calc(double x);

private:
	ARRAY<double>	a, b, c, d, left, right;
	int				line_cnt;
};


}
