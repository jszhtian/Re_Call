//
// Config.cpp
//



#include <windows.h>
#include <stdio.h>
#include <math.h>


#include "main.h"



FILE* OpenMyFile(LPSTR name);
char* m_buffer = NULL;


void MakeGinga1(LPSTR name);
void MakeGinga2(LPSTR name);

void MakeRGB1(LPSTR name);

void MakeBomb1(LPSTR name);

void MakeKeshi1(LPSTR name);

void MakeJig(LPSTR name);
void MakeJig2(LPSTR name);
void MakeJig3(LPSTR name);

void MakeJigSub(LPSTR name,int tateKosuu = 16,int yokoKosuu = 16,int pochiSize = 4);

void MakeNova(LPSTR name);

void MakeCircles(LPSTR name,int n);
void MakeCirclesSub(int blockSizeX,int blockSizeY,int x,int y);

int m_houkou[]=
{
	0,-1,1,0,0,1,-1,0
};

int m_houkou2[]=
{
	1,-1, 1,1, -1,1, -1,-1
};

int WINAPI WinMain(  HINSTANCE hInstance,   HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nCmdShow)
{
	m_buffer = new char[800*600*3];

	CreateDirectory("bmp",NULL);
	CreateDirectory("bmp\\bg",NULL);

	MakeCircles("circle36",6);
	MakeCircles("circle25",5);
	MakeCircles("circle16",4);
	MakeCircles("circle9",3);
//MessageBox(NULL,"‚¨‚í‚è‚Ü‚µ‚½‚É‚á","OK",MB_OK);
//delete [] m_buffer;
//return 0;


	MakeGinga1("ginga1");	//ok
	MakeGinga2("ginga2");	//ok

	MakeRGB1("rgb1");		//ok

	MakeBomb1("bomb1");		//ok
	MakeKeshi1("keshi1");	//ok

	MakeJig("jig");			//ok
	MakeJig2("jig2");		//ok
	MakeJig3("jig3");		//ok

	MakeNova("nova");		//ok

	MessageBox(NULL,"‚¨‚í‚è‚Ü‚µ‚½‚É‚á","OK",MB_OK);


	delete [] m_buffer;
	return 0;
}


FILE* OpenMyFile(LPSTR name)
{
	char filename[256];
//	sprintf(filename,"..\\nnnEditRelease\\bmp\\bg\\bg_movemask_%s.bmp",name);
	wsprintf(filename,"bmp\\bg\\bg_wipemask_%s.bmp",name);

	FILE* file = NULL;
	fopen_s(&file,filename,"wb");
	if (file != NULL)
	{
		BITMAPFILEHEADER bmf;
		BITMAPINFOHEADER bmi;

		ZeroMemory(&bmf,sizeof(bmf));
		ZeroMemory(&bmi,sizeof(bmi));

		bmf.bfSize = sizeof(bmf) + sizeof(bmi) + 800*600*3;
		bmf.bfType = 'MB';
		bmf.bfOffBits = sizeof(bmf) + sizeof(bmi);
		fwrite(&bmf,sizeof(bmf),1,file);

		bmi.biSize = sizeof(bmi);
		bmi.biWidth = 800;
		bmi.biHeight = -600;
		bmi.biPlanes = 1;
		bmi.biBitCount = 24;

		fwrite(&bmi,sizeof(bmi),1,file);
	}

	return file;
}

void MakeGinga1(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int j=0;j<600;j++)
		{
			for (int i=0;i<800;i++)
			{
				double x = (double)i;
				double y = (double)j;

				x -= 399.5;
				y -= 299.5;

				double r = sqrt(x*x + y*y);
				double th = atan2(y,x);

				double th1 = th*2.0 + r * 3.14159/100.0;

				double dBase = (cos(th1) + 1.0) / 2.0;
				dBase *= 80.0;

				double dBase2 = -r / 500.0;
				dBase2 += 1.0;
				if (dBase2<0) dBase2 = 0.0;
				if (dBase2>1.0) dBase2 = 1.0;

				dBase2 *= 160.0;

				int d = (int)(dBase + dBase2 + 0.5);
				if (d<0) d = 0;
				if (d>255) d = 255;


				int k = i + j*800;
				char c = (char)d;

				m_buffer[k*3+0] = c;
				m_buffer[k*3+1] = c;
				m_buffer[k*3+2] = c;





			}
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}

