//
// selectobjectonly.h
//

#if !defined __TINYANA_NNNEDIT_SELECTOBJECTONLY__
#define __TINYANA_NNNEDIT_SELECTOBJECTONLY__


class CSelectObjectOnly
{
public:
	CSelectObjectOnly();
	~CSelectObjectOnly();
	void End(void);

	HGDIOBJ Sel(HDC hdc, HGDIOBJ obj);

private:

};



#endif
/*_*/

