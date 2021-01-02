#include "Hooks.h"
#include "Utilities.h"

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, LPVOID Reserved)
{
    if (Reason == DLL_PROCESS_ATTACH) {
        if (GetModuleHandleW(L"steamservice")) {
            PBYTE SteamPattern = FindPattern(L"steamservice", "\x74\x47\x6A\x01\x6A", 0);
            if (SteamPattern) {
                DWORD Offset;
                VirtualProtect(SteamPattern, 1, PAGE_EXECUTE_READWRITE, &Offset);
                *SteamPattern = 0xEB;
                VirtualProtect(SteamPattern, 1, Offset, &Offset);
                ImportHook(L"steamservice", "kernel32.dll", "LoadLibraryExW", LoadLibraryExW);
                MessageBoxW(NULL, L"Initialization was successful!", L"VAC bypass", MB_OK | MB_ICONINFORMATION);
            }
        } else {
            ImportHook(NULL, "kernel32.dll", "LoadLibraryExW", SteamClient_LoadLibraryExW);
        }
        DisableThreadLibraryCalls(Module);
    }
    return TRUE;
}