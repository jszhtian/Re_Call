#include <Windows.h>
#include "PEFile.h"

int wmain(int argc, WCHAR* argv[])
{

	PEFile pe(L"Shinku.exe");

	// Add "MessageBoxA" & "ShowWindow" functions to the import table
	char* functions[] = { "LinkProc"};
	pe.addImport("dllSample32.dll", functions, 1);

	// Add a new section named ".at4re" with size "0x1000" byte
	pe.addSection(".CryS", 0x1000, true);

	// Save the modified file
	pe.saveToFile(L"Shinku1.exe");
	return 0;
}

