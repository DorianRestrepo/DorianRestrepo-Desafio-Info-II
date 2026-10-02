#include "bits.h"

int calcularByteInicio(int indice) {
    int bitInicial = indice * 3;
    return bitInicial / 8;
}

int calcularOffset(int indice) {
    int bitInicial = indice * 3;
    return bitInicial % 8;
}

unsigned char leerCelda(const unsigned char *tablero, int indice) {
    int byteInicio = calcularByteInicio(indice);
    int offset = calcularOffset(indice);

    // Armamos una ventana de 16 bits con el byte donde inicia la ficha.
    // Si los 3 bits se salen del primer byte (offset + 3 > 8), traemos también
    // el byte siguiente para poder extraer la ficha completa de un solo golpe.
    unsigned int ventana = tablero[byteInicio];
    if (offset + 3 > 8) {
        ventana |= (unsigned int)tablero[byteInicio + 1] << 8;
    }

    ventana = ventana >> offset;
    return (unsigned char)(ventana & 0x07);
}

void escribirCelda(unsigned char *tablero, int indice, unsigned char valor) {

    int byteInicio = calcularByteInicio(indice);
    int offset = calcularOffset(indice);

    //Mueve (111) hacia la izquierda tantas posiciones como indique offset
    //marcando los bits que se van a modificar.
    unsigned int mascara = (unsigned int)0x07 << offset;   // 3 bits en su posición real

    //Mueve el valor de la ficha hacia la izquierda para que sus bits coincidan con la posición destino.
    unsigned int datoNuevo = (unsigned int)valor << offset;

    if (offset + 3 <= 8) {

        //ficha completa dentro de un solo byte
        unsigned char m = (unsigned char)mascara;
        unsigned char d = (unsigned char)datoNuevo;
        tablero[byteInicio] = (tablero[byteInicio] & ~m) | d;
    } else {

        // la ficha cruza al byte siguiente.

        unsigned int ventana = tablero[byteInicio] | ((unsigned int)tablero[byteInicio + 1] << 8);
        ventana = (ventana & ~mascara) | datoNuevo;

        tablero[byteInicio] = (unsigned char)(ventana & 0xFF);
        tablero[byteInicio + 1] = (unsigned char)((ventana >> 8) & 0xFF);
    }
}
