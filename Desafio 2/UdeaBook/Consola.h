#ifndef CONSOLA_H
#define CONSOLA_H


#include <string>
#include <iostream>

using namespace std;

class Consola {

private:

    static bool Entrada_Cerrada;

public:

    static const int ANCHO = 64;


    static void mostrarPortada();
    static void linea(char Caracter);
    static void titulo(const string& Texto);
    static void centrado(const string& Texto);
    static void exito(const string& Texto);
    static void error(const string& Texto);
    static void aviso(const string& Texto);

    // Funciones de Texto
    static string ajustar(const string& Texto, int Ancho);
    static string recortar(const string& Texto);

    // Funciones de Lectura S
    static string leerLinea(const string& Mensaje);
    static int    leerEntero(const string& Mensaje, int Minimo, int Maximo);
    static char   leerOpcion(const string& Mensaje);
    static void   pausar();
    static bool   entradaCerrada();
};

#endif // CONSOLA_H
