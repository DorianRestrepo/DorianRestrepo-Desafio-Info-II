#ifndef CONFIGURACION_H
#define CONFIGURACION_H

// Librerias Necesarias
#include <string>   // Libreria para el Manejo de Cadenas de Texto

using namespace std;

// Rutas del Almacenamiento Permanente (Archivos de Texto Plano)
const string RUTA_DATOS_DEFECTO  = "datos/";            // Carpeta por Defecto Donde se Encuentran los Archivos
const string ARCHIVO_USUARIOS    = "usuarios.txt";      // Archivo con los Datos Personales de Cada Usuario
const string ARCHIVO_AMISTADES   = "amistades.txt";     // Archivo con las Parejas de Amigos (Una Vez por Pareja)
const string ARCHIVO_SOLICITUDES = "solicitudes.txt";   // Archivo con las Solicitudes de Amistad Pendientes
const string ARCHIVO_MENSAJES    = "mensajes.txt";      // Archivo con Todos los Mensajes Enviados
const string ARCHIVO_ALBUMES     = "albumes.txt";       // Archivo con los Albumes y las Fotos de Cada Uno

// Ruta Externa Donde Viven las Fotos (Notacion Linux)
const string RUTA_BASE_FOTOS = "/users/udeastorage/";   // Equivale a /users/(sub-ruta-X-fija)/ de la Guia

// Limites y Valores Fijos del Sistema
const int LIMITE_MENSAJE       = 512;   // Maximo de Caracteres Permitidos en un Mensaje
const int MIN_SUGERENCIAS      = 1;     // Valor Minimo de K para las Sugerencias de Amistad
const int MAX_SUGERENCIAS      = 20;    // Valor Maximo de K para las Sugerencias de Amistad
const int FOTOS_MEJORES_AMIGOS = 3;     // Cantidad de Fotos de Perfil que se Muestran al Ingresar

// Pesos de Prioridad para Elegir las Fotos de los Mejores Amigos
const double PESO_SIMPLE = 1.0;   // Amigo con 10 o Menos Amigos en Comun
const double PESO_MEDIO  = 2.5;   // Amigo con Entre 11 y 20 Amigos en Comun
const double PESO_ALTO   = 3.5;   // Amigo con 21 o Mas Amigos en Comun

// Codigos de Privacidad de los Albumes (Se Guarda un Solo Caracter en Memoria)
const char PRIVACIDAD_PUBLICO = 'P';   // Lo Puede Ver Cualquier Usuario de la Plataforma
const char PRIVACIDAD_AMIGOS  = 'A';   // Solo lo Pueden Ver los Amigos del Dueno
const char PRIVACIDAD_PRIVADO = 'R';   // Solo lo Puede Ver el Dueno

#endif // CONFIGURACION_H
