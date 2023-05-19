#ifndef FILE_H
#define FILE_H

#include <string>
#include <fstream>
#include <unistd.h> //access function usada no método ExistsFile(). Verifica se existe
#include "File_Interface.h"

using namespace std;

class File : public File_Interface{ //classe usada para fazer a leitura dos arquivos
    public:
        File();
        File(string);
        virtual ~File();
        virtual void CloseFile();
        virtual int OpenFile();
        virtual string ReadFile(int);
        static bool ExistsFile(string);
        virtual int FileSize();
        virtual int RemainingFileSize();
    private:
        ifstream *file=NULL;
        string directory;
        int sizeOfFile;
};

#endif // FILE_H
