#pragma once

#include "QQmlExtensionPlugin"

typedef class SMTCSessionManager SMTCSessionManager;

#define SOURCE_APP_ID 0
#define TITLE 3
#define ARTIST 4
#define CLOSE_SOCKET 0xFF

class SMTCSession : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool inited READ Inited)
    Q_PROPERTY(bool currentSession READ IsCurrentSession)
public:
    explicit SMTCSession(QObject* parent = nullptr);
    void SetManager(SMTCSessionManager* manager = nullptr);
    void SetCurrentSession(bool current_session);
    bool IsCurrentSession();
    bool Inited();
    QString GetQString(char id);
    Q_INVOKABLE QString GetSourceAppId();
    Q_INVOKABLE QString GetTitle();
    Q_INVOKABLE QString GetArtist();
    void GetSendData(char buf[6]);
private:
    SMTCSessionManager* m_manager = nullptr;
    bool m_inited = false;
    bool m_current_session = false;
};
