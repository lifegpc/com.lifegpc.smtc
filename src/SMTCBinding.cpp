#include "SMTCBinding.h"
#include "SMTCSession.h"
#include "SMTCSessionManager.h"
#include "SMTCSMServer.h"
#include "QtQml"

void SMTCBinding::registerTypes(const char* uri) {
    qmlRegisterType<SMTCSession>(uri, 1, 0, "SMTCSession");
    qmlRegisterType<SMTCSessionManager>(uri, 1, 0, "SMTCSessionManager");
    qmlRegisterType<SMTCSMServer>(uri, 1, 0, "SMTCSMServer");
}

#include "moc_SMTCBinding.cpp"
