/*
    Grupo:
    Carlos Willian Silva Camargos
    Rafael Borges de Assis
 */

#include "Server.h"

using namespace std;

int main(int argc, char* argv[])
{
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
}
