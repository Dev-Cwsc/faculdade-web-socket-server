/*
    Carlos Willian Silva Camargos
    Lucas Dael'olio de Lima
    Rafael Borges de Assis
*/ 

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h> //access function verifica se existe
//lib de sockets
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>

#include <thread> //biblioteca de threads

#include <limits.h> //realpath
#include <stdlib.h>

#include <iomanip> //para trabalhar com a hora do sistema
#include <ctime>
#include <sstream>

using namespace std;

class Socket_interface{ //as classes Socket_interface e File_Interface são virtuais, servem apenas para definir a assinatura dos métodos que serão sobrescritos pelas classes filhas
    public:
        virtual ~Socket_interface(){}
        virtual void CreateSocket(int port)=0;
        virtual void CloseSocket() =0;
        virtual int CreateConnection()=0;
        virtual void CloseConnection(int idConnection)=0;
        virtual string ReadRequest(int idConnection)=0;
        virtual void SendResponse(int idConnection, string buffer, int bufferSize)=0;
};

class File_Interface
{
public:
    virtual ~File_Interface(){};
    virtual int OpenFile()=0;
    virtual void CloseFile()=0;
    virtual string readFile(int length)=0;
    static bool ExistsFile(string directory);
    virtual int SizeFile()=0; 
    virtual int RemainingSizeFile()=0;
};

class File: public File_Interface //classe usada para fazer a leitura dos arquivos
{
private:
    ifstream *file=NULL;
    string directory;
    int sizeOfFile;
    
    /* data */
public:
    File(){};
    File(string directory);
    ~File(){CloseFile(); delete file;}; //fecha o arquivo e deleta o objeto de file
    virtual int OpenFile(){
        if (!file->is_open()) //caso nao esteja aberto tenta abrir novamente
            file->open(directory,ifstream::in); 
        if (file->is_open())
            return 0;
        return -1;
    };
    virtual void CloseFile(){file->close();};
    virtual string readFile(int length){
        if (length<0) throw "Invalid negative argument";
        char *buffer = new char [length];
        
        if((*file).is_open()) //se o arquivo foi aberto corretamente
        {
            //se o usuario solicitar um tamanho maior que o tamanho restante do arquivo
            if (length>RemainingSizeFile()) 
                (*file).read(buffer,RemainingSizeFile());
            else 
                (*file).read(buffer,length);
             
        }else{
            throw "Error opening file";
        }

        return string(buffer);
    };
    static bool ExistsFile(string directory){
        //access retorna 0=EXISTE 1=Nao existe
        return !access(directory.c_str(),0);
        //return false; /*TODO*/
    };
    virtual int SizeFile(){return sizeOfFile;}; //retorna tamanho do arquivo
    virtual int RemainingSizeFile(){return sizeOfFile - file->tellg();;};
};

File::File(string directory){
    this->directory= directory;//armazena o nome do arquivo
    if (!ExistsFile(directory)) throw "Path not Exists";
    file = new ifstream(directory, ifstream::in);
    if (file->is_open()){
        file->seekg(0, ios::end);//seta para final do arquivo
        sizeOfFile = file->tellg();// pega o valor total do arquivo
        file->seekg(0, ios::beg);//seta novamente para o começo do arquivo
    }else{
        //realiza 10 tentativas de abrir o arquivo caso nao abra uma exception e lançada
        int tmp;
        for(int attempt=0;attempt<10;attempt++){    
            tmp=OpenFile();
            if (tmp==0) break;
        }
        if (!file->is_open()) throw "Open file error";
    }
}

class Socket: public Socket_interface{ //classe responsável por inicializar o socket e fazer as configurações iniciais
    private:
        int Num_Socket;
        struct sockaddr_in addr;
    public:
        Socket(int port){
            CreateSocket(port);
        }
        ~Socket(){}
        virtual void CreateSocket(int port){
            int option=1;
            if((Num_Socket = socket(AF_INET, SOCK_STREAM, 6)) < 0 ){
                cout<<"Error trying to create socket."<<endl;
                throw "Error trying to create socket.";
            }
            
            if( setsockopt(Num_Socket,SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&option, sizeof(option)) ){
                cout<<"Error trying to configure socket."<<endl;
                throw "Error trying to configure socket.";
            }
            
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = INADDR_ANY;
            addr.sin_family = AF_INET;

            if(bind(Num_Socket, (struct sockaddr *) &addr, sizeof(addr))  < 0){
                cout<<"Error trying to establish connection." << endl;
                throw "Error trying to establish connection." ;
            }
            if(listen(Num_Socket,1) < 0){
                cout<<"Error trying to listen to socket." << endl;
                throw "Error trying to listen to socket.";
            }
        };

        virtual void CloseSocket() {
            close(Num_Socket); //fecha o socket que foi aberto
        };
        virtual int CreateConnection(){
            int size = sizeof(addr);
            //retorna -1 caso ocorra um erro
            return accept(Num_Socket,(struct sockaddr * ) &addr, (socklen_t*) &size);
                    
        };
        virtual void CloseConnection(int idConnection){close(idConnection);};
        virtual string ReadRequest(int idConnection){
            int bufferSize = 1024;
            char buffer[bufferSize];
            read( idConnection , buffer, bufferSize);
            return string(buffer);
        };
        virtual void SendResponse(int idConnection, string buffer, int bufferSize){
            send(idConnection, buffer.c_str()
            ,buffer.length(), MSG_CONFIRM);
        };
};

class HTTP //classe responsavel por fazer tratamentos de request e response
{
    private:
        /* data */
    public:
        HTTP(/* args */){};
        ~HTTP(){};
        //pega as variaveis de dentro do corpo da request
        static void getArguments(string request,char * &method, char * &fileName){
            char * bufferAux= new char[30];
            //cria buffers auxiliares para  armazenar os valores
            int i, countArgs=0, count=0; 
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
                bufferAux[count]=request[i];
                count++;
            }
            bufferAux[count]='\0';
            
            fileName=bufferAux;
        
        cout<<endl;


        };
        static string* generateResponse(string statusCode,string Connection,
            string content, string fileType){
            auto t = time(nullptr);
            auto tm = *localtime(&t);

            ostringstream oss;
            oss << put_time(&tm, "%a, %d %b %Y %H:%M:%S GMT-3");
            auto str = oss.str();
            //constroi a string que sera utilizada na resposta da requisição
            return new string("HTTP/1.1 " +statusCode +"\nConnection: "+Connection+"\n Date: "+str+"\nServer: cwrbld/1.0.0\nLast-Modified: "+oss.str()+"\nContent-Length: "+to_string( content.length())+"\nContent-Type: text/"+fileType+"\n\n"+content);
                
        };

};

class Server{
    private:
        Socket_interface * socket=NULL;
    public:
        
        Server(string directory, int port){
            this->socket= new Socket(port);//cria o Socket
            thread *thr;//apontador para thread futuramente pode ser substituido por uma lista de thread
            int idConnection;
            while (true)
            {
                // cria um id para referencia a conexão estabelecida
                idConnection= socket->CreateConnection();
                if (idConnection>=0)
                    // cria uma nova thread passando o id da conexão e o diretorio.
                    thr = new thread (this->RequestTratament, directory,idConnection, this->socket);
            }
        }
        ~Server(){
        };
        static void RequestTratament(string directory,int ConnectionSock, Socket_interface *socket){
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
                http->getArguments(request, method, fileName);
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
                response= http->generateResponse("404 Not Found","Close","", "html"); 
            }
            // verifica se o diretorio existe e se é um subdiretorio do diretorio
            if (access((*chkAcess).c_str(),0) || pos!=0){
                cout<<"NÃO EXISTE"<<endl;
                // gera uma string com a resposta da request
                response= http->generateResponse("404 Not Found","Close","", "html");
                // envia a resposta para o cliente
                socket->SendResponse(ConnectionSock, *response,1024);
            
            }else{
                // se existir o arquivo tenta mandar uma response
                cout<<"EXISTE"<<endl;
                try{
                    //caso nao consiga abrir gera uma excessao que é capturada pelo try
                    file = new File(*chkAcess);
                    response= http->generateResponse("200 OK","Close",file->readFile(file->SizeFile()), "html");

                }catch(...){
                    response= http->generateResponse("404 Not Found","Close","", "html");
                
                }
                file->CloseFile();// fecha o arquivo

                socket->SendResponse(ConnectionSock, *response,1024);
                //delete file,strMethod,strFileName,response;
            
            }

            socket->CloseConnection(ConnectionSock);

        };

        static void DefaultPath(char * &pathResolved){
            int count=0,i;
            for (i = 0; pathResolved[i]!='\0'; i++)
            {
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

        };

};

int main(int argc, char* argv[]){
    try{
        string directory;
        if(argc ==2 )
            directory= argv[1]; //abre o servidor no diretório especificado por parâmetro ao executar o programa
        else
            directory="./"; //caso não seja especificado nenhum diretório, inicia o servidor no diretório atual

        char * realDirectory = new char[PATH_MAX];
        realpath(directory.c_str(), realDirectory);
        string * strRealDirectory= new string(realDirectory);
        cout<<"Server in: "<<*strRealDirectory<<endl;
        int port=3000;
        cout<<"PORT: "<<port<<endl;        
        Server server((*strRealDirectory),port);
        

    }catch(const char* erro){
        cout<<"ERROR: "<<string(erro)<<endl;
    }
    catch(...){
        cout<<"\n Error generic "<<endl;

    }

    return 0;
}