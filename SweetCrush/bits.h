#ifndef BITS_H
#define BITS_H

// Calcula en qué byte del buffer inicia la ficha ubicada en la posición 'indice'
int calcularByteInicio(int indice);

// Calcula el offset (0-7) dentro del byte de inicio donde comienza la ficha
int calcularOffset(int indice);

// Lee el valor (0-7) almacenado en la posición lógica 'indice' del tablero de bits.
unsigned char leerCelda(const unsigned char *tablero, int indice);

// Escribe 'valor' (se enmascara con & 0x07) en la posición lógica 'indice' del tablero.
void escribirCelda(unsigned char *tablero, int indice, unsigned char valor);

#endif // BITS_H
