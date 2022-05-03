#include "SMTCSession.h"
#include "SMTCSessionManager.h"
#include <string.h>

SMTCSession::SMTCSession(QObject* parent) : QObject(parent) {};

void SMTCSession::SetManager(SMTCSessionManager* manager) {
    m_manager = manager;
    if (m_manager) {
        m_inited = true;
    }
}

bool SMTCSession::Inited() {
    return m_inited;
}

void SMTCSession::SetCurrentSession(bool current_session) {
    m_current_session = current_session;
}

bool SMTCSession::IsCurrentSession() {
    return m_current_session;
}

QString SMTCSession::GetQString(char id) {
    if (!m_inited) return "";
    char buf[6];
    GetSendData(buf);
    buf[0] = id;
    if (!m_manager->Connect()) return "";
    if (m_manager->GetSocket().Send(buf) < 6) return "";
    std::string data = m_manager->GetSocket().RecvString();
    return std::move(QString::fromUtf8(data.c_str(), (int)data.size()));
}

QString SMTCSession::GetSourceAppId() {
    return GetQString((char)SOURCE_APP_ID);
}

void SMTCSession::GetSendData(char buf[6]) {
    memset(buf + 1, 0, 5);
}

QString SMTCSession::GetTitle() {
    return GetQString((char)TITLE);
}

QString SMTCSession::GetArtist() {
    return GetQString((char)ARTIST);
}
