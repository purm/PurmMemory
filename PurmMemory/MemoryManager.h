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
		enum { _ERROR_OPENPROCESS_FAILED, _ERROR_READMEMORY_FAILED };

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
				::SetLastError(MemoryManager::_ERROR_READMEMORY_FAILED);
				return NULL;
			}
		}

		bool WriteMemory(DWORD address, void* buffer, int size);
		template<class T> bool WriteMemory(DWORD address, T buffer) {
			return this->WriteMemory(address, &buffer, sizeof(T));
		}

		bool InjectDLl(TCHAR* path);
		bool InjectCode(BYTE* opCode);

	private:
		HANDLE _processHandle;
		DWORD _processId;

		DWORD GetProcessIdByName(TCHAR* processName);
	};
}