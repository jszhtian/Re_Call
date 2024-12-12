// extSCX.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "Native.h"

#pragma pack (1)
struct SCXHeader {
    char Magic[4]; // "SC3\0"
    DWORD JmpTableStartOffset;
    DWORD JmpTableEndOffset;
};
#pragma pack ()

string DecodeString(BYTE* text)
{
    string ret;
    for (int i = 0; i != 0xFF;)
    {
        BYTE* c = text + i;
        if (*c >= 0x80)
        {
            int pos = ReadWordBE(c) & 0x7FFF;
            if (pos >= MapChars.size())
            {
                cout << hex << pos << endl;
                E("pos over than MapChars' size");
                ExitProcess(-1);
            }
            if (MapChars[pos] == u8"\ue100")
                ret += "…";
            else
                ret += MapChars[pos];

            i += 2;
            continue;
        }
        StringType Type = (StringType)*c;

        switch (Type) {
        case LineBreak: {
            ret += "[linebreak]";
            break;
        }
        case AltLineBreak: {
            ret += "[alt-linebreak]";
            break;
        }
        case CharacterNameStart: {
            ret += "[name]";
            break;
        }
        case DialogueLineStart: {
            ret += "[line]";
            break;
        }
        case Present: {
            ret += "[%p]";
            break;
        }
        case SetColor: {
            ret += "[color index=\"";
            stream << exprToString(&remaining);
            stream << "\"]";
            break;
        }
        case Present_ResetAlignment: {
            ret += "[%e]";
            break;
        }
        case RubyBaseStart: {
            ret += "[ruby-base]";
            break;
        }
        case RubyTextStart: {
            ret += "[ruby-text-start]";
            break;
        }
        case RubyTextEnd: {
            ret += "[ruby-text-end]";
            break;
        }
        case SetFontSize: {
            if (remaining < 2) {
                throw std::runtime_error("Not enough data when parsing string");
            }
            remaining -= 2;
            stream << "[font size=\"";
            print(stream, readUint16BE());
            stream << "\"]";
            break;
        }
        case PrintInParallel: {
            ret += "[parallel]";
            break;
        }
        case CenterText: {
            ret += "[center]";
            break;
        }
        case SetTopMargin: {
            if (remaining < 2) {
                throw std::runtime_error("Not enough data when parsing string");
            }
            remaining -= 2;
            stream << "[margin top=\"";
            print(stream, readUint16BE());
            stream << "\"]";
            break;
        }
        case SetLeftMargin: {
            if (remaining < 2) {
                throw std::runtime_error("Not enough data when parsing string");
            }
            remaining -= 2;
            stream << "[margin left=\"";
            print(stream, readUint16BE());
            stream << "\"]";
            break;
        }
        case GetHardcodedValue: {
            if (remaining < 2) {
                throw std::runtime_error("Not enough data when parsing string");
            }
            remaining -= 2;
            stream << "[hardcoded-value index=\"";
            print(stream, readUint16BE());
            stream << "\"]";
            break;
        }
        case EvaluateExpression: {
            stream << "[evaluate expr=\"";
            stream << exprToString(&remaining);
            stream << "\"]";
            break;
        }
        case AutoForward: {
            ret += "[auto-forward]";
            break;
        }
        case AutoForward_1A: {
            ret += "[auto-forward-1a]";
            break;
        }
        case Present_0x18: {
            ret += "[%18]";
            break;
        }
        case Unk_1E: {
            ret += "[unk-1e]";
            break;
        }
        default: {
            std::stringstream error;
            error << "Unrecognized string token " << (int)tokenType;
            throw std::runtime_error(error.str());
        }
        }
        i += 1;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        MessageBox(0, L"extSCX.exe <input SCX file>", L"AyamiKaze", 0);
        //return -1;
    }
    char* FileName = (char*)"yns_01_01.scx";
    FILE* fp = fopen(FileName, "rb");
    fseek(fp, 0, SEEK_END);
    DWORD Size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    BYTE* Buff = new BYTE[Size];
    fread(Buff, Size, 1, fp);
    fclose(fp);

    SCXHeader* header = (SCXHeader*)Buff;
    if (memcmp(header->Magic, "SC3\0", 4))
        return E("Magic Not Match");

    DWORD JmpTableSize = header->JmpTableEndOffset - header->JmpTableStartOffset;

    if (JmpTableSize == 0)
    {
        cout << "JmpTableSize is 0." << endl;
        return 0;
    }

    for (DWORD i = 0; i < JmpTableSize; i += 4)
    {
        DWORD JmpOffset = *(DWORD*)(Buff + header->JmpTableStartOffset + i);
        BYTE* TextOffset = (Buff + JmpOffset);

        wstring Text;
    }
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
