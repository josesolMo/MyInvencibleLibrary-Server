//
// Created by jose on 08/06/19.
//
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



#define PORT 3550
#define BACKLOG 4
#define MAXDATASIZE 1000

using namespace std;


static string JSONReturn;

///Creacion del JSON por enviar
json_object *jObj = json_object_new_object();


int sendJSON(json_object *jObj, string destinatary) {


    ///Guarda el ip que se utilizara para la comunicacion
    string ipAddress;

    if (destinatary == "RAID") {
        ipAddress = "192.168.100.4";
    } else if (destinatary == "MetadataDB") {
        ipAddress = "192.168.100.2";
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
        ///Variable por guardar o funcion por llamar
        JSONReturn = json_object_get_string(tempNewGallery);
        cout << JSONReturn << ": just added." << endl;
    }


    ///KEY: NEWIMAGE
    struct json_object *tempNewImage;
    json_object *parsed_jsonNewImage = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonNewImage, "NEWIMAGE", &tempNewImage);
    if (json_object_get_string(tempNewImage) != nullptr){
        ///Variable por guardar o funcion por llamar
        JSONReturn = json_object_get_string(tempNewImage);
    }




/*
    struct json_object *tempXxxxx;
    json_object *parsed_jsonXxxxx = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonXxxxx, "", &tempXxxxx);
    if (json_object_get_int(tempXxxxx) != 0){
        ///Variable por guardar o funcion por llamar
        JSONReturn = json_object_get_int(tempXxxxx);
    }
*/



    ///Se limpian los Buffers
    memset(recvBuff, 0, MAXDATASIZE);
    memset(sendBuff, 0, MAXDATASIZE);

    ::close(fd);

}



/**
 * Envia un JSON a la base de datos para verificar la disponibilidad de el nombre de la galeria entrante.
 * @param KEY
 * @param data
 * @param destinatary
 * @return string (JSON)
 */
string sendNewGallery(string newGallery, string newGalleryKey) {

    ///Nuevo para sendJSON

    ///Toma la data que le entra como parametro en la funcion
    json_object *jstringNewGallery = json_object_new_string(newGallery.c_str());
    ///Toma el key que le entra como parametro en la funcion
    json_object_object_add(jObj,newGalleryKey.c_str(), jstringNewGallery);

    sendJSON(jObj, "MetadataDB");

    ///Preparacion del JSON con el resultado de la disponibilidad
    ///del nombre de la imagen por agregar.

    if (JSONReturn == "1" || JSONReturn == "0") {
        cout << "JSONReturn: " << JSONReturn << endl;
        json_object *jobjSendNewGallery = json_object_new_object();
        json_object *jstringSendNewGallery = json_object_new_string(
                JSONReturn.c_str()); /// "1" si se agrega o "0" si no
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
 * @param KEY
 * @param data
 * @param destinatary
 * @return string (JSON)
 */
string sendNewImage(string newImage,  string newImageKey, string gallery, string galleryKey,
        string binaryData, string binaryDataKey) {

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


    if (JSONReturn == "1" && JSONReturn == "0") {

        if (JSONReturn == "1") {
            sendJSON(jObj, "RAID");
        }

        json_object *jstringSendNewImage = json_object_new_string(JSONReturn.c_str()); /// "1" o "0"
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
 * Corre el servidor
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
            cout<<"NEWGALLERY"<<endl;
            json_object *parsed_jsonNewGallery = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonNewGallery, "NEWGALLERY", &tempNewGallery);

            ///KEY: NEWIMAGE
            ///Obtiene el nombre de la nueva imagen para verificar si puede ser guardada.
            struct json_object *tempNewImage;
            cout<<"NEWIMAGE"<<endl;
            json_object *parsed_jsonNewImage = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonNewImage, "NEWIMAGE", &tempNewImage);

            ///KEY: GALLERY
            ///Obtiene la galeria donde se cambiara o se retornara su contenido
            struct json_object *tempGallery;
            cout<<"GALLERY"<<endl;
            json_object *parsed_jsonGallery = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonGallery, "GALLERY", &tempGallery);

            ///KEY: BINARYDATA
            ///Obtiene un request para dejar el binaryData de la imagen por
            struct json_object *tempBinaryData;
            cout<<"BINARYDATA"<<endl;
            json_object *parsed_jsonBinaryData = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonBinaryData, "BINARYDATA", &tempBinaryData);



            /*
            ///KEY: TEMPLATE
            ///Obtiene un request para
            struct json_object *tempZZ;
            cout<<"ZZ"<<endl;
            json_object *parsed_jsonZZ = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonZZ, "ZZ", &tempZZ);
             */





            ///JSON Writes



            ///Obtendra un request para obtener la disponibilidad de la galeria.
            ///Verifica que reciba los KEYS: NEWGALLERY
            if (json_object_get_string(tempNewGallery) != nullptr ) {
                ///JSON saliente del servidor
                string newGallery = sendNewGallery(json_object_get_string(tempNewGallery), "NEWGALLERY");
                cout << "newGallery: " << newGallery << endl;

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











            /*
            ///Obtendra un request para obtener
            ///Verifica que reciba los KEYS: TEMPLATE
            if (json_object_get_string(tempZZ) != nullptr ) {
                ///JSON saliente del servidor
                string aTypeZZ = sendAType("ZZ",json_object_get_string(tempZZ));
                ///Envio al cliente
                send(fd2, aTypeZZ.c_str(), MAXDATASIZE, 0);
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






int main(){

    runServer();

    return 0;
}

