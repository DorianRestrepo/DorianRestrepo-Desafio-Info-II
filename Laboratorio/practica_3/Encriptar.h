#ifndef ENCRIPTAR_H
#define ENCRIPTAR_H


unsigned char rotIzq(unsigned char b, int n);

unsigned char rotDer(unsigned char b, int n);

void encriptar(unsigned char* datos, int len, int n, unsigned char K);

void desencriptar(unsigned char* datos, int len, int n, unsigned char K);


#endif // ENCRIPTAR_H
