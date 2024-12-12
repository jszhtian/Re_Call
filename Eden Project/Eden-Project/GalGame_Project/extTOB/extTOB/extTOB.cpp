// extTOB.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <set>
#include <map>

std::map<uint32_t, std::string> map; // resume table?
std::set<uint32_t> set; // resume table?

bool is_upper_jis_surrogate(int c)
{
    return (c >= 0x80 and c <= 0xA1) or (c >= 0xE0 and c <= 0xFF);
}

int main(int argc, char ** argv)
{
    //int strings = 0;
        map.clear();
        set.clear();
        
        //auto f = fopen(argv[1], "rb");
        auto f = fopen("s01_02a.tob", "rb");
        if (!f)
            return -1;
        char magic[4];
        int n = fread(magic, 1, 4, f);
        if(n != 4 or strncmp(magic, "TOB0", 4) != 0) 
            return -1;
        
        //puts(argv[1]);
        
        uint32_t header_size;
        uint32_t header_elements;
        fread(&header_size, 4, 1, f);
        fread(&header_elements, 4, 1, f);
        
        for(int j = 0; j < header_elements; j++)
        {
            uint8_t size;
            fread(&size, 1, 1, f);
            
            char * text = (char *)malloc(size-4);
            fread(text, 1, size-4, f);
            
            uint32_t value;
            fread(&value, 4, 1, f);
            map[value] = text;
            
            free(text);
        }
        if(ftell(f) != header_size+4)
        {
            puts("desynchronized"), puts(argv[1]);
            exit(0);
        }
        uint32_t table_size;
        uint32_t table_elements;
        fread(&table_size, 4, 1, f);
        fread(&table_elements, 4, 1, f);
        uint32_t table_end = table_size + header_size + 4;
        while(ftell(f) < table_end)
        {
            //printf("Header at %08X\n", ftell(f));
            uint32_t word;
            fread(&word, 4, 1, f);
            set.insert(word);
        }
        
        fseek(f, table_end, SEEK_SET);
        
        std::string text = "";
        uint32_t start = ftell(f);
        while(1)
        {
            uint32_t location = ftell(f);
            
            // I have absolutely no idea if these are correct.
            auto c = fgetc(f);
            bool closefile = feof(f) or ferror(f) or c < 0;
            if(closefile)
            {
                if(text != "")
                {
                    printf("%s\n", text.data());
                }
                break;
            }
            if(c == 0x5B) // STOP MAKING INHERENTLY ASCII INCOMPATIBLE FORMATS REEEEEEEEEEEEEEEEEEEEEEE
            {
                if(text != "")
                {
                    //printf("%08X \"%s\"\n", start, text.data());
                    printf("%s\n", text.data());
                }
                text = "";
                
                start = ftell(f);
            }
            else if(c == 0x20)
            {
                fseek(f, 4, SEEK_CUR);
                continue;
            }
            else if(c == 0x01)
            {
                uint32_t length;
                fread(&length, 4, 1, f);
                if(length < 4)
                {
                    printf("mayday in op 01 at %08X in %s\n", location, argv[1]);
                    exit(0);
                }
                fseek(f, length-4, SEEK_CUR);
                continue;
            }
            else if(c == 0x03)
            {
                // I have no idea how this works. Don't blame me if it breaks.
                uint16_t unknown1;
                fread(&unknown1, 2, 1, f);
                if(unknown1 > 0xFF)
                {
                    printf("mayday 1 in op 03 at %08X in %s\n", location, argv[1]);
                    exit(0);
                }
                fseek(f, unknown1+1, SEEK_CUR);
                
                uint16_t unknown2;
                fread(&unknown2, 2, 1, f);
                if(unknown2 < 2)
                {
                    printf("mayday 2 in op 03 at %08X in %s\n", location, argv[1]);
                    exit(0);
                }
                unknown2 -= 2;
                fseek(f, unknown2-2, SEEK_CUR);
                
                uint32_t unknown3;
                fread(&unknown3, 4, 1, f);
                if(unknown3 < 4)
                {
                    printf("mayday 3 in op 03 at %08X in %s\n", location, argv[1]);
                    exit(0);
                }
                fseek(f, unknown3-4, SEEK_CUR);
                continue;
            }
            else if(c == 0x02)
            {
                // I have no idea how this works. Don't blame me if it breaks.
                fseek(f, 8, SEEK_CUR);
                
                uint32_t unknown1;
                fread(&unknown1, 4, 1, f);
                if(unknown1 < 4)
                {
                    printf("mayday 2 in op 03 at %08X in %s\n", location, argv[1]);
                    exit(0);
                }
                fseek(f, unknown1-4, SEEK_CUR);
                continue;
            }
            else if(c == 0x73)
            {
                uint32_t length;
                fread(&length, 4, 1, f);
                if(length > 0x0000FFFF)
                {
                    printf("mayday 1 in op 73 at %08X in %s\n", location, argv[1]);
                    exit(0);
                }
                fseek(f, length+1, SEEK_CUR);
                fread(&length, 4, 1, f);
                if(length < 4)
                {
                    printf("mayday 2 in op 73 at %08X in %s\n", location, argv[1]);
                    exit(0);
                }
                fseek(f, length-4, SEEK_CUR);
                continue;
            }
            else if(is_upper_jis_surrogate(c))
            {
                bool intext = true;
                while(intext)
                {
                    text += c;
                    if(is_upper_jis_surrogate(c))
                        text += fgetc(f);
                    c = fgetc(f);
                    if(c == 0x5B)
                        intext = false;
                }
            }
            else if(c == 0)
            {
                if(fgetc(f) >= 0)
                {
                    printf("unknown operation %02X at %08X in %s\n", c, location, argv[1]);
                    exit(0);
                }
                else
                    break;
            }
            else
            {
                printf("unknown operation %02X at %08X in %s\n", c, location, argv[1]);
                //exit(0);
                break;
            }
        }
        
        fclose(f);d
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
