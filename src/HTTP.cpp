#include "HTTP.h"

HTTP::HTTP()
{
    //ctor
}

HTTP::~HTTP()
{
    //dtor
}

void HTTP::GetArguments(string request,char * &method, char * &fileName){//pega as variaveis de dentro do corpo da request
    char * bufferAux= new char[30];
    //cria buffers auxiliares para  armazenar os valores
    unsigned int i, countArgs=0;
    cout<<"\nGet Arguments"<<endl;
    //encontra o primeiro "/" na request que representa o diretorio
    size_t pos= request.find("/");
    for( i=0; i<pos-1;i++ ){// pega o metodo,
        bufferAux[i]=request[i];
    }
    bufferAux[i]='\0';
    method=bufferAux;
    bufferAux = new char[PATH_MAX];
    for( i=pos; request[i]!=' ';i++ ){
        bufferAux[countArgs]=request[i];
        countArgs++;
    }
    bufferAux[countArgs]='\0';

    fileName=bufferAux;

    cout<<endl;
}

string* HTTP::GenerateResponse(string statusCode, string connection, string content, string fileType){
    auto t = time(nullptr);
    auto tm = *localtime(&t);
    ostringstream oss;
    oss << put_time(&tm, "%a, %d %b %Y %H:%M:%S GMT-3"); //formata a data e hora para serem enviadas na resposta
    auto str = oss.str();
    //constroi a string que sera utilizada na resposta da requisição
    return new string("HTTP/1.1 " +statusCode +"\nConnection: "+connection+"\n Date: "+str
                      +"\nServer: cwrbld/1.0.0\nLast-Modified: "+oss.str()+"\nContent-Length: "+
                      to_string( content.length())+"\nContent-Type: text/"+fileType+"\n\n"+content);
}
