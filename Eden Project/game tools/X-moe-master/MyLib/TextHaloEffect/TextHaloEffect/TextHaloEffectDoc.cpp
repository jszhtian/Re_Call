// TextHaloEffectDoc.cpp :  CTextHaloEffectDoc ���ʵ��
//

#include "stdafx.h"
#include "TextHaloEffect.h"

#include "TextHaloEffectDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextHaloEffectDoc

IMPLEMENT_DYNCREATE(CTextHaloEffectDoc, CDocument)

BEGIN_MESSAGE_MAP(CTextHaloEffectDoc, CDocument)
END_MESSAGE_MAP()


// CTextHaloEffectDoc ����/����

CTextHaloEffectDoc::CTextHaloEffectDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CTextHaloEffectDoc::~CTextHaloEffectDoc()
{
}

BOOL CTextHaloEffectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CTextHaloEffectDoc ���л�

void CTextHaloEffectDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CTextHaloEffectDoc ���

#ifdef _DEBUG
void CTextHaloEffectDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTextHaloEffectDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTextHaloEffectDoc ����
