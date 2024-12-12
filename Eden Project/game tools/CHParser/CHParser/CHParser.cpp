// CHParser.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;
struct OpBlock
{
	uint32_t Op;
	uint32_t Len;
	uint32_t para0, para1, para2, para3;
};
int main(int argc, char* argv[])
{
    if(argc!=2)
    {
		cout << "Usage:" << argv[0] << " asb file";
    }
	fstream fst;
	vector<string> TextList;
	fst.open(argv[1], ios::binary | ios::in);
	int Filesize;
	fst.seekg(0, ios::end);
	Filesize = fst.tellg();
	char* TMPBlock;
	TMPBlock = new char[Filesize];
	fst.seekg(0x0, ios::beg);
	fst.read(TMPBlock, Filesize);
	fst.close();
	int RPtr = 0;
	while(RPtr< Filesize)
	{
		char* CurPtr = TMPBlock + RPtr;
		OpBlock* OpBlk = (OpBlock*)CurPtr;
		if(OpBlk->Op==0)
		{
			break;//EOF Flag
		}
		if(OpBlk->Len>sizeof(OpBlock))
		{
			char* TxtPtr = CurPtr + sizeof(OpBlock);
			string txt(TxtPtr);
			TextList.push_back(txt);
		}
		RPtr += OpBlk->Len;
	}
	string outname = argv[1];
	outname += ".txt";
	ofstream fout(outname);
	for(auto var:TextList)
	{
		fout << var;
		fout << endl;
	}
	fout.close();
}

// ���г���: Ctrl + F5 ����� >����ʼִ��(������)���˵�
// ���Գ���: F5 ����� >����ʼ���ԡ��˵�

// ����ʹ�ü���: 
//   1. ʹ�ý��������Դ�������������/�����ļ�
//   2. ʹ���Ŷ���Դ�������������ӵ�Դ�������
//   3. ʹ��������ڲ鿴���������������Ϣ
//   4. ʹ�ô����б��ڲ鿴����
//   5. ת������Ŀ��>���������Դ����µĴ����ļ�����ת������Ŀ��>�����������Խ����д����ļ���ӵ���Ŀ
//   6. ��������Ҫ�ٴδ򿪴���Ŀ����ת�����ļ���>���򿪡�>����Ŀ����ѡ�� .sln �ļ�
