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

		bool ReadMemory(void* address, void* buffer, int size);
		template<class T> T ReadMemory(void* address) {
			T buffer;
			if(this->ReadMemory(address, &buffer, sizeof(T))) {
				return buffer;
			} else {
				::SetLastError(MemoryManager::_ERROR_READMEMORY_FAILED);
				return NULL;
			}
		}

		bool WriteMemory(void* address, void* buffer, int size);
		template<class T> bool WriteMemory(void* address, T buffer) {
			return this->WriteMemory(address, &buffer, sizeof(T));
		}

		bool InjectDll(TCHAR* path);
		bool InjectCode(BYTE* opCode);

		LPVOID WINAPI AllocateMemory(SIZE_T dwSize, LPVOID lpAddress = NULL, DWORD flAllocationType = MEM_COMMIT, DWORD flProtect = PAGE_EXECUTE_READWRITE);
		BOOL WINAPI FreeMemory(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType = MEM_RELEASE);
		HANDLE WINAPI CreateThread(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, LPDWORD lpThreadId, DWORD dwCreationFlags = 0, SIZE_T dwStackSize = 0, LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL);

	private:
		HANDLE _processHandle;
		DWORD _processId;

		DWORD GetProcessIdByName(TCHAR* processName);
	};
}