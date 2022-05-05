#include "moc_SMTCSessionManager.cpp"
#include "SMTCSession.h"

SMTCSessionManager::SMTCSessionManager(QObject* parent) : QObject(parent) {
    m_currentSession = new SMTCSession(this);
    if (!m_socket.Inited()) {
        return;
    }
    m_inited = true;
    if (m_inited) {
        m_currentSession->SetManager(this);
        m_currentSession->SetCurrentSession(true);
    }
}

SMTCSessionManager::~SMTCSessionManager() {
    if (m_currentSession) delete m_currentSession;
    if (m_socket.Connected()) {
        char buf[1];
        buf[0] = (char)CLOSE_SOCKET;
        if (m_socket.Send(buf) >= 1) {
            m_socket.Recv(buf);
        }
    }
}

bool SMTCSessionManager::Inited() {
    return m_inited;
}

QString SMTCSessionManager::GerErrorMsg() {
    return m_errmsg;
}

SMTCSession* SMTCSessionManager::GetCurrentSession() {
    return m_currentSession;
}

bool SMTCSessionManager::Connect() {
    if (m_socket.Connected()) return true;
    if (m_socket.Connect()) return true;
    return false;
}

SocketHelper& SMTCSessionManager::GetSocket() {
    return m_socket;
}
