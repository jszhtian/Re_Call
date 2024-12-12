// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "framework.h"

typedef HFONT(WINAPI* fnCreateFontA)(
    int nHeight, // logical height of font height
    int nWidth, // logical average character width
    int nEscapement, // angle of escapement
    int nOrientation, // base-line orientation angle
    int fnWeight, // font weight
    DWORD fdwItalic, // italic attribute flag
    DWORD fdwUnderline, // underline attribute flag
    DWORD fdwStrikeOut, // strikeout attribute flag
    DWORD fdwCharSet, // character set identifier
    DWORD fdwOutputPrecision, // output precision
    DWORD fdwClipPrecision, // clipping precision
    DWORD fdwQuality, // output quality
    DWORD fdwPitchAndFamily, // pitch and family
    LPCSTR lpszFace // pointer to typeface name string
    );
fnCreateFontA CreateFontAOLD;
HFONT WINAPI CreateFontAEx(int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight, DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily, LPCSTR lpszFace)
{
    fdwCharSet = GB2312_CHARSET;
    //cout << lpszFace << endl;
    //return CreateFontW(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, wcs);
    return CreateFontA(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, "黑体");
}

PVOID g_pOldCreateFontIndirectA = CreateFontIndirectA;
typedef HFONT(WINAPI* PfuncCreateFontIndirectA)(LOGFONTA* lplf);
HFONT WINAPI NewCreateFontIndirectA(LOGFONTA* lplf)
{
    lplf->lfCharSet = GB2312_CHARSET;
    //cout << lplf->lfFaceName << endl;
    //if (!strcmp(lplf->lfFaceName, "MS UI Gothic"))
    //	strcpy(lplf->lfFaceName, "黑体");
    strcpy(lplf->lfFaceName, "黑体");
    return ((PfuncCreateFontIndirectA)g_pOldCreateFontIndirectA)(lplf);
}

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

FILE* g_cur_mjo_fp = nullptr;
uint32_t g_cur_mjo_pos = 0;
NakedMemory* g_cur_new_mjo = nullptr;

FILE* (__fastcall* sub_4810B0)(char* FullPath, char* Mode);
FILE* __fastcall Ffopen(char* fnm, char* md)
{
    //cout << fnm << endl;
    //auto pos = strrchr(fnm, '\\');
    //if (pos == nullptr)
    //    return sub_4810B0(fnm, md);
    auto mjo_name = wstring(AnsiToUnicode(fnm, 932));

    FILE* Oldfp = sub_4810B0(fnm, md);
    if (!Oldfp)
        return Oldfp;
    //cout << fnm << endl;
    transform(mjo_name.begin(), mjo_name.end(), mjo_name.begin(), tolower);
    if (mjo_name.find(L".mjo") == mjo_name.length() - 4)
    {
        //cout << fnm << endl;
        wchar_t cur_path[1000];
        GetCurrentDirectory(ARRAYSIZE(cur_path), cur_path);
        auto txt_name = wstring(cur_path) + L"\\#txt\\" + mjo_name.substr(0, mjo_name.length() - 4) + L".txt";
        auto read_mjo_name = wstring(cur_path) + L"\\#mjo\\" + mjo_name;
        MyFileReader reader;
        auto txt_mem = reader.ReadToMem(txt_name.c_str());
        if (!txt_mem.Get())
        {
            return Oldfp;
        }
        auto ansi_txt = CvtToAnsi(txt_mem);
        auto ls = SplitTxtA(ansi_txt);
        if (ls[ls.size() - 1] == "")
        {
            ls.pop_back();
        }

        auto mjo_mem = reader.ReadToMem(read_mjo_name.c_str());
        if (!mjo_mem.Get())
        {
            //cout << 2 << endl;
            return Oldfp;
        }
        Mjo mjo_file;
        if (!mjo_file.read(mjo_mem) ||
            !mjo_file.pack(ls))
        {
            Err(L"read or pack mjo fail: %s", mjo_name.c_str());
            return Oldfp;
        }
        delete g_cur_new_mjo;
        g_cur_new_mjo = new NakedMemory(std::move(mjo_file.write()));
        g_cur_mjo_pos = 0;
        g_cur_mjo_fp = Oldfp;

        //cout << fnm << " readed" << endl;
    }
    return Oldfp;
}

