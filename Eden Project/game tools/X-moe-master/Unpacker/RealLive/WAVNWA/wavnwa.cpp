#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
static char wavheader[0x2c] = {
		'R','I','F','F',
		0x00,0x00,0x00,0x00, /* +0x04: riff size*/
		'W','A','V','E',
		'f','m','t',' ',
		0x10,0x00,0x00,0x00, /* +0x10 : fmt size=0x10 */
		0x01, 0x00,    /* +0x14 : tag : pcm = 1 */
		0x02, 0x00,    /* +0x16 : channels */
		0x00,0x00,0x00,0x00, /* +0x18 : samples per second */
		0x00,0x00,0x00,0x00, /* +0x1c : average bytes per second */
		0x00,0x00,     /* +0x20 : block alignment */
		0x00,0x00,     /* +0x22 : bits per sample */
		'd','a','t','a',
		0x00,0x00,0x00,0x00	/* +0x28 : data size */
};
static char nwaheader[0x2c] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
int fileflag=-1;			//用于区分文件类型;
char filename[32];
inline int write_little_endian_int(char* buf, int number) {
	int c = *(int*)buf; *(int*)buf = number; return c;
}
inline int write_little_endian_short(char* buf, int number) {
	int c = *(short*)buf; *(short*)buf = number; return c;
}
int filecheck(FILE *fp){
	long int filehead=0;
	filehead=readfile(fp,8);
	if(filehead==0x45564157)
		fileflag=0;     //为wav文件
	else
		if(filehead==0xFFFFFFFF)
			fileflag=1;		//为clannad类型的nwa文件
		else{
			printf("file format not supported now!\n");
			exit(0);		//文件内容有问题，不支持
		}
	return 0;
}
int buildWAVhead(FILE *infile){
	int channelnumber,sample_bits,sample_frequence,datalen;
	channelnumber=readfile_short(infile,0);
	sample_bits=readfile_short(infile,2);
	sample_frequence=readfile(infile,0x04);
	datalen=readfile(infile,0x14);
	printf("NWA file detail:\n");
	printf("channel(s): %d      sample rate: %d      \nsample frequence: %d      data length: %d\n",channelnumber,sample_bits,sample_frequence,datalen);
	write_little_endian_int(wavheader+0x04, datalen+0x24);					//riff大小，endian格式，定义为0x24+数据部分长度
	write_little_endian_short(wavheader+0x16, channelnumber);				//endian格式，表示声道数目
	write_little_endian_int(wavheader+0x18, sample_frequence);			//音频采样频率，endian方式
	write_little_endian_short(wavheader+0x22, sample_bits);					//endian格式，表示音频采样位数
	write_little_endian_int(wavheader+0x28, datalen);								//endian格式，表示数据部分大小
	int byps = (sample_bits+7)>>3;
	write_little_endian_int(wavheader+0x1c, sample_frequence*byps*channelnumber);			//endian格式，平均每秒的byte数目
	write_little_endian_short(wavheader+0x20, byps*channelnumber);										//endian格式，block anlignment=声道数*采样位数/8
	return 1;
}
int buildNWAhead(FILE *infile){
	int channelnumber,sample_bits,sample_frequence,datalen;
	channelnumber=readfile_short(infile,0x16);
	datalen=readfile(infile,0x04)-0x24;
	sample_frequence=readfile(infile,0x18);
	sample_bits=readfile_short(infile,0x22);
	printf("WAV file detail:\n");
	printf("channel(s): %d      sample rate: %d      \nsample frequence: %d      data length: %d\n",channelnumber,sample_bits,sample_frequence,datalen);
	write_little_endian_short(nwaheader,channelnumber);							//endian格式，表示声道数目
	write_little_endian_short(nwaheader+0x02,sample_bits);					//endian格式，表示音频采样位数
	write_little_endian_int(nwaheader+0x04,sample_frequence);				//endian格式，表示音频采样频率
	write_little_endian_int(nwaheader+0x14,datalen);								//endian格式，表示数据长度
	write_little_endian_int(nwaheader+0x1c,datalen/2);							//endian格式，samplecont=数据长度/2(猜测)
	return 1;
}
int output_data(FILE *fp,FILE *fout){
	fseek(fp,0,SEEK_END);
	int length=ftell(fp)-0x2c;
	unsigned char *dat;
	fseek(fp,0x2c,SEEK_SET);
	dat=new unsigned char[length];
	fread(dat,length,1,fp);
	if(fileflag==0)
		fwrite(nwaheader, 1, 0x2c, fout);
	if(fileflag==1)
		fwrite(wavheader, 1, 0x2c, fout);
	fwrite(dat, 1, length, fout);
	delete []dat;
	return length;
}
int create_filename(int flag,char* ls){
	sscanf(ls,"%[^.]",&filename);
	if(flag==0)
		sprintf(filename,"%s%.4s",filename,".nwa");
	if(flag==1)
		sprintf(filename,"%s%.4s",filename,".wav");
	return 1;
}
int main(int argc, char* argv[]){
	programhead();
	FILE *infile,*outfile;
	int nouse;
	if((infile=fopen(argv[1],"rb"))==NULL){
		printf("Opening input file error!\n");
		exit(0);
	}
	nouse=filecheck(infile);
	if(fileflag==0)
		buildNWAhead(infile);
	if(fileflag==1)
		buildWAVhead(infile);
	nouse=create_filename(fileflag,argv[1]);
	printf("Opening output file and outputing.......\n");
	if((outfile=fopen(filename,"wb"))==NULL){
		printf("Opening output file error!\n");
		exit(0);
	}
	nouse=output_data(infile,outfile);
	fclose(infile);
	fclose(outfile);
	printf("%c output successully!\n");
	return 0;
}
