#include <Windows.h>

#include "Hooks.h"
#include "Utilities.h"

HMODULE WINAPI LoadLibraryExW(LPCWSTR Library, HANDLE File, DWORD Flags)
{
    HMODULE Result = LoadLibraryExW(Library, File, Flags);

    ImportHook(Library, "kernel32.dll", "GetProcAddress", GetProcAddress);
    ImportHook(Library, "kernel32.dll", "GetSystemInfo", GetSystemInfo);

    return Library;
}

HMODULE WINAPI SteamClient_LoadLibraryExW(LPCWSTR Library, HANDLE File, DWORD Flags)
{
    HMODULE Result = LoadLibraryExW(Library, File, Flags);

    if (wcsstr(Library, L"steamui.dll")) {
        ImportHook(Library, "kernel32.dll", "LoadLibraryExW", SteamClient_LoadLibraryExW);
    } else if (wcsstr(Library, L"steamservice.dll")) {
        PBYTE SteamPattern = FindPattern(L"steamservice", "\x74\x47\x6A\x01\x6A", 0);
        if (SteamPattern) {
            DWORD Offset;
            VirtualProtect(SteamPattern, 1, PAGE_EXECUTE_READWRITE, &Offset);
            *SteamPattern = 0xEB;
            VirtualProtect(SteamPattern, 1, Offset, &Offset);
            ImportHook(L"steamservice", "kernel32.dll", "LoadLibraryExW", LoadLibraryExW);
            MessageBoxW(NULL, L"Initialization was successful!", L"Arizona's VAC Bypass", MB_OK | MB_ICONINFORMATION);
        }
    }
    return Result;
}

FARPROC WINAPI GetProcAddress(HMODULE Module, LPCSTR ProcessName)
{
    FARPROC Result = GetProcAddress(Module, ProcessName);
 
    if (Result) {
        if (!strcmp(ProcessName, "GetProcAddress"))
            return (FARPROC)GetProcAddress;
        else if (!strcmp(ProcessName, "GetSystemInfo"))
            return (FARPROC)GetSystemInfo;
        else if (!strcmp(ProcessName, "GetVersionExA"))
            return (FARPROC)GetVersionExA;
        else if (!strcmp(ProcessName, "GetSystemDirectoryW"))
            return (FARPROC)GetSystemDirectoryW;
        else if (!strcmp(ProcessName, "GetWindowsDirectoryW"))
            return (FARPROC)GetWindowsDirectoryW;
        else if (!strcmp(ProcessName, "GetCurrentProcessId"))
            return (FARPROC)GetCurrentProcessId;
        else if (!strcmp(ProcessName, "GetCurrentThreadId"))
            return (FARPROC)GetCurrentThreadId;
    }
    return Result;
}

VOID WINAPI GetSystemInfo(LPSYSTEM_INFO SystemInfo)
{
    GetSystemInfo(SystemInfo);
    SystemInfo->dwPageSize = 1337;
}

BOOL WINAPI GetVersionExA(LPOSVERSIONINFOEXA VersionInformation)
{
    MessageBoxW(NULL, L"Bypass malfunction has been detected - steam is now closing.", L"Arizona's VAC Bypass", MB_OK | MB_ICONERROR);
    ExitProcess(1);
    return FALSE;
}

UINT WINAPI GetSystemDirectoryW(LPWSTR Buffer, UINT Size)
{
    MessageBoxW(NULL, L"Bypass malfunction has been detected - steam is now closing.", L"Arizona's VAC Bypass", MB_OK | MB_ICONERROR);
    ExitProcess(1);
    return 0;
}

UINT WINAPI GetWindowsDirectoryW(LPWSTR Buffer, UINT Size)
{
    MessageBoxW(NULL, L"Bypass malfunction has been detected - steam is now closing.", L"Arizona's VAC Bypass", MB_OK | MB_ICONERROR);
    ExitProcess(1);
    return 0;
}

DWORD WINAPI GetCurrentProcessId(VOID)
{
    return 0;
}

DWORD WINAPI GetCurrentThreadId(VOID)
{
    return 0;
}