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
        },
        StringValue {
            name: "SourceDeviceId"
            title: qsTr("Source device id")
            description: qsTr("The source device's id of the session.")
        },
        StringValue {
            name: "RenderDeviceId"
            title: qsTr("Render device id")
            description: qsTr("The render device's id of the session.")
        },
        StringValue {
            name: "Title"
            title: qsTr("Title")
            description: qsTr("The title of the media.")
        },
        StringValue {
            name: "Artist"
            title: qsTr("Artist")
            description: qsTr("The artist of the media.")
        },
        StringValue {
            name: "Subtitle"
            title: qsTr("Subtitle")
            description: qsTr("The subtitle of the media.")
        },
        StringValue {
            name: "AlbumArtist"
            title: qsTr("Album artist")
            description: qsTr("The album artist of the media.")
        },
        StringValue {
            name: "AlbumTitle"
            title: qsTr("Album title")
            description: qsTr("The album title of the media.")
        },
        StringValue {
            name: "MediaClassPrimaryId"
            title: qsTr("Media class primary id")
        }
    ]
}
