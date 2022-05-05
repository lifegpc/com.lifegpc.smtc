#pragma once
#include <Windows.h>
#include <string>
#include "QQmlExtensionPlugin"

class SMTCSMServer: public QObject {
    Q_OBJECT
public:
    explicit SMTCSMServer(QObject* parent = nullptr);
    std::string FindServer();
    Q_INVOKABLE bool connected();
    Q_INVOKABLE bool isAlive();
    Q_INVOKABLE bool isStarted();
    Q_INVOKABLE void close();
    Q_INVOKABLE bool start();
private:
    STARTUPINFOW m_si;
    PROCESS_INFORMATION m_pi;
    bool m_started = false;
};
