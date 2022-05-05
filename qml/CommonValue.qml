import QtQml 2.12
import NERvGear.Templates 1.0 as T
import "impl" 1.0

T.Value {
    property var updateValueImpl
    property var updateValueParam
    interval: 1000
    status: T.Value.Null
    update.execute: function() {
        status = T.Value.Null
        if (Manager.inited) {
            if (Manager.currentSession.inited) {
                updateValueImpl(updateValueParam)
            }
        }
    }
}
