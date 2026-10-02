#ifndef ARCHIVOS_H
#define ARCHIVOS_H

unsigned char* leerArchivo(const char* ruta, int& len);

void escribirArchivo(const char* ruta, const unsigned char* datos, int len);

bool buffersIguales(const unsigned char* a, int la, const unsigned char* b, int lb);

#endif
