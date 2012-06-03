#include "stdafx.h"
#include "MemoryManager.h"

namespace PurmMemory {
	MemoryManager::MemoryManager(void)
	{
		#if(_DEBUG)
			::OutputDebugString(_T("MemoryManager::MemoryManager(): Object Instance created\r\n"));
		#endif
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

	//sets the process handle
	bool MemoryManager::UseOpenedProcessHandle(HANDLE processHandle) {
		DWORD exitCode;
		if(!::GetExitCodeProcess(processHandle, &exitCode)) {
			if(::GetLastError() != STILL_ACTIVE) {
				this->_processHandle = processHandle;
				return true;
			}
		}

		#if(_DEBUG)
			::OutputDebugString(_T("MemoryManager::UseOpenedProcessHandle(...): Process of the Handle is not running\r\n"));
		#endif

		return false;
	}

	//closes the process handle
	bool MemoryManager::CloseProcess() {
		if(this->_processHandle != INVALID_HANDLE_VALUE) {
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
		} else {
			#if(_DEBUG)
				::OutputDebugString(_T("MemoryManager::CloseProcess(...): invalid HANDLE\r\n"));
			#endif

			return false;
		}
	}

	//get/set accessors
	DWORD MemoryManager::getProcessID() {
		return this->_processId;
	}

	//reads from memory
	bool MemoryManager::ReadMemory(void* address, void* buffer, int size) {
		SIZE_T read;
		if(!::ReadProcessMemory(this->_processHandle, address, buffer, size, &read)) {
			#if(_DEBUG)
				::OutputDebugString(_T("MemoryManager::ReadMemory(...): ReadProcessMemory returned false\r\n"));
			#endif

			return false;
		} else {
			if(read != size) {
				#if(_DEBUG)
					::OutputDebugString(_T("MemoryManager::ReadMemory(...): readen size not as it should\r\n"));
				#endif

				return false;
			} else {
				return true;
			}
		}
	}

	bool MemoryManager::WriteMemory(void* address, void* buffer, int size) {
		SIZE_T written;
		if(!::WriteProcessMemory(this->_processHandle, address, buffer, size, &written)) {
			#if(_DEBUG)
				::OutputDebugString(_T("MemoryManager::WriteMemory(...): WriteProcessMemory returned false\r\n"));
			#endif

			return false;
		} else {
			if(written != size) {
				#if(_DEBUG)
					::OutputDebugString(_T("MemoryManager::WriteMemory(...): written size not as it should\r\n"));
				#endif

				return false;
			} else {
				return true;
			}
		}
	}

	//gets a process by its Name; returns NULL if fails or not found
	DWORD MemoryManager::GetProcessIdByName(TCHAR* processName) {
		HANDLE snapHandle = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
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
						::CloseHandle(snapHandle);
						snapHandle = NULL;
						#if(_DEBUG)
							::OutputDebugString(_T("MemoryManager::GetProcessIdByName(...): Process ID was resolved successfully\r\n"));
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

			::CloseHandle(snapHandle);
			snapHandle = NULL;
			return NULL;
		}
	}

	//injects a dll into the process
	bool MemoryManager::InjectDll(TCHAR* path) {
		size_t pathLenght = _tcsclen(path);
		if(!pathLenght) {
			#if(_DEBUG)
				::OutputDebugString(_T("MemoryManager::InjectDll(...): pathLenght was 0"));
			#endif

			return false;
		}

		HMODULE module = ::GetModuleHandle(_T("kernel32.dll"));
		if(!module) {
			#if(_DEBUG)
				::OutputDebugString(_T("MemoryManager::InjectDll(...): Kernel32 Module could not be found"));
			#endif

			return false;
		}

		#ifdef _UNICODE
			void* loadLibraryAddress = ::GetProcAddress(module, "LoadLibraryW");
		#else
			void* loadLibraryAddress = ::GetProcAddress(module, "LoadLibraryA");
		#endif

		if(!loadLibraryAddress) {
			#if(_DEBUG)
				::OutputDebugString(_T("MemoryManager::InjectDll(...): LoadLibrary Address could not be found"));
			#endif

			return false;
		}

		void* address = this->AllocateMemory(pathLenght);
		if(!address) {
			#if(_DEBUG)
				::OutputDebugString(_T("MemoryManager::InjectDll(...): Remote Memory Allocating failed"));
			#endif

			return false;
		}

		if(!this->WriteMemory(address, path, pathLenght)) {
			#if(_DEBUG)
				::OutputDebugString(_T("MemoryManager::InjectDll(...): Memory Writing failed"));
			#endif

			this->FreeMemory(address, pathLenght);

			return false;
		}

		HANDLE threadHandle;
		DWORD threadId;
		threadHandle = this->CreateThread((LPTHREAD_START_ROUTINE)loadLibraryAddress, address, &threadId);

		if(!threadHandle) {
			#if(_DEBUG)
				::OutputDebugString(_T("MemoryManager::InjectDll(...): CreateRemoteThread failed"));
			#endif

			this->FreeMemory(address, pathLenght);

			return false;
		}

		if(::WaitForSingleObject(threadHandle, INFINITE) == WAIT_FAILED) {
			#if(_DEBUG)
				::OutputDebugString(_T("MemoryManager::InjectDll(...): WaitForSingleObject(...) -> WAIT_FAILED"));
			#endif

			this->FreeMemory(address, pathLenght);

			return false;
		}

		this->FreeMemory(address, pathLenght);

		return true;
	}

	//injects opcodes into the proccess
	bool MemoryManager::InjectCode(BYTE* opCode) {
		//TODO: implement
		return false;
	}

	//allocates some memory in the target process
	LPVOID WINAPI MemoryManager::AllocateMemory(SIZE_T dwSize, LPVOID lpAddress, DWORD flAllocationType, DWORD flProtect)
	{
		return ::VirtualAllocEx(this->_processHandle, lpAddress, dwSize, flAllocationType, flProtect);
	}

	//frees some memory in the process
	BOOL WINAPI MemoryManager::FreeMemory(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType)
	{
		return ::VirtualFreeEx(this->_processHandle, lpAddress, dwSize, dwFreeType);
	}

	//creates a thread inside the target process
	HANDLE WINAPI MemoryManager::CreateThread(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, LPDWORD lpThreadId, SIZE_T dwStackSize, DWORD dwCreationFlags, LPSECURITY_ATTRIBUTES lpThreadAttributes)
	{
		return ::CreateRemoteThread(this->_processHandle, lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
	}
}