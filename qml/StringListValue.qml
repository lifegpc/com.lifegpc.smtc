import QtQml 2.12
import NERvGear.Preferences 1.0
import NERvGear.Templates 1.0 as T
import "impl" 1.0

CommonValue {
    current: ""
    minimum: ""
    maximum: ""
    updateValueImpl: function(updateValueParam) {
        let data = Manager.currentSession["get" + name](updateValueParam);
        if (data.length) {
            current = data.join(update.configuration ? update.configuration : ", ");
            status = T.Value.Ready
        }
    }
    update.preference: TextFieldPreference {
        label: qsTr("Separator")
        hint: ", "
        message: qsTr("Specify the string to separate each pair of elements in array.")
    }
}
