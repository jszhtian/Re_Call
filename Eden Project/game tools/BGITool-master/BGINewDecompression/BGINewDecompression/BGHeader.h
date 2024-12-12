#ifndef _CBG_H
#define _CBG_H

#include <memory>

static int T004C0180[] = { 0, 1, 8, 16, 9, 2, 3, 10, 17, 24, 32, 25, 18, 11, 4, 5, 12, 19, 26,
33, 40, 48, 41, 34, 27, 20, 13, 6, 7, 14, 21, 28, 35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15,
23, 30, 37, 44, 51, 58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63 };    /* Z��ǰ��*/

static float DCTable[] = { 1.0f, 1.38704f, 1.30656f, 1.17588f, 1.0f, 0.785695f, 0.541196f,
0.275899f, 1.38704f, 1.92388f, 1.81225f, 1.63099f, 1.38704f, 1.08979f, 0.750661f, 0.382683f,
1.30656f, 1.81225f, 1.70711f, 1.53636f, 1.30656f, 1.02656f, 0.707107f, 0.36048f, 1.17588f,
1.63099f, 1.53636f, 1.38268f, 1.17588f, 0.92388f, 0.636379f, 0.324423f, 1.0f, 1.38704f, 1.30656f,
1.17588f, 1.0f, 0.785695f, 0.541196f, 0.275899f, 0.785695f, 1.08979f, 1.02656f, 0.92388f,
0.785695f, 0.617317f, 0.425215f, 0.216773f, 0.541196f, 0.750661f, 0.707107f, 0.636379f,
0.541196f, 0.425215f, 0.292893f, 0.149316f, 0.275899f, 0.382683f, 0.36048f, 0.324423f,
0.275899f, 0.216773f, 0.149316f, 0.0761205f };    /* ֱ��ֵ */

static char error0[] = "No Error";
static char error1[] = "Not a BG File";
static char error2[] = "New type only";
static char error3[] = "Check sum error";
static char error4[] = "Size check error";
static char error5[] = "only support 32-bit data";
static char error6[] = "unknown zero compress";
static char *ErrorInfo = error0;

#pragma pack(1)
class BmpHead             /*  λͼ�ļ�ͷ */
{
public:
	short int mark;
	int FileSize;
	int Zero;
	int DataOffset;
	int HeadSize;
	int Width;
	int Height;
	short int Frame;
	short int BitCount;
	int Compress;
	int DataSize;
	int PerMeterX;
	int PerMeterY;
	int ColourUsed;
	int ColourImportant;
	BmpHead(char *adre, int wi, int hi);
};
#pragma pack()
BmpHead::BmpHead(char *adre, int wi, int hi)
{
	int a = wi * hi * 4;
	mark = 19778;
	FileSize = a + 0x36 + 2;
	Zero = 0;
	DataOffset = 0x36;
	HeadSize = 0x28;
	Width = wi;
	Height = hi;
	Frame = 1;
	BitCount = 32;
	Compress = 0;
	DataSize = a;
	PerMeterX = 0;
	PerMeterY = 0;
	ColourUsed = 0;
	ColourImportant = 0;
	memcpy(adre, this, 54);
}

class CBGHead
{
public:
	char Mark[16];           /*====   CompressedBG__   =====*/
	short int Width;
	short int Height;
	int ColourDeep;
	int Unknow1;
	int Unknow2;
	int CompressZeroCount;      //��ѹ����0�ĸ���
	int Key;
	int EcryptLen;                     //���ܵĳ���
	unsigned char SumCheak;
	unsigned char XorCheak;
	short int Type;             /* ���������ֵ��2 */
};

class RGBBlock             /* RGB���ݿ� */
{
public:
	int RGBSize;                     /* RGB���ݴ�С */
	int BlockSize;                   /* ���ݿ��С */
	char *BlockMark;             /* ���ݿ�ѹ����־λ��*/
	char *EncBlock;             /* �������ݿ��С��λ�� */
	char *RGBStart;               /* ���ݿ�����ʵ��λ�� */
	char *BmpWriteTo;          /* Ҫд����λͼλ�� */
};

