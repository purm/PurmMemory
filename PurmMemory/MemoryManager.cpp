#include "stdafx.h"
#include "MemoryManager.h"

namespace PurmMemory {
	MemoryManager::MemoryManager(void)
	{
	}


	MemoryManager::~MemoryManager(void)
	{
		#if(_DEBUG)
			::OutputDebugString(_T("MemoryManager::~MemoryManager(): Deleting Object Instance\r\n"));
		#endif

		this->CloseProcess();

		#if(_DEBUG)
			::OutputDebugString(_T("MemoryManager::~MemoryManager(): Object Instance deleted\r\n"));
		#endif
	}

	//opens a process for reading/writing/other stuff
	bool MemoryManager::OpenProcess(DWORD processId, DWORD processRights) {
		this->_processHandle = ::OpenProcess(processRights, false, processId);
		if(!this->_processHandle) {
			::SetLastError(MemoryManager::_ERROR_OPENPROCESS_FAILED);
			#if(_DEBUG)
				::OutputDebugString(_T("MemoryManager::OpenProcess(...): OpenProcess returned false\r\n"));
			#endif
			return false;
		}

		#if(_DEBUG)
			else {
				::OutputDebugString(_T("MemoryManager::OpenProcess(...): OpenProcess returned true\r\n"));
			}
		#endif

		return true;
	}

	//opens a process by its name
	bool MemoryManager::OpenProcess(TCHAR* processName, DWORD processRights) {
		return this->OpenProcess(this->GetProcessIdByName(processName), processRights);
	}

	//closes the process handle
	bool MemoryManager::CloseProcess() {
		BOOL success = ::CloseHandle(this->_processHandle);
		#if(_DEBUG)
			if(!success) {
				::OutputDebugString(_T("MemoryManager::CloseProcess(): CloseProcess returned false\r\n"));
			} else {
				::OutputDebugString(_T("MemoryManager::CloseProcess(): CloseProcess returned true\r\n"));
			}
		#endif

		this->_processHandle = NULL;
		return (success != 0);
	}

	//get/set accessors
	DWORD MemoryManager::getProcessID() {
		return this->_processId;
	}

	//gets a process by its HANDLE; returns NULL if fails or not found
	DWORD MemoryManager::GetProcessIdByName(TCHAR* processName) {
		HANDLE snapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if(snapHandle == INVALID_HANDLE_VALUE) {
			#if(_DEBUG)
				::OutputDebugString(_T("MemoryManager::GetProcessIdByName(...): CreateToolhelp32Snapshot returned an INvalid HANDLE\r\n"));
			#endif

			return NULL;
		} else {
			#if(_DEBUG)
				::OutputDebugString(_T("MemoryManager::GetProcessIdByName(...): CreateToolhelp32Snapshot returned an VALID HANDLE\r\n"));
			#endif

			PROCESSENTRY32 currentEntry;
			currentEntry.dwSize = sizeof(PROCESSENTRY32);

			if(Process32First(snapHandle, &currentEntry)) {
				do {
					if(!_tcscmp(processName, currentEntry.szExeFile)) {
						CloseHandle(snapHandle);
						snapHandle = NULL;
						#if(_DEBUG)
							::OutputDebugString(_T("MemoryManager::GetProcessIdByName(...): Process ID was refactored successfully\r\n"));
						#endif
						return currentEntry.th32ProcessID;
					}
				} while(Process32Next(snapHandle, &currentEntry));
			}
			#if(_DEBUG)
				else {
					::OutputDebugString(_T("MemoryManager::GetProcessIdByName(...): Process32First returned null"));
				}
			#endif

			CloseHandle(snapHandle);
			snapHandle = NULL;
			return NULL;
		}
	}
}