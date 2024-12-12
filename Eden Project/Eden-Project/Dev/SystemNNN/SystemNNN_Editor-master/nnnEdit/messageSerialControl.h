//
//
//

#if !defined __NNNEDIT_MESSAGESERIALCONTROL__
#define __NNNEDIT_MESSAGESERIALCONTROL__

class CMyApplication;
class CNameList;
class CMessageSerialControl
{
public:
	CMessageSerialControl(CMyApplicationBase* app);
	~CMessageSerialControl();
	void End(void);


	BOOL SetAllMessageSerial(BOOL updateFlag = FALSE);
	BOOL SetMessageSerial(BOOL updateFlag = FALSE);

private:
	CMyApplicationBase* m_app;
	CNameList* m_projectList;
	int GetMaxSerial(void);
	int GetMaxSelectSerial(void);
	int m_selectSerialID;
};



#endif
/*_*/

