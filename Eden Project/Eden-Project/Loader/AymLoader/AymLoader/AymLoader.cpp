#include <winternl.h>
#include "BlackBone/Process/Process.h"
#include "BlackBone/Process/ProcessModules.h"
#ifdef DEBUG
#pragma comment(lib,"Lib/BlackBoned.lib")
#else
#pragma comment(lib,"Lib/BlackBone.lib")
#endif // DEBUG



bool __stdcall Runinject(wchar_t* DLLFullPath, wchar_t* EXEFullPath)
{
    NTSTATUS status_CreateProcess;
    blackbone::Process _Process;
    status_CreateProcess = _Process.CreateAndAttach(EXEFullPath, true);
    //auto pThread = _Process.threads().getMostExecuted();
    if (!NT_SUCCESS(status_CreateProcess))
    {
        return false;
    }
    //if (!pThread)
    //{
    //    _Process.Resume();
    //    _Process.Detach();
    //    return false;
    //}
    //auto status_Inject=_Process.modules().Inject(DLLFullPath, pThread);
    auto status_Inject = _Process.modules().Inject(DLLFullPath);
    if (!NT_SUCCESS(status_Inject.status))
    {
        _Process.Resume();
        _Process.Detach();
        return false;
    }
    _Process.Resume();
    _Process.Detach();
    return true;


}