// TextHaloEffectView.h : CTextHaloEffectView ��Ľӿ�
//


#pragma once


class CTextHaloEffectView : public CView
{
protected: // �������л�����
	CTextHaloEffectView();
	DECLARE_DYNCREATE(CTextHaloEffectView)

// ����
public:
	CTextHaloEffectDoc* GetDocument() const;

// ����
public:

// ��д
	public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CTextHaloEffectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // TextHaloEffectView.cpp �ĵ��԰汾
inline CTextHaloEffectDoc* CTextHaloEffectView::GetDocument() const
   { return reinterpret_cast<CTextHaloEffectDoc*>(m_pDocument); }
#endif

