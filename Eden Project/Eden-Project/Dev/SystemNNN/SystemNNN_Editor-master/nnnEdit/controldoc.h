//
// controldoc.h
//

#if !defined __TINYAN_NNNEDIT_CONTROLDOC__
#define __TINYAN_NNNEDIT_CONTROLDOC__

#include "mydocument.h"

class CControlDoc : public CMyDocument
{
public:
	CControlDoc(CMyApplicationBase* lpApp);
	~CControlDoc();
	void End(void);

//	void OnCloseButton(void);

	void OnControlButton(int n);
	int GetPlayCommand(void);


	int GetFilmPlayMode(void);
private:

};


#endif
/*_*/

