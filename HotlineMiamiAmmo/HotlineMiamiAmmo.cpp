
#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>


uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName) {
    uintptr_t modBaseAddr = 0;

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

    if (hSnap != INVALID_HANDLE_VALUE) {
        MODULEENTRY32W modEntry; 
        modEntry.dwSize = sizeof(modEntry); 

        if (Module32FirstW(hSnap, &modEntry)) {
            do {
                if (wcscmp(modEntry.szModule, modName) == 0) {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32NextW(hSnap, &modEntry));
        }
    }

    CloseHandle(hSnap);
    return modBaseAddr;
}
int main()
{
    HWND hwnd = FindWindowA(NULL, "Hotline Miami");
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    uintptr_t base = GetModuleBaseAddress(pid, L"HotlineGL.exe");
    HANDLE miami_client = OpenProcess(PROCESS_ALL_ACCESS, true, pid); //0x00257850 + 0x30 + 0x50 + 0x0 + 0xD4 + 0x4 + 0x8
    DWORD ammo = 0;
    ReadProcessMemory(miami_client, (LPCVOID)(base + 0x00257850), &ammo, sizeof(ammo), NULL);
    ReadProcessMemory(miami_client, (LPCVOID)(ammo + 0x30), &ammo, sizeof(ammo), NULL);
    ReadProcessMemory(miami_client, (LPCVOID)(ammo + 0x50), &ammo, sizeof(ammo), NULL);
    ReadProcessMemory(miami_client, (LPCVOID)(ammo + 0x0), &ammo, sizeof(ammo), NULL);
    ReadProcessMemory(miami_client, (LPCVOID)(ammo + 0xD4), &ammo, sizeof(ammo), NULL);
    ReadProcessMemory(miami_client, (LPCVOID)(ammo + 0x4), &ammo, sizeof(ammo), NULL);
    DWORD ammo_new = 999;

    while (true) {
        try {
            if(!WriteProcessMemory(miami_client, (void*)(ammo + 0x8), &ammo_new, 4, NULL))
            {
                throw 505;
            }
        }
        catch (int error) {
            std::cout << error;
        }
        Sleep(100);
    }
}
