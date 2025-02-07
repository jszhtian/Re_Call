// encoding.cpp

#include <windows.h>
#include "encoding.h"

namespace Encoding
{
	std::wstring AnsiToUnicode(const std::string& source, int codePage)
	{
		if (source.length() == 0)
		{
			return std::wstring();
		}

		int length = MultiByteToWideChar(codePage, 0, source.c_str(), source.length(), NULL, 0);

		if (length == 0)
		{
			return std::wstring();
		}

		std::wstring output(length, L'\0');

		if (MultiByteToWideChar(codePage, 0, source.c_str(), source.length(), (LPWSTR)output.data(), output.length() + 1) == 0)
		{
			return std::wstring();
		}

		return output;
	}

	std::string UnicodeToAnsi(const std::wstring& source, int codePage)
	{
		if (source.length() == 0)
		{
			return std::string();
		}

		int length = WideCharToMultiByte(codePage, 0, source.c_str(), source.length(), NULL, 0, NULL, NULL);

		if (length == 0)
		{
			return std::string();
		}

		std::string output(length, '\0');

		if (WideCharToMultiByte(codePage, 0, source.c_str(), source.length(), (LPSTR)output.data(), output.length() + 1, NULL, NULL) == 0)
		{
			return std::string();
		}

		return output;
	}
}

