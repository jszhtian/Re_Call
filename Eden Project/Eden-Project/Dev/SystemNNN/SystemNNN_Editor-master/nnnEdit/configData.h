//
//
//

#if !defined __NNNEDIT_CONFIGDATA__
#define __NNNEDIT_CONFIGDATA__



class CNameList;

class CConfigData
{
public:
	CConfigData();
	virtual ~CConfigData();
	void End(void);


	LPSTR GetString(LPSTR name);
	int GetData(LPSTR name);
	void SetString(LPSTR name,LPSTR mes);
	void SetData(LPSTR name,int d);

	void Save(void);

	BOOL CheckLoadOk(void){return m_loadOkFlag;}
	BOOL CheckEdit(void){return m_editFlag;}
	int SearchName(LPSTR name);
private:
	CNameList* m_nameList;
	int m_kosuu;

	BOOL m_loadOkFlag;
	BOOL m_editFlag;

	int SearchAkiOrExpand(LPSTR name);
};





#endif
/*_*/
