/*  senario_flags.cc
 *     シナリオファイルの変数の操作に関係する
 *     処理を行う
 */
/*
 *
 *  Copyright (C) 2000-   Kazunori Ueno(JAGARL) <jagarl@creator.club.ne.jp>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
*/

#include "file.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "senario.h"

#define pp(X) do { printf("not supported command! : "); printf X; } while(0)

/*
#define DEBUG_READDATA
#define DEBUG_CalcVar
#define DEBUG_CalcStr
#define DEBUG_Select
#define DEBUG_Condition
*/

// change unsigned -> signed...
inline char* strcpy(unsigned char* d, const unsigned char* s) { 
	return strcpy( (char*)d, (const char*)s );
}

inline int strlen(const unsigned char* s) {
	return strlen( (const char*)s);
}

unsigned int SENARIO_FLAGS::bits[32] = { // LITTLE ENDIAN...
	0x00000080, 0x00000040, 0x00000020, 0x00000010,
	0x00000008, 0x00000004, 0x00000002, 0x00000001,
	0x00008000, 0x00004000, 0x00002000, 0x00001000,
	0x00000800, 0x00000400, 0x00000200, 0x00000100,
	0x00800000, 0x00400000, 0x00200000, 0x00100000,
	0x00080000, 0x00040000, 0x00020000, 0x00010000,
	0x80000000, 0x40000000, 0x20000000, 0x10000000,
	0x08000000, 0x04000000, 0x02000000, 0x01000000};

SENARIO_FLAGS::SENARIO_FLAGS(void) {
	int i;
	memset(variables, 0, sizeof(unsigned int)*2048);
	memset(bit_variables, 0, sizeof(unsigned int)*64);
	for (i=0; i<128; i++) {
		memset(string_variables[i],0, 0x40);
	}
	dirty = 0;
	bit_dirty = 0;
	memset(var_dirty, 0, sizeof(unsigned int)*32);
	memset(str_dirty, 0, sizeof(unsigned int)*4);
}

SENARIO_FLAGS::~SENARIO_FLAGS() {
}

void SENARIO_FLAGS::SetStrVar(int number, const char* src) {
	if (src == 0) src = "";
	number &= 127;
	SetStrDirty(number);
	strncpy(string_variables[number], src, 63);
	string_variables[number][63] = 0;
}

void SENARIO_FLAGS::Copy(const SENARIO_FLAGS& src) {
	dirty = src.dirty; bit_dirty = src.bit_dirty;
	memcpy(variables, src.variables, sizeof(unsigned int)*2048);
	memcpy(bit_variables, src.bit_variables, sizeof(unsigned int)*64);
	int i;for (i=0; i<128; i++) {
		memcpy(string_variables[i], src.string_variables[i], 0x40);
	}
	memcpy(var_dirty, src.var_dirty, sizeof(unsigned int)*32);
	memcpy(str_dirty, src.str_dirty, sizeof(unsigned int)*4);
}

/* 変数の処理に関係したシナリオ処理 */
/* 変数の代入、計算など */

/* for debug */
#ifdef p
#  undef p
#endif /* defined(p) */

#ifdef DEBUG_CalcVar
#define p(X) printf X
#else
#define p(X)
#endif

void SENARIO_FLAGSDecode::DecodeSenario_CalcVar(SENARIO_DECODE& decoder) {
	unsigned char cmd = decoder.Cmd();
	int index, data = 0;
	index = decoder.ReadData();
	if (cmd != 0x56) { // 0x56 : 引数が１つ
		data = decoder.ReadData();
	}
	switch(cmd) {
		// bit variable
		case 0x37:
			p(( "cmd 0x37 : SetBit %d(%d) = %d\n",index,
				(GetBit(index)!=0), data));
			SetBit(index, data);
			break;
		case 0x39:
			p(( "cmd 0x39 : SetBit %d(%d) = index %d(%d)\n",index,
				(GetBit(index)!=0), data,GetBit(data)!=0));
			SetBit(index, GetBit(data));
			break;
		// operation with immediate value
		case 0x3b:
			p(( "cmd 0x3b : SetVar %d(%d) = %d\n",index,
				GetVar(index),data));
			SetVar(index, data);
			break;
		case 0x3c:
			p(( "cmd 0x3c : SetVar %d(%d) += %d\n",index,
				GetVar(index),data));
			SetVar(index, GetVar(index)+data);
			break;
		case 0x3d:
			p(( "cmd 0x3d : SetVar %d(%d) -= %d\n",index,
				GetVar(index),data));
			SetVar(index, GetVar(index)-data);
			break;
		case 0x3e:
			p(( "cmd 0x3e : SetVar %d(%d) *= %d\n",index,
				GetVar(index),data));
			SetVar(index, GetVar(index)*data);
			break;
		case 0x3f:
			p(( "cmd 0x3f : SetVar %d(%d) /= %d\n",index,
				GetVar(index),data));
			if (data != 0) SetVar(index, GetVar(index)/data);
			break;
		case 0x40:
			p(( "cmd 0x40 : SetVar %d(%d) %%= %d\n",index,
				GetVar(index),data));
			if (data != 0) SetVar(index, GetVar(index)%data);
			break;
		case 0x41:
			p(( "cmd 0x41 : SetVar %d(%d) &= %d\n",index,
				GetVar(index),data));
			SetVar(index, GetVar(index)&data);
			break;
		case 0x42:
			p(( "cmd 0x42 : SetVar %d(%d) |= %d\n",index,
				GetVar(index),data));
			SetVar(index, GetVar(index)|data);
			break;
		case 0x43:
			p(( "cmd 0x43 : SetVar %d(%d) ^= %d\n",index,
				GetVar(index),data));
			SetVar(index, GetVar(index)^data);
			break;
		// operation with memory value
		case 0x49:
			p(( "cmd 0x49 : SetVar %d(%d) = index %d(%d)\n",index,
				GetVar(index),data, GetVar(data)));
			SetVar(index, GetVar(data) );
			break;
		case 0x4a:
			p(( "cmd 0x4a : SetVar %d(%d) += var[%d](%d)\n",index,
				GetVar(index),data, GetVar(data)));
			SetVar(index, GetVar(index)+GetVar(data) );
			break;
		case 0x4b:
			p(( "cmd 0x4b : SetVar %d(%d) -= var[%d](%d)\n",index,
				GetVar(index),data, GetVar(data)));
			SetVar(index, GetVar(index)-GetVar(data) );
			break;
		case 0x4c:
			p(( "cmd 0x4c : SetVar %d(%d) *= var[%d](%d)\n",index,
				GetVar(index),data, GetVar(data)));
			SetVar(index, GetVar(index)*GetVar(data) );
			break;
		case 0x4d:
			p(( "cmd 0x4d : SetVar %d(%d) /= var[%d](%d)\n",index,
				GetVar(index),data, GetVar(data)));
			if (GetVar(data) != 0) SetVar(index, GetVar(index)/GetVar(data) );
			break;
		case 0x4e:
			p(( "cmd 0x4e : SetVar %d(%d) %%= var[%d](%d)\n",index,
				GetVar(index),data, GetVar(data)));
			if (GetVar(data) != 0) SetVar(index, GetVar(index)%GetVar(data) );
			break;
		case 0x4f:
			p(( "cmd 0x4f : SetVar %d(%d) &= var[%d](%d)\n",index,
				GetVar(index),data, GetVar(data)));
			SetVar(index, GetVar(index)&GetVar(data) );
			break;
		case 0x50:
			p(( "cmd 0x50 : SetVar %d(%d) |= var[%d](%d)\n",index,
				GetVar(index),data, GetVar(data)));
			SetVar(index, GetVar(index)|GetVar(data) );
			break;
		case 0x51:
			p(( "cmd 0x51 : SetVar %d(%d) ^= var[%d](%d)\n",index,
				GetVar(index),data, GetVar(data)));
			SetVar(index, GetVar(index)^GetVar(data) );
			break;
		/* rand */
		case 0x56:
			p(( "cmd 0x56 : SetBit %d(%d) = rand",index,
				(GetBit(index)!=0)));
#ifndef SUPRESS_RAND
			SetBit(index, decoder.local_system.Rand(1) );
#endif
			p(( "(%d)\n",(GetBit(index)!=0) ));
			break;
		case 0x57:  { int max = decoder.ReadData();
			p(( "cmd 0x57 : SetVar %d(%d) = rand(%d - %d",index,
				GetVar(index),data,max));
#ifndef SUPRESS_RAND
			SetVar(index, decoder.local_system.Rand(max-data+1)+data);
#endif
			p(( " : %d )\n",GetVar(index) ));
			break;}
	}
	return;
}

