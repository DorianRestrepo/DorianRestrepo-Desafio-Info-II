#include "Usuario.h"
#include "Mensaje.h"          // Se Necesita la Definicion Completa para Ordenar y Liberar Mensajes
#include "Album.h"            // Se Necesita la Definicion Completa para Liberar Albumes
#include "Configuracion.h"    // Ruta Base de las Fotos

// Constructor con Datos
Usuario::Usuario(int Id, const string& Nombre_Usuario, const string& Contrasena, const string& Fecha_Nacimiento,
                 const string& Correo, const string& Ciudad, const string& Pais, const string& Fecha_Inscripcion){   // Se Declara el Constructor

    // Asignacion de los Datos Personales
    this->Id                = Id;                  // Se Guarda el Id (Posicion en el Sistema)
    this->Nombre_Usuario    = Nombre_Usuario;      // Se Guarda el Nombre de Usuario
    this->Contrasena        = Contrasena;          // Se Guarda la Contrasena
    this->Fecha_Nacimiento  = Fecha_Nacimiento;    // Se Guarda la Fecha de Nacimiento
    this->Correo            = Correo;              // Se Guarda el Correo
    this->Ciudad            = Ciudad;              // Se Guarda la Ciudad
    this->Pais              = Pais;                // Se Guarda el Pais
    this->Fecha_Inscripcion = Fecha_Inscripcion;   // Se Guarda la Fecha de Inscripcion
    this->Mensajes_No_Leidos = 0;                  // Inicia sin Mensajes Nuevos

    // Construccion de la Ruta de la Foto de Perfil Segun la Guia
    Ruta_Foto_Perfil = RUTA_BASE_FOTOS + Nombre_Usuario + "/albums/profile.png";   // Ej: /users/udeastorage/juanito76/albums/profile.png

    // Registro de la Memoria de los Textos
    Recursos::reservar(memoriaCadenas());   // Se Registran los Textos en el Heap

}

// Destructor: el Usuario es Dueno de sus Albumes y de sus Mensajes Enviados
Usuario::~Usuario(){   // Se Declara el Destructor

    // Ciclo que Libera los Albumes
    for (int i = 0 ; i < Albumes.obtenerTamano() ; i++){   // Recorre Cada Album
        delete Albumes[i];                                  // Se Libera el Album (y sus Fotos)
        Recursos::liberar(sizeof(Album));                   // Se Registra la Liberacion del Objeto
    }

    // Ciclo que Libera los Mensajes Enviados (el Receptor Solo Guarda una Referencia)
    for (int i = 0 ; i < Mensajes_Enviados.obtenerTamano() ; i++){   // Recorre Cada Mensaje Enviado
        delete Mensajes_Enviados[i];                                  // Se Libera el Mensaje
        Recursos::liberar(sizeof(Mensaje));                           // Se Registra la Liberacion del Objeto
    }

    // Liberacion de los Textos Propios
    Recursos::liberar(memoriaCadenas());   // Se Registra la Liberacion de los Textos

}

// Funcion que Inserta un Mensaje Manteniendo el Orden por Fecha
void Usuario::insertarPorFecha(ListaDinamica<Mensaje*>& Lista, Mensaje* Nuevo){   // Se Declara la Funcion de Insercion Ordenada

    // Declaracion de Variables Necesarias
    int Posicion = Lista.obtenerTamano();   // Se Empieza a Buscar Desde el Final (Caso Mas Comun)

    // Ciclo que Retrocede Mientras el Nuevo Mensaje sea Mas Antiguo
    while (Posicion > 0 && *Nuevo < *Lista[Posicion - 1]){   // Usa la Sobrecarga < de Mensaje
        Posicion--;                                          // Retrocede una Posicion
        Recursos::contarIteracion();                         // Registra la Iteracion
    }

    // Insercion en la Posicion Encontrada
    Lista.insertar(Posicion, Nuevo);   // Un Mensaje Nuevo Casi Siempre Queda al Final (O(1))

}

// Getters de los Datos Personales (Retornan Referencias para Evitar Copias)
int           Usuario::obtenerId() const{               return Id; }                  // Retorna el Id
const string& Usuario::obtenerNombreUsuario() const{    return Nombre_Usuario; }      // Retorna el Nombre
const string& Usuario::obtenerFechaNacimiento() const{  return Fecha_Nacimiento; }    // Retorna el Nacimiento
const string& Usuario::obtenerCorreo() const{           return Correo; }              // Retorna el Correo
const string& Usuario::obtenerRutaFotoPerfil() const{   return Ruta_Foto_Perfil; }    // Retorna la Foto de Perfil
const string& Usuario::obtenerCiudad() const{           return Ciudad; }              // Retorna la Ciudad
const string& Usuario::obtenerPais() const{             return Pais; }                // Retorna el Pais
const string& Usuario::obtenerFechaInscripcion() const{ return Fecha_Inscripcion; }   // Retorna la Inscripcion
const string& Usuario::obtenerContrasena() const{       return Contrasena; }          // Retorna la Contrasena

