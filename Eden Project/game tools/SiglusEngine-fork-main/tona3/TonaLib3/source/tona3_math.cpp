#include	"tona3_pch.h"
#include	"tona3_math.h"

namespace NT3
{

// ****************************************************************
// 桁数を求める
//		負数の場合は符号を無視する
//		0 の場合は 1 を返す
// ================================================================
int get_keta(int num)
{
	if (num == 0)	return 1;	// 0 の処理
	num = abs(num);				// 負数の処理

	// 桁数計算
	int keta = 0;
	for (; num > 0; num /= 10, keta++);

	return keta;
}

// ****************************************************************
// 桁分解
//		負数の場合は符号を無視する
//		0 の場合は 1 を返す
// ================================================================
int get_keta(int num, int* digit, int digit_cnt)
{
	num = abs(num);
	int keta = 1;

	for (int i = digit_cnt - 1; i >= 0; i--)	{
		digit[i] = num % 10;
		num /= 10;
		if (num > 0)	keta++;
	}

	return keta;
}

// ****************************************************************
// 符号を求める
// ================================================================
int get_sign(int num)
{
	if (false);
	else if (num > 0)	return 1;
	else if (num < 0)	return -1;
	else				return 0;
}

// ****************************************************************
// スプライン
// ================================================================
Cspline::Cspline()
{
	line_cnt = 0;
}

// ****************************************************************
// スプライン：初期化
// ================================================================
void Cspline::init(double* x, double* y, int vertex_cnt)
{
	int i;
	
	// 曲線の数 = 頂点の数 - 1
	line_cnt = vertex_cnt - 1;

	// 曲線データ
	a.resize(line_cnt + 1);
	b.resize(line_cnt + 1);
	c.resize(line_cnt + 1);
	d.resize(line_cnt + 1);
	left.resize(line_cnt + 1);
	right.resize(line_cnt + 1);

	// テンポラリ値
	ARRAY<double> dx, r, l, m, z;
	dx.resize(line_cnt + 1);
	r.resize(line_cnt + 1);
	l.resize(line_cnt + 1);
	m.resize(line_cnt + 1);
	z.resize(line_cnt + 1);

	// 各曲線の両端を設定
	for (i = 0; i < line_cnt; i++)	{
		left[i]  = x[i];
		right[i] = x[i + 1];
	}

	for (i = 0; i < line_cnt + 1; i++)
		a[i] = y[i];

	for (i = 0; i < line_cnt; i++)
		dx[i] = x[i+1] - x[i];

	for (i = 1; i < line_cnt; i++)
		r[i] = 3.0 * (a[i + 1] - a[i]) / dx[i] - 3.0 * (a[i] - a[i - 1]) / dx[i - 1];

	l[0] = 1.0;
	m[0] = 0.0;
	z[0] = 0.0;

	for (i = 1; i < line_cnt; i++)	{
		l[i] = 2.0 * (x[i + 1] - x[i - 1]) - dx[i - 1] * m[i - 1];
		m[i] = dx[i] / l[i];
		z[i] = (r[i] - dx[i - 1] * z[i - 1]) / l[i];
	}

	l[line_cnt] = 1.0;
	z[line_cnt] = 0.0;
	c[line_cnt] = 0.0;

	for (i = line_cnt - 1; i >= 0; i--)	{
		c[i] = z[i] - m[i] * c[i + 1];
		b[i] = (a[i+1] - a[i]) / dx[i] - dx[i] * (c[i + 1] + 2 * c[i]) / 3;
		d[i] = (c[i+1] - c[i]) / (3 * dx[i]);
	}
}

// ****************************************************************
// スプライン：計算
// ================================================================
double Cspline::calc(double x)
{
	int i;

	if (line_cnt == 0)
		return 0.0;

	// 使う曲線番号を決める
	int j = line_cnt - 1;
	for (i = 0; i < line_cnt; i++)	{
		if (x < right[i])	{
			j = i;
			break;
		}
	}

	double dx = x - left[j];
	return a[j] + dx * (b[j] + dx * (c[j] + dx * d[j]));
}










}
