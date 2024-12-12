#include	"pch.h"



#define			CLIP_UPPER		0
#define			CLIP_LOWER		1
#define			CLIP_LEFT		2
#define			CLIP_RIGHT		3



/*========================================================================================
	�N���b�s���O
		����P�����@<=�@��R����
		����Q�����A��S�����͑召�֌W����
		�������͎�舵���Ȃ��i���̃��[�`���ł͎��O�ɉ��ʂ𗚂����Ă���j
		�������̖��O�͂��܂�Ӗ����Ȃ��i "UI x1" ���P�_�ڂ̂w���W�Ƃ͌���Ȃ��j
UI _fclip(UI x1, UI y1, UI x2, UI y2, UI clip);
		IN		UI x1 - y2 : 
				UI clip : �N���b�v���W
		OUT		UI ��������̍��W
----------------------------------------------------------------------------------------*/
UI _fclip(UI x1, UI y1, UI x2, UI y2, UI clip){

	_asm{

		push	esi
		push	edi

		mov		eax, x1
		mov		ebx, x2
		mov		ecx, y1
		mov		edx, y2

		mov		esi, eax
		add		esi, ebx
		rcr		esi, 1			/* �L�����[���܂ރV�t�g */
		mov		edi, ecx
		add		edi, edx
		rcr		edi, 1			/* �L�����[���܂ރV�t�g */

	cliping_loop:
		cmp		esi, clip
		je		cliping_end
		ja		cliping_step1

/* ���_���N���b�v���W��菬���� */
		mov		eax, esi		/* ���������W�̕���ウ�� */
		add		esi, ebx
		rcr		esi, 1			/* �L�����[���܂ރV�t�g */
		cmp		esi, eax
		je		cliping_end
		mov		ecx, edi
		add		edi, edx
		rcr		edi, 1			/* �L�����[���܂ރV�t�g */
		jmp		short cliping_loop

/* ���_���N���b�v���W���傫�� */
	cliping_step1:
		mov		ebx, esi		/* ���������W�̕���ウ�� */
		add		esi, eax
		rcr		esi, 1			/* �L�����[���܂ރV�t�g */
		cmp		esi, ebx
		je		cliping_end
		mov		edx, edi
		add		edi, ecx
		rcr		edi, 1			/* �L�����[���܂ރV�t�g */
		jmp		short cliping_loop

	cliping_end:

		mov		x1, edi

		pop		edi
		pop		esi
	}

	return(x1);

}