// Funcion que Verifica la Contrasena Ingresada
bool Usuario::verificarContrasena(const string& Intento) const{   // Se Declara la Funcion de Verificacion
    return Contrasena == Intento;                                 // Coincide si son Exactamente Iguales
}

// Setter del Correo
void Usuario::establecerCorreo(const string& Nuevo_Correo){   // Se Declara el Setter
    Recursos::liberar(Recursos::memoriaCadena(Correo));       // Se Descuenta el Texto Viejo
    Correo = Nuevo_Correo;                                    // Se Guarda el Texto Nuevo
    Recursos::reservar(Recursos::memoriaCadena(Correo));      // Se Registra el Texto Nuevo
}

// Setter de la Ciudad
void Usuario::establecerCiudad(const string& Nueva_Ciudad){   // Se Declara el Setter
    Recursos::liberar(Recursos::memoriaCadena(Ciudad));       // Se Descuenta el Texto Viejo
    Ciudad = Nueva_Ciudad;                                    // Se Guarda el Texto Nuevo
    Recursos::reservar(Recursos::memoriaCadena(Ciudad));      // Se Registra el Texto Nuevo
}

// Funcion que Agrega un Amigo
void Usuario::agregarAmigo(Usuario* Amigo){   // Se Declara la Funcion de Agregar Amigo
    if (Amigo != nullptr && Amigo != this){   // No se Agregan Nulos ni a Si Mismo
        Amigos.agregar(Amigo);                // Se Guarda el Puntero (O(1) Amortizado)
    }
}

// Funcion que Retorna un Amigo
Usuario* Usuario::obtenerAmigo(int Indice) const{   // Se Declara la Funcion de Consulta
    return Amigos.obtener(Indice);                  // Retorna nullptr si no Existe
}

// Funcion que Retorna la Cantidad de Amigos
int Usuario::cantidadAmigos() const{   // Se Declara la Funcion de Consulta
    return Amigos.obtenerTamano();     // Se Retorna el Tamano de la Lista
}

// Funcion que Envia una Solicitud de Amistad
void Usuario::enviarSolicitud(Usuario* Destino){   // Se Declara la Funcion de Envio
    if (Destino != nullptr){                       // Solo si el Destino Existe
        Destino->recibirSolicitud(this);           // La Solicitud Queda en la Lista del Destino
    }
}

// Funcion que Registra una Solicitud Recibida
void Usuario::recibirSolicitud(Usuario* Remitente){   // Se Declara la Funcion de Recepcion
    if (Remitente != nullptr){                        // Solo si el Remitente Existe
        Solicitudes_Amistad.agregar(Remitente);       // Se Guarda Quien Envio la Solicitud
    }
}

// Funcion que Indica si Hay una Solicitud Pendiente de un Usuario
bool Usuario::tieneSolicitudDe(Usuario* Remitente) const{   // Se Declara la Funcion de Verificacion
    return Solicitudes_Amistad.contiene(Remitente);         // Busca al Remitente en las Solicitudes
}

// Funcion que Acepta una Solicitud por su Posicion (Sobrecarga 1)
Usuario* Usuario::aceptarSolicitud(int Indice){   // Se Declara la Funcion de Aceptar

    // Declaracion de Variables Necesarias
    Usuario* Remitente = Solicitudes_Amistad.obtener(Indice);   // Usuario que Envio la Solicitud

    // Verificacion y Creacion de la Amistad Mutua
    if (Remitente != nullptr){                   // Si la Solicitud Existe
        Solicitudes_Amistad.eliminar(Indice);    // Se Quita de las Pendientes
        agregarAmigo(Remitente);                 // El Remitente es Ahora Mi Amigo
        Remitente->agregarAmigo(this);           // Y Yo Soy Amigo del Remitente
    }

    // Fin de la Funcion
    return Remitente;   // Se Retorna el Nuevo Amigo (nullptr si no Existia)

}

// Funcion que Acepta una Solicitud por Usuario (Sobrecarga 2)
bool Usuario::aceptarSolicitud(Usuario* Remitente){        // Se Declara la Funcion de Aceptar
    int Posicion = Solicitudes_Amistad.buscar(Remitente);  // Se Busca la Solicitud del Remitente
    return aceptarSolicitud(Posicion) != nullptr;          // Se Reutiliza la Sobrecarga por Posicion
}