class HuffmanNote                 /* �������ڵ� */
{
public:
	int Valid;                                 /* �Ƿ���Ч��� */
	unsigned int NoteWeight;                      /* Ȩ�� */
	int IsFatherNote;                     /* �Ƿ񸸽ڵ� 1�� 0 �� */
	int FatherNoteIndex;              /* ���ڵ����� */
	int LeftChildIndex;                  /* ���ӽڵ����� */
	int RightChildIndex;                /* ���ӽڵ����� */
	HuffmanNote()                      /*��ʼ���ڵ����ݣ�ԭ������Ʋ�������ʵ�ֵģ���ʼ�����������ɽڵ������� */
	{
		Valid = 0;
		NoteWeight = 0;
		IsFatherNote = 0;           /* ԭ������ʹ�õ��� char ����Ȼ 4�ֽڶ��� */
		FatherNoteIndex = -1;
		LeftChildIndex = -1;
		RightChildIndex = -1;
	};
};
/* =========================�����￪ʼ=====================================*/
class CBG
{
public:
	CBGHead *Head;             /* �ļ�ͷ */
	char *EcryptData;           /* �����ܵĽڵ���Ϣ */
	char *WeightInfo;            /* ������ĩ��Ҷ�ڵ�Ȩֵ */
	int *Block;                      /* ���ݿ飬�ܼ��и߶� /8 ������¼�������ݿ��ƫ�ƣ���NoteWeight��ʼ���� */
	char *ZeroBlock;               /* 0�����ݿ飬�����ݿ���ͬ����¼��Ϊ͸��ͨ�� */
	int BlockCount;              /* ���ݿ����� */

	char *BmpData;             /* ָ��Bmp���ݵ�ָ�� */
	int Size;                          /* BMP�ļ��Ĵ�С */
	int PerLineSize;              /*ÿһ�еĴ�С */
	int Wi;                           /* ͼ��� */
	int Hi;                            /* ͼ��� */

	CBG();
	~CBG();
	bool Uncompress(char *FileBuf, int FileSize);  /* ��ѹ������ͼԪ�ļ���ʧ��ʹ��GetError( ) ��ȡ��Ϣ */
	char* GetError(void);      /* ʧ����Ϣ */
	bool CheakFile(CBGHead *buf);    /* ���һЩ��־��ȷ���Ƿ���֧�ֵ����� */
	bool DecEncryptData(char *EncData, int EncSize, char SumN, char XorN, int key); /* ���ܱ�����Ϣ������Ƿ���� */
	int WeightCalculation(int *Weight, char *Data, int Count);
	bool BuildBinaryTree(HuffmanNote *Tree, char *Data, int *Root, int *Deep); /* �ؽ������� */
	int BuildLeastNote(HuffmanNote *Note, int *WeightAry, int Count);
	bool Alpha(char *WriteTo, char *data, unsigned int MaxData);
	bool unRGB(char *WriteTo, HuffmanNote *Tree, int *Root);
	bool UncomRGBInfo(short int *result, RGBBlock *RGBs, HuffmanNote *TreeA, HuffmanNote *TreeB, int RootA, int RootB);
	int FindLeaf(HuffmanNote *Tree, int MaxNum, int Root, char *data, int *bit, int *pos);
	short int GetBit(char *data, int *bit, int *pos, int len);
	int MulEncrypt(float *dest, char * Mul, int Count);
	int FuckLast(short int *ColourInfo, float *Mul, RGBBlock *BK);
	int JpegDctFloat(float *dest, short int *AC, float *ACMul);
	short int FtoI(float result);
	unsigned char CheakRGB(float value);
	int WriteToBmp(char *dest, short int *from);
	int FixPic(char *from, int Ow, int Oh);
private:
	int TressASize;         /* ������A��Ҷ�ӽڵ�������ӦΪ0x10 */
	int TressBSize;          /* ������B��Ҷ�ӽڵ�������ӦΪ0xb0 */
	int CompressLine;      /* ÿ�����н���ѹ����ͨ����ÿ8����Ϊһ��ѹ���� */
	int CompressMarkLen;       /* ѹ����ǵĳ��� = �� /64 �����ܸպ�����Ҫ+1 */
	int TheFileSize;           /* ����ѹ�������ļ����ݳߴ� v0.1.1 */

};
CBG::CBG()
{
	BmpData = 0;
	TressASize = 0x10;
	TressBSize = 0xB0;
	CompressLine = 8;          /* DCT�任������8 ?*/
}

