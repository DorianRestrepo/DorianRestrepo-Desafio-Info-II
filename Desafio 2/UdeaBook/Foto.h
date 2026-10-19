#ifndef FOTO_H
#define FOTO_H


#include <string>
#include <iostream>
#include "ListaDinamica.h"

using namespace std;


class Foto {

private:

    //atributos
    string             Fecha_Captura;
    int                Tamano;
    string             Ruta_Archivo;
    ListaDinamica<int> Usuarios_Like;   // Ids de los Usuarios que Dieron Like

public:

    // Constructores y Destructor
    Foto(const string& Fecha_Captura, int Tamano, const string& Ruta_Archivo);
    Foto(const Foto& Otra);
    ~Foto();

    // Getters
    const string& obtenerFechaCaptura() const;
    int           obtenerTamano() const;
    const string& obtenerRutaArchivo() const;
    string        obtenerNombreArchivo() const;
    int           obtenerConteoLikes() const;
    int           obtenerIdLike(int Indice) const;

    // Setter
    void establecerTamano(int Nuevo_Tamano);

    // Funciones de Likes
    bool tieneLike(int Id_Usuario) const;
    bool darLike(int Id_Usuario);
    bool quitarLike(int Id_Usuario);

    // Funcion de Medicion
    long memoriaCadenas() const;   // Bytes que sus Textos Ocupan en el Heap

    // Funcion Amiga para Imprimir la Foto
    friend ostream& operator<<(ostream& Salida, const Foto& Objeto);
};

#endif // FOTO_H
