#include "vcl.h"

#include <string.h>
#include <mbstring.h>


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
AnsiString::AnsiString()
{
	m_szBuffer = NULL;
}

//----------------------------------------------------------------------------
AnsiString::AnsiString(const AnsiString& str)
{
	if ( str.Length() == 0 )
	{
		m_szBuffer = NULL;
		return;
	}
	int n = strlen(str.c_str());
	m_szBuffer = (char*)malloc( n + 1 );
	strcpy(m_szBuffer, str.c_str());
}

//----------------------------------------------------------------------------
// int �^�̐��l�� AnsiString ������ɕϊ�����
AnsiString::AnsiString(int val)
{
	int n = 12; // INT_MAX=2147483647 + '-' + '\0'
	m_szBuffer = (char*)malloc( n + 1 );
	sprintf(m_szBuffer, "%d", val );
}

//----------------------------------------------------------------------------
AnsiString::AnsiString(const char* sz)
{
	if ( sz == NULL )
	{
		m_szBuffer = NULL;
		return;
	}
	int n = strlen(sz);
	m_szBuffer = (char*)malloc( n + 1 );
	strcpy(m_szBuffer, sz);
}
//----------------------------------------------------------------------------
AnsiString::AnsiString(const wchar_t* psz)
{
	int n;
	for(n=0; psz[n] != 0; n++);

	m_szBuffer = (char*)malloc( 2*n + 1 );

	wxMBConvLibc conv;
	conv.WC2MB( m_szBuffer, psz, n );
}
//----------------------------------------------------------------------------
// NULL �I�[������̐擪����w��̃o�C�g������ AnsiString ������ɕϊ�����
// ���̏ꍇ�Alen �� src �̃o�C�g�����傫���l���w�肷�邱�Ƃ͂ł��Ȃ�
AnsiString::AnsiString(const char* src, unsigned int len)
{
	int n = std::max( (size_t)len, strlen(src) );
	m_szBuffer = (char*)malloc( n + 1 );
	strcpy(m_szBuffer, src);
	m_szBuffer[len] = '\0';
}

//----------------------------------------------------------------------------
AnsiString::~AnsiString()
{
	if ( m_szBuffer )
		free(m_szBuffer);
}
//----------------------------------------------------------------------------
int AnsiString::AnsiCompareIC(const AnsiString& str) const
{
	if ( m_szBuffer == str.c_str() ) return 0;
	if ( m_szBuffer == NULL ) return -1;
	if ( str.c_str() == NULL ) return 1;
	return _stricmp( m_szBuffer, str.c_str() );
}
//----------------------------------------------------------------------------
// ���̊֐��͂����ƍ��ĂȂ��̂ŁA�܂����x
int  AnsiString::AnsiPos(const AnsiString& str) const
{
	if ( m_szBuffer == str.m_szBuffer ) return 0;
	if ( m_szBuffer == NULL || str.m_szBuffer == NULL ) return 0;
	char* p = (char*)_mbsstr((unsigned char*)m_szBuffer, (unsigned char*)str.m_szBuffer);
	if ( p == NULL ) return 0;
	return p - m_szBuffer + 1; // �擪�����Ƀq�b�g������ 1 ��Ԃ��炵��
}
//----------------------------------------------------------------------------
AnsiString AnsiString::SubString(int head, int cnt) const
{
	AnsiString str( m_szBuffer + head );
	str.c_str()[cnt] = '\0';
	return str;
}

//----------------------------------------------------------------------------
AnsiString AnsiString::operator+(const char c) const
{
	char sz[2] = {0};
	sz[0] = c;
	return operator+(sz);
}

//----------------------------------------------------------------------------
AnsiString AnsiString::operator+(const char * sz) const
{
	int n = strlen(sz);
	AnsiString str;

	str.m_szBuffer = (char*)malloc(Length() + n + 1);

	if ( m_szBuffer )
	{
		strcpy( str.m_szBuffer, m_szBuffer );
		strcat( str.m_szBuffer, sz );
	}
	else
	{
		strcpy( str.m_szBuffer, sz );
	}
	return str;
}

//----------------------------------------------------------------------------
AnsiString AnsiString::operator+(const AnsiString & str) const
{
	return operator+(str.c_str());
}


//----------------------------------------------------------------------------
bool AnsiString::operator< (const AnsiString & str) const
{
	return strcmp(m_szBuffer, str.c_str()) > 0;
}

//----------------------------------------------------------------------------
AnsiString& AnsiString::operator=(const char * sz)
{
	if ( sz == NULL )
	{
		free(m_szBuffer);
		m_szBuffer = NULL;
	}
	else
	{
		int n = strlen(sz);
		m_szBuffer = (char*)realloc( m_szBuffer, n + 1 );
		strcpy(m_szBuffer, sz);
	}

	return *this;
}

//----------------------------------------------------------------------------
AnsiString& AnsiString::operator=(const AnsiString & str)
{
	int n = str.Length();
	if ( n == 0 )
	{
		if ( m_szBuffer != NULL )
			m_szBuffer[0] = '\0';
	}
	else
	{
		m_szBuffer = (char*)realloc( m_szBuffer, n + 1 );
		strcpy(m_szBuffer, str.c_str());
	}

	return *this;
}

//----------------------------------------------------------------------------
void AnsiString::operator += (const char c)
{
	char sz[2] = {0};
	sz[0] = c;
	operator+= (sz);
}
//----------------------------------------------------------------------------
void AnsiString::operator += (const char * sz)
{
	if ( sz == NULL )
		return;
	int n = strlen(sz);

	if ( m_szBuffer )
	{
		m_szBuffer = (char*)realloc(m_szBuffer, Length() + n + 1);
		strcat( m_szBuffer, sz );
	}
	else
	{
		m_szBuffer = (char*)malloc(n + 1);
		strcpy( m_szBuffer, sz );
	}
}
//----------------------------------------------------------------------------
void AnsiString::operator += (const AnsiString& str)
{
	operator+= (str.c_str());
}

//----------------------------------------------------------------------------
bool AnsiString::operator == (const AnsiString& str) const
{
	if ( c_str() == str.c_str() )
		return true;
	if ( c_str() == NULL )     return str.c_str()[0] == '\0';
	if ( str.c_str() == NULL ) return c_str()[0] == '\0';
	if ( strcmp(c_str(), str.c_str() ) == 0 )
		return true;
	return false;
}


//----------------------------------------------------------------------------
// �N���X�O���Z�q
//----------------------------------------------------------------------------
AnsiString operator + (const char *lhs, const AnsiString & rhs)
{
	return AnsiString(lhs) + rhs;
}