CBG::~CBG()
{
	if (BmpData != 0) delete[] BmpData;
	BmpData = 0;
}

bool CBG::Uncompress(char *FileBuf, int FileSize)
{
	int RootNote[2];
	Head = (CBGHead *)FileBuf;
	EcryptData = FileBuf + 0x30;                                     /* ���ܵı��� */
	WeightInfo = FileBuf + 0x30 + Head->EcryptLen;     /* ���ܵ�Ҷ�ڵ���Ϣ */
	TheFileSize = FileSize;
	if (CheakFile(Head) == false) return false;

	Wi = Head->Width;
	Wi += Wi % 8;        /* ��Ȳ���8����Ҫ���� */
	Hi = Head->Height;
	Hi += Hi % 8;        /* �߶Ȳ���8����Ҫ���� */
	CompressMarkLen = Wi / 64;
	if (Wi % 64 != 0) CompressMarkLen++;        /* ѹ����ǳ��ȣ�0û��ѹ����1ѹ�� */
	PerLineSize = Wi * 4;

	HuffmanNote *Trees = new HuffmanNote[0x17F];
	int deep = 0;
	BuildBinaryTree(Trees, WeightInfo, &RootNote[0], &deep);  /* �ؽ������������������ڵ㣬ʹ�õ��ֽ��� */
	Block = (int *)(WeightInfo + deep);

	WeightCalculation(&deep, WeightInfo + *(Block)+0x10, 1);
	BlockCount = Hi / CompressLine;                                        /* ѹ�������� */
	ZeroBlock = WeightInfo + *(Block + BlockCount);
	if (*(int *)ZeroBlock != 1 && Head->ColourDeep != 24)
	{
		ErrorInfo = error6;
		return false;
	}

	Size = Wi * Hi * 4;
	if (BmpData != 0) delete[] BmpData;
	BmpData = new char[Size + 56];
	memset(BmpData, 0, Size);
	if (Head->ColourDeep == 32)  Alpha(BmpData, ZeroBlock + 4, FileSize - (ZeroBlock - FileBuf - 4));     /* ��ѹ LZѹ����͸��ͨ�� */
	unRGB(BmpData, Trees, &RootNote[0]);   /* JPEG 100%���� */
	delete[] Trees;

	FixPic(BmpData, Head->Width, Head->Height);   /* ����ͼ����ߣ���תͼ�� */
	BmpHead a(BmpData, Head->Width, Head->Height);   /* д��λͼ�ļ�ͷ */
	Size = (Head->Width) * (Head->Height) * 4 + 56;

	return true;
}

bool CBG::CheakFile(CBGHead *buf)
{
	if (buf->Mark[0] != 67 || buf->Mark[1] != 111 || buf->Mark[2] != 109)   /* ����CBG�ļ������ؼ٣����Լ�� */
	{
		ErrorInfo = error1;
		return false;
	}
	if (Head->Type != 2)        /* ��֧�������� */
	{
		ErrorInfo = error2;
		return false;
	}
	if (Head->ColourDeep != 32 && Head->ColourDeep != 24)  /* ��֧��32λ ��24λ */
	{
		ErrorInfo = error5;
		return false;
	}
	if (DecEncryptData(EcryptData, Head->EcryptLen, Head->SumCheak, Head->XorCheak, Head->Key) == false)
	{
		ErrorInfo = error3;   /* DCֱ���������� */
		return false;
	}
	return true;
}
char *CBG::GetError(void)
{
	return ErrorInfo;
}
int CBG::WeightCalculation(int *Weight, char *Data, int Count)
{
	int   i, bit, deep = 0;                               /* ��ɵ���� */
	char result;

	for (i = 0; i < Count; i++)
	{
		result = -1;
		*Weight = 0;
		for (bit = 0; result < 0; bit += 7)
		{
			result = *(Data + deep);
			*Weight |= (result & 0x7F) << bit;
			deep++;
		}
		Weight++;
	}
	return deep;
}
bool CBG::DecEncryptData(char *EncData, int EncSize, char SumN, char XorN, int key)    /* ����Ȩֵ���� ����У���� */
{
	int i, a, b, c;
	unsigned char sumb, xorb;
	sumb = xorb = 0;

	for (i = 0; i < EncSize; i++)
	{
		a = (key & 0xffff) * 20021;
		b = ((key >> 16) & 0xffff) * 20021;
		c = key * 346 + b;
		c += (a >> 16) & 0xffff;
		key = (c << 16) + (a & 0xffff) + 1;
		(*EncData) -= (char)c;
		sumb += (*EncData);
		xorb ^= (*EncData);
		EncData++;
	}
	if (sumb == SumN && xorb == XorN) return false;
	return true;
}


