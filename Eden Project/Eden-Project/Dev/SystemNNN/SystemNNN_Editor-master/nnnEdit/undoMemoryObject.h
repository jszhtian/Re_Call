#if !defined __TINYAN_NNNEDIT_UNDOMEMORYOBJECT__
#define __TINYAN_NNNEDIT_UNDOMEMORYOBJECT__


#define UNDO_TYPE_MESSAGE 1
#define UNDO_TYPE_KOMA 2
#define UNDO_TYPE_FILM 3
#define UNDO_TYPE_FILMCASE 4
#define UNDO_TYPE_COMMAND 10
#define UNDO_TYPE_STORY 11
#define UNDO_TYPE_STORYBOOK 12

#define UNDO_DATA_INSERT 1
#define UNDO_DATA_DELETE 2
#define UNDO_DATA_MODIFY 3
#define UNDO_DATA_ALL 4


class CUndoMemoryObject
{
public:
	CUndoMemoryObject();
	virtual ~CUndoMemoryObject();
	void End(void);

	void Clear(int type = 0,int dataType = 0,int startN = 0,int endN = 0);
	void Start(void);

	BOOL Read(LPVOID ptr,size_t elementSize,size_t count);
	BOOL Write(LPVOID ptr,size_t elementSize,size_t count);

	BOOL CheckAndExpandWork(size_t elementSize,size_t count);
	BOOL CheckSize(size_t elementSize,size_t count);
	BOOL CheckDataExist(size_t elementSize,size_t count);

	int GetUndoType(void){return m_undoType;}
	int GetUndoDataType(void){return m_undoDataType;}
	int GetUndoStartN(void){return m_undoStartN;}
	int GetUndoEndN(void){return m_undoEndN;}

private:
	unsigned int m_bufferSize;
	unsigned int m_dataSize;
	unsigned int m_readPointer;
	char* m_buffer;

	int m_undoType;
	int m_undoDataType;
	int m_undoStartN;
	int m_undoEndN;


};

#endif
