#ifndef RECURSOS_H
#define RECURSOS_H

// Librerias Necesarias
#include <string>     // Libreria para el Manejo de Cadenas de Texto
#include <iostream>   // Libreria para Mostrar el Reporte en Consola

using namespace std;

// Clase Estatica que Mide el Consumo de Recursos de Todo el Sistema
class Recursos {

private:

    // Contadores Globales del Sistema
    static long Iteraciones;        // Iteraciones Realizadas por la Funcionalidad Actual
    static long Memoria_Dinamica;   // Bytes Vivos en el Heap por Causa de Todos los Objetos y Estructuras
    static long Memoria_Local;      // Bytes de Variables Locales y Parametros por Valor de la Funcionalidad
    static long Usos_Ctime;         // Llamadas a Funciones de la Libreria Externa <ctime>
    static long Usos_Aleatorio;     // Numeros Generados con la Libreria Externa <random>
    static long Lineas_Archivo;     // Lineas Leidas o Escritas con la Libreria Externa <fstream>

    // Funciones Auxiliares de Formato
    static string formatearNumero(long Numero);   // Agrega Separadores de Miles a un Numero
    static string formatearBytes(long Bytes);     // Convierte una Cantidad de Bytes a KB o MB

public:

    // Funciones de Registro
    static void contarIteracion();                    // Suma una Iteracion al Contador
    static void reservar(long Bytes);                 // Registra Memoria Pedida al Heap (new)
    static void liberar(long Bytes);                  // Registra Memoria Devuelta al Heap (delete)
    static void sumarMemoriaLocal(long Bytes);        // Registra Variables Locales y Parametros por Valor
    static void contarUsoCtime();                     // Registra una Llamada a <ctime>
    static void contarUsoAleatorio();                 // Registra un Numero Aleatorio Generado
    static void contarLineaArchivo();                 // Registra una Linea Leida o Escrita en Disco
    static long memoriaCadena(const string& Texto);   // Bytes que un String Ocupa Fuera de su Objeto

    // Funciones de Consulta
    static long obtenerIteraciones();      // Retorna las Iteraciones Acumuladas
    static long obtenerMemoriaDinamica();  // Retorna los Bytes Vivos en el Heap
    static long obtenerMemoriaLocal();     // Retorna los Bytes Locales de la Funcionalidad
    static long obtenerMemoriaTotal();     // Retorna la Suma de Heap + Locales

    // Funciones de Control
    static void reiniciar();                                  // Reinicia los Contadores de la Funcionalidad
    static void mostrarReporte(const string& Funcionalidad);  // Imprime las Metricas en Pantalla
};

#endif // RECURSOS_H
