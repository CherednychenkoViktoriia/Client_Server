#include <iostream>
#include <vector>
#include <Windows.h>
#include "WinException.h"
#include "Server.h"
#include "Client.h"

int main()
{
    try {
        Server server;
        Client client1(server);
        Sleep(3000);
        Client client2(server);
    }
    catch (const std::exception& ex) {
        std::cout << ex.what();
    }
    Sleep(1000);  
}