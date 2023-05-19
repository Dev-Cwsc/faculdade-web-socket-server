#ifndef SERVER_H
#define SERVER_H

#include <thread>
#include "File.h"
#include "Socket.h"
#include "HTTP.h"

using namespace std;

class Server : public File, public Socket, public HTTP{
    public:
        Server();
        Server(string, int);
        virtual ~Server();
        static void RequestTreatment(string, int, Socket_Interface *);
        static void DefaultPath(char*&);
    private:
        Socket_Interface * socket = NULL;
};

#endif // SERVER_H
