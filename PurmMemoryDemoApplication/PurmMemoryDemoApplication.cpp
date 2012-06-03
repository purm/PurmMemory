// PurmMemoryDemoApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <tchar.h>
#include <direct.h>

using namespace std;

#include <PurmMemory\MemoryManager.h>

int _tmain(int argc, _TCHAR* argv[])
{
	PurmMemory::MemoryManager* instance = new PurmMemory::MemoryManager();

	wcout << "MemoryManager created @ " << instance << endl;
	
	//Open Process
	wcout << "Process open " << ((instance->OpenProcess(_T("pidgin.exe")) == true) ? "succeded" : "failed") << endl;

	//Writing Example 1
	char* writeBuffer = "hallo!";
	wcout << "MemoryWrite " << ((instance->WriteMemory((void*)0x0028F59E, writeBuffer, 6) == true) ? "succeded" : "failed") << endl;

	//Writing Example 2
	wcout << "MemoryWrite " << ((instance->WriteMemory<int>((void*)0x01EA0490, 1337) == true) ? "succeded" : "failed") << endl;

	//Reading Example 1
	char* readBuffer = new char[8];
	wcout << "MemoryRead " << ((instance->ReadMemory((void*)0x0028F59E, readBuffer, 6) == true) ? "succeded" : "failed") << endl;
	readBuffer[6] = '\0';
	wcout << "Buffer: " << readBuffer << endl;
	delete[] readBuffer;

	//Reading Example 2
	wcout << "MemoryRead resulted: " << instance->ReadMemory<int>((void*)0x01EA0490) << endl;

	//DLL injection example
	TCHAR* pathBuff = new TCHAR[MAX_PATH];
	_tgetcwd(pathBuff, MAX_PATH);
	_tcscat(pathBuff, _T("\\PurmMemoryDemoInjectionDll.dll"));

	wcout << "injection " << ((instance->InjectDll(pathBuff) == true) ? "succeded" : "failed") << endl;

	cin.get();
	delete instance;

	cin.get();

	return 1;
}

