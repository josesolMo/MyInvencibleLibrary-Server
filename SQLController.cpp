//
// Created by ruben on 13/06/19.
//

#include "SQLController.h"
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <string>
#include <vector>



using namespace std;
/**
 * Constructor
 */
SQLController::SQLController() {
    stringToRead = "";
}


///Metodos

void SQLController::funcionInsert(string comando)
{
    string subs = comando.substr(0,5);
    if (subs.compare("INTO ") == 0 ||subs.compare("into ") == 0){
        subs = comando.substr(5);
        size_t space = subs.find(" ");
        if (subs.length()< space){
            cout << "Syntax error" << endl;
            return;
        }
        string galery = subs.substr(0, space);
        cout << galery << endl;
        subs = subs.substr(space+1);
        vector <string> order;
        if((subs.substr(0,1)).compare("(") == 0){
            size_t columnsEnd = subs.find(")");
            if (subs.length()< columnsEnd){
                cout << "Syntax error" << endl;
                return;
            }
            string columns = subs.substr(1, columnsEnd);
            cout << columns << endl;
            subs = subs.substr(columnsEnd+1);
            while (columns.compare(")") != 0){
                size_t coma = columns.find(",");
                string current;
                if (columns.length()< coma){
                    current = columns.substr(0, columns.length()-1);
                    //cout << current << endl;
                    columns = columns.substr(current.length(),columns.length()-1);
                }
                else {
                    current = columns.substr(0, coma);
                    //cout << current << endl;
                    columns = columns.substr(coma+1);
                    //cout << columns << endl;
                }
                if(current[0] == ' '){
                    current = current.substr(1);
                }
                if (current[current.length()-1] == ' '){
                    current = current.substr(0, current.length()-1);
                }
                cout << current << endl;
                cout << columns << endl;
                current = columnaGET(current);
                if(current.compare("F") == 0){
                    cout << "No existe ese dato en la tabla" << endl;
                    return;
                }
                else {
                    order.push_back(current);
                }
            }
            int C = 0;
            while (C < order.size()){
                cout << order[C] << endl;
                C++;
            }
        }
        else {
            order.push_back("A");
            order.push_back("B");
            order.push_back("C");
            order.push_back("D");
            order.push_back("E");

        }
        size_t values = subs.find("VALUES ");
        if(values > 2){
            cout << "Syntax error" << endl;
            return;
        }

        subs = subs.substr(values+7);
        cout << subs << endl;

        if((subs.substr(0,1)).compare("(") == 0){
            size_t valuesEnd = subs.find(")");
            if (subs.length()< valuesEnd){
                cout << "Syntax error" << endl;
                return;
            }
            string values = subs.substr(1, valuesEnd);
            cout << values << endl;
            int c = 0;
            while (values.compare(")") != 0){
                size_t coma = values.find(",");
                string current;
                if (values.length()< coma){
                    current = values.substr(0, values.length()-1);
                    cout << current << endl;
                    values = values.substr(current.length(),values.length()-1);
                    int min = order.size()-1;
                    if(c != min){
                        cout << "Faltan valores o sobran" << endl;
                        return;
                    }
                }
                else {
                    current = values.substr(0, coma);
                    cout << current << endl;
                    values = values.substr(coma+1);
                    cout << values << endl;
                }
                if(current[0] == '"'){
                    current = current.substr(1);
                }
                if (current[current.length()-1] == '"'){
                    current = current.substr(0, current.length()-1);
                }
                //cout << current << endl;
                //cout << columns << endl;
                addToTable(order[c],current);
                c++;
            }
            //ui->LineaCMD->clear();
            return;
        }
        else {
            cout << "Syntax error" << endl;
            return;
        }
    }
    else {
        cout << "Syntax error" << endl;
        return;
    }
}

