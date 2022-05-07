import NERvGear 1.0 as NVG
import "impl" 1.0

NVG.Module {
    initialize: function() {
        if (!Server.connected()) {
            if (!Server.start()) {
                console.warn("Failed to start SMTCSM server.")
                return false;
            }
            console.log('Started SMTCSM server')
        }
        return true;
    }
    cleanup: function() {
        Manager.close()
        Server.close()
    }
}
