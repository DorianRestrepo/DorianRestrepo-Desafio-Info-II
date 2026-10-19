#ifndef RECURSOS_H
#define RECURSOS_H


#include <string>
#include <iostream>

using namespace std;

class Recursos {

private:


    static long Iteraciones;
    static long Memoria_Dinamica;
    static long Memoria_Local;
    static long Usos_Ctime;
    static long Usos_Aleatorio;
    static long Lineas_Archivo;


    static string formatearNumero(long Numero);
    static string formatearBytes(long Bytes);

public:


    static void contarIteracion();
    static void reservar(long Bytes);
    static void liberar(long Bytes);
    static void sumarMemoriaLocal(long Bytes);
    static void contarUsoCtime();
    static void contarUsoAleatorio();
    static void contarLineaArchivo();
    static long memoriaCadena(const string& Texto);


    static long obtenerIteraciones();
    static long obtenerMemoriaDinamica();
    static long obtenerMemoriaLocal();
    static long obtenerMemoriaTotal();


    static void reiniciar();
    static void mostrarReporte(const string& Funcionalidad);
};

#endif // RECURSOS_H
