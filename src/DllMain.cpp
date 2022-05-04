#include "DllMain.h"
#include <Windows.h>
#include "wchar_util.h"
#include "SMTCSMServer.h"

static std::string dll_path;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    // Perform actions based on the reason for calling.
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        {
            wchar_t path[MAX_PATH];
            DWORD count = GetModuleFileNameW(hinstDLL, path, MAX_PATH);
            if (count) {
                std::wstring tmp(path, count);
                wchar_util::wstr_to_str(dll_path, tmp, CP_UTF8);
            }
        }
        break;
    default:
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

std::string GetDllPath() {
    return dll_path;
}
