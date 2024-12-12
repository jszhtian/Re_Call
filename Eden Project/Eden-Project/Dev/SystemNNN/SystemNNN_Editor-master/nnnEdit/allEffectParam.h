//
//
//

#if !defined __NNNEDIT_ALLEFFECTPARAM__
#define __NNNEDIT_ALLEFFECTPARAM__


class CEffectParam;

class CAllEffectParam
{
public:
	CAllEffectParam();
	~CAllEffectParam();
	void End(void);

	CEffectParam* GetEffectParam(int n){return m_effectParam[n];}

private:
	CEffectParam* m_effectParam[256];
	CNameList* m_effectParamList;

};




#endif
/*_*/

