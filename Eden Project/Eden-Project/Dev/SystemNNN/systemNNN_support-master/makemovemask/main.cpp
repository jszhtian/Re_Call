//
// Config.cpp
//



#include <windows.h>
#include <stdio.h>
#include <math.h>


#include "main.h"



FILE* OpenMyFile(LPSTR name);
char* m_buffer = NULL;

void MakeAllRnd1(LPSTR name);
void MakeAllRnd2(LPSTR name);
void MakeAllRnd3(LPSTR name);

void MakeAllRnd10(LPSTR name);
void MakeAllRnd12(LPSTR name);

void MakeAllUzu1(LPSTR name);
void MakeAllUzu2(LPSTR name);
void MakeAllUzu3(LPSTR name);
void MakeAllUzu4(LPSTR name);

void MakeAllCenter1(LPSTR name);
void MakeAllCenter2(LPSTR name);

void MakeAllLine1(LPSTR name);

void MakeAllBox1(LPSTR name);
void MakeAllBox2(LPSTR name);

void MakeAllGlass1(LPSTR name);
void MakeAllGlass2(LPSTR name);

void MakeAllWave1(LPSTR name);

void MakeAllBlood(LPSTR name);


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


//	MakeAllRnd1("random1");	//->del
//	MakeAllRnd2("random2");	//->del
	MakeAllRnd3("random3");	//ok
//	MakeAllRnd10("random10");	//->del
	MakeAllRnd12("random12");	//ok

	MakeAllUzu1("uzu1");	//ok
	MakeAllUzu2("uzu2");	//ok
	MakeAllUzu3("uzu3");	//ok
	MakeAllUzu4("uzu4");	//ok

	MakeAllCenter1("center1");	//ok
	MakeAllCenter2("center2");	//ok

	MakeAllLine1("line1");	//ok

	MakeAllBox1("box1");	//ok
	MakeAllBox2("box2");	//ok

	MakeAllGlass1("glass1");	//ok
	MakeAllGlass2("glass2");	//ok

	MakeAllWave1("wave1");	//ok

	MakeAllBlood("blood");

	delete [] m_buffer;
	MessageBox(NULL,"‚¨‚í‚è‚Ü‚µ‚½‚É‚á","OK",MB_OK);
	return 0;
}


FILE* OpenMyFile(LPSTR name)
{
	char filename[256];
//	sprintf(filename,"..\\nnnEditRelease\\bmp\\bg\\bg_movemask_%s.bmp",name);
	wsprintf(filename,"bmp\\bg\\bg_movemask_%s.bmp",name);

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

		for (int i=0;i<30000;i++)
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


void MakeAllBlood(LPSTR name)
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

		for (int n=0;n<50;n++)
		{
			int xx = rand() % 1000;
			int yy = rand() % 800;
			xx -= 100;
			yy -= 100;

			int r1 = (rand() % 80) + 40;
			int r2 = (rand() % 30) + 10;

			r2 *= r1;
			r2 /= 100;

			double kt = (double)((rand() % 10) + 1);

			double pw = (double)((rand() % 30) + 10);

			int yStart = yy - (r1+r2);
			int yEnd = yy + (r1+r2);
			int xStart = xx - (r1+r2);
			int xEnd = xx + (r1+r2);

			if (yStart<0) yStart = 0;
			if (yEnd>=600) yEnd = 599;

			if (xStart<0) xStart = 0;
			if (xEnd>=800) xEnd = 799;

			if ((yEnd>=yStart) && (xEnd>=xStart))
			{
				for (int j=yStart;j<=yEnd;j++)
				{
					for (int i=xStart;i<=xEnd;i++)
					{
						int dx = i - xx;
						int dy = j - yy;

						double r = sqrt((double)(dx*dx+dy*dy));
						double th = atan2((double)dy,(double)dx);
						double th2 = th * kt;

						double r0 = r1 + r2 * sin(th2);
						if (r<=r0)
						{
							double pw2 = pw * (r0-r) / r0;

							int moveX = (int)(pw2 * cos(th) + 0.5);
							int moveY = (int)(pw2 * sin(th) + 0.5);

							int k = (i + j*800)*3;
							m_buffer[k] = 128-moveX;
							m_buffer[k+1] = 128-moveY;
							m_buffer[k+2] = 0;
						}
					}
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


		for (int i=0;i<80000;i++)
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


		for (int i=0;i<30000;i++)
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


void MakeAllWave1(LPSTR name)
{
	FILE* file = OpenMyFile(name);
	if (file != NULL)
	{
		for (int j=0;j<600;j++)
		{
			double th1 = (double)j;
			th1 /= 600.0;
			th1 *= 3.14159*2.0 * 5;


			for (int i=0;i<800;i++)
			{
				double th2 = (double)i;
	
				th2 /= 800.0;
				th2 *= 3.14149*2.0 * 7;


				int x = (int)(20.0*cos(th1) + 0.5);
				int y = (int)(13.0*sin(th2) + 0.5);

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
