#include "Usuario.h"
#include "Mensaje.h"          // Se Necesita la Definicion Completa para Ordenar y Liberar Mensajes
#include "Album.h"
#include "Configuracion.h"

// Constructor con Datos
Usuario::Usuario(int Id, const string& Nombre_Usuario, const string& Contrasena, const string& Fecha_Nacimiento, const string& Correo, const string& Ciudad, const string& Pais, const string& Fecha_Inscripcion){

    this->Id                = Id;
    this->Nombre_Usuario    = Nombre_Usuario;
    this->Contrasena        = Contrasena;
    this->Fecha_Nacimiento  = Fecha_Nacimiento;
    this->Correo            = Correo;
    this->Ciudad            = Ciudad;
    this->Pais              = Pais;
    this->Fecha_Inscripcion = Fecha_Inscripcion;
    this->Mensajes_No_Leidos = 0;


    Ruta_Foto_Perfil = RUTA_BASE_FOTOS + Nombre_Usuario + "/albums/profile.png";   // Ej: /users/udeastorage/juanito76/albums/profile.png

    Recursos::reservar(memoriaCadenas());

}

// Destructor: el Usuario es Dueno de sus Albumes y de sus Mensajes Enviados
Usuario::~Usuario(){

    for (int i = 0 ; i < Albumes.obtenerTamano() ; i++){
        delete Albumes[i];
        Recursos::liberar(sizeof(Album));
    }

    for (int i = 0 ; i < Mensajes_Enviados.obtenerTamano() ; i++){
        delete Mensajes_Enviados[i];
        Recursos::liberar(sizeof(Mensaje));
    }

    Recursos::liberar(memoriaCadenas());

}


void Usuario::insertarPorFecha(ListaDinamica<Mensaje*>& Lista, Mensaje* Nuevo){

    int Posicion = Lista.obtenerTamano();

    // Ciclo que Retrocede Mientras el Nuevo Mensaje sea Mas Antiguo
    while (Posicion > 0 && *Nuevo < *Lista[Posicion - 1]){
        Posicion--;
        Recursos::contarIteracion();
    }

    // Insercion en la Posicion Encontrada
    Lista.insertar(Posicion, Nuevo);

}

// Getters de los Datos Personales
int           Usuario::obtenerId() const{               return Id; }
const string& Usuario::obtenerNombreUsuario() const{    return Nombre_Usuario; }
const string& Usuario::obtenerFechaNacimiento() const{  return Fecha_Nacimiento; }
const string& Usuario::obtenerCorreo() const{           return Correo; }
const string& Usuario::obtenerRutaFotoPerfil() const{   return Ruta_Foto_Perfil; }
const string& Usuario::obtenerCiudad() const{           return Ciudad; }
const string& Usuario::obtenerPais() const{             return Pais; }
const string& Usuario::obtenerFechaInscripcion() const{ return Fecha_Inscripcion; }
const string& Usuario::obtenerContrasena() const{       return Contrasena; }

// Funcion que verifica la contrasena ingresada
bool Usuario::verificarContrasena(const string& Intento) const{
    return Contrasena == Intento;
}

// Setter del correo
void Usuario::establecerCorreo(const string& Nuevo_Correo){
    Recursos::liberar(Recursos::memoriaCadena(Correo));
    Correo = Nuevo_Correo;
    Recursos::reservar(Recursos::memoriaCadena(Correo));
}

// Setter de la ciudad
void Usuario::establecerCiudad(const string& Nueva_Ciudad){
    Recursos::liberar(Recursos::memoriaCadena(Ciudad));
    Ciudad = Nueva_Ciudad;
    Recursos::reservar(Recursos::memoriaCadena(Ciudad));
}

// Funcion que agrega un amigo
void Usuario::agregarAmigo(Usuario* Amigo){
    if (Amigo != nullptr && Amigo != this){
        Amigos.agregar(Amigo);
    }
}

// Funcion que retorna un amigo
Usuario* Usuario::obtenerAmigo(int Indice) const{
    return Amigos.obtener(Indice);
}

