#ifndef HTTP_H
#define HTTP_H

#include <iostream>
#include <sstream>
#include <string>
#include <limits.h>
#include <iomanip>

using namespace std;

class HTTP{ //classe responsavel por fazer tratamentos de request e response
    public:
        HTTP();
        virtual ~HTTP();
        static void GetArguments(string, char*&, char*&);
        static string* GenerateResponse(string, string, string, string);
    protected:

    private:
};

#endif // HTTP_H
