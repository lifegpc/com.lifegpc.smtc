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

QString SMTCSession::getSourceAppId() {
    return GetQString((char)SOURCE_APP_ID);
}

void SMTCSession::GetSendData(char buf[6]) {
    memset(buf + 1, 0, 5);
}

QString SMTCSession::getTitle() {
    return GetQString((char)TITLE);
}

QString SMTCSession::getArtist() {
    return GetQString((char)ARTIST);
}

QString SMTCSession::getSourceDeviceId() {
    return GetQString((char)SOURCE_DEVICE_ID);
}

QString SMTCSession::getRenderDeviceId() {
    return GetQString((char)RENDER_DEVICE_ID);
}

QString SMTCSession::getSubtitle() {
    return GetQString((char)SUBTITLE);
}

QString SMTCSession::getAlbumArtist() {
    return GetQString((char)ALBUM_ARTIST);
}

QString SMTCSession::getAlbumTitle() {
    return GetQString((char)ALBUM_TITLE);
}

QString SMTCSession::getMediaClassPrimaryId() {
    return GetQString((char)MEDIA_CLASS_PRIMARY_ID);
}

#include "moc_SMTCSession.cpp"
