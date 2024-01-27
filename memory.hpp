#include <iostream>
#include <windows.h>
#include <string.h>
#include <cstdint>
#include <tlhelp32.h>

class Memory{
private:
    DWORD pID = 0;
    std::uintptr_t dwModuleBaseAddress = 0;

public:

    //the function that gets the process id
    DWORD GetProcId(std::string_view processname) {
        bool hResult;
        PROCESSENTRY32 entry;
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (INVALID_HANDLE_VALUE == hSnapshot) {
            return 0;
        }

        entry.dwSize = sizeof(PROCESSENTRY32);
        hResult = Process32First(hSnapshot, &entry);

        while (hResult) {
            if (strcmp(std::string(processname).c_str(), entry.szExeFile) == 0) {
                pID = entry.th32ProcessID;
                break;
            }
            hResult = Process32Next(hSnapshot, &entry);
        }
        CloseHandle(hSnapshot);
        return pID;
    }

    //the funtion that gets the module base adress
    std::uintptr_t GetModuleBaseAddress(std::string_view modulename, DWORD pId)
    {
		MODULEENTRY32 entry = {0};
		entry.dwSize = sizeof(::MODULEENTRY32);
		const HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
		std::uintptr_t dwModuleBaseAddress = 0;

		while (Module32Next(hSnapshot, &entry)) {
			if (!modulename.compare(entry.szModule)) {
				dwModuleBaseAddress = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
				break;
			}
		}

		if (hSnapshot) {
			CloseHandle(hSnapshot);
        }

		return dwModuleBaseAddress;
    }

    //read/write process memory functions
    template<typename T>
    T ReadMem(HANDLE handle, std::uintptr_t address) {
        T value = 0;
        ReadProcessMemory(handle, (void*)address, &value, sizeof(T), nullptr);
        return value;
    }

    template<typename T>
    T WriteMem(HANDLE handle, std::uintptr_t address, T value) {
        WriteProcessMemory(handle, (void*)address, value, sizeof(T), nullptr);
    }
};