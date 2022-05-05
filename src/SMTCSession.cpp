#include "moc_SMTCSession.cpp"
#include "SMTCSessionManager.h"
#include <string.h>
#include "str_util.h"

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

QString SMTCSession::formatString(QString format, ActionWhenDataNotFound actionWhenDataNotFound, ActionWhenAllDataNotFound actionWhenAllDataNotFound) {
    bool have_valid_data = false;
    auto fstrl = str_util::str_split(format.toStdString(), "%");
    bool is_origin_data = false;
    if (fstrl.size() <= 1) return format;
    for (auto i = fstrl.begin(); i != fstrl.end(); i++) {
        is_origin_data = !is_origin_data;
        auto &data = *i;
        if (!is_origin_data) {
            auto odata = data;
            if (data == "title") {
                data = getTitle().toStdString();
            } else if (data == "artist") {
                data = getArtist().toStdString();
            } else if (data == "source_app_id") {
                data = getSourceAppId().toStdString();
            } else if (data == "source_device_id") {
                data = getSourceDeviceId().toStdString();
            } else if (data == "render_device_id") {
                data = getRenderDeviceId().toStdString();
            } else if (data == "subtitle") {
                data = getSubtitle().toStdString();
            } else if (data == "album" || data == "album_title") {
                data = getAlbumTitle().toStdString();
            } else if (data == "album_artist") {
                data = getAlbumArtist().toStdString();
            } else if (data == "media_class_primary_id") {
                data = getMediaClassPrimaryId().toStdString();
            } else {
                data = "";
            }
            if (data.empty()) {
                switch (actionWhenDataNotFound) {
                case ActionWhenDataNotFound::DO_NOTHING:
                    data = std::string("%") + odata + "%";
                    break;
                case ActionWhenDataNotFound::REPLACE_WITH_EMPTY:
                    break;
                case ActionWhenDataNotFound::REPLACE_WITH_UNDEFINED:
                    data = "undefined";
                    break;
                }
            } else {
                have_valid_data = true;
            }
        }
    }
    if (!have_valid_data && actionWhenAllDataNotFound == ActionWhenAllDataNotFound::TREAT_AS_NULL) {
        return "";
    }
    std::string re;
    for (auto i = fstrl.begin(); i != fstrl.end(); i++) {
        re += *i;
    }
    return QString::fromUtf8(re.c_str(), (int)re.size());
}
