#pragma once
#include <windows.h>
#include <string>
#include <iostream>

std::vector<int> vec(5);
int g_i = 0;

const std::string& GetLastErrorAsString()
{
    DWORD errorMessageID = GetLastError();
    if (errorMessageID == 0) {
        return std::string();
    }
    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&messageBuffer, 0, NULL);
    std::string message(messageBuffer, size);
    LocalFree(messageBuffer);
    return message;
}

class Server {
public:
    static DWORD WINAPI ThreadServer(PVOID pvRaram) {
        while (!m_exit) {
            DWORD resWaitForClientEvent = WaitForSingleObject(m_clientEvent, INFINITE);
            if (resWaitForClientEvent == WAIT_FAILED) {
                std::string str = GetLastErrorAsString();
                const char* c = str.c_str();
                throw std::exception(c, GetLastError());
            }
            std::cout << "Server start" << std::endl;
            for (size_t i = 0; i < 5; ++i, ++g_i) {
                vec[i] = g_i;
            }
            Sleep(1000);
            std::cout << "Server end" << std::endl;
            BOOL resSetResultsEvent = SetEvent(m_resultsEvent);
            if (resSetResultsEvent == 0) {
                std::string str = GetLastErrorAsString();
                const char* c = str.c_str();
                throw std::exception(c, GetLastError());
            }
        }
        return 0;
    }
    static void Connect() {
        BOOL resSetEventClient = SetEvent(m_clientEvent);
        if (resSetEventClient == 0) {
            std::string str = GetLastErrorAsString();
            const char* c = str.c_str();
            throw std::exception(c, GetLastError());
        }
    }
    static void WaitResults() {
        BOOL resWaitForResultsEvent = WaitForSingleObject(m_resultsEvent, INFINITE);
        if (resWaitForResultsEvent == WAIT_FAILED) {
            std::string str = GetLastErrorAsString();
            const char* c = str.c_str();
            throw std::exception(c, GetLastError());
        }
    }
    Server() {
        DWORD threadId = 0;
        m_threadServer = CreateThread(0, 0, ThreadServer, 0, 0, &threadId);
        if (m_threadServer == INVALID_HANDLE_VALUE) {
            throw WinException("Can't create thread for server");
        }
        m_clientEvent = CreateEventA(0, false, false, 0);
        if (m_clientEvent == INVALID_HANDLE_VALUE) {
            throw WinException("Can't create thread for client event");
        }
        m_resultsEvent = CreateEventA(0, false, false, 0);
        if (m_resultsEvent == INVALID_HANDLE_VALUE) {
            throw WinException("Can't create thread for results event");
        }
    }
    ~Server() {
        m_exit = true;
        BOOL resWaitForThreadServer = WaitForSingleObject(m_threadServer, INFINITE);
        if (resWaitForThreadServer == WAIT_FAILED) {
            std::cout << GetLastErrorAsString() << std::endl;
        }
        BOOL resCloseClientEventHandle = CloseHandle(m_clientEvent);
        if (resCloseClientEventHandle == 0) {            
            std::cout << GetLastErrorAsString() << std::endl;
        }
        BOOL resCloseResultsEventHandle = CloseHandle(m_resultsEvent);
        if (resCloseResultsEventHandle == 0) {
            std::cout << GetLastErrorAsString() << std::endl;
        }
        BOOL resCloseThreadServerHandle = CloseHandle(m_threadServer);
        if (resCloseThreadServerHandle == 0) {
            std::cout << GetLastErrorAsString() << std::endl;
        }
    }
private:
    HANDLE m_threadServer = INVALID_HANDLE_VALUE;
    static HANDLE m_clientEvent;
    static HANDLE m_resultsEvent;
    static bool m_exit;
};

HANDLE Server::m_clientEvent = INVALID_HANDLE_VALUE;
HANDLE Server::m_resultsEvent = INVALID_HANDLE_VALUE;
bool Server::m_exit = false;