/*========================================================================================
	�N���b�s���O
int fclip(int *x1, int *y1, int *x2, int *y2, int ex1, int ey1, int ex2, int ey2);
		IN		int *x1 - *y2 : ���W�ւ̃|�C���^�[
		IN		int ex1 - ey2 : �N���b�v�͈�
		OUT		0=�s��
				1=�N���b�v����܂���
----------------------------------------------------------------------------------------*/
int fclip(int *x1, int *y1, int *x2, int *y2, int ex1, int ey1, int ex2, int ey2){
	UI gex1, gey1, gex2, gey2;	// ���ʃG���A
	UI tx1, ty1, tx2, ty2;		// �e���|���W
	UI gx1, gy1, gx2, gy2;		// ���ʍ��W
	int clip_coad1[4];
	int clip_coad2[4];
	int i;

	UI tmp;

	gex1 = ex1 + 0x80000000;
	gey1 = ey1 + 0x80000000;
	gex2 = ex2 + 0x80000000;
	gey2 = ey2 + 0x80000000;

	for(i=0; i<4; i++){
		clip_coad1[i] = 0;
		clip_coad2[i] = 0;
	}

/* �N���b�s���O�R�[�h�쐬 */
	if		(*x1 < ex1){clip_coad1[CLIP_LEFT]  = 1;}
	else if	(*x1 > ex2){clip_coad1[CLIP_RIGHT] = 1;}
	if		(*y1 < ey1){clip_coad1[CLIP_UPPER] = 1;}
	else if	(*y1 > ey2){clip_coad1[CLIP_LOWER] = 1;}
	if		(*x2 < ex1){clip_coad2[CLIP_LEFT]  = 1;}
	else if	(*x2 > ex2){clip_coad2[CLIP_RIGHT] = 1;}
	if		(*y2 < ey1){clip_coad2[CLIP_UPPER] = 1;}
	else if	(*y2 > ey2){clip_coad2[CLIP_LOWER] = 1;}

/* ���S�s������i���ꑤ�ɓ_�����݂���j*/
	for(i=0; i<4; i++){
		if(clip_coad1[i] && clip_coad2[i]){
			return(0);
		}
	}

/* ���ʂ𗚂����� */
	gx1 = tx1 = *x1 + 0x80000000;
	gy1 = ty1 = *y1 + 0x80000000;
	gx2 = tx2 = *x2 + 0x80000000;
	gy2 = ty2 = *y2 + 0x80000000;


/* �P�_�ډ����� */
	if(clip_coad1[CLIP_LEFT] || clip_coad1[CLIP_RIGHT] || clip_coad1[CLIP_UPPER] || clip_coad1[CLIP_LOWER]){
		while(1){
			if(clip_coad1[CLIP_LEFT]){
					tmp = _fclip(gx1, gy1, gx2, gy2, gex1);
					if(tmp>=gey1 && tmp<=gey2){
						tx1=gex1; ty1=tmp; break;
					}
			}
			else if(clip_coad1[CLIP_RIGHT]){
					tmp = _fclip(gx2, gy2, gx1, gy1, gex2);
					if(tmp>=gey1 && tmp<=gey2){
						tx1=gex2; ty1=tmp; break;
					}
			}
			if(clip_coad1[CLIP_UPPER]){
					tmp = _fclip(gy1, gx1, gy2, gx2, gey1);
					if(tmp>=gex1 && tmp<=gex2){
						tx1=tmp; ty1=gey1; break;
					}
			}
			else if(clip_coad1[CLIP_LOWER]){
					tmp = _fclip(gy2, gx2, gy1, gx1, gey2);
					if(tmp>=gex1 && tmp<=gex2){
						tx1=tmp; ty1=gey2; break;
					}
			}
			return(0);
		}
	}

/* �Q�_�ډ����� */
	if(clip_coad2[CLIP_LEFT] || clip_coad2[CLIP_RIGHT] || clip_coad2[CLIP_UPPER] || clip_coad2[CLIP_LOWER]){
		while(1){
			if(clip_coad2[CLIP_LEFT]){
					tmp = _fclip(gx2, gy2, gx1, gy1, gex1);
					if(tmp>=gey1 && tmp<=gey2){
						tx2=gex1; ty2=tmp; break;
					}
			}
			else if(clip_coad2[CLIP_RIGHT]){
					tmp = _fclip(gx1, gy1, gx2, gy2, gex2);
					if(tmp>=gey1 && tmp<=gey2){
						tx2=gex2; ty2=tmp; break;
					}
			}
			if(clip_coad2[CLIP_UPPER]){
					tmp = _fclip(gy2, gx2, gy1, gx1, gey1);
					if(tmp>=gex1 && tmp<=gex2){
						tx2=tmp; ty2=gey1; break;
					}
			}
			else if(clip_coad2[CLIP_LOWER]){
					tmp = _fclip(gy1, gx1, gy2, gx2, gey2);
					if(tmp>=gex1 && tmp<=gex2){
						tx2=tmp; ty2=gey2; break;
					}
			}
			return(0);
		}
	}

/* ���ʂ�E������ */
	*x1 = tx1 - 0x80000000;
	*y1 = ty1 - 0x80000000;
	*x2 = tx2 - 0x80000000;
	*y2 = ty2 - 0x80000000;
	return(1);
}



