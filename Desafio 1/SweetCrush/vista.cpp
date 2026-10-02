#include <iostream>
#include "vista.h"
#include "bits.h"
#include "tablero.h"
#include "constantes.h"

void mostrarTableroFichas(const unsigned char *tablero, int filas, int columnas) {
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            unsigned char valor = leerCelda(tablero, f * columnas + c);
            char simbolo;

            switch (valor) {
                case FICHA_A: simbolo = 'A'; break;
                case FICHA_B: simbolo = 'B'; break;
                case FICHA_C: simbolo = 'C'; break;
                case FICHA_D: simbolo = 'D'; break;
                case FICHA_E: simbolo = 'E'; break;
                case FICHA_F: simbolo = 'F'; break;
                case VACIO:   simbolo = '.'; break;
                default:      simbolo = '#'; break; // FUERA_TABLERO
            }

            std::cout << simbolo << " ";
        }
        std::cout << "\n";
    }
}

void mostrarTableroBinario(const unsigned char *tablero, int filas, int columnas) {
    int totalBytes = bytesNecesarios(filas, columnas);

    for (int i = 0; i < totalBytes; i++) {
        std::cout << "Byte " << i << ": ";
        for (int bit = 7; bit >= 0; bit--) {
            unsigned char bitValor = (tablero[i] >> bit) & 0x01;
            std::cout << (int)bitValor; // (int) para que imprima 0/1, no un caracter
        }
        std::cout << "\n";
    }
}

void mostrarEstadoJuego(int eliminaciones, int fichasEliminadasTotal, int combinacionesDetectadas,
                         int cascadasUltimaEliminacion, long puntuacion) {
    std::cout << "\n--- Estado del juego ---\n";
    std::cout << "Eliminaciones realizadas por el usuario: " << eliminaciones << "\n";
    std::cout << "Fichas eliminadas en total: " << fichasEliminadasTotal << "\n";
    std::cout << "Combinaciones/cascadas detectadas en total: " << combinacionesDetectadas << "\n";
    std::cout << "Cascadas producidas por la ultima eliminacion: " << cascadasUltimaEliminacion << "\n";
    std::cout << "Puntuacion: " << puntuacion << "\n";
    std::cout << "-------------------------\n";
}
