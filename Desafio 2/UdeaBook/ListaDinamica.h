#ifndef LISTADINAMICA_H
#define LISTADINAMICA_H


#include "Recursos.h"

// Plantilla de Clase que Administra un Arreglo Dinamico Generico

template <class T>
class ListaDinamica {

private:

    // Atributos de la Lista
    T*  Elementos;
    int Capacidad;
    int Tamano_Actual;


    void redimensionar(int Nueva_Capacidad);

public:

    // Constructores y Destructor
    ListaDinamica();
    ListaDinamica(const ListaDinamica<T>& Otra);
    ~ListaDinamica();

    // Sobrecarga de Operadores
    ListaDinamica<T>& operator=(const ListaDinamica<T>& Otra);
    T&       operator[](int Indice);
    const T& operator[](int Indice) const;

    // Funciones Esenciales
    void agregar(const T& Elemento);
    void insertar(int Indice, const T& Elemento);
    T    obtener(int Indice) const;
    void eliminar(int Indice);
    int  buscar(const T& Elemento) const;
    bool contiene(const T& Elemento) const;
    int  obtenerTamano() const;
    int  obtenerCapacidad() const;
    bool estaVacia() const;
    void vaciar();
    void compactar();
};

// Constructor por Defecto
template <class T>
ListaDinamica<T>::ListaDinamica(){
    Elementos     = nullptr;
    Capacidad     = 0;
    Tamano_Actual = 0;
}

// Constructor de Copia
template <class T>
ListaDinamica<T>::ListaDinamica(const ListaDinamica<T>& Otra){

    // Inicializacion de lista nueva
    Elementos     = nullptr;
    Capacidad     = 0;
    Tamano_Actual = 0;


    if (Otra.Tamano_Actual > 0){
        Elementos = new T[Otra.Tamano_Actual];
        Recursos::reservar(static_cast<long>(Otra.Tamano_Actual) * sizeof(T));
        Capacidad = Otra.Tamano_Actual;

        for (int i = 0 ; i < Otra.Tamano_Actual ; i++){
            Elementos[i] = Otra.Elementos[i];
            Recursos::contarIteracion();
        }
        Tamano_Actual = Otra.Tamano_Actual;
    }
}

// Destructor
template <class T>
ListaDinamica<T>::~ListaDinamica(){
    if (Elementos != nullptr){
        delete[] Elementos;
        Recursos::liberar(static_cast<long>(Capacidad) * sizeof(T));
    }
}

// Funcion que Cambia el Tamano del Arreglo Interno
template <class T>
void ListaDinamica<T>::redimensionar(int Nueva_Capacidad){


    if (Nueva_Capacidad < Tamano_Actual){
        Nueva_Capacidad = Tamano_Actual;
    }
    if (Nueva_Capacidad == Capacidad){
        return;
    }


    T* Nuevo_Arreglo = nullptr;

    // Reserva del Nuevo Arreglo
    if (Nueva_Capacidad > 0){
        Nuevo_Arreglo = new T[Nueva_Capacidad];
        Recursos::reservar(static_cast<long>(Nueva_Capacidad) * sizeof(T));
    }

    // Ciclo que copia los elementos
    for (int i = 0 ; i < Tamano_Actual ; i++){
        Nuevo_Arreglo[i] = Elementos[i];
        Recursos::contarIteracion();
    }

    // Liberacion del Arreglo Anterior
    if (Elementos != nullptr){
        delete[] Elementos;
        Recursos::liberar(static_cast<long>(Capacidad) * sizeof(T));
    }

    // Actualizacion de los Atributos
    Elementos = Nuevo_Arreglo;
    Capacidad = Nueva_Capacidad;

}

// Sobrecarga del operador de asignacion
template <class T>
ListaDinamica<T>& ListaDinamica<T>::operator=(const ListaDinamica<T>& Otra){

    // Verificacion de Autoasignacion
    if (this == &Otra){
        return *this;
    }

    // Liberacion de los Datos Actuales y Copia de los Nuevos
    vaciar();
    redimensionar(Otra.Tamano_Actual);
    for (int i = 0 ; i < Otra.Tamano_Actual ; i++){
        Elementos[i] = Otra.Elementos[i];
        Recursos::contarIteracion();
    }
    Tamano_Actual = Otra.Tamano_Actual;


    return *this;

}

