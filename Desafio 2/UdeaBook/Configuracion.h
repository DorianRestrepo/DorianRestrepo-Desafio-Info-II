#ifndef CONFIGURACION_H
#define CONFIGURACION_H


#include <string>

using namespace std;


const string RUTA_DATOS_DEFECTO  = "datos/";
const string ARCHIVO_USUARIOS    = "usuarios.txt";
const string ARCHIVO_AMISTADES   = "amistades.txt";
const string ARCHIVO_SOLICITUDES = "solicitudes.txt";
const string ARCHIVO_MENSAJES    = "mensajes.txt";
const string ARCHIVO_ALBUMES     = "albumes.txt";


const string RUTA_BASE_FOTOS = "/users/udeastorage/";

const int LIMITE_MENSAJE       = 512;   //maximo de caracteres en un mensaje
const int MIN_SUGERENCIAS      = 1;     //valor minimo de sugerencias de amistad
const int MAX_SUGERENCIAS      = 20;    //valor Maximo de sugerencias de amistad
const int FOTOS_MEJORES_AMIGOS = 3;     //cantidad de fotos de perfil que se muestran al ingresar

//Peso de sugerencia de amistad
const double PESO_SIMPLE = 1.0;   // 10 o menos amigos en comun
const double PESO_MEDIO  = 2.5;   // Entre 11 y 20 amigos en comun
const double PESO_ALTO   = 3.5;   // 21 o mas amigos en comun

//Privacidad de los albumes
const char PRIVACIDAD_PUBLICO = 'P';   // Lo puede ver cualquier usuario de la plataforma
const char PRIVACIDAD_AMIGOS  = 'A';   // Solo lo pueden ver los amigos del dueno
const char PRIVACIDAD_PRIVADO = 'R';   // Solo lo puede ver el dueno

#endif // CONFIGURACION_H
