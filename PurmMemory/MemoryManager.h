#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>

namespace PurmMemory {
	class MemoryManager
	{
	public:
		MemoryManager(void);
		~MemoryManager(void);

		//enums
		enum { _ERROR_OPENPROCESS_FAILED };

		//get/set accessors
		DWORD getProcessID();

		bool OpenProcess(DWORD processId, DWORD processRights = PROCESS_ALL_ACCESS);
		bool OpenProcess(TCHAR* processName, DWORD processRights = PROCESS_ALL_ACCESS);
		bool CloseProcess();
		bool UseOpenedProcessHandle(HANDLE processHandle);

		bool ReadMemory(DWORD address, void* buffer, int size);
		template<class T> T ReadMemory(DWORD address) {
			T buffer;
			if(this->ReadMemory(address, &buffer, sizeof(T))) {
				return buffer;
			} else {
				return NULL;
			}
		}

	private:
		HANDLE _processHandle;
		DWORD _processId;

		DWORD GetProcessIdByName(TCHAR* processName);
	};
}