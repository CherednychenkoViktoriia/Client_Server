#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <Server.h>

class Client {
public:
    Client(Server& server) :
        m_server(server)
    {
        DWORD threadId = 0;
        m_threadClient = CreateThread(0, 0, ThreadClient, 0, 0, &threadId);
        if (m_threadClient == INVALID_HANDLE_VALUE) {
            std::string str = GetLastErrorAsString();
            const char* c = str.c_str();
            throw std::exception(c, GetLastError());
        }
    }
    ~Client() {
        BOOL resCloseThreadClientHandle = CloseHandle(m_threadClient);
        if (resCloseThreadClientHandle == 0) {
            std::string str = GetLastErrorAsString();
            std::cout << str << std::endl;
        }
    }
    static DWORD WINAPI ThreadClient(PVOID pvParam) {
        Server::Connect();
        std::cout << "Client start" << std::endl;
        Server::WaitResults();
        for (size_t i = 0; i < 5; ++i) {
            std::cout << vec[i] << std::endl;
        }
        std::cout << "Client end" << std::endl;
        return 0;
    }
private:
    HANDLE m_threadClient = INVALID_HANDLE_VALUE;
    Server& m_server;
};