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

	//Writing Example 1
	char* writeBuffer = "hallo!";
	cout << "MemoryWrite " << ((instance->WriteMemory(0x0028F59E, writeBuffer, 6) == true) ? "succeded" : "failed") << endl;

	//Reading Example 1
	char* readBuffer = new char[8];
	cout << "MemoryRead " << ((instance->ReadMemory(0x0028F59E, readBuffer, 6) == true) ? "succeded" : "failed") << endl;
	readBuffer[6] = '\0';
	cout << "Buffer: " << readBuffer << endl;
	delete[] readBuffer;

	//Reading Example 2
	cout << "MemoryRead resulted: " << instance->ReadMemory<int>(0x01EA0490) << endl;

	cin.get();
	delete instance;

	cin.get();

	return 1;
}

