// CUFFS �ėp�@���b�Z�[�W���[�_�[
// �J�����ԁ@2007-01-18------2007-04-28 nota
//
// ���Ή��m�F�Q�[��
// ������ނ���
// �����R�ƃ����[
// �A���T���T
// 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DEBUG  0 // 0.�ʏ�o�� 1.�f�o�b�O���[�h

// �v���g�^�C�v�錾
void conv1(void);//��ʃf�R�[�h�p
void conv2(void);//��l�����O����p
void conv3(void);//�S�~�����p

// �O���[�o���Ŏg�����ߊO���錾
char version[]="2007/04/28 VER 1.3";	// �o�[�W�����A�c�[���ŏI�����
//2007/04/28 VER 1.3 �A���T���T�ɐ����Ή�
//2007/04/01 VER 1.2 ��������A���T���T�ɂ��Ή�
//2007/02/14 VER 1.1 �����R�ƃ����[���Ή�
//2007/01/19 VER 1.0 ������ނ��ѐ�p

FILE *fp,*to,*tbl;					// �t�@�C���|�C���^

fpos_t fsize = 1;//�T�C�Y�Ȃ�тɁA�ꎞ�I�ȃ|�W�V�����̋L���p
unsigned long i;				// �ėp
unsigned long i_bak;				// �ėp
unsigned long r;			// �ėp
unsigned char iGetFile;		// ���[�h�f�[�^�i�[�p
unsigned char iGetFile2;		// ���[�h�f�[�^�i�[�p

unsigned char table[65536][3];		// UNICODE to SHIFTJIS�ϊ��e�[�u���p

unsigned long rsize;				// �t�@�C���T�C�Y 
unsigned char name[100];			// �t�@�C���ǂݍ��ݖ��O 
unsigned char select_f;			// �I�����J�E���^  ��
unsigned char t[40];			// �ėp
unsigned char count_a;			// �ėp

