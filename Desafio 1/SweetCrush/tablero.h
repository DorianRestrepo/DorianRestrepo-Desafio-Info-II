#ifndef TABLERO_H
#define TABLERO_H

// Reserva memoria dinámica (new[]) para un tablero de 'filas' x 'columnas' posiciones
// e inicializa todas las celdas en VACIO. Retorna NULL si filas o columnas no son válidas.
unsigned char* crearTablero(int filas, int columnas);

// Libera la memoria dinámica reservada para el tablero (delete[])
void liberarTablero(unsigned char *tablero);

// Calcula la cantidad mínima de bytes necesaria para almacenar un tablero
// de 'filas' x 'columnas' posiciones (3 bits por posición, redondeado hacia arriba)
int bytesNecesarios(int filas, int columnas);

// Reserva un buffer nuevo dimensionado para (filasNuevas x columnasNuevas), copia
// bit a bit (usando leerCelda/escribirCelda) el contenido que aún es válido del
// buffer actual, rellena el resto con VACIO, libera el buffer viejo y retorna el nuevo.
// Pensada para cambios de tamaño donde las posiciones conservan su mismo índice lógico
// (por ejemplo agregar/quitar al final). Para inserciones/eliminaciones en posiciones
// intermedias, el desplazamiento de índices se resuelve en el módulo filas_columnas.
unsigned char* reconstruirBuffer(unsigned char *actual, int filasActuales, int columnasActuales,
                                  int filasNuevas, int columnasNuevas);

// Calcula qué porcentaje de la memoria reservada está realmente en uso
double porcentajeUso(int posicionesUsadas, int posicionesReservadas);

// Indica si el uso real de memoria cayó por debajo del 65% del tamaño reservado,
// condición bajo la cual el enunciado exige reducir la memoria física del tablero
bool debeReducirMemoria(int posicionesUsadas, int posicionesReservadas);

#endif // TABLERO_H
