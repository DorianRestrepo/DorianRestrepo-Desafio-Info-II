#include "Album.h"
#include "Foto.h"

// Constructor
Album::Album(const string& Nombre, const string& Ruta_Almacenamiento, char Privacidad){
    this->Nombre              = Nombre;
    this->Ruta_Almacenamiento = Ruta_Almacenamiento;
    this->Privacidad          = Privacidad;
    Recursos::reservar(memoriaCadenas());
}

// Destructor
Album::~Album(){


    for (int i = 0 ; i < Fotos.obtenerTamano() ; i++){
        delete Fotos[i];
        Recursos::liberar(sizeof(Foto));
    }


    Recursos::liberar(memoriaCadenas());   // Se Registra la Liberacion de los Textos

}

// Funcion que Agrega una Foto al Album
void Album::agregarFoto(Foto* Nueva){
    if (Nueva != nullptr){
        Fotos.agregar(Nueva);
    }
}

// Funcion que Retorna la Foto de una Posicion
Foto* Album::obtenerFoto(int Indice) const{
    return Fotos.obtener(Indice);
}

// Funcion que Retorna la Cantidad de Fotos
int Album::cantidadFotos() const{
    return Fotos.obtenerTamano();
}

// Funcion que Retorna el Nombre
const string& Album::obtenerNombre() const{
    return Nombre;
}

// Funcion que Retorna la Ruta de Almacenamiento
const string& Album::obtenerRuta() const{
    return Ruta_Almacenamiento;
}

// Funcion que Retorna el Codigo de Privacidad
char Album::obtenerPrivacidad() const{
    return Privacidad;
}

// Funcion que Retorna la Privacidad en Palabras
string Album::privacidadTexto() const{
    return privacidadATexto(Privacidad);
}

// Funcion que Indica si un Usuario Puede Ver el Album
bool Album::esVisible(bool Es_Dueno, bool Es_Amigo) const{

    // Evaluacion de las Reglas de Privacidad
    if (Es_Dueno){
        return true;
    }
    if (Privacidad == PRIVACIDAD_PUBLICO){
        return true;
    }
    if (Privacidad == PRIVACIDAD_AMIGOS && Es_Amigo){
        return true;
    }

    return false;

}

// Funcion que Ajusta la Lista de Fotos a su Tamano Real
void Album::compactar(){
    Fotos.compactar();
}

// Funcion que Calcula los Bytes que los Textos Ocupan en el Heap
long Album::memoriaCadenas() const{
    return Recursos::memoriaCadena(Nombre) + Recursos::memoriaCadena(Ruta_Almacenamiento);
}

// Funcion que Convierte el Texto del Archivo al Codigo de Privacidad
char Album::privacidadDesdeTexto(const string& Texto){
    if (Texto == "publico") return PRIVACIDAD_PUBLICO;
    if (Texto == "amigos")  return PRIVACIDAD_AMIGOS;
    if (Texto == "privado") return PRIVACIDAD_PRIVADO;
    return '\0';
}

// Funcion que Convierte el Codigo de Privacidad a Texto
string Album::privacidadATexto(char Codigo){
    if (Codigo == PRIVACIDAD_PUBLICO) return "publico";
    if (Codigo == PRIVACIDAD_AMIGOS)  return "amigos";
    return "privado";
}
