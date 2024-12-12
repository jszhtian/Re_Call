//
// selectobjectonly.cpp
//

#include <windows.h>


#include "selectobjectonly.h"

CSelectObjectOnly::CSelectObjectOnly()
{
}

CSelectObjectOnly::~CSelectObjectOnly()
{
	End();
}

void CSelectObjectOnly::End(void)
{
}

HGDIOBJ CSelectObjectOnly::Sel(HDC hdc, HGDIOBJ obj)
{
	int a = 0;
	a++;
//	OutputDebugString("[");
	HGDIOBJ old = SelectObject(hdc,obj);
//	OutputDebugString("]");

	return old;
}

