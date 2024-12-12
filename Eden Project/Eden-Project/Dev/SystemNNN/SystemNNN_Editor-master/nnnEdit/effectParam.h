//
//
//

#if !defined __NNNEDIT_EFFECTPARAM__
#define __NNNEDIT_EFFECTPARAM__


class CNameList;
class CEffectParam
{
public:
	CEffectParam(LPSTR filename);
	virtual ~CEffectParam();
	void End(void);

	LPSTR GetEffectName(void);
	int GetNeedParaKosuu(void){return m_needParaKosuu;}
	int GetParaKosuu(void){return m_paraKosuu;}
	LPSTR GetMainDesc(int n);

	LPSTR GetParamName(int n);
	int GetParaMin(int n);
	int GetParaMax(int n);
	int GetParaDefault(int n);
	int GetParaEditType(int n);
	int GetParaEditPara(int n);
	LPSTR GetParamDesc(int n,int k);

private:
	CNameList* m_nameList;
	int m_needParaKosuu;
	int m_paraKosuu;

	int* m_para;

};






#endif
/*_*/

