import QtQml 2.3
import NERvGear.Preferences 1.0
import NERvGear.Templates 1.0 as T
import "impl" 1.0

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
        },
        StringListValue {
            name: "Genres"
            title: qsTr("Genres")
            description: qsTr("The genres of the media.")
        },
        IntValue {
            name: "AlbumTrackCount"
            minimum: 1
            title: qsTr("Album track count")
            description: qsTr("The track count of the album.")
        },
        IntValue {
            name: "TrackNumber"
            minimum: 1
            title: qsTr("Track number")
            description: qsTr("The track number of the current song in the album.")
        },
        StringValue {
            name: "Thumbnail"
            title: qsTr("Thumbnail")
            description: qsTr("The thumbnail of the media.")
        },
        StringValue {
            name: "custom"
            title: qsTr("Custom text")
            updateValueImpl: function(updateValueParam) {
                var fmt = "%title% - %artist%"
                var conf = update.configuration || {}
                if (conf.format) {
                    fmt = conf.format
                }
                current = Manager.currentSession.formatString(fmt, conf.aWDF, conf.aWADF)
                if (current.length) {
                    status = T.Value.Ready
                }
            }
            update.preference: PreferenceGroup {
                SelectPreference {
                    name: "aWDF"
                    label: qsTr("Action when data not found")
                    model: [
                        { data: SMTCSession.ActionWhenDataNotFound.DO_NOTHING, label: qsTr("Do nothing") },
                        { data: SMTCSession.ActionWhenDataNotFound.REPLACE_WITH_EMPTY, label: qsTr("Replace with empty string") },
                        { data: SMTCSession.ActionWhenDataNotFound.REPLACE_WITH_UNDEFINED, label: qsTr("Replace with \"undefined\"") }
                    ]
                    message: qsTr("Specify the action if one key is not recognized or the value of that key is not available.")
                    textRole: "label"
                    load: function (newData) {
                        let newValue = 0;
                        if (newData !== undefined) {
                            model.find(function(d, index) {
                                if (d.data === newData) {
                                    newValue = index;
                                    return true;
                                }
                            })
                        }
                        value = newValue;
                    }
                    save: function() {
                        return model[value].data;
                    }
                }
                SelectPreference {
                    name: "aWADF"
                    label: qsTr("Action when all data not found")
                    defaultValue: 1
                    model: [
                        { data: SMTCSession.ActionWhenAllDataNotFound.DO_NOTHING, label: qsTr("Do nothing") },
                        { data: SMTCSession.ActionWhenAllDataNotFound.TREAT_AS_NULL, label: qsTr("Return as empty string") }
                    ]
                    message: qsTr("Specify the action if all keys are not recognized or the all values are not available.")
                    textRole: "label"
                    load: function (newData) {
                        let newValue = defaultValue;
                        if (newData !== undefined) {
                            model.find(function(d, index) {
                                if (d.data === newData) {
                                    newValue = index;
                                    return true;
                                }
                            })
                        }
                        value = newValue;
                    }
                    save: function() {
                        return model[value].data;
                    }
                }
                TextFieldPreference {
                    name: "format"
                    label: qsTr("Format string")
                    hint: "%title% - %artist%"
                    message: qsTr("Available fields") + ": %title%, %artist%, %source_app_id%, %source_device_id%, %render_device_id%, %subtitle%, %album% (%album_title%), %album_artist%, %media_class_primary_id%, %genres[|<separator>]%, %album_track_count%, %track_number%"
                }
            }
        }
    ]
}