// Funcion que retorna la cantidad de amigos
int Usuario::cantidadAmigos() const{
    return Amigos.obtenerTamano();
}

// Funcion que envia una solicitud de amistad
void Usuario::enviarSolicitud(Usuario* Destino){
    if (Destino != nullptr){
        Destino->recibirSolicitud(this);
    }
}

// Funcion que registra una solicitud recibida
void Usuario::recibirSolicitud(Usuario* Remitente){
    if (Remitente != nullptr){
        Solicitudes_Amistad.agregar(Remitente);
    }
}

// Funcion que indica si hay una solicitud pendiente de un usuario
bool Usuario::tieneSolicitudDe(Usuario* Remitente) const{
    return Solicitudes_Amistad.contiene(Remitente);
}

// Funcion que acepta una solicitud por su posicion
Usuario* Usuario::aceptarSolicitud(int Indice){

    // Declaracion de variables necesarias
    Usuario* Remitente = Solicitudes_Amistad.obtener(Indice);

    // Verificacion y creacion de la amistad mutua
    if (Remitente != nullptr){
        Solicitudes_Amistad.eliminar(Indice);
        agregarAmigo(Remitente);
        Remitente->agregarAmigo(this);
    }


    return Remitente;

}

// Funcion que Acepta una Solicitud por Usuario (Sobrecarga 2)
bool Usuario::aceptarSolicitud(Usuario* Remitente){
    int Posicion = Solicitudes_Amistad.buscar(Remitente);
    return aceptarSolicitud(Posicion) != nullptr;
}

// Funcion que Rechaza una Solicitud por su Posicion
Usuario* Usuario::rechazarSolicitud(int Indice){
    Usuario* Remitente = Solicitudes_Amistad.obtener(Indice);
    if (Remitente != nullptr){
        Solicitudes_Amistad.eliminar(Indice);
    }
    return Remitente;
}

// Funcion que Retorna una Solicitud
Usuario* Usuario::obtenerSolicitud(int Indice) const{
    return Solicitudes_Amistad.obtener(Indice);
}

// Funcion que Retorna la Cantidad de Solicitudes
int Usuario::cantidadSolicitudes() const{
    return Solicitudes_Amistad.obtenerTamano();
}

// Funcion que Registra un Mensaje Enviado
void Usuario::enviarMensaje(Mensaje* Nuevo){
    insertarPorFecha(Mensajes_Enviados, Nuevo);
}

// Funcion que Registra un Mensaje Recibido
void Usuario::recibirMensaje(Mensaje* Nuevo){
    insertarPorFecha(Mensajes_Recibidos, Nuevo);
    if (Nuevo->estaLeido() == false){
        Mensajes_No_Leidos++;
    }
}

// Funciones de Consulta de Mensajes
Mensaje* Usuario::obtenerEnviado(int Indice) const{  return Mensajes_Enviados.obtener(Indice); }
Mensaje* Usuario::obtenerRecibido(int Indice) const{ return Mensajes_Recibidos.obtener(Indice); }
int      Usuario::cantidadEnviados() const{          return Mensajes_Enviados.obtenerTamano(); }
int      Usuario::cantidadRecibidos() const{         return Mensajes_Recibidos.obtenerTamano(); }
int      Usuario::obtenerNoLeidos() const{           return Mensajes_No_Leidos; }

// Funcion que Resta Uno a los Mensajes sin Leer
void Usuario::descontarNoLeido(){
    if (Mensajes_No_Leidos > 0){
        Mensajes_No_Leidos--;
    }
}

