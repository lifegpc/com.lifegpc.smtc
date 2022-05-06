import QtQml 2.12
import NERvGear.Templates 1.0 as T
import "impl" 1.0

CommonValue {
    property var invalid_value: 0
    current: 0
    minimum: -Infinity
    maximum: Infinity
    updateValueImpl: function(updateValueParam) {
        current = Manager.currentSession["get" + name](updateValueParam);
        if (current !== invalid_value) {
            status = T.Value.Ready
        }
    }
}
