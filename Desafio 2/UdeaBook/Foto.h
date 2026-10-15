#ifndef FOTO_H
#define FOTO_H

// Librerias Necesarias
#include <string>             // Libreria para el Manejo de Cadenas de Texto
#include <iostream>           // Libreria para Sobrecargar el Operador <<
#include "ListaDinamica.h"    // Estructura Propia para Guardar Quienes Dieron Like

using namespace std;

// Clase que Representa una Foto Dentro de un Album
class Foto {

private:

    // Atributos de la Foto
    string             Fecha_Captura;   // Fecha en que se Tomo la Foto (Solo se Muestra)
    int                Tamano;          // Tamano del Archivo en Bytes
    string             Ruta_Archivo;    // Ruta Absoluta del Archivo .png
    ListaDinamica<int> Usuarios_Like;   // Ids de los Usuarios que Dieron Like (el Conteo es su Tamano)

public:

    // Constructores y Destructor
    Foto(const string& Fecha_Captura, int Tamano, const string& Ruta_Archivo);   // Constructor con Datos
    Foto(const Foto& Otra);                                                      // Constructor de Copia
    ~Foto();                                                                     // Destructor

    // Getters
    const string& obtenerFechaCaptura() const;   // Retorna la Fecha de Captura
    int           obtenerTamano() const;         // Retorna el Tamano en Bytes
    const string& obtenerRutaArchivo() const;    // Retorna la Ruta del Archivo
    string        obtenerNombreArchivo() const;  // Retorna Solo el Nombre (sin Carpeta ni .png)
    int           obtenerConteoLikes() const;    // Retorna la Cantidad de Likes
    int           obtenerIdLike(int Indice) const;   // Retorna el Id de Quien Dio el Like N

    // Setter
    void establecerTamano(int Nuevo_Tamano);     // Cambia el Tamano Registrado

    // Funciones de Likes
    bool tieneLike(int Id_Usuario) const;   // Indica si el Usuario ya Dio Like
    bool darLike(int Id_Usuario);           // Agrega el Like del Usuario (si no lo Tenia)
    bool quitarLike(int Id_Usuario);        // Quita el Like del Usuario (si lo Tenia)

    // Funcion de Medicion
    long memoriaCadenas() const;   // Bytes que sus Textos Ocupan en el Heap

    // Funcion Amiga para Imprimir la Foto
    friend ostream& operator<<(ostream& Salida, const Foto& Objeto);
};

#endif // FOTO_H
