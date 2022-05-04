import QtQml 2.3
import NERvGear 1.0 as NVG
import "impl" 1.0 as SMTC

NVG.Module {
    property SMTC.SMTCSMServer server: SMTC.SMTCSMServer { }
    initialize: function() {
        if (!server.Connected()) {
            if (!server.Start()) {
                console.warn("Failed to start SMTCSM server.")
                return false;
            }
            console.log('Started SMTCSM server')
        }
        return true;
    }
    ready: function() {
        if (!server.Connected()) {
            console.warn("Failed to connect SMTCSM server.")
        }
    }
    cleanup: function() {
        server.Close()
    }
}
