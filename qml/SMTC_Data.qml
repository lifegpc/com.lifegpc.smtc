import QtQml 2.3
import NERvGear.Templates 1.0 as T

T.Data {
    id: root
    title: qsTr("SMTC Session Data")
    description: qsTr("Get music session data from System Media Transport Controls.")
    values: [
        StringValue {
            name: "SourceAppId"
            title: qsTr("Application name")
            description: qsTr("The apllication name of the session.")
            updateValueImpl: "GetSourceAppId"
        },
        StringValue {
            name: "Title"
            title: qsTr("Title")
            description: qsTr("The title of the music.")
            updateValueImpl: "GetTitle"
        },
        StringValue {
            name: "Artist"
            title: qsTr("Artist")
            description: qsTr("The artist of the music.")
            updateValueImpl: "GetArtist"
        }
    ]
}
