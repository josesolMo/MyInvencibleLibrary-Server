
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <strings.h>
#include <unistd.h>
#include <cstring>
#include <json-c/json.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <string>

#include "ServerController.h"

#define PORT 3550
#define BACKLOG 4
#define MAXDATASIZE 1000

using namespace std;


/**
 * Server de MyInvincibleLibrary
 *
 * @since 08/06/19
 */


static ServerController* serverController;
static string JSONReturnGen;
static json_object *JSONReturnConsole;

///Creacion inicial del JSON por enviar
json_object *jObj = json_object_new_object();


/**
 *
 * @param jObj
 * @param destinatary
 * @return
 */
int sendJSON(json_object *jObj, string destinatary) {

    ///Guarda el ip que se utilizara para la comunicacion
    string ipAddress;

    if (destinatary == "RAID") {
        ipAddress = "192.168.100.4";
    } else if (destinatary == "MetadataDB") {
        ipAddress = "192.168.100.2"; //192.168.100.5
        }


    char* str;
    int fd, numbytes;
    struct sockaddr_in client;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    char sendBuff[MAXDATASIZE];
    char recvBuff[MAXDATASIZE];

    struct hostent *he;

    if (fd < 0)
    {
        printf("Error : Could not create socket\n");
        return 1;
    }
    else
    {
        client.sin_family = AF_INET;
        client.sin_port = htons(PORT);
        ///Toma el ipAddress dependiendo del destinatary
        client.sin_addr.s_addr = inet_addr(ipAddress.c_str());
        memset(client.sin_zero, '\0', sizeof(client.sin_zero));
    }

    if (::connect(fd, (const struct sockaddr *)&client, sizeof(client)) < 0)
    {
        printf("ERROR connecting to server\n");
        return 1;
    }


    ///Se agrega el objeto JSON estatico


    if (strcpy(sendBuff, json_object_to_json_string(jObj)) == NULL) {
        printf("ERROR strcpy()");
        exit(-1);
    }

    if (write(fd, sendBuff, strlen(sendBuff)) == -1)
    {
        printf("ERROR write()");
        exit(-1);
    }

    if ((numbytes=recv(fd,recvBuff,MAXDATASIZE,0)) < 0){

        printf("Error en recv() \n");
        exit(-1);
    }


    ///Manejo de KEYS


    ///KEY: NEWGALLERY
    struct json_object *tempNewGallery;
    json_object *parsed_jsonNewGallery = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonNewGallery, "NEWGALLERY", &tempNewGallery);
    if (json_object_get_string(tempNewGallery) != nullptr){
        JSONReturnGen = json_object_get_string(tempNewGallery);
    }

    ///KEY: NEWIMAGE
    struct json_object *tempNewImage;
    json_object *parsed_jsonNewImage = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonNewImage, "NEWIMAGE", &tempNewImage);
    if (json_object_get_string(tempNewImage) != nullptr){
        JSONReturnGen = json_object_get_string(tempNewImage);
    }

    ///KEY: CONSOLE
    struct json_object *tempConsole;
    json_object *parsed_jsonNewConsole = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonNewConsole, "CONSOLE", &tempConsole);
    if (json_object_get_string(tempConsole) != nullptr){
        //JSONReturnGen = json_object_get_string(tempConsole);
        JSONReturnConsole = tempConsole;
    }

    ///KEY: INITIAL
    struct json_object *tempInitial;
    json_object *parsed_jsonInitial = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonInitial, "INITIAL", &tempInitial);
    if (json_object_get_string(tempInitial) != 0) {
        ///Variable por guardar o funcion por llamar
        cout << "Respuesta MetadataDB" << endl;
        JSONReturnConsole = tempInitial;
    }

    ///KEY: COMMIT
    struct json_object *tempCommit;
    json_object *parsed_jsonCommit = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonCommit, "COMMIT", &tempCommit);
    if (json_object_get_string(tempCommit) != 0) {
        ///Variable por guardar o funcion por llamar
        JSONReturnGen = json_object_get_string(tempCommit);
    }

    ///KEY: ROLLBACK
    struct json_object *tempRollback;
    json_object *parsed_jsonRollback = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonRollback, "ROLLBACK", &tempRollback);
    if (json_object_get_string(tempRollback) != 0) {
        ///Variable por guardar o funcion por llamar
        JSONReturnGen = json_object_get_string(tempRollback);
    }

    ///KEY: DELETEGALLERY
    struct json_object *tempDeleteGallery;
    json_object *parsed_jsonDeleteGallery = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonDeleteGallery, "DELETEGALLERY", &tempDeleteGallery);
    if (json_object_get_string(tempDeleteGallery) != 0) {
        ///Variable por guardar o funcion por llamar
        JSONReturnGen = json_object_get_string(tempDeleteGallery);
    }