// Sobrecarga del operador [] (Lectura y Escritura)
template <class T>
T& ListaDinamica<T>::operator[](int Indice){
    return Elementos[Indice];
}

// Sobrecarga del operador [] (Lectura)
template <class T>
const T& ListaDinamica<T>::operator[](int Indice) const{
    return Elementos[Indice];
}

// Funcion que Agrega un Elemento al Final de la Lista
template <class T>
void ListaDinamica<T>::agregar(const T& Elemento){

    // Verificacion de Espacio Disponible
    if (Tamano_Actual == Capacidad){
        redimensionar(Capacidad == 0 ? 4 : Capacidad * 2);
    }

    // Insercion del Elemento
    Elementos[Tamano_Actual] = Elemento;
    Tamano_Actual++;
}

// Funcion que inserta un elemento en una posicion especifica
template <class T>
void ListaDinamica<T>::insertar(int Indice, const T& Elemento){

    // Validacion del indice recibido
    if (Indice < 0){
        Indice = 0;
    }
    if (Indice > Tamano_Actual){
        Indice = Tamano_Actual;
    }

    // Verificacion de espacio disponible
    if (Tamano_Actual == Capacidad){
        redimensionar(Capacidad == 0 ? 4 : Capacidad * 2);
    }

    // Ciclo que corre los elementos una posicion a la derecha
    for (int i = Tamano_Actual ; i > Indice ; i--){
        Elementos[i] = Elementos[i - 1];
        Recursos::contarIteracion();
    }

    // Insercion del elemento
    Elementos[Indice] = Elemento;
    Tamano_Actual++;

}

// Funcion que retorna el elemento de una posicion
template <class T>
T ListaDinamica<T>::obtener(int Indice) const{
    if (Indice < 0 || Indice >= Tamano_Actual){
        return T();
    }
    return Elementos[Indice];
}

// Funcion que elimina el elemento de una Posicion
template <class T>
void ListaDinamica<T>::eliminar(int Indice){

    // Validacion del Indice Recibido
    if (Indice < 0 || Indice >= Tamano_Actual){
        return;
    }

    // Ciclo que corre los elementos una posicion a la izquierda
    for (int i = Indice ; i < Tamano_Actual - 1 ; i++){
        Elementos[i] = Elementos[i + 1];
        Recursos::contarIteracion();
    }


    Tamano_Actual--;

}

// Funcion que busca un elemento y retorna su posicion
template <class T>
int ListaDinamica<T>::buscar(const T& Elemento) const{

    // Ciclo de Busqueda Lineal
    for (int i = 0 ; i < Tamano_Actual ; i++){
        Recursos::contarIteracion();
        if (Elementos[i] == Elemento){
            return i;
        }
    }


    return -1;   //Retorna -1 si el elemento no existe

}

// Funcion que indica si un elemento existe en la lista
template <class T>
bool ListaDinamica<T>::contiene(const T& Elemento) const{
    return buscar(Elemento) != -1;
}

// Funcion que retorna la cantidad de elementos
template <class T>
int ListaDinamica<T>::obtenerTamano() const{
    return Tamano_Actual;
}

// Funcion que retorna la cantidad de espacios reservados
template <class T>
int ListaDinamica<T>::obtenerCapacidad() const{
    return Capacidad;
}

// Funcion que indica si la lista esta vacia
template <class T>
bool ListaDinamica<T>::estaVacia() const{
    return Tamano_Actual == 0;
}

// Funcion que vacia la lista y libera su arreglo
template <class T>
void ListaDinamica<T>::vaciar(){
    if (Elementos != nullptr){
        delete[] Elementos;
        Recursos::liberar(static_cast<long>(Capacidad) * sizeof(T));
    }
    Elementos     = nullptr;
    Capacidad     = 0;
    Tamano_Actual = 0;
}

// Funcion que ajusta la capacidad al tamano real
template <class T>
void ListaDinamica<T>::compactar(){
    redimensionar(Tamano_Actual);
}

#endif // LISTADINAMICA_H
