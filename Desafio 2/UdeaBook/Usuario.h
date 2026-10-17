#ifndef USUARIO_H
#define USUARIO_H

// Librerias Necesarias
#include <string>             // Libreria para el Manejo de Cadenas de Texto
#include <iostream>           // Libreria para Sobrecargar el Operador <<
#include "ListaDinamica.h"    // Estructura Propia para Todas las Relaciones
#include "Fecha.h"            // Clase Propia para Buscar Mensajes por Fecha

using namespace std;

// Declaraciones Adelantadas (Solo se Guardan Punteros)
class Mensaje;
class Album;

// Clase que Representa a un Usuario de la Red Social
class Usuario {

private:

    // Datos Personales
    int    Id;                  // Posicion del Usuario en el Sistema (Fila/Columna de la Matriz)
    string Nombre_Usuario;      // Nombre de Usuario Unico
    string Contrasena;          // Credencial para Iniciar Sesion
    string Fecha_Nacimiento;    // Fecha de Nacimiento (Solo se Muestra)
    string Correo;              // Correo Electronico
    string Ruta_Foto_Perfil;    // Ruta Absoluta de la Foto de Perfil
    string Ciudad;              // Ciudad de Residencia
    string Pais;                // Pais de Residencia
    string Fecha_Inscripcion;   // Fecha de Inscripcion (Solo se Muestra)
    int    Mensajes_No_Leidos;  // Contador para Avisar Mensajes Nuevos sin Recorrer la Lista

    // Relaciones (Todas con la Estructura Propia ListaDinamica)
    ListaDinamica<Usuario*> Amigos;                // Amigos Confirmados (Relacion Mutua)
    ListaDinamica<Mensaje*> Mensajes_Enviados;     // Mensajes Enviados Ordenados por Fecha (el Usuario es su Dueno)
    ListaDinamica<Mensaje*> Mensajes_Recibidos;    // Mensajes Recibidos Ordenados por Fecha
    ListaDinamica<Usuario*> Solicitudes_Amistad;   // Usuarios que le Enviaron Solicitud (Pendientes)
    ListaDinamica<Usuario*> Sugerencias_Amistad;   // Ultimas Sugerencias Calculadas
    ListaDinamica<Album*>   Albumes;               // Albumes del Usuario (el Usuario es su Dueno)

    // Funcion Auxiliar Privada
    static void insertarPorFecha(ListaDinamica<Mensaje*>& Lista, Mensaje* Nuevo);   // Mantiene el Orden Cronologico

public:

    // Constructor y Destructor
    Usuario(int Id, const string& Nombre_Usuario, const string& Contrasena, const string& Fecha_Nacimiento,
            const string& Correo, const string& Ciudad, const string& Pais, const string& Fecha_Inscripcion);   // Constructor con Datos
    ~Usuario();                                                                                                // Destructor

    // Se Prohibe Copiar un Usuario (Sus Relaciones son Unicas y Evita Dobles Liberaciones)
    Usuario(const Usuario& Otro) = delete;
    Usuario& operator=(const Usuario& Otro) = delete;

    // Getters
    int           obtenerId() const;               // Retorna el Id
    const string& obtenerNombreUsuario() const;    // Retorna el Nombre de Usuario
    const string& obtenerFechaNacimiento() const;  // Retorna la Fecha de Nacimiento
    const string& obtenerCorreo() const;           // Retorna el Correo
    const string& obtenerRutaFotoPerfil() const;   // Retorna la Ruta de la Foto de Perfil
    const string& obtenerCiudad() const;           // Retorna la Ciudad
    const string& obtenerPais() const;             // Retorna el Pais
    const string& obtenerFechaInscripcion() const; // Retorna la Fecha de Inscripcion
    const string& obtenerContrasena() const;       // Retorna la Contrasena (Solo para Guardar en Disco)
    bool          verificarContrasena(const string& Intento) const;   // Compara la Contrasena

    // Setters (Ajustan el Registro de Memoria al Cambiar un Texto)
    void establecerCorreo(const string& Nuevo_Correo);   // Cambia el Correo
    void establecerCiudad(const string& Nueva_Ciudad);   // Cambia la Ciudad

    // Funciones de Amigos
    void     agregarAmigo(Usuario* Amigo);       // Agrega un Amigo (Quien Llama Valida que no Exista)
    Usuario* obtenerAmigo(int Indice) const;     // Retorna el Amigo de una Posicion
    int      cantidadAmigos() const;             // Retorna la Cantidad de Amigos

    // Funciones de Solicitudes de Amistad
    void     enviarSolicitud(Usuario* Destino);           // Envia una Solicitud a Otro Usuario
    void     recibirSolicitud(Usuario* Remitente);        // Registra una Solicitud Recibida
    bool     tieneSolicitudDe(Usuario* Remitente) const;  // Indica si Hay Solicitud Pendiente de Alguien
    Usuario* aceptarSolicitud(int Indice);                // Acepta por Posicion (Sobrecarga 1)
    bool     aceptarSolicitud(Usuario* Remitente);        // Acepta por Usuario (Sobrecarga 2)
    Usuario* rechazarSolicitud(int Indice);               // Rechaza por Posicion
    Usuario* obtenerSolicitud(int Indice) const;          // Retorna la Solicitud de una Posicion
    int      cantidadSolicitudes() const;                 // Retorna la Cantidad de Solicitudes

    // Funciones de Mensajes
    void     enviarMensaje(Mensaje* Nuevo);                  // Registra un Mensaje Enviado
    void     recibirMensaje(Mensaje* Nuevo);                 // Registra un Mensaje Recibido
    Mensaje* obtenerEnviado(int Indice) const;               // Retorna un Mensaje Enviado
    Mensaje* obtenerRecibido(int Indice) const;              // Retorna un Mensaje Recibido
    int      cantidadEnviados() const;                       // Retorna la Cantidad de Enviados
    int      cantidadRecibidos() const;                      // Retorna la Cantidad de Recibidos
    int      buscarPrimerRecibidoDesde(const Fecha& Inicio) const;   // Busqueda Binaria por Fecha
    int      obtenerNoLeidos() const;                        // Retorna los Mensajes sin Leer
    void     descontarNoLeido();                             // Resta Uno a los Mensajes sin Leer

    // Funciones de Sugerencias
    void     limpiarSugerencias();                  // Borra las Sugerencias Anteriores
    void     agregarSugerencia(Usuario* Sugerido);  // Agrega una Sugerencia
    Usuario* obtenerSugerencia(int Indice) const;   // Retorna una Sugerencia
    int      cantidadSugerencias() const;           // Retorna la Cantidad de Sugerencias

    // Funciones de Albumes
    void   agregarAlbum(Album* Nuevo);          // Agrega un Album
    Album* obtenerAlbum(int Indice) const;      // Retorna un Album
    int    cantidadAlbumes() const;             // Retorna la Cantidad de Albumes

    // Funciones de Memoria
    void compactarListas();          // Ajusta Todas las Listas a su Tamano Real
    long memoriaCadenas() const;     // Bytes que sus Textos Ocupan en el Heap

    // Sobrecarga de Operadores
    bool operator==(const Usuario& Otro) const;   // Dos Usuarios son Iguales si Tienen el Mismo Nombre
    bool operator<(const Usuario& Otro) const;    // Orden Alfabetico por Nombre de Usuario

    // Funcion Amiga para Imprimir el Perfil
    friend ostream& operator<<(ostream& Salida, const Usuario& Objeto);
};

#endif // USUARIO_H
