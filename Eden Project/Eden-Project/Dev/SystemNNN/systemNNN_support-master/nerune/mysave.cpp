//
// mysave.cpp
//

#include <windows.h>
#include <stdio.h>

#include "mysave.h"




char nopackHeader[]= "\
IF PACKTYPE==0  \
CUT THIS 64 BYTE\
THEN REMAKE WAV \
PACKTYPE=0      ";

char packHeader2[]= "\
IF PACKTYPE==2  \
CUT THIS 108BYTE\
THEN REMAKE OGG \
PACKTYPE=2      ";


CMySave::CMySave()
{
}

CMySave::~CMySave()
{
	End();
}

void CMySave::End(void)
{
}


BOOL CMySave::Save(LPSTR filename, void* headerBuffer, void* dataBuffer, int dataSize, int type)
{
	FILE* file = NULL;
	fopen_s(&file,filename,"wb");

	//FILE* file = fopen(filename,"wb");
	if (file == NULL)
	{
		MessageBox(NULL,"Save Error","FATAL",MB_OK);
		return FALSE;
	}


	//vaw header
	if (type == 2)
	{
		fwrite(packHeader2,sizeof(char),64,file);
	}
	else if (type == 0)
	{
		fwrite(nopackHeader,sizeof(char),64,file);
	}
	else
	{
//		fwrite(nopackHeader,sizeof(char),64,file);
	}

	//wave header


	char* ptr = (char*)(headerBuffer);
	int ln = *((int*)(ptr+16));
	ln += 20;

	fwrite(headerBuffer,sizeof(char),ln,file);


	fwrite(dataBuffer,sizeof(char),dataSize,file);

	fclose(file);
	return TRUE;
}
