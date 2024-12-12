// extMJO.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "framework.h"

const int Mjo::OpTable[80] = {
    4,	-1,	8,	4,	-3,	-3,	-3,	-3,	-3,	-3,	-3,	-3,	-3,	-3,	-3,	10,
    10,	-3,	-3,	-3,	-3,	-3,	-3,	-3,	-3,	-3,	-3,	-3,	-3,	-3,	-3,	-3,
    -3,	-3,	-3,	-3,	-3,	-3,	-3,	-3,	-3,	-1,	-3,	0,	-4,	-4,	-4,	0,
    -4,	-4,	-4,	-4,	6,	6,	-1,	8,	-4,	-4,	2,	-4,	-4,	-4,	0,	0,
    -2,	0,	-1,	-4,	0,	-4,	0,	-4,	-3,	-3,	-3,	-3,	-3,	-3,	-3,	-3,
};

void Mjo::xor_dec(void* block, uint32_t size)
{
    auto bf = (uint8_t*)block;
    for (uint32_t i = 0; i < size; i++)
    {
        bf[i] ^= g_crc32_table[i & 0x3ff];
    }
}

bool Mjo::read(const NakedMemory& mem)
{
    if (!read_to_mem(mem))
    {
        Err(L"header error.");
        return false;
    }
    if (!parse_code())
    {
        Err(L"code parse error.");
        return false;
    }
    return true;
}

bool Mjo::read_to_mem(const NakedMemory& mem)
{
    if (mem.GetSize() < sizeof(info_))
    {
        return false;
    }
    auto cur = (uint8_t*)mem.Get();
    memcpy(info_.magic, cur, 16);
    auto get_u32 = [&cur]() {
        cur += 4;
        return *(uint32_t*)(cur - 4);
        };
    cur += 16;
    info_.entry_point = get_u32();
    info_.unk = get_u32();
    info_.iat_cnt = get_u32();

    if (sizeof(info_) + info_.iat_cnt * 8 > mem.GetSize())
    {
        return false;
    }
    for (size_t i = 0; i < info_.iat_cnt; i++)
    {
        iat_.push_back(*(MjoIAT*)cur);
        cur += 8;
    }
    info_.code_size = get_u32();
    if (sizeof(info_) + info_.iat_cnt * 8 + info_.code_size > mem.GetSize())
    {
        return false;
    }

    code_.SetSize((info_.code_size + 300) * 2);
    memset(code_.Get(), 0, code_.GetSize());
    memcpy(code_.Get(), cur, info_.code_size);

    xor_dec(code_.Get(), info_.code_size);
    return true;
}

bool Mjo::parse_code()
{
    auto cur = (uint8_t*)code_.Get();
    auto get_u16 = [&cur]() {
        cur += 2;
        return *(uint16_t*)(cur - 2);
        };
    auto get_i32 = [&cur]() {
        cur += 4;
        return *(int32_t*)(cur - 4);
        };
    auto get_str = [&]() {
        auto len = get_u16();
        auto real_len = cur[len - 1] == 0 ? len - 1 : len;
        auto s = string((char*)cur, real_len);
        cur += len;
        return s;
        };
    auto get_multi_str = [&]() {
        auto s = get_str();
        //if (*(uint16_t*)cur == OpLineNumber && *(uint16_t*)(cur + 4) == OpCatString)
        //{
        //    cur += 6;
        //    s += '@' + get_str();
        //}
        while (*(cur + 6) == 'n' &&
            *(uint16_t*)cur == OpProcessString &&
            *(uint16_t*)(cur + 2) == OpCtrlString &&
            *(uint16_t*)(cur + 12) == OpCatString)
        {
            cur += 14;
            s += "\\n" + get_str();
        }
        return s;
        };
    auto set_err = [&](uint32_t op) {
        Err(L"error, pos:0x%x, op:0x%x", cur - (uint8_t*)code_.Get(), op);
        };

    auto end_ptr = (uint8_t*)code_.Get() + info_.code_size;
    auto last_string_ptr = cur;

    while (cur < end_ptr)
    {
        auto op = get_u16();
        if (op <= 0x1a9)
        {
            continue;
        }
        else if (op <= 0x320)
        {
            cur += 8;
        }
        else if (op == OpPushStr)
        {
            auto old = cur;
            auto s = get_str();
            if ((uint8_t)s[0] >= 0x81)
            {
                last_string_ptr = old;
                lines_.push_back(s);
                ptrs_.push_back({ old, (uint32_t)(cur - old) });
            }
        }
        else if (op >= 0x800 && op <= 0x847)
        {
            auto flag = OpTable[op - 0x800];
            if (flag >= 0)
            {
                cur += flag;
                continue;
            }
            int32_t dist = 0;
            uint8_t* old = nullptr;
            switch (flag)
            {
            case -1:
                cur += get_u16();
                break;
            case -2:
                last_string_ptr = cur;
                lines_.push_back(get_multi_str());
                ptrs_.push_back({ last_string_ptr, (uint32_t)(cur - last_string_ptr) });
                break;
            case -3:
                set_err(op);
                return false;
            case -4:
                dist = get_i32();
                if (dist > 0 || cur + dist < last_string_ptr)
                {
                    jmp_tbl_.push_back({ cur - 4 });
                }
                break;
            default:
                assert(false);
                break;
            }
        }
        else if (op == OpJumpTable)
        {
            auto cnt = get_u16();
            mjmp_tbl_.push_back({ cnt, cur });
            cur += cnt * 4;
        }
        else
        {
            set_err(op);
            return false;
        }
    }
    return true;
}