void SQLController::funcionSelect(string comando)
{
    string subs = comando;
    cout << subs << endl;
    if (subs[0] == '*'){
        size_t From = subs.find("FROM ");
        if (3 < From){
            cout << "Syntax error" << endl;
            return;
        }
        subs = subs.substr(From+5);
        size_t pycoma = subs.find(";");
        if (subs.length()< pycoma){
            cout << "Syntax error" << endl;
            return;
        }
        string tabla = subs.substr(0, pycoma);
        cout << tabla << endl;
        if(tabla[0] == ' '){
            tabla = tabla.substr(1);
        }
        if (tabla[tabla.length()-1] == ' '){
            tabla = tabla.substr(0, tabla.length()-1);
        }
        subs = subs.substr(pycoma+1);
        cout << subs << endl;
        if(subs.compare("") == 0 || subs.compare(" ") == 0){
            cout << "Imprimir valores de tabla" << endl;
            return;
        }
        else{
            size_t where = subs.find("WHERE ");
            if (2 < where){
                cout << "Syntax error" << endl;
                return;
            }
            subs = subs.substr(where+6);
            if (subs.substr(0,4).compare("NOT ") == 0){
                subs = subs.substr(0,4);
                size_t comand = subs.find("= ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+2);
                    cout << column << endl;
                    size_t pycoma = subs.find(";");
                    if (subs.length()< pycoma){
                        cout << "Syntax error" << endl;
                        return;
                    }
                    string value = subs.substr(0, pycoma);
                    cout << value << endl;
                    if(value[0] == '"'){
                        value = value.substr(1);
                    }
                    if (value[value.length()-1] == '"'){
                        value = value.substr(0, value.length()-1);
                    }
                    cout << value << endl;
                    return;
                }

                comand = subs.find("< ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+2);
                    cout << column << endl;
                }

                comand = subs.find("> ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+2);
                    cout << column << endl;
                }

                comand = subs.find(">= ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+3);
                    cout << column << endl;
                }

                comand = subs.find("<= ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+3);
                    cout << column << endl;
                }
                else{
                    cout << "Syntax error" << endl;
                    return;
                }

            }
            else {

                size_t comand = subs.find("NOT ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+3);
                    cout << column << endl;
                }

                comand = subs.find("BETWEEN ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+8);
                    cout << column << endl;
                }

                comand = subs.find("IN ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+3);
                    cout << column << endl;
                }

                comand = subs.find("= ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+2);
                    cout << column << endl;
                }

                comand = subs.find("< ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+2);
                    cout << column << endl;
                }

                comand = subs.find("> ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+2);
                    cout << column << endl;
                }

                comand = subs.find(">= ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+3);
                    cout << column << endl;
                }

                comand = subs.find("<= ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+3);
                    cout << column << endl;
                }

                comand = subs.find("IS ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+3);
                    cout << column << endl;
                }

                comand = subs.find("LIKE ");
                if (subs.length() > comand){
                    string column = subs.substr(0, comand);
                    subs = subs.substr(comand+5 );
                    cout << column << endl;
                }
                else{
                    cout << "Syntax error" << endl;
                    return;
                }
            }


            /*
             * COLOCAR TODAS LAS POSIBLES CONDICIONALES[BETWEEN(AND, OR, NOT),IS(NULL, NOT NULL),LIKE]
             */
        }
    }
    else if(subs.find("FROM ") < subs.length()){

    }
    else {
        cout << "Syntax error" << endl;
        return;
    }

}

void SQLController::funcionUpdate(string comando)
{
    cout << comando << endl;
}

void SQLController::funcionDelete(string comando)
{
    cout << comando << endl;
}

string SQLController::columnaGET(string evaluar)
{
    if(evaluar.compare("Fecha") == 0){
        return "A";
    }
    else if(evaluar.compare("Nombre") == 0){
        return "B";

    }
    else if(evaluar.compare("Autor") == 0){
        return "C";

    }
    else if(evaluar.compare("Tamano") == 0){
        return "D";

    }
    else if(evaluar.compare("Descripcion") == 0){
        return "E";

    }
    else{
        return "F";

    }
}

void SQLController::addToTable(string columna, string value)
{
    if(columna.compare("A") == 0){
        //this->fecha = value;
    }
    else if(columna.compare("B") == 0){
        //this->nombre = value;
    }
    else if(columna.compare("C") == 0){
        //this->autor = value;
    }
    else if(columna.compare("D") == 0){
        //this->size = value;
    }
    else if(columna.compare("E") == 0){
        //this->descripcion = value;
    }
}


///Getters y Setters


/**
 * Getter
 * @return
 */
string SQLController::getStringToRead() {
    return stringToRead;
}

/**
 * Setter
 * @param str
 */
void SQLController::setStringToRead(string str) {
    stringToRead = str;
}

void SQLController::makeFunction(string comando) {

    if(comando.length() < 7){
        cout << "Syntax error" << endl;

    }
    else {
        string subs = comando.substr(0,7);
        string cmd = comando.substr(7);
        if (subs.compare("INSERT ") == 0 || subs.compare("insert ") == 0){
            cout << "Funcion INSERT" << endl;
            funcionInsert(cmd);
        }else if (subs.compare("SELECT ") == 0 || subs.compare("select ") == 0){
            cout << "Funcion SELECT" << endl;
            funcionSelect(cmd);
            //ui->LineaCMD->clear();
        }else if (subs.compare("UPDATE ") == 0 || subs.compare("update ") == 0){
            cout << "Funcion UPDATE" << endl;
            funcionUpdate(cmd);
            //ui->LineaCMD->clear();
        }else if (subs.compare("DELETE ") == 0 || subs.compare("delete ") == 0){
            cout << "Funcion DELETE" << endl;
            funcionDelete(cmd);
            //ui->LineaCMD->clear();
        }else {
            cout << "Syntax error" << endl;
        }
    }

}
