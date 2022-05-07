#include "moc_SMTCSession.cpp"
#include "SMTCSessionManager.h"
#include <inttypes.h>
#include <string.h>
#include "fileop.h"
#include "str_util.h"

#define INT_CONVERT_TO_STRING(target, src, fmt) { \
    char tmp[64]; \
    snprintf(tmp, sizeof(tmp), "%" fmt, src); \
    target = tmp; \
}

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

QList<QString> SMTCSession::GetQStringList(char id) {
    if (!m_inited) return QList<QString>();
    char buf[6];
    GetSendData(buf);
    buf[0] = id;
    if (!m_manager->Connect()) return QList<QString>();
    if (m_manager->GetSocket().Send(buf) < 6) return QList<QString>();
    QList<QString> li;
    auto data = m_manager->GetSocket().RecvStrings();
    for (auto i = data.begin(); i != data.end(); i++) {
        auto &s = *i;
        li.push_back(QString::fromUtf8(s.c_str(), (int)s.size()));
    }
    return li;
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

QList<QString> SMTCSession::getGenres() {
    return GetQStringList((char)GENRES);
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
            } else if (data == "genres" || data.find("genres|") == 0) {
                auto arg = str_util::str_split(data, "|", 2);
                std::string sep = arg.size() == 1 ? ", " : arg.back();
                data = JoinQStringList(getGenres(), QString::fromUtf8(sep.c_str(), (int)sep.size())).toStdString();
            } else if (data == "album_track_count") {
                quint32 num = getAlbumTrackCount();
                if (num != 0) INT_CONVERT_TO_STRING(data, num, PRIu32) else data = "";
            } else if (data == "track_number") {
                quint32 num = getTrackNumber();
                if (num != 0) INT_CONVERT_TO_STRING(data, num, PRIu32) else data = "";
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

QString SMTCSession::JoinQStringList(QList<QString>& list, QString sep) {
    QString tmp;
    bool first = true;
    for (auto i = list.begin(); i != list.end(); i++) {
        if (first) {
            tmp = *i;
            first = false;
        } else {
            tmp += sep;
            tmp += *i;
        }
    }
    return tmp;
}

quint32 SMTCSession::GetUint32(char id) {
    if (!m_inited) return 0;
    char buf[6];
    GetSendData(buf);
    buf[0] = id;
    if (!m_manager->Connect()) return 0;
    if (m_manager->GetSocket().Send(buf) < 6) return 0;
    return m_manager->GetSocket().RecvUInt32();
}

quint32 SMTCSession::getAlbumTrackCount() {
    return GetUint32((char)ALBUM_TRACK_COUNT);
}

quint32 SMTCSession::getTrackNumber() {
    return GetUint32((char)TRACK_NUMBER);
}

std::string SMTCSession::getThumbnailHash() {
    if (!m_inited) return "";
    char buf[6];
    buf[0] = (char)THUMBNAIL_HASH;
    GetSendData(buf);
    if (!m_manager->Connect()) return "";
    if (m_manager->GetSocket().Send(buf) < 6) return "";
    char re[64];
    if (m_manager->GetSocket().Recv(re) < 64) return "";
    return std::string(re, 64);
}

void SMTCSession::close(bool force) {
    cleanWaited();
    if (!m_currentThumbnailFile.empty()) {
        if (fileop::exists(m_currentThumbnailFile)) {
            if (!fileop::remove(m_currentThumbnailFile)) {
                m_waitToClean.push_back(m_currentThumbnailFile);
            }
        }
        m_currentThumbnailFile = "";
    }
    m_currentThumbnailHash = "";
    if (force) {
        fileop::fcloseall();
        cleanWaited();
    }
}

void SMTCSession::cleanWaited() {
    for (auto i = m_waitToClean.begin(); i != m_waitToClean.end(); i++) {
        auto& d = *i;
        if (!fileop::exists(d) || fileop::remove(d)) {
            m_waitToClean.remove(d);
            i = m_waitToClean.begin();
        }
    }
}

SMTCSession::~SMTCSession() {
    close();
}

QString SMTCSession::getThumbnail() {
    auto now_hash = getThumbnailHash();
    if (now_hash.empty() || m_manager->IsInBlackList(now_hash)) {
        close(false);
        return "";
    }
    if (now_hash == m_currentThumbnailHash) {
        if (fileop::exists(m_currentThumbnailFile)) {
            cleanWaited();
            return getFileUrl(m_currentThumbnailFile);
        }
    }
    auto file = getThumbnailFile();
    if (file.empty() || !fileop::exists(file)) {
        close(false);
        return "";
    }
    close(false);
    m_currentThumbnailHash = now_hash;
    m_currentThumbnailFile = file;
    return getFileUrl(m_currentThumbnailFile);
}

std::string SMTCSession::getThumbnailFile() {
    return GetQString((char)THUMBNAIL_FILE).toStdString();
}

QString SMTCSession::getFileUrl(std::string file) {
    auto tmp = str_util::str_replace(file, "\\", "/");
    tmp = "file:///" + tmp;
    return QString::fromUtf8(tmp.c_str(), (int)tmp.size());
}
