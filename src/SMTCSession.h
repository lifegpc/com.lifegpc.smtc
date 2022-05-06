#pragma once

#include "QQmlExtensionPlugin"

typedef class SMTCSessionManager SMTCSessionManager;

#define SOURCE_APP_ID 0
#define SOURCE_DEVICE_ID 1
#define RENDER_DEVICE_ID 2
#define TITLE 3
#define ARTIST 4
#define SUBTITLE 5
#define ALBUM_ARTIST 6
#define ALBUM_TITLE 7
#define MEDIA_CLASS_PRIMARY_ID 8
#define GENRES 9
#define ALBUM_TRACK_COUNT 10
#define TRACK_NUMBER 11
#define CLOSE_SOCKET 0xFF

class SMTCSession : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool inited READ Inited)
    Q_PROPERTY(bool currentSession READ IsCurrentSession)
public:
    enum class ActionWhenDataNotFound { DO_NOTHING, REPLACE_WITH_EMPTY, REPLACE_WITH_UNDEFINED };
    enum class ActionWhenAllDataNotFound { DO_NOTHING, TREAT_AS_NULL };
    Q_ENUM(ActionWhenDataNotFound)
    Q_ENUM(ActionWhenAllDataNotFound)
    explicit SMTCSession(QObject* parent = nullptr);
    void SetManager(SMTCSessionManager* manager = nullptr);
    void SetCurrentSession(bool current_session);
    bool IsCurrentSession();
    bool Inited();
    QString GetQString(char id);
    QList<QString> GetQStringList(char id);
    quint32 GetUint32(char id);
    QString JoinQStringList(QList<QString>& list, QString sep);
    Q_INVOKABLE QString getSourceAppId();
    Q_INVOKABLE QString getSourceDeviceId();
    Q_INVOKABLE QString getRenderDeviceId();
    Q_INVOKABLE QString getTitle();
    Q_INVOKABLE QString getArtist();
    Q_INVOKABLE QString getSubtitle();
    Q_INVOKABLE QString getAlbumArtist();
    Q_INVOKABLE QString getAlbumTitle();
    Q_INVOKABLE QString getMediaClassPrimaryId();
    Q_INVOKABLE QList<QString> getGenres();
    Q_INVOKABLE quint32 getAlbumTrackCount();
    Q_INVOKABLE quint32 getTrackNumber();
    Q_INVOKABLE QString formatString(QString format, ActionWhenDataNotFound actionWhenDataNotFound = ActionWhenDataNotFound::DO_NOTHING, ActionWhenAllDataNotFound actionWhenAllDataNotFound = ActionWhenAllDataNotFound::TREAT_AS_NULL);
    void GetSendData(char buf[6]);
private:
    SMTCSessionManager* m_manager = nullptr;
    bool m_inited = false;
    bool m_current_session = false;
};