bool Mjo::pack(const std::vector<std::string>& lines)
{
    if (lines.size() != lines_.size())
    {
        Err(L"lines cnt error, ori:%d, new:%d", lines_.size(), lines.size());
        return false;
    }
    temp_.SetSize(code_.GetSize());
    for (size_t i = 0; i < lines.size(); i++)
    {
        if (!pack_line(lines[i], i))
        {
            return false;
        }
    }
    return true;
}

bool Mjo::pack_line(const std::string& line, uint32_t idx)
{
    auto cur = ptrs_[idx].ptr;
    auto ori_size = ptrs_[idx].ori_size;
    if (*(uint16_t*)(cur - 2) == OpPushStr && line.find("\\n") != string::npos)
    {
        Err(L"inst PushStr don't need \\n, line: %d", idx + 1);
        return false;
    }
    auto new_insts = gen_new_inst(line);
    insert_block(cur, ori_size, (uint8_t*)new_insts.Get(), new_insts.GetSize(), (uint8_t*)code_.Get() + info_.code_size);
    ptrs_[idx].ori_size = new_insts.GetSize();
    fix_offsets(idx, cur, new_insts.GetSize() - ori_size);
    info_.code_size += (new_insts.GetSize() - ori_size);
    return true;
}

NakedMemory Mjo::gen_new_inst(const std::string& line)
{
    auto estimate_line_cnt = std::count(line.begin(), line.end(), '\\');
    NakedMemory mem(line.length() + 3 + estimate_line_cnt * 30);
    auto cur = (uint8_t*)mem.Get();
    auto wu16 = [&cur](uint16_t val) {
        *(uint16_t*)cur = val;
        cur += 2;
        };
    auto wcstr = [&](const char* s, uint32_t n) {
        memcpy(cur, s, n);
        cur[n] = 0;
        cur += n + 1;
        };
    auto wpstr = [&](const char* s, uint32_t n) {
        wu16(n + 1);
        wcstr(s, n);
        };

    auto pos = line.find("\\n");
    if (pos == string::npos)
    {
        pos = line.length();
    }
    wpstr(line.c_str(), pos);

    pos += 2;
    while (pos < line.length())
    {
        auto next = line.find("\\n", pos);
        if (next == string::npos)
        {
            next = line.length();
        }
        wu16(OpProcessString);
        wu16(OpCtrlString);
        wpstr("n", 1);
        wu16(OpLineNumber);
        wu16(1);
        wu16(OpCatString);
        wpstr(line.c_str() + pos, next - pos);
        pos = next + 2;
    }
    mem.SetSize(cur - (uint8_t*)mem.Get());
    return std::move(mem);
}

void Mjo::insert_block(uint8_t* cur, uint32_t cur_size, uint8_t* newp, uint32_t new_size, uint8_t* cur_end)
{
    if (cur_size >= new_size + 16)
    {
        memcpy(cur, newp, new_size);
        memcpy(cur + new_size, cur + cur_size, cur_end - (cur + cur_size));
    }
    else if (new_size == cur_size)
    {
        memcpy(cur, newp, new_size);
    }
    else
    {
        memcpy(temp_.Get(), cur + cur_size, cur_end - (cur + cur_size));
        memcpy(cur, newp, new_size);
        memcpy(cur + new_size, temp_.Get(), cur_end - (cur + cur_size));
    }
}

