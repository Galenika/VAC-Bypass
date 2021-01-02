#pragma once

#include <Windows.h>

HMODULE           WINAPI     LoadLibraryExW(LPCWSTR, HANDLE, DWORD);
HMODULE           WINAPI     SteamClient_LoadLibraryExW(LPCWSTR, HANDLE, DWORD);
FARPROC           WINAPI     GetProcAddress(HMODULE, LPCSTR);
VOID              WINAPI     GetSystemInfo(LPSYSTEM_INFO);
BOOL              WINAPI     GetVersionExA(LPOSVERSIONINFOEXA);
UINT              WINAPI     GetSystemDirectoryW(LPWSTR, UINT);
UINT              WINAPI     GetWindowsDirectoryW(LPWSTR, UINT);
DWORD             WINAPI     GetCurrentProcessId(VOID);
DWORD             WINAPI     GetCurrentThreadId(VOID);