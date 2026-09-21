#ifndef JUEGO_H
#define JUEGO_H

// Genera un valor aleatorio uniforme entre las fichas válidas (0..NUM_FICHAS_VALIDAS-1)
unsigned char fichaAleatoria();

// Llena todas las posiciones del tablero con fichas iniciales aleatorias
void inicializarFichas(unsigned char *tablero, int filas, int columnas);

// Marca como VACIO la posición (fila, columna) elegida por el usuario.
// No hace nada si la posición está fuera de rango.
void eliminarFicha(unsigned char *tablero, int filas, int columnas, int fila, int columna);

// Reserva un arreglo dinámico de banderas (0/1), del mismo tamaño lógico que el tablero,
// para marcar qué posiciones forman parte de alguna combinación. Inicializado en 0.
unsigned char* crearMarcadas(int filas, int columnas);

// Libera el arreglo de banderas reservado con crearMarcadas
void liberarMarcadas(unsigned char *marcadas);

// Recorre cada fila y marca (con 1) las posiciones que forman parte de 3 o más
// fichas iguales consecutivas en sentido horizontal. Ignora VACIO y FUERA_TABLERO.
void detectarCombinacionesHorizontales(const unsigned char *tablero, int filas, int columnas, unsigned char *marcadas);

// Igual que la anterior, pero recorriendo cada columna en sentido vertical
void detectarCombinacionesVerticales(const unsigned char *tablero, int filas, int columnas, unsigned char *marcadas);

// Pone en VACIO todas las posiciones marcadas con 1. Retorna cuántas fichas se eliminaron.
int resolverCombinaciones(unsigned char *tablero, int filas, int columnas, const unsigned char *marcadas);

// Para cada columna, hace "caer" las fichas no vacías hacia abajo, dejando los
// espacios vacíos agrupados en la parte de arriba de esa columna
void aplicarGravedad(unsigned char *tablero, int filas, int columnas);

// Reemplaza cada posición en VACIO por una ficha aleatoria nueva
void rellenarVacios(unsigned char *tablero, int filas, int columnas);

// Indica si, en el estado actual del tablero, existe al menos una combinación
// de 3 o más fichas iguales (horizontal o vertical) sin resolver
bool hayCombinacionesPendientes(const unsigned char *tablero, int filas, int columnas);

// Ciclo completo de una cascada: detecta -> resuelve -> gravedad -> rellena -> repite
// mientras sigan apareciendo combinaciones nuevas. Acumula el total de fichas eliminadas
// en 'fichasEliminadasTotal' (pasado por referencia) y retorna cuántas cascadas se produjeron.
int procesarCascadas(unsigned char *tablero, int filas, int columnas, int &fichasEliminadasTotal);

// Calcula los puntos ganados en una cascada según la cantidad de fichas eliminadas
// y el número de cascada en curso (0 = eliminación inicial del jugador, sin bono).
// Criterio: 10 puntos por ficha, con un bono del 50% adicional por cada cascada encadenada.
long calcularPuntuacion(int fichasEliminadas, int numeroCascada);

#endif // JUEGO_H