bool CBG::BuildBinaryTree(HuffmanNote *Tree, char *Data, int *Root, int *Deep)
{
	int ss[0x15F];
	*Deep = WeightCalculation(&ss[0], Data, TressASize);
	*Root = BuildLeastNote(Tree, ss, TressASize);
	*Deep += WeightCalculation(&ss[0], Data + (*Deep), TressBSize);
	*(Root + 1) = BuildLeastNote(Tree + 0x1F, ss, TressBSize);
	return true;
}

int CBG::BuildLeastNote(HuffmanNote *Note, int *WeightAry, int Count)
{
	unsigned int LeastNote, SumNote, SumWeight = 0;
	int LR, LeastNoteIndex[2], i, Level, max;
	HuffmanNote *NewNote;

	for (i = 0; i < Count; i++)
	{
		max = *(WeightAry + i);
		(Note + i)->NoteWeight = (*(WeightAry + i));
		if (*(WeightAry + i) != 0)  (Note + i)->Valid = 1;
		SumWeight += *(WeightAry + i);
	}

	for (Level = 0; Level < Count; Level++)
	{
		max = Count + Level;
		SumNote = 0;
		for (LR = 0; LR < 2; LR++)
		{
			for (LeastNoteIndex[LR] = 0; (Note + LeastNoteIndex[LR])->Valid == 0 && LeastNoteIndex[LR] < max; LeastNoteIndex[LR]++);
			/* Ĭ�ϵ���С�ڵ�ӵ�һ����Ч�Ľڵ㿪ʼ */
			LeastNote = (Note + LeastNoteIndex[LR])->NoteWeight;
			for (i = LR + 1; i < max + LR + 1; i++)
			{
				NewNote = Note + i;
				if (NewNote->Valid != 0 && (NewNote->NoteWeight) < LeastNote)
				{
					LeastNoteIndex[LR] = i;
					LeastNote = NewNote->NoteWeight;
				}
			}
			NewNote = Note + LeastNoteIndex[LR];
			if (NewNote->Valid == 0) return -1;  /* ������*/
			NewNote->Valid = 0;
			NewNote->IsFatherNote = LR;
			NewNote->FatherNoteIndex = max;
			SumNote += NewNote->NoteWeight;
		}
		NewNote = Note + max;
		NewNote->Valid = 1;
		NewNote->NoteWeight = SumNote;
		NewNote->IsFatherNote = -1;
		NewNote->FatherNoteIndex = max;
		NewNote->LeftChildIndex = LeastNoteIndex[0];
		NewNote->RightChildIndex = LeastNoteIndex[1];
		if (SumNote >= SumWeight) return max;
	}
	return max;
}




