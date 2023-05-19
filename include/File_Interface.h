#ifndef FILE_INTERFACE_H
#define FILE_INTERFACE_H

#include <string>

using namespace std;

class File_Interface{//as classes Socket_interface e File_Interface s�o virtuais, servem apenas para definir a assinatura dos m�todos que ser�o sobrescritos pelas classes filhas
    public:
        virtual ~File_Interface(){};
        virtual int OpenFile()=0;
        virtual void CloseFile()=0;
        virtual string ReadFile(int length)=0;
        static bool ExistsFile(string directory);
        virtual int FileSize()=0;
        virtual int RemainingFileSize()=0;
};

#endif // FILE_INTERFACE_H
