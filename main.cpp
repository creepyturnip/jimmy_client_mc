#include <iostream>
#include <windows.h>

#include "memory.hpp"

int main() {
    Memory mem;
    DWORD pID;
    HANDLE handle;
    uintptr_t base;

    mem.GetProcId("javaw.exe");
    handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    base = mem.GetModuleBaseAddress("jvm.dll", pID);

    jmy_init(handle, (void*)base);
}