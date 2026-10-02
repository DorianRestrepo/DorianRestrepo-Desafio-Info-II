#ifndef VISTA_H
#define VISTA_H

// Imprime el tablero usando un símbolo por ficha (A-F), '.' para VACIO
// y '#' para FUERA_TABLERO. Pensado para que el jugador entienda el estado del juego.
void mostrarTableroFichas(const unsigned char *tablero, int filas, int columnas);

// Imprime el tablero byte por byte en binario, útil para demostrar/depurar
// que la representación compacta de 3 bits por ficha está funcionando bien.
void mostrarTableroBinario(const unsigned char *tablero, int filas, int columnas);

// Imprime un resumen del estado actual de la partida
void mostrarEstadoJuego(int eliminaciones, int fichasEliminadasTotal, int combinacionesDetectadas,
                         int cascadasUltimaEliminacion, long puntuacion);

#endif // VISTA_H
