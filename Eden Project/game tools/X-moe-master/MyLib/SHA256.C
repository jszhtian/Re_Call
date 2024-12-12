#include<stddef.h>
#include<stdio.h>


#define SHR(x,n)     ((x)>>(n))
#define ROTR(x,n)    (((x)>>(n)) | ((x)<<(32-(n))))
#define CH(sha256_a,sha256_b,sha256_c)    	(((sha256_a)&(sha256_b)) ^ ((~sha256_a)&(sha256_c)))
#define MAJ(sha256_a,sha256_b,sha256_c) 	(((sha256_a)&(sha256_b)) ^ ((sha256_a)&(sha256_c)) ^ ((sha256_b)&(sha256_c)))
#define E0(x)        (ROTR((x),2) ^ ROTR((x),13) ^ ROTR((x),22))
#define E1(x)        (ROTR((x),6) ^ ROTR((x),11) ^ ROTR((x),25))
#define Q0(x)        (ROTR((x),7) ^ ROTR((x),18) ^ SHR((x),3))
#define Q1(x)        (ROTR((x),17) ^ ROTR((x),19) ^ SHR((x),10))

unsigned long sha256_hh[8];
unsigned long sha256_a,sha256_b,sha256_c,sha256_d,sha256_e,sha256_f,sha256_g,sha256_h;
unsigned long sha256_w[64];
unsigned long sha256_K[64]=
{
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};


unsigned long sha256_GetStrLen(unsigned char* ptr)
{
	unsigned char* pTmp;
	unsigned long len=0;
	
	pTmp=ptr;
	
	while(pTmp!=NULL&&(*pTmp!='\0'))
	{
		len++;
		pTmp++;
	}
	return len;
}

void sha256_AddBitLen(unsigned long blen)
{
	sha256_w[14]=0x00000000;
	sha256_w[15]=blen;
}

void sha256_ByteToWord(unsigned char* ptr,unsigned char n)
{
	unsigned char* pTmp;
	unsigned char i,j;
	
	pTmp=ptr;
	
	for(i=0;i<n;i++)
	{
		sha256_w[i]=0;
		for(j=0;j<4;j++)
		{
			sha256_w[i]<<=8;
			sha256_w[i]+=*pTmp;
			pTmp++;
		}
	}

}

void sha256_ClearW()
{
	unsigned char i;
	
	for(i=0;i<64;i++)
	{
		sha256_w[i]=0x00000000;
	}
}

void sha256_ProChunk()
{
	short i;
	unsigned long t1,t2;
	

	for(i=0;i<64;i++)
	{
		if(0<=i&&i<=15)
		{
		}
		if(16<=i&&i<=63)
		{
			sha256_w[i]=Q1(sha256_w[i-2])+sha256_w[i-7]+Q0(sha256_w[i-15])+sha256_w[i-16];
		}    
	}
	

	sha256_a=sha256_hh[0];
	sha256_b=sha256_hh[1];
	sha256_c=sha256_hh[2];
	sha256_d=sha256_hh[3];
	sha256_e=sha256_hh[4];
	sha256_f=sha256_hh[5];
	sha256_g=sha256_hh[6];
	sha256_h=sha256_hh[7];
	

	for(i=0;i<64;i++)
	{
		t1=sha256_h+E1(sha256_e)+CH(sha256_e,sha256_f,sha256_g)+sha256_K[i]+sha256_w[i];
		t2=E0(sha256_a)+MAJ(sha256_a,sha256_b,sha256_c);
		sha256_h=sha256_g;
		sha256_g=sha256_f;
		sha256_f=sha256_e;
		sha256_e=sha256_d+t1;
		sha256_d=sha256_c;
		sha256_c=sha256_b;
		sha256_b=sha256_a;
		sha256_a=t1+t2;
	}
	

	sha256_hh[0] += sha256_a;
	sha256_hh[1] += sha256_b;
	sha256_hh[2] += sha256_c;
	sha256_hh[3] += sha256_d;
	sha256_hh[4] += sha256_e;
	sha256_hh[5] += sha256_f;
	sha256_hh[6] += sha256_g;
	sha256_hh[7] += sha256_h;
}

void SHA256_Test(unsigned char* ptr)
{
	unsigned char* pTmp;
	unsigned long len,len1;
	unsigned char m,n;
	
	pTmp=ptr;
	
	sha256_hh[0] = 0x6a09e667;
	sha256_hh[1] = 0xbb67ae85;
	sha256_hh[2] = 0x3c6ef372;
	sha256_hh[3] = 0xa54ff53a;
	sha256_hh[4] = 0x510e527f;
	sha256_hh[5] = 0x9b05688c;
	sha256_hh[6] = 0x1f83d9ab;
	sha256_hh[7] = 0x5be0cd19;
	
	len=sha256_GetStrLen(pTmp);
	len1=len;

	while(len/64)
	{
		sha256_ClearW();¡£
		sha256_ByteToWord(pTmp,64);
		sha256_ProChunk();
		pTmp+=64;
		len-=64;
	}	
	
	m=len/4;
	n=len%4;
	sha256_ClearW();
	sha256_ByteToWord(pTmp,m);
	switch(n)
	{
		case 0:
			sha256_w[m]=0x80000000;
		break;
		case 1:
			sha256_w[m]=*pTmp;
			sha256_w[m]<<=24;
			sha256_w[m]|=0x00800000;
		break;
		case 2:
			sha256_w[m]=*pTmp;
			sha256_w[m]<<=8;
			pTmp++;
			sha256_w[m]+=*pTmp;
			sha256_w[m]<<=16;
			sha256_w[m]|=0x00008000;
		case 3:
			sha256_w[m]=*pTmp;
			sha256_w[m]<<=8;
			pTmp++;
			sha256_w[m]+=*pTmp;
			sha256_w[m]<<=8;
			pTmp++;
			sha256_w[m]+=*pTmp;
			sha256_w[m]<<=8;
			sha256_w[m]|=0x00000080;
		default:;    
	}
	
	if((len1%64)>=56)
	{
		sha256_ProChunk();
		sha256_ClearW();    
	}
	
	sha256_AddBitLen(len1*8);
	sha256_ProChunk();
	
	sha256_Display();
}