size_t(__cdecl* sub_495739)(void* Buffer, size_t ElementSize, size_t ElementCount, FILE* Stream);
size_t __cdecl Ffread(void* Buffer, size_t ElementSize, size_t ElementCount, FILE* Stream)
{
    if (g_cur_mjo_fp == Stream && g_cur_new_mjo)
    {
        auto read_size = ElementSize * ElementCount;
        if (read_size > g_cur_new_mjo->GetSize() - g_cur_mjo_pos)
        {
            read_size = g_cur_new_mjo->GetSize() - g_cur_mjo_pos;
        }
        memcpy(Buffer, (uint8_t*)g_cur_new_mjo->Get() + g_cur_mjo_pos, read_size);
        g_cur_mjo_pos += read_size;
        return ElementCount;
    }
    else
    {
        return sub_495739(Buffer, ElementSize, ElementCount, Stream);
    }
}

int(__cdecl* sub_494B54)(FILE* fp);
int __cdecl Ffclose(FILE* fp)
{
    if (fp == g_cur_mjo_fp)
    {
        g_cur_mjo_fp = nullptr;
        delete g_cur_new_mjo;
        g_cur_new_mjo = nullptr;
    }
    return sub_494B54(fp);
}

void SetAPIHook()
{

    CreateFontAOLD = (fnCreateFontA)GetProcAddress(GetModuleHandle(L"gdi32.dll"), "CreateFontA");
    DetourTransactionBegin();
    DetourAttach((void**)&CreateFontAOLD, CreateFontAEx);
    DetourAttach(&g_pOldCreateFontIndirectA, NewCreateFontIndirectA);

    if (DetourTransactionCommit() != NOERROR)
    {
        MessageBox(NULL, L"ERROR", L"SetAPIHook", MB_OK);
        ExitProcess(-1);
    }
}

void SetMJOHook()
{
    *(DWORD*)&sub_4810B0 = BaseAddr + 0x810B0;
    *(DWORD*)&sub_495739 = BaseAddr + 0x95739;
    *(DWORD*)&sub_494B54 = BaseAddr + 0x94B54;


    DetourAttach((void**)&sub_4810B0, Ffopen);
    DetourAttach((void**)&sub_495739, Ffread);
    DetourAttach((void**)&sub_494B54, Ffclose);
    if (DetourTransactionCommit() != NOERROR) 
    {
        MessageBox(NULL, L"ERROR", L"SetMJOHook", MB_OK);
        ExitProcess(-1);
    }
}
void Main()
{
    DWORD off[] = {
        0xC5700,
        0xC5774,
        0xC579C,
        0xC57A0,
        0xC57A4,
        0xC57A8,
        0xC57AC,
        0xC57B0,
        0xC58F8,
        0xC5900,
        0xC5908,
        0xC5910,
        0xC5918,
        0xC5920,
    };

    WCHAR temp[200] = { 0 };

    DWORD old_prot;
    VirtualProtect((LPVOID)(BaseAddr + 0xC5700), 0x300, PAGE_READWRITE, &old_prot);
    for (int i = 0; i < ARRAYSIZE(off); i++)
    {
        //cout << hex << off[i] << endl;

        auto CurAddr = (char*)BaseAddr + off[i];
        //cout << hex << BaseAddr + off[i] << endl;

        MultiByteToWideChar(932, 0, CurAddr, -1, temp, 200);
        WideCharToMultiByte(936, 0, temp, -1, CurAddr, 200, 0, 0);

        //cout << CurAddr << endl;
    }


    //cout << hex << BaseAddr + 0x810B0 << endl;

    SetAPIHook();
    SetMJOHook();

}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        //make_console();
        Main();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport)void WINAPI Proc()
{
}