unsigned char g_name1[50];			// �Q�[���� 
unsigned char f_name1[30];			// �t�@�C���� 
unsigned char h_name1[30];			// ��l������ 
unsigned char h_name2[30];			// ��l�����O 
unsigned char h_name3[30];			// ��l�������� 
unsigned char title_f;				// �V�[���^�C�g���t���O 
unsigned char buf[1000];			// �o�b�t�@
unsigned char seen_f;				//�Z�[�u�^�C�g���t���O
// ���C�����[�`��
int main(void)
{
	printf("�� CUFFS �ėp ���b�Z�[�W���[�_�[ ��\n%s\n",version);
	printf("------------------------------------\n");



	//�Q�[�����e�[�u���쐬
	if((tbl=fopen("setting.ini","rb"))==NULL){
		printf("���G���[\n");
		printf("�ݒ�t�@�C�� [setting.ini] ��������܂���B\n");
		printf("[cuffs_con.exe]�Ɠ����t�H���_�ɓ���Ă���ēx���s���Ă��������B\n");
		printf("�iEnter�L�[�������ƏI�����܂��j\n");
		if(!DEBUG){
			getchar();//�_�~�[
		}
		exit(1);
	}
	printf("���Q�[�����擾�isetting.ini�Q�ƒ��c�j\n");
	fseek(tbl,0,SEEK_SET);
	for(i=0;i<=40;i++){//�Q�[�����e�[�u���쐬�i���͕ϊ��ɂ͖��֌W�Ȃ�ł����j
		fread(&g_name1[i],1,1,tbl);
		if(g_name1[i]==0x0d){
			g_name1[i]=0x00;
			printf("�@�Q�[�����@�@:%s\n",g_name1);
			fseek(tbl,1,SEEK_CUR);
			break;
		}
	}
	for(i=0;i<=20;i++){//�V�i���I�t�@�C���e�[�u���쐬
		fread(&f_name1[i],1,1,tbl);
		if(f_name1[i]==0x0d){
			f_name1[i]=0x00;
			printf("�@�t�@�C�����@:%s\n",f_name1);
			fseek(tbl,1,SEEK_CUR);
			break;
		}
	}
	for(i=0;i<=20;i++){//��l�������e�[�u���쐬
		fread(&h_name1[i],1,1,tbl);
		if(h_name1[i]==0x0d){
			h_name1[i]=0x00;
			printf("�@�����@�@�@�@:%s\n",h_name1);
			fseek(tbl,1,SEEK_CUR);
			break;
		}
	}
	for(i=0;i<=20;i++){//��l�����O�e�[�u���쐬
		fread(&h_name2[i],1,1,tbl);
		if(h_name2[i]==0x0d){
			h_name2[i]=0x00;
			printf("�@���O�@�@�@�@:%s\n",h_name2);
			fseek(tbl,1,SEEK_CUR);
			break;
		}
	}
	for(i=0;i<=20;i++){//��l�������ȃe�[�u���쐬
		fread(&h_name3[i],1,1,tbl);
		if(h_name3[i]==0x0d){
			h_name3[i]=0x00;
			printf("�@�����ȁ@�@�@:%s\n",h_name3);
			fseek(tbl,1,SEEK_CUR);
			break;
		}
	}
	for(i=0;i<=0;i++){//�V�[���^�C�g���t���O�i�A���A���T�̓I�t�j
		fread(&title_f,1,1,tbl);
		title_f-=0x30;
		printf("�@�^�C�g���o��:");
		if(title_f==0){
			printf("�I�t\n");
		}
		if(title_f==1){
			printf("�I��\n");
		}
	}
	fclose(tbl);


	// �V�i���I�t�@�C���I�[�v��
	if((fp=fopen(f_name1,"rb"))==NULL){
		printf("���G���[\n");
		printf("�@�V�i���I�t�@�C�� [%s] ��������܂���B\n",f_name1);
		printf("�@[cuffs_con.exe]�Ɠ����t�H���_�ɓ���Ă���ēx���s���Ă��������B\n");
		printf("�iEnter�L�[�������ƏI�����܂��j\n");
		if(!DEBUG){
			getchar();//�_�~�[
		}
		exit(1);
	}

	//Unicode����Shitjis�ւ̕ϊ��e�[�u���쐬
	if((tbl=fopen("uni2shi.tbl","rb"))==NULL){
		printf("���G���[\n");
		printf("�@�e�[�u���t�@�C�� [uni2shi.tbl] ��������܂���B\n");
		printf("�@[cuffs_con.exe]�Ɠ����t�H���_�ɓ���Ă���ēx���s���Ă��������B\n");
		printf("�iEnter�L�[�������ƏI�����܂��j\n");
		if(!DEBUG){
			getchar();//�_�~�[
		}
		exit(1);
	}
	for(i=0;i<=65535;i++){//Unicode����shiftjis�ւ̕ϊ��e�[�u���쐬
		fread(&iGetFile,1,1,tbl);
		table[i][0]=iGetFile;
		fread(&iGetFile,1,1,tbl);
		table[i][1]=iGetFile;
		table[i][2]=0x00;
	}
	fclose(tbl);


	printf("���V�i���I�ϊ���...\n"); 
	printf("�i���̂܂܂��҂����������j\n");

	conv1();  // ���b�Z�[�W�R���o�[�g

	// �t�@�C���N���[�Y
	fclose(fp);

	printf("����l�����O�C����...\n"); 
	printf("�i���̂܂܂��҂����������j\n");
	fp=fopen("temp.nota","rb");
	conv2();  // ���b�Z�[�W�R���o�[�g
	fclose(fp);

	printf("���S�~������...\n"); 
	printf("�i���̂܂܂��҂����������j\n");
	fp=fopen("temp2.nota","rt");
	conv3();  // ���b�Z�[�W�R���o�[�g
	fclose(fp);

	remove("temp.nota");
	remove("temp2.nota");

	printf("���V�i���I�ϊ�����\n");
	printf("�iEnter�L�[�������ƏI�����܂��j\n");
	if(!DEBUG){
		getchar();//�_�~�[
	}

	return 0;
}

void conv1(void){
	unsigned long ulStart;
	unsigned long ulEnd;

	to=fopen("temp.nota","wt");//���ʊi�[�p�t�@�C��

	fseek(fp,-1,SEEK_END); 
	fgetpos(fp,&fsize);//�t�@�C���T�C�Y�Q�b�g
	seen_f=0;
	ulStart=0;
	ulEnd  =ulStart+fsize;

	fseek(fp,ulStart,SEEK_SET);
	for(i=ulStart;i<=ulEnd;i++){
		fread(&iGetFile,1,1,fp);

		if(i%2000000==0){//�i�s�󋵃J�E���^
			printf("%10d/%9d completed...\n",i,ulEnd);
		}

		if(iGetFile==0x41){//�I��������
			//AddSelect 41 00 64 00 64 00 53 00 65 00 6C 00 65 00 63 00 74 00
			fseek(fp,-3,SEEK_CUR);
			i-=3;
			fread(&iGetFile,1,1,fp);
			i++;
			fread(&iGetFile2,1,1,fp);
			i++;
			if(iGetFile==0x00&&iGetFile2==0x00){// \0\0Talk�`�F�b�N
				//OK�������Ƃ�
				fseek(fp,1,SEEK_CUR);
				i++;
			}else{//"AddSelect"�łȂ������Ƃ�
				fseek(fp,+2,SEEK_CUR);
				i+=2;
				continue;
			}
			//AddSelect 41 00 64 00 64 00 53 00 65 00
			for(r=0;r<=8;r++){//AddSe��������
				fread(&iGetFile,1,1,fp);
				i++;
				t[r]=iGetFile;
			}
			if(t[0]==0x00&&t[1]==0x64&&t[2]==0x00&&t[3]==0x64&&t[4]==0x00&&t[5]==0x53&&t[6]==0x00&&t[7]==0x65&&t[8]==0x00){

//fprintf(to,"\n%8x/ad/\n",i);//debug
				//AddSe�������Ƃ�
				fseek(fp,8,SEEK_CUR);
				i+=8;
				fread(&iGetFile,1,1,fp);
				i++;
				fread(&iGetFile2,1,1,fp);
				i++;
				if(iGetFile==0x01&&iGetFile2==0x02){//OK
					fseek(fp,-0x24,SEEK_CUR);
					i-=0x24;
					while(i<ulEnd){//�I�������J�n��������
						fread(&iGetFile,1,1,fp);
						i++;
						if(iGetFile==0x00){
							fseek(fp,-2,SEEK_CUR);
							i-=2;
							fread(&iGetFile,1,1,fp);
							i++;
							if(iGetFile==0x00){//OK
								fseek(fp,1,SEEK_CUR);
								i++;
								break;
							}else{
								fseek(fp,-2,SEEK_CUR);
								i-=2;
								continue;
							}
						}
						fseek(fp,-2,SEEK_CUR);
						i-=2;
					}
					fprintf(to,"��");
					while(i<ulEnd){
						fread(&iGetFile2,1,1,fp);
						i++;
						fread(&iGetFile,1,1,fp);
						i++;
						if(iGetFile==0x00&&iGetFile2==0x02){//����
							fprintf(to,"\n");
							fseek(fp,0x15,SEEK_CUR);
							i+=0x15;
							break;
						}
						fprintf(to,"%s",table[iGetFile*256+iGetFile2]);
					}
				}else{//AddSe+??�̈Ⴄ���䕶�̂Ƃ�
					continue;
				}
			}else{//Talk��Mess�łȂ������Ƃ�
				fseek(fp,-9,SEEK_CUR);
				i-=9;
				continue;
			}
		}




/////////////////////////////////�����ҏ�������
		if(iGetFile==0x06){//02 00 06 XX 00 00 00 :XX*2�������҃o�C�g��
			fread(&iGetFile,1,1,fp);
			i++;
			if(iGetFile<0x01&&iGetFile>0x0f){//NG���O������
				fseek(fp,-1,SEEK_CUR);
				i--;
				seen_f=0;
				continue;
			}
			count_a=iGetFile*2;//�o�C�g������

			fseek(fp,-4,SEEK_CUR);
			i-=4;
			fread(&iGetFile,1,1,fp);
			i++;
			if(iGetFile!=0x02){//00�`�F�b�N����-2
				fseek(fp,2,SEEK_CUR);
				i+=2;
				continue;
			}

			fread(&iGetFile,1,1,fp);
			i++;
			if(iGetFile!=0x00){//00�`�F�b�N����-1
				fseek(fp,1,SEEK_CUR);
				i+=1;
				continue;
			}
			fseek(fp,2,SEEK_CUR);
			i+=2;

			fread(&iGetFile,1,1,fp);
			i++;
			if(iGetFile!=0x00){//00�`�F�b�N���̂P
				fseek(fp,-2,SEEK_CUR);
				i-=2;
				continue;
			}
			fread(&iGetFile,1,1,fp);
			i++;
			if(iGetFile!=0x00){//00�`�F�b�N���̂Q
				fseek(fp,-3,SEEK_CUR);
				i-=3;				seen_f=0;
				continue;
			}
			fread(&iGetFile,1,1,fp);
			i++;
			if(iGetFile!=0x00){//00�`�F�b�N���̂R
				fseek(fp,-4,SEEK_CUR);
				i-=4;
				continue;
			}

//fprintf(to,"\n%8x/se/\n",i);//debug

			if(seen_f==0 || title_f==0){
				fprintf(to,"�y");
			}else{
				fprintf(to,"��");//�Z�[�u�^�C�g��
			}
			for(r=1;r<=count_a;r+=2){
				fread(&iGetFile2,1,1,fp);
				i++;
				fread(&iGetFile,1,1,fp);
				i++;
				fprintf(to,"%s",table[iGetFile*256+iGetFile2]);
			}
			if(seen_f==0 || title_f==0){
				fprintf(to,"�z\n");
			}else{
				fprintf(to,"��\n");//�Z�[�u�^�C�g��
				seen_f=0;
			}
		}
//////////////////////////////////////////

/*
*/
		//�Z�[�u�^�C�g���t���O
		if(iGetFile==0x53){//53 00 63 00 65 00 6E 00 61 00 72 00 69 00 6F 00 45 00 6E 00 74 00 65 00 72 :ScenarioEnter
			for(r=0;r<=23;r++){//ScenarioEnter(�Z�[�u�^�C�g���\��)
				fread(&iGetFile,1,1,fp);
				i++;
				t[r]=iGetFile;
			}
			if(t[0]==0x00&&t[1]==0x63&&t[2]==0x00&&t[3]==0x65&&t[4]==0x00&&t[5]==0x6e&&t[6]==0x00&&t[7]==0x61&&t[8]==0x00&&t[9]==0x72&&t[10]==0x00&&t[11]==0x69&&t[12]==0x00&&t[13]==0x6f&&t[14]==0x00&&t[15]==0x45&&t[16]==0x00&&t[17]==0x6e&&t[18]==0x00&&t[19]==0x74&&t[20]==0x00&&t[21]==0x65&&t[22]==0x00&&t[23]==0x72){
				seen_f=1;
				continue;
			}else{
				fseek(fp,-24,SEEK_CUR);
				i-=24;
			}
			continue;
		}
















		///���͏����o���n��
		if(iGetFile==0x54||iGetFile==0x4d){
			//TalK 54 00 61 00 6C 00 6B 00
			//Mess 4D 00 65 00 73 00 73 00
			fseek(fp,-3,SEEK_CUR);
			i-=3;
			fread(&iGetFile,1,1,fp);
			i++;
			fread(&iGetFile2,1,1,fp);
			i++;
			if(iGetFile==0x00&&iGetFile2==0x00){// \0\0Talk�`�F�b�N
				//Talk��Mess�������Ƃ�
				fseek(fp,1,SEEK_CUR);
				i++;
			}else{//"Talk"��"Mess"�łȂ������Ƃ�
				fseek(fp,+2,SEEK_CUR);
				i+=2;
				continue;
			}
			//TalK 54 00 61 00 6C 00 6B 00
			//Mess 4D 00 65 00 73 00 73 00
			for(r=0;r<=6;r++){//TalK Mess��������
				fread(&iGetFile,1,1,fp);
				i++;
				t[r]=iGetFile;
			}

			if((t[0]==0x00&&t[1]==0x61&&t[2]==0x00&&t[3]==0x6c&&t[4]==0x00&&t[5]==0x6b&&t[6]==0x00)||(t[0]==0x00&&t[1]==0x65&&t[2]==0x00&&t[3]==0x73&&t[4]==0x00&&t[5]==0x73&&t[6]==0x00)){
				//Talk��
				//Mess�������Ƃ�
				fread(&iGetFile,1,1,fp);
				i++;
				fread(&iGetFile2,1,1,fp);
				i++;
				if(iGetFile==0x01&&iGetFile2==0x02){//OK
//fprintf(to,"\n%8x/me/\n",i);//debug
					fseek(fp,6,SEEK_CUR);
					i+=6;
				}else{//talk+??�Ƃ��̈Ⴄ���䕶�̂Ƃ�
					continue;
				}
			}else{//Talk��Mess�łȂ������Ƃ�
				fseek(fp,-7,SEEK_CUR);
				i-=7;
				continue;
			}
			while(i<ulEnd){//���͏����o��
				fread(&iGetFile2,1,1,fp);
				i++;
				fread(&iGetFile,1,1,fp);
				i++;
				if(iGetFile==0x05&&iGetFile2==0x08){//���b�Z�[�W�G���h
					break;
				}
				if(iGetFile==0x00&&iGetFile2==0x02){//���b�Z�[�W�G���h
					fprintf(to,"\n");
					break;
				}
				if(iGetFile2==0x08&&iGetFile==0x05){//����
					break;
				}
				fprintf(to,"%s",table[iGetFile*256+iGetFile2]);
			}
		}
	}
	fprintf(to,"�ł��`�ǂ��イ�[���[\n");//�t�@�C���G���h�ی��\��
	fclose(to);
}







