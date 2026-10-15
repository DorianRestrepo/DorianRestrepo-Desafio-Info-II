#include "Album.h"
#include "Foto.h"   // Se Necesita la Definicion Completa para Liberar las Fotos

// Constructor con Datos
Album::Album(const string& Nombre, const string& Ruta_Almacenamiento, char Privacidad){   // Se Declara el Constructor
    this->Nombre              = Nombre;                                                  // Se Guarda el Nombre
    this->Ruta_Almacenamiento = Ruta_Almacenamiento;                                     // Se Guarda la Ruta
    this->Privacidad          = Privacidad;                                              // Se Guarda la Privacidad
    Recursos::reservar(memoriaCadenas());                                                // Se Registran los Textos en el Heap
}

// Destructor: el Album es Dueno de sus Fotos (Composicion)
Album::~Album(){   // Se Declara el Destructor

    // Ciclo que Libera Cada Foto del Album
    for (int i = 0 ; i < Fotos.obtenerTamano() ; i++){   // Recorre Cada Foto
        delete Fotos[i];                                  // Se Libera la Foto (su Destructor Libera sus Textos)
        Recursos::liberar(sizeof(Foto));                  // Se Registra la Liberacion del Objeto
    }

    // Liberacion de los Textos Propios
    Recursos::liberar(memoriaCadenas());   // Se Registra la Liberacion de los Textos

}

// Funcion que Agrega una Foto al Album
void Album::agregarFoto(Foto* Nueva){   // Se Declara la Funcion de Agregar
    if (Nueva != nullptr){              // Solo se Agregan Fotos Validas
        Fotos.agregar(Nueva);           // Se Guarda el Puntero en la Lista
    }
}

// Funcion que Retorna la Foto de una Posicion
Foto* Album::obtenerFoto(int Indice) const{   // Se Declara la Funcion de Consulta
    return Fotos.obtener(Indice);             // Retorna nullptr si el Indice no Existe
}

// Funcion que Retorna la Cantidad de Fotos
int Album::cantidadFotos() const{   // Se Declara la Funcion de Consulta
    return Fotos.obtenerTamano();   // Se Retorna el Tamano de la Lista
}

// Funcion que Retorna el Nombre
const string& Album::obtenerNombre() const{   // Se Declara la Funcion de Consulta
    return Nombre;                            // Se Retorna por Referencia
}

// Funcion que Retorna la Ruta de Almacenamiento
const string& Album::obtenerRuta() const{   // Se Declara la Funcion de Consulta
    return Ruta_Almacenamiento;             // Se Retorna por Referencia
}

// Funcion que Retorna el Codigo de Privacidad
char Album::obtenerPrivacidad() const{   // Se Declara la Funcion de Consulta
    return Privacidad;                   // Se Retorna el Codigo
}

// Funcion que Retorna la Privacidad en Palabras
string Album::privacidadTexto() const{     // Se Declara la Funcion de Consulta
    return privacidadATexto(Privacidad);   // Se Traduce el Codigo a Texto
}

// Funcion que Indica si un Usuario Puede Ver el Album
bool Album::esVisible(bool Es_Dueno, bool Es_Amigo) const{   // Se Declara la Funcion de Visibilidad

    // Evaluacion de las Reglas de Privacidad
    if (Es_Dueno){                                   // El Dueno Siempre Puede Ver sus Albumes
        return true;                                 // Visible
    }
    if (Privacidad == PRIVACIDAD_PUBLICO){           // Los Albumes Publicos los Ve Cualquiera
        return true;                                 // Visible
    }
    if (Privacidad == PRIVACIDAD_AMIGOS && Es_Amigo){   // Los de Amigos Solo los Ven los Amigos
        return true;                                    // Visible
    }

    // Fin de la Funcion
    return false;   // En Cualquier Otro Caso no es Visible

}

// Funcion que Ajusta la Lista de Fotos a su Tamano Real
void Album::compactar(){   // Se Declara la Funcion de Compactar
    Fotos.compactar();     // Se Libera el Espacio Sobrante del Arreglo
}

// Funcion que Calcula los Bytes que los Textos Ocupan en el Heap
long Album::memoriaCadenas() const{   // Se Declara la Funcion de Medicion
    return Recursos::memoriaCadena(Nombre) + Recursos::memoriaCadena(Ruta_Almacenamiento);   // Suma de Ambos Textos
}

// Funcion que Convierte el Texto del Archivo al Codigo de Privacidad
char Album::privacidadDesdeTexto(const string& Texto){   // Se Declara la Funcion de Conversion
    if (Texto == "publico") return PRIVACIDAD_PUBLICO;   // Album Publico
    if (Texto == "amigos")  return PRIVACIDAD_AMIGOS;    // Album Solo para Amigos
    if (Texto == "privado") return PRIVACIDAD_PRIVADO;   // Album Privado
    return '\0';                                         // Texto no Reconocido
}

// Funcion que Convierte el Codigo de Privacidad a Texto
string Album::privacidadATexto(char Codigo){             // Se Declara la Funcion de Conversion
    if (Codigo == PRIVACIDAD_PUBLICO) return "publico";  // Album Publico
    if (Codigo == PRIVACIDAD_AMIGOS)  return "amigos";   // Album Solo para Amigos
    return "privado";                                    // Cualquier Otro Caso es Privado
}
