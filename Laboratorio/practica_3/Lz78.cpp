#include "Lz78.h"
#include <stdexcept>

using namespace std;

const int LIMITE = 1000000000;

static int* ampliarInt(int* viejo, int usados, int nuevaCap) {
    int* nuevo = new int[nuevaCap];
    for (int k = 0; k < usados; k++) {
        nuevo[k] = viejo[k];
    }
    delete[] viejo;
    return nuevo;
}


static unsigned char* ampliarChar(unsigned char* viejo, int usados, int nuevaCap) {
    unsigned char* nuevo = new unsigned char[nuevaCap];
    for (int k = 0; k < usados; k++) {
        nuevo[k] = viejo[k];
    }
    delete[] viejo;
    return nuevo;
}

void lz78Comprimir(const unsigned char* txt, int len, int*& prefijos, unsigned char*& caracteres, int& n, bool& ultimaSinChar) {
    if (len <= 0 || txt == nullptr) {
        throw invalid_argument("Entrada invalida para LZ78");
    }

    int cap = 16;
    int cuenta = 0;
    int* pre = new int[cap];
    unsigned char* car = new unsigned char[cap];
    int actual = 0;

    for (int i = 0; i < len; i++) {
        int encontrado = 0;

        for (int k = 0; k < cuenta; k++) {

            if (pre[k] == actual && car[k] == txt[i]) {
                encontrado = k + 1;
                break;
            }
        }

        if (encontrado != 0) {
            actual = encontrado;
        } else {
            if (cuenta == cap) {
                pre = ampliarInt(pre, cuenta, cap * 2);
                car = ampliarChar(car, cuenta, cap * 2);
                cap = cap * 2;
            }
            pre[cuenta] = actual;
            car[cuenta] = txt[i];
            cuenta++;
            actual = 0;
        }
    }

    bool pendiente = false;
    if (actual != 0) {
        if (cuenta == cap) {
            pre = ampliarInt(pre, cuenta, cap + 1);
            car = ampliarChar(car, cuenta, cap + 1);
            cap = cap + 1;
        }
        pre[cuenta] = actual;
        car[cuenta] = 0;
        cuenta++;
        pendiente = true;
    }

    prefijos = pre;
    caracteres = car;
    n = cuenta;
    ultimaSinChar = pendiente;
}


unsigned char* lz78Descomprimir(const int* prefijos, const unsigned char* caracteres,
                                int n, bool ultimaSinChar, int& lenOut) {
    if (n <= 0 || prefijos == nullptr || caracteres == nullptr) {
        throw invalid_argument("Pares invalidos");
    }

    int* largo = new int[n + 1];
    largo[0] = 0;
    int total = 0;

    for (int k = 1; k <= n; k++) {
        int p = prefijos[k - 1];

        if (p < 0 || p >= k) {
            delete[] largo;
            throw std::runtime_error("Datos LZ78 invalidos: prefijo fuera de rango");
        }

        bool sinChar = (ultimaSinChar && k == n);

        if (sinChar && p == 0) {
            delete[] largo;
            throw runtime_error("Datos LZ78 invalidos: frase vacia");
        }

        largo[k] = largo[p] + 1;

        if (sinChar) {
            total = total + largo[p];
        } else {
            total = total + largo[k];
        }

        if (total > LIMITE) {
            delete[] largo;
            throw runtime_error("Resultado demasiado grande");
        }
    }

    lenOut = total;
    unsigned char* out = new unsigned char[total];


    int pos = 0;
    for (int k = 1; k <= n; k++) {
        bool sinChar = (ultimaSinChar && k == n);
        int L;
        int idx = prefijos[k - 1];

        if (sinChar) {
            L = largo[idx];
        } else {
            L = largo[k];
        }

        int escribir = pos + L - 1;

        if (!sinChar) {
            out[escribir] = caracteres[k - 1];
            escribir--;
        }

        while (idx != 0) {
            out[escribir] = caracteres[idx - 1];
            escribir--;
            idx = prefijos[idx - 1];
        }

        pos = pos + L;
    }

    delete[] largo;
    return out;
}

// lz78Serializar: convierte los arreglos paralelos en una secuencia de bytes
unsigned char* lz78Serializar(const int* prefijos, const unsigned char* caracteres, int n, bool ultimaSinChar, int& lenOut) {
    if (n <= 0 || n > 400000000) {
        throw invalid_argument("Cantidad de pares invalida");
    }

    lenOut = 1 + 5 * n;
    unsigned char* buf = new unsigned char[lenOut];
    if (ultimaSinChar) {
        buf[0] = 1;
    } else {
        buf[0] = 0;
    }

    for (int k = 0; k < n; k++) {
        int p = 1 + 5 * k;
        unsigned int v = prefijos[k];

        buf[p]     = (v >> 24) & 0xFF;
        buf[p + 1] = (v >> 16) & 0xFF;
        buf[p + 2] = (v >> 8) & 0xFF;
        buf[p + 3] = v & 0xFF;
        buf[p + 4] = caracteres[k];
    }
    return buf;
}

// lz78Deserializar: hace lo contrario de lz78Serializar
void lz78Deserializar(const unsigned char* datos, int len, int*& prefijos, unsigned char*& caracteres, int& n, bool& ultimaSinChar) {
    if (datos == nullptr || len < 6 || (len - 1) % 5 != 0) {
        throw runtime_error("Datos LZ78 invalidos: formato incorrecto");
    }
    if (datos[0] > 1) {
        throw runtime_error("Datos LZ78 invalidos: bandera incorrecta");
    }

    int cuenta = (len - 1) / 5;
    int* pre = new int[cuenta];
    unsigned char* car = new unsigned char[cuenta];

    for (int k = 0; k < cuenta; k++) {
        int p = 1 + 5 * k;

        unsigned int v = ((unsigned int)datos[p] << 24) |((unsigned int)datos[p + 1] << 16) |((unsigned int)datos[p + 2] << 8) | (unsigned int)datos[p + 3];

        if (v > 2000000000u) {
            delete[] pre;
            delete[] car;
            throw runtime_error("Datos LZ78 invalidos: indice demasiado grande");
        }
        pre[k] = (int)v;
        car[k] = datos[p + 4];
    }

    prefijos = pre;
    caracteres = car;
    n = cuenta;
    ultimaSinChar = (datos[0] == 1);
}
