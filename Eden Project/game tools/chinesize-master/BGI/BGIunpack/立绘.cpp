// ����.cpp : �������̨Ӧ�ó������ڵ㡣
// This program must use unicode
#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include "cstring"
#include "windows.h"
//#define _debug_
#include "SHB.h"
#include "CBG.h"          /* ������� */
#include "ARC.h"          /* ��.ARC����ȡ�ļ� */

 
int filen(FILE *handle);          //ȡ�ļ����� 

int _tmain(int argc, _TCHAR* argv[])
{
	FILE *pf ;
	int i ;
	CBG pic ;
	ARC pics ;
	wchar_t name[ 1024 ] ;
	wchar_t savedir[ 1024 ] ;
	wchar_t toname[ 64 ] ;
	memset( name , 0 , sizeof( name ) ) ;
	memset( savedir , 0 , sizeof( savedir ) ) ;
	memset( toname , 0 , sizeof( toname ) ) ;
	SetConsoleTitle( _T( "BGI��ͼ���ʽ������Գ��� xcjfgt@126.com" ) ) ;
#ifndef _debug_
	std::cout<<" *********     Make by xcjfht@126.com     ********** \n" ;
	std::cout << "**********        Ver 0.1.1      *********** \n" ;
	std::cout<<"������������...... \n " ;
	getchar( ) ;
	setlocale(LC_ALL,"Chinese-simplified") ;           /* ʹ�ü������� */
	for( ; name[ 0 ] == 0 ; SHBopen( name , _T( "ѡ��Ҫ�����ARC�ļ�" ) , 1 ) ) ;
	std::wcout << name ;
	for( ; savedir[ 0 ] == 0 ; SHBopen( savedir , _T( "ѡ��Ҫ���浽���ļ���" ) , 0 ) ) ;

	if ( pics.ARCopen( name ) ) std::cout<<"�򿪳ɹ���\n" ;
	else
	{
		std::cout<<"��ARC�ļ�ʧ�ܣ�\n" ;
		system( "pause" );
		return -1 ;
	}

	for( i = 0 ; i < pics.FileNum ; i++ )
	{
		if ( pics.ARCread( i ) ) 
		{
			printf( "%s ��ȡ�ɹ� " , pics.OneName ) ;
			if ( pic.Uncompress( pics.OneBuf , ( pics.OneFile )->FileSize ) ) 
			{
				std::cout<<" ����ɹ�" ;
				mbstowcs( toname , pics.OneName , strlen( pics.OneName) ) ;
				wcscpy( name , savedir ) ;
				wcscat( name , _T( "\\" ) ) ;
				wcscat( name , toname ) ;
				wcscat( name , _T( ".bmp" ) ) ;
				pf = _wfopen( name , _T( "wb" ) ) ;
				if ( pf != NULL )
				 {
					fwrite( pic.BmpData , 1 , pic.Size , pf ) ;
					fclose( pf ) ;          //һ��Ҫ�ǵ��ͷ��ļ����......
					std::cout << "  д���ɹ� \n" ;
				}
				else std::cout << "д��ʧ��  \n" ;
				continue ;
			}
			  std::cout<<" ����ʧ�� \n" ;
			  printf( "%s \n" , pic.GetError( ) ) ;
			  continue ;
		}
		std::cout<<" ��ȡʧ��" ;
	}
	std::cout<<"�������" ;
	system( "pause" ) ;
#endif
#ifdef _debug_
	pf = fopen( "f:/mes/sp0001a0.cbg" ,"rb" ) ;
	int pfsize = filen( pf ) ;
	char *pftp = new char[ pfsize ] ;
	fread( pftp , 1 , pfsize , pf ) ;
	fclose( pf ) ;
	pic.Uncompress( pftp , pfsize ) ;

	pf = fopen( "f:/mes/1.bmp" , "wb" ) ;
	fwrite( pic.BmpData , 1 , pic.Size , pf )  ;
	fclose( pf ) ;
	delete [ ] pftp ;
#endif
	return 0;
}
int filen(FILE *handle)
{
    int len;
     fseek(handle,0,2);
     len=ftell(handle);
     fseek(handle,0,0);
     return len;
}

