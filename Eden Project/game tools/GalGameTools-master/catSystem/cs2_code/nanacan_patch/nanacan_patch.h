// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� NANACAN_PATCH_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// NANACAN_PATCH_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef NANACAN_PATCH_EXPORTS
#define NANACAN_PATCH_API __declspec(dllexport)
#else
#define NANACAN_PATCH_API __declspec(dllimport)
#endif

// �����Ǵ� nanacan_patch.dll ������
class NANACAN_PATCH_API Cnanacan_patch {
public:
	Cnanacan_patch(void);
	// TODO: �ڴ�������ķ�����
};

extern NANACAN_PATCH_API int nnanacan_patch;

NANACAN_PATCH_API int fnnanacan_patch(void);
