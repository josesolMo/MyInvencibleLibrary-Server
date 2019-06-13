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
        ipAddress = "192.168.100.3";
    } else if (destinatary == "MetadataDB") {
        ipAddress = "192.168.100.18";
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


    struct json_object *tempNewGalery;
    json_object *parsed_jsonNewGalery = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonNewGalery, "", &tempNewGalery);
    if (json_object_get_int(tempNewGalery) != 0){
        ///Variable por guardar o funcion por llamar
        JSONReturn = json_object_get_string(tempNewGalery);
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




int sendJSON(string KEY, string data, string destinatary){

    ///Guarda el ip que se utilizara para la comunicacion
    string ipAddress;

    if (destinatary == "RAID") {
        ipAddress = "192.168.100.3";
    } else if (destinatary == "MetadataDB") {
        ipAddress = "192.168.100.18";
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


    ///Toma la data que le entra como parametro en la funcion
    json_object *jstring = json_object_new_string(data.c_str());
    ///Toma el key que le entra como parametro en la funcion
    json_object_object_add(jObj,KEY.c_str(), jstring);


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


    struct json_object *tempNewGalery;
    json_object *parsed_jsonNewGalery = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonNewGalery, "", &tempNewGalery);
    if (json_object_get_int(tempNewGalery) != 0){
        ///Variable por guardar o funcion por llamar
        JSONReturn = json_object_get_string(tempNewGalery);
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



string sendNewGallery(string KEY, string data, string destinatary) {

    //sendJSON(key, data, destinatary);


    ///Toma la data que le entra como parametro en la funcion
    json_object *jstring = json_object_new_string(data.c_str());
    ///Toma el key que le entra como parametro en la funcion
    json_object_object_add(jObj,KEY.c_str(), jstring);
    ///Envia el JSON, llamando a la funcion sendJSON con su objeto estatico
    //sendJSON(jObj, destinatary);


    string isAvailable = "1";//JSONReturn; //Va a enviar un JSON a la base de datos preguntando por la
    //disponibilidad del nombre de la galeria

    if (isAvailable == "1") {

        bool saved = true; //Por si ocurre un error en el proceso

        ///Si no se guarda
        if (!saved) {
            cerr << "Error in checkAvailability()" << endl;
            isAvailable = false;
        }

    }

    ///Preparacion del JSON con el resultado de la disponibilidad
    ///del nombre de la imagen por agregar.

    json_object *jobjNewGallery = json_object_new_object();
    json_object *jstringNewGallery = json_object_new_string(isAvailable.c_str()); /// "1" o "0"
    json_object_object_add(jobjNewGallery,"NEWGALLERY", jstringNewGallery);
    return json_object_to_json_string(jobjNewGallery);

}





string sendGallery(string gallery, string arrowIndex) {}

string sendFile(string gallery, string arrowIndex) {}

string sendBinary(string gallery, string arrowIndex) {}


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

    printf("\nServidor 'INVINCIBLE LIBRARY' abierto.\n");

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
            ///Obtiene el nombre de la nueva galeria para verificar si puede ser guardado.
            struct json_object *tempNewGallery;
            cout<<"NEWGALLERY"<<endl;
            json_object *parsed_jsonNewGallery = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonNewGallery, "NEWGALLERY", &tempNewGallery);

            ///KEY: NEWIMAGE
            ///Obtiene el nombre de
            /*
            struct json_object *tempNewGallery;
            cout<<"NEWGALLERY"<<endl;
            json_object *parsed_jsonNewGallery = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonNewGallery, "NEWGALLERY", &tempNewGallery);
*/








            ///KEY: GALLERY
            ///Obtiene las galerias
            struct json_object *tempGallery;
            cout<<"GALLERY"<<endl;
            json_object *parsed_jsonGallery = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonGallery, "GALLERY", &tempGallery);


            ///KEY: FILENAME
            ///Obtiene un request para
            struct json_object *tempFile;
            cout<<"FILENAME"<<endl;
            json_object *parsed_jsonFile = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonFile, "FILENAME", &tempFile);


            ///KEY: BINARYSTRING
            ///Obtiene un request para
            struct json_object *tempBinary;
            cout<<"BINARYSTRING"<<endl;
            json_object *parsed_jsonBinary = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonBinary, "BINARYSTRING", &tempBinary);


            /*
            ///KEY: TEMPLATE
            ///Obtiene un request para
            struct json_object *tempZZ;
            cout<<"ZZ"<<endl;
            json_object *parsed_jsonZZ = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonZZ, "ZZ", &tempZZ);
             */





            ///JSON Writes



            ///Obtendra un request para obtener la galeria
            ///Verifica que reciba los KEYS: NEWGALLERY
            if (json_object_get_string(tempNewGallery) != nullptr ) {
                ///JSON saliente del servidor
                string newgallery = sendNewGallery("NEWGALLERY",json_object_get_string(tempNewGallery), "MetadataDB");

                cout << newgallery << endl;

                ///Envio al cliente
                send(fd2, newgallery.c_str(), MAXDATASIZE, 0);
            }




            ///Obtendra un request para obtener la galeria
            ///Verifica que reciba los KEYS: GALLERY
            if (json_object_get_string(tempGallery) != nullptr ) {
                ///JSON saliente del servidor
                string gallery = sendGallery("GALLERY",json_object_get_string(tempGallery));

                cout << gallery << endl;

                ///Envio al cliente
                send(fd2, gallery.c_str(), MAXDATASIZE, 0);
            }





            ///Obtendra un request para obtener
            ///Verifica que reciba los KEYS: TEMPLATE
            if (json_object_get_string(tempFile) != nullptr ) {
                ///JSON saliente del servidor
                string aFile = sendFile("FILENAME",json_object_get_string(tempFile));
                ///Envio al cliente
                send(fd2, aFile.c_str(), MAXDATASIZE, 0);
            }


            ///Obtendra un request para obtener
            ///Verifica que reciba los KEYS: TEMPLATE
            if (json_object_get_string(tempBinary) != nullptr ) {
                ///JSON saliente del servidor
                string aBinary = sendBinary("BINARYSTRING",json_object_get_string(tempBinary));
                ///Envio al cliente
                send(fd2, aBinary.c_str(), MAXDATASIZE, 0);
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

