#include <Windows.h>
#include "PEFile.h"

int wmain(int argc, WCHAR* argv[])
{
	PEFile pe(L"64BitSample.exe");

	// Add "MessageBoxA" & "ShowWindow" functions to the import table
	char* functions[] = { "LinkProc" };
	pe.addImport("dllSample64.dll", functions, 1);

	// Add a new section named ".at4re" with size "0x1000" byte
	pe.addSection(".link", 0x1000, false);

	// Save the modified file
	pe.saveToFile(L"64BitSample1.exe");
	return 0;
}

