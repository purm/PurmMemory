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
	
	//TODO: example stuff

	cout << "Process open " << ((instance->OpenProcess(_T("pidgin.exe")) == true) ? "succeded" : "failed") << endl;

	char* buffer = new char[8];
	cout << "MemoryRead " << ((instance->ReadMemory(0x0028F59E, buffer, 6) == true) ? "succeded" : "failed") << endl;
	buffer[6] = '\0';
	cout << "Buffer: " << buffer << endl;

	cin.get();
	delete instance;

	cin.get();

	return 0;
}

