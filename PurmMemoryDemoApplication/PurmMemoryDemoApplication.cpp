// PurmMemoryDemoApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

#include <PurmMemory\MemoryManager.h>

int _tmain(int argc, _TCHAR* argv[])
{
	PurmMemory::MemoryManager* instance = new PurmMemory::MemoryManager();

	cout << "MemoryManager created @ " << instance << endl;
	
	//Open Process
	cout << "Process open " << ((instance->OpenProcess(_T("pidgin.exe")) == true) ? "succeded" : "failed") << endl;

	//Reading Example 1
	char* buffer = new char[8];
	cout << "MemoryRead " << ((instance->ReadMemory(0x0028F59E, buffer, 6) == true) ? "succeded" : "failed") << endl;
	buffer[6] = '\0';
	cout << "Buffer: " << buffer << endl;
	delete[] buffer;

	//Reading Example 2
	cout << "MemoryRead resulted: " << instance->ReadMemory<int>(0x01EA0490) << endl;

	cin.get();
	delete instance;

	cin.get();

	return 0;
}

