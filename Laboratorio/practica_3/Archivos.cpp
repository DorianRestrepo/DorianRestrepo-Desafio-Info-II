#include "archivos.h"
#include <fstream>
#include <stdexcept>

using namespace std;


unsigned char* leerArchivo(const char* ruta, int& len) {
    ifstream f(ruta, ios::binary);
    if (!f) {
        throw runtime_error("No se pudo abrir el archivo de entrada");
    }

    f.seekg(0, ios::end);
    int tam = (int)f.tellg();
    f.seekg(0, ios::beg);

    if (tam < 0) {
        throw runtime_error("No se pudo determinar el tamano del archivo");
    }
    if (tam == 0) {
        throw runtime_error("El archivo esta vacio");
    }

    len = tam;
    unsigned char* buf = new unsigned char[len];
    f.read((char*)buf, len);
    if (!f) {
        delete[] buf;
        throw runtime_error("Error leyendo el archivo");
    }
    return buf;
}


void escribirArchivo(const char* ruta, const unsigned char* datos, int len) {
    ofstream f(ruta, ios::binary);
    if (!f) {
        throw runtime_error("No se pudo crear el archivo de salida");
    }
    f.write((const char*)datos, len);
    if (!f) {
        throw runtime_error("Error escribiendo el archivo de salida");
    }
}

bool buffersIguales(const unsigned char* a, int la, const unsigned char* b, int lb) {
    if (la != lb) {
        return false;
    }
    for (int i = 0; i < la; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}
