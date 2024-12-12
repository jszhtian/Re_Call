// TextHaloEffectView.cpp : CTextHaloEffectView 类的实现
//

#include "stdafx.h"
#include "TextHaloEffect.h"

#include "TextHaloEffectDoc.h"
#include "TextHaloEffectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextHaloEffectView

IMPLEMENT_DYNCREATE(CTextHaloEffectView, CView)

BEGIN_MESSAGE_MAP(CTextHaloEffectView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CTextHaloEffectView 构造/析构

CTextHaloEffectView::CTextHaloEffectView()
{
	// TODO: 在此处添加构造代码

}

CTextHaloEffectView::~CTextHaloEffectView()
{
}

BOOL CTextHaloEffectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CView::PreCreateWindow(cs);
}

// CTextHaloEffectView 绘制

void CTextHaloEffectView::OnDraw(CDC* pDC)
{
	CTextHaloEffectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect ClientRect;
	GetClientRect(&ClientRect);
	CSize ClientSize(ClientRect.Width(),ClientRect.Height());
	using namespace Gdiplus;
	Graphics g(pDC->m_hDC);
	RectF ClientRectangle(ClientRect.top,ClientRect.left,ClientRect.Width(),ClientRect.Height()); 
	g.FillRectangle(&SolidBrush(Color::Black),ClientRectangle);
	Bitmap bm(ClientSize.cx/5,ClientSize.cy/5,&g);

	//Create a GraphicsPath object.  

	GraphicsPath pth;

	//Add the string in the chosen style.  
    int style = FontStyleRegular;
	pth.AddString(L"文字光圈",-1,&FontFamily(L"宋体"),style,100,Point(20,20),NULL);

	//Get the graphics object for the image.  

	Graphics* bmpg = Graphics::FromImage(&bm);

	//Create a matrix that shrinks the drawing output by the fixed ratio.  

	Matrix mx(1.0f/5,0,0,1.0f/5,-(1.0f/5),-(1.0f/5));

	//Choose an appropriate smoothing mode for the halo.  

	bmpg->SetSmoothingMode(SmoothingModeAntiAlias);

	//Transform the graphics object so that the same half may be used for both halo and text output.  

	bmpg->SetTransform(&mx);

	//Using a suitable pen...

	Pen p(Color::Yellow,3);

	//Draw around the outline of the path

	bmpg->DrawPath(&p,&pth);

	//and then fill in for good measure.  

	bmpg->FillPath(&SolidBrush(Color::Yellow),&pth);



	//this just shifts the effect a little bit so that the edge isn't cut off in the demonstration 

	g.SetTransform(&Matrix(1,0,0,1,50,50));

	//setup the smoothing mode for path drawing

	g.SetSmoothingMode(SmoothingModeAntiAlias);

	//and the interpolation mode for the expansion of the halo bitmap 

	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	//expand the halo making the edges nice and fuzzy.  

	g.DrawImage(&bm,ClientRectangle,0,0,bm.GetWidth(),bm.GetHeight(),UnitPixel);

	//Redraw the original text 

	g.FillPath(&SolidBrush(Color::Black),&pth);

	//and you're done.  


}





// CTextHaloEffectView 打印

BOOL CTextHaloEffectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTextHaloEffectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印前添加额外的初始化
}

void CTextHaloEffectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印后添加清除过程
}


// CTextHaloEffectView 诊断

#ifdef _DEBUG
void CTextHaloEffectView::AssertValid() const
{
	CView::AssertValid();
}

void CTextHaloEffectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTextHaloEffectDoc* CTextHaloEffectView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTextHaloEffectDoc)));
	return (CTextHaloEffectDoc*)m_pDocument;
}
#endif //_DEBUG


// CTextHaloEffectView 消息处理程序
