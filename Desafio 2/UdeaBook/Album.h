#ifndef ALBUM_H
#define ALBUM_H


#include <string>
#include "ListaDinamica.h"
#include "Configuracion.h"

using namespace std;


class Foto;


class Album {

private:

    // Atributos del Album
    string               Nombre;                // Nombre
    string               Ruta_Almacenamiento;   // Carpeta
    char                 Privacidad;            // 'P' Publico, 'A' Amigos, 'R' Privado
    ListaDinamica<Foto*> Fotos;                 // Fotos que Contiene

public:


    Album(const string& Nombre, const string& Ruta_Almacenamiento, char Privacidad);   // Constructor
    ~Album();                                                                          // Destructor


    Album(const Album& Otro) = delete;
    Album& operator=(const Album& Otro) = delete;


    void  agregarFoto(Foto* Nueva);
    Foto* obtenerFoto(int Indice) const;
    int   cantidadFotos() const;

    // Getters
    const string& obtenerNombre() const;
    const string& obtenerRuta() const;
    char          obtenerPrivacidad() const;
    string        privacidadTexto() const;


    bool esVisible(bool Es_Dueno, bool Es_Amigo) const;
    void compactar();
    long memoriaCadenas() const;


    static char   privacidadDesdeTexto(const string& Texto);
    static string privacidadATexto(char Codigo);
};

#endif // ALBUM_H
