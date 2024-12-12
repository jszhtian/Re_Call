#include "MultiCoreTask.h"

MultiCoreTask::MultiCoreTask(ULONG Cores)
{
	TaskManager.resize(Cores);
}

MultiCoreTask::~MultiCoreTask()
{
	for (auto it : TaskManager)
	{
		while (it.TaskQueue.empty() == false)
		{
			it.TaskQueue.pop();
		}
	}
	TaskManager.clear();
}

bool MultiCoreTask::PushTask(ULONG CoreId, wstring& FileName, wstring& PluginName)
{
	if (CoreId >= 0 && CoreId < TaskManager.size())
	{
		TaskAtom o;
		o.FileName = FileName;
		o.PluginName = PluginName;
		TaskManager[CoreId].TaskQueue.push(o);
	}
	return false;
}

bool MultiCoreTask::PopTask(ULONG CoreId)
{
	if (CoreId >= 0 && CoreId < TaskManager.size())
	{
		if (TaskManager[CoreId].TaskQueue.empty())
		{
			return false;
		}
		else
		{
			TaskManager[CoreId].TaskQueue.pop();
			return true;
		}
	}
	return false;
}

//get and pop
bool MultiCoreTask::GetTask(ULONG CoreId, wstring& FileName, wstring& PluginName)
{
	if (CoreId >= 0 && CoreId < TaskManager.size())
	{
		if (TaskManager[CoreId].TaskQueue.empty())
		{
			return false;
		}
		else
		{
			FileName = TaskManager[CoreId].TaskQueue.front().FileName;
			PluginName = TaskManager[CoreId].TaskQueue.front().PluginName;
			TaskManager[CoreId].TaskQueue.pop();
			return true;
		}
	}
	return false;
}

void MultiCoreTask::Clear()
{
	for (auto it : TaskManager)
	{
		while (it.TaskQueue.empty() == false)
		{
			it.TaskQueue.pop();
		}
	}
}
