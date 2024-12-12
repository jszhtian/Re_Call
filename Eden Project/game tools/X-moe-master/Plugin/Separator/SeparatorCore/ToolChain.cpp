#include "ToolChain.h"

void TrimW(wstring& s)
{
	const wstring DropChar = L" ";
	s.erase(s.find_last_not_of(DropChar)+1);
	s.erase(0, s.find_first_not_of(DropChar));
}
