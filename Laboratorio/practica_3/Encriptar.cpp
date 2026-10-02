#include "Encriptar.h"
#include <stdexcept>

using namespace std;

unsigned char rotIzq(unsigned char b, int n) {
    unsigned char izquierda = b << n;
    unsigned char derecha = b >> (8 - n);
    return izquierda | derecha;
}

unsigned char rotDer(unsigned char b, int n) {
    unsigned char derecha = b >> n;
    unsigned char izquierda = b << (8 - n);
    return derecha | izquierda;
}

void encriptar(unsigned char* datos, int len, int n, unsigned char K) {
    if (n <= 0 || n >= 8) {
        throw invalid_argument("n debe estar entre 1 y 7");
    }
    if (datos == nullptr || len < 0) {
        throw invalid_argument("Datos invalidos");
    }
    for (int i = 0; i < len; i++) {
        unsigned char rotado = rotIzq(datos[i], n);
        datos[i] = rotado ^ K;
    }
}

void desencriptar(unsigned char* datos, int len, int n, unsigned char K) {
    if (n <= 0 || n >= 8) {
        throw invalid_argument("n debe estar entre 1 y 7");
    }
    if (datos == nullptr || len < 0) {
        throw invalid_argument("Datos invalidos");
    }
    for (int i = 0; i < len; i++) {
        unsigned char sinXor = datos[i] ^ K;
        datos[i] = rotDer(sinXor, n);
    }
}
