#pragma once

#include <WinSock2.h>
#include <stdint.h>
#include <list>
#include <string>

class SocketHelper {
public:
    SocketHelper(int32_t port = 23412);
    ~SocketHelper();
    bool Inited();
    bool Connect();
    void Close();
    bool Connected();
    int GetLastError();
    int Send(const char* buf, int len);
    template<int T>
    int Send(char(&buf)[T]) {
        return Send(buf, T);
    }
    int Send(std::string buf);
    int Recv(char* buf, int len, int flag = 0);
    template<int T>
    int Recv(char(&buf)[T], int flag = 0) {
        return Recv(buf, T, flag);
    }
    std::string Recv(int len, int flag = 0);
    std::string RecvAll(int len);
    std::string RecvString();
    std::list<std::string> RecvStrings();
private:
    void HandleErrorCode();
    int32_t m_port;
    WSADATA m_wsadata;
    struct addrinfoW* m_addr = nullptr;
    SOCKET m_socket = INVALID_SOCKET;
    bool m_inited = false;
    bool m_wsa_started = false;
    bool m_connected = false;
};
