#include "SMTCSMServer.h"
#include "DllMain.h"
#include "fileop.h"
#include "wchar_util.h"

static class SMTCSMServer server;

SMTCSMServer::SMTCSMServer() {
    memset(&m_si, 0, sizeof(m_si));
    memset(&m_pi, 0, sizeof(m_pi));
    m_si.cb = sizeof(m_si);
}

std::string SMTCSMServer::FindServer() {
    std::string dll_path = GetDllPath();
    if (!dll_path.empty()) {
        std::string target = fileop::join(fileop::dirname(dll_path), "SMTCSM.exe");
        if (fileop::exists(target)) {
            return target;
        }
    }
    if (fileop::exists("SMTCSM.exe")) {
        return "SMTCSM.exe";
    }
    return "";
}

bool SMTCSMServer::IsAlive() {
    if (!m_started) return false;
    DWORD code = 0;
    if (!GetExitCodeProcess(m_pi.hProcess, &code)) {
        return false;
    }
    return code == STILL_ACTIVE;
}

bool SMTCSMServer::IsStarted() {
    return m_started;
}

void SMTCSMServer::Close() {
    if (m_started) {
        if (IsAlive()) {
            TerminateProcess(m_pi.hProcess, 0);
        }
        CloseHandle(m_pi.hProcess);
        CloseHandle(m_pi.hThread);
        memset(&m_si, 0, sizeof(m_si));
        memset(&m_pi, 0, sizeof(m_pi));
        m_si.cb = sizeof(m_si);
    }
}

bool SMTCSMServer::Start() {
    Close();
    std::string path = FindServer();
    if (path.empty()) return false;
    std::wstring wpath;
    if (!wchar_util::str_to_wstr(wpath, path, CP_UTF8)) return false;
    std::string dir = fileop::dirname(path);
    std::wstring wdir;
    if (!dir.empty() && !wchar_util::str_to_wstr(wdir, dir, CP_UTF8)) return false;
    if (!CreateProcessW(wpath.c_str(), nullptr, nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, wdir.empty() ? nullptr : wdir.c_str(), &m_si, &m_pi)) {
        memset(&m_si, 0, sizeof(m_si));
        memset(&m_pi, 0, sizeof(m_pi));
        m_si.cb = sizeof(m_si);
        return false;
    }
    return true;
}

SMTCSMServer& GetServer() {
    return server;
}
