#ifndef __CCImage__
#define __CCImage__

#include <afx.h>
#include <atlimage.h>

class CCImage : public CImage
{
public:
	HRESULT Load(IStream* pStream, bool bPreMultiply);
};

#endif
