#include <cstdlib>
#include "juego.h"
#include "bits.h"
#include "constantes.h"

unsigned char fichaAleatoria() {
    return (unsigned char)(rand() % NUM_FICHAS_VALIDAS);
}

void inicializarFichas(unsigned char *tablero, int filas, int columnas) {
    int total = filas * columnas;
    for (int i = 0; i < total; i++) {
        escribirCelda(tablero, i, fichaAleatoria());
    }
}

void eliminarFicha(unsigned char *tablero, int filas, int columnas, int fila, int columna) {
    if (fila < 0 || fila >= filas || columna < 0 || columna >= columnas) {
        return; // posicion invalida, no se hace nada
    }
    int indice = fila * columnas + columna;
    escribirCelda(tablero, indice, VACIO);
}

unsigned char* crearMarcadas(int filas, int columnas) {
    int total = filas * columnas;
    unsigned char *marcadas = new unsigned char[total];
    for (int i = 0; i < total; i++) {
        marcadas[i] = 0;
    }
    return marcadas;
}

void liberarMarcadas(unsigned char *marcadas) {
    delete[] marcadas;
}

void detectarCombinacionesHorizontales(const unsigned char *tablero, int filas, int columnas, unsigned char *marcadas) {
    for (int f = 0; f < filas; f++) {
        int c = 0;
        while (c < columnas) {
            unsigned char valor = leerCelda(tablero, f * columnas + c);

            // solo nos interesan fichas validas (0..5); VACIO y FUERA_TABLERO nunca combinan
            if (valor < NUM_FICHAS_VALIDAS) {
                int inicio = c;
                while (c < columnas && leerCelda(tablero, f * columnas + c) == valor) {
                    c++;
                }
                int longitud = c - inicio;
                if (longitud >= 3) {
                    for (int k = inicio; k < c; k++) {
                        marcadas[f * columnas + k] = 1;
                    }
                }
            } else {
                c++;
            }
        }
    }
}

void detectarCombinacionesVerticales(const unsigned char *tablero, int filas, int columnas,
                                      unsigned char *marcadas) {
    for (int c = 0; c < columnas; c++) {
        int f = 0;
        while (f < filas) {
            unsigned char valor = leerCelda(tablero, f * columnas + c);

            if (valor < NUM_FICHAS_VALIDAS) {
                int inicio = f;
                while (f < filas && leerCelda(tablero, f * columnas + c) == valor) {
                    f++;
                }
                int longitud = f - inicio;
                if (longitud >= 3) {
                    for (int k = inicio; k < f; k++) {
                        marcadas[k * columnas + c] = 1;
                    }
                }
            } else {
                f++;
            }
        }
    }
}

int resolverCombinaciones(unsigned char *tablero, int filas, int columnas, const unsigned char *marcadas) {
    int total = filas * columnas;
    int eliminadas = 0;
    for (int i = 0; i < total; i++) {
        if (marcadas[i] == 1) {
            escribirCelda(tablero, i, VACIO);
            eliminadas++;
        }
    }
    return eliminadas;
}

void aplicarGravedad(unsigned char *tablero, int filas, int columnas) {
    for (int c = 0; c < columnas; c++) {
        int escribirFila = filas - 1;

        for (int leerFila = filas - 1; leerFila >= 0; leerFila--) {
            unsigned char valor = leerCelda(tablero, leerFila * columnas + c);
            if (valor != VACIO && valor != FUERA_TABLERO) {
                if (escribirFila != leerFila) {
                    escribirCelda(tablero, escribirFila * columnas + c, valor);
                }
                escribirFila--;
            }
        }


        for (int f = escribirFila; f >= 0; f--) {
            escribirCelda(tablero, f * columnas + c, VACIO);
        }
    }
}

void rellenarVacios(unsigned char *tablero, int filas, int columnas) {
    int total = filas * columnas;
    for (int i = 0; i < total; i++) {
        if (leerCelda(tablero, i) == VACIO) {
            escribirCelda(tablero, i, fichaAleatoria());
        }
    }
}

bool hayCombinacionesPendientes(const unsigned char *tablero, int filas, int columnas) {
    unsigned char *marcadas = crearMarcadas(filas, columnas);

    detectarCombinacionesHorizontales(tablero, filas, columnas, marcadas);
    detectarCombinacionesVerticales(tablero, filas, columnas, marcadas);

    bool hay = false;
    int total = filas * columnas;
    for (int i = 0; i < total && !hay; i++) {
        if (marcadas[i] == 1) {
            hay = true;
        }
    }

    liberarMarcadas(marcadas);
    return hay;
}

int procesarCascadas(unsigned char *tablero, int filas, int columnas, int &fichasEliminadasTotal) {
    int cascadas = 0;
    int total = filas * columnas;
    unsigned char *marcadas = crearMarcadas(filas, columnas);

    bool sigueHabiendo = true;
    while (sigueHabiendo) {
        for (int i = 0; i < total; i++) {
            marcadas[i] = 0;
        }

        detectarCombinacionesHorizontales(tablero, filas, columnas, marcadas);
        detectarCombinacionesVerticales(tablero, filas, columnas, marcadas);

        int eliminadasEsteCiclo = resolverCombinaciones(tablero, filas, columnas, marcadas);

        if (eliminadasEsteCiclo > 0) {
            cascadas++;
            fichasEliminadasTotal += eliminadasEsteCiclo;
            aplicarGravedad(tablero, filas, columnas);
            rellenarVacios(tablero, filas, columnas);
        } else {
            sigueHabiendo = false;
        }
    }

    liberarMarcadas(marcadas);
    return cascadas;
}

long calcularPuntuacion(int fichasEliminadas, int numeroCascada) {
    // Criterio de puntuacion (documentar igual en el informe):
    // 10 puntos por cada ficha eliminada, con un bono adicional del 50%
    // por cada cascada encadenada (numeroCascada = 0 en la eliminacion inicial).
    long base = (long)fichasEliminadas * 10L;
    long bono = (long)(base * 0.5 * numeroCascada);
    return base + bono;
}
