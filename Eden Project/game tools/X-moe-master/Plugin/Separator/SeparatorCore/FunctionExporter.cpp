#include "FunctionExporter.h"
#include "HashSearcher.h"

static tTJSHashTable<wstring, void *> ExportFuncs;
static bool ExportFuncsInit = false;


void AddExportFunction(const WCHAR *name, void *ptr)
{
	ExportFuncs.Add(name, ptr);
}

static void TVPInitExportFuncs()
{
	if (ExportFuncsInit) return;
	ExportFuncsInit = true;


	// Export functions
	TVPExportFunctions();
}

struct FunctionExporter : InterfaceFunctionExporter
{
	void*   __stdcall QueryFunction(const WCHAR* func);
	HRESULT __stdcall AddTask();
};

