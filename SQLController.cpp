//
// Created by ruben on 13/06/19.
//

#include "SQLController.h"

/**
 * Constructor
 */
SQLController::SQLController() {
    stringToRead = "";
}


///Metodos




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