// ExecYSV.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;


int main()
{
    uint16_t Count = 0;
    FILE* fp = fopen("ysv.ybn", "rb");
    fseek(fp, 8, SEEK_SET);
    fread(&Count, sizeof(uint16_t), 1, fp);

    FILE* out = fopen("list.txt", "wb");

    cout << "ArgCount:0x" << hex << Count << endl;

    fprintf(out, "ArgCount:0x%x\n", Count);

    for (int i = 0; i < Count; i++)
    {
        uint8_t DataType1 = 0;
        uint8_t field_0 = 0;
        uint16_t ScriptID = 0;
        uint16_t StoreID = 0;

        uint8_t DataType2 = 0;
        uint8_t ArgCount = 0;

        fread(&DataType1, sizeof(uint8_t), 1, fp);
        fread(&field_0, sizeof(uint8_t), 1, fp);
        fread(&ScriptID, sizeof(uint16_t), 1, fp);
        fread(&StoreID, sizeof(uint16_t), 1, fp);

        fread(&DataType2, sizeof(uint8_t), 1, fp);
        fread(&ArgCount, sizeof(uint8_t), 1, fp);

        for (int j = 0; j < ArgCount; j++)
        {
            uint32_t tmp = 0;
            fread(&tmp, sizeof(uint32_t), 1, fp);
        }

        char str[1024] = { 0 };
        char* ptr_str = nullptr;
        uint64_t tmp1 = 0;
        double tmp2 = 0.0;
        uint16_t len = 0;

        switch (DataType2)
        {
        case 0:
            break;
        case 1:
            fread(&tmp1, sizeof(uint64_t), 1, fp);
            break;
        case 2:
            fread(&tmp2, sizeof(double), 1, fp);
            break;
        case 3:
            fread(&len, sizeof(uint16_t), 1, fp);
            fread(str, len, 1, fp);
            ptr_str = str + 3;
            break;
        default:
            cout << "unk type:" << hex << DataType2 << endl;
            system("pause");
            return 0;
        }

        if (DataType2 == 3)
            fprintf(out, "DataType1:0x%x | field_0:0x%x | ScriptID:%d | StoreID:0x%x | DataType2:0x%x | ArgCount:0x%x | Text:%s\n", 
                DataType1, field_0, ScriptID, StoreID, DataType2, ArgCount, ptr_str);
        else
            fprintf(out, "DataType1:0x%x | field_0:0x%x | ScriptID:%d | StoreID:0x%x | DataType2:0x%x | ArgCount:0x%x\n",
                DataType1, field_0, ScriptID, StoreID, DataType2, ArgCount);
    }
    fclose(fp);
    fclose(out);
    system("pause");
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
