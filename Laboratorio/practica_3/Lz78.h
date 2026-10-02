#ifndef LZ78_H
#define LZ78_H


void lz78Comprimir(const unsigned char* txt, int len, int*& prefijos, unsigned char*& caracteres, int& n, bool& ultimaSinChar);

// Reconstruye el texto original a partir de los arreglos paralelos. Deja el tamano en 'lenOut'.
unsigned char* lz78Descomprimir(const int* prefijos, const unsigned char* caracteres, int n, bool ultimaSinChar, int& lenOut);

// Convierte los pares a bytes (para poder encriptarlos y guardarlos en un archivo).
unsigned char* lz78Serializar(const int* prefijos, const unsigned char* caracteres, int n, bool ultimaSinChar, int& lenOut);

// Operacion inversa: convierte bytes de vuelta a dos arreglos paralelos.
void lz78Deserializar(const unsigned char* datos, int len, int*& prefijos, unsigned char*& caracteres, int& n, bool& ultimaSinChar);

#endif
