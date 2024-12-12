// TextHaloEffectDoc.cpp :  CTextHaloEffectDoc 类的实现
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


// CTextHaloEffectDoc 构造/析构

CTextHaloEffectDoc::CTextHaloEffectDoc()
{
	// TODO: 在此添加一次性构造代码

}

CTextHaloEffectDoc::~CTextHaloEffectDoc()
{
}

BOOL CTextHaloEffectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CTextHaloEffectDoc 序列化

void CTextHaloEffectDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CTextHaloEffectDoc 诊断

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


// CTextHaloEffectDoc 命令
