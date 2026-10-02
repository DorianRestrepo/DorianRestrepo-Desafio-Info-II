#include "tablero.h"
#include "bits.h"
#include "constantes.h"

unsigned char* crearTablero(int filas, int columnas) {
    if (filas <= 0 || columnas <= 0) {
        return nullptr;
    }

    int totalBytes = bytesNecesarios(filas, columnas);
    unsigned char *tablero = new unsigned char[totalBytes];

    // Arrancamos el bloque físico en cero antes de escribir celda por celda,
    // así los bits "sobrantes" del último byte quedan en un estado conocido
    for (int i = 0; i < totalBytes; i++) {
        tablero[i] = 0;
    }

    int totalPosiciones = filas * columnas;
    for (int i = 0; i < totalPosiciones; i++) {
        escribirCelda(tablero, i, VACIO);
    }

    return tablero;
}

void liberarTablero(unsigned char *tablero) {
    delete[] tablero;
}

int bytesNecesarios(int filas, int columnas) {
    int totalPosiciones = filas * columnas;
    int totalBits = totalPosiciones * 3;

    int totalBytes = totalBits / 8;
    if (totalBits % 8 != 0) {
        totalBytes++; // redondeo hacia arriba sin usar ceil()
    }
    return totalBytes;
}

unsigned char* reconstruirBuffer(unsigned char *actual, int filasActuales, int columnasActuales,
                                  int filasNuevas, int columnasNuevas) {
    int bytesNuevos = bytesNecesarios(filasNuevas, columnasNuevas);
    unsigned char *nuevo = new unsigned char[bytesNuevos];

    for (int i = 0; i < bytesNuevos; i++) {
        nuevo[i] = 0;
    }

    int totalNuevas = filasNuevas * columnasNuevas;
    for (int i = 0; i < totalNuevas; i++) {
        escribirCelda(nuevo, i, VACIO);
    }

    // Copiamos, índice a índice, solo lo que existía en el buffer viejo y
    // sigue teniendo sentido en el nuevo tamaño
    int totalActuales = filasActuales * columnasActuales;
    int limite = totalActuales;
    if (totalNuevas < limite) {
        limite = totalNuevas;
    }

    for (int i = 0; i < limite; i++) {
        unsigned char valor = leerCelda(actual, i);
        escribirCelda(nuevo, i, valor);
    }

    delete[] actual;
    return nuevo;
}

double porcentajeUso(int posicionesUsadas, int posicionesReservadas) {
    if (posicionesReservadas == 0) {
        return 0.0;
    }
    return (double)posicionesUsadas / (double)posicionesReservadas * 100.0;
}

bool debeReducirMemoria(int posicionesUsadas, int posicionesReservadas) {
    if (posicionesReservadas == 0) {
        return false;
    }
    return porcentajeUso(posicionesUsadas, posicionesReservadas) < 65.0;
}