// Funcion que Rechaza una Solicitud por su Posicion
Usuario* Usuario::rechazarSolicitud(int Indice){                  // Se Declara la Funcion de Rechazar
    Usuario* Remitente = Solicitudes_Amistad.obtener(Indice);     // Usuario que Envio la Solicitud
    if (Remitente != nullptr){                                    // Si la Solicitud Existe
        Solicitudes_Amistad.eliminar(Indice);                     // Solo se Quita de las Pendientes
    }
    return Remitente;                                             // Se Retorna el Usuario Rechazado
}

// Funcion que Retorna una Solicitud
Usuario* Usuario::obtenerSolicitud(int Indice) const{   // Se Declara la Funcion de Consulta
    return Solicitudes_Amistad.obtener(Indice);         // Retorna nullptr si no Existe
}

// Funcion que Retorna la Cantidad de Solicitudes
int Usuario::cantidadSolicitudes() const{     // Se Declara la Funcion de Consulta
    return Solicitudes_Amistad.obtenerTamano(); // Se Retorna el Tamano de la Lista
}

// Funcion que Registra un Mensaje Enviado
void Usuario::enviarMensaje(Mensaje* Nuevo){         // Se Declara la Funcion de Envio
    insertarPorFecha(Mensajes_Enviados, Nuevo);      // Se Guarda en Orden Cronologico
}

// Funcion que Registra un Mensaje Recibido
void Usuario::recibirMensaje(Mensaje* Nuevo){       // Se Declara la Funcion de Recepcion
    insertarPorFecha(Mensajes_Recibidos, Nuevo);    // Se Guarda en Orden Cronologico
    if (Nuevo->estaLeido() == false){               // Si el Mensaje no se ha Leido
        Mensajes_No_Leidos++;                       // Se Aumenta el Contador de Nuevos
    }
}

// Funciones de Consulta de Mensajes
Mensaje* Usuario::obtenerEnviado(int Indice) const{  return Mensajes_Enviados.obtener(Indice); }    // Retorna un Enviado
Mensaje* Usuario::obtenerRecibido(int Indice) const{ return Mensajes_Recibidos.obtener(Indice); }   // Retorna un Recibido
int      Usuario::cantidadEnviados() const{          return Mensajes_Enviados.obtenerTamano(); }    // Cantidad de Enviados
int      Usuario::cantidadRecibidos() const{         return Mensajes_Recibidos.obtenerTamano(); }   // Cantidad de Recibidos
int      Usuario::obtenerNoLeidos() const{           return Mensajes_No_Leidos; }                   // Mensajes sin Leer

// Funcion que Resta Uno a los Mensajes sin Leer
void Usuario::descontarNoLeido(){   // Se Declara la Funcion de Descuento
    if (Mensajes_No_Leidos > 0){    // Nunca Puede Quedar Negativo
        Mensajes_No_Leidos--;       // Se Resta Uno
    }
}

// Funcion que Busca el Primer Mensaje Recibido con Fecha >= Inicio (Busqueda Binaria)
int Usuario::buscarPrimerRecibidoDesde(const Fecha& Inicio) const{   // Se Declara la Funcion de Busqueda Binaria

    // Declaracion de Variables Necesarias para la Busqueda
    int Izquierda = 0;                                    // Limite Izquierdo del Rango de Busqueda
    int Derecha   = Mensajes_Recibidos.obtenerTamano();   // Limite Derecho (Excluido)
    int Medio     = 0;                                    // Posicion Central del Rango

    // Ciclo de Busqueda Binaria (la Lista ya esta Ordenada por Fecha)
    while (Izquierda < Derecha){                                             // Mientras el Rango no este Vacio
        Recursos::contarIteracion();                                         // Registra la Iteracion
        Medio = Izquierda + (Derecha - Izquierda) / 2;                       // Calcula el Centro sin Desbordar
        if (Mensajes_Recibidos[Medio]->obtenerFechaEnvio() < Inicio){        // Si el Mensaje es Anterior al Inicio
            Izquierda = Medio + 1;                                           // Se Descarta la Mitad Izquierda
        }
        else {                                                               // Si el Mensaje esta Dentro o Despues
            Derecha = Medio;                                                 // Se Descarta la Mitad Derecha
        }
    }

    // Fin de la Funcion
    return Izquierda;   // Posicion del Primer Mensaje en el Rango (o el Tamano si no Hay)

}

// Funcion que Borra las Sugerencias Anteriores
void Usuario::limpiarSugerencias(){   // Se Declara la Funcion de Limpieza
    Sugerencias_Amistad.vaciar();     // Se Libera el Arreglo de Sugerencias
}

