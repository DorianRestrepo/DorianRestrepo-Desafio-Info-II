#include <iostream>
#include <ctime>
#include "tablero.h"
#include "filas_columnas.h"
#include "juego.h"
#include "vista.h"

using namespace std;


int leerEntero() {
    int valor;
    while (!(cin >> valor)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Entrada invalida, intenta de nuevo: ";
    }
    return valor;
}

int mostrarMenu() {
    cout << "\n===== SWEET CRUSH =====\n";
    cout << "1. Mostrar tablero (fichas)\n";
    cout << "2. Mostrar tablero (binario)\n";
    cout << "3. Eliminar ficha\n";
    cout << "4. Agregar fila\n";
    cout << "5. Eliminar fila\n";
    cout << "6. Agregar columna\n";
    cout << "7. Eliminar columna\n";
    cout << "8. Ver estado del juego\n";
    cout << "0. Salir\n";
    cout << "Opcion: ";
    return leerEntero();
}

void pedirCoordenadas(int filas, int columnas, int &filaElegida, int &columnaElegida) {
    bool valido = false;
    while (!valido) {
        cout << "Fila (0.." << filas - 1 << "): ";
        filaElegida = leerEntero();
        cout << "Columna (0.." << columnas - 1 << "): ";
        columnaElegida = leerEntero();

        if (filaElegida >= 0 && filaElegida < filas && columnaElegida >= 0 && columnaElegida < columnas) {
            valido = true;
        } else {
            cout << "Coordenadas fuera de rango, intenta de nuevo.\n";
        }
    }
}

// Pide un entero entre 0 y 'limiteMaximo' (inclusive), repitiendo hasta que sea valido.
int pedirPosicionEstructural(const char *etiqueta, int limiteMaximo) {
    int posicion;
    bool valido = false;
    while (!valido) {
        cout << etiqueta << " (0.." << limiteMaximo << "): ";
        posicion = leerEntero();
        if (posicion >= 0 && posicion <= limiteMaximo) {
            valido = true;
        } else {
            cout << "Posicion fuera de rango, intenta de nuevo.\n";
        }
    }
    return posicion;
}

int main() {
    srand((unsigned int)time(NULL)); // generador aleatorio

    cout << "=== Creacion del tablero ===\n";
    cout << "Numero de filas: ";
    int filas = leerEntero();
    cout << "Numero de columnas: ";
    int columnas = leerEntero();

    if (filas <= 0 || columnas <= 0 || filas > 1000 || columnas > 1000) {
        cout << "Dimensiones fuera de rango permitido.\n";
        return 1;
    }

    unsigned char *tablero = crearTablero(filas, columnas);
    if (tablero == nullptr) {
        cout << "Dimensiones invalidas, cerrando el programa.\n";
        return 1;
    }
    inicializarFichas(tablero, filas, columnas);

    // Estado del juego (variables sueltas, sin struct)
    int eliminacionesUsuario = 0;
    int fichasEliminadasTotal = 0;
    int combinacionesDetectadas = 0;
    int cascadasUltimaEliminacion = 0;
    long puntuacion = 0;

    cout << "\nTablero inicial:\n";
    mostrarTableroFichas(tablero, filas, columnas);

    bool jugando = true;
    while (jugando) {
        int opcion = mostrarMenu();

        switch (opcion) {
        case 1:
            mostrarTableroFichas(tablero, filas, columnas);
            break;

        case 2:
            mostrarTableroBinario(tablero, filas, columnas);
            break;

        case 3: {
            int fila, columna;
            pedirCoordenadas(filas, columnas, fila, columna);
            eliminarFicha(tablero, filas, columnas, fila, columna);
            eliminacionesUsuario++;


            aplicarGravedad(tablero, filas, columnas);
            rellenarVacios(tablero, filas, columnas);


            int fichasEliminadasEstaJugada = 1; // la ficha que elimino el jugador
            int cascadas = procesarCascadas(tablero, filas, columnas, fichasEliminadasEstaJugada);

            fichasEliminadasTotal += fichasEliminadasEstaJugada;
            cascadasUltimaEliminacion = cascadas;
            combinacionesDetectadas += cascadas;

            if (fichasEliminadasEstaJugada > 0) {
                puntuacion += calcularPuntuacion(fichasEliminadasEstaJugada, cascadas);
            }

            cout << "Jugada procesada: " << fichasEliminadasEstaJugada
                      << " fichas eliminadas, " << cascadas << " cascadas.\n";
            mostrarTableroFichas(tablero, filas, columnas);
            break;
        }

        case 4: {
            int filaDestino = pedirPosicionEstructural("Fila donde insertar", filas);
            agregarFila(tablero, filas, columnas, filaDestino);

            rellenarVacios(tablero, filas, columnas);
            int fichasEliminadas = 0;
            int cascadas = procesarCascadas(tablero, filas, columnas, fichasEliminadas);
            fichasEliminadasTotal += fichasEliminadas;
            combinacionesDetectadas += cascadas;

            cout << "Fila agregada. Nuevas dimensiones: " << filas << " x " << columnas << "\n";
            mostrarTableroFichas(tablero, filas, columnas);
            break;
        }

        case 5: {
            if (filas <= 1) {
                cout << "No se puede eliminar la unica fila del tablero.\n";
            } else {
                int filaOrigen = pedirPosicionEstructural("Fila a eliminar", filas - 1);
                eliminarFila(tablero, filas, columnas, filaOrigen);

                int fichasEliminadas = 0;
                int cascadas = procesarCascadas(tablero, filas, columnas, fichasEliminadas);
                fichasEliminadasTotal += fichasEliminadas;
                combinacionesDetectadas += cascadas;

                cout << "Fila eliminada. Nuevas dimensiones: " << filas << " x " << columnas << "\n";
                mostrarTableroFichas(tablero, filas, columnas);
            }
            break;
        }

        case 6: {
            int columnaDestino = pedirPosicionEstructural("Columna donde insertar", columnas);
            agregarColumna(tablero, filas, columnas, columnaDestino);

            rellenarVacios(tablero, filas, columnas);
            int fichasEliminadas = 0;
            int cascadas = procesarCascadas(tablero, filas, columnas, fichasEliminadas);
            fichasEliminadasTotal += fichasEliminadas;
            combinacionesDetectadas += cascadas;

            cout << "Columna agregada. Nuevas dimensiones: " << filas << " x " << columnas << "\n";
            mostrarTableroFichas(tablero, filas, columnas);
            break;
        }

        case 7: {
            if (columnas <= 1) {
                cout << "No se puede eliminar la unica columna del tablero.\n";
            } else {
                int columnaOrigen = pedirPosicionEstructural("Columna a eliminar", columnas - 1);
                eliminarColumna(tablero, filas, columnas, columnaOrigen);

                int fichasEliminadas = 0;
                int cascadas = procesarCascadas(tablero, filas, columnas, fichasEliminadas);
                fichasEliminadasTotal += fichasEliminadas;
                combinacionesDetectadas += cascadas;

                cout << "Columna eliminada. Nuevas dimensiones: " << filas << " x " << columnas << "\n";
                mostrarTableroFichas(tablero, filas, columnas);
            }
            break;
        }

        case 8:
            mostrarEstadoJuego(eliminacionesUsuario, fichasEliminadasTotal, combinacionesDetectadas, cascadasUltimaEliminacion, puntuacion);
            break;

        case 0:
            jugando = false;
            break;

        default:
            cout << "Opcion invalida.\n";
            break;
        }
    }

    liberarTablero(tablero);
    cout << "Hasta la proximaaaaaaaaaa\n";

    return 0;
}