void MakeGinga2(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int j=0;j<600;j++)
		{
			for (int i=0;i<800;i++)
			{
				double x = (double)i;
				double y = (double)j;

				x -= 399.5;
				y -= 299.5;

				double r = sqrt(x*x + y*y);
				double th = atan2(y,x);

				double th1 = th*3.0 + r * 3.14159/100.0;

				double dBase = (cos(th1) + 1.0) / 2.0;
				if (dBase<0.5)
				{
					dBase = 0.0;
				}
				else
				{
					dBase = 1.0;
				}

				dBase *= 120.0;

				double dBase2 = -r / 500.0;
				dBase2 += 1.0;
				if (dBase2<0) dBase2 = 0.0;
				if (dBase2>1.0) dBase2 = 1.0;

				dBase2 *= 120.0;

				int d = (int)(dBase + dBase2 + 0.5);
				if (d<0) d = 0;
				if (d>255) d = 255;


				int k = i + j*800;
				char c = (char)d;

				m_buffer[k*3+0] = c;
				m_buffer[k*3+1] = c;
				m_buffer[k*3+2] = c;





			}
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}


void MakeRGB1(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{

		for (int j=0;j<600;j++)
		{
			for (int i=0;i<800;i++)
			{
				int dx1 = i-400;
				int dy1 = j-200;

				int dx2 = i-(400-173);
				int dy2 = j-400;

				int dx3 = i-(400+173);
				int dy3 = j-400;

				int r1 = (int)(sqrt((double)(dx1*dx1+dy1*dy1)) * 0.5);
				int r2 = (int)(sqrt((double)(dx2*dx2+dy2*dy2)) * 0.5);
				int r3 = (int)(sqrt((double)(dx3*dx3+dy3*dy3)) * 0.5);

				r1 = 255 - r1;
				r2 = 255 - r2;
				r3 = 255 - r3;

				if (r1<0) r1 = 0;
				if (r2<0) r2 = 0;
				if (r3<0) r3 = 0;

				if (r1>255) r1 = 255;
				if (r2>255) r2 = 255;
				if (r3>255) r3 = 255;

				int k = i + j*800;
				m_buffer[k*3+0] = (char)r3;
				m_buffer[k*3+1] = (char)r2;
				m_buffer[k*3+2] = (char)r1;
			}
		}
		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}


void MakeBomb1(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		ZeroMemory(m_buffer,800*600*3);

		for (int n=1;n<500;n++)
		{
			int cx = rand() % 800;
			int cy = rand() % 600;
			int rr = (rand() % 50) + 30;

			int c = (rand() % 200) + 50;

			for (int j=-rr;j<rr;j++)
			{
				int y = cy + j;
				if ((y>=0) && (y<600))
				{
					for (int i=-rr;i<rr;i++)
					{
						int x = cx + i;
						if ((x>=0) && (x<800))
						{
							int r = (int)(sqrt((double)(i*i+j*j)));
							if (r<=rr)
							{
								int col = c - r;
								if (col<0) col = 0;
								if (col>255) col = 255;

								int k=x+y*800;

								if (col >= (((int)(m_buffer[k*3+0])) & 0xff))
								{
									m_buffer[k*3+0] = (char)col;
									m_buffer[k*3+1] = (char)col;
									m_buffer[k*3+2] = (char)col;
								}
							}
						}
					}
				}
			}
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}


void MakeJig(LPSTR name)
{
	MakeJigSub(name,16,16,4);
}

void MakeJig2(LPSTR name)
{
	MakeJigSub(name,10,10,6);
}

void MakeJig3(LPSTR name)
{
	MakeJigSub(name,8,8,8);
}

void MakeJigSub(LPSTR name,int tateKosuu,int yokoKosuu,int pochiSize)
{
	int tate = tateKosuu;
	int yoko = yokoKosuu;
	int rr = pochiSize;

	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		ZeroMemory(m_buffer,800*600*3);

		int* colors = new int[yoko*tate];
		for (int n=0;n<yoko*tate;n++)
		{
			int c = n;
			if (yoko*tate<256)
			{
				c *= 256;
				c /= (yoko*tate);
			}

			colors[n] = c % 256;
		}

		//shuffle
		for (int n=0;n<yoko*tate;n++)
		{
			int r = rand() % (yoko*tate);
			int c1 = colors[n];
			colors[n] = colors[r];
			colors[r] = c1;
		}


		int** yokoTable = new int*[yoko+1];
		int** tateTable = new int*[tate+1];

		for (int n=0;n<yoko+1;n++)
		{
			yokoTable[n] = new int[600];
		}

		for (int n=0;n<tate+1;n++)
		{
			tateTable[n] = new int[800];
		}


		for (int n=0;n<800;n++)
		{
			tateTable[0][n] = 0;
			tateTable[tate][n] = 599;
		}

		for (int n=0;n<600;n++)
		{
			yokoTable[0][n] = 0;
			yokoTable[yoko][n] = 799;
		}




		for (int nx=1;nx<yoko;nx++)
		{
			double r1 = (double)((rand() % 2) + 2);
			double r2 = (double)((rand() % 2) + 1);
			double w1 = double((rand() % 6) + 3);
			double w2 = double((rand() % 4) + 1);
			w1 *= 3.14159*2 / 360.0;
			w2 *= 3.14159*2 / 360.0;
			double fay1 = (double)(rand() % 360);
			double fay2 = (double)(rand() % 360);
			fay1 *= 3.14159*2 / 360.0;
			fay2 *= 3.14159*2 / 360.0;


			int lineX = (nx * 800) / yoko;
			for (int n=0;n<600;n++)
			{
				double th1 = (double)n;
				double th2 = (double)n;
				th1 *= w1;
				th2 *= w2;
				th1 += fay1;
				th2 += fay2;
				double dd = r1*cos(th1) + r2*cos(th2);
				int d = (int)(dd+0.5);

				d += lineX;
				if (d<0) d = 0;
				if (d>=800) d = 799;

				yokoTable[nx][n] = d;
			}
		}




		for (int ny=1;ny<tate;ny++)
		{
			double r1 = (double)((rand() % 2) + 2);
			double r2 = (double)((rand() % 2) + 1);
			double w1 = double((rand() % 6) + 3);
			double w2 = double((rand() % 4) + 1);
			w1 *= 3.14159*2 / 360.0;
			w2 *= 3.14159*2 / 360.0;
			double fay1 = (double)(rand() % 360);
			double fay2 = (double)(rand() % 360);
			fay1 *= 3.14159*2 / 360.0;
			fay2 *= 3.14159*2 / 360.0;

			int lineY = (ny * 600) / tate;

			for (int n=0;n<800;n++)
			{
				double th1 = (double)n;
				double th2 = (double)n;
				th1 *= w1;
				th2 *= w2;
				th1 += fay1;
				th2 += fay2;
				double dd = r1*cos(th1) + r2*cos(th2);
				int d = (int)(dd+0.5);

				d += lineY;
				if (d<0) d = 0;
				if (d>=600) d = 599;

				tateTable[ny][n] = d;

			}
		}







		for (int j=0;j<tate;j++)
		{
			int yStart = ((j-1) * 600) / tate;
			int yEnd = ((j+2) * 600) / tate;

			for (int i=0;i<yoko;i++)
			{
				int xStart = ((i-1) * 800) / yoko;
				int xEnd = ((i+2) * 800) / yoko;

				char cc = (char)(colors[i+j*yoko]);

				for (int y=yStart;y<=yEnd;y++)
				{
					if ((y<0) || (y>=600)) continue;

					for (int x=xStart;x<=xEnd;x++)
					{
						if ((x<0) || (x>=800)) continue;

						int checkX1 = yokoTable[i][y];
						int checkX2 = yokoTable[i+1][y];

						if ((x>=checkX1) && (x<=checkX2))
						{
							int checkY1 = tateTable[j][x];
							int checkY2 = tateTable[j+1][x];
	
							if ((y>=checkY1) && (y<=checkY2))
							{
								int k = (x + y * 800) * 3;
								m_buffer[k] = cc;
								m_buffer[k+1] = cc;
								m_buffer[k+2] = cc;
							}
						}
					}
				}
			}
		}



		for (int j=0;j<tate;j++)
		{
			int centerY = (j * 600) / tate;
			centerY += (600/(tate*2));

			for (int i=1;i<yoko;i++)
			{
				int xx = i;
				int dx = -1;
				if ((rand() % 100) < 50)
				{
					xx -= 1;
					dx = 1;
				}

				char cc = (char)(colors[xx+j*yoko]);
				int centerX = yokoTable[i][centerY];

				centerX += dx * (rr-2);

				for (int jj=-rr;jj<=rr;jj++)
				{
					for (int ii=-rr;ii<=rr;ii++)
					{
						if (ii*ii+jj*jj<=rr*rr)
						{
							int x = centerX + ii;
							int y = centerY + jj;
							if ((x>=0) && (x<800) && (y>=0) && (y<600))
							{
								int k = (x + y * 800) * 3;
								m_buffer[k] = cc;
								m_buffer[k+1] = cc;
								m_buffer[k+2] = cc;
							}
						}
					}
				}
			}
		}

		for (int i=0;i<yoko;i++)
		{
			int centerX = (i * 800) / yoko;
			centerX += (800/(yoko*2));

			for (int j=1;j<tate;j++)
			{
				int yy = j;
				int dy = -1;
				if ((rand() % 100) < 50)
				{
					yy -= 1;
					dy = 1;
				}

				char cc = (char)(colors[i+yy*yoko]);
				int centerY = tateTable[j][centerX];

				centerY += dy * (rr-2);

				for (int jj=-rr;jj<=rr;jj++)
				{
					for (int ii=-rr;ii<=rr;ii++)
					{
						if (ii*ii+jj*jj<=rr*rr)
						{
							int x = centerX + ii;
							int y = centerY + jj;
							if ((x>=0) && (x<800) && (y>=0) && (y<600))
							{
								int k = (x + y * 800) * 3;
								m_buffer[k] = cc;
								m_buffer[k+1] = cc;
								m_buffer[k+2] = cc;
							}
						}
					}
				}
			}
		}



		for (int n=0;n<tate;n++)
		{
			delete [] tateTable[n];
		}

		for (int n=0;n<yoko;n++)
		{
			delete [] yokoTable[n];
		}

		delete [] tateTable;
		delete [] yokoTable;

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}


void MakeNova(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		ZeroMemory(m_buffer,800*600*3);

		for (int n=1;n<1000;n++)
		{
			double th = (double)(rand() % 30000);
			th *= 3.14159*2 / 30000;

			int c1 = (rand() % 192) + 64;
			int c2 = (rand() % 64);

			int rr = (rand() % 600) + 100;

			for (int r=0;r<rr;r++)
			{
				int c = c1 + ((c2-c1) * r) / rr;
				char cc = (char)c;

				int xx = 400 + (int)(cos(th) * (double)r);
				int yy = 300 + (int)(sin(th) * (double)r);

				if ((xx>=1) && (xx<800-1) && (yy>=1) && (yy<600-1))
				{
					for (int dy=-1;dy<=1;dy++)
					{
						int y = yy + dy;
						for (int dx=-1;dx<=1;dx++)
						{
							int x = xx + dx;
							int k = (x + y*800)*3;
							char c2 = m_buffer[k];
							int ci = (int)c2;
							ci &= 0xff;
							if (c > ci)
							{
								m_buffer[k] = cc;
								m_buffer[k+1] = cc;
								m_buffer[k+2] = cc;
							}
						}
					}
				}
			}
		}



		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}

void MakeCircles(LPSTR name,int n)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		ZeroMemory(m_buffer,800*600*3);

		int blockSizeX = 800/n;
		int blockSizeY = 600/n;
		if ((blockSizeX * n) < 800) blockSizeX++;
		if ((blockSizeY * n) < 600) blockSizeY++;

		for (int y=0;y<n;y++)
		{
			for (int x=0;x<n;x++)
			{
				MakeCirclesSub(blockSizeX,blockSizeY,x,y);
			}
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}

void MakeCirclesSub(int blockSizeX,int blockSizeY,int x,int y)
{
	int rx = blockSizeX / 2;
	int ry = blockSizeY / 2;
	int rr = (int)sqrt((double)(rx*rx + ry*ry));
	rr += 2;

	for (int k=0;k<255;k++)
	{
		int c = 255-k;
		int r = rr * k / 255;

		int yStart = ry - r;
		int yEnd = ry + r;
		if (yStart < 0) yStart = 0;
		if (yEnd >= blockSizeY) yEnd = blockSizeY-1;

		char cc = (char)c;

		for (int j=yStart;j<=yEnd;j++)
		{
			int yy = j - ry;
			int dx = (int)sqrt((double)(r*r - yy*yy));
			int xStart = rx - dx;
			int xEnd = rx + dx;

			if (xStart < 0) xStart = 0;
			if (xEnd >= blockSizeX) xEnd = blockSizeX - 1;

			int pointY = blockSizeY * y + j;

			for (int i=xStart;i<=xEnd;i++)
			{
				int pointX = blockSizeX * x + i;

				if (m_buffer[3*(800*pointY+pointX)] == 0)
				{
					m_buffer[3*(800*pointY+pointX) + 0]=cc;
					m_buffer[3*(800*pointY+pointX) + 1]=cc;
					m_buffer[3*(800*pointY+pointX) + 2]=cc;
				}
			}
		}
	}


}

int m_keshiTable[]=
{
	38,155,1,1,

	143,101,20,4,
	83,276,20,4,
	63,423,20,4,
	70,558,20,4,
	143,316,20,4,
	221,153,20,4,
	197,404,20,4,
	208,557,20,4,
	290,324,20,4,
	336,99,20,4,
	366,346,20,4,
	368,508,20,4,
	449,253,20,4,
	514,94,20,4,
	490,343,20,4,
	494,557,20,4,
	549,381,20,4,
	602,194,20,4,
	663,65,20,4,
	654,318,20,4,
	613,483,20,4,
	737,393,20,4,

	20,30,40,15,
	700,40,30,7,
	130,64,30,7,
	720,95,30,7,
	10,115,35,7,
	630,165,30,7,
	25,190,30,7,
	710,220,30,7,
	110,260,30,7,
	660,290,30,7,
	20,340,30,7,
	710,360,30,7,
	180,395,30,7,
	640,420,30,7,
	10,455,30,7,
	720,480,30,7,
	120,510,30,7,
	740,540,30,5,

	0,540,40,9,
	0,0,40,9,
	740,5,40,9,
	740,540,40,9,



	-9999,-9999,1,
};

void MakeKeshi1(LPSTR name)
{
	int maskTable[60*60];


	int sizeX = 60;
	int sizeY = 60;

	for (int my=0;my<sizeY;my++)
	{
		for (int mx=0;mx<sizeX;mx++)
		{
			if ((mx-sizeX/2)*(mx-sizeX/2)+(my-sizeY/2)*(my-sizeY/2)<sizeX*sizeY/2/2)
			{
				maskTable[mx+my*sizeX] = rand() & 0x7fff;
			}
			else
			{
				maskTable[mx+my*sizeX] = 0;
			}
		}
	}


	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		ZeroMemory(m_buffer,800*600*3);

		int fromX = m_keshiTable[0];
		int fromY = m_keshiTable[1];

		int col0 = 255;

		for (int n=1;n<100;n++)
		{
			int nextX = m_keshiTable[n*4];
			int nextY = m_keshiTable[n*4+1];
			int dv = m_keshiTable[n*4+2];
			int colSub = m_keshiTable[n*4+3];
			if (nextX == -9999) break;


			for (int k=0;k<dv;k++)
			{
				int xx = fromX + ((nextX-fromX)*k) / dv;
				int yy = fromY + ((nextY-fromY)*k) / dv;
				int col = col0 - (colSub*k) / dv;
				if (col<0) col = 0;

				xx += (rand() % 5);
				yy += (rand() % 5);

				for (int j=0;j<sizeY;j++)
				{
					int y = yy+j;
					if ((y>=0) && (y<600))
					{
						for (int i=0;i<sizeX;i++)
						{
							int x = xx+i;
							if ((x>=0) && (x<800))
							{
								if (maskTable[i+j*sizeX] != 0)
								{
									if (((maskTable[i+j*sizeX] + col+k+x+y) & 0x7) < 6)
									{
										int k = x+y*800;
										if (m_buffer[k*3] == 0)
										{
											m_buffer[k*3+0] = (char)col;
											m_buffer[k*3+1] = (char)col;
											m_buffer[k*3+2] = (char)col;
										}
									}
								}
							}
						}
					}
				}

			}



			col0 -= colSub;
			if (col0 < 0) col0 = 0;

			fromX = nextX;
			fromY = nextY;
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}


/*
void MakeAllRnd1(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int i=0;i<800*600;i++)
		{
			int x = (rand() % 255) + 1;
			int y = (rand() % 255) + 1;
			m_buffer[i*3+0] = (char)x;
			m_buffer[i*3+1] = (char)y;
			m_buffer[i*3+2] = (char)0;
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}

void MakeAllRnd2(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int i=0;i<800*600;i++)
		{
			double r = (double)((rand() % 64) + 62);
			double th = (double)(rand() % 360);

		//	r = 122.0;

			th /= 360.0;
			th *= 3.14159*2;



			int x = (int)(r*cos(th) + 0.5);
			int y = (int)(r*sin(th) + 0.5);

			x += 128;
			y += 128;


			m_buffer[i*3+0] = (char)x;
			m_buffer[i*3+1] = (char)y;
			m_buffer[i*3+2] = (char)0;
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}


void MakeAllRnd3(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int i=0;i<800*600;i++)
		{
			double r = (double)((rand() % 28) + 4);
			double th = (double)(rand() % 360);

		//	r = 122.0;

			th /= 360.0;
			th *= 3.14159*2;



			int x = (int)(r*cos(th) + 0.5);
			int y = (int)(r*sin(th) + 0.5);

			x += 128;
			y += 128;


			m_buffer[i*3+0] = (char)x;
			m_buffer[i*3+1] = (char)y;
			m_buffer[i*3+2] = (char)0;
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}


void MakeAllRnd10(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int j=0;j<150;j++)
		{
			for (int i=0;i<200;i++)
			{
				double r = (double)((rand() % 34) + 32);
				double th = (double)(rand() % 360);

				th /= 360.0;
				th *= 3.14159*2;



				int x = (int)(r*cos(th) + 0.5);
				int y = (int)(r*sin(th) + 0.5);

				x += 128;
				y += 128;


				for (int jj=0;jj<4;jj++)
				{
					for (int ii=0;ii<4;ii++)
					{

						int k = i*4+ii + (j*4+jj) * 800;

						m_buffer[k*3+0] = (char)x;
						m_buffer[k*3+1] = (char)y;
						m_buffer[k*3+2] = (char)0;
					}
				}
			}
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}

void MakeAllRnd12(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int j=0;j<150;j++)
		{
			for (int i=0;i<200;i++)
			{
				double r = (double)((rand() % 21) + 10);
				double th = (double)(rand() % 360);

				th /= 360.0;
				th *= 3.14159*2;



				int x = (int)(r*cos(th) + 0.5);
				int y = (int)(r*sin(th) + 0.5);

				x += 128;
				y += 128;


				for (int jj=0;jj<4;jj++)
				{
					for (int ii=0;ii<4;ii++)
					{

						int k = i*4+ii + (j*4+jj) * 800;

						m_buffer[k*3+0] = (char)x;
						m_buffer[k*3+1] = (char)y;
						m_buffer[k*3+2] = (char)0;
					}
				}
			}
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}

void MakeAllUzu1(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int j=0;j<600;j++)
		{
			for (int i=0;i<800;i++)
			{
				double dx = (double)(i - 400);
				double dy = (double)(j - 300);

				double r = (sqrt(dx*dx + dy*dy));

				if (r>120.0) r = 120.0;

				double th = atan2(dy,dx);
				th += 3.14159*0.5;

				int x = (int)(r*cos(th) + 0.5);
				int y = (int)(r*sin(th) + 0.5);

				x += 128;
				y += 128;


				int k = i + j * 800;

				m_buffer[k*3+0] = (char)x;
				m_buffer[k*3+1] = (char)y;
				m_buffer[k*3+2] = (char)0;
			}
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}


void MakeAllUzu2(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int j=0;j<600;j++)
		{
			for (int i=0;i<800;i++)
			{
				double dx = (double)(i - 400);
				double dy = (double)(j - 300);

				double r = (sqrt(dx*dx + dy*dy));


				double r2 = 120.0 * 40.0 / (r + 20.0);

				if (r2 > 120.0) r2 = 120.0;


				r2 *= cos(r * 0.1);

				r2 *= 0.3;

//				if (r2 >  120.0) r2 =  120.0;
//				if (r2 < -120.0) r2 = -120.0;


				double th = atan2(dy,dx);
				th += 3.14159*0.5;

				int x = (int)(r2*cos(th) + 0.5);
				int y = (int)(r2*sin(th) + 0.5);

				x += 128;
				y += 128;


				int k = i + j * 800;

				m_buffer[k*3+0] = (char)x;
				m_buffer[k*3+1] = (char)y;
				m_buffer[k*3+2] = (char)0;
			}
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}
*/


/*

void MakeAllUzu3(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int j=0;j<600;j++)
		{
			for (int i=0;i<800;i++)
			{
				double dx = (double)(i - 400);
				double dy = (double)(j - 300);

				double r = (sqrt(dx*dx + dy*dy));

				double r2 = r;

				if (r2 > 120.0) r2 = 120.0;


				r2 *= cos(r * 0.1);

				r2 *= 0.3;

//				if (r2 >  120.0) r2 =  120.0;
//				if (r2 < -120.0) r2 = -120.0;


				double th = atan2(dy,dx);
				th += 3.14159*0.5;

				int x = (int)(r2*cos(th) + 0.5);
				int y = (int)(r2*sin(th) + 0.5);

				x += 128;
				y += 128;


				int k = i + j * 800;

				m_buffer[k*3+0] = (char)x;
				m_buffer[k*3+1] = (char)y;
				m_buffer[k*3+2] = (char)0;
			}
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}


void MakeAllUzu4(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int j=0;j<600;j++)
		{
			for (int i=0;i<800;i++)
			{
				double dx = (double)(i - 400);
				double dy = (double)(j - 300);

				double r = (sqrt(dx*dx + dy*dy));

				double r2 = r;

				r2 *= 0.5;

				if (r2 > 40.0) r2 = 40.0;


				r2 *= cos(r * 0.03);

//				if (r2 >  120.0) r2 =  120.0;
//				if (r2 < -120.0) r2 = -120.0;


				double th = atan2(dy,dx);
				th += 3.14159*0.5;

				int x = (int)(r2*cos(th) + 0.5);
				int y = (int)(r2*sin(th) + 0.5);

				x += 128;
				y += 128;


				int k = i + j * 800;

				m_buffer[k*3+0] = (char)x;
				m_buffer[k*3+1] = (char)y;
				m_buffer[k*3+2] = (char)0;
			}
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}



void MakeAllCenter1(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int j=0;j<600;j++)
		{
			for (int i=0;i<800;i++)
			{
				double dx = (double)(i - 400);
				double dy = (double)(j - 300);

				double r = (sqrt(dx*dx + dy*dy));

				r += 100.0;
				r = 100.0*40.0 / r;

				if (r>120.0) r = 120.0;

				if ((j==300) && (i==400))
				{
					r = 0.0;
				}

				double th = atan2(dy,dx);
				th += 3.14159*1.0;

				int x = (int)(r*cos(th) + 0.5);
				int y = (int)(r*sin(th) + 0.5);

				x += 128;
				y += 128;


				int k = i + j * 800;

				m_buffer[k*3+0] = (char)x;
				m_buffer[k*3+1] = (char)y;
				m_buffer[k*3+2] = (char)0;
			}
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}

void MakeAllCenter2(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int j=0;j<600;j++)
		{
			for (int i=0;i<800;i++)
			{
				double dx = (double)i;
				double dy = (double)j;
				dx -= 399.5;
				dy -= 299.5;


				double r = (sqrt(dx*dx + dy*dy));

			//	r += 100.0;
			//	r = 100.0*40.0 / r;

				r *= 0.1;

				if (r>120.0) r = 120.0;


				double th = atan2(dy,dx);
				th += 3.14159*1.0;

				double rndTh = (double)((rand() % 60) - 30);
				rndTh /= 360.0;
				rndTh *= 3.14159*2.0;

				th += rndTh;

				int x = (int)(r*cos(th) + 0.5);
				int y = (int)(r*sin(th) + 0.5);

				x += 128;
				y += 128;


				int k = i + j * 800;

				m_buffer[k*3+0] = (char)x;
				m_buffer[k*3+1] = (char)y;
				m_buffer[k*3+2] = (char)0;
			}
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}

*/


/*

void MakeAllLine1(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int i=0;i<800*600;i++)
		{
			m_buffer[i*3+0] = (char)128;
			m_buffer[i*3+1] = (char)128;
			m_buffer[i*3+2] = (char)0;
		}

		for (i=0;i<30000;i++)
		{
			int x = rand() % 799;
			int y = rand() % 597;
			int dx = (rand() % 20) + 8;
			if ((x+dx) > 800) dx = 800-x;

			int speedX = rand() % 40 - 20;
			int speedY = 0;
			
			speedX += 128;
			speedY += 128;

			for (int jj=0;jj<4;jj++)
			{
				for (int ii=0;ii<dx;ii++)
				{
					int k = x+ii + (y+jj)*800;
					m_buffer[k*3+0] = (char)speedX;
					m_buffer[k*3+1] = (char)speedY;
					m_buffer[k*3+2] = (char)0;
				}
			}
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}



void MakeAllGlass1(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int i=0;i<800*600;i++)
		{
			m_buffer[i*3+0] = (char)128;
			m_buffer[i*3+1] = (char)128;
			m_buffer[i*3+2] = (char)0;
		}


		for (i=0;i<80000;i++)
		{
			int x = rand() % 800;
			int y = rand() % 597;

			int h1 = 0;
			
			int gx = x - 400;
			int gy = y - 300;

			if (-gy < gx)
			{
				h1 = 1;
			}

			int h2 = 0;
			if (gy > gx)
			{
				h2 = 1;
			}

			int h;

			if ((rand() % 100) < 50)
			{
				h = 3 - h1*2;
			}
			else
			{
				h = h2*2;
			}


			int hx = m_houkou2[h*2];
			int hy = m_houkou2[h*2+1];

			int loops = (rand() % 30) + 20;

			int mul = 6+(rand() % 12);

			int speedX = -(hx * mul);
			int speedY = -(hy * mul);

			speedX += 128;
			speedY += 128;

			if ((rand() % 100) < 10)
			{
				hx *= -1;
				hy *= -1;
			}

			for (int n=0;n<loops;n++)
			{
				for (int p=0;p<4;p++)
				{
					int k = x+(y+p)*800;

					m_buffer[k*3+0] = (char)speedX;
					m_buffer[k*3+1] = (char)speedY;
					m_buffer[k*3+2] = (char)0;
				}

				x -= hx;
				y -= hy;
				if ((x<0) || (x>=800) || (y<0) || (y>=597))
				{
					break;
				}
			}
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}


void MakeAllGlass2(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int i=0;i<800*600;i++)
		{
			m_buffer[i*3+0] = (char)128;
			m_buffer[i*3+1] = (char)128;
			m_buffer[i*3+2] = (char)0;
		}


		for (i=0;i<30000;i++)
		{
			int x = rand() % 800;
			int y = rand() % 591;

			int h1 = 0;
			
			int gx = x - 400;
			int gy = y - 300;

			if (-gy < gx)
			{
				h1 = 1;
			}

			int h2 = 0;
			if (gy > gx)
			{
				h2 = 1;
			}

			int h;

			if ((rand() % 100) < 50)
			{
				h = 3 - h1*2;
			}
			else
			{
				h = h2*2;
			}


			int hx = m_houkou2[h*2];
			int hy = m_houkou2[h*2+1];

			int loops = (rand() % 30) + 20;

			int mul = 6+(rand() % 12);

			int speedX = -(hx * mul);
			int speedY = -(hy * mul);

			speedX += 128;
			speedY += 128;

			if ((rand() % 100) < 10)
			{
				hx *= -1;
				hy *= -1;
			}

			for (int n=0;n<loops;n++)
			{
				for (int p=0;p<10;p++)
				{
					int k = x+(y+p)*800;

					m_buffer[k*3+0] = (char)speedX;
					m_buffer[k*3+1] = (char)speedY;
					m_buffer[k*3+2] = (char)0;
				}

		//		if ((n % 2) == 0)
				if (0)
				{
					if (speedX<128) speedX++;
					if (speedX>128) speedX--;
					if (speedY<128) speedY++;
					if (speedY>128) speedY--;
				}

				x -= hx;
				y -= hy;
				if ((x<0) || (x>=800) || (y<0) || (y>=591))
				{
					break;
				}
			}
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}

*/


/*

void MakeAllBox1(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int j=0;j<60;j++)
		{
			for (int i=0;i<80;i++)
			{
				int h = rand() % 4;
				int dx = m_houkou[h*2];
				int dy = m_houkou[h*2+1];

				dx *= (rand() % 10 + 6);
				dy *= (rand() % 10 + 6);

				dx += 128;
				dy += 128;

				for (int jj=0;jj<10;jj++)
				{
					for (int ii=0;ii<10;ii++)
					{

						int k = i*10+ii + (j*10+jj)*800;
						m_buffer[k*3+0] = (char)dx;
						m_buffer[k*3+1] = (char)dy;
						m_buffer[k*3+2] = (char)0;
					}
				}
			}
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}


void MakeAllBox2(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int j=0;j<15;j++)
		{
			for (int i=0;i<20;i++)
			{
				int h = rand() % 4;
				int dx = m_houkou[h*2];
				int dy = m_houkou[h*2+1];

				dx *= (rand() % 10 + 6);
				dy *= (rand() % 10 + 6);

				dx += 128;
				dy += 128;

				for (int jj=0;jj<40;jj++)
				{
					for (int ii=0;ii<40;ii++)
					{

						int k = i*40+ii + (j*40+jj)*800;
						m_buffer[k*3+0] = (char)dx;
						m_buffer[k*3+1] = (char)dy;
						m_buffer[k*3+2] = (char)0;
					}
				}
			}
		}

		fwrite(m_buffer,sizeof(char),800*600*3,file);
		fclose(file);
	}
}
*/


