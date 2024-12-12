// BuildText.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define ex

#include <stdio.h>
#include <tchar.h>
#include "windows.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string.h>


#ifdef bu
#include <map>
#else
#include <unordered_map>
#endif

//#include <map>

using namespace std;

#pragma pack(1)
struct aym_text_struct
{
	unsigned int off;
    unsigned int sz;
	unsigned int jmp;
};
struct aym_header
{
	char flag[23];
	unsigned int str_count;
};

struct aym_text_buff {
    unsigned int sz;
    WCHAR* txt;
};
#pragma pack()

LPWSTR ctowUTF(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, out, dwMinSize);//转换
	return out;
}

char* wtoc(LPCTSTR str,unsigned int cp)
{
    DWORD dwMinSize;
    dwMinSize = WideCharToMultiByte(cp, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
    char* out = new char[dwMinSize];
    WideCharToMultiByte(cp, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
    return out;
}

#ifdef bu
#define ScriptName "mix2.txt"

map<unsigned int, wstring>TextList;

unsigned int GetOffsetNum(wstring line)
{
    for (unsigned int c = 0; c < line.size(); c++)
    {
        if (line[c] == L'|')
            return c;
    }
    return 0;
}

unsigned int SetMap()
{
    unsigned int line_count = 0;
    ifstream txt(ScriptName);
    if (txt.is_open())
    {
        const int LineMax = 0x1000;
        char str[LineMax];
        while (txt.getline(str, LineMax))
        {
            auto wtmp = ctowUTF(str);
            wstring wline = wtmp;
            unsigned int cint = GetOffsetNum(wline);
            if (cint == 0)
            {
                MessageBox(0, L"cint is 0.", L"InitText", MB_OK | MB_ICONERROR);
                txt.close();
                return 0;
            }
            wstring off_str = wline.substr(0, cint);
            wstring text_str = wline.substr(cint + 1);
            unsigned int off = wcstoul(off_str.c_str(), NULL, 0);
            //cout << off << endl;
            TextList.insert(pair<unsigned int, wstring>(off, text_str));
            line_count++;
        }
        txt.close();
    }
    return line_count;
}

int main()
{
    aym_header header;
    aym_text_struct entry;
    unsigned int line_count = SetMap();
    if (line_count == 0)
    {
        cout << "Line Count Is Zero" << endl;
        system("pause");
        return -1;
    }
    strcpy(header.flag, "AyamiKazeScriptVer1.00");
    header.str_count = line_count;

    FILE* fp = fopen("Script.aym", "wb");
    fwrite(&header, sizeof(header), 1, fp);

    unsigned int entry_buffer = line_count * sizeof(entry);
    unsigned int pos = 0;
    unsigned int pos2 = 0;

    for (auto& iter : TextList) 
    {
        WCHAR* str = (WCHAR*)iter.second.c_str();

        wcscat(str, L"\0");

        entry.off = iter.first;
        entry.sz = wcslen(str) * 2 + 2;
        entry.jmp = pos2;
        fwrite(&entry, sizeof(entry), 1, fp);

        fseek(fp, pos2 + sizeof(header) + entry_buffer, SEEK_SET);
        fwrite(str, entry.sz, 1, fp);

        pos2 += entry.sz;
        pos += sizeof(entry);

        fseek(fp, pos + sizeof(header), SEEK_SET);
    }
    fclose(fp);
	system("pause");
	return 0;
}
#else

class ReadTextFromAymScript
{
private:
    aym_header header;
    aym_text_buff t_buf;
    //WCHAR tmp[0x1000];
    unordered_map<unsigned int, aym_text_buff>AymScriptList;

    FILE* fp;
    unsigned int FileSize;
    unsigned int EntrySize;
    unsigned int TextSize;
    aym_text_struct* EntryBuff;
    char* TextBuff;

    void AymGetFileSize(FILE* fp);
public:
    ReadTextFromAymScript();
    ~ReadTextFromAymScript();

    bool InitAymScript();

    pair<WCHAR*, unsigned int> GetTextByOff(unsigned int off);
};

ReadTextFromAymScript::ReadTextFromAymScript()
{
    fp = NULL;
    EntrySize = NULL;
    TextSize = NULL;
    EntryBuff = NULL;
    TextBuff = NULL;
}

ReadTextFromAymScript::~ReadTextFromAymScript()
{
    fp = NULL;
    EntrySize = NULL;
    TextSize = NULL;

    if (EntryBuff)
        free(EntryBuff);
    EntryBuff = NULL;

    if (TextBuff)
        free(TextBuff);
    TextBuff = NULL;
}

pair<WCHAR*, unsigned int> ReadTextFromAymScript::GetTextByOff(unsigned int off)
{
    auto itr = AymScriptList.find(off);
    if (itr != AymScriptList.end())
    {
        return pair<WCHAR*, unsigned int>(itr->second.txt, itr->second.sz);
    }
    return pair<WCHAR*, unsigned int>(NULL, NULL);
}

void ReadTextFromAymScript::AymGetFileSize(FILE* fp)
{
    fseek(fp, 0, SEEK_END);
    FileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
}

bool ReadTextFromAymScript::InitAymScript()
{

    fp = fopen("Script.aym", "rb");
    if (!fp)
    {
        MessageBox(0, L"Can't Read Script.aym", 0, 0);
        return false;
    }

    AymGetFileSize(fp);

    fread(&header, sizeof(aym_header), 1, fp);

    if (strncmp(header.flag, "AyamiKazeScriptVer1.00", 23))
    {
        MessageBox(0, L"Magic not Match", 0, 0);
        return false;
    }

    EntrySize = header.str_count * sizeof(aym_text_struct);
    aym_text_struct* EntryBuff = (aym_text_struct*)malloc(EntrySize);
    if (!EntryBuff)
    {
        MessageBox(0, L"Can't alloc EntryBuff", 0, 0);
        return false;
    }
    fread(EntryBuff, EntrySize, 1, fp);

    TextSize = FileSize - EntrySize - sizeof(header);
    TextBuff = (char*)malloc(TextSize);
    if (!TextBuff)
    {
        MessageBox(0, L"Can't alloc TextBuff", 0, 0);
        return false;
    }
    fread(TextBuff, TextSize, 1, fp);
    fclose(fp);

    cout << "ReadTextFin" << endl;

    for (unsigned int i = 0; i < header.str_count; i++)
    {
        t_buf.sz = EntryBuff[i].sz;
        t_buf.txt = (WCHAR*)(TextBuff + EntryBuff[i].jmp);
        AymScriptList.insert(pair<unsigned int, aym_text_buff>(EntryBuff[i].off, t_buf));
    }
    /*
    for (auto& iter : AymScriptList)
    {
        cout << iter.first << "||" << wtoc(iter.second.txt, CP_ACP) << endl;
    }
    */
    cout << "MakeTextListFin" << endl;

    return true;
}

int main()
{
    
    ReadTextFromAymScript RTFAS;
    if (!RTFAS.InitAymScript())
    {
        MessageBox(0, L"Can't comp Script.aym", 0, 0);
        return 0;
    }
    while (true) 
    {
        unsigned int off;
        cout << "input off:" << endl;
        cin >> off;
        
        pair<WCHAR*, unsigned int>str_it = RTFAS.GetTextByOff(off);
        if (str_it.first == NULL && str_it.second == NULL)
        {
            MessageBox(0, L"str_it is 0", 0, 0);
            return 0;
        }
        cout << "size:" << str_it.second << endl;
        cout << "str:" << wtoc(str_it.first, CP_ACP) << endl;

    }
    /*
    aym_header header;
    aym_text_buff t_buf;
    map<unsigned int, aym_text_buff>AymScriptList;


    FILE* fp = fopen("Script.aym", "rb");

    fseek(fp, 0, SEEK_END);
    unsigned int FileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    fread(&header, sizeof(header), 1, fp);

    cout << "header.Magic:" << header.flag << endl;
    cout << "header.str_count:" << header.str_count << endl;
    if (strncmp(header.flag, "AyamiKazeScriptVer1.00", 23))
    {
        cout << "Magic Not Match" << endl;
        cout << header.flag << endl;
        system("pause");
        return 0;
    }

    unsigned int EntrySize = header.str_count * sizeof(aym_text_struct);
    aym_text_struct* EntryBuff = (aym_text_struct*)malloc(EntrySize);
    fread(EntryBuff, EntrySize, 1, fp);

    unsigned int TextSize = FileSize - EntrySize - sizeof(header);
    char* TextBuff = (char*)malloc(TextSize);
    fread(TextBuff, TextSize, 1, fp);
    fclose(fp);
    
    for (unsigned int i = 0; i < header.str_count; i++)
    {
        t_buf.sz = EntryBuff[i].sz;
        t_buf.txt = (WCHAR*)(TextBuff + EntryBuff[i].jmp);

        //cout << EntryBuff[i].off << "||" << t_buf.sz << "||" << wtoc(t_buf.txt,CP_ACP) << endl;
        AymScriptList.insert(pair<unsigned int, aym_text_buff>(EntryBuff[i].off, t_buf));
    }
    for (auto& iter : AymScriptList)
    {
        cout << "----------------------Info----------------------" << endl;
        cout << iter.first << "||" << wtoc(iter.second.txt, CP_ACP) << endl;
    }

    free(TextBuff);
    free(EntryBuff);

    */
    system("pause");
    return 0;

}
#endif
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
