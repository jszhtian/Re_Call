#include "NutScript.h"
#include <iostream>
#include <fstream>
FILE* fp;
ofstream TXT;
const char* version = "0.02";
const char* nutVersion = "2.2.4";

void Usage( void )
{
	std::cout << "NutCracker Squirrel script decompiler, ver " << version << std::endl;
	std::cout << "for binary nut file version " << nutVersion << std::endl;
	std::cout << std::endl;
	std::cout << "  Usage:" << std::endl;
	std::cout << "    nutcracker [options] <file to decompile>" << std::endl;
	std::cout << "    nutcracker -cmp <file1> <file2>" << std::endl;
	std::cout << std::endl;
	std::cout << "  Options:" << std::endl;
	std::cout << "   -h         Display usage info" << std::endl;
	std::cout << "   -cmp       Compare two binary files" << std::endl;
	std::cout << "   -d <name>  Display debug decompilation for function" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}


int Compare( const char* file1, const char* file2, bool general )
{
	try
	{
		NutScript s1, s2;
		s1.LoadFromFile(file1);
		s2.LoadFromFile(file2);

		if (general)
		{
			std::ofstream nullStream("null");
			bool result = s1.GetMain().DoCompare(s2.GetMain(), "", nullStream);

			if (result)
				std::cout << "[         ]";
			else
				std::cout << "[! ERROR !]";

			std::cout << " : " << file1 << std::endl;

			return result ? 0 : -1;
		}
		else
		{
			bool result = s1.GetMain().DoCompare(s2.GetMain(), "", std::cout);
			std::cout << std::endl << "Result: " << (result ? "Ok" : "ERROR") << std::endl;

			return result ? 0 : -1;
		}
	}
	catch( std::exception& ex )
	{
		std::cout << "Error: " << ex.what() << std::endl;
		return -1;
	}
}


void DebugFunctionPrint( const NutFunction& function )
{
	g_DebugMode = true;
	function.GenerateFunctionSource(0, std::cout);
}


int Decompile( const char* file, const char* debugFunction )
{
	try
	{
		NutScript script;
		script.LoadFromFile(file);

		if (debugFunction)
		{
			if (0 == strcmp(debugFunction, "main"))
			{
				DebugFunctionPrint(script.GetMain());
				fclose(fp);
				return 0;
			}
			else
			{
				const NutFunction* func = script.GetMain().FindFunction(debugFunction);
				if (!func)
				{
					std::cout << "Unable to find function \"" << debugFunction << "\"." << std::endl;
					fclose(fp);
					return -2;
				}

				DebugFunctionPrint(*func);
				fclose(fp);
				return 0;
			}
		}

		script.GetMain().GenerateBodySource(0, std::cout);
	}
	catch( std::exception& ex )
	{
		std::cout << "Error: " << ex.what() << std::endl;
		fclose(fp);
		return -1;
	}
	fclose(fp);
	return 0;
}

static void mCreateFile(char* FileName)
{
	fp = fopen(FileName, "wb");
	if (!fp)
	{
		//MessageBox(0, L"CreateTextFileError.", L"Aym", 0);
		cout << "CreateTextFileError." << endl;
		system("pause");
		exit(-1);
	}
}


int main( int argc, char* argv[] )
{
	const char* debugFunction = NULL;
	/*
	for( int i = 1; i < argc; ++i)
	{
		if (0 == _stricmp(argv[i], "-h"))
		{
			Usage();
			return 0;
		}
		else if (0 == _stricmp(argv[i], "-d"))
		{
			if ((argc - i) < 2)
			{
				Usage();
				return -1;
			}
			debugFunction = argv[i + 1];
			i += 1;
		}
		else if (0 == _stricmp(argv[i], "-cmp"))
		{
			if ((argc - i) < 3)
			{
				Usage();
				return -1;
			}
			return Compare(argv[i + 1], argv[i + 2], false);
		}
		else if (0 == _stricmp(argv[i], "-cmpg"))
		{
			if ((argc - i) < 3)
			{
				Usage();
				return -1;
			}
			return Compare(argv[i + 1], argv[i + 2], true);
		}
		else
		{
			string fnm(argv[i]);
			string OutName = fnm + ".txt";
			TXT2.open(OutName.c_str(), ios::out | ios::app | ios::binary, _SH_DENYNO);
			int ret = Decompile(argv[i], debugFunction);
			TXT2.close();
			return ret;
		}
	}
	*/
	//TXT2.close();
	//Usage();
	//return -1;
	//char* FileName = argv[1];
	char* FileName = (char*)"0010_10eup_¥Á¥å©`¥È¥ê¥¢¥ë.nut";
	string fnm(FileName);
	string OutName = fnm + ".txt";
	string Fn = fnm + "_disam.dsm";
	//fp = fopen(OutName.c_str(), "wb");
	mCreateFile((char*)OutName.c_str());
	TXT.open(Fn.c_str());
	int ret = Decompile(FileName, debugFunction);
	return ret;
}
