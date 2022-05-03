#pragma once

#include "QQmlExtensionPlugin"
#include "SocketHelper.h"

typedef class SMTCSession SMTCSession;

class SMTCSessionManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool inited READ Inited)
    Q_PROPERTY(QString errmsg READ GerErrorMsg)
    Q_PROPERTY(SMTCSession* currentSession READ GetCurrentSession)
public:
    explicit SMTCSessionManager(QObject* parent = nullptr);
    ~SMTCSessionManager();
    bool Inited();
    QString GerErrorMsg();
    SMTCSession* GetCurrentSession();
    bool Connect();
    SocketHelper& GetSocket();
private:
    SMTCSession* m_currentSession = nullptr;
    bool m_inited = false;
    QString m_errmsg;
    SocketHelper m_socket;
};