/*
    ///KEY:
    struct json_object *temp;
    json_object *parsed_json = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_json, "KEY", &temp);
    if (json_object_get_string(temp) != 0) {
        ///Variable por guardar o funcion por llamar
        JSONReturnGen = json_object_get_string(temp);
    }
*/



    ///Se limpian los Buffers
    memset(recvBuff, 0, MAXDATASIZE);
    memset(sendBuff, 0, MAXDATASIZE);

    ::close(fd);

}



/**
 * Obtiene la lista inicial de galerias e imagenes para iniciar el cliente
 * @param initial
 * @param initialKey
 * @return JSON
 */
string sendInitial(string initial, string initialKey) {

    ///Redefine el jObject para eliminar sus keys antiguos
    jObj = json_object_new_object();

    ///Toma el nombre de la nueva imagen
    json_object *jstringConsole = json_object_new_string(initial.c_str());
    ///Toma el key de la nueva imagen
    json_object_object_add(jObj,initialKey.c_str(), jstringConsole);

    ///Se manda el texto de la consola en el cliente al MetadataDB
    sendJSON(jObj, "MetadataDB");

    ///Toma el key de la nueva imagen
    json_object_object_add(jObj,initialKey.c_str(), JSONReturnConsole);

    return json_object_to_json_string(jObj);

}


/**
 * Envia un JSON a la base de datos para verificar la disponibilidad de el nombre de la galeria entrante.
 * @param newGallery
 * @param newGalleryKey
 * @return JSON
 */
string sendNewGallery(string newGallery, string newGalleryKey) {

    ///Redefine el jObject para eliminar sus keys antiguos
    jObj = json_object_new_object();

    ///Toma la data que le entra como parametro en la funcion
    json_object *jstringNewGallery = json_object_new_string(newGallery.c_str());
    ///Toma el key que le entra como parametro en la funcion
    json_object_object_add(jObj,newGalleryKey.c_str(), jstringNewGallery);

    sendJSON(jObj, "MetadataDB");

    ///Preparacion del JSON con el resultado de la disponibilidad
    ///del nombre de la imagen por agregar.

    if (JSONReturnGen == "1" || JSONReturnGen == "0") {
        cout << "JSONReturnGen: " << JSONReturnGen << endl;
        json_object *jobjSendNewGallery = json_object_new_object();
        json_object *jstringSendNewGallery = json_object_new_string(
                JSONReturnGen.c_str()); /// "1" si se agrega o "0" si no
        json_object_object_add(jobjSendNewGallery, "NEWGALLERY", jstringSendNewGallery);
        return json_object_to_json_string(jobjSendNewGallery);
    }
    else {
        json_object *jobjError = json_object_new_object();
        json_object *jstringError = json_object_new_string("ERROR: NEWGALLERY");
        json_object_object_add(jobjError, "NEWGALLERY", jstringError);
        return json_object_to_json_string(jobjError);

    }

}

/**
 * Envia un JSON a la base de datos para verificar la disponibilidad de el nombre de la imagen entrante.
 * @param newImage
 * @param newImageKey
 * @param gallery
 * @param galleryKey
 * @param binaryData
 * @param binaryDataKey
 * @return JSON
 */
