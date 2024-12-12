//
// mysave.h
//

#if !defined __TINYAN_NERUNE_MYSAVE__
#define __TINYAN_NERUNE_MYSAVE__

class CMySave
{
public:
	CMySave();
	~CMySave();
	void End(void);

	BOOL Save(LPSTR filename, void* headerBuffer, void* dataBuffer, int dataSize, int type);


private:
};

#endif
/*_*/

