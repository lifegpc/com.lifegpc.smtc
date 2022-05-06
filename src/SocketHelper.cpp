#include "SocketHelper.h"

#include <ws2tcpip.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include "cstr_util.h"

SocketHelper::SocketHelper(int32_t port) {
    struct addrinfoW hints;
    wchar_t cport[32];
    m_port = port;
    int re = WSAStartup(MAKEWORD(2, 2), &m_wsadata);
    if (re) {
        return;
    }
    m_wsa_started = true;
    memset(&hints, 0, sizeof(addrinfoW));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    _snwprintf_s(cport, 32, L"%" PRIu32, port);
    cport[31] = 0;
    re = GetAddrInfoW(L"127.0.0.1", cport, &hints, &m_addr);
    if (re) {
        return;
    }
    m_inited = true;
}

SocketHelper::~SocketHelper() {
    Close();
    if (m_addr) {
        FreeAddrInfoW(m_addr);
        m_addr = nullptr;
    }
    if (m_wsa_started) {
        WSACleanup();
    }
}

bool SocketHelper::Connect() {
    Close();
    addrinfoW* ptr = nullptr;
    SOCKET sock = INVALID_SOCKET;
    for (ptr = m_addr; ptr != nullptr; ptr = ptr->ai_next) {
        sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sock == INVALID_SOCKET) {
            return false;
        }
        int re = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (re == SOCKET_ERROR) {
            closesocket(sock);
            sock = INVALID_SOCKET;
            continue;
        }
        break;
    }
    if (sock == INVALID_SOCKET) return false;
    DWORD timeout = 100;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) == SOCKET_ERROR) {
        closesocket(sock);
        sock = INVALID_SOCKET;
        return false;
    }
    m_connected = true;
    m_socket = sock;
    return true;
}

void SocketHelper::Close() {
    if (m_connected) {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
        m_connected = false;
    }
}

bool SocketHelper::Inited() {
    return m_inited;
}

bool SocketHelper::Connected() {
    return m_connected;
}

int SocketHelper::GetLastError() {
    return WSAGetLastError();
}

int SocketHelper::Send(const char* buf, int len) {
    if (!m_connected) {
        if (!Connect()) return -1;
    }
    int re = send(m_socket, buf, len, 0);
    if (re == SOCKET_ERROR) {
        HandleErrorCode();
        return -1;
    }
    return re;
}

int SocketHelper::Send(std::string buf) {
    return Send(buf.c_str(), (int)buf.size());
}

int SocketHelper::Recv(char* buf, int len, int flag) {
    if (!m_connected) {
        return -1;
    }
    int re = recv(m_socket, buf, len, flag);
    if (re == SOCKET_ERROR) {
        HandleErrorCode();
        return -1;
    }
    return re;
}

std::string SocketHelper::Recv(int len, int flag) {
    char* data = new char[len];
    std::string re;
    int r = Recv(data, len, flag);
    if (r == -1) {
        goto end;
    }
    re = std::string(data, r);
end:
    if (data) delete[] data;
    return re;
}

std::string SocketHelper::RecvAll(int len) {
    return Recv(len, MSG_WAITALL);
}

void SocketHelper::HandleErrorCode() {
    int err = GetLastError();
    if (err == WSAENOTCONN || err == WSAENOTSOCK || err == WSAEINVAL || err == WSAECONNABORTED || err == WSAECONNRESET || err == WSAETIMEDOUT || err == WSAENETRESET || err == WSAESHUTDOWN) {
        Close();
    }
}

std::string SocketHelper::RecvString() {
    char buf[4];
    int re = Recv(buf);
    if (re < 4) return "";
    int32_t length = cstr_read_int32((const uint8_t*)buf, 0);
    if (length <= 0) return "";
    return RecvAll((int)length);
}

std::list<std::string> SocketHelper::RecvStrings() {
    char buf[4];
    int re = Recv(buf);
    if (re < 4) return std::list<std::string>();
    int32_t length = cstr_read_int32((const uint8_t*)buf, 0);
    if (length <= 0) return std::list<std::string>();
    std::list<std::string> res;
    for (int i = 0; i < length; i++) {
        res.push_back(RecvString());
    }
    return res;
}

uint32_t SocketHelper::RecvUInt32() {
    char buf[4];
    int re = Recv(buf);
    if (re < 4) return 0;
    return cstr_read_uint32((const uint8_t*)buf, 0);
}
