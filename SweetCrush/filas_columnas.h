#ifndef FILAS_COLUMNAS_H
#define FILAS_COLUMNAS_H

// Inserta una fila vacía (todas sus celdas en VACIO) en la posición 'filaDestino' (0..filas).
// 'tablero' y 'filas' se pasan por referencia porque el buffer se reubica y las dimensiones cambian.
void agregarFila(unsigned char *&tablero, int &filas, int columnas, int filaDestino);

// Elimina la fila 'filaOrigen' (0..filas-1) y reconstruye el tablero sin ella.
void eliminarFila(unsigned char *&tablero, int &filas, int columnas, int filaOrigen);

// Inserta una columna vacía en la posición 'columnaDestino' (0..columnas).
void agregarColumna(unsigned char *&tablero, int filas, int &columnas, int columnaDestino);

// Elimina la columna 'columnaOrigen' (0..columnas-1) y reconstruye el tablero sin ella.
void eliminarColumna(unsigned char *&tablero, int filas, int &columnas, int columnaOrigen);

// Desplaza hacia la IZQUIERDA, dentro del MISMO buffer, todas las celdas desde 'indiceInicio'
// hasta 'totalPosiciones'-1, moviendo cada una 'cantidadCeldas' posiciones atrás.
void desplazarCeldasIzquierda(unsigned char *tablero, int indiceInicio, int cantidadCeldas, int totalPosiciones);

// Desplaza hacia la DERECHA, dentro del MISMO buffer, todas las celdas válidas desde
// 'indiceInicio' hasta 'totalPosiciones'-1, moviendo cada una 'cantidadCeldas' posiciones adelante.
void desplazarCeldasDerecha(unsigned char *tablero, int indiceInicio, int cantidadCeldas, int totalPosiciones);

#endif // FILAS_COLUMNAS_H
