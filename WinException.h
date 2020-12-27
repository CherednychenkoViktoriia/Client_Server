#pragma once
#include <iostream>
#include <Windows.h>

class WinException : public std::runtime_error {
public:
    WinException(const char* message, DWORD errorMessageID) :
        std::runtime_error(message),
        m_errorMessageID(errorMessageID)
    {
    }
private:
    DWORD m_errorMessageID;
};