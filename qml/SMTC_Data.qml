import QtQml 2.3
import NERvGear.Templates 1.0 as T
import "impl" 1.0 as SMTC

T.Data {
    id: root
    title: qsTr("SMTC Session Data")
    description: qsTr("Get music session data from System Media Transport Controls.")
    property SMTC.SMTCSessionManager manager: SMTC.SMTCSessionManager { }
    values: [
        T.Value {
            name: "SourceAppId"
            title: qsTr("Application name")
            description: qsTr("The apllication name of the session.")
            interval: 1000
            status: T.Value.Null
            update.execute: function() {
                status = T.Value.Null
                if (manager.inited) {
                    if (manager.currentSession.inited) {
                        current = manager.currentSession.GetSourceAppId();
                        if (current.length) {
                            status = T.Value.Ready
                        }
                    }
                }
            }
        },
        T.Value {
            name: "Title"
            title: qsTr("Title")
            description: qsTr("The title of the music.")
            interval: 1000
            status: T.Value.Null
            update.execute: function() {
                status = T.Value.Null
                if (manager.inited) {
                    if (manager.currentSession.inited) {
                        current = manager.currentSession.GetTitle();
                        if (current.length) {
                            status = T.Value.Ready
                        }
                    }
                }
            }
        },
        T.Value {
            name: "Artist"
            title: qsTr("Artist")
            description: qsTr("The artist of the music.")
            interval: 1000
            status: T.Value.Null
            update.execute: function() {
                status = T.Value.Null
                if (manager.inited) {
                    if (manager.currentSession.inited) {
                        current = manager.currentSession.GetArtist();
                        if (current.length) {
                            status = T.Value.Ready
                        }
                    }
                }
            }
        }
    ]
}