void conv2(void){//��l�����O����p
	unsigned long ulStart;
	unsigned long ulEnd;

	to=fopen("temp2.nota","wb");//���ʊi�[�p�t�@�C��

	fseek(fp,-1,SEEK_END); 
	fgetpos(fp,&fsize);//�t�@�C���T�C�Y�Q�b�g

	ulStart=0;
	ulEnd  =ulStart+fsize;

	fseek(fp,ulStart,SEEK_SET);
	for(i=ulStart;i<=ulEnd;i++){
		fread(&iGetFile,1,1,fp);
		if(i%400000==0){//�i�s�󋵃J�E���^
			printf("%10d/%9d completed...\n",i,ulEnd);
		}
		if(iGetFile==0x81){//���O���
			fread(&iGetFile,1,1,fp);
			i++;

			if(iGetFile==0x97){//��l��������������
				fread(&iGetFile,1,1,fp);
				i++;
				if(iGetFile==0x81){//��l��������������
					fread(&iGetFile,1,1,fp);
					i++;
					if(iGetFile==0x97){//��l��������������
						fseek(fp,4,SEEK_CUR);
						i+=4;
						fprintf(to,"%s",h_name1);
						continue;
					}
					fseek(fp,-3,SEEK_CUR);
					i-=3;
					fprintf(to,"%c",0x81);
					continue;
				}
				fseek(fp,-2,SEEK_CUR);
				i-=2;
				fprintf(to,"%c",0x81);
				continue;
			}

			if(iGetFile==0x96){//��l�����O��������
				fread(&iGetFile,1,1,fp);
				i++;
				if(iGetFile==0x81){//��l��������������
					fread(&iGetFile,1,1,fp);
					i++;
					if(iGetFile==0x96){//��l�����O��������
						fseek(fp,4,SEEK_CUR);
						i+=4;
						fprintf(to,"%s",h_name2);
						continue;
					}
					fseek(fp,-3,SEEK_CUR);
					i-=3;
					fprintf(to,"%c",0x81);
					continue;
				}
				fseek(fp,-2,SEEK_CUR);
				i-=2;
				fprintf(to,"%c",0x81);
				continue;
			}

			if(iGetFile==0x90){//��l�������ȁ�������
				fread(&iGetFile,1,1,fp);
				i++;
				if(iGetFile==0x81){//��l��������������
					fread(&iGetFile,1,1,fp);
					i++;
					if(iGetFile==0x90){//��l�������ȁ�������
						fseek(fp,4,SEEK_CUR);
						i+=4;
						fprintf(to,"%s",h_name3);
						continue;
					}
					fseek(fp,-3,SEEK_CUR);
					i-=3;
					fprintf(to,"%c",0x81);
					continue;
				}
				fseek(fp,-2,SEEK_CUR);
				i-=2;
				fprintf(to,"%c",0x81);
				continue;
			}

			fprintf(to,"%c",0x81);
			fseek(fp,-1,SEEK_CUR);
			i--;
			continue;
		}else{
			fprintf(to,"%c",iGetFile);
			continue;
		}
	}
	fclose(to);
}

