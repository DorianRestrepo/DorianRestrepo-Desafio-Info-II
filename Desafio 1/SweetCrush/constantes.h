#ifndef CONSTANTES_H
#define CONSTANTES_H

// Fichas del juego (usan 6 de las 8 combinaciones posibles de 3 bits)

const unsigned char FICHA_A = 0; // 000
const unsigned char FICHA_B = 1; // 001
const unsigned char FICHA_C = 2; // 010
const unsigned char FICHA_D = 3; // 011
const unsigned char FICHA_E = 4; // 100
const unsigned char FICHA_F = 5; // 101

// Estados especiales (las 2 combinaciones restantes)
const unsigned char VACIO = 6;         // 110  casilla libre
const unsigned char FUERA_TABLERO = 7; // 111  relleno cuando sobran bits

const int NUM_FICHAS_VALIDAS = 6; // FICHA_A..FICHA_F

#endif // CONSTANTES_H
