#pragma once
#include <Windows.h>
#include <string>

class SMTCSMServer {
public:
    SMTCSMServer();
    std::string FindServer();
    bool IsAlive();
    bool IsStarted();
    void Close();
    bool Start();
private:
    STARTUPINFOW m_si;
    PROCESS_INFORMATION m_pi;
    bool m_started = false;
};

SMTCSMServer& GetServer();
