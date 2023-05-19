#include "Server.h"

Server::Server(){
    //ctor
}

Server::Server(string directory, int port){
    this->socket= new Socket(port);//cria o Socket
    int idConnection;
    while (true)
    {
        // cria um id para referencia a conexão estabelecida
        idConnection = socket->CreateConnection();
        if (idConnection>=0)
            // cria uma nova thread passando o id da conexão e o diretorio.
            thread *thr = new thread (this->RequestTreatment, directory, idConnection, this->socket);
    }
}

Server::~Server(){
    //dtor
}

void Server::RequestTreatment(string directory, int ConnectionSock, Socket_Interface *socket){
    HTTP * http = new HTTP();//cria um objeto http para tratar requisições
    char * method, * fileName,*pathReal=new char[PATH_MAX];
    string *strMethod,*strFileName;
    File_Interface * file;
    string *response,*chkAcess;
    int pos=-1;

    // lê a mensagem que é enviada para o servidor
    try{
        string request(socket->ReadRequest(ConnectionSock));
        cout<<"\n\n\n"<<request<<endl;
        // função que captura os campos method e o diretorio do arquivo
        http->GetArguments(request, method, fileName);
        strMethod= new string(method);
        strFileName= new string(fileName);
        // padroniza o path do arquivo com o diretório real da maquina
        realpath((directory+(*strFileName)).c_str(),pathReal);
        DefaultPath(pathReal);
        chkAcess = new string(pathReal);
        //tenta encontrar o diretorio  raiz dentro do diretorio resultante se for 0 esta no começo
        pos = chkAcess->find(directory);
        cout <<"Posicao Diretorio: "<<pos<<endl;
        cout<<"RealPATH: "<< *chkAcess<<endl;
        cout<<"Method: "<< *strMethod << "\n Nome do Arquivo: "<<*strFileName<<endl;
    }catch(...){
        response= http->GenerateResponse("404 Not Found","Close","", "html");
    }
    // verifica se o diretorio existe e se é um subdiretorio do diretorio
    if (access((*chkAcess).c_str(),0) || pos!=0){
        cout<<"NÃO EXISTE"<<endl;
        // gera uma string com a resposta da request
        response= http->GenerateResponse("404 Not Found","Close","", "html");
        // envia a resposta para o cliente
        socket->SendResponse(ConnectionSock, *response,1024);

    }else{
        // se existir o arquivo tenta mandar uma response
        cout<<"EXISTE"<<endl;
        try{
            //caso nao consiga abrir gera uma excessao que é capturada pelo try
            file = new File(*chkAcess);
            response= http->GenerateResponse("200 OK","Close",file->ReadFile(file->FileSize()), "html");

        }catch(...){
            response= http->GenerateResponse("404 Not Found","Close","", "html");

        }
        file->CloseFile();// fecha o arquivo

        socket->SendResponse(ConnectionSock, *response,1024);
        //delete file,strMethod,strFileName,response;
    }
    //socket->SendResponse(ConnectionSock, *response,1024);
    socket->CloseConnection(ConnectionSock);
}

void Server::DefaultPath(char *&pathResolved){
    int count=0,i;
    for (i = 0; pathResolved[i]!='\0'; i++){
        count++;
    }
    cout<<"DefaultPath.pathResolved:"<<pathResolved<<endl<<"length: "<<count<<endl;

    bool isFile=false;
    for (i=count-1; i>=0 && pathResolved[i]!='/';i--){
        if (pathResolved[i]=='.')
            isFile=true;
    }
    if (!isFile){
        for(i=count;i<(count+12);i++){
            pathResolved[i]= "/index.html\0"[i-count];
        }
        cout<<"DefaultPath.pathResolved:"<<pathResolved<<endl;
    }
}
