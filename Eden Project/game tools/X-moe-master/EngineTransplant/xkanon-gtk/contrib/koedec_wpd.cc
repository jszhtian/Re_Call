#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"music.h"

extern "C" int is_koe_wpd(char* head);
extern "C" short* decode_koe_wpd(AvgKoeInfo info, int* dest_len);

inline unsigned int read_little_endian_int(const char* buf) {
	unsigned int c1 = ((int)(buf[0])) & 0xff;
	unsigned int c2 = ((int)(buf[1])) & 0xff;
	unsigned int c3 = ((int)(buf[2])) & 0xff;
	unsigned int c4 = ((int)(buf[3])) & 0xff;
	return (c1) | (c2<<8) | (c3<<16) | (c4<<24);
}

inline unsigned short read_little_endian_short(const char* buf) {
	unsigned short c1 = ((int)(buf[0])) & 0xff;
	unsigned short c2 = ((int)(buf[1])) & 0xff;
	return (c1) | (c2<<8);
}


inline int write_little_endian_int(char* buf, int number) {
        int c = read_little_endian_int(buf);
        unsigned char *p = (unsigned char *) buf;
        unsigned int unum = (unsigned int) number;
        p[0] = unum & 255;
        unum >>= 8;
        p[1] = unum & 255;
        unum >>= 8;
        p[2] = unum & 255;
        unum >>= 8;
        p[3] = unum & 255;
        return c;
}

inline int write_little_endian_short(char* buf, int number) {
        int c = read_little_endian_short(buf);
        unsigned char *p = (unsigned char *) buf;
        unsigned int unum = (unsigned int) number;
        p[0] = unum & 255;
        unum >>= 8;
        p[1] = unum & 255;
        return c;
}


/* 声データのビットストリームを読みだすためのクラス */
struct GB {
        unsigned int v;
        int b;
        char* d;

        char* d4; int d4_flag;
        char* d8;
        char* d16;
public:
        GB(char* dd) {
                d = dd + read_little_endian_int(dd);
                d4 = dd + read_little_endian_int(dd+4);
                d8 = dd + read_little_endian_int(dd+8);
                d16 = dd + read_little_endian_int(dd+12);
                v=read_little_endian_int(d); b=0; d=d+4;
                d4_flag = 0;
        }
        int get3(void){
                int ret = (v>>b)&7;
                b += 3;
                if(b>8) {
                        b-=8;
                        v>>=8;
                        v|=int(*(unsigned char*)d)<<24;
                        d++;
                }
                return ret;
        }
        int get4(void) {
                if (d4_flag) {
                        d4_flag = 0; d4++;
                        return *(unsigned char*)(d4-1)>>4;
                } else {
                        d4_flag = 1;
                        return *d4 & 0x0f;
                }
        }
        int get8(void) {
                return *(unsigned char*)d8++;
        }
        int get12(void){
                return (get4()<<8) | get8();
        }
        int get16(void) {
                d16 += 2;
                return read_little_endian_short(d16-2);
        }
};

extern "C" int is_koe_wpd(char* head) {
        char magic[4] = {0x20, 0x44, 0x50, 0x57};
	if (strncmp(head, magic, 4) == 0) return 1;
	else return 0;
}

extern "C" short* decode_koe_wpd(AvgKoeInfo info, int* dest_len) {
	if (info.stream == 0) return 0;
        char* buf = new char[info.length];
	fseek(info.stream, info.offset, 0);
        fread(buf,info.length,1,info.stream);
        char* header = buf;
        if (read_little_endian_int(header+4) != 1) {
                delete[] buf;
                return 0;
        }
        if (read_little_endian_int(header+8) != 2) {
                fprintf(stderr,"no supported header (wpd)\n");
                delete[] buf;
                return 0;
        }

        int outsize = read_little_endian_int(header+0x18);
	outsize /= 2;
        short* outbuf = (short*)malloc(outsize*2*sizeof(short)+1024);
        short* outptr = outbuf;
        short* outend = outbuf + outsize*2;

        int data = 0;
        GB bits(buf+0x44);
        while(outptr < outend) {
                int flag = bits.get3();
                switch(flag) {
                case 0: data=bits.get16(); break;
                        break;
                case 1: data += bits.get4(); break;
                case 2: data -= bits.get4(); break;
                case 3: data += bits.get8(); break;
                case 4: data -= bits.get8(); break;
                case 5: data += bits.get12(); break;
                case 6: data -= bits.get12(); break;
                case 7: {
                        int count = bits.get8();
                        int i;
                        for (i=0; i<count; i++) {
				*outptr++ = data*8;
				*outptr++ = data*8;
                        }
                        outptr -= 2;
                        break; }
                }
		*outptr++ = data*8;
		*outptr++ = data*8;
        }
        if (outptr - outend != 0) {
            fprintf(stderr, "wpd: overrun = %d\n", outptr - outend);
        }
        delete[] buf;
	if (dest_len) *dest_len = outsize;
	return outbuf;
}
