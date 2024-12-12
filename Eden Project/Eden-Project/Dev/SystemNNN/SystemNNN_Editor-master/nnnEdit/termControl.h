#if !defined __NNNEDIT_TERMCONTROL__
#define __NNNEDIT_TERMCONTROL__



class CNameList;
class CTermControl
{
public:
	CTermControl();
	~CTermControl();
	void End(void);

	void SetTermList(CNameList* list);
	void Start(LPSTR mes);
	int GetNextTerm(void);
private:
	int m_termKosuu;
	CNameList* m_termList;
	LPSTR m_checkMessage;
	int m_nowCheck;


};


#endif
/*_*/