bool CBG::Alpha(char *WriteTo, char *data, unsigned int MaxData)
{
	char WTR;        /* LZ�ж���һ���ֽ��Ƿ��ظ���һ���ֽڵ�ÿ��λ����һ��״̬ 0���ظ� 1�ظ� */
	int offset = 3;
	int WTRcount, DataCount, bit;  /* �ж��ֽ�ƫ�ƣ�����ƫ�ƣ�λ */
	int LZword, count, crossline, LORpos, i;
	/* ����LZ��Ϣ���֣� �ظ��Ĵ�����LZ������-���� ���ض�λ��������-�ض�λλ�� */
	char *pos, *Rpos;

	WTRcount = bit = 0;
	DataCount = 1;

	while (offset < Size)
	{
		WTR = *(data + WTRcount);
		if ((WTR & (1 << bit)) != 0)
		{
			LZword = *(short int *)(data + DataCount);
			count = ((LZword >> 9) & 0x7F) + 3;     /* 0-6λ */
			crossline = (LZword >> 6) & 7;                  /* 7-8?9λ */
			LORpos = LZword & 0x3F;                           /* 9-15 λ */
			if (LORpos > 0x1F) LORpos |= 0xFFFFFFC0;      /* ��9λ��0����ô������� -LORpos */
			if (crossline != 0) crossline |= 0xFFFFFFF8;   /* ���б�ǲ�Ϊ0 �����������������ǰȡֵ�� */
			Rpos = WriteTo + offset + LORpos * 4 + crossline * PerLineSize;
			pos = WriteTo + offset;

			for (i = 0; i< count; i++)
			{
				*pos = *Rpos;
				pos += 4;
				Rpos += 4;
			}
			DataCount += 2;
			offset += count * 4;
		}
		else
		{
			*(WriteTo + offset) = *(data + DataCount);
			offset += 4;
			DataCount++;
		}
		bit++;
		if (bit == 8)
		{
			bit = 0;
			WTRcount = DataCount;
			DataCount++;
		}
		if (DataCount >= MaxData) break;
	}
	return true;
}
bool CBG::unRGB(char *WriteTo, HuffmanNote *Tree, int *Root)
{
	int i, a, PerSize;
	RGBBlock BK;
	float Mul[128];

	MulEncrypt(&Mul[0], EcryptData, 0x80);
	PerSize = Wi * CompressLine * 4;

	for (i = 0; i < BlockCount; i++)
	{
		BK.BlockMark = WeightInfo + *(Block + i);
		BK.EncBlock = BK.BlockMark + CompressMarkLen;
		BK.BlockSize = *(Block + i + 1) - *(Block + i) - CompressMarkLen;
		if (BK.BlockSize < 0) BK.BlockSize = TheFileSize - *(Block + i) - CompressMarkLen;   //ver 0.1.1 24λ_λͼ���һ��û�в��գ���Ҫ���⴦��
		BK.RGBStart = BK.EncBlock + WeightCalculation(&a, BK.EncBlock, 1);
		BK.RGBSize = Wi * 8 * 3;
		BK.BmpWriteTo = WriteTo + PerSize * i;
		short int *ss = new short int[BK.RGBSize];
		memset(ss, 0, BK.RGBSize * 2);
		UncomRGBInfo(ss, &BK, Tree, Tree + 0x1F, *Root, *(Root + 1));
		FuckLast(ss, &Mul[0], &BK);
		delete[] ss;
	}
	return true;
}

