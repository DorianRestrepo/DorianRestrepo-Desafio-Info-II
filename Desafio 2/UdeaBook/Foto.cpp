#include "Foto.h"

// Constructor con Datos
Foto::Foto(const string& Fecha_Captura, int Tamano, const string& Ruta_Archivo){   // Se Declara el Constructor
    this->Fecha_Captura = Fecha_Captura;                                          // Se Guarda la Fecha de Captura
    this->Tamano        = Tamano;                                                 // Se Guarda el Tamano
    this->Ruta_Archivo  = Ruta_Archivo;                                           // Se Guarda la Ruta del Archivo
    Recursos::reservar(memoriaCadenas());                                         // Se Registran los Textos en el Heap
}

// Constructor de Copia (la Lista de Likes se Copia con su Propio Constructor de Copia)
Foto::Foto(const Foto& Otra) : Usuarios_Like(Otra.Usuarios_Like){   // Se Declara el Constructor de Copia
    Fecha_Captura = Otra.Fecha_Captura;                              // Se Copia la Fecha
    Tamano        = Otra.Tamano;                                     // Se Copia el Tamano
    Ruta_Archivo  = Otra.Ruta_Archivo;                               // Se Copia la Ruta
    Recursos::reservar(memoriaCadenas());                            // Se Registran los Textos Copiados
}

// Destructor
Foto::~Foto(){                              // Se Declara el Destructor
    Recursos::liberar(memoriaCadenas());    // Se Registra la Liberacion de los Textos
}                                           // La Lista de Likes se Libera Sola con su Destructor

// Funcion que Retorna la Fecha de Captura
const string& Foto::obtenerFechaCaptura() const{   // Se Declara la Funcion de Consulta
    return Fecha_Captura;                          // Se Retorna por Referencia (sin Copias)
}

// Funcion que Retorna el Tamano en Bytes
int Foto::obtenerTamano() const{   // Se Declara la Funcion de Consulta
    return Tamano;                 // Se Retorna el Tamano
}

// Funcion que Retorna la Ruta del Archivo
const string& Foto::obtenerRutaArchivo() const{   // Se Declara la Funcion de Consulta
    return Ruta_Archivo;                          // Se Retorna por Referencia (sin Copias)
}

// Funcion que Extrae Solo el Nombre del Archivo (Para Guardarlo en Disco)
string Foto::obtenerNombreArchivo() const{   // Se Declara la Funcion de Extraccion

    // Declaracion de Variables Necesarias
    size_t Ultima_Barra = Ruta_Archivo.rfind('/');   // Posicion de la Ultima Barra de la Ruta
    size_t Punto        = Ruta_Archivo.rfind('.');   // Posicion del Punto de la Extension
    size_t Inicio       = (Ultima_Barra == string::npos) ? 0 : Ultima_Barra + 1;   // Primer Caracter del Nombre

    // Verificacion de la Extension
    if (Punto == string::npos || Punto < Inicio){   // Si no Hay Extension
        Punto = Ruta_Archivo.size();                // Se Toma Hasta el Final
    }

    // Fin de la Funcion
    return Ruta_Archivo.substr(Inicio, Punto - Inicio);   // Se Retorna el Nombre sin Carpeta ni Extension

}

// Funcion que Retorna la Cantidad de Likes
int Foto::obtenerConteoLikes() const{     // Se Declara la Funcion de Consulta
    return Usuarios_Like.obtenerTamano();  // El Conteo es el Tamano de la Lista (no se Duplica el Dato)
}

// Funcion que Retorna el Id de Quien Dio un Like
int Foto::obtenerIdLike(int Indice) const{   // Se Declara la Funcion de Consulta
    return Usuarios_Like.obtener(Indice);    // Se Retorna el Id de la Posicion Pedida
}

// Funcion que Cambia el Tamano Registrado de la Foto
void Foto::establecerTamano(int Nuevo_Tamano){   // Se Declara el Setter
    if (Nuevo_Tamano >= 0){                      // Solo se Aceptan Tamanos Validos
        Tamano = Nuevo_Tamano;                   // Se Guarda el Nuevo Tamano
    }
}

// Funcion que Indica si un Usuario ya Dio Like
bool Foto::tieneLike(int Id_Usuario) const{   // Se Declara la Funcion de Verificacion
    return Usuarios_Like.contiene(Id_Usuario);   // Busca el Id en la Lista de Likes
}

// Funcion que Agrega el Like de un Usuario
bool Foto::darLike(int Id_Usuario){   // Se Declara la Funcion de Dar Like
    if (tieneLike(Id_Usuario)){       // Si el Usuario ya Habia Dado Like
        return false;                 // No se Repite el Like
    }
    Usuarios_Like.agregar(Id_Usuario);   // Se Registra el Like
    return true;                         // Se Indica que el Like se Agrego
}

// Funcion que Quita el Like de un Usuario
bool Foto::quitarLike(int Id_Usuario){                // Se Declara la Funcion de Quitar Like
    int Posicion = Usuarios_Like.buscar(Id_Usuario);  // Se Busca el Like del Usuario
    if (Posicion == -1){                              // Si el Usuario no Habia Dado Like
        return false;                                 // No Hay Nada que Quitar
    }
    Usuarios_Like.eliminar(Posicion);                 // Se Elimina el Like
    return true;                                      // Se Indica que el Like se Quito
}

// Funcion que Calcula los Bytes que los Textos Ocupan en el Heap
long Foto::memoriaCadenas() const{   // Se Declara la Funcion de Medicion
    return Recursos::memoriaCadena(Fecha_Captura) + Recursos::memoriaCadena(Ruta_Archivo);   // Suma de Ambos Textos
}

// Funcion Amiga que Imprime la Informacion de la Foto
ostream& operator<<(ostream& Salida, const Foto& Objeto){   // Se Declara el Operador <<
    Salida << "    Ruta    : " << Objeto.Ruta_Archivo << endl;                          // Ruta del Archivo
    Salida << "    Captura : " << Objeto.Fecha_Captura                                  // Fecha de Captura
           << "   |   Tamano: " << (Objeto.Tamano / 1024) << " KB"                      // Tamano en KB
           << "   |   Likes: " << Objeto.obtenerConteoLikes() << endl;                  // Cantidad de Likes
    return Salida;                                                                      // Se Retorna el Flujo
}
