#include "SMTCBinding.h"
#include "SMTCSession.h"
#include "SMTCSessionManager.h"
#include "SMTCSMServer.h"
#include "QtQml"
#include "QQmlEngine"

void SMTCBinding::registerTypes(const char* uri) {
    qmlRegisterType<SMTCSession>(uri, 1, 0, "SMTCSession");
    qmlRegisterSingletonType<SMTCSessionManager>(uri, 1, 0, "Manager", [](QQmlEngine* engine, QJSEngine* scriptEngine) -> QObject* {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        SMTCSessionManager* server = new SMTCSessionManager();
        return server;
    });
    qmlRegisterSingletonType<SMTCSMServer>(uri, 1, 0, "Server", [](QQmlEngine* engine, QJSEngine* scriptEngine) -> QObject* {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        SMTCSMServer* server = new SMTCSMServer();
        return server;
    });
}

#include "moc_SMTCBinding.cpp"
