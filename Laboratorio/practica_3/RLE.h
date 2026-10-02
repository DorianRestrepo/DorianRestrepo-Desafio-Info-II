
#ifndef RLE_H
#define RLE_H

#include <string>

using namespace std;

string rleComprimir(const string& s);      // Recibe un texto y devuelve el texto comprimido
string rleDescomprimir(const string& s);   // Recibe un texto comprimido y devuelve el original

unsigned char* rleComprimirBytes(const unsigned char* datos, int len, int& lenOut);
unsigned char* rleDescomprimirBytes(const unsigned char* datos, int len, int& lenOut);

#endif