bool CBG::UncomRGBInfo(short int *result, RGBBlock *BK, HuffmanNote *TreeA, HuffmanNote *TreeB, int RootA, int RootB)
{
	int i, bit, DataOffset, len, Value, SK;
	char *NewStart;

	SK = bit = DataOffset = 0;
	for (i = 0; i < BK->RGBSize; i += 0x40)
	{
		len = FindLeaf(TreeA, 0x10, RootA, BK->RGBStart, &bit, &DataOffset);
		if (len != 0)
		{
			Value = GetBit(BK->RGBStart, &bit, &DataOffset, len);
			if (((1 << (len - 1)) & Value) == 0)  Value = (0xFFFFFFFF << len) | Value + 1;    /* ���λ����1 ���ֵ�Ǹ�ֵ*/
			SK += Value;
		}
		*(result + i) = SK & 0xFFFF;        /* ������Ƕ������� */
		if (DataOffset > BK->BlockSize) break;
	}
	if (bit != 0) DataOffset++;

	NewStart = BK->RGBStart + DataOffset;
	bit = DataOffset = 0;
	for (i = 0; i < BK->RGBSize; i += 0x40)
	{
		for (SK = 1; SK < 0x40;)
		{
			len = FindLeaf(TreeB, 0xB0, RootB, NewStart, &bit, &DataOffset);
			if (len == 0) break;
			if (len < 0xF)
			{
				SK += 0x10;
				continue;
			}
			SK += len & 0xF;      /* ��4λ���ƫ����Ϣ */
			len = len >> 4;           /* ���²��ִ�ų�����Ϣ  */
			Value = GetBit(NewStart, &bit, &DataOffset, len);
			if (((1 << (len - 1)) & Value) == 0 && len != 0)  Value = (0xFFFFFFFF << len) | Value + 1;    /* ���λ����1 ���ֵ�Ǹ�ֵ*/
			if (SK > 64) break;         /* ������ */
			*(result + i + T004C0180[SK]) = Value & 0xFFFF;
			SK++;
		}
		if (DataOffset > BK->BlockSize) break;
	}
	return true;
}
int CBG::FindLeaf(HuffmanNote *Tree, int MaxNum, int Root, char *data, int *bit, int *pos)
{
	HuffmanNote *NewIndex = Tree + Root;
	char HFword;
	bool result = true;
	int ValveLen, rbit, rpos;
	rbit = *bit;
	rpos = *pos;

	while (result)
	{
		HFword = *(data + rpos);
		if ((HFword >> (8 - rbit - 1) & 1) == 1)             /*  ��BITλΪ 1 �����ұߣ��ж��Ƿ񵽴�Ҷ�ڵ� */
		{
			ValveLen = NewIndex->RightChildIndex;
			NewIndex = Tree + ValveLen;
			if (ValveLen < MaxNum) result = false;
		}
		else
		{
			ValveLen = NewIndex->LeftChildIndex;
			NewIndex = Tree + ValveLen;
			if (ValveLen < MaxNum) result = false;
		}
		rbit++;
		if (rbit >= 8)              /*  bit��λ��ָ����һ���ֽ� */
		{
			rbit = 0;
			rpos++;
		}
	}

	*bit = rbit;
	*pos = rpos;
	return ValveLen;
}
short int CBG::GetBit(char *data, int *bit, int *pos, int len)
{
	int BitValue, rbit, rpos;
	short int value;
	rbit = *bit;
	rpos = *pos;
	char debug;

	for (value = 0; len > 0; len--)
	{
		debug = *(data + rpos);
		BitValue = (*(data + rpos)) >> (8 - rbit - 1);
		value = (value << 1) + (BitValue & 1);
		rbit++;
		if (rbit >= 8)
		{
			rbit = 0;
			rpos++;
		}
	}

	*bit = rbit;
	*pos = rpos;
	return value;
}
int CBG::MulEncrypt(float *dest, char * Mul, int Count)
{
	int i, a;
	for (a = 0; a < Count; a += 0x40)
	{
		for (i = 0; i < 0x40; i++)
		{
			*dest = (*Mul) * DCTable[i];
			Mul++;
			dest++;
		}
	}
	return 0;
}
int CBG::FuckLast(short int *ColourInfo, float *Mul, RGBBlock *BK)
{
	int count = BK->RGBSize / 64 / 3;
	int i, Channel, bit, Offset;
	short int *TRGB;
	float temp[64];

	bit = 8;
	for (Offset = i = 0; i < count; i++)          //ver 0.1.1
	{
		bit--;
		if (bit < 0)         /* ѹ����Ǽ�� */
		{
			bit = 8;
			Offset = 1;
		}
		if (((*(BK->BlockMark + Offset) >> bit) & 1) == 0) continue;
		for (Channel = 0; Channel < 3; Channel++)
		{
			TRGB = ColourInfo + i * 64 + Channel * Wi * CompressLine;
			if (Channel > 0) JpegDctFloat(&temp[0], TRGB, Mul + 64);  //ver 0.1.1 
			else JpegDctFloat(&temp[0], TRGB, Mul);

		}
		WriteToBmp(BK->BmpWriteTo, TRGB);
		BK->BmpWriteTo += 32;
	}
	return 0;
}
int CBG::JpegDctFloat(float *dest, short int *AC, float *ACMul)
{
	short int inptr[64];
	float dv[128];
	float tp[64];
	float debug[8];
	int i;
	float tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
	float tmp10, tmp11, tmp12, tmp13;
	float z5, z10, z11, z12, z13;

	for (i = 0; i < 64; i++)
	{
		inptr[i] = *(AC + i);
		/* inptr[ i ] = i / 8 + 1 ;   // debug */
	}
	for (i = 0; i < 128; i++)
	{
		dv[i] = *(ACMul + i);
	}
	for (i = 0; i < 8; i++)
	{
		if (inptr[8 + i] == 0 && inptr[16 + i] == 0 && inptr[24 + i] == 0 && inptr[32 + i] == 0
			&& inptr[40 + i] == 0 && inptr[48 + i] == 0 && inptr[56 + i] == 0)
		{
			tmp0 = inptr[i] * dv[i];
			tp[i] = tmp0;
			tp[8 + i] = tmp0;
			tp[16 + i] = tmp0;
			tp[24 + i] = tmp0;
			tp[32 + i] = tmp0;
			tp[40 + i] = tmp0;
			tp[48 + i] = tmp0;
			tp[56 + i] = tmp0;
			continue;
		}

		tmp0 = inptr[i] * dv[i];
		tmp1 = inptr[16 + i] * dv[16 + i];
		tmp2 = inptr[32 + i] * dv[32 + i];
		tmp3 = inptr[48 + i] * dv[48 + i];
		tmp10 = tmp0 + tmp2;	/* phase 3 */
		tmp11 = tmp0 - tmp2;
		tmp13 = tmp1 + tmp3;	/* phases 5-3 */
		tmp12 = (tmp1 - tmp3) * 1.414213562 - tmp13; /* 2*c4 */
		tmp0 = tmp10 + tmp13;	/* phase 2 */
		tmp3 = tmp10 - tmp13;
		tmp1 = tmp11 + tmp12;
		tmp2 = tmp11 - tmp12;
		tmp4 = inptr[8 + i] * dv[8 + i];
		tmp5 = inptr[24 + i] * dv[24 + i];
		tmp6 = inptr[40 + i] * dv[40 + i];
		tmp7 = inptr[56 + i] * dv[56 + i];
		z13 = tmp6 + tmp5;		/* phase 6 */
		z10 = tmp6 - tmp5;
		z11 = tmp4 + tmp7;
		z12 = tmp4 - tmp7;

		tmp7 = z11 + z13;		/* phase 5 */
		tmp11 = (z11 - z13) * 1.414213562; /* 2*c4 */
		z5 = (z10 + z12) * 1.847759065; /* 2*c2 */
		tmp10 = z12 * 1.082392200 - z5; /* 2*(c2-c6) */
		tmp12 = z10 * (-2.613125930) + z5; /* -2*(c2+c6) */

		tmp6 = tmp12 - tmp7;	/* phase 2 */
		tmp5 = tmp11 - tmp6;
		tmp4 = tmp10 + tmp5;

		tp[i] = tmp0 + tmp7;
		tp[56 + i] = tmp0 - tmp7;
		tp[8 + i] = tmp1 + tmp6;
		tp[48 + i] = tmp1 - tmp6;
		tp[16 + i] = tmp2 + tmp5;
		tp[40 + i] = tmp2 - tmp5;
		tp[32 + i] = tmp3 + tmp4;
		tp[24 + i] = tmp3 - tmp4;
	}
	/*
	for( i = 0 ; i < 8 ; i++ )
	{
	tp[ i ] = i + 1 ;
	}    */
	for (i = 0; i < 8; i++)
	{
		z5 = tp[i * 8];
		tmp10 = z5 + tp[8 * i + 4];
		tmp11 = z5 - tp[8 * i + 4];

		tmp13 = tp[8 * i + 2] + tp[8 * i + 6];
		tmp12 = (tp[8 * i + 2] - tp[8 * i + 6]) * 1.414213562f - tmp13;

		tmp0 = tmp10 + tmp13;
		tmp3 = tmp10 - tmp13;
		tmp1 = tmp11 + tmp12;
		tmp2 = tmp11 - tmp12;

		z13 = tp[8 * i + 5] + tp[8 * i + 3];
		z10 = tp[8 * i + 5] - tp[8 * i + 3];
		z11 = tp[8 * i + 1] + tp[8 * i + 7];
		z12 = tp[8 * i + 1] - tp[8 * i + 7];

		tmp7 = z11 + z13;
		tmp11 = (z11 - z13) *  1.414213562;

		z5 = (z10 + z12) * 1.847759065; /* 2*c2 */
		tmp10 = z5 - z12 * 1.082392200; /* 2*(c2-c6) */
		tmp12 = z5 - z10 * 2.613125930; /* 2*(c2+c6) */

		tmp6 = tmp12 - tmp7;
		tmp5 = tmp11 - tmp6;
		tmp4 = tmp10 - tmp5;

		debug[0] = tmp0 + tmp7;
		debug[1] = tmp0 - tmp7;
		debug[2] = tmp1 + tmp6;
		debug[3] = tmp1 - tmp6;
		debug[4] = tmp2 + tmp5;
		debug[5] = tmp2 - tmp5;
		debug[6] = tmp3 + tmp4;
		debug[7] = tmp3 - tmp4;

		*AC = FtoI(tmp0 + tmp7);
		*(AC + 7) = FtoI(tmp0 - tmp7);
		*(AC + 1) = FtoI(tmp1 + tmp6);
		*(AC + 6) = FtoI(tmp1 - tmp6);
		*(AC + 2) = FtoI(tmp2 + tmp5);
		*(AC + 5) = FtoI(tmp2 - tmp5);
		*(AC + 3) = FtoI(tmp3 + tmp4);
		*(AC + 4) = FtoI(tmp3 - tmp4);
		AC += 8;
	}
	return 0;
}

