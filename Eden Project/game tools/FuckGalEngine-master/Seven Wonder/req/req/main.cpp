
#include <stdio.h>

typedef unsigned char alpha;

#pragma pack(1)

struct HDR {
    alpha name[0xc];
    long entry_count;
};

struct ENT_NAM {
    alpha name[0x20];
};

struct ENT_INFO {
    long offset;
    long length;
};

#pragma pack()

int main(int argc, char* argv[])
{
    if(argc < 2) return -1;
    FILE* fp = fopen(argv[1], "rb");
    if(!fp) return -1;
    fseek(fp, 0, SEEK_END);
    long fl = ftell(fp);
    rewind(fp);
    alpha* buf = new alpha[fl + 1024];
    fread(buf, sizeof(alpha), fl, fp);
    fclose(fp);

    HDR *hdr = (HDR*)buf;
    int ent_cnt = hdr->entry_count;
    ENT_NAM* ent_nam = (ENT_NAM*)(hdr + 1);
    ENT_INFO* ent_info = (ENT_INFO*)(ent_nam + ent_cnt);
    alpha* data = (alpha*)(ent_info + ent_cnt);

    for(int i = 0; i < ent_cnt; ++i) {
        fp = fopen((char*)(ent_nam+i)->name, "wb+");
        if(!fp) return 1;
        fwrite(data + (ent_info+i)->offset, sizeof(alpha), (ent_info+i)->length, fp);
        fclose(fp);
    }
    delete [] buf;
    return 0;
}
