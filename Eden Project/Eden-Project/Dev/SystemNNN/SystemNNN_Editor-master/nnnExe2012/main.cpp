//
// main.cpp
//



#include <windows.h>
#include <stdio.h>
#include <math.h>









#include <Math.h>
//#include <bits.nan.h>

#include "resource.h"

#define __TINYANLIBDIR__ "d:\\tinyan\\systemNNNLib\\"
#define __LIBPNGDIR__ "d:\\libpng\\lib\\"

#define __TINYANINCLUDEDIR__ "d:\\tinyan\\systemNNNLib\\"
//#define MYINCLUDE(x) __TINYANINCLUDEDIR__ #x
//#define MYINCLUDE2(x) "d:\\tinyan\\systemNNNLib\\include\\" #x

//#include MYINCLUDE2("aaa")





#include "d:\tinyan\systemNNNLib\\nyanLib\include\libinclude2017.h"

#include "d:\tinyan\systemNNNLib\nyanLib\include\commonmacro.h"

#include "d:\tinyan\systemNNNLib\nnnUtilLib\commongameVersion.h"

#include "d:\tinyan\systemNNNLib\nnnLib\mainControl.h"
#include "d:\tinyan\systemNNNLib\nnnLib\gameCallBack.h"

#include "d:\tinyan\systemNNNLib\\nnnUtilLib\currentDirControl.h"


#include "d:\tinyan\systemNNNLib\\nnnUtillib\allPackFile.h"


#include "game.h"
#include "main.h"














