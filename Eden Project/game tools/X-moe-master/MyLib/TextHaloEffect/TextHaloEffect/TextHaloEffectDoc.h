// TextHaloEffectDoc.h :  CTextHaloEffectDoc ��Ľӿ�
//


#pragma once

class CTextHaloEffectDoc : public CDocument
{
protected: // �������л�����
	CTextHaloEffectDoc();
	DECLARE_DYNCREATE(CTextHaloEffectDoc)

// ����
public:

// ����
public:

// ��д
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CTextHaloEffectDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


