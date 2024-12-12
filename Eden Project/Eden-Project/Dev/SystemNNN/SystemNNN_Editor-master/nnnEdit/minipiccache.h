//
// minipiccache.e
//

#if !defined __NNNEDIT_MINIPICCACHE__
#define __NNNEDIT_MINIPICCACHE__

class CPicture;
class CMiniPicCache
{
public:
	CMiniPicCache();
	~CMiniPicCache();
	void End(void);

	int GetBufferNumber(LPSTR filename,int nStart);
	int* GetPicture(LPSTR filename,int nStart = 0);
	int SearchFile(LPSTR filename,int nStart = 0);

	void PictureToMini(int n);
private:
	CPicture* m_pic;

	int m_picKosuu;
	int m_picKosuuMax;

	int** m_lplpBuffer;

	void AddDIBBuffer(int n);

	char* m_lpName;


};


#endif
/*_*/

