#include <iostream>
#include <stdexcept>
#include "archivos.h"
#include "RLE.h"
#include "Lz78.h"
#include "Encriptar.h"

using namespace std;


static int leerEntero(const char* mensaje, int min, int max) {
    int v;
    cout << mensaje;
    cin >> v;
    if (cin.fail()) {
        throw runtime_error("Debes escribir un numero");
    }
    if (v < min || v > max) {
        throw invalid_argument("El valor esta fuera del rango permitido");
    }
    return v;
}


static void leerRuta(const char* mensaje, char* destino, int tam) {
    cout << mensaje;
    cin.getline(destino, tam);
    if (cin.fail() || destino[0] == '\0') {
        throw runtime_error("La ruta es invalida o esta vacia");
    }

    int largo = 0;
    while (destino[largo] != '\0') {
        largo++;
    }
    if (largo >= 2 && destino[0] == '"' && destino[largo - 1] == '"') {   //
        for (int i = 0; i < largo - 2; i++) {
            destino[i] = destino[i + 1];
        }
        destino[largo - 2] = '\0';
    }
    if (destino[0] == '\0') {
        throw runtime_error("La ruta es invalida o esta vacia");   // ...avisamos del error
    }
}

int main() {

    unsigned char* original = nullptr;
    unsigned char* comprimido = nullptr;
    unsigned char* cifrado = nullptr;
    unsigned char* recuperado = nullptr;

    int* prefijos = nullptr;                   // Diccionario LZ78 al comprimir: arreglo de prefijos
    unsigned char* caracteres = nullptr;       // Diccionario LZ78 al comprimir: arreglo de caracteres
    int nPares = 0;                            // Cantidad de pares generados al comprimir
    bool ultSinChar = false;                   // Si el ultimo par generado no tiene caracter

    int* prefijosLeidos = nullptr;             // Diccionario LZ78 reconstruido desde los bytes: prefijo
    unsigned char* caracteresLeidos = nullptr; // Diccionario LZ78 reconstruido desde los bytes: caracteres
    int nLeidos = 0;                           // Cantidad de pares leidos de los bytes
    bool ultLeida = false;                     // Bandera leida de los bytes

    int codigo = 0;

    try {
        int metodo = leerEntero("Metodo de compresion (1 = RLE, 2 = LZ78): ", 1, 2);
        int n = leerEntero("Rotacion de bits n (1 a 7): ", 1, 7);
        int K = leerEntero("Clave K (0 a 255): ", 0, 255);

        cin.ignore(1000, '\n');

        char rutaEntrada[256];
        char rutaCifrado[256];
        char rutaSalida[256];
        leerRuta("Archivo de entrada (texto original): ", rutaEntrada, 256);
        leerRuta("Archivo para guardar el mensaje cifrado: ", rutaCifrado, 256);
        leerRuta("Archivo para guardar el texto recuperado: ", rutaSalida, 256);

        int lenOrig = 0;                                   //cuantos bytes tiene el original
        original = leerArchivo(rutaEntrada, lenOrig);

        int lenComp = 0;
        if (metodo == 1) {
            comprimido = rleComprimirBytes(original, lenOrig, lenComp);
        } else {                                           // Si eligio LZ78...
            lz78Comprimir(original, lenOrig, prefijos, caracteres, nPares, ultSinChar);   //genera los dos arreglos paralelos...
            comprimido = lz78Serializar(prefijos, caracteres, nPares, ultSinChar, lenComp);   //los convierte en bytes
        }

        encriptar(comprimido, lenComp, n, (unsigned char)K);
        escribirArchivo(rutaCifrado, comprimido, lenComp);

        int lenCif = 0;                                    // bytes del archivo cifrado
        cifrado = leerArchivo(rutaCifrado, lenCif);
        desencriptar(cifrado, lenCif, n, (unsigned char)K);

        int lenRec = 0;
        if (metodo == 1) {
            recuperado = rleDescomprimirBytes(cifrado, lenCif, lenRec);
        } else {
            lz78Deserializar(cifrado, lenCif, prefijosLeidos, caracteresLeidos, nLeidos, ultLeida);   // Convierte los bytes en arreglos paralelos
            recuperado = lz78Descomprimir(prefijosLeidos, caracteresLeidos, nLeidos, ultLeida, lenRec);   // Reconstruye el texto
        }

        escribirArchivo(rutaSalida, recuperado, lenRec);

        cout << "\nTamano original:   " << lenOrig << " bytes\n";   // Muestra el tamano original
        cout << "Tamano comprimido: " << lenComp << " bytes\n";     // Muestra el tamano comprimido

        if (buffersIguales(original, lenOrig, recuperado, lenRec)) {
            cout << "Verificacion: Texto recuperado igual al original\n";
        } else {
            cout << "Verificacion: ERROR (el texto recuperado NO coincide)\n";
            codigo = 2;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        codigo = 1;
    }


    delete[] original;                         // Libera el texto original
    delete[] comprimido;                       // Libera los datos comprimidos/cifrados
    delete[] cifrado;                          // Libera los datos leidos del archivo cifrado
    delete[] recuperado;                       // Libera el texto recuperado
    delete[] prefijos;                         // Libera los prefijos generados al comprimir
    delete[] caracteres;                       // Libera los caracteres generados al comprimir
    delete[] prefijosLeidos;                   // Libera los prefijos reconstruidos desde los bytes
    delete[] caracteresLeidos;                 // Libera los caracteres reconstruidos desde los bytes
    return codigo;                             // Devuelve el codigo de salida al sistema operativo
}
