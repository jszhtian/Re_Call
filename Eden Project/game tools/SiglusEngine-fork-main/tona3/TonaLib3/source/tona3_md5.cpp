#include	"tona3_pch.h"
#include	"tona3_md5.h"



#define		F(x,y,z)		( (x & y) | (~x & z) )	// ( (X and Y) or (not(X) and Y) )
#define		G(x,y,z)		( (x & z) | (y & ~z) )	// ( (X and Z) or (Y and not(Z)) )
#define		H(x,y,z)		( x ^ y ^ z          )	// ( X xor Y xor Z               )
#define		I(x,y,z)		( y ^ (x | ~z)       )	// ( Y xor (X or not(Z))         )

#define		FF(a,b,c,d,k,s,i){ \
				a += (F(b,c,d) + k + i); \
				__asm mov eax, a \
				__asm rol eax, s \
				__asm add eax, b \
				__asm mov a, eax \
			}

#define		GG(a,b,c,d,k,s,i){ \
				a += (G(b,c,d) + k + i); \
				__asm mov eax, a \
				__asm rol eax, s \
				__asm add eax, b \
				__asm mov a, eax \
			}

#define		HH(a,b,c,d,k,s,i){ \
				a += (H(b,c,d) + k + i); \
				__asm mov eax, a \
				__asm rol eax, s \
				__asm add eax, b \
				__asm mov a, eax \
			}

#define		II(a,b,c,d,k,s,i){ \
				a += (I(b,c,d) + k + i); \
				__asm mov eax, a \
				__asm rol eax, s \
				__asm add eax, b \
				__asm mov a, eax \
			}

/*=============================================================================
	�l�c�T ������
void MD5init(MD5_STRUCT *md5st, UI total_data_cnt);
		IN		�Ȃ�
		OUT		�Ȃ�
-----------------------------------------------------------------------------*/
void MD5init(MD5_STRUCT *md5st, UINT total_data_cnt){
	int alpha, add_data_cnt;
	UINT data_bit_cnt;
	BYTE *sp;

	//������������������������������������������������������������������������������������������
	// ��Step 1. �p�f�B���O�r�b�g�̕t��

	ZeroMemory(md5st->add_data, MD5_ADD_DATA_MAX);
	md5st->add_data[0] = 0x80;

	//������������������������������������������������������������������������������������������
	// ��Step 2. �����̕t��

	// ���b�Z�[�W���� 64 �o�C�g�̐����{�ɂ��邽�߁A���̃��b�Z�[�W�ɂ܂� 0x80�������A
	// ���ꂩ�烁�b�Z�[�W���� (64 �̔{�� - 8) �o�C�g�ɂȂ�悤�ɁA0x00 �������Ă����܂��B
	// ���̌��ʂɁA(�p�f�B���O����O�̃��b�Z�[�W��) * 8 �� 16 �i����t�����܂��B
	// ���̎��A���� 4 �o�C�g���ɕt������Ƃ������ɒ��ӂ��܂��B

	alpha = ((total_data_cnt + 1) & 0x03f);	// [+1 == 0x80�̕�] [&0x03f == %64]
	if(alpha <= 56){
		add_data_cnt = 1 + (56 - alpha) + 8;
	}
	else{
		add_data_cnt = 1 + (56 + (64 - alpha)) + 8;
	}

	data_bit_cnt = total_data_cnt << 3;		// [<<3 == *8]
	sp = (BYTE *)&data_bit_cnt;
	md5st->add_data[add_data_cnt - 8] = *(sp + 0);
	md5st->add_data[add_data_cnt - 7] = *(sp + 1);
	md5st->add_data[add_data_cnt - 6] = *(sp + 2);
	md5st->add_data[add_data_cnt - 5] = *(sp + 3);
	// add_data[add_data_cnt - 4] = 0;
	// add_data[add_data_cnt - 3] = 0;
	// add_data[add_data_cnt - 2] = 0;
	// add_data[add_data_cnt - 1] = 0;
	// add_data[add_data_cnt - 0] = 0xaa;

	//������������������������������������������������������������������������������������������
	// ��Step 3. MD �o�b�t�@�̏�����

	// ���b�Z�[�W�_�C�W�F�X�g���v�Z���邽�߂� 4 �̃��[�h�o�b�t�@ (A, B, C, D) ���g�p�����B
	// �����ŁAA, B, C, D ���ꂼ�� 32 �r�b�g�̃��W�X�^�ł���B
	// �����̃��W�X�^�́A�ȉ��� 16 �i�̒l�ŁA���ʃo�C�g���珇�ɏ����������B
	md5st->code[0] = 0x67452301;
	md5st->code[1] = 0xefcdab89;
	md5st->code[2] = 0x98badcfe;
	md5st->code[3] = 0x10325476;

	//������������������������������������������������������������������������������������������
	// ��Step 4. 16 ���[�h�u���b�N���̃��b�Z�[�W����

	// �܂��AF, G, H, I �Ƃ����S�̕⏕�֐��ƁAT[i] = int(4294967296 * abs(sin(i))) ���`���܂��B
	// �A���Ai �̒P�ʂ̓��W�A���ł��B

	// ���̃X�e�b�v�ł́Asin �֐�����\�������U�S�̗v�f�����e�[�u�� T[1 ... 64] ���g�p����B
	// T[i] �̓e�[�u���� i �Ԗڂ̗v�f�Ƃ��A4294967296 �� abs(sin(i)) �̐ς̐������ɓ��������̂Ƃ���B
	// �����ŁAi �̒P�ʂ̓��W�A���ł���B
	// ���l�ŏ�������̂Ńe�[�u���͕s�v
	//DWORD T[64+1];
	//double dtmp;
	//for(i=1; i<=64; i++){		// 1 ����
	//	dtmp = sin(i);
	//	if(dtmp < 0){dtmp = -dtmp;}
	//	T[i] = (int)(4294967296 * dtmp);	// �}�C�i�X�l�����肦�邯�ǁA����ŗǂ��̂��ȁH
	//}

	// ��������A��Step 2. �ō�����p�f�B���O�σ��b�Z�[�W���S�o�C�g�P�ʂłP�U�i�v�U�S�o�C�g�j���������Ă����܂��B
	md5st->loop_cnt = (total_data_cnt + add_data_cnt) >> 6;		// [>>6 == /64]
	md5st->nokori   = total_data_cnt;
}



