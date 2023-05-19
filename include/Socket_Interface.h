#ifndef SOCKET_INTERFACE_H
#define SOCKET_INTERFACE_H

#include <string>

using namespace std;

class Socket_Interface {//as classes Socket_interface e File_Interface são virtuais, servem apenas para definir a assinatura dos métodos que serão sobrescritos pelas classes filhas
    public:
        virtual ~Socket_Interface(){};
        virtual void CreateSocket(int port)=0;
        virtual void CloseSocket() =0;
        virtual int CreateConnection()=0;
        virtual void CloseConnection(int idConnection)=0;
        virtual string ReadRequest(int idConnection)=0;
        virtual void SendResponse(int idConnection, string buffer, int bufferSize)=0;
};

#endif // SOCKET_INTERFACE_H
