#pragma once
#include <Windows.h>

class WinException : public std::runtime_error {
public:
    WinException(const char* message) :
        std::runtime_error(message)        
    {
        m_errorMessageID = GetLastError();
    }
private:
    DWORD m_errorMessageID = 0;
};