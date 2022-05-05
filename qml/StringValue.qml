import QtQml 2.12
import NERvGear.Templates 1.0 as T
import "impl" 1.0

CommonValue {
    current: ""
    minimum: ""
    maximum: ""
    updateValueImpl: function(updateValueParam) {
        current = Manager.currentSession["get" + name](updateValueParam);
        if (current.length) {
            status = T.Value.Ready
        }
    }
}
