#include "pgd.h"

int main(int agrc, char* agrv[])
{
	cout << "project��Niflheim-SOFTPAL_ADV_SYSTEM\n���ڽ�PNGת����PGD����ʱ֧��32��24λ��ѹ������Ϊ2��3���ļ���\nby Destiny�λ�� 2016.11.13\n";
	if (agrc != 2)
		cout << "\nUsage:png2pgd pgdfile\n";
	else
	{
		PGD pgd(agrv[1]);
		if (pgd.pgd_compress())
			cout << "���!\n";
	}
	return 0;
}