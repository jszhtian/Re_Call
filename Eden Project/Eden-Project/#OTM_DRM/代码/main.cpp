#include"pack.h"

//1. ��д�ӿ�����
//    ��������Σ���DLL������ָ������ͷ���������ݣ���OEP����Ϊ�Ǵ���OEP
//2. ��д�Ǵ��룺�Ǵ�������һ�� -> ִ����Ǵ������ת��ԭOEP
//3. ��д�ӿ������ԿǴ������IAT�޸� -> �ڿǴ�������Ӵ��룬�޸��Ǵ����ض�λ(��ַ������ƫ��)��
//3. ��д�Ǵ��룺����IAT -> �ӹ�IAT���Լ��޸�IAT������IAT
//4. ���ܳ��򣺼��ܡ�ѹ�����ӿ�ָ��������
//5. �� dll �б�д��ѹ�����ܳ���ͽ���IAT����

int main() {
	Pack PackObj;
	PackObj.SetDllSecName(".text");			
	PackObj.OpenPeFile("testa.exe");			//�򿪱����ܳ���
	PackObj.OpenDllFile("DllMain.dll");			//�򿪿Ǵ���dll
	PackObj.CopySection(".pack");				//��������ͷ
	PackObj.SetOep();							//����OEPΪ�Ǵ���OEP
	PackObj.Encryption(".text");				//�Ա��ӿǳ����text�μ���
	PackObj.Compression();						//ѹ�������
	PackObj.SetIatTo0();						//��Ǵ���ӹ�IAT��
	PackObj.SetTls();							//����TLS�����TLS
	PackObj.FixReloc();							//��Dllģ���ض�λ	
	PackObj.SetReloc();							//���ݲ��滻�ض�λ��
	PackObj.CopySecData(".pack");				//������������
	PackObj.WriteToFile("Test_packa.exe");		//���ӿǺ�ĳ���д�����
	return 0;
}

