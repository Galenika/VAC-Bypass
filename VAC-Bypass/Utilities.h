#pragma once

#include <Windows.h>

PVOID FindPattern(PCWSTR, PCSTR, SIZE_T);
VOID ImportHook(PCWSTR, PCSTR, PCSTR, PVOID);