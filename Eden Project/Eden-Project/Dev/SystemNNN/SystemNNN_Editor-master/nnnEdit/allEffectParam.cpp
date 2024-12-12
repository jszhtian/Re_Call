//
//
//


#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"


#include "..\..\systemNNN\nnnUtilLib\nameList.h"

#include "effectParam.h"
#include "allEffectParam.h"






CAllEffectParam::CAllEffectParam()
{
	m_effectParamList = new CNameList();
	m_effectParamList->LoadFile("nnndir\\setup\\effect\\effectlist.txt");

	int i;
	for (i=0;i<256;i++)
	{
		m_effectParam[i] = NULL;
	}

	int kosuu = m_effectParamList->GetNameKosuu() / 2;
	for (i=0;i<kosuu;i++)
	{
		int n = atoi(m_effectParamList->GetName(i*2));
		if ((n>=0) && (n<256))
		{
			LPSTR filename = m_effectParamList->GetName(i*2+1);
			m_effectParam[n] = new CEffectParam(filename);
		}
	}
}

CAllEffectParam::~CAllEffectParam()
{
	End();
}


void CAllEffectParam::End(void)
{
	for (int i=0;i<256;i++)
	{
		ENDDELETECLASS(m_effectParam[i]);
	}

	ENDDELETECLASS(m_effectParamList);
}


/*_*/