void conv3(void){//�S�~�����p
	unsigned long ulStart;
	unsigned long ulEnd;

	to=fopen("cuffs_dec.txt","wt");//���ʊi�[�p�t�@�C��

	fseek(fp,-1,SEEK_END); 
	fgetpos(fp,&fsize);//�t�@�C���T�C�Y�Q�b�g

	ulStart=0;
	ulEnd  =ulStart+fsize;

	fseek(fp,ulStart,SEEK_SET);
	while(1){
		fgets(buf, 0x200, fp);

		if(i%400000<=6){//�i�s�󋵃J�E���^
			printf("%10d/%9d completed...\n",i,ulEnd);
		}
		for(r=0;r<=0x200;r++){
			if(buf[r]==0x0d||buf[r]==0x0a){
				buf[r]=0x00;
				break;
			}
			if(r==0x200){
				buf[r]=0x00;
				break;
			}
		}

		if(buf[0]==0x81&&buf[1]==0xa1){//�I�����d������
			fseek(to,-r-4,SEEK_CUR); 
			fprintf(to,"%s\n",buf);
			continue;
		}
		if(strcmp(buf, "�y���ʁz")==0){//�댟�o
			continue;
			i=ftell(fp);
		}
		if(strcmp(buf, "�y�S�̐��z")==0){//���O�Ȃ�
			continue;
			i=ftell(fp);
		}
		if(strcmp(buf, "�y���z")==0){//���O�Ȃ�
			continue;
			i=ftell(fp);
		}
		if(strcmp(buf, "�y�z")==0){//�댟�o
			continue;
			i=ftell(fp);
		}
		if(strcmp(buf, "����")==0){//�댟�o
			continue;
			i=ftell(fp);
		}
		if(strcmp(buf, "�ł��`�ǂ��イ�[���[")==0){//�f�R�[�h����
			break;
		}
		fprintf(to,"%s\n",buf);
		i=ftell(fp);
		continue;
	}
	fclose(to);
}


