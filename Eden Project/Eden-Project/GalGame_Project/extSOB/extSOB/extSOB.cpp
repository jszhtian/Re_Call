/*
source:
http://wiki.wareya.moe/Lucifen%20Library

Engine:
Lucifen

Game:
[Navel] 俺たちに翼はない ―――under the innocent sky

Scripts:
*.sob

Make:
g++ -o sobRead.exe sobRead.cpp

Use Sample:
sobR.exe M:\ZUBASA\SCRIPT\s04_13a.sob>>s04_13a.txt

*/


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <Windows.h>
using namespace std;
vector<map<uint32_t, uint32_t>> maps;

map<uint32_t, string> examples;

//deduplicate identical strings within each scene even if they have different addresses
bool deduplciate = true;
std::set<uint32_t> seen;
int main(int argc, char** argv)
{
    int strings = 0;
    int totalspace = 0;
    //for (int i = 0; i < argc; i++)
   // {
        maps.clear();
        seen.clear();

        auto f = fopen(argv[1], "rb");
        ofstream TXT(argv[2]);
        if (!f) 
            return 0;
        char magic[4];
        int n = fread(magic, 1, 4, f);
        if (n != 4 or strncmp(magic, "SOB0", 4) != 0) return 0;
        uint32_t table_size;
        fread(&table_size, 4, 1, f);
        uint32_t table_end = table_size + 0x8;
        int header_count = 0;
        while (ftell(f) < table_end)
        {
            uint32_t word_pairs;
            fread(&word_pairs, 4, 1, f);
            std::map<uint32_t, uint32_t> newmap;
            for (int i = 0; i < word_pairs; i++)
            {
                uint32_t key;
                uint32_t value;
                fread(&key, 4, 1, f);
                fread(&value, 4, 1, f);
                newmap[key] = value;
            }
            maps.push_back(newmap);
            header_count += 1;
        }
        if (maps.size() < 5)
        {
            printf("not enough maps");
            exit(0);
        }
        fseek(f, table_end, SEEK_SET);
        uint32_t strings_start;
        uint32_t strings_end;
        fread(&strings_start, 4, 1, f);
        fread(&strings_end, 4, 1, f);
        totalspace += strings_end - strings_start;
        uint32_t header_end = ftell(f);
        uint32_t code_end = strings_start + header_end;
        std::string text_memory;
        uint32_t text_memory_addr;
        cout << hex << (table_end+strings_start) << endl;
        cout << hex << strings_end << endl;
        bool justdidtext = false;

        while (ftell(f) < code_end)
        {
            uint32_t address = ftell(f) - header_end;
            uint32_t command;
            fread(&command, 4, 1, f);
            //cout << "0x" << hex << (DWORD)command << ":" << text_memory.data() << endl;
            //TXT << "0x" << hex << (DWORD)command << ":" << text_memory.data() << endl;
            if ((command & 0xFFF00000) == 0x01800000 and maps[4].count(address) and maps[4][address] >= strings_start and maps[4][address] < strings_end || (command == 0x000001C8 or command == 0x000001CF) and text_memory != "" and (!deduplciate or !seen.count(text_memory_addr)))
            {
                strings++;
                fseek(f, maps[4][address] + header_end, SEEK_SET);
                int c = fgetc(f);

                if (c == 0) // 00 XX XX <string
                    fseek(f, 2, SEEK_CUR);
                else // <string>
                    fseek(f, -1, SEEK_CUR);

                auto start = ftell(f);
                while ((c = fgetc(f)) != 0);
                auto end = ftell(f);
                char* text = (char*)malloc(end - start);
                fseek(f, start, SEEK_SET);
                fread(text, 1, end - start, f);
                std::string str = std::string(text);
                if (!examples.count(command))
                    examples[command] = str;

                text_memory = str;
                text_memory_addr = maps[4][address];
                if (strlen(text_memory.data()) != 0 && strcmp(text_memory.data(), "0") != 0)
                {
                    cout << text_memory.data() << endl;
                    TXT << text_memory.data() << endl;
                }
                free(text);
                justdidtext = true;
            }
            /*
            else
            {
                // I have NO CERTAINTY that this is correct
                if ((command == 0x000001C8 or command == 0x000001CF) and text_memory != "" and (!deduplciate or !seen.count(text_memory_addr)))
                {
                   // puts(text_memory.data());
                    //TXT << text_memory.data() << endl;
                    if (deduplciate) seen.insert(text_memory_addr);
                }
                justdidtext = false;
            }
            */
            fseek(f, address + header_end + 4, SEEK_SET);
        }

        fclose(f);
   // }
    system("pause");
    return 0;
}