#include "Socket.h"

Socket::Socket()
{
    //ctor
}

Socket::Socket(int port){
    CreateSocket(port);
}

Socket::~Socket()
{
    //dtor
}

void Socket::CreateSocket(int port){ //abre e configura um socket e armazena o número no atributo Num_Socket.
    int option=1;
    if((Num_Socket = socket(AF_INET, SOCK_STREAM, 6)) < 0 ){ //abre o socket
        cout<<"Error trying to create socket."<<endl;
        throw "Error trying to create socket.";
    }

    if( setsockopt(Num_Socket,SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&option, sizeof(option)) ){ //configura o socket
        cout<<"Error trying to configure socket."<<endl;
        throw "Error trying to configure socket.";
    }

    addr.sin_port = htons(port); //struct usada para guardar o endereço da porta e setar parâmetros do socket
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;

    if(bind(Num_Socket, (struct sockaddr *) &addr, sizeof(addr))  < 0){ //assimila o endereço contido na struct addr ao socket criado
        cout<<"Error trying to bind address." << endl;
        throw "Error trying to bind address." ;
    }
    if(listen(Num_Socket,1) < 0){ //configura o socket como passivo, ou seja, está pronto para aceitar uma conexão através do método accept()
        cout<<"Error trying to configure socket." << endl;
        throw "Error trying to configure socket.";
    }
}

void Socket::CloseSocket(){
    close(Num_Socket); //fecha um socket que foi aberto
}

int Socket::CreateConnection(){ //inicia a conexão
    int size = sizeof(addr);
    //retorna -1 caso ocorra um erro
    return accept(Num_Socket,(struct sockaddr * ) &addr, (socklen_t*) &size); //abre a conexão e começa a aguardar o recebimento de uma requisição
}

void Socket::CloseConnection(int idConnection){
    close(idConnection); //fecha uma conexão que foi aberta
}

string Socket::ReadRequest(int idConnection){
    int bufferSize = 1024;
    char buffer[bufferSize];
    read(idConnection , buffer, bufferSize); //lê uma requisição recebida pelo socket, armazena em um buffer (char*) e a retorna como um objeto string
    return string(buffer);
}

void Socket::SendResponse(int idConnection, string buffer, int bufferSize){
    send(idConnection, buffer.c_str(), buffer.length(), MSG_CONFIRM); //envia a mensagem contida no objeto buffer via TCP à "outra ponta" do socket
}