/*
char m_to2byteTable[]= "\
Å@ÅIÅhÅîÅêÅìÅïÅfÅiÅjÅñÅ{ÅCÅ|ÅDÅ^\
ÇOÇPÇQÇRÇSÇTÇUÇVÇWÇXÅFÅGÅÉÅÅÅÑÅH\
ÅóÇ`ÇaÇbÇcÇdÇeÇfÇgÇhÇiÇjÇkÇlÇmÇn\
ÇoÇpÇqÇrÇsÇtÇuÇvÇwÇxÇyÅmÅèÅnÅOÅQ\
ÅeÇÅÇÇÇÉÇÑÇÖÇÜÇáÇàÇâÇäÇãÇåÇçÇéÇè\
ÇêÇëÇíÇìÇîÇïÇñÇóÇòÇôÇöÅoÅbÅpÅ`Å°\
Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°\
Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°\
ÉAÅBÅuÅvÅAÅEÉíÉ@ÉBÉDÉFÉHÉÉÉÖÉáÉb\
Å[ÉAÉCÉEÉGÉIÉJÉLÉNÉPÉRÉTÉVÉXÉZÉ\\
É^É`ÉcÉeÉgÉiÉjÉkÉlÉmÉnÉqÉtÉwÉzÉ}\
É~ÉÄÉÅÉÇÉÑÉÜÉàÉâÉäÉãÉåÉçÉèÉìÅJÅK";

	typedef struct _tagCOLORNAMETABLE
	{
		int name;
		int col;
	} COLORNAMETABLE;

COLORNAMETABLE m_colorNameTable[100]=
{
	{'çï',0x000000},
	{'ê¬',0xff0000},
	{'ê‘',0x0000ff},
	{'éá',0xff00ff},
	{'óŒ',0x00ff00},
	{'êÖ',0xffff00},
	{'â©',0x00ffff},
	{'îí',0xffffff},

	{'ûÚ',0x0080ff},

	{'ç˜',0xc4d2fc},//252 210 196
	{'ìç',0x938af6},//246 138 147
	{'ä`',0x4760da},//218 96 71
	{'éÈ',0x648aff},//255 138 100
	{'åœ',0x3968a7},//167 104 57
	{'äD',0x6f7d90},//144 125 111
	{'ÈÚ',0x556876},//118 104 85
	{'ë€',0x4c8e9a},//154 142 76
	{'ïc',0x67bd9c},//156 189 103
	{'í|',0x74a27d},//125 162 116
	{'ï…',0x727b4b},//75 123 114
	{'ìS',0x44483b},//59 72 68
	{'óï',0x5a5535},//53 85 90
	{'ãÛ',0xb99e5f},//95 158 185
	{'çÆ',0x483025},//37 48 72
	{'ì°',0xac8399},//153 131 172
	{'î~',0x68539f},//159 83 104
	{'çg',0x533abc},//188 58 83
	{'‰ï',0x4f00c5},//197 0 79
	{'à«',0x6797e2},//226 151 103
	{'àπ',0x3d719e},//158 113 61
	{'åÕ',0x96c3ea},//234 195 150
	{'åI',0x0b1544},//68 21 11
	{'ã‡',0x0081a8},//168 129 0
	{'ót',0xa4cfbc},//188 207 164
	{'âî',0xf9fbf9},//249 251 249
	{'îß',0xacc6f2},//242 198 172
	{'èI',0x000000},//242 
};

//402387260077093773543702433923003985719374864210714632543799910429938512398629020592044208486969404800479988610197196058631666872994808558901323829669944590997424504087073759918823627727188732519779505950995276120874975462497043601418278094646496291056393887437886487337119181045825783647849977012476632889835955735432513185323958463075557409114262417474349347553428646576611667797396668820291207379143853719588249808126867838374559731746136085379534524221586593201928090878297308431392844403281231558611036976801357304216168747609675871348312025478589320767169132448426236131412508780208000261683151027341827977704784635868170164365024153691398281264810213092761244896359928705114964975419909342221566832572080821333186116811553615836546984046708975602900950537616475847728421889679646244945160765353408198901385442487984959953319101723355556602139450399736280750137837615307127761926849034352625200015888535147331611702103968175921510907788019393178114194545257223865541461062892187960223838971476088506276862967146674697562911234082439208160153780889893964518263243671616762179168909779911903754031274622289988005195444414282012187361745992642956581746628302955570299024324153181617210465832036786906117260158783520751516284225540265170483304226143974286933061690897968482590125458327168226458066526769958652682272807075781391858178889652208164348344825993266043367660176999612831860788386150279465955131156552036093988180612138558600301435694527224206344631797460594682573103790084024432438465657245014402821885252470935190620929023136493273497565513958720559654228749774011413346962715422845862377387538230483865688976461927383814900140767310446640259899490222221765904339901886018566526485061799702356193897017860040811889729918311021171229845901641921068884387121855646124960798722908519296819372388642614839657382291123125024186649353143970137428531926649875337218940694281434118520158014123344828015051399694290153483077644569099073152433278288269864602789864321139083506217095002597389863554277196742822248757586765752344220207573630569498825087968928162753848863396909959826280956121450994871701244516461260379029309120889086942028510640182154399457156805941872748998094254742173582401063677404595741785160829230135358081840096996372524230560855903700624271243416909004153690105933983835777939410970027753472000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000










#include <locale.h>
//#define __DEMO_VERSION__

void test(void);
void test(void)
{
	int qtable[][10]=
	{
		{1,1,1,1,1,1,1,1,1,1},
		{2,2,2,2,2,2,2,2,2,2},
		{3,3,3,3,3,3,3,3,3,3},

		{1,1,1,1,1,1,1,1,2,2},
		{2,2,2,2,2,2,2,2,3,3},
		{3,3,3,3,3,3,3,3,1,1},

		{1,1,1,1,2,2,2,2,2,2},
		{2,2,2,2,3,3,3,3,3,3},
		{3,3,3,3,1,1,1,1,1,1},

		{1,1,2,2,2,2,1,1,2,2},
		{2,2,3,3,3,3,2,2,3,3},
		{3,3,1,1,1,1,3,3,1,1},

		{1,2,2,1,2,1,1,2,2,1},
		{2,3,3,2,3,2,2,3,3,2},
		{3,1,1,3,1,3,3,1,1,3},
	};



	







	int* anstable = new int[1024*1024*2];

	int bt[10];
	bool f = TRUE;
	
	int lp = 15;

	for (int i=0;i<1024*1024;i++)
	{
		int d = i;
		for (int k=0;k<10;k++)
		{
			bt[k] = d & 3;
			d >>= 2;
		}

		int a[15];
		for (int p=0;p<lp;p++)
		{
			int s = 0;
			for (int q=0;q<10;q++)
			{
				if (bt[q] == qtable[p][q]-1) s++;
			}
			a[p] = s;
		}



		int aa[2];
		aa[0] = 0;
		aa[1] = 0;
		int ml = 1;
		for (int k=0;k<lp;k++)
		{
			aa[k & 1] += a[k] * ml;
			if (k & 1) ml *= 11;
		}

		anstable[i*2+0] = aa[0];
		anstable[i*2+1] = aa[1];

		//check
		for (int c=0;c<i;c++)
		{
			if ((aa[1] == anstable[c*2+1]) && (aa[0] == anstable[c*2+0]))
			{
				f = false;
				char mes[256];
				sprintf_s(mes,256,"bad num=%d :",c);
				OutputDebugString(mes);



				for (int k=0;k<10;k++)
				{
					char m[256];
					sprintf_s(m,256,"%d ",bt[k]+1);
					OutputDebugString(m);
				}

				OutputDebugString(":");
				int d2 = c;
				for (int k=0;k<10;k++)
				{
					char m[256];
					sprintf_s(m,256,"%d ",(d2 & 3)+1);
					OutputDebugString(m);
					d2 >>= 2;
				}


				break;
			}
		}

		if (!f) break;

		if ((i & 0xffff) == 0)
		{
			OutputDebugString("*");
		}

	}

	delete [] anstable;

	exit(0);
}
*/