/* 文字変数の操作に関するコマンド */
/* コマンド 0x59 */
/* for debug */
#ifdef p
#  undef p
#endif /* defined(p) */

#ifdef DEBUG_CalcStr
#define p(X) printf X
#else
#define p(X)
#endif

inline int strwlen(const char* s) {
	int l=0;
	while(*s) { 
		if (*s < 0 && s[1] != 0) s++;
		l++; s++;
	}
	return l;
}
inline int strwpos(const char* s, int p) {
	const char* s_orig = s;
	while(*s && p) {
		if (*s < 0 && s[1] != 0) s++;
		s++; p--;
	}
	return s - s_orig;

}
inline void strwfix(char* s, int len) {
	while(*s && len) {
		if (*s < 0 && s[1] != 0) {
			if (len <= 1) break;
			s++; len--;
		}
		s++; len--;
	}
	*s = 0; return;
}

void SENARIO_FLAGSDecode::DecodeSenario_CalcStr(SENARIO_DECODE& decoder) {
	unsigned char c = decoder.NextCharwithIncl();
	int arg1, arg2, arg3; char tmp[1024]; char* tmp2; char ktemp1[1024], ktemp2[1024];
	char* s1, * s2;
	if (c < 1 || c > 0x40) {
		fprintf(stderr, "Invalid senario operation : code 0x59 \n");
		return;
	}
	arg1 = decoder.ReadData();
	switch(c) {
		case 1: // 代入
			tmp2 = decoder.ReadString(tmp);
			kconv((unsigned char*)StrVar(arg1), (unsigned char*)ktemp1);
			kconv((unsigned char*)tmp, (unsigned char*)ktemp2);
			p(( "cmd 0x59 - 1. SetStr %d(%s) <- %s\n",
				arg1,ktemp1,ktemp2 ));
			SetStrVar(arg1, tmp);
			break;
		case 2: // strlen
			arg2 = decoder.ReadData();
			kconv((unsigned char*)StrVar(arg2), (unsigned char*)ktemp1);
			p(( "cmd 0x59 - 2. Strlen var[%d](%d) <- strlen(Str[%d] = %s) = %d\n", arg1, GetVar(arg1), arg2, ktemp1, strlen(StrVar(arg2)) ));
			SetVar(arg1, strlen(StrVar(arg2)));
			break;
		case 3: // strcmp
			arg2 = decoder.ReadData(); arg3 = decoder.ReadData();
			kconv((unsigned char*)StrVar(arg2), (unsigned char*)ktemp1);
			kconv((unsigned char*)StrVar(arg3), (unsigned char*)ktemp2);
			p(( "cmd 0x59-3. Strcmp var[%d](%d) <- strcmp(Str[%d] = %s, Str[%d] = %s) = %d\n",arg1,GetVar(arg1),arg2,ktemp1,arg3,ktemp2,strcmp(StrVar(arg2),StrVar(arg3)) ));
			SetVar(arg1, strcmp( StrVar(arg2), StrVar(arg3)) );
			break;
		case 4: // strcat
			arg2 = decoder.ReadData();
			kconv((unsigned char*)StrVar(arg1), (unsigned char*)ktemp1);
			kconv((unsigned char*)StrVar(arg2), (unsigned char*)ktemp2);
			p(( "cmd 0x59-4. Strcat Str[%d]=%s <- Str[%d]=%s\n",
				arg1,ktemp1,arg2,ktemp2 ));
			strcat(StrVar_nonConst(arg1), StrVar(arg2));
			break;
		case 5: // strcpy
			arg2 = decoder.ReadData();
			kconv((unsigned char*)StrVar(arg1), (unsigned char*)ktemp1);
			kconv((unsigned char*)StrVar(arg2), (unsigned char*)ktemp2);
			p(( "cmd 0x59-5. Strcpy Str[%d]=%s <- Str[%d]=%s\n",
				arg1,ktemp1,arg2,ktemp2 ));
			SetStrVar(arg1, StrVar(arg2));
			break;
		case 6: // itoa
			arg2 = decoder.ReadData(); arg3 = decoder.ReadData();
			kconv((unsigned char*)StrVar(arg2), (unsigned char*)ktemp1);
			p(( "cmd 0x59-6. itoa. Str[%d]=%s <- "
				"Var[%d] = %d ; radix = %d\n",
				arg2, ktemp1,
				arg1, GetVar(arg1),
				arg3 ));
			if (arg3 == 10) { // １０進
				sprintf(StrVar_nonConst(arg2), "%d", GetVar(arg1));
			} else if (arg3 == 16) { // １６進
				sprintf(StrVar_nonConst(arg2), "%x", GetVar(arg1));
			} else {
				fprintf(stderr, "Unsupported radix : %d : code 0x59 \n", arg3);
			}
			break;
		case 7: // han2zen
			tmp2 = SENARIO_DECODE::Han2Zen( StrVar(arg1) );
			kconv((unsigned char*)StrVar(arg1), (unsigned char*)ktemp1);
			kconv((unsigned char*)tmp2, (unsigned char*)ktemp1);
			p(( "cmd 0x59-7. han2zen. Str[%d] = %s <- %s\n",
				arg1, ktemp1, ktemp2 ));
			SetStrVar(arg1, tmp2);
			delete[] tmp2;
			break;
		case 8: // atoi
			// from akz. version
			arg2 = decoder.ReadData();
			kconv((unsigned char*)StrVar(arg1), (unsigned char*)ktemp1);
			p(( "cmd 0x59-8. atoi. var[%d](%d) <- Str[%d]=%s\n",
				arg2, GetVar(arg2), arg1, ktemp1 ));
			SetVar(arg2, atoi(StrVar(arg1)) );
			break;
		case 0x10: // trimleft
			{ arg2 = decoder.ReadData(); arg3 = decoder.ReadData();
			p(("cmd 0x59 - 0x10 : cut left %d charactor of Str[%d] and copy to Str[%d]\n", arg3, arg2, arg1));
			int l = strlen(StrVar(arg2));
			if (arg3 > l) arg3 = l;
			strcpy(tmp, StrVar(arg2)); strcpy(tmp+512, StrVar(arg2));
			s1 = tmp; s1[arg3] = 0;
			s2 = tmp+512+arg3;
			}
			goto copy_2str;
		case 0x11: // trimright
			{ arg2 = decoder.ReadData(); arg3 = decoder.ReadData();
			p(("cmd 0x59 - 0x11 : cut left %d charactor of Str[%d] and copy to Str[%d]\n", arg3, arg2, arg1));
			int l = strlen(StrVar(arg2));
			if (arg3 > l) arg3 = l;
			strcpy(tmp, StrVar(arg2)); strcpy(tmp+512, StrVar(arg2));
			s2 = tmp; s2[l-arg3] = 0;
			s1 = tmp+512+l-arg3;
			}
			goto copy_2str;
		case 0x20: // trimleft (全角対応)
			{ arg2 = decoder.ReadData(); arg3 = decoder.ReadData();
			p(("cmd 0x59 - 0x20 : cut right %d wide charactor of Str[%d] and copy to Str[%d]\n", arg3, arg2, arg1));
			int l = strwlen(StrVar(arg2));
			if (arg3 > l) arg3 = l;
			strcpy(tmp, StrVar(arg2)); strcpy(tmp+512, StrVar(arg2));
			arg3 = strwpos(tmp, arg3); /* 全角位置への変換 */
			s1 = tmp; s1[arg3] = 0;
			s2 = tmp+512+arg3;
			}
			goto copy_2str;
		case 0x21: // trimright(全角対応)
			{ arg2 = decoder.ReadData(); arg3 = decoder.ReadData();
			p(("cmd 0x59 - 0x21 : cut right %d wide charactor of Str[%d] and copy to Str[%d]\n", arg3, arg2, arg1));
			int l = strwlen(StrVar(arg2));
			if (arg3 > l) arg3 = l;
			strcpy(tmp, StrVar(arg2)); strcpy(tmp+512, StrVar(arg2));
			arg3 = strwpos(tmp, l-arg3);
			s2 = tmp; s2[arg3] = 0;
			s1 = tmp+512+arg3;
			}
			goto copy_2str;
		copy_2str:
			SetStrVar(arg2, s2);
			SetStrVar(arg1, s1);
			break;
		case 0x12: // left$
			{ arg2 = decoder.ReadData(); arg3 = decoder.ReadData();
			p(("cmd 0x59 - 0x12 : copy left %d charactor of Str[%d] to Str[%d]\n",arg3,arg2,arg1));
			int l = strlen(StrVar(arg2));
			if (arg3 > l) arg3 = l;
			strcpy(tmp, StrVar(arg2));
			s1 = tmp; s1[arg3] = 0;
			}
			goto copy_1str;
		case 0x13: // right$
			{ arg2 = decoder.ReadData(); arg3 = decoder.ReadData();
			p(("cmd 0x59 - 0x13 : copy right %d charactor of Str[%d] to Str[%d]\n",arg3,arg2,arg1));
			int l = strlen(StrVar(arg2));
			if (arg3 > l) arg3 = l;
			strcpy(tmp, StrVar(arg2));
			s1 = tmp; s1 += l-arg3;
			}
			goto copy_1str;
		case 0x14: // mid$
			{ arg2 = decoder.ReadData(); int pos = decoder.ReadData(); int len = decoder.ReadData();
			p(("cmd 0x59 - 0x14 : copy %d charactor from %d pos of Str[%d] to Str[%d]\n",len,pos,arg2,arg1));
			int l = strlen(StrVar(arg2));
			if (pos > l) pos = l;
			if (pos+len > l) len = l-pos;
			strcpy(tmp, StrVar(arg2));
			s1 = tmp+pos; s1[len] = 0;
			}
			goto copy_1str;
		case 0x22: // left$
			{ arg2 = decoder.ReadData(); arg3 = decoder.ReadData();
			p(("cmd 0x59 - 0x22 : copy left %d wide charactor of Str[%d] to Str[%d]\n",arg3,arg2,arg1));
			int l = strwlen(StrVar(arg2));
			if (arg3 > l) arg3 = l;
			strcpy(tmp, StrVar(arg2));
			arg3 = strwpos(tmp, arg3);
			s1 = tmp; s1[arg3] = 0;
			}
			goto copy_1str;
		case 0x23: // right$
			{ arg2 = decoder.ReadData(); arg3 = decoder.ReadData();
			p(("cmd 0x59 - 0x23 : copy right %d wide charactor of Str[%d] to Str[%d]\n",arg3,arg2,arg1));
			int l = strwlen(StrVar(arg2));
			if (arg3 > l) arg3 = l;
			strcpy(tmp, StrVar(arg2));
			arg3 = strwpos(tmp, l-arg3);
			s1 = tmp; s1 += arg3;
			}
			goto copy_1str;
		case 0x24: // mid$
			{ arg2 = decoder.ReadData(); int pos = decoder.ReadData(); int len = decoder.ReadData();
			p(("cmd 0x59 - 0x24 : copy %d wide charactor from %d pos of Str[%d] to Str[%d]\n",len,pos,arg2,arg1));
			int l = strwlen(StrVar(arg2));
			if (pos > l) pos = l;
			if (pos+len > l) len = l-pos;
			strcpy(tmp, StrVar(arg2));
			len = strwpos(tmp, pos+len) - strwpos(tmp, pos);
			pos = strwpos(tmp, pos);
			s1 = tmp+pos; s1[len] = 0;
			}
			goto copy_1str;
		copy_1str:
			SetStrVar(arg1, s1);
			break;
		case 0x15: // trimleftchar
			{ arg2 = decoder.ReadData();
			p(("cmd 0x59 - 0x15 : cut first chararctor of Str[%d] and set its code to Var[%d]", arg2, arg1));
			strcpy(tmp, StrVar(arg2));
			SetVar(arg1, int(*tmp)&0xff);
			if (tmp[0] != 0) SetStrVar(arg2, tmp+1);
			break;
			}
		case 0x16: // trimrightchar
			{ arg2 = decoder.ReadData();
			p(("cmd 0x59 - 0x16 : cut last chararctor of Str[%d] and set its code to Var[%d]", arg2, arg1));
			strcpy(tmp, StrVar(arg2));
			int l = strlen(tmp); if (l == 0) l=1;
			SetVar(arg1, int(tmp[l-1])&0xff);
			tmp[l-1] = 0;
			SetStrVar(arg2, tmp);
			break;
			}
		case 0x25: // trimleftchar
			{ arg2 = decoder.ReadData();
			p(("cmd 0x59 - 0x25 : cut first wide chararctor of Str[%d] and set its code to Var[%d]", arg2, arg1));
			strcpy(tmp, StrVar(arg2));
			if (tmp[0] == 0) SetVar(arg1,0);
			else if (tmp[1] == 0 || tmp[0] > 0) { SetVar(arg1, int(tmp[0])&0xff); SetStrVar(arg2, tmp+1);}
			else { SetVar(arg1, (int(tmp[0])&0xff) | ((int(tmp[1]&0xff))<<8)); SetStrVar(arg2, tmp+2);}
			break;
			}
		case 0x26: // trimrightchar
			{ arg2 = decoder.ReadData();
			p(("cmd 0x59 - 0x26 : cut last wide chararctor of Str[%d] and set its code to Var[%d]", arg2, arg1));
			strcpy(tmp, StrVar(arg2));
			int l = strwlen(tmp); if (l == 0) l=1;
			l = strwpos(tmp,l-1);
			if (tmp[l] < 0) SetVar(arg1, (int(tmp[l])&0xff) | ((int(tmp[l+1])&0xff)<<8));
			else SetVar(arg1, int(tmp[l])&0xff);
			tmp[l] = 0;
			SetStrVar(arg2, tmp);
			break;
			}
		case 0x17: // insertleftchar
			{ arg2 = decoder.ReadData();
			p(("cmd 0x59 - 0x17 : insert charactor <%02x> to the top of Str[%d]\n",arg2&0xff,arg1));
			strcpy(tmp+1, StrVar(arg1));
			tmp[0] = arg2;
			SetStrVar(arg1, tmp);
			break;
			}
		case 0x18: // insertrightchar
			{ arg2 = decoder.ReadData();
			p(("cmd 0x59 - 0x18 : insert charactor <%02x> to the end of Str[%d]\n",arg2&0xff,arg1));
			strcpy(tmp, StrVar(arg1));
			int l = strlen(tmp); tmp[l] = arg2; tmp[l+1] = 0;
			SetStrVar(arg1, tmp);
			break;
			}
		case 0x19: // insertmidchar
			{ arg2 = decoder.ReadData(); int pos = decoder.ReadData();
			p(("cmd 0x59 - 0x19 : insert charactor <%02x> to position %d of Str[%d]\n",arg2&0xff,arg3,arg1));
			strcpy(tmp, StrVar(arg1));
			int l = strlen(tmp); if (pos > l) pos = l;
			strcpy(tmp+pos+1, StrVar(arg1)+pos);
			tmp[pos] = arg2;
			SetStrVar(arg1, tmp);
			break;
			}
		case 0x27: // insertleftchar
			{ arg2 = decoder.ReadData();
			if ( (arg2&0xff00) < 0x8000 && (arg2&0xff) > 0x80) arg2 &= 0xff7f; /* 不正な文字は半角にする */
			p(("cmd 0x59 - 0x27 : insert charactor <%04x> to the top of Str[%d]\n",arg2&0xffff,arg1));
			if (arg2&0xff00) {
				strcpy(tmp+2, StrVar(arg1));
				tmp[0] = arg2>>8; tmp[1] = arg2;
			} else { /* １文字 */
				strcpy(tmp+1, StrVar(arg1));
				tmp[0] = arg2;
			}
			strwfix(tmp, 63);
			SetStrVar(arg1, tmp);
			break;
			}
		case 0x28: // insertrightchar
			{ arg2 = decoder.ReadData();
			if ( (arg2&0xff00) < 0x8000 && (arg2&0xff) > 0x80) arg2 &= 0xff7f; /* 不正な文字は半角にする */
			p(("cmd 0x59 - 0x28 : insert wide charactor <%04x> to the end of Str[%d]\n",arg2&0xffff,arg1));
			strcpy(tmp, StrVar(arg1));
			int l = strlen(tmp);
			if (arg2&0xff00) {
				strcpy(tmp, StrVar(arg1));
				tmp[l] = arg2>>8; tmp[l+1] = arg2; tmp[l+2] = 0;
			} else { /* １文字 */
				strcpy(tmp, StrVar(arg1));
				tmp[l] = arg2; tmp[l+1] = 0;
			}
			strwfix(tmp, 63);
			SetStrVar(arg1, tmp);
			break;
			}
		case 0x29: // insertmidchar
			{ arg2 = decoder.ReadData(); int pos = decoder.ReadData();
			if ( (arg2&0xff00) < 0x8000 && (arg2&0xff) > 0x80) arg2 &= 0xff7f; /* 不正な文字は半角にする */
			p(("cmd 0x59 - 0x19 : insert wide charactor <%04x> to position %d of Str[%d]\n",arg2&0xffff,pos,arg1));
			strcpy(tmp, StrVar(arg1));
			int l = strwlen(tmp); if (pos > l) pos = l;
			pos = strwpos(tmp, pos);
			if (arg2 & 0xff00) {
				strcpy(tmp+pos+2, StrVar(arg1)+pos);
				tmp[pos]=arg2>>8; tmp[pos+1] = arg2;
			} else {
				strcpy(tmp+pos+1, StrVar(arg1)+pos);
				tmp[pos]=arg2;
			}
			strwfix(tmp, 63);
			SetStrVar(arg1, tmp);
			break;
			}
		case 0x30: // マクロ展開
			{
			char* str = (char*)decoder.Macro().DecodeMacro((unsigned char*)StrVar(arg1), (unsigned char*)tmp);
			kconv((unsigned char*)StrVar(arg1), (unsigned char*)ktemp1);
			kconv((unsigned char*)str, (unsigned char*)ktemp2);
			p(("cmd 0x59 - 0x30 : extract macro; Str [%d] = %s <- %s\n",arg1,ktemp1,ktemp2));
			SetStrVar(arg1, str);
			break;
			}
        }
	return;

}

