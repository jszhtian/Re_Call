//
//
//

#include <windows.h>
#include <stdio.h>

#include "case.h"
#include "messageData.h"
#include "myApplicationBase.h"

#include "myInputDialog.h"

#include "defaultControl.h"


CDefaultControl::CDefaultControl(CMyApplicationBase* app)
{
	m_app = app;
	m_input = m_app->GetInputDialog();
}

CDefaultControl::~CDefaultControl()
{
	End();
}

void CDefaultControl::End(void)
{
}


void CDefaultControl::SetDefaultMovieRate(void)
{
	int old = m_app->GetConfig("defaultMovieRate");
	int newData = old;

	if (m_input->GetNumber(old,&newData))
	{
		if (newData >= 1)
		{
			m_app->SetConfig("defaultMovieRate",newData);
		}
	}
}

void CDefaultControl::SetDefaultMovieScale(void)
{
	int old = m_app->GetConfig("defaultMovieScale");
	int newData = old;

	if (m_input->GetNumber(old,&newData))
	{
		if (newData >= 1)
		{
			m_app->SetConfig("defaultMovieScale",newData);
		}
	}
}

void CDefaultControl::SetDefaultBGLayer(void)
{
	int old = m_app->GetConfig("defaultBGLayer");
	int newData = old;

	if (m_input->GetNumber(old,&newData))
	{
		if ((newData >= 0) && (newData<=3))
		{
			m_app->SetConfig("defaultBGLayer",newData);
		}
	}
}

void CDefaultControl::SetDefaultTALayer(void)
{
	int old = m_app->GetConfig("defaultTALayer");
	int newData = old;

	if (m_input->GetNumber(old,&newData))
	{
		if ((newData >= 4) && (newData<=15))
		{
			m_app->SetConfig("defaultTALayer",newData);
		}
	}
}


void CDefaultControl::SetDefaultOverrapTime(void)
{
	int old = m_app->GetConfig("defaultOverrapTime");
	int newData = old;

	if (m_input->GetNumber(old,&newData))
	{
		if (newData > 0)
		{
			m_app->SetConfig("defaultOverrapTime",newData);
		}
	}
}

/*_*/


