//
// Created by xobtah on 23/01/17.
//

#include <iostream>

#include "Server.hpp"
#include "DataBase.hpp"
#include "Struct.hpp"

int main() try
{
    Zia::DataBase<Zia::ClientStruct>  db;
    Zia::Server _server(db);
    char        packet[PACKET_MAX_SIZE];
    Zia::ClientStruct   *client;

    while (42)
    {
        if (_server.Select(reinterpret_cast<uint8_t*>(packet), &client))
        {
            std::cout << packet << std::endl;
            _server.SendPacket(*client, reinterpret_cast<uint8_t*>(packet), PACKET_MAX_SIZE);
        }
    }
    return (0);
}
catch (Zia::ServerException const &se) { std::cerr << "ServerException: " + std::string(se.what()) << std::endl; }