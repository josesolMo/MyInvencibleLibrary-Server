//
// Created by ruben on 13/06/19.
//

#ifndef MYINVINCIBLELIBRARY_SERVER_SQLCONTROLLER_H
#define MYINVINCIBLELIBRARY_SERVER_SQLCONTROLLER_H

#include <string>

using namespace std;

class SQLController {

private:
    string stringToRead;

public:
    SQLController();
    string getStringToRead();
    void setStringToRead(string str);

};


#endif //MYINVINCIBLELIBRARY_SERVER_SQLCONTROLLER_H
