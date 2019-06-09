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

void funcionInsert(string comando)
{
    string subs = comando.substr(0,5);
    if (subs.compare("INTO ") == 0 ||subs.compare("into ") == 0){
        subs = comando.substr(5);
        size_t space = subs.find(" ");
        string galery = subs.substr(0, space+1);
        cout << galery << endl;
    }
}

void funcionSelect(string comando)
{
    cout << comando << endl;

}

void funcionUpdate(string comando)
{
    cout << comando << endl;
}

void funcionDelete(string comando)
{
    cout << comando << endl;
}

int sendJSON(string KEY, string data){
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
        client.sin_addr.s_addr = inet_addr("10.0.2.15"); //192.168.100.6
        memset(client.sin_zero, '\0', sizeof(client.sin_zero));
    }

    if (::connect(fd, (const struct sockaddr *)&client, sizeof(client)) < 0)
    {
        printf("ERROR connecting to server\n");
        return 1;
    }


    json_object *jobj = json_object_new_object();

    json_object *jstring = json_object_new_string(data.c_str());

    json_object_object_add(jobj,KEY.c_str(), jstring);


    if (strcpy(sendBuff, json_object_to_json_string(jobj)) == NULL) {
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
/*
    struct json_object *tempGallery;
    json_object *parsed_jsonZoneSize = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonZoneSize, "", &tempGallery);
    if (json_object_get_int(tempGallery) != 0){
        zoneSize = json_object_get_int(tempGallery);
    }



    struct json_object *xActual;
    json_object *parsed_jsonxActual = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonxActual, "XCOORD", &xActual);
    if (json_object_get_int(xActual) != 0 ){
        xNodeActual = json_object_get_int(xActual);
    }

    struct json_object *yActual;
    json_object *parsed_jsonyActual = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonyActual, "YCOORD", &yActual);
    if (json_object_get_int(yActual) != 0){
        yNodeActual = json_object_get_int(yActual);
    }

    struct json_object *xTorre;
    json_object *parsed_jsonxTorre = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonxTorre, "XCOORDTORRE", &xTorre);
    if (json_object_get_int(xTorre) != 0 ){
        xTorreActual = json_object_get_int(xTorre);
    }

    struct json_object *yTorre;
    json_object *parsed_jsonyTorre = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonyTorre, "YCOORDTORRE", &yTorre);
    if (json_object_get_int(yTorre) != 0){
        yTorreActual = json_object_get_int(yTorre);
    }

    //Gladiador 1
    struct json_object *xG1;
    json_object *parsed_jsonxG1 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonxG1, "XCOORDGP1", &xG1);
    if (json_object_get_int(xG1) != 0){
        xActG1 = json_object_get_int(xG1);
    }

    struct json_object *yG1;
    json_object *parsed_jsonyG1 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonyG1, "YCOORDGP1", &yG1);
    if (json_object_get_int(yG1) != 0){
        yActG1 = json_object_get_int(yG1);
    }

    struct json_object *ResistenciaG1;
    json_object *parsed_jsonResistenciaG1 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonResistenciaG1, "RESISTENCIAG1", &ResistenciaG1);
    if (json_object_get_string(ResistenciaG1) != 0) {
        resistenciaG1 = stoi(json_object_get_string(ResistenciaG1));
    }

    struct json_object *HITG1;
    json_object *parsed_jsonHITG1 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonHITG1, "HITG1", &HITG1);
    if (json_object_get_int(HITG1) >= 0){
        hitG1 = (json_object_get_int(HITG1));
    }

    struct json_object *ATYPEG1;
    json_object *parsed_jsonATYPEG1 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonATYPEG1, "ATYPEG1", &ATYPEG1);
    if (json_object_get_int(ATYPEG1) != 0){
        aTypeG1 = (json_object_get_int(ATYPEG1));
    }

    struct json_object *XCOORDFLECHAG1;
    json_object *parsed_jsonXCOORDFLECHAG1 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonXCOORDFLECHAG1, "XCOORDFLECHAG1", &XCOORDFLECHAG1);
    if (json_object_get_int(XCOORDFLECHAG1) != 0){
        xCoordG1 = json_object_get_int(XCOORDFLECHAG1);
    }

    struct json_object *YCOORDFLECHAG1;
    json_object *parsed_jsonYCOORDFLECHAG1 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonYCOORDFLECHAG1, "YCOORDFLECHAG1", &YCOORDFLECHAG1);
    if (json_object_get_int(YCOORDFLECHAG1) != 0){
        yCoordG1 = json_object_get_int(YCOORDFLECHAG1);
    }


    //Gladiador 2
    struct json_object *xG2;
    json_object *parsed_jsonxG2 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonxG2, "XCOORDGP2", &xG2);
    if (json_object_get_int(xG2) != 0){
        xActG2 = json_object_get_int(xG2);
    }

    struct json_object *yG2;
    json_object *parsed_jsonyG2 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonyG2, "YCOORDGP2", &yG2);
    if (json_object_get_int(yG2) != 0){
        yActG2 = json_object_get_int(yG2);
    }

    struct json_object *ResistenciaG2;
    json_object *parsed_jsonResistenciaG2 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonResistenciaG2, "RESISTENCIAG2", &ResistenciaG2);
    if (json_object_get_string(ResistenciaG2) != 0) {
        resistenciaG2 = stoi(json_object_get_string(ResistenciaG2));
    }

    struct json_object *HITG2;
    json_object *parsed_jsonHITG2 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonHITG2, "HITG2", &HITG2);
    if (json_object_get_int(HITG2) >= 0){
        hitG2 = json_object_get_int(HITG2);
    }

    struct json_object *ATYPEG2;
    json_object *parsed_jsonATYPEG2 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonATYPEG2, "ATYPEG2", &ATYPEG2);
    if (json_object_get_int(ATYPEG2) != 0){
        aTypeG2 = json_object_get_int(ATYPEG2);
    }

    struct json_object *XCOORDFLECHAG2;
    json_object *parsed_jsonXCOORDFLECHAG2 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonXCOORDFLECHAG2, "XCOORDFLECHAG2", &XCOORDFLECHAG2);
    if (json_object_get_int(XCOORDFLECHAG2) != 0){
        xCoordG2 = json_object_get_int(XCOORDFLECHAG2);
    }

    struct json_object *YCOORDFLECHAG2;
    json_object *parsed_jsonYCOORDFLECHAG2 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonYCOORDFLECHAG2, "YCOORDFLECHAG2", &YCOORDFLECHAG2);
    if (json_object_get_int(YCOORDFLECHAG2) != 0){
        yCoordG2 = json_object_get_int(YCOORDFLECHAG2);
    }


    ///JSON de los labels en tiempo real del gladiador

    ///Gladiador 1 Datos
    struct json_object *NOMBREG1;
    json_object *parsed_jsonNOMBREG1 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonNOMBREG1, "NOMBREG1", &NOMBREG1);
    if (json_object_get_string(NOMBREG1) != 0) {
        QString qstr = QString::fromStdString(json_object_get_string(NOMBREG1));
        GenG1->modificarLabel(0, qstr);
    }

    struct json_object *EDADG1;
    json_object *parsed_jsonEDADG1 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonEDADG1, "EDADG1", &EDADG1);
    if (json_object_get_string(EDADG1) != 0) {
        QString qstr = QString::fromStdString(json_object_get_string(EDADG1));
        GenG1->modificarLabel(1, qstr);
    }

    struct json_object *RESISTENCIAG1;
    json_object *parsed_jsonRESISTENCIAG1 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonRESISTENCIAG1, "RESISTENCIAG1", &RESISTENCIAG1);
    if (json_object_get_string(RESISTENCIAG1) != 0) {
        QString qstr = QString::fromStdString(json_object_get_string(RESISTENCIAG1));
        GenG1->modificarLabel(2, qstr);
    }

    struct json_object *PROBABILIDADG1;
    json_object *parsed_jsonPROBABILIDADG1 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonPROBABILIDADG1, "PROBABILIDADG1", &PROBABILIDADG1);
    if (json_object_get_string(PROBABILIDADG1) != 0) {
        QString qstr = QString::fromStdString(json_object_get_string(PROBABILIDADG1));
        GenG1->modificarLabel(3, qstr);
    }

    struct json_object *INTELIGENCIAG1;
    json_object *parsed_jsonINTELIGENCIAG1 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonINTELIGENCIAG1, "INTELIGENCIAG1", &INTELIGENCIAG1);
    if (json_object_get_string(INTELIGENCIAG1) != 0) {
        QString qstr = QString::fromStdString(json_object_get_string(INTELIGENCIAG1));
        GenG1->modificarLabel(4, qstr);
    }

    struct json_object *TSUPERIORG1;
    json_object *parsed_jsonTSUPERIORG1 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonTSUPERIORG1, "TSUPERIORG1", &TSUPERIORG1);
    if (json_object_get_string(TSUPERIORG1) != 0) {
        QString qstr = QString::fromStdString(json_object_get_string(TSUPERIORG1));
        GenG1->modificarLabel(5, qstr);
    }

    struct json_object *TINFERIORG1;
    json_object *parsed_jsonTINFERIORG1 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonTINFERIORG1, "TINFERIORG1", &TINFERIORG1);
    if (json_object_get_string(TSUPERIORG1) != 0) {
        QString qstr = QString::fromStdString(json_object_get_string(TINFERIORG1));
        GenG1->modificarLabel(6, qstr);
    }

    struct json_object *EXPECTATIVAG1;
    json_object *parsed_jsonEXPECTATIVAG1 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonEXPECTATIVAG1, "EXPECTATIVAG1", &EXPECTATIVAG1);
    if (json_object_get_string(EXPECTATIVAG1) != 0) {
        QString qstr = QString::fromStdString(json_object_get_string(EXPECTATIVAG1));
        GenG1->modificarLabel(7, qstr);
    }


    ///Gladiador 2 Datos
    struct json_object *NOMBREG2;
    json_object *parsed_jsonNOMBREG2 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonNOMBREG2, "NOMBREG2", &NOMBREG2);
    if (json_object_get_string(NOMBREG2) != 0) {
        QString qstr = QString::fromStdString(json_object_get_string(NOMBREG2));
        GenG2->modificarLabel(0, qstr);
    }

    struct json_object *EDADG2;
    json_object *parsed_jsonEDADG2 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonEDADG2, "EDADG2", &EDADG2);
    if (json_object_get_string(EDADG2) != 0) {
        QString qstr = QString::fromStdString(json_object_get_string(EDADG2));
        GenG2->modificarLabel(1, qstr);
    }

    struct json_object *RESISTENCIAG2;
    json_object *parsed_jsonRESISTENCIAG2 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonRESISTENCIAG2, "RESISTENCIAG2", &RESISTENCIAG2);
    if (json_object_get_string(RESISTENCIAG2) != 0) {
        QString qstr = QString::fromStdString(json_object_get_string(RESISTENCIAG2));
        GenG2->modificarLabel(2, qstr);
    }

    struct json_object *PROBABILIDADG2;
    json_object *parsed_jsonPROBABILIDADG2 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonPROBABILIDADG2, "PROBABILIDADG2", &PROBABILIDADG2);
    if (json_object_get_string(PROBABILIDADG2) != 0) {
        QString qstr = QString::fromStdString(json_object_get_string(PROBABILIDADG2));
        GenG1->modificarLabel(3, qstr);
    }

    struct json_object *INTELIGENCIAG2;
    json_object *parsed_jsonINTELIGENCIAG2 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonINTELIGENCIAG2, "INTELIGENCIAG2", &INTELIGENCIAG2);
    if (json_object_get_string(INTELIGENCIAG2) != 0) {
        QString qstr = QString::fromStdString(json_object_get_string(INTELIGENCIAG2));
        GenG2->modificarLabel(4, qstr);
    }

    struct json_object *TSUPERIORG2;
    json_object *parsed_jsonTSUPERIORG2 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonTSUPERIORG2, "TSUPERIORG2", &TSUPERIORG2);
    if (json_object_get_string(TSUPERIORG2) != 0) {
        QString qstr = QString::fromStdString(json_object_get_string(TSUPERIORG2));
        GenG2->modificarLabel(5, qstr);
    }

    struct json_object *TINFERIORG2;
    json_object *parsed_jsonTINFERIORG2 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonTINFERIORG2, "TINFERIORG2", &TINFERIORG2);
    if (json_object_get_string(TSUPERIORG2) != 0) {
        QString qstr = QString::fromStdString(json_object_get_string(TINFERIORG2));
        GenG2->modificarLabel(6, qstr);
    }

    struct json_object *EXPECTATIVAG2;
    json_object *parsed_jsonEXPECTATIVAG2 = json_tokener_parse(recvBuff);
    json_object_object_get_ex(parsed_jsonEXPECTATIVAG2, "EXPECTATIVAG2", &EXPECTATIVAG2);
    if (json_object_get_string(EXPECTATIVAG2) != 0) {
        QString qstr = QString::fromStdString(json_object_get_string(EXPECTATIVAG2));
        GenG2->modificarLabel(7, qstr);
    }
     */



    ///KEYS Para Las Poblaciones



    ///Se limpian los Buffers
    memset(recvBuff, 0, MAXDATASIZE);
    memset(sendBuff, 0, MAXDATASIZE);

    ::close(fd);

}
/*

string sendGallery(string gallery, string arrowIndex) {

    Gladiador* gladInstance;
    int hit;

    if ( gladiador == "1" ) {

        gladInstance = juego->getGladiador1();

        hit = gladInstance->isHit(pathIndexG1, stoi(arrowIndex) );

        json_object *jobjHit = json_object_new_object();

        json_object *jstringHit = json_object_new_string( to_string(hit).c_str());

        json_object_object_add(jobjHit, "HITG1", jstringHit);

        return json_object_to_json_string(jobjHit);



    } else if (gladiador == "2") {

        gladInstance = juego->getGladiador2();

        hit = gladInstance->isHit(pathIndexG2, stoi(arrowIndex) );

        json_object *jobjHit = json_object_new_object();

        json_object *jstringHit = json_object_new_string( to_string(hit).c_str());

        json_object_object_add(jobjHit, "HITG2", jstringHit);

        return json_object_to_json_string(jobjHit);

    }
    else {
        cout << "sendHit failed" << endl;
    }

}

 */

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


            ///KEY: GALLERY
            ///Obtiene las galerias
            struct json_object *tempGallery;
            cout<<"GALLERY"<<endl;
            json_object *parsed_jsonGallery = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonGallery, "GALLERY", &tempGallery);


            /*
            ///KEY: TEMPLATE
            ///Obtiene un request para
            struct json_object *tempZZ;
            cout<<"ZZ"<<endl;
            json_object *parsed_jsonZZ = json_tokener_parse(buff);
            json_object_object_get_ex(parsed_jsonZZ, "ZZ", &tempZZ);
             */

            ///JSON Writes

            /*
            ///Obtendra un request para obtener
            ///Verifica que reciba los KEYS: TEMPLATE
            if (json_object_get_string(tempZZ) != nullptr ) {
                ///JSON saliente del servidor
                string aTypeZZ = sendAType("ZZ",json_object_get_string(tempZZ));
                ///Envio al cliente
                send(fd2, aTypeZZ.c_str(), MAXDATASIZE, 0);
            }


            ///Obtendra un request para obtener la galeria
            ///Verifica que reciba los KEYS: GALLERY
            if (json_object_get_string(tempGallery) != nullptr ) {
                ///JSON saliente del servidor
                string gallery = sendGallery("GALLERY",json_object_get_string(tempGallery));
                ///Envio al cliente
                send(fd2, gallery.c_str(), MAXDATASIZE, 0);
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

