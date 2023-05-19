#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "Socket_Interface.h"

using namespace std;

class Socket : public Socket_Interface{//classe responsável por inicializar o socket e fazer as configurações iniciais
    public:
        Socket();
        Socket(int);
        virtual ~Socket();
        virtual void CreateSocket(int);
        virtual void CloseSocket();
        virtual int CreateConnection();
        virtual void CloseConnection(int);
        virtual string ReadRequest(int);
        virtual void SendResponse(int, string, int);
    private:
        int Num_Socket;
        struct sockaddr_in addr;
};

#endif // SOCKET_H