/*=============================================================================
	�l�c�T ����
int MD5funk(MD5_STRUCT *md5st, UC *data, UI data_cnt);
		IN		data_cnt : �f�[�^�̑���������ꍇ�͂U�S�o�C�g�̔{���P�ʂő��荞�ގ�
		OUT		ret : 0=���� 1=�I�� 2=�d�q�q
-----------------------------------------------------------------------------*/
int MD5funk(MD5_STRUCT *md5st, BYTE *data, UINT data_cnt){
	DWORD A, B, C, D, AA, BB, CC, DD;
	UINT loop_cnt, nokori;
	DWORD X[16];
	BYTE *sp;
	int ret;

	ret = 0;

	// �p�����[�^����
	A = md5st->code[0];
	B = md5st->code[1];
	C = md5st->code[2];
	D = md5st->code[3];

	loop_cnt = md5st->loop_cnt;
	nokori   = md5st->nokori;

	sp = data;

	// ��������A��Step 2. �ō�����p�f�B���O�σ��b�Z�[�W���S�o�C�g�P�ʂłP�U�i�v�U�S�o�C�g�j���������Ă����܂��B
	while(1){

		// ��1.�܂��AX[16] �Ƃ����e�[�u���ɂS�o�C�g���i�[���Ă����܂��B
		//   �����ł��A���ʃo�C�g���Ɋi�[����̂Œ��ӂ��ĉ������B
		//   abc �̗�Ō����΁AX[0] = 0x80636261 �ƂȂ�܂��B
		//      a        b        c        1 |  448�r�b�g�ڂ܂Ŗ��߂�     |       24        0
		// bit: 01010001 01010010 01010011 1 0000000 0000000  ..  00000000 00011000 00000000
		// hex:    6   1    6   2    6   3     8   0    0  0  00     0   0    1   8    0   0
		//      61 62 63 80 00 00 .. 18 00 00 00
		// �u���b�N i �� X �ɃR�s�[
		if(nokori >= 64){
			if(data_cnt < 64){ret=2; break;}		// err
			CopyMemory((BYTE *)X, sp, 64);
			sp += 64;
			nokori -= 64;
			data_cnt -= 64;
		}
		else if(nokori > 0){
			// �R�R��ʂ�ƏI���̂͂�
			if(data_cnt < nokori){ret=2; break;}	// err
			CopyMemory((BYTE *)X, sp, nokori);
			CopyMemory((BYTE *)X+nokori, md5st->add_data, (64 - nokori));
			nokori = 0;
			data_cnt = 0;
		}
		else{
			// �R�R��ʂ�ƏI���̂͂�
			if(data_cnt != 0){ret=2; break;}		// err
			CopyMemory((BYTE *)&X[0], md5st->add_data, 64);
		}

		// ��2.���� (A, B, C, D) �ɓ����Ă�����̂� (AA, BB, CC, DD) �ɃR�s�[���Ă����܂��B
		// A �� AA�AB �� BB�AC �� CC�AD �� DD �Ƃ��ĕۑ�
		AA=A; BB=B; CC=C; DD=D;

		// ��3.FF(a,b,c,d,k,s,i) �� a = b + ((a + F(b,c,d) + X[k] + T[i]) <<< s) �Ƃ��āA���� 16 �̏��������s���܂��B
		FF(A,B,C,D,X[ 0], 7,0xd76aa478) FF(D,A,B,C,X[ 1],12,0xe8c7b756) FF(C,D,A,B,X[ 2],17,0x242070db) FF(B,C,D,A,X[ 3],22,0xc1bdceee)
		FF(A,B,C,D,X[ 4], 7,0xf57c0faf) FF(D,A,B,C,X[ 5],12,0x4787c62a) FF(C,D,A,B,X[ 6],17,0xa8304613) FF(B,C,D,A,X[ 7],22,0xfd469501)
		FF(A,B,C,D,X[ 8], 7,0x698098d8) FF(D,A,B,C,X[ 9],12,0x8b44f7af) FF(C,D,A,B,X[10],17,0xffff5bb1) FF(B,C,D,A,X[11],22,0x895cd7be)
		FF(A,B,C,D,X[12], 7,0x6b901122) FF(D,A,B,C,X[13],12,0xfd987193) FF(C,D,A,B,X[14],17,0xa679438e) FF(B,C,D,A,X[15],22,0x49b40821)

		// ��4.GG(a,b,c,d,k,s,i) �� a = b + ((a + G(b,c,d) + X[k] + T[i]) <<< s) �Ƃ��āA���� 16 �̏��������s���܂��B
		GG(A,B,C,D,X[ 1], 5,0xf61e2562) GG(D,A,B,C,X[ 6], 9,0xc040b340) GG(C,D,A,B,X[11],14,0x265e5a51) GG(B,C,D,A,X[ 0],20,0xe9b6c7aa)
		GG(A,B,C,D,X[ 5], 5,0xd62f105d) GG(D,A,B,C,X[10], 9,0x02441453) GG(C,D,A,B,X[15],14,0xd8a1e681) GG(B,C,D,A,X[ 4],20,0xe7d3fbc8)
		GG(A,B,C,D,X[ 9], 5,0x21e1cde6) GG(D,A,B,C,X[14], 9,0xc33707d6) GG(C,D,A,B,X[ 3],14,0xf4d50d87) GG(B,C,D,A,X[ 8],20,0x455a14ed)
		GG(A,B,C,D,X[13], 5,0xa9e3e905) GG(D,A,B,C,X[ 2], 9,0xfcefa3f8) GG(C,D,A,B,X[ 7],14,0x676f02d9) GG(B,C,D,A,X[12],20,0x8d2a4c8a)

		// ��5.HH(a,b,c,d,k,s,i) �� a = b + ((a + H(b,c,d) + X[k] + T[i]) <<< s) �Ƃ��āA���� 16 �̏��������s���܂��B
		HH(A,B,C,D,X[ 5], 4,0xfffa3942) HH(D,A,B,C,X[ 8],11,0x8771f681) HH(C,D,A,B,X[11],16,0x6d9d6122) HH(B,C,D,A,X[14],23,0xfde5380c)
		HH(A,B,C,D,X[ 1], 4,0xa4beea44) HH(D,A,B,C,X[ 4],11,0x4bdecfa9) HH(C,D,A,B,X[ 7],16,0xf6bb4b60) HH(B,C,D,A,X[10],23,0xbebfbc70)
		HH(A,B,C,D,X[13], 4,0x289b7ec6) HH(D,A,B,C,X[ 0],11,0xeaa127fa) HH(C,D,A,B,X[ 3],16,0xd4ef3085) HH(B,C,D,A,X[ 6],23, 0x4881d05)
		HH(A,B,C,D,X[ 9], 4,0xd9d4d039) HH(D,A,B,C,X[12],11,0xe6db99e5) HH(C,D,A,B,X[15],16,0x1fa27cf8) HH(B,C,D,A,X[ 2],23,0xc4ac5665)

		// ��6.II(a,b,c,d,k,s,i) �� a = b + ((a + I(b,c,d) + X[k] + T[i]) <<< s) �Ƃ��āA���� 16 �̏��������s���܂��B
		II(A,B,C,D,X[ 0], 6,0xf4292244) II(D,A,B,C,X[ 7],10,0x432aff97) II(C,D,A,B,X[14],15,0xab9423a7) II(B,C,D,A,X[ 5],21,0xfc93a039)
		II(A,B,C,D,X[12], 6,0x655b59c3) II(D,A,B,C,X[ 3],10,0x8f0ccc92) II(C,D,A,B,X[10],15,0xffeff47d) II(B,C,D,A,X[ 1],21,0x85845dd1)
		II(A,B,C,D,X[ 8], 6,0x6fa87e4f) II(D,A,B,C,X[15],10,0xfe2ce6e0) II(C,D,A,B,X[ 6],15,0xa3014314) II(B,C,D,A,X[13],21,0x4e0811a1)
		II(A,B,C,D,X[ 4], 6,0xf7537e82) II(D,A,B,C,X[11],10,0xbd3af235) II(C,D,A,B,X[ 2],15,0x2ad7d2bb) II(B,C,D,A,X[ 9],21,0xeb86d391)

		// ��7.�2.��ŃR�s�[���Ă����� (AA, BB, CC, DD) ���A(A, B, C, D) �ɉ����܂��B
		// �ȉ��̉��Z�����s���� (���̃u���b�N���J�n�����O�ɕێ����Ă����l�ɁA�S�̃��W�X�^�����ꂼ����Z)
		A+=AA; B+=BB; C+=CC; D+=DD;

		if(data_cnt == 0){break;}						// �����܂�

		loop_cnt--; if(loop_cnt == 0){ret=1; break;}	// �����ł�
	}

	//������������������������������������������������������������������������������������������
	// ��Step 5. �o��
	// Step 4. �Ōv�Z���� MD ���o�͂��܂��B
	// �o�͂Ƃ��Đ�������郁�b�Z�[�W�_�C�W�F�X�g�́AA, B, C, D �ł���B
	// ���Ȃ킿�AA �̉��ʃo�C�g����n�܂�AD �̍��ʃo�C�g�ɂďI���B
	// MD �́AA, B, C, D ����ׂ� 16 �o�C�g�̃f�[�^�ƂȂ�܂����A
	// Step 4. ���I��������_�ł� A, B, C, D �͈ȉ��̂悤�ɂȂ��Ă��܂��B
	// A = 0x98500190
	// B = 0xb04fd23c
	// C = 0x7d3f96c6
	// D = 0x727fe128
	// �����ł��e�f�[�^�͉��ʃo�C�g���珇�ɕ��ׂ�Ƃ������ɒ��ӂ���΁A���߂� MD �͎��̂悤�ɂȂ�܂��B
	// MD5 ("abc") = 900150983cd24fb0d6963f7d28e17f72
	// �p�����[�^�L��
	md5st->code[0] = A;
	md5st->code[1] = B;
	md5st->code[2] = C;
	md5st->code[3] = D;

	md5st->loop_cnt = loop_cnt;
	md5st->nokori   = nokori;

	return(ret);
}
