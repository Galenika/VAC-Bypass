#include <shlwapi.h>
#include <stdbool.h>
#include <stdio.h>
#include <Windows.h>
#include <Psapi.h>
#include <time.h>

#include "Utilities.h"

PVOID FindPattern(PCWSTR Module, PCSTR Pattern, SIZE_T Offset)
{
    MODULEINFO ModuleInfo;
    HMODULE ModuleHandler = GetModuleHandleW(Module);

    if (ModuleHandler && GetModuleInformation(GetCurrentProcess(), ModuleHandler, &ModuleInfo, sizeof(ModuleInfo))) {
        for (PCHAR Character = ModuleInfo.lpBaseOfDll; Character != (PBYTE)ModuleInfo.lpBaseOfDll + ModuleInfo.SizeOfImage; Character++) {
            bool Matched = true;

            for (PCSTR NewPattern = Pattern, NewCharacter = Character; *NewPattern; NewPattern++, NewCharacter++) {
                if (*NewPattern != '?' && *NewCharacter != *NewPattern) {
                    Matched = false;
                    break;
                }
            }
            if (Matched)
                return Character + Offset;
        }
    }
    return NULL;
}

VOID ImportHook(PCWSTR ModuleName, PCSTR ImportedModuleName, PCSTR FunctionName, PVOID Offset)
{
    PBYTE Module = (PBYTE)GetModuleHandleW(ModuleName);

    if (Module) {
        PIMAGE_NT_HEADERS Headers = (PIMAGE_NT_HEADERS)(Module + ((PIMAGE_DOS_HEADER)Module)->e_lfanew);
        PIMAGE_IMPORT_DESCRIPTOR Imports = (PIMAGE_IMPORT_DESCRIPTOR)(Module + Headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

        for (PIMAGE_IMPORT_DESCRIPTOR Import = Imports; Import->Name; Import++) {
            if (_strcmpi(Module + Import->Name, ImportedModuleName))
                continue;

            for (PIMAGE_THUNK_DATA FirstChunkData = (PIMAGE_THUNK_DATA)(Module + Import->OriginalFirstThunk), FirstChunk = (PIMAGE_THUNK_DATA)(Module + Import->FirstThunk); FirstChunkData->u1.AddressOfData; FirstChunkData++, FirstChunk++) {
                if (strcmp((PCSTR)((PIMAGE_IMPORT_BY_NAME)(Module + FirstChunkData->u1.AddressOfData))->Name, FunctionName))
                    continue;

                PDWORD FunctionAddress = &FirstChunk->u1.Function;

                DWORD NewOffset;
                if (VirtualProtect(FunctionAddress, sizeof(Offset), PAGE_READWRITE, &NewOffset)) {
                    *FunctionAddress = (DWORD)Offset;
                    VirtualProtect(FunctionAddress, sizeof(Offset), NewOffset, &NewOffset);
                }
                break;
            }
            break;
        }
    }
}