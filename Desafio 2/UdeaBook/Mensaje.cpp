#include "Mensaje.h"
#include "Usuario.h"
#include "Recursos.h"

// Constructor con Datos
Mensaje::Mensaje(Usuario* Emisor, Usuario* Receptor, const string& Contenido, const Fecha& Fecha_Envio, bool Estado_Leido)
    : Fecha_Envio(Fecha_Envio){
    this->Emisor       = Emisor;
    this->Receptor     = Receptor;
    this->Contenido    = Contenido;
    this->Estado_Leido = Estado_Leido;
    Recursos::reservar(memoriaCadenas());
}

// Destructor
Mensaje::~Mensaje(){
    Recursos::liberar(memoriaCadenas());
}

// Funcion que Retorna el Emisor
Usuario* Mensaje::obtenerEmisor() const{
    return Emisor;
}

// Funcion que Retorna el Receptor
Usuario* Mensaje::obtenerReceptor() const{
    return Receptor;
}

// Funcion que Retorna el Contenido
const string& Mensaje::obtenerContenido() const{
    return Contenido;
}

// Funcion que Retorna la Fecha de Envio
const Fecha& Mensaje::obtenerFechaEnvio() const{
    return Fecha_Envio;
}

// Funcion que Indica si el Mensaje ya fue Leido
bool Mensaje::estaLeido() const{
    return Estado_Leido;
}

// Funcion que Marca el Mensaje como Leido
bool Mensaje::marcarComoLeido(){
    if (Estado_Leido){
        return false;
    }
    Estado_Leido = true;
    return true;
}

// Sobrecarga del Operador < (Compara por Fecha de Envio)
bool Mensaje::operator<(const Mensaje& Otro) const{
    return Fecha_Envio < Otro.Fecha_Envio;
}

// Funcion que Calcula los Bytes que el Texto Ocupa en el Heap
long Mensaje::memoriaCadenas() const{
    return Recursos::memoriaCadena(Contenido);
}

// Funcion Amiga que Imprime el Mensaje
ostream& operator<<(ostream& Salida, const Mensaje& Objeto){
    Salida << "  [" << Objeto.Fecha_Envio << "]  De: " << Objeto.Emisor->obtenerNombreUsuario()
           << "  ->  Para: " << Objeto.Receptor->obtenerNombreUsuario()
           << (Objeto.Estado_Leido ? "   (leido)" : "   (NUEVO)") << endl;
    Salida << "      " << Objeto.Contenido << endl;
    return Salida;
}
