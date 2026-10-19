#include "Foto.h"

// Constructor
Foto::Foto(const string& Fecha_Captura, int Tamano, const string& Ruta_Archivo){
    this->Fecha_Captura = Fecha_Captura;
    this->Tamano        = Tamano;
    this->Ruta_Archivo  = Ruta_Archivo;
    Recursos::reservar(memoriaCadenas());
}

// Constructor de Copia
Foto::Foto(const Foto& Otra) : Usuarios_Like(Otra.Usuarios_Like){
    Fecha_Captura = Otra.Fecha_Captura;
    Tamano        = Otra.Tamano;
    Ruta_Archivo  = Otra.Ruta_Archivo;
    Recursos::reservar(memoriaCadenas());
}

// Destructor
Foto::~Foto(){
    Recursos::liberar(memoriaCadenas());
}

// Funcion que Retorna la Fecha de Captura
const string& Foto::obtenerFechaCaptura() const{
    return Fecha_Captura;
}

// Funcion que Retorna el Tamano en Bytes
int Foto::obtenerTamano() const{
    return Tamano;
}

// Funcion que Retorna la Ruta del Archivo
const string& Foto::obtenerRutaArchivo() const{
    return Ruta_Archivo;
}

// Funcion que Extrae Solo el Nombre del Archivo (Para Guardarlo en Disco)
string Foto::obtenerNombreArchivo() const{


    size_t Ultima_Barra = Ruta_Archivo.rfind('/');
    size_t Punto        = Ruta_Archivo.rfind('.');
    size_t Inicio       = (Ultima_Barra == string::npos) ? 0 : Ultima_Barra + 1;

    // Verificacion de la Extension
    if (Punto == string::npos || Punto < Inicio){
        Punto = Ruta_Archivo.size();
    }

    // Fin de la Funcion
    return Ruta_Archivo.substr(Inicio, Punto - Inicio);

}

// Funcion que Retorna la Cantidad de Likes
int Foto::obtenerConteoLikes() const{
    return Usuarios_Like.obtenerTamano();
}

// Funcion que Retorna el Id de Quien Dio un Like
int Foto::obtenerIdLike(int Indice) const{
    return Usuarios_Like.obtener(Indice);
}

// Funcion que Cambia el Tamano Registrado de la Foto
void Foto::establecerTamano(int Nuevo_Tamano){
    if (Nuevo_Tamano >= 0){
        Tamano = Nuevo_Tamano;
    }
}

// Funcion que Indica si un Usuario ya Dio Like
bool Foto::tieneLike(int Id_Usuario) const{
    return Usuarios_Like.contiene(Id_Usuario);
}

// Funcion que Agrega el Like de un Usuario
bool Foto::darLike(int Id_Usuario){
    if (tieneLike(Id_Usuario)){
        return false;
    }
    Usuarios_Like.agregar(Id_Usuario);
    return true;
}

// Funcion que Quita el Like de un Usuario
bool Foto::quitarLike(int Id_Usuario){
    int Posicion = Usuarios_Like.buscar(Id_Usuario);
    if (Posicion == -1){
        return false;
    }
    Usuarios_Like.eliminar(Posicion);
    return true;
}

// Funcion que Calcula los Bytes que los Textos Ocupan en el Heap
long Foto::memoriaCadenas() const{
    return Recursos::memoriaCadena(Fecha_Captura) + Recursos::memoriaCadena(Ruta_Archivo);
}

// Funcion Amiga que Imprime la Informacion de la Foto
ostream& operator<<(ostream& Salida, const Foto& Objeto){
    Salida << "    Ruta    : " << Objeto.Ruta_Archivo << endl;
    Salida << "    Captura : " << Objeto.Fecha_Captura
           << "   |   Tamano: " << (Objeto.Tamano / 1024) << " KB"
           << "   |   Likes: " << Objeto.obtenerConteoLikes() << endl;
    return Salida;
}