void Mjo::fix_offsets(uint32_t cur_idx, uint8_t* start, int32_t dist)
{
    if (dist == 0)
    {
        return;
    }

    for (auto i = cur_idx + 1; i < ptrs_.size(); i++)
    {
        ptrs_[i].ptr += dist;
    }

    auto cur_off = (uint32_t)(start - (uint8_t*)code_.Get());
    for (auto& ia : iat_)
    {
        if (ia.offset > cur_off)
        {
            ia.offset += dist;
        }
    }

    if (info_.entry_point > cur_off)
    {
        info_.entry_point += dist;
    }

    for (auto& jmp : jmp_tbl_)
    {
        if (jmp.offset > start)
        {
            jmp.offset += dist;
        }
        auto ori_dist = *(int32_t*)jmp.offset;
        auto dest = jmp.offset + 4 + ori_dist;
        if (dest <= start && jmp.offset > start)
        {
            *(int32_t*)jmp.offset -= dist;
        }
        else if (dest > start && jmp.offset < start)
        {
            *(int32_t*)jmp.offset += dist;
        }
    }

    for (auto& mjmp : mjmp_tbl_)
    {
        if (mjmp.offset > start)
        {
            mjmp.offset += dist;
        }
        auto next_inst_ptr = mjmp.offset + mjmp.tbl_cnt * 4;
        auto offsets = (int32_t*)mjmp.offset;
        for (size_t i = 0; i < mjmp.tbl_cnt; i++)
        {
            auto dest = next_inst_ptr + offsets[i];
            if (dest <= start && mjmp.offset > start)
            {
                offsets[i] -= dist;
            }
            else if (dest > start && mjmp.offset < start)
            {
                offsets[i] += dist;
            }
        }
    }
}

NakedMemory Mjo::write()
{
    NakedMemory mem(0x20 + info_.iat_cnt * 8 + info_.code_size);
    auto p = (uint8_t*)mem.Get();
    memcpy(p, info_.magic, 16);
    p += 16;
    auto wu32 = [&p](uint32_t val) {
        *(uint32_t*)p = val;
        p += 4;
        };
    wu32(info_.entry_point);
    wu32(info_.unk);
    wu32(info_.iat_cnt);
    for (auto& ia : iat_)
    {
        wu32(ia.hash);
        wu32(ia.offset);
    }
    wu32(info_.code_size);
    memcpy(p, code_.Get(), info_.code_size);
    xor_dec(p, info_.code_size);
    return std::move(mem);
}

void __stdcall TestPack(const wchar_t* mjo_name, const wchar_t* txt_name, const wchar_t* new_mjo_name)
{
    MyFileReader reader;
    auto mem = reader.ReadToMem(mjo_name);

    Mjo mjo;
    if (!mjo.read(mem))
    {
        Err(L"read faild");
        return;
    }

    auto txtmem = reader.ReadToMem(txt_name);
    auto ls = SplitTxtA(txtmem);
    if (ls[ls.size() - 1] == "")
    {
        ls.pop_back();
    }

    if (!mjo.pack(ls))
    {
        Err(L"%s pack fail, txt: %s", mjo_name, txt_name);
        return;
    }

    auto nm = mjo.write();

    FILE* fp;
    _wfopen_s(&fp, new_mjo_name, L"wb");
    if (!fp)
    {
        return;
    }
    fwrite(nm.Get(), 1, nm.GetSize(), fp);
    fclose(fp);
}

void __stdcall TestParse(const wchar_t* mjo_name, const wchar_t* txt_name)
{
    MyFileReader reader;
    auto mem = reader.ReadToMem(mjo_name);

    Mjo mjo;
    if (!mjo.read(mem))
    {
        Err(L"read faild");
        return;
    }
    auto& ls = mjo.get_lines();

    FILE* fp = nullptr;
    _wfopen_s(&fp, txt_name, L"wb");
    if (!fp)
    {
        return;
    }
    for (auto& s : ls)
    {
        fwrite(s.c_str(), 1, s.length(), fp);
        fwrite("\r\n", 1, 2, fp);
    }
    fclose(fp);
}

int main(int argc, char* argv[])
{
    WCHAR NameBox[4096] = { 0 };
    wsprintfW(NameBox, L"%s.txt", AnsiToUnicode(argv[1], CP_ACP));
    //TestParse(L"novel.mjo", L"novel.mjo.txt");
    cout << argv[1]  << endl;
    TestParse(AnsiToUnicode(argv[1], CP_ACP), NameBox);
    //TestPack(L"narsumi00.mjo", L"narsumi00.txt", L"narsumi00.mjo.new");
    //system("pause");
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
