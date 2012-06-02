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
}