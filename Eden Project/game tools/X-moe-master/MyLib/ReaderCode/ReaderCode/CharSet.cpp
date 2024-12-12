#include "CharSet.h"

int inline WideCharToUtf8(WCHAR in, char * out)
{
	if (in < (1 << 7))
	{
		if (out)
		{
			out[0] = (char)in;
		}
		return 1;
	}
	else if (in < (1 << 11))
	{
		if (out)
		{
			out[0] = (char)(0xc0 | (in >> 6));
			out[1] = (char)(0x80 | (in & 0x3f));
		}
		return 2;
	}
	else if (in < (1 << 16))
	{
		if (out)
		{
			out[0] = (char)(0xe0 | (in >> 12));
			out[1] = (char)(0x80 | ((in >> 6) & 0x3f));
			out[2] = (char)(0x80 | (in & 0x3f));
		}
		return 3;
	}
	else
	{
		MessageBoxW(NULL, L"Cannot Convert UTF-16 to UTF-8", NULL, MB_OK);
	}
	return -1;
}

int WideCharToUtf8String(const WCHAR *in, char * out)
{
	int count = 0;
	while (*in)
	{
		int n;
		if (out)
		{
			n = WideCharToUtf8(*in, out);
			out += n;
		}
		else
		{
			n = WideCharToUtf8(*in, NULL);
		}
		if (n == -1) return -1; // invalid character found
		count += n;
		in++;
	}
	return count;
}

bool inline Utf8ToWideChar(const char * & in, WCHAR *out)
{
	const unsigned char * & p = (const unsigned char * &)in;
	if (p[0] < 0x80)
	{
		if (out) *out = (WCHAR)in[0];
		in++;
		return true;
	}
	else if (p[0] < 0xc2)
	{
		return false;
	}
	else if (p[0] < 0xe0)
	{
		if ((p[1] & 0xc0) != 0x80) return false;
		if (out) *out = ((p[0] & 0x1f) << 6) + (p[1] & 0x3f);
		in += 2;
		return true;
	}
	else if (p[0] < 0xf0)
	{
		if ((p[1] & 0xc0) != 0x80) return false;
		if ((p[2] & 0xc0) != 0x80) return false;
		if (out) *out = ((p[0] & 0x1f) << 12) + ((p[1] & 0x3f) << 6) + (p[2] & 0x3f);
		in += 3;
		return true;
	}
	else if (p[0] < 0xf8)
	{
		// four bytes (21bits)
		if ((p[1] & 0xc0) != 0x80) return false;
		if ((p[2] & 0xc0) != 0x80) return false;
		if ((p[3] & 0xc0) != 0x80) return false;
		if (out) *out = ((p[0] & 0x07) << 18) + ((p[1] & 0x3f) << 12) +
			((p[2] & 0x3f) << 6) + (p[3] & 0x3f);
		in += 4;
		return true;
	}
	else if (p[0] < 0xfc)
	{
		// five bytes (26bits)
		if ((p[1] & 0xc0) != 0x80) return false;
		if ((p[2] & 0xc0) != 0x80) return false;
		if ((p[3] & 0xc0) != 0x80) return false;
		if ((p[4] & 0xc0) != 0x80) return false;
		if (out) *out = ((p[0] & 0x03) << 24) + ((p[1] & 0x3f) << 18) +
			((p[2] & 0x3f) << 12) + ((p[3] & 0x3f) << 6) + (p[4] & 0x3f);
		in += 5;
		return true;
	}
	else if (p[0] < 0xfe)
	{
		// six bytes (31bits)
		if ((p[1] & 0xc0) != 0x80) return false;
		if ((p[2] & 0xc0) != 0x80) return false;
		if ((p[3] & 0xc0) != 0x80) return false;
		if ((p[4] & 0xc0) != 0x80) return false;
		if ((p[5] & 0xc0) != 0x80) return false;
		if (out) *out = ((p[0] & 0x01) << 30) + ((p[1] & 0x3f) << 24) +
			((p[2] & 0x3f) << 18) + ((p[3] & 0x3f) << 12) +
			((p[4] & 0x3f) << 6) + (p[5] & 0x3f);
		in += 6;
		return true;
	}
	return false;
}

int Utf8ToWideCharString(const char * in, WCHAR*out)
{
	// convert input utf-8 string to output wide string
	int count = 0;
	while (*in)
	{
		WCHAR c;
		if (out)
		{
			if (!Utf8ToWideChar(in, &c))
				return -1; // invalid character found
			*out++ = c;
		}
		else
		{
			if (!Utf8ToWideChar(in, NULL))
				return -1; // invalid character found
		}
		count++;
	}
	return count;
}

size_t Mbstowcs(WCHAR *pwcs, const char *s, size_t n, ULONG CodePage)
{
	if (pwcs && n == 0) return 0;

	if (pwcs)
	{
		int count = MultiByteToWideChar(CodePage,
			MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, s, -1, pwcs, n);
		if (count != 0) return count - 1;

		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) return (size_t)-1;

		int charcnt = n;
		const unsigned char *p;
		for (p = (const unsigned char*)s; charcnt-- && *p; p++)
		{
			if (IsDBCSLeadByteEx(CodePage, *p)) p++;
		}
		int bytecnt = (int)(p - (const unsigned char *)s);

		count = MultiByteToWideChar(CodePage, MB_PRECOMPOSED, s, bytecnt, pwcs, n);
		if (count == 0) return (size_t)-1;

		return count;
	}
	else
	{
		int count = MultiByteToWideChar(CodePage, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
			s, -1, NULL, 0);
		if (count == 0) return (size_t)-1;
		return count - 1;
	}
}