// Funcion que Busca el Primer Mensaje Recibido con Fecha >= Inicio
int Usuario::buscarPrimerRecibidoDesde(const Fecha& Inicio) const{

    // Declaracion de Variables Necesarias para la Busqueda
    int Izquierda = 0;
    int Derecha   = Mensajes_Recibidos.obtenerTamano();
    int Medio     = 0;

    // Ciclo de Busqueda Binaria (la Lista ya esta Ordenada por Fecha)
    while (Izquierda < Derecha){
        Recursos::contarIteracion();
        Medio = Izquierda + (Derecha - Izquierda) / 2;
        if (Mensajes_Recibidos[Medio]->obtenerFechaEnvio() < Inicio){
            Izquierda = Medio + 1;
        }
        else {
            Derecha = Medio;
        }
    }


    return Izquierda;

}

// Funcion que Borra las Sugerencias Anteriores
void Usuario::limpiarSugerencias(){
    Sugerencias_Amistad.vaciar();     // Se Libera el Arreglo de Sugerencias
}

// Funcion que Agrega una Sugerencia
void Usuario::agregarSugerencia(Usuario* Sugerido){
    Sugerencias_Amistad.agregar(Sugerido);
}

// Funcion que Retorna una Sugerencia
Usuario* Usuario::obtenerSugerencia(int Indice) const{
    return Sugerencias_Amistad.obtener(Indice);
}

// Funcion que Retorna la Cantidad de Sugerencias
int Usuario::cantidadSugerencias() const{
    return Sugerencias_Amistad.obtenerTamano();
}

// Funcion que Agrega un Album
void Usuario::agregarAlbum(Album* Nuevo){
    if (Nuevo != nullptr){
        Albumes.agregar(Nuevo);
    }
}

// Funcion que Retorna un Album
Album* Usuario::obtenerAlbum(int Indice) const{
    return Albumes.obtener(Indice);
}

// Funcion que Retorna la Cantidad de Albumes
int Usuario::cantidadAlbumes() const{
    return Albumes.obtenerTamano();
}

// Funcion que Ajusta Todas las Listas a su Tamano Real (Despues de Cargar)
void Usuario::compactarListas(){
    Amigos.compactar();
    Mensajes_Enviados.compactar();
    Mensajes_Recibidos.compactar();
    Solicitudes_Amistad.compactar();
    Albumes.compactar();

    for (int i = 0 ; i < Albumes.obtenerTamano() ; i++){
        Albumes[i]->compactar();
        Recursos::contarIteracion();
    }
}

// Funcion que Calcula los Bytes que los Textos Ocupan en el Heap
long Usuario::memoriaCadenas() const{
    return Recursos::memoriaCadena(Nombre_Usuario)   + Recursos::memoriaCadena(Contrasena) +
           Recursos::memoriaCadena(Fecha_Nacimiento) + Recursos::memoriaCadena(Correo) +
           Recursos::memoriaCadena(Ruta_Foto_Perfil) + Recursos::memoriaCadena(Ciudad) +
           Recursos::memoriaCadena(Pais)             + Recursos::memoriaCadena(Fecha_Inscripcion);
}

// Sobrecarga del Operador ==
bool Usuario::operator==(const Usuario& Otro) const{
    return Nombre_Usuario == Otro.Nombre_Usuario;
}

// Sobrecarga del Operador < (Orden Alfabetico)
bool Usuario::operator<(const Usuario& Otro) const{
    return Nombre_Usuario < Otro.Nombre_Usuario;
}

// Funcion Amiga que Imprime la Tarjeta de Perfil del Usuario
ostream& operator<<(ostream& Salida, const Usuario& Objeto){
    Salida << "    Usuario        : " << Objeto.Nombre_Usuario << endl;
    Salida << "    Ciudad / Pais  : " << Objeto.Ciudad << ", " << Objeto.Pais << endl;
    Salida << "    Nacimiento     : " << Objeto.Fecha_Nacimiento << endl;
    Salida << "    Correo         : " << Objeto.Correo << endl;
    Salida << "    Miembro desde  : " << Objeto.Fecha_Inscripcion << endl;
    Salida << "    Amigos         : " << Objeto.Amigos.obtenerTamano() << endl;
    Salida << "    Foto de perfil : " << Objeto.Ruta_Foto_Perfil << endl;
    return Salida;
}
