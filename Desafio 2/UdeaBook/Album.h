#ifndef ALBUM_H
#define ALBUM_H

// Librerias Necesarias
#include <string>             // Libreria para el Manejo de Cadenas de Texto
#include "ListaDinamica.h"    // Estructura Propia para Guardar las Fotos
#include "Configuracion.h"    // Codigos de Privacidad

using namespace std;

// Declaracion Adelantada (Solo se Guardan Punteros)
class Foto;

// Clase que Representa un Album de Fotos de un Usuario
class Album {

private:

    // Atributos del Album
    string               Nombre;                // Nombre del Album
    string               Ruta_Almacenamiento;   // Carpeta Donde Viven sus Fotos
    char                 Privacidad;            // 'P' Publico, 'A' Amigos, 'R' Privado (1 Byte)
    ListaDinamica<Foto*> Fotos;                 // Fotos que Contiene (el Album es su Dueno)

public:

    // Constructor y Destructor
    Album(const string& Nombre, const string& Ruta_Almacenamiento, char Privacidad);   // Constructor con Datos
    ~Album();                                                                          // Destructor (Libera sus Fotos)

    // Se Prohibe Copiar un Album (Evita que Dos Albumes Liberen las Mismas Fotos)
    Album(const Album& Otro) = delete;
    Album& operator=(const Album& Otro) = delete;

    // Funciones de Fotos
    void  agregarFoto(Foto* Nueva);        // Agrega una Foto al Album
    Foto* obtenerFoto(int Indice) const;   // Retorna la Foto de una Posicion
    int   cantidadFotos() const;           // Retorna la Cantidad de Fotos

    // Getters
    const string& obtenerNombre() const;   // Retorna el Nombre
    const string& obtenerRuta() const;     // Retorna la Ruta de Almacenamiento
    char          obtenerPrivacidad() const;   // Retorna el Codigo de Privacidad
    string        privacidadTexto() const;     // Retorna la Privacidad en Palabras

    // Funciones de Logica
    bool esVisible(bool Es_Dueno, bool Es_Amigo) const;   // Indica si Alguien Puede Ver el Album
    void compactar();                                     // Ajusta la Lista de Fotos a su Tamano Real
    long memoriaCadenas() const;                          // Bytes que sus Textos Ocupan en el Heap

    // Funciones Estaticas de Conversion de Privacidad
    static char   privacidadDesdeTexto(const string& Texto);   // "publico" -> 'P' ('\0' si es Invalida)
    static string privacidadATexto(char Codigo);               // 'P' -> "publico"
};

#endif // ALBUM_H
