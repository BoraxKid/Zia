//
// Created by xobtah on 23/01/17.
//

#ifndef ZIA_SERVER_HPP
#define ZIA_SERVER_HPP

#include "DataBase.hpp"
#include "Struct.hpp"
#include "TCPAcceptor.hpp"
#include "TCPStream.hpp"

#define PACKET_MAX_SIZE 512

namespace Zia
{
    // ServerException
    class ServerException : public std::exception
    {
    public:
        ServerException(std::string what) throw() : _what(what) {}
        ~ServerException() {}

        virtual const char  *what() const throw() { return _what.c_str(); }

    private:
        std::string _what;
    };

    // Server
    class Server
    {
    public:
        Server(DataBase<ClientStruct>&, int port = 4242, int queue = 42);
        ~Server();

        bool    Select(uint8_t*, ClientStruct **client = NULL, unsigned int usec_timeout = 500000);

        int     SendPacket(ClientStruct const &, uint8_t*, unsigned int);

    private:
        DataBase<ClientStruct>  _clients;
        TCPAcceptor             _acceptor;

        int     CreateFdSet(fd_set*);
        int     OSSelect(int, fd_set*, fd_set *writefd = NULL, struct timeval *to = NULL, fd_set *exceptfd = NULL);
        void    NewClient();
        void    RemoveClient(unsigned int);
    };
}

#endif //ZIA_SERVER_HPP
