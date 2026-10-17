#ifndef CONSOLA_H
#define CONSOLA_H

// Librerias Necesarias
#include <string>     // Libreria para el Manejo de Cadenas de Texto
#include <iostream>   // Libreria de Entrada y Salida por Consola

using namespace std;

// Clase Estatica que Centraliza la Interaccion con la Consola
class Consola {

private:

    static bool Entrada_Cerrada;   // Indica si la Entrada Estandar se Cerro (Fin de Archivo)

public:

    static const int ANCHO = 64;   // Ancho de los Marcos Dibujados en Pantalla

    // Funciones de Dibujo
    static void mostrarPortada();                     // Muestra el Titulo de Bienvenida del Programa
    static void linea(char Caracter);                 // Dibuja una Linea Horizontal
    static void titulo(const string& Texto);          // Dibuja un Titulo Enmarcado
    static void centrado(const string& Texto);        // Escribe un Texto Centrado
    static void exito(const string& Texto);           // Mensaje de Operacion Exitosa
    static void error(const string& Texto);           // Mensaje de Error
    static void aviso(const string& Texto);           // Mensaje Informativo

    // Funciones de Texto
    static string ajustar(const string& Texto, int Ancho);   // Rellena o Recorta un Texto a un Ancho Fijo
    static string recortar(const string& Texto);             // Quita Espacios al Inicio y al Final

    // Funciones de Lectura Segura
    static string leerLinea(const string& Mensaje);                       // Lee una Linea Completa
    static int    leerEntero(const string& Mensaje, int Minimo, int Maximo);   // Lee un Entero en un Rango
    static char   leerOpcion(const string& Mensaje);                      // Lee una Opcion de una Letra
    static void   pausar();                                               // Espera a que se Presione ENTER
    static bool   entradaCerrada();                                       // Indica si ya no Hay Entrada
};

#endif // CONSOLA_H