string sendNewImage(string newImage,  string newImageKey, string gallery, string galleryKey,
        string binaryData, string binaryDataKey) {

    ///Redefine el jObject para eliminar sus keys antiguos
    jObj = json_object_new_object();

    ///Toma el nombre de la nueva imagen
    json_object *jstringNewImage = json_object_new_string(newImage.c_str());
    ///Toma el key de la nueva imagen
    json_object_object_add(jObj,newImageKey.c_str(), jstringNewImage);

    ///Toma el nombre del gallery donde se guardara la imagen
    json_object *jstringGallery = json_object_new_string(gallery.c_str());
    ///Toma el key de la nueva imagen
    json_object_object_add(jObj,galleryKey.c_str(), jstringGallery);

    sendJSON(jObj, "MetadataDB");


    ///Se crea el objeto JSON que se devolvera al cliente
    json_object *jobjSendNewImage = json_object_new_object();


    if (JSONReturnGen == "1" || JSONReturnGen == "0") {

        if (JSONReturnGen == "1") {
            sendJSON(jObj, "RAID");
        }

        json_object *jstringSendNewImage = json_object_new_string(JSONReturnGen.c_str()); /// "1" o "0"
        json_object_object_add(jobjSendNewImage, "NEWIMAGE", jstringSendNewImage);
        return json_object_to_json_string(jObj);

    } else {

        json_object *jobjError = json_object_new_object();
        json_object *jstringError = json_object_new_string("ERROR: NEWIMAGE");
        json_object_object_add(jobjError, "NEWIMAGE", jstringError);
        return json_object_to_json_string(jobjError);

    }
}

/**
 * Evia el texto que se escribio en la consola a la clase SQLController
 * @param console
 * @param consoleKey
 * @return string
 */
string sendConsole(string console, string consoleKey) {

    ///Redefine el jObject para eliminar sus keys antiguos
    jObj = json_object_new_object();

    ///Toma el nombre de la nueva imagen
    json_object *jstringConsole = json_object_new_string(console.c_str());
    ///Toma el key de la nueva imagen
    json_object_object_add(jObj,consoleKey.c_str(), jstringConsole);

    ///Se manda el texto de la consola en el cliente al MetadataDB
    sendJSON(jObj, "MetadataDB");

    ///Redefine el jObject para eliminar sus keys antiguos
    jObj = json_object_new_object();


    ///Toma el key de la nueva imagen
    json_object_object_add(jObj,consoleKey.c_str(), JSONReturnConsole);


    cout << "SENDING: " << json_object_to_json_string(jObj) << endl;


    return json_object_to_json_string(jObj);

}

/**
 *
 * @param commit
 * @param commitKey
 * @return
 */
string sendCommit(string commit, string commitKey) {

    ///Redefine el jObject para eliminar sus keys antiguos
    jObj = json_object_new_object();

    ///Toma el nombre de la nueva imagen
    json_object *jstringCommit = json_object_new_string(commit.c_str());
    ///Toma el key de la nueva imagen
    json_object_object_add(jObj,commitKey.c_str(), jstringCommit);

    ///Se manda el texto de la consola en el cliente al MetadataDB
    sendJSON(jObj, "MetadataDB");

    ///Redefine el jObject para eliminar sus keys antiguos
    jObj = json_object_new_object();

    ///Toma el nombre de la nueva imagen
    json_object *jstringCommit2 = json_object_new_string(JSONReturnGen.c_str());

    ///Toma el key de la nueva imagen
    json_object_object_add(jObj,commitKey.c_str(), jstringCommit2);

    return json_object_to_json_string(jObj);




}

/**
 *
 * @param rollback
 * @param rollbackKey
 * @return
 */
string sendRollback(string rollback, string rollbackKey) {

    ///Redefine el jObject para eliminar sus keys antiguos
    jObj = json_object_new_object();

    ///Toma el nombre de la nueva imagen
    json_object *jstringRollback = json_object_new_string(rollback.c_str());
    ///Toma el key de la nueva imagen
    json_object_object_add(jObj,rollbackKey.c_str(), jstringRollback);

    ///Se manda el texto de la consola en el cliente al MetadataDB
    sendJSON(jObj, "MetadataDB");

    ///Redefine el jObject para eliminar sus keys antiguos
    jObj = json_object_new_object();

    ///Toma el nombre de la nueva imagen
    json_object *jstringRollback2 = json_object_new_string(JSONReturnGen.c_str());

    ///Toma el key de la nueva imagen
    json_object_object_add(jObj,rollbackKey.c_str(), jstringRollback2);

    return json_object_to_json_string(jObj);

}



