//
// Contemessagedoc.h
//

#if !defined __TINYAN_NNNEDIT_CONTEMESSAGEDOC__
#define __TINYAN_NNNEDIT_CONTEMESSAGEDOC__

#include "mydocument.h"

class CConteMessageDoc : public CMyDocument
{
public:
	CConteMessageDoc(CMyApplicationBase* lpApp);
	~CConteMessageDoc();
	void End(void);
private:
//	void SetWriteStatus(int layer,int st);
};


#endif
/*_*/

