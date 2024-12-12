//
// Contedoc.h
//

#if !defined __TINYAN_NNNEDIT_CONTEDOC__
#define __TINYAN_NNNEDIT_CONTEDOC__

#include "mydocument.h"

class CConteDoc : public CMyDocument
{
public:
	CConteDoc(CMyApplicationBase* lpApp);
	~CConteDoc();
	void End(void);

//	HBITMAP GetPicture(int n);
	void OnClickFilm(int n);

	int GetScrollObjectKosuu(int vh);
	int GetNowSelectNumber(void);
	CFilmData* GetNowSelectFilm(void);

	HBITMAP GetHBitmap(int placeNumber);


	void ReCalcuScrollPara(void);

	CCase* GetNowSelectCaseObject(void);
	void OnSelectNumber(int n);

private:
	void SetWriteStatus(int layer,int st);


};


#endif
/*_*/