/*
struct aaa
{
	aaa() {}
	~aaa() {}
	int x;
};

struct bbb
{
	bbb() {}
	virtual ~bbb() {}
	virtual void ccc(void) {}
	virtual void ddd(void) {}
	int x;
};
*/




#include <time.h>












int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{


	/*

	for (int i = 1; i < 100; i++)
	{
		float db = 6000.0f + 2000.0f * (log(i) / log(10));
		char mes[256];
		sprintf_s(mes, 256, "\nvol=%d:%dDB", i, (int)db);
		OutputDebugString(mes);
		//6000.0f + 2000.0f * log(m_volume) / log(10);

	}
	
	return 0;
	*/


	/*
	srand(time(NULL));

	int a = (int)(rand()%1000);
	a = 789;
	int b = 765;
	int c = 666;
	int d = 333;
	int e = 0;
	int f = 0;

	srand(a+b+c+d+e+f);
	int x = (int)(rand() % 1000);
	int delta = abs(x - a) - abs(x - b);


	return 0;
	*/


//	double a = 0x7000000000001234;
//	long long b = (long long)a;
//	b += 0;

	/*
	int a = sizeof(aaa);
	int b = sizeof(bbb);
	char mes[256];
	sprintf_s(mes, 256, "%d %d", a, b);
	OutputDebugString(mes);

	return 0;
	*/

	/*
	int saikoro[6];
	for (int i = 0; i < 6; i++)
	{
		saikoro[i] = 0;
	}
	for (int a = 1; a <= 8; a++)
	{
		for (int b = 1; b <= 8; b++)
		{
			for (int c0 = 1; c0 <= 8; c0++)
			{
				int c = c0;
				if (b == c0)
				{
					c = 9 - b;
				}

				int r = -1;
				int minbc = min(b, c);
				int maxbc = max(b, c);

				if (a < minbc)
				{
					r = a;
				}
				else if ((minbc <= a) && (a < maxbc))
				{
					r = a - 1;
				}
				else if (a >= maxbc)
				{
					r = a - 2;
				}
				else
				{
					OutputDebugString("Error!!!!");
				}

				if ((r < 1) || (r > 6))
				{
					char mes[256];
					sprintf_s(mes, 256, "%d %d %d = %d\n", a,b,c0, r);
					OutputDebugString(mes);

				}
				saikoro[r-1]++;

			}
		}
	}

	for (int i = 0; i < 6; i++)
	{
		char mes[256];
		sprintf_s(mes, 256, "%d = %d\n", i + 1, saikoro[i]);
		OutputDebugString(mes);
	}

	return 0;
	*/


	/*
	for (int n=1;n<30;n++)
	{
//		int loopK = (int)(2.0*n*log(n+2.0));
		int loopK = (int)(2.0*n*log(n+0.0)+2);
		int sum = 2;
		for (int k=2;k<=loopK;k++)
		{



			int sum3 = 0;
			for (int j=2;j<=k;j++)
			{
				int sum4 = 0;
				for (int i=1;i<=j;i++)
				{
					sum4 += ((int)(j/i) - (int)((j-1)/i));
				}

				double ss = ((2.0-sum4)/j);
				int ss4 = (int)(ss);
				if (ss<0)
				{
					ss4 = (int)(-ss);
					ss4 *= -1;
					ss4 -= 1;
				}

//				sum3 += (1 + (int)((2-sum4)/j));
				sum3 += (1 + ss4);



			}

			double sss = (double)sum3;
			sss /= n;
			int ss3 = (int)sss;
			if (sss<0)
			{
				ss3 = (int)(-sss);
				ss3 *= -1;
				ss3 -= 1;
			}


//			int sum2 = 1-(int)(sum3 / n);
			int sum2 = 1-ss3;

			sum += sum2;
		}

		char mes[256];
		sprintf_s(mes,256,"%d : %d\x00d\x00a",n,sum);
		OutputDebugString(mes);

	}
	return 0;
	*/


	/*
	CoInitialize(NULL);
	HIMC hIMC = ImmCreateContext();

	char comp[] = "Ç†Ç¢";
	int rt = ImmSetCompositionString(hIMC,SCS_SETSTR,comp,4,NULL,0);


	char buf[1024];
	CANDIDATELIST* ptr = (CANDIDATELIST*)buf;
	ImmGetCandidateList(hIMC,0,ptr,1023);





	ImmDestroyContext(hIMC);

	return 0;
	*/



//	test();

//	OutputDebugString("12" + 1);
//	return 0;

	/*
	double p = 3.1415926535;
	double pp = 1/p;
	double total = 0;
	double kj=1;

	for (int i=0;i<10;i++)
	{
		int n = i*2+1;
		pp *= (p*p);
		if (i>0)
		{
			pp /= (n*(n-1));
		}

		if ((i % 2) == 0)
		{
			total += pp;
		}
		else
		{
			total -= pp;
		}

		char mes[256];
		sprintf_s(mes,256,"%f %f \x00d\x00a ",pp,total);
		OutputDebugString(mes);

	}

	return 0;
	*/

	/*
	FILE* file = NULL;
	fopen_s(&file,"sj2.txt","wb");
	for (int j=0;j<37;j++)
	{
		char mes[256];

		sprintf_s(mes,256,"{0x%x,",m_colorNameTable[j].name);
		fwrite(mes,sizeof(char),strlen(mes),file);


		sprintf_s(mes,256,"0x%x},\x00a",m_colorNameTable[j].col);
		fwrite(mes,sizeof(char),strlen(mes),file);
	}
	fclose(file);

	return 0;
	*/

	/*
	CAllPackFile* allPack = new CAllPackFile("iosPack.nya");


//	INT64 seek = allPack->GetFileOffset("dwq\\ta\\ta_kyuu.dwq");
	INT64 seek = allPack->GetFileOffset("init\\config.xtx");
	char tmp[4096];
	if (seek > 0)
	{
		FILE* file = NULL;
		fopen_s(&file,"iosPack.nya","rb");
		if (file != NULL)
		{
			fsetpos(file,&seek);
			fread(tmp,sizeof(char),4096,file);
			fclose(file);
		}

	}

	delete allPack;

	*/

	/*


	char a[11];
	putchar (64);
	for (int k=0;k<10;k++)
	{
a[k]=
		68 + (4+(((888*k*k*k*k + 454*k*k +(k/8)*4)%4000000)%2050019)%435)%20
	;
	}
	a[10]=0;
	printf(a);
	//return 0;

	char hello[] = "HELLOWORLD";
	int world[10];
	int am[10];
	LONGLONG aaaaa = 0;
	for (int i=0;i<10;i++)
	{
		world[i] = hello[i] - 77;

		aaaaa += world[i]+9;
		char mes[256];
		sprintf_s(mes,256,"%d ",world[i]+9);
		OutputDebugString(mes);

		if (i<9) aaaaa *= 19;
	}
	//0.41687806675200

	//%19
	//0.1315291488960

	//0.04010808111911140800

	return 0;

	for (int i=0;i<10;i++)
	{
		am[i] = (world[i]  + 9 ) % 20;
	}

	*/

	/*
//	int found = 0;
	int foundA[1000];
	int foundB[1000];
	int foundMD[1000];
	for (int i=0;i<1000;i++)
	{
		foundA[i] = 0;
		foundB[i] = 0;
		foundMD[i] = 0;
	}

	for (int a=888;a<1000;a++)
	{
		for (int b=444;b<500;b++)
		{
			for (int md = 20;md<1000;md++)
			{
				for (int i=1;i<10;i++)
				{
					int d = a*i*i*i*i + b*i*i;

					int amari = d % md;
					amari += am[0];
					amari += 20;
					amari %= 20;
					if (amari != am[i])
					{
						break;
					}

					if (i>=6)
					{
						foundA[a] = a;
						foundB[b] = b;
						foundMD[md] = md;

					}
				}

			}
		}
	}

	for (int i=0;i<1000;i++)
	{
		if (foundA[i])
		{
			char mes[256];
			sprintf_s(mes,256,"A=%d\n",i);
			OutputDebugString(mes);
		}
	}
	for (int i=0;i<1000;i++)
	{
		if (foundB[i])
		{
			char mes[256];
			sprintf_s(mes,256,"B=%d\n",i);
			OutputDebugString(mes);
		}
	}
	for (int i=0;i<1000;i++)
	{
		if (foundMD[i])
		{
			char mes[256];
			sprintf_s(mes,256,"md=%d\n",i);
			OutputDebugString(mes);
		}
	}
	*/
//	return 0;

	/*
	for (int a=888;a<889;a++)
	{
		for (int b=454;b<455;b++)
		{
			for (int md=435;md<436;md++)
			{
				for (int i=1;i<10;i++)
				{
//					int d = a*i*i*i*i + b*i*i + (i/7)*6 + (i/8)*4 -(i/9)*11;
					int d = (a*i*i*i*i + b*i*i)%1200009;

					int amari = d % md;
					amari += am[0];
					amari += 20;
					amari %= 20;
					if (amari != am[i])
					{
						break;
					}

					int dbg[10];
					int dbg2[10];
					if (i >= 6)
					{
						char mes[256];
						sprintf_s(mes,256,"**********:a=%d b=%d md=%d i=%d\n",a,b,md,i);
						for (int k=0;k<10;k++)
						{
//							int d2 = a*k*k*k*k + b*k*k + (k/7)*6 + (k/8)*4 -(k/9)*11;
							
							int d2 = (a*k*k*k*k + b*k*k +(k/8)*4);
							dbg[k]=d2;
							d2 %= 4000000;
							d2 %= (2050019);

							int d3 = 68 + (4+(((888*k*k*k*k + 454*k*k +(k/8)*4)%4000000)%2050019)%435)%20;
					//		d2 += (k/8)*4;
							dbg2[k]=d2;
							int amari2 = d2 % md;
							amari2 += am[0];
							amari2 += 20;
							amari2 %= 20;

							mes[k] = 77 + amari2 - 9;
							mes[k] = d3;
						}
						OutputDebugString(mes);

					}

				}
			}
		}
	}

	return 0;

	double limit = 0.00020;
	double limit1 = -limit;
	double limit2 = limit;

	double th[10];
	for (int k=42580000;k<100000000;k++)
	{
		for (int i=0;i<10;i++)
		{
			double t = (double)k;
			t *= 0.00001;

		//	th[i] = sin(t*(double)(i*i+i));
			th[i] = sin(t*(double)(exp((double)(i))-1));
		}

		double delta0 = th[0] - th[1];
		if (((delta0 >= -0.4) && (delta0 <= -0.1)) || ((delta0 >= 0.1) && (delta0 < 0.4)))
		{
			double delta = th[2] - th[3];
			if ((delta >= limit1) && (delta <= limit2))
			{
				double delta2 = th[4] - th[6];
				if ((delta2 >= limit1) && (delta2 <= limit2))
				{
					double delta3 = th[2] - th[8];
//					if ((delta3 >= limit1) && (delta3 <= limit2))
					if (1)
					{
						char mes[256];
						sprintf_s(mes,256,"%d\n",k);
						OutputDebugString(mes);

						break;
					}
				}
			}
		}
	}

	double th2[10];
	double dd = th[8]-th[2];
	dd = (3.1415926*2) / dd;
	for (int i=0;i<10;i++)
	{
		th2[i] = sin(th[i] * dd);
	}
	*/

/*
	for (int i=0;i<10;i++)
	{
	//	th[i] = exp(1.8 * (double)i) - 1.0;
		th[i] = sin(8*(double)(i*i));
		char mes[256];
		sprintf_s(mes,256,"%d\n",(int)(th[i]*1000));
		OutputDebugString(mes);
	}
*/
	 
//	double thStart = 2.140;
/*	
	for (int k=0;k<10000;k++)
	{
		double c = 12.0 * cos(thStart + 0 * th2[0]);
		int d = (int)(c+0.5);
		if (d == world[0])
		{
			char mes[256];
			sprintf_s(mes,256,"%d %d\n",k,(int)(thStart*1000));
			OutputDebugString(mes);
		}
		thStart += 0.001;

	}

	return 0;
	*/

//	double m = 1.0;
	
	/*
	for (int k=7000;k<100000000;k++)
	{
		double m = (double)k;
		m /= 100.0;
		m += 49868.638;
//		m=0;
		double w[10];

//		for (int i=0;i<10;i++)
//		{
//			w[i] = 11.65 * cos(thStart + m * th2[i] );
//		}

		for (int i=0;i<10;i++)
		{
			double c = 11.65 * cos(thStart + m * th2[i] );
			w[i] = c;
			int d = (int)(c+0.4);
			if (d != world[i])
			{
				if (i >= 6)
				{
					char mes[256];
					sprintf_s(mes,256,"* : %d %d %d\n",k,i,(int)(thStart*1000));
					mes[0] = 77+d;
					OutputDebugString(mes);
				}
				break;
			}

			if (i>=6)
			{
				char mes[256];
				sprintf_s(mes,256,"%d %d %d\n",k,i,(int)(thStart*1000));
				OutputDebugString(mes);
			}


		}
//		thStart += 0.001;
	}

	return 0;
	*/

	/*
	double th0 = 0.0;
	double w = 1.0f;

	double world2[10];

	for (int i=0;i<10;i++)
	{
			double t = i;
			t *= 0.6283;
			t += 3;
			world2[i] = world[i] - 7 * cos(t);
	}


	for (int k=0;k<100000000;k++)
	{
		BOOL flg = TRUE;


		th0 = rand() % 10000;
		w = rand() % 10000;
		th0 *= 0.001;
		w *= 0.02;
		
//		th0 = 1.322;
		int g = 0;
		for (int i=0;i<3;i++)
		{
			double t = i;
			t *= 0.6283;
			t += 3;

			double th = i*i;
//			th = cos(th);
//			th *= i;

			th *= w;
			th += th0;
//			double d = 5.0 * cos(th);
			double d = 5*cos(th);
			d += 7*cos(t);

			if ( (int)d == world[i])
			{
				int bbb = 0;
				bbb++;
			}
			else
			{
				flg = FALSE;
				break;
			}

			g += (i+1);
		}
		if (flg)
		{
			char mes[256];
			sprintf_s(mes,256,"\n\nfound!\n\n %d:%d %d\n",k,(int)(th0*10000),(int)(w*10000));
			OutputDebugString(mes);
			break;
		}
	}

	return 0;
	*/




//	float aa = NAN;
//	float bb = NaN;




/*
aaa
/* aaa */
/*
bbb
	*/

	
	//	for (int a=1,i,j;;scanf("%d",i),j=i%10,i/=10,a=a-i?a-j:a?i:j,printf("%d",a));

/*
	int* ans=new int[400000];
	int* buf=new int[400000];
	int ans_len;
	int buf_len;
	ans_len = 1;
	buf_len = 1;
	for (int i=0;i<400000;i++)
	{
		ans[i] = 0;
		buf[i] = 0;
	}

	ans[0] = 1;
	for (int i=2;i<=1000;i++)
	{
		buf_len = ans_len;
		for (int k=0;k<buf_len;k++)
		{
			buf[k] = 0;
		}

		for (int k=0;k<ans_len;k++)
		{
			int d = ans[k] * i;
			
			int keta = k;
			while (d>0)
			{
				d += buf[keta];
				buf[keta] = d % 10;
				d /= 10;
				keta++;
				if (d>0)
				{
					if (keta > buf_len-1)
					{
						buf[keta]=0;
						buf_len = keta+1;
					}
				}
			}
		}


		ans_len = buf_len;
		for (int k=0;k<buf_len;k++)
		{
			ans[k] = buf[k];
		}
	}


	for (int i=ans_len-1;i>=0;i--)
	{
		char mes[256];
		sprintf_s(mes,256,"%d",ans[i]);
		OutputDebugString(mes);
	}

	delete [] buf;
	delete [] ans;

	return 0;
*/


/*



	FILE* file0 = NULL;
	FILE* file1 = NULL;


	char oldfile[1024];
	WIN32_FIND_DATA m_findData;

	HANDLE m_handle = FindFirstFile("cdvaw\\*.vtb",&m_findData);
	while (m_handle)
	{
		char filename1[1024];
		char filename2[1024];
		sprintf_s(filename1,1024,"cdvaw\\%s",m_findData.cFileName);

		for (int i=0;i<1022;i++)
		{
			filename2[i] = filename1[i];
			if (filename2[i] == '.')
			{
				filename2[i+1] = 'v';
				filename2[i+2] = 'p';
				filename2[i+3] = 'k';
				filename2[i+4] = 0;
				break;
			}
		}

		fopen_s(&file0,filename1,"rb");
		fopen_s(&file1,filename2,"rb");

		if ((file0 != NULL) && (file1 != NULL))
		{
			OutputDebugString("\nchecking");
			OutputDebugString(filename1);
			OutputDebugString("...");
			char tmp[10];
			tmp[8] = tmp[9] = 0;
			int offset;
			while (fread(tmp,sizeof(char),8,file0) > 0)
			{
				fread(&offset,sizeof(int),1,file0);

				fseek(file1,offset,SEEK_SET);
				char data[16];
				fread(data,sizeof(char),16,file1);

				if ((data[0] != 'I') || (data[1] != 'F'))
				{
					OutputDebugString(tmp);
					char ermes[1024];
					sprintf_s(ermes,1024,"bad file=%s",oldfile);
					MessageBox(NULL,ermes,"ñ‚ëËî≠å©",MB_OK | MB_ICONEXCLAMATION);
					break;
				}

				memcpy(oldfile,tmp,10);
			}



			fclose(file0);
			fclose(file1);
		}
		else
		{
			OutputDebugString("open error");
		}


		if (FindNextFile(m_handle,&m_findData) == FALSE)
		{
			FindClose(m_handle);
			m_handle = NULL;
			break;
		}
	}


	

	MessageBox(NULL,"É`ÉFÉbÉNèIóπ","end",MB_OK);

	return 0;

*/


	CCurrentDirControl::ChangeCurrentToExe();

/*
	int m_winNTFlag = FALSE;
	OSVERSIONINFO osv;
	ZeroMemory(&osv,sizeof(osv));
	osv.dwOSVersionInfoSize = sizeof(osv);
	if (GetVersionEx(&osv))
	{
		if (osv.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			if (osv.dwMajorVersion >= 4)
			{
				m_winNTFlag = TRUE;
			}
		}
	}
*/








	//
	// ïKóvÇ»ÇÁÇ±Ç±Ç≈ëÃå±î≈ÉtÉâÉOÇ»Ç«ÇÇΩÇƒÇÈ
	//
//	CCommonGameVersion::SetTaikenVersion();
//	CCommonGameVersion::SetLoadOk();
//	CCommonGameVersion::SetSaveOk();
	CCommonGameVersion::SetAllOnOk();

	//
	// ïKóvÇ»ÇÁÇ±Ç±Ç≈ì∆é©ÇÃwheelMouse,systemfileÇçÏê¨Ç∑ÇÈ
	//
	m_game = NULL;
//	m_wheelMouse = new CWheelMouse();
//	m_systemFile = new CCommonSystemFile();
//	m_systemFile->Load(TRUE);

		





	m_mainControl = new CMainControl();	//ÉRÉìÉXÉgÉâÉNÉ^Ç≈wheelMouse,systemfileÇéwíËÇ≈Ç´ÇÈ éwíËÇ»ÇµÅAÇ‹ÇΩÇÕNULLÇæÇ∆ì‡ïîÇ≈ïWèÄÇÃÇçÏê¨Ç∑ÇÈ

	if (m_mainControl->CheckSystem())
	{
		HICON icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		HWND hWnd = m_mainControl->CreateWindowRoutine(hInstance,icon);//hinstance,icon,proc    proc == NULLÇæÇ∆CMainControlì‡ïîÇÃïWèÄcallBack

		CCommonSystemFile* lpSystemFile = m_mainControl->GetSystemFile();
		SIZE sz = m_mainControl->GetWindowSize();
		int bpp = m_mainControl->GetBpp();
		m_game = new CGame(hWnd,hInstance,lpSystemFile,sz.cx,sz.cy,bpp);
		m_mainControl->Game(m_game);
	}

	ENDDELETECLASS(m_game);
	ENDDELETECLASS(m_mainControl);

	CCurrentDirControl::RestoreCurrent();

	return 0;
}








/*_*/


