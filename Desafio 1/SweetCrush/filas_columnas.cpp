#include "filas_columnas.h"
#include "bits.h"
#include "tablero.h"
#include "constantes.h"

void desplazarCeldasIzquierda(unsigned char *tablero, int indiceInicio, int cantidadCeldas, int totalPosiciones) {
    for (int i = indiceInicio; i < totalPosiciones; i++) {
        unsigned char valor = leerCelda(tablero, i);
        escribirCelda(tablero, i - cantidadCeldas, valor);
    }
}

void desplazarCeldasDerecha(unsigned char *tablero, int indiceInicio, int cantidadCeldas, int totalPosiciones) {
    for (int i = totalPosiciones - 1; i >= indiceInicio; i--) {
        unsigned char valor = leerCelda(tablero, i);
        escribirCelda(tablero, i + cantidadCeldas, valor);
    }
}

void agregarFila(unsigned char *&tablero, int &filas, int columnas, int filaDestino) {
    int filasNuevas = filas + 1;
    int totalViejas = filas * columnas;
    int bytesNuevos = bytesNecesarios(filasNuevas, columnas);

    unsigned char *nuevo = new unsigned char[bytesNuevos];
    for (int i = 0; i < bytesNuevos; i++) {
        nuevo[i] = 0;
    }

    // 1. Copiamos todo el contenido viejo a las MISMAS posiciones dentro del buffer
    //    nuevo (que ya tiene espacio de sobra al final para la fila que vamos a insertar)
    for (int i = 0; i < totalViejas; i++) {
        unsigned char valor = leerCelda(tablero, i);
        escribirCelda(nuevo, i, valor);
    }

    // 2. Corremos hacia la derecha, dentro del buffer nuevo, todo lo que está desde
    //    'filaDestino' en adelante, para abrir el hueco donde va la fila nueva
    int indiceInicio = filaDestino * columnas;
    if (indiceInicio < totalViejas) {
        desplazarCeldasDerecha(nuevo, indiceInicio, columnas, totalViejas);
    }

    // 3. El hueco que quedó libre es la fila nueva: la llenamos de VACIO
    for (int c = 0; c < columnas; c++) {
        escribirCelda(nuevo, indiceInicio + c, VACIO);
    }

    delete[] tablero;
    tablero = nuevo;
    filas = filasNuevas;
}

void eliminarFila(unsigned char *&tablero, int &filas, int columnas, int filaOrigen) {
    int filasNuevas = filas - 1;
    int totalViejas = filas * columnas;

    // 1. Compactamos DENTRO del buffer viejo: todo lo que está después de la fila
    //    eliminada se corre 'columnas' posiciones hacia la izquierda, tapando el hueco
    int indiceInicio = (filaOrigen + 1) * columnas;
    if (indiceInicio < totalViejas) {
        desplazarCeldasIzquierda(tablero, indiceInicio, columnas, totalViejas);
    }

    // 2. Ahora reservamos el buffer definitivo, ya del tamaño reducido, y copiamos
    //    solo la parte compactada que sigue siendo válida
    int totalNuevas = filasNuevas * columnas;
    int bytesNuevos = bytesNecesarios(filasNuevas, columnas);
    unsigned char *nuevo = new unsigned char[bytesNuevos];
    for (int i = 0; i < bytesNuevos; i++) {
        nuevo[i] = 0;
    }

    for (int i = 0; i < totalNuevas; i++) {
        unsigned char valor = leerCelda(tablero, i);
        escribirCelda(nuevo, i, valor);
    }

    delete[] tablero;
    tablero = nuevo;
    filas = filasNuevas;
}

void agregarColumna(unsigned char *&tablero, int filas, int &columnas, int columnaDestino) {
    int columnasNuevas = columnas + 1;
    int totalNuevas = filas * columnasNuevas;
    int bytesNuevos = bytesNecesarios(filas, columnasNuevas);

    unsigned char *nuevo = new unsigned char[bytesNuevos];
    for (int i = 0; i < bytesNuevos; i++) {
        nuevo[i] = 0;
    }
    for (int i = 0; i < totalNuevas; i++) {
        escribirCelda(nuevo, i, VACIO);
    }

    // A diferencia de las filas, una columna NO es un bloque contiguo de índices:
    // aparece una vez por cada fila, así que hay que recorrer fila por fila
    for (int f = 0; f < filas; f++) {
        // celdas antes de la columna nueva: mismo índice de columna
        for (int c = 0; c < columnaDestino; c++) {
            int indiceViejo = f * columnas + c;
            int indiceNuevo = f * columnasNuevas + c;
            escribirCelda(nuevo, indiceNuevo, leerCelda(tablero, indiceViejo));
        }
        // la columna nueva (columnaDestino) ya quedó en VACIO
        // celdas después de la columna nueva: se corren una posición a la derecha
        for (int c = columnaDestino; c < columnas; c++) {
            int indiceViejo = f * columnas + c;
            int indiceNuevo = f * columnasNuevas + (c + 1);
            escribirCelda(nuevo, indiceNuevo, leerCelda(tablero, indiceViejo));
        }
    }

    delete[] tablero;
    tablero = nuevo;
    columnas = columnasNuevas;
}

void eliminarColumna(unsigned char *&tablero, int filas, int &columnas, int columnaOrigen) {
    int columnasNuevas = columnas - 1;
    int bytesNuevos = bytesNecesarios(filas, columnasNuevas);

    // NOTA / PENDIENTE: misma observación que en eliminarFila respecto al umbral del 65%.
    unsigned char *nuevo = new unsigned char[bytesNuevos];
    for (int i = 0; i < bytesNuevos; i++) {
        nuevo[i] = 0;
    }

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnaOrigen; c++) {
            int indiceViejo = f * columnas + c;
            int indiceNuevo = f * columnasNuevas + c;
            escribirCelda(nuevo, indiceNuevo, leerCelda(tablero, indiceViejo));
        }
        for (int c = columnaOrigen + 1; c < columnas; c++) {
            int indiceViejo = f * columnas + c;
            int indiceNuevo = f * columnasNuevas + (c - 1);
            escribirCelda(nuevo, indiceNuevo, leerCelda(tablero, indiceViejo));
        }
    }

    delete[] tablero;
    tablero = nuevo;
    columnas = columnasNuevas;
}
