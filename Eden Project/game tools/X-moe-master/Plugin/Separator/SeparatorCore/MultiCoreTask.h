#ifndef _MultiCoreTask_
#define _MultiCoreTask_

#include <Windows.h>
#include <string>
#include <queue>
#include <vector>

using std::wstring;
using std::queue;
using std::vector;


typedef struct TaskAtom
{
	TaskAtom& operator = (TaskAtom& o)
	{
		this->FileName = o.FileName;
		this->PluginName = o.PluginName;
		return *this;
	}
	wstring FileName;
	wstring PluginName;
}TaskAtom;


typedef struct TaskInfo
{
	queue<TaskAtom> TaskQueue;
}TaskInfo, *pTaskInfo;

class MultiCoreTask
{
public:
	MultiCoreTask(ULONG Cores);
	~MultiCoreTask();

	void Clear();

	bool PushTask(ULONG CoreId, wstring& FileName, wstring& PluginName);
	bool PopTask(ULONG CoreId);
	//get and pop
	bool GetTask(ULONG CoreId, wstring& FileName, wstring& PluginName);
private:
	vector<TaskInfo> TaskManager;
};

#endif