// Funcion que Agrega una Sugerencia
void Usuario::agregarSugerencia(Usuario* Sugerido){   // Se Declara la Funcion de Agregar
    Sugerencias_Amistad.agregar(Sugerido);            // Se Guarda el Usuario Sugerido
}

// Funcion que Retorna una Sugerencia
Usuario* Usuario::obtenerSugerencia(int Indice) const{   // Se Declara la Funcion de Consulta
    return Sugerencias_Amistad.obtener(Indice);          // Retorna nullptr si no Existe
}

// Funcion que Retorna la Cantidad de Sugerencias
int Usuario::cantidadSugerencias() const{       // Se Declara la Funcion de Consulta
    return Sugerencias_Amistad.obtenerTamano();  // Se Retorna el Tamano de la Lista
}

// Funcion que Agrega un Album
void Usuario::agregarAlbum(Album* Nuevo){   // Se Declara la Funcion de Agregar
    if (Nuevo != nullptr){                  // Solo se Agregan Albumes Validos
        Albumes.agregar(Nuevo);             // Se Guarda el Puntero
    }
}

// Funcion que Retorna un Album
Album* Usuario::obtenerAlbum(int Indice) const{   // Se Declara la Funcion de Consulta
    return Albumes.obtener(Indice);               // Retorna nullptr si no Existe
}

// Funcion que Retorna la Cantidad de Albumes
int Usuario::cantidadAlbumes() const{   // Se Declara la Funcion de Consulta
    return Albumes.obtenerTamano();     // Se Retorna el Tamano de la Lista
}

// Funcion que Ajusta Todas las Listas a su Tamano Real (Despues de Cargar)
void Usuario::compactarListas(){   // Se Declara la Funcion de Compactacion
    Amigos.compactar();               // Se Libera el Espacio Sobrante de Amigos
    Mensajes_Enviados.compactar();    // Se Libera el Espacio Sobrante de Enviados
    Mensajes_Recibidos.compactar();   // Se Libera el Espacio Sobrante de Recibidos
    Solicitudes_Amistad.compactar();  // Se Libera el Espacio Sobrante de Solicitudes
    Albumes.compactar();              // Se Libera el Espacio Sobrante de Albumes

    for (int i = 0 ; i < Albumes.obtenerTamano() ; i++){   // Ciclo que Recorre Cada Album
        Albumes[i]->compactar();                            // Se Compacta la Lista de Fotos
        Recursos::contarIteracion();                        // Registra la Iteracion
    }
}

// Funcion que Calcula los Bytes que los Textos Ocupan en el Heap
long Usuario::memoriaCadenas() const{   // Se Declara la Funcion de Medicion
    return Recursos::memoriaCadena(Nombre_Usuario)   + Recursos::memoriaCadena(Contrasena) +
           Recursos::memoriaCadena(Fecha_Nacimiento) + Recursos::memoriaCadena(Correo) +
           Recursos::memoriaCadena(Ruta_Foto_Perfil) + Recursos::memoriaCadena(Ciudad) +
           Recursos::memoriaCadena(Pais)             + Recursos::memoriaCadena(Fecha_Inscripcion);   // Suma de Todos los Textos
}

// Sobrecarga del Operador == (Compara por Nombre de Usuario)
bool Usuario::operator==(const Usuario& Otro) const{   // Se Declara el Operador ==
    return Nombre_Usuario == Otro.Nombre_Usuario;      // Son Iguales si Tienen el Mismo Nombre
}

// Sobrecarga del Operador < (Orden Alfabetico)
bool Usuario::operator<(const Usuario& Otro) const{   // Se Declara el Operador <
    return Nombre_Usuario < Otro.Nombre_Usuario;      // Compara los Nombres Alfabeticamente
}

// Funcion Amiga que Imprime la Tarjeta de Perfil del Usuario
ostream& operator<<(ostream& Salida, const Usuario& Objeto){   // Se Declara el Operador <<
    Salida << "    Usuario        : " << Objeto.Nombre_Usuario << endl;                         // Nombre
    Salida << "    Ciudad / Pais  : " << Objeto.Ciudad << ", " << Objeto.Pais << endl;          // Ubicacion
    Salida << "    Nacimiento     : " << Objeto.Fecha_Nacimiento << endl;                       // Nacimiento
    Salida << "    Correo         : " << Objeto.Correo << endl;                                 // Correo
    Salida << "    Miembro desde  : " << Objeto.Fecha_Inscripcion << endl;                      // Inscripcion
    Salida << "    Amigos         : " << Objeto.Amigos.obtenerTamano() << endl;                 // Cantidad de Amigos
    Salida << "    Foto de perfil : " << Objeto.Ruta_Foto_Perfil << endl;                       // Foto de Perfil
    return Salida;                                                                              // Se Retorna el Flujo
}
