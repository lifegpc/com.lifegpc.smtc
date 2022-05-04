#pragma once
#include <Windows.h>
#include <string>
#include "QQmlExtensionPlugin"

class SMTCSMServer: public QObject {
    Q_OBJECT
public:
    explicit SMTCSMServer(QObject* parent = nullptr);
    std::string FindServer();
    Q_INVOKABLE bool Connected();
    Q_INVOKABLE bool IsAlive();
    Q_INVOKABLE bool IsStarted();
    Q_INVOKABLE void Close();
    Q_INVOKABLE bool Start();
private:
    STARTUPINFOW m_si;
    PROCESS_INFORMATION m_pi;
    bool m_started = false;
};
