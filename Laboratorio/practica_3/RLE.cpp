#include "RLE.h"
#include <stdexcept>

using namespace std;

string rleComprimir(const string& s) {
    string salida;
    int total = s.size();
    int i = 0;

    while (i < total) {
        int j = i;
        while (j < total && s[j] == s[i] && (j - i) < 255) {
            j++;
        }

        int cantidad = j - i;
        salida += (char)cantidad;
        salida += s[i];
        i = j;
    }
    return salida;
}


string rleDescomprimir(const string& s) {
    int total = s.size();

    if (total % 2 != 0) {
        throw runtime_error("Datos RLE invalidos: longitud impar");
    }

    string salida;
    for (int i = 0; i < total; i += 2) {
        int cantidad = (unsigned char)s[i];

        if (cantidad == 0) {
            throw runtime_error("Datos RLE invalidos: cantidad cero");
        }

        for (int k = 0; k < cantidad; k++) {
            salida += s[i + 1];
        }
    }
    return salida;
}


unsigned char* rleComprimirBytes(const unsigned char* datos, int len, int& lenSalida) {
    string s;
    for (int i = 0; i < len; i++) {
        s += (char)datos[i];
    }

    string r = rleComprimir(s);
    lenSalida = r.size();

    unsigned char* salida = new unsigned char[lenSalida];
    for (int i = 0; i < lenSalida; i++) {
        salida[i] = (unsigned char)r[i];
    }
    return salida;
}


unsigned char* rleDescomprimirBytes(const unsigned char* datos, int len, int& lenOut) {
    string s;                           // String temporal con el contenido del arreglo comprimido
    for (int i = 0; i < len; i++) {          // Recorre cada byte del arreglo
        s += (char)datos[i];                 // Lo agrega al string
    }

    string r = rleDescomprimir(s);      // Descomprime usando la funcion de strings (puede lanzar excepcion)
    lenOut = r.size();                       // Entrega el tamano del resultado a quien llamo

    unsigned char* salida = new unsigned char[lenOut];   // Pide memoria dinamica para el resultado
    for (int i = 0; i < lenOut; i++) {       // Recorre el string descomprimido
        salida[i] = (unsigned char)r[i];        // Copia cada caracter al arreglo
    }
    return salida;                              // Devuelve el arreglo (hay que liberarlo con delete[])
}






