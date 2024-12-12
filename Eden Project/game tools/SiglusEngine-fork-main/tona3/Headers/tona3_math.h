#pragma		once

#include	"tona3_array.h"

namespace NT3
{

// ****************************************************************
// �X���b�v
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
// �͈͐���
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
// ���`�ϊ�
// ================================================================
template <typename TYPE_IN, typename TYPE_OUT>
inline TYPE_OUT linear(TYPE_IN in, TYPE_IN in1, TYPE_OUT out1, TYPE_IN in2, TYPE_OUT out2)
{
	if (in1 == in2)
		return out2;

	// in1 != in2 ��O��ɂ��Ă��܂��I
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

	// in1 != in2 ��O��ɂ��Ă��܂��I
	return (out2 - out1) * (in - in1) / (in2 - in1) + out1;
}

// ****************************************************************
// ���l�֘A
// ================================================================

// �������擾
int get_keta(int num);
int get_keta(int num, int* digit, int digit_cnt);

// �������擾
int get_sign(int num);

// ****************************************************************
// �X�v���C��
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
