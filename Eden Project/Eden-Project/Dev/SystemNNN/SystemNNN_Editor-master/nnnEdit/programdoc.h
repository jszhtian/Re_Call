//
// programdoc.h
//

#if !defined __TINYAN_NNNEDIT_PROGRAMDOC__
#define __TINYAN_NNNEDIT_PROGRAMDOC__

#include "mydocument.h"

class CMyInputDialog;

class CProgramDoc : public CMyDocument
{
public:
	CProgramDoc(CMyApplicationBase* lpApp);
	~CProgramDoc();
	void End(void);

//	void OnCloseButton(void);
	void OnEditText(int n);

private:
	CMyInputDialog* m_inputDialog;
};


#endif
/*_*/

