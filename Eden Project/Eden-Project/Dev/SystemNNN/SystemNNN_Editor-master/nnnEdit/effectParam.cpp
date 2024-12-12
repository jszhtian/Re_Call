//
//
//

#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"
#include "..\..\systemNNN\nnnUtilLib\nameList.h"

#include "effectParam.h"



CEffectParam::CEffectParam(LPSTR filename)
{
	m_nameList = new CNameList();
	char filename2[256];
	wsprintf(filename2,"nnndir\\setup\\effect\\%s.txt",filename);
	m_nameList->LoadFile(filename2);

	int kosuu = m_nameList->GetNameKosuu();
	
	m_needParaKosuu = atoi(m_nameList->GetName(1));
	m_paraKosuu = atoi(m_nameList->GetName(2));

	m_para = NULL;
	if (m_paraKosuu > 0)
	{
		m_para = new int[m_paraKosuu * 5];
		for (int i=0;i<m_paraKosuu;i++)
		{
			for (int k=0;k<5;k++)
			{
				m_para[i*5+k] = atoi(m_nameList->GetName(10+i*10+1+k));
			}
		}
	}
}


CEffectParam::~CEffectParam()
{
	End();
}

void CEffectParam::End(void)
{
	DELETEARRAY(m_para);
	ENDDELETECLASS(m_nameList);
}


LPSTR CEffectParam::GetEffectName(void)
{
	return m_nameList->GetName(0);
}

LPSTR CEffectParam::GetMainDesc(int n)
{
	return m_nameList->GetName(3+n);
}


LPSTR CEffectParam::GetParamName(int n)
{
	return m_nameList->GetName(10+n*10);
}

int CEffectParam::GetParaMin(int n)
{
	return m_para[n*5];
}

int CEffectParam::GetParaMax(int n)
{
	return m_para[n*5+1];
}

int CEffectParam::GetParaDefault(int n)
{
	return m_para[n*5+2];
}

int CEffectParam::GetParaEditType(int n)
{
	return m_para[n*5+3];
}

int CEffectParam::GetParaEditPara(int n)
{
	return m_para[n*5+4];
}

LPSTR CEffectParam::GetParamDesc(int n,int k)
{
	return m_nameList->GetName(10+n*10+6+k);
}




/*_*/

