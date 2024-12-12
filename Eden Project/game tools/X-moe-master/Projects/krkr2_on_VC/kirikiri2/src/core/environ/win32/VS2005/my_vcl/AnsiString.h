#pragma once

#include <string.h>

class AnsiString
{
private:
	char* m_szBuffer;
public:
	AnsiString();
	AnsiString(int);
	AnsiString(const AnsiString&);
	AnsiString(const char*);
	AnsiString(const wchar_t*);

	// NULL �I�[������̐擪����w��̃o�C�g������ AnsiString ������ɕϊ�����
	// ���̏ꍇ�Alen �� src �̃o�C�g�����傫���l���w�肷�邱�Ƃ͂ł��Ȃ�
	AnsiString(const char* src, unsigned int len);

	virtual ~AnsiString();
	const char * c_str() const { return m_szBuffer; }
	char * c_str() { return m_szBuffer; }

	int Length(void) const { return m_szBuffer ? strlen(m_szBuffer) : 0; }
	int AnsiCompareIC(const AnsiString&) const;
	int AnsiPos(const AnsiString& str) const;
	AnsiString SubString(int, int) const;

	AnsiString operator+(const char) const;
	AnsiString operator+(const char *) const;
	AnsiString operator+(const AnsiString&) const;
	bool operator < (const AnsiString&) const;
	AnsiString& operator = (const char *);
	AnsiString& operator = (const AnsiString&);
	void operator += (const char);
	void operator += (const char *);
	void operator += (const AnsiString&);
	bool operator == (const AnsiString&) const;
	bool operator != (const AnsiString& str) const { return ! operator==(str); }

	friend AnsiString operator + (const char *lhs, const AnsiString &rhs);
};