short int CBG::FtoI(float result)
{
	int a;
	a = ((int)result) >> 3;
	a += 0x80;
	if (a < 0) return 0;
	if (a < 0xff) return (unsigned short int)a;
	if (a < 0x180) return 0xff;
	return 0;
}
int CBG::WriteToBmp(char *dest, short int *from)
{
	unsigned char Y, Cr, Cb;
	float R, G, B;
	int h, w, Off = Wi * 8;
	char *rdest;
	for (h = 0; h < 8; h++)
	{
		rdest = dest + h * PerLineSize;
		for (w = 0; w < 8; w++)
		{
			Y = *(char *)(from - 2 * Off);  //0
			Cb = *(char *)(from - Off); //1
			Cr = *(char *)from; //2

			R = Y + 1.402f * Cr - 178.956f;
			G = Y + 44.04992f - 0.34414f * Cb + 91.90992f - 0.71414f * Cr;
			B = Y + 1.772f * Cb - 226.316f;

			*(rdest + 2) = CheakRGB(R);     /* �߽��飬�����Ҫ����ֵ */
			*(rdest + 1) = CheakRGB(G);
			*rdest = CheakRGB(B);
			rdest += 4;
			from++;
		}
	}
	return 0;
}
unsigned char CBG::CheakRGB(float value)
{
	if (value < 0) return 0;
	if (value < 255) return (unsigned)value;
	if (value >255 && value < 512) return 255;
	return 0;
}
int CBG::FixPic(char *from, int Ow, int Oh)
{
	char *to = new char[Ow * Oh * 4 + 56];
	int *writeto, *writefrom, i, a;

	writeto = (int *)(to + 54);
	for (i = 0; i < Oh; i++)
	{
		writefrom = (int *)from + Wi * (Oh - i - 1) - (Wi - Ow);
		for (a = 0; a < Ow; a++)         /* ver 0.1.1 */
		{
			*writeto = *writefrom;
			writeto++;
			writefrom++;
		}
	}
	delete[] BmpData;
	BmpData = to;

	return 0;
}


#endif