/* for debug */
#ifdef p
#  undef p
#endif /* defined(p) */

#ifdef DEBUG_Select
#define p(X) printf X
#else
#define p(X)
#endif

void SENARIO_FLAGSDecode::DecodeSenario_Select(SENARIO_DECODE& decoder) {
	int cur_point = decoder.GetPoint()-1;
	unsigned char c = decoder.NextCharwithIncl();
	char buf[1024]; memset(buf,0,1024);
	int arg; int i;
	switch(c) {
		case 3:
		case 1: {
			decoder.BackLog().AddSelect2Start(cur_point, decoder.GetSeen());
			char* buf2 = buf;
			TextAttribute textlist[100];
			int list_deal = 0; int select_deal = 0; int window_height = 0;
			arg = decoder.ReadData();
			if (c == 1)
				p(("cmd 0x58-1 : arg %d\n",arg));
			else
				p(("cmd 0x58-1 : arg %d ;; not supported yet. use cmd 0x58 - 1.\n",arg));
			if (decoder.local_system.IsDumpMode()) p(("text: (選択肢)\n"));
			if (decoder.NextChar() == 0x22) decoder.NextCharwithIncl();
			if (decoder.NextChar() == 0) decoder.NextCharwithIncl();
			while(decoder.NextChar() != 0x23) {
				decoder.ReadStringWithFormat(textlist[list_deal]);
				textlist[list_deal].SetText( (char*)decoder.Macro().DecodeMacro((unsigned char*)textlist[list_deal].Text(), (unsigned char*) buf2));
				if (decoder.local_system.IsDumpMode()) {
					buf[511] = 0; 
					kconv( (unsigned char*)textlist[list_deal].Text(), (unsigned char*)(buf+512));
					p(("text: %s\n",buf+512));
				}
				if (textlist[list_deal].Condition()) {
					decoder.BackLog().AddSelect2(textlist[list_deal].Text());
					if (textlist[list_deal].Attribute() != 0x22 && textlist[list_deal].Attribute() != 0x21) select_deal++;
					window_height++;
				} else {
					textlist[list_deal].SetAttr(0x21,0); /* 描画しない */
				}
				list_deal++;
				if (list_deal >= 100) break;
			}
			decoder.NextCharwithIncl();
#ifndef SUPRESS_KEY
			int ret = 1;
			{
			/* 選択肢ウィンドウの幅を決める*/
			int win_width = 0;
			for (i=0; i<list_deal; i++) {
				/* 半角・全角を区別して文字数を数える */
				unsigned char* s = (unsigned char*)(textlist[i].Text());
				int slen = 0;
				while(*s) {
					if (s[0] > 0x80 && s[1] > 0x80) { s++;
					}
					slen++; s++;
				}
				if (win_width < slen) win_width = slen;
			}
			win_width += 1; /* １文字分はマージン */
			/* 選択の前準備 */
			decoder.Senario().RestoreGrp();
			/* text の座標を保存 */
			/* textの座標変更 */
			decoder.local_system.config->SetParam("#COM_MESSAGE_SIZE", 2, win_width, window_height);
			/* 選択 */
			decoder.local_system.SetClickEvent(AyuSys::NO_EVENT);
			decoder.local_system.SelectEvent();
			ret = decoder.local_system.SelectItem(textlist, list_deal, 2);
			decoder.local_system.SetClickEvent(AyuSys::END_TEXTFAST);
			/* text の座標を回復 */
			}
#else
			int ret = 1;
#endif
			if (ret > 0) {
				decoder.BackLog().AddSelect2Result(ret);
				SetVar(arg, ret);
			} else if (ret == -2) { /* 一つ前のテキストに戻る */
				decoder.local_system.SetBacklog(2);
			} else {
				// intterupt
				decoder.local_system.DeleteText();
				decoder.SetPoint(cur_point);
			}
			break;
		}
		case 2: { // selection
			char* buf2 = buf;
			TextAttribute textlist[10]; int list_deal = 0; int select_deal = 0;
			decoder.BackLog().AddSelect2Start(cur_point, decoder.GetSeen());
			arg = decoder.ReadData();
			p(("cmd 0x58-2 : arg %d\n",arg));
			if (decoder.local_system.IsDumpMode()) p(("text: (選択肢)\n"));

			if (decoder.NextChar() == 0x22) decoder.NextCharwithIncl();
			if (decoder.NextChar() == 0) decoder.NextCharwithIncl();
			while(decoder.NextChar() != 0x23) {
				decoder.ReadStringWithFormat(textlist[list_deal]);
				textlist[list_deal].SetText( (char*)decoder.Macro().DecodeMacro((unsigned char*)textlist[list_deal].Text(), (unsigned char*) buf2));
				if (decoder.local_system.IsDumpMode()) {
					buf[511] = 0; 
					kconv( (unsigned char*)textlist[list_deal].Text(), (unsigned char*)(buf+512));
					p(("text: %s\n",buf+512));
				}
				if (textlist[list_deal].Condition()) {
					decoder.BackLog().AddSelect2(textlist[list_deal].Text());
					if (textlist[list_deal].Attribute() != 0x22 && textlist[list_deal].Attribute() != 0x21) select_deal++;
				} else {
					textlist[list_deal].SetAttr(0x21,0); /* 描画しない */
				}
				list_deal++;
				if (list_deal >= 10) break;
			}
			decoder.NextCharwithIncl();
#ifndef SUPRESS_KEY
			int ret = 1;
#if 0
			if (select_deal > 1) {
#endif
			if (1) {
				decoder.Senario().RestoreGrp();
				decoder.local_system.SetClickEvent(AyuSys::NO_EVENT);
				decoder.local_system.SelectEvent();
				ret = decoder.local_system.SelectItem(textlist, list_deal,1);
				decoder.local_system.SetClickEvent(AyuSys::END_TEXTFAST);
			} else { /* 選択できる選択肢が１つ以下 */
				int i; for (i=0; i<list_deal; i++) {
					if (textlist[i].Attribute() != 0x22 && textlist[i].Attribute() != 0x21) {
						ret = i+1; break;
					}
				}
			}
#else
			int ret = 1;
#endif
			if (ret > 0) {
				decoder.BackLog().AddSelect2Result(ret);
				SetVar(arg, ret);
			} else if (ret == -2) {
				decoder.local_system.SetBacklog(2); /* 一つ前のテキストに戻る */
			} else {
				// intterupt
				decoder.local_system.DeleteText();
				decoder.SetPoint(cur_point);
			}
			break;
		}
		case 4: { // セーブデータのロードの窓を開く
			arg = decoder.ReadData();
			p(("cmd 0x58 - 4 : arg = %d , load save file? :  not supported yet.\n",arg));
			SetVar(arg, decoder.local_system.SelectLoadWindow());
			break;
		}
		case 5: { // ???
			arg = decoder.ReadData();
			pp(("cmd 0x58 - 5 : arg = %d ???? :  not supported yet.\n",arg));
			break;
		}
		default:
			p(("cmd 0x58-%d : not supported yet.",c));
			break;
	}
}

void SENARIO_FLAGSDecode::DecodeSenario_VargroupRead(SENARIO_DECODE& decoder) {
	int cmd = decoder.NextCharwithIncl();
	int index = decoder.ReadData();
	if (cmd == 1) { // set var
		p(("cmd 0x5b - 1 : read var group to var[%d]- : ",index));
		while(decoder.NextChar() != 0) {
			int data = decoder.ReadData();
			p(("%d , ",data));
			SetVar(index, data);
			index++;
		}
		p(("\n"));
		decoder.NextCharwithIncl();
	} else if (cmd == 2) { // set bit
		p(("cmd 0x5b - 2 : read var group to bit[%d]- : ",index));
		while(decoder.NextChar() != 0) {
			int data = decoder.ReadData();
			p(("%d , ",data));
			SetBit(index, data);
			index++;
		}
		p(("\n"));
		decoder.NextCharwithIncl();
	}
}

void SENARIO_FLAGSDecode::DecodeSenario_VargroupSet(SENARIO_DECODE& decoder) {
	int cmd = decoder.NextCharwithIncl();
	int index1 = decoder.ReadData(); int index2 = decoder.ReadData();
	int data = decoder.ReadData();
	if (index2 >= 2000) index2 = 1999;
	if (cmd == 1) { // set var
		p(("cmd 0x5c - 1 : set var group : from %d to %d , data = %d\n",index1,index2,data));
		int i; for (i=index1; i<=index2; i++) SetVar(i, data);
	} else if (cmd == 2) { // set bit
		p(("cmd 0x5c - 2 : set bit group : from %d to %d , data = %d\n",index1,index2,data));
		int i; for (i=index1; i<=index2; i++) SetBit(i, data);
	}
	return;
}

void SENARIO_FLAGSDecode::DecodeSenario_VargroupCopy(SENARIO_DECODE& decoder) {
	int cmd = decoder.NextCharwithIncl();
	int src = decoder.ReadData(); int dest = decoder.ReadData();
	int len = decoder.ReadData();
	if (cmd == 1) { // set var
		p(("cmd 0x5d - 1 : copy var group from var[%d] to var[%d] , length %d\n",src,dest,len));
		int i; for (i=0; i<len; i++) SetVar(dest++, GetVar(src++));
	} else if (cmd == 2) { // set bit
		p(("cmd 0x5d - 2 : copy bit group from bit[%d] to bit[%d] , length %d\n",src,dest,len));
		int i; for (i=0; i<len; i++) SetBit(dest++, GetBit(src++));
	}
}

/* for debug */
#ifdef p
#  undef p
#endif /* defined(p) */

#ifdef DEBUG_Condition
#define p(X) printf X
#else
#define p(X)
#endif

int SENARIO_FLAGSDecode::DecodeSenario_Condition(SENARIO_DECODE& decoder, TextAttribute& attr) {
	// 条件判定をする
	char stack_buf[1024]; // シナリオバッファ
	char opinfo_buf[1024]; // () の保存のためのバッファ
	char* stack = stack_buf;
	char* opinfo = opinfo_buf;
#define OPINFO_AND opinfo[0]
#define OPINFO_OR opinfo[1]
#define OPINFO_PUSH opinfo+=2
#define OPINFO_POP opinfo-=2
#define STACK_OR { stack--; stack[-1] |= stack[0]; }
#define STACK_AND { stack--; stack[-1] &= stack[0]; }
#define STACK_PUSH(x) { *stack++ = x; }
#define STACK_AND_IMM(x) { stack[-1] &= x; }
#define STACK_RESULT stack[-1]
#define STACK_ISERROR (stack != (stack_buf+1))
	const char and_char = 0x27; const char or_char = 0x26;
	OPINFO_AND = OPINFO_OR = 0;

	// まず、条件判定を読み込む
	p(( "Condition: " ));
	int parenth_depth = 0; // '(...)' の多重度
	int parse_error = 0; /* 1 になるとエラー、-1 なら終了 */
	// 終了条件は、'(...)' が閉じること。
	int c;
	while(! parse_error) {
		c = decoder.NextCharwithIncl();
	retry: /* 0x58 用 */
		if (c == 0xff) { /* ぴよっ */
			int arg1 = 0;
			if (decoder.local_system.Version() >= 3) {
				arg1 = decoder.ReadInt();
			}
			char buf[1024];
			char* str = decoder.ReadString(buf);
			if (decoder.local_system.IsDumpMode()) {
				kconv( (unsigned char*)str, (unsigned char*)(buf+512));
				char* s = buf+512;
				while(strchr(s,'\n')) {
					*(strchr(s,'\n')) = '\0';
					p(("text: %s\n",s));
					s += strlen(s)+1;
				}
				p(("0xff: %s\n",s));
			}
		} else if (c < 0x30) { /* (),and,or */
			if (c < 0x26 || c > 0x29) {parse_error = 1; continue;}
			switch(c) {
				case '(':
					OPINFO_PUSH; OPINFO_AND=0; OPINFO_OR=0;
					parenth_depth++;
					p(("("));
					break;
				case ')':
					/* 必要に応じて or, and を行う */
					if (parenth_depth==0) {parse_error=1; break; }
					if (OPINFO_OR) {
						STACK_OR;
					}
					OPINFO_POP;
					if (OPINFO_AND) {
						STACK_AND;
					}
					OPINFO_AND = 0;
					p((")"));
					parenth_depth--;
					if (parenth_depth == 0) { parse_error = -1; break; /* 終了 */ }
					break;
				case and_char:
					/* & の次に ( が来たら対応する ) がくるまで保留
					** 条件が来たら、前の条件と & する
					*/
					if (OPINFO_AND) {parse_error=1; break;}
					OPINFO_AND=1;
					p(("&&"));
					break;
				case or_char:
					/* | の前に | があったなら前の条件を or する。
					** その後、| か ) の時点で実際の or 処理は行う
					*/
					if(OPINFO_OR) {
						STACK_OR;
					}
					OPINFO_OR=1;
					p(("||"));
					break;
			}
		} else if (c < 0x58) { // 条件判定の、条件
			if (c < 0x36 || c > 0x55) { parse_error = 1; continue; }
			int arg1 = decoder.ReadData(); int arg2 = decoder.ReadData();
			// 前処理
			if (c <= 0x37) {
					p(("arg1: Bit[%d]",arg1));
				if (GetBit(arg1)) arg1 = 1;
				else arg1 = 0;
				c += 4;
			} else if (c <= 0x39) {
					p(("arg1: Bit[%d]",arg1));
				if (GetBit(arg1)) arg1 = 1;
				else arg1 = 0;
					p(("arg2: Bit[%d]",arg2));
				if (GetBit(arg2)) arg2 = 1;
				else arg2 = 0;
				c += 2;
			} else if (c <= 0x47) {
					p(("arg1: Var[%d]",arg1));
				arg1 = GetVar(arg1);
			} else if (c <= 0x55) {
					p(("arg1: Var[%d]",arg1));
					p(("arg2: Var[%d]",arg2));
				arg1 = GetVar(arg1);
				arg2 = GetVar(arg2);
				c -= 0x0e;
			}
			char ret = 1; // とりあえず true を入れておく
			switch(c) {
				case 0x3a: // case 0x36: case 0x38: case 0x48:
					p(("cmd:arg1:%d ==arg2:%d.",arg1,arg2));
					if (arg1 != arg2) ret = 0;
					break;
				case 0x3b: // case 0x37: case 0x39: case 0x49:
					p(("cmd:arg1:%d != arg2:%d.",arg1,arg2));
					if (arg1 == arg2) ret = 0;
					break;
				case 0x41: case 0x42: // case 0x4f: case 0x50:
					p(("cmd:arg1:%d & arg2:%d.",arg1,arg2));
					if (arg1 & arg2) ret = 0;
					break;
				case 0x43: // case 0x51:
					p(("cmd:arg1:%d, arg2:%d.",arg1,arg2));
					if (arg1 ^ arg2) ret = 0;
					break;
				case 0x44: // case 0x52:
					p(("cmd:arg1:%d<=arg2:%d.",arg1,arg2));
					if (arg1 > arg2) ret = 0;
					break;
				case 0x45: // case 0x53:
					p(("cmd:arg1:%d>=arg2:%d.",arg1,arg2));
					if (arg1 < arg2) ret = 0;
					break;
				case 0x46: // case 0x54:
					p(("cmd:arg1:%d<arg2:%d.",arg1,arg2));
					if (arg1 >= arg2) ret = 0;
					break;
				case 0x47: // case 0x55:
					p(("cmd:arg1:%d>arg2:%d.",arg1,arg2));
					if (arg1 <= arg2) ret = 0;
					break;
			}
			p(( ";%s ", (ret == 1 ? "true" : "false") ));
			/* 前が and ならその処理、or 節の最初なら stack に結果を代入 */
			if (OPINFO_AND) {
				OPINFO_AND = 0;
				STACK_AND_IMM(ret);
			} else {
				STACK_PUSH(ret);
			}
		} else if (c == 0x58) {
			/* 選択肢文字列のcase条件。前の節の結果が true なら
			** attribute セット。まだ条件が続くなら or 節としてあつかう
			** (or 節のはずだが、条件分岐が逆なので and になっている)
			*/
			int arg1 = decoder.NextCharwithIncl();
			int arg2 = (arg1 != 0x21) ? decoder.ReadData() : 0;
			p(("(0x58 - %x, %d)",arg1,arg2));
			if (! STACK_RESULT) {
				attr.SetAttr(arg1, arg2);
			}
			if (decoder.NextChar() == '(') { c = and_char; goto retry; }
		} else parse_error = 1;
	}
	if (parse_error == 1 || STACK_ISERROR ) {
		fprintf(stderr, "Warning: Unsupported condition\n");
		decoder.DumpData();
	}
	return STACK_RESULT;
}

void SENARIO_FLAGSDecode::DecodeSkip_CalcVar(SENARIO_DECODE& decoder) {
	unsigned char cmd = decoder.Cmd();
	decoder.ReadData();
	if (cmd != 0x56) { // 0x56 : 引数が１つ
		decoder.ReadData();
	}
	if (cmd == 0x57) { // 0x57: 引数が３つ
		decoder.ReadData();
	}
	return;
}

void SENARIO_FLAGSDecode::DecodeSkip_CalcStr(SENARIO_DECODE& decoder) {
	unsigned char c = decoder.NextCharwithIncl();
	char tmp[1024];
	if (c < 1 || c > 0x40) {
		return;
	}
	decoder.ReadData();
	switch(c) {
		case 1: // 代入
			decoder.ReadString(tmp); break;
		case 2: // strlen
			decoder.ReadData(); break;
		case 3: // strcmp
			decoder.ReadData(); decoder.ReadData(); break;
		case 4: // strcat
			decoder.ReadData(); break;
		case 5: // strcpy
			decoder.ReadData(); break;
		case 6: // itoa
			decoder.ReadData(); decoder.ReadData(); break;
		case 7: // han2zen
			break;
		case 8: // atoi
			decoder.ReadData(); break;
		case 0x10:
		case 0x11:
		case 0x20:
		case 0x21:
		case 0x12:
		case 0x13:
		case 0x22:
		case 0x23:
		case 0x19:
		case 0x29:
			decoder.ReadData(); decoder.ReadData(); break;
		case 0x14:
		case 0x24:
			decoder.ReadData(); decoder.ReadData(); decoder.ReadData(); break;
		case 0x15:
		case 0x16:
		case 0x25:
		case 0x26:
		case 0x17:
		case 0x18:
		case 0x27:
		case 0x28:
			decoder.ReadData(); break;
		case 0x30: break;
        }
	return;

}

void SENARIO_FLAGSDecode::DecodeSkip_Select(SENARIO_DECODE& decoder) {
	unsigned char c = decoder.NextCharwithIncl();
	switch(c) {
		case 1: 
		case 2: 
		case 3: { // selection
			decoder.ReadData();
			if (decoder.NextChar() == 0x22) decoder.NextCharwithIncl();
			if (decoder.NextChar() == 0) decoder.NextCharwithIncl();
			while(decoder.NextChar() != 0x23) {
				TextAttribute text;
				decoder.ReadStringWithFormat(text,0);
			}
			decoder.NextCharwithIncl();
			break;
		}
		case 4: { // ???
			decoder.ReadData();
			break;
		}
		default:
			break;
	}
}

void SENARIO_FLAGSDecode::DecodeSkip_VargroupRead(SENARIO_DECODE& decoder) {
	decoder.NextCharwithIncl();
	decoder.ReadData();
	while(decoder.NextChar() != 0) {
		decoder.ReadData();
	}
	decoder.NextCharwithIncl();
}

void SENARIO_FLAGSDecode::DecodeSkip_VargroupSet(SENARIO_DECODE& decoder) {
	decoder.NextCharwithIncl();
	decoder.ReadData();
	decoder.ReadData();
	decoder.ReadData();
	return;
}

void SENARIO_FLAGSDecode::DecodeSkip_VargroupCopy(SENARIO_DECODE& decoder) {
	decoder.NextCharwithIncl();
	decoder.ReadData();
	decoder.ReadData();
	decoder.ReadData();
}

int SENARIO_FLAGSDecode::DecodeSkip_Condition(SENARIO_DECODE& decoder) {
	int parenth_count = 0; // '(...)' の多重度
	// 終了条件は、'(...)' が閉じること。
	while(1) {
		int c = decoder.NextCharwithIncl();
		// '(..)' の処理
		if (c == '(') {
			parenth_count++;
		} else if (c == ')') {
			if (parenth_count == 0 || parenth_count == 1) break; // ')' が来たら終了
			parenth_count--;
		// その他の条件の処理
		// } else if (c == and_char || c == or_char) { // 条件判定の and , or 演算子
		} else if (c >= 0x36 && c <= 0x55) { // 条件判定の、条件
			decoder.ReadData(); decoder.ReadData();
		}
	}
	return 0;
}
