// nanacan_patch.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "nanacan_patch.h"


// ���ǵ���������һ��ʾ��
NANACAN_PATCH_API int nnanacan_patch=0;

// ���ǵ���������һ��ʾ����
NANACAN_PATCH_API int fnnanacan_patch(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� nanacan_patch.h
Cnanacan_patch::Cnanacan_patch()
{
	return;
}
