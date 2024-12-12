#include<stdio.h>
#include<string.h>

#include "senario_backlog.h"

/* バックログの構造
**	           Pushで次にデータが入る場所    log_orig+LOG_LEN
**	           |データの先頭(log)         データの末尾|
**	log_orig   ||                                    ||
**	v          vv                                    vv
**	|----------|**************************************|
**	(top)                                       (bottom)
**
**  データ内部：（メモリ上では逆順に push していく）
**
** 	BL_END / BL_END2 / BL_RET / BL_SEL2R: 1byte で 1命令
**	BL_TEXT / BL_SEL2S:
**		BL_TEXT <senario_no/short> <senario_point/int> BL_TEXT の１０バイト
**	BL_TEXT_WI / BL_SEL2S_WI: (with information)
**		BL_TEXT_WI <senario_no/short> <senario_point/int> <flag info/int>
**			<graphcis info/int> <music info/int> <stack info/int> <BL_TEXT_WI>
**			合計２４バイト
**	BL_XXX_INF: information などを格納。別の場所から参照される。
**		BL_XXX_INF <len> ... <len> BL_OTHER
**	BL_MSGPOS / BL_MSGSIZ / BL_MOJSIZ :
**		BL_XXX <x> <y> <old-x> <old-y> BL_XXX
**	BL_ISWAKU:
**		BL_ISWAKU <1byte data> <old data> BL_ISWAKU
**	BL_SEL2 / BL_TITLE :
**		BL_SEL2 <len/short> <text> <len/short> BL_SEL2
**	これにより、逆からも読むことができる backlog となる。
*/


// assume little endian...

inline int read_little_endian_int(char* buf) {
	return *(int*)buf;
}

inline int read_little_endian_short(char* buf) {
	return *(short*)buf;
}

inline int write_little_endian_int(char* buf, int number) {
	int c = *(int*)buf; *(int*)buf = number; return c;
}

inline int write_little_endian_short(char* buf, int number) {
	int c = *(short*)buf; *(short*)buf = number; return c;
}
static int savebuflen[0x100] = {
//0   1   2   3   4   5   6   7   8  **   9   A   B   C   D   E   F
 16,  0,  2,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +00
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +10
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +20
  0,  0,  6,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +30
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  9,  0,  0,  0, // +40
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +50
  0,  0,  0,  0,  9,  0,  9,  0,  9,/**/  0,  8,  0,  0,  0,  0,  0, // +60
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +70
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +80
  0,  0,  0,  0,  0,  0, 35, 35,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +90
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +A0
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +B0
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +C0
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +D0
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0, // +E0
  0,  0,  0,  0,  0,  0,  0,  0,  0,/**/  0,  0,  0,  0,  0,  0,  0};// +F0

void DumpGrp(char* log, int gpoint, int lpoint) {
	int args[100];
	log = log+lpoint-gpoint-1;
	int cmd = *log++;
	int glen = read_little_endian_short(log); log+=2; glen-=2+4+4;
	int old_p = read_little_endian_short(log); log+=4;
	char* buf = log;

	/* grp 読み込み */
	int deal = *log++;
	int i; for (i=0; i<deal; i++) {
		int cmd = int(*log)&0xff;
		int sz = savebuflen[cmd];
		printf("\t\t%d : cmd %2x, ",i,cmd);
		log++;
		memcpy(args,log,sz*4); log += sz*4;
		int j; for (j=0; j<sz; j++) { printf("%d,",args[j]);}
		if (cmd < 0x20) {
			printf("file %s\n",log);
			log += strlen(log)+1;
		} else if (cmd > 0x90) {
			char* buf = log;
			printf("file ");
			int j; for (j=0; j<args[32]; j++) {
				printf("'%s',",buf);
				buf += strlen(buf)+1;
			}
			printf("\n");
			log += args[sz-1];
		} else {
			printf("\n");
		}
	}
}

void DumpStack(char* log) {
	log++;
	int len = *(short*)log; log+=2;
	int sl = *(int*)log; log += 4;
	int i;
	printf("deal %d\n",sl);
	for (i=0; i<sl; i++) {
		int l = *(int*)log; log+=4;
		int s = *(int*)log; log+=4;
		printf("seen %d,local %d\n",s,l);
	}
	sl = *(int*)log; log += 4;
	printf("deal %d\n",sl);
	for (i=0; i<sl; i++) {
		int l = *(int*)log; log+=4;
		int s = *(int*)log; log+=4;
		printf("seen %d,local %d\n",s,l);
	}
}

char* DumpHead(char* log, int* ret_len, int* point) {
	int grpinfo[23];
	int log_point = read_little_endian_int(log); log+=4;
	memcpy(grpinfo, log, sizeof(grpinfo)); log += sizeof(grpinfo);
	int len = read_little_endian_int(log); log += 4;
	printf("point %d, length %d\n",log_point, len);
	printf("old grp point %d, grp info len %d\n",grpinfo[0],grpinfo[1]);
	int i;for (i=0;i<10;i++) {
		printf("grp info %d = hash %x, info %d\n",
			i,grpinfo[12+i],grpinfo[2+i]);
		if (i < grpinfo[1])
			DumpGrp(log,grpinfo[2+i],log_point);
	}
	if (ret_len != NULL) *ret_len = len;
	if (point != NULL) *point = log_point;
	return log;
}


void DumpType(char* log, int len, int point) {
	char* log_end = log+len;
	while(log < log_end) {
		int cmd = *log;
		if (cmd > BL_MAX || cmd < 0) {
			printf("INVALID COMMAND\n");
			log++; point--;
			continue;
		}
		int clen = bl_len[cmd];
		if (clen == -1) clen = read_little_endian_short(log+1);
		if (clen == 0) clen = 1;
		if (cmd != BL_TEXT && cmd != BL_SEL2S) {
			printf("%5d: cmd %2d(%8s), len %d\n",point,cmd,bl_name[cmd],clen);
		} else {
			printf("%5d: cmd %2d(%8s), len %d, seen %d, point %d\n",point,cmd,bl_name[cmd],clen,*(short*)(log+5),*(int*)(log+1));
		}
		if (cmd == BL_STACK_INF) DumpStack(log);
		log += clen;
		point -= clen;
	}
}

int main(int argc, char* argv[]) {
	if (argc != 2) return 0;
	FILE* f = fopen(argv[1], "rb");
	if (f == 0) return 0;
	char* buf; int len;
	fseek(f,0,2); len=ftell(f); fseek(f,0,0);
	buf=new char[len];
	fread(buf, len, 1, f);
	fclose(f);
	int point;
	char* log = DumpHead(buf, &len, &point);
	DumpType(log, len, point);
	delete[] buf;
	return 0;
}
