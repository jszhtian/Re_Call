#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanLib\include\commonmacro.h"

#include "undoMemoryObject.h"

CUndoMemoryObject::CUndoMemoryObject()
{
	m_bufferSize = 0;
	m_dataSize = 0;
	m_readPointer = 0;
	m_buffer = NULL;

	m_bufferSize = 1024*1024*8;
	m_buffer = new char[m_bufferSize];
}

CUndoMemoryObject::~CUndoMemoryObject()
{
	End();
}

void CUndoMemoryObject::End(void)
{
	DELETEARRAY(m_buffer);
}

BOOL CUndoMemoryObject::CheckDataExist(size_t elementSize,size_t count)
{
	if (m_readPointer + elementSize * count > m_dataSize) return FALSE;
	return TRUE;
}

BOOL CUndoMemoryObject::CheckSize(size_t elementSize,size_t count)
{
	if (m_dataSize + elementSize * count > m_bufferSize) return FALSE;
	return TRUE;
}

BOOL CUndoMemoryObject::CheckAndExpandWork(size_t elementSize,size_t count)
{
	if (CheckSize(elementSize,count)) return TRUE;
	//expand x2
	char* tmp = new char[m_bufferSize * 2];
	memcpy(tmp,m_buffer,m_dataSize);
	delete [] m_buffer;
	m_buffer = tmp;
	m_bufferSize *= 2;
	return TRUE;
}

void CUndoMemoryObject::Start(void)
{
	m_readPointer = 0;
}

void CUndoMemoryObject::Clear(	int type ,int dataType ,int startN ,int endN)
{
	m_readPointer = 0;
	m_dataSize = 0;

	m_undoType = type;
	m_undoDataType = dataType;
	m_undoStartN = startN;
	m_undoEndN = endN;
}

BOOL CUndoMemoryObject::Read(LPVOID ptr,size_t elementSize,size_t count)
{
	if (CheckDataExist(elementSize,count))
	{
		memcpy(ptr,m_buffer + m_readPointer,elementSize*count);
		m_readPointer += elementSize*count;

		return TRUE;
	}

	return FALSE;
}

BOOL CUndoMemoryObject::Write(LPVOID ptr,size_t elementSize,size_t count)
{
	while (!CheckAndExpandWork(elementSize,count))
	{
		;
	}

	memcpy(m_buffer+m_dataSize,ptr,elementSize*count);
	m_dataSize += elementSize*count;

	return TRUE;
}


/*_*/


