#include "sg.h"

int main(int agrc, char* agrv[])
{
	cout << "project��Niflheim-Ivory\n���ڽ�sg�ļ�������PNG��\nby Destiny�λ�� 2018.04.26\n";
	if (agrc != 2)
		cout << "\nUsage:sg2png sgfile\n";
	else
	{
		sg sg(agrv[1]);
		sg.sg_decode();
		cout << "���!\n";
	}
}