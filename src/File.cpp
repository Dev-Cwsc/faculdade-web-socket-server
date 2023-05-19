#include "File.h"

File::File(){
    //ctor
}

File::File(string dir){
    directory = dir;//armazena o nome do arquivo
    if (!ExistsFile(directory))
        throw "Path not exists.";
    file = new ifstream(directory, ifstream::in);
    if (file->is_open()){
        file->seekg(0, ios::end);//seta para final do arquivo
        sizeOfFile = file->tellg();// pega o valor total do arquivo
        file->seekg(0, ios::beg);//seta novamente para o começo do arquivo
    }else{
        //realiza 10 tentativas de abrir o arquivo caso nao abra uma exception é lançada
        int tmp;
        for(int attempt=0;attempt<10;attempt++){
            tmp=OpenFile();
            if (tmp==0) break;
        }
        if (!file->is_open()) throw "Open file error";
    }
}

File::~File(){
    CloseFile(); //fecha o arquivo
    delete file; //deleta o objeto file
}

void File::CloseFile(){
    file->close();
}

int File::OpenFile(){
    if (!file->is_open()) //caso nao esteja aberto tenta abrir novamente
        file->open(directory,ifstream::in);
    if (file->is_open())
        return 0;
    return -1;
}

string File::ReadFile(int lenght){
    if (lenght<0) throw "Invalid negative argument.";
        char *buffer = new char [lenght];
    if((*file).is_open()) //se o arquivo foi aberto corretamente
    {
        if (lenght>RemainingFileSize()) //se o usuario solicitar um tamanho maior que o tamanho restante do arquivo
            (*file).read(buffer,RemainingFileSize());
        else
            (*file).read(buffer,lenght);
    }else{
        throw "Error opening file.";
    }
    return string(buffer);
}

bool File::ExistsFile(string dir){
    //access retorna false=EXISTE true=Não existe
    return !access(dir.c_str(),0);
}

int File::FileSize(){
    return sizeOfFile; //retorna tamanho do arquivo
}

int File::RemainingFileSize(){
    return sizeOfFile - file->tellg(); //calcula quanto espa�o livre h� no arquivo
}
