#pragma once

#include <Windows.h>
#include <tchar.h>

namespace PurmMemory {
	class MemoryManager
	{
	public:
		MemoryManager(void);
		~MemoryManager(void);

		bool OpenProcess(DWORD processId, DWORD processRights = PROCESS_ALL_ACCESS);
		bool CloseProcess();
		bool UseOpenedProcessHandle(HANDLE processHandle);

		//get/set accessors
		DWORD getProcessID();

		//enums
		enum { _ERROR_OPENPROCESS_FAILED };
	private:
		HANDLE _processHandle;
		DWORD _processId;
	};
}