string sendDeleteGallery(string gallery, string deleteGalleryKey) {

    ///Redefine el jObject para eliminar sus keys antiguos
    jObj = json_object_new_object();

    ///Toma el nombre de la nueva imagen
    json_object *jstringRollback = json_object_new_string(gallery.c_str());
    ///Toma el key de la nueva imagen
    json_object_object_add(jObj,deleteGalleryKey.c_str(), jstringRollback);

    ///Se manda el texto de la consola en el cliente al MetadataDB
    sendJSON(jObj, "MetadataDB");

    ///Redefine el jObject para eliminar sus keys antiguos
    jObj = json_object_new_object();

    ///Toma el nombre de la nueva imagen
    json_object *jstringRollback2 = json_object_new_string(JSONReturnGen.c_str());

    ///Toma el key de la nueva imagen
    json_object_object_add(jObj,deleteGalleryKey.c_str(), jstringRollback2);

    return json_object_to_json_string(jObj);

}


/**
 * Corre el servidor.
 * @return int
 */
int runServer() {

    int fd, fd2;

    struct sockaddr_in server;
    struct sockaddr_in client;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("error en socket()\n");
        exit(-1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    bzero(&(server.sin_zero), 8);

    if (bind(fd, (struct sockaddr *) &server,
             sizeof(struct sockaddr)) == -1) {
        printf("error en bind() \n");
        exit(-1);
    }

    if (listen(fd, BACKLOG) == -1) {
        printf("error en listen()\n");
        exit(-1);
    }

    printf("\nServidor 'My Invincible Library - Server' abierto.\n");

    while (true) {

        unsigned int address_size = sizeof(client);

        if ((fd2 = accept(fd, (struct sockaddr *) &client,
                          &address_size)) == -1) {
            printf("error en accept()\n");
            exit(-1);
        }

        printf("\nSe obtuvo una conexión de un cliente.\n");

        ssize_t r;

        char buff[MAXDATASIZE];

        for (;;) {
            r = read(fd2, buff, MAXDATASIZE);

            if (r == -1) {
                perror("read");
                return EXIT_FAILURE;
            }
            if (r == 0)
                break;
            printf("\nREAD: %s\n", buff);


            ///JSON Reads


            ///KEY: NEWGALLERY
            ///Obtiene el nombre de la nueva galeria para verificar si puede ser guardada.
            struct json_object *tempNewGallery;
            json_object *parsed_jsonNewGallery = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonNewGallery, "NEWGALLERY", &tempNewGallery);

            ///KEY: NEWIMAGE
            ///Obtiene el nombre de la nueva imagen para verificar si puede ser guardada.
            struct json_object *tempNewImage;
            json_object *parsed_jsonNewImage = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonNewImage, "NEWIMAGE", &tempNewImage);

            ///KEY: GALLERY
            ///Obtiene la galeria donde se cambiara o se retornara su contenido
            struct json_object *tempGallery;
            json_object *parsed_jsonGallery = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonGallery, "GALLERY", &tempGallery);

            ///KEY: BINARYDATA
            ///Obtiene un request para obtener el binaryData de la imagen por
            struct json_object *tempBinaryData;
            json_object *parsed_jsonBinaryData = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonBinaryData, "BINARYDATA", &tempBinaryData);

            ///KEY: CONSOLE
            ///Obtiene un request para enviar el texto de la consola e interpretarlo
            struct json_object *tempConsole;
            json_object *parsed_jsonConsole = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonConsole, "CONSOLE", &tempConsole);

            ///KEY: INITIAL
            ///Obtiene un request para obtener las galerias e imagenes en la base de datos
            struct json_object *tempInitial;
            json_object *parsed_jsonInitial = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonInitial, "INITIAL", &tempInitial);

            ///KEY: COMMIT
            ///Obtiene un request para
            struct json_object *tempCommit;
            json_object *parsed_jsonCommit = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonCommit, "COMMIT", &tempCommit);

            ///KEY: ROLLBACK
            ///Obtiene un request para
            struct json_object *tempRollback;
            json_object *parsed_jsonRollback = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonRollback, "ROLLBACK", &tempRollback);

            ///KEY: DELETEGALLERY
            ///Obtiene un request para
            struct json_object *tempDeleteGallery;
            json_object *parsed_jsonDeleteGallery = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonDeleteGallery, "DELETEGALLERY", &tempDeleteGallery);



            /*
            ///KEY: TEMPLATE
            ///Obtiene un request para
            struct json_object *tempZZ;
            json_object *parsed_jsonZZ = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonZZ, "ZZ", &tempZZ);
             */





            ///JSON Writes



            ///Obtendra un request para obtener la disponibilidad de la galeria.
            ///Verifica que reciba los KEYS: NEWGALLERY
            if (json_object_get_string(tempNewGallery) != nullptr ) {
                ///JSON saliente del servidor
                string newGallery = sendNewGallery(json_object_get_string(tempNewGallery), "NEWGALLERY");
                ///Envio al cliente
                send(fd2, newGallery.c_str(), MAXDATASIZE, 0);
            }


            ///Obtendra un request para obtener la disponibilidad de la nueva imagen.
            ///Verifica que reciba los KEYS: NEWIMAGE, GALLERY & BINARYDATA
            if (json_object_get_string(tempNewImage) != nullptr && json_object_get_string(tempGallery) != nullptr &&
                    json_object_get_string(tempBinaryData) != nullptr) {
                ///JSON saliente del servidor
                string newImage = sendNewImage(json_object_get_string(tempNewImage), "NEWIMAGE",
                        json_object_get_string(tempGallery), "GALLERY",
                        json_object_get_string(tempBinaryData), "BINARYDATA");
                ///Envio al cliente
                send(fd2, newImage.c_str(), MAXDATASIZE, 0);
            }

            ///Obtendra un request para obtener
            ///Verifica que reciba los KEYS: CONSOLE
            if (json_object_get_string(tempConsole) != nullptr ) {
                ///JSON saliente del servidor
                string sendConsoleBack = sendConsole(json_object_get_string(tempConsole), "CONSOLE");
                ///Envio al cliente
                send(fd2, sendConsoleBack.c_str(), MAXDATASIZE, 0);
            }

            ///Obtendra un request para obtener
            ///Verifica que reciba los KEYS: INITIAL
            if (json_object_get_string(tempInitial) != nullptr ) {
                ///JSON saliente del servidor
                string sendInitialData = sendInitial(json_object_get_string(tempInitial), "INITIAL");
                ///Envio al cliente
                send(fd2, sendInitialData.c_str(), MAXDATASIZE, 0);
            }

            ///Obtendra un request para obtener
            ///Verifica que reciba los KEYS: COMMIT
            if (json_object_get_string(tempCommit) != nullptr ) {
                ///JSON saliente del servidor
                string commit = sendCommit(json_object_get_string(tempCommit), "COMMIT");
                ///Envio al cliente
                send(fd2, commit.c_str(), MAXDATASIZE, 0);
            }

            ///Obtendra un request para obtener
            ///Verifica que reciba los KEYS: ROLLBACK
            if (json_object_get_string(tempRollback) != nullptr ) {
                ///JSON saliente del servidor
                string rollback = sendRollback(json_object_get_string(tempRollback), "ROLLBACK");
                ///Envio al cliente
                send(fd2, rollback.c_str(), MAXDATASIZE, 0);
            }

            ///Obtendra un request para obtener
            ///Verifica que reciba los KEYS: DELETEGALLERY
            if (json_object_get_string(tempDeleteGallery) != nullptr ) {
                ///JSON saliente del servidor
                string deleteGallery = sendDeleteGallery(json_object_get_string(tempDeleteGallery), "DELETEGALLERY");
                ///Envio al cliente
                send(fd2, deleteGallery.c_str(), MAXDATASIZE, 0);
            }

            /*

            ///Obtendra un request para obtener
            ///Verifica que reciba los KEYS: TEMPLATE
            if (json_object_get_string(tempZZ) != nullptr ) {
                ///JSON saliente del servidor
                string variable = sendAType(json_object_get_string(tempZZ), "KEY");
                ///Envio al cliente
                send(fd2, variable.c_str(), MAXDATASIZE, 0);
            }

            */


        }

        ///Reestablece el buffer
        memset(buff, 0, MAXDATASIZE);

        cout <<
             "\n\n--------------------------------------------------------------------------------"
             "END--------------------------------------------------------------------------------\n"
             << endl;

    }

    close(fd2);

}


/**
 *
 * @return
 */
int main(){

    ///Instancia del controlador de la sintaxis de SQL.
    serverController = new ServerController();

    ///Corre el servidor
    runServer();

    return 0;
}

