#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include <iostream>
#include "ListaDinamica.h"
#include "Fecha.h"

using namespace std;

class Mensaje;
class Album;

class Usuario {

private:

    // Datos Personales
    int    Id;
    string Nombre_Usuario;
    string Contrasena;
    string Fecha_Nacimiento;
    string Correo;
    string Ruta_Foto_Perfil;
    string Ciudad;
    string Pais;
    string Fecha_Inscripcion;
    int    Mensajes_No_Leidos;


    // Relaciones

    ListaDinamica<Usuario*> Amigos;
    ListaDinamica<Mensaje*> Mensajes_Enviados;
    ListaDinamica<Mensaje*> Mensajes_Recibidos;
    ListaDinamica<Usuario*> Solicitudes_Amistad;
    ListaDinamica<Usuario*> Sugerencias_Amistad;
    ListaDinamica<Album*>   Albumes;


    static void insertarPorFecha(ListaDinamica<Mensaje*>& Lista, Mensaje* Nuevo);

public:

    // Constructor y Destructor
    Usuario(int Id, const string& Nombre_Usuario, const string& Contrasena, const string& Fecha_Nacimiento,
            const string& Correo, const string& Ciudad, const string& Pais, const string& Fecha_Inscripcion);
    ~Usuario();


    Usuario(const Usuario& Otro) = delete;
    Usuario& operator=(const Usuario& Otro) = delete;

    // Getters
    int           obtenerId() const;
    const string& obtenerNombreUsuario() const;
    const string& obtenerFechaNacimiento() const;
    const string& obtenerCorreo() const;
    const string& obtenerRutaFotoPerfil() const;
    const string& obtenerCiudad() const;
    const string& obtenerPais() const;
    const string& obtenerFechaInscripcion() const;
    const string& obtenerContrasena() const;
    bool          verificarContrasena(const string& Intento) const;

    // Setters

    void establecerCorreo(const string& Nuevo_Correo);
    void establecerCiudad(const string& Nueva_Ciudad);

    // Funciones de amigos
    void     agregarAmigo(Usuario* Amigo);
    Usuario* obtenerAmigo(int Indice) const;
    int      cantidadAmigos() const;

    // Funciones de solicitudes de amistad
    void     enviarSolicitud(Usuario* Destino);
    void     recibirSolicitud(Usuario* Remitente);
    bool     tieneSolicitudDe(Usuario* Remitente) const;
    Usuario* aceptarSolicitud(int Indice);
    bool     aceptarSolicitud(Usuario* Remitente);
    Usuario* rechazarSolicitud(int Indice);
    Usuario* obtenerSolicitud(int Indice) const;
    int      cantidadSolicitudes() const;

    // Funciones de mensajes
    void     enviarMensaje(Mensaje* Nuevo);
    void     recibirMensaje(Mensaje* Nuevo);
    Mensaje* obtenerEnviado(int Indice) const;
    Mensaje* obtenerRecibido(int Indice) const;
    int      cantidadEnviados() const;
    int      cantidadRecibidos() const;
    int      buscarPrimerRecibidoDesde(const Fecha& Inicio) const;
    int      obtenerNoLeidos() const;
    void     descontarNoLeido();

    // Funciones de sugerencias
    void     limpiarSugerencias();
    void     agregarSugerencia(Usuario* Sugerido);
    Usuario* obtenerSugerencia(int Indice) const;
    int      cantidadSugerencias() const;

    // Funciones de albumes
    void   agregarAlbum(Album* Nuevo);
    Album* obtenerAlbum(int Indice) const;
    int    cantidadAlbumes() const;

    // Funciones de memoria
    void compactarListas();
    long memoriaCadenas() const;

    // Sobrecarga de operadores
    bool operator==(const Usuario& Otro) const;
    bool operator<(const Usuario& Otro) const;


    friend ostream& operator<<(ostream& Salida, const Usuario& Objeto);
};

#endif // USUARIO_H
