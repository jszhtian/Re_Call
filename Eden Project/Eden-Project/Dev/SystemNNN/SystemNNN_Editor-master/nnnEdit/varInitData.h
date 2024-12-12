#if !defined __NNNEDIT_VARINITDATA__
#define __NNNEDIT_VARINITDATA__


class CNameList;
class CVarInitData
{
public:
	CVarInitData();
	~CVarInitData();
	void End(void);

	BOOL Load(LPSTR filename);
	BOOL Save(LPSTR filename);

	int GetData(int n);
	void SetData(int n,int d);

	BOOL CheckModify(void){return m_editFlag;}
private:
	CNameList* m_list;
	int m_data[2200];
	BOOL m_editFlag;
};



#endif
/*_*/

