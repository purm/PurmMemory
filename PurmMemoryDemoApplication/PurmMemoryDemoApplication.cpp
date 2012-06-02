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

	cin.get();
	delete instance;

	cin.get();

	return 0;
}

