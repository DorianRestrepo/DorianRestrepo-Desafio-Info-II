#ifndef LISTADINAMICA_H
#define LISTADINAMICA_H

// Librerias Necesarias
#include "Recursos.h"   // Clase que Registra Iteraciones y Memoria

// Plantilla de Clase que Administra un Arreglo Dinamico Generico
// Nota: al ser una Plantilla, su Implementacion Completa Debe Estar en el .h
template <class T>
class ListaDinamica {

private:

    // Atributos de la Lista
    T*  Elementos;       // Puntero al Arreglo Guardado en el Heap
    int Capacidad;       // Cantidad de Espacios Reservados en el Arreglo
    int Tamano_Actual;   // Cantidad de Espacios Realmente Ocupados

    // Funcion Auxiliar Privada
    void redimensionar(int Nueva_Capacidad);   // Cambia el Tamano del Arreglo Copiando los Datos

public:

    // Constructores y Destructor
    ListaDinamica();                                // Constructor por Defecto (Lista Vacia sin Reservar Memoria)
    ListaDinamica(const ListaDinamica<T>& Otra);    // Constructor de Copia (Copia Profunda del Arreglo)
    ~ListaDinamica();                               // Destructor (Libera el Arreglo del Heap)

    // Sobrecarga de Operadores
    ListaDinamica<T>& operator=(const ListaDinamica<T>& Otra);   // Asignacion con Copia Profunda
    T&       operator[](int Indice);                             // Acceso Directo para Modificar
    const T& operator[](int Indice) const;                       // Acceso Directo de Solo Lectura

    // Funciones Esenciales
    void agregar(const T& Elemento);                 // Agrega un Elemento al Final
    void insertar(int Indice, const T& Elemento);    // Inserta un Elemento en una Posicion
    T    obtener(int Indice) const;                  // Retorna el Elemento de una Posicion (con Validacion)
    void eliminar(int Indice);                       // Elimina el Elemento de una Posicion
    int  buscar(const T& Elemento) const;            // Retorna la Posicion de un Elemento o -1
    bool contiene(const T& Elemento) const;          // Indica si el Elemento Existe en la Lista
    int  obtenerTamano() const;                      // Retorna la Cantidad de Elementos
    int  obtenerCapacidad() const;                   // Retorna la Cantidad de Espacios Reservados
    bool estaVacia() const;                          // Indica si la Lista no Tiene Elementos
    void vaciar();                                   // Elimina Todo y Libera el Arreglo
    void compactar();                                // Ajusta la Capacidad al Tamano Real
};

// Constructor por Defecto
template <class T>
ListaDinamica<T>::ListaDinamica(){   // Se Declara el Constructor por Defecto
    Elementos     = nullptr;         // No se Reserva Memoria Hasta que se Necesite
    Capacidad     = 0;               // Sin Espacios Reservados
    Tamano_Actual = 0;               // Sin Elementos Guardados
}

// Constructor de Copia (Copia Profunda)
template <class T>
ListaDinamica<T>::ListaDinamica(const ListaDinamica<T>& Otra){   // Se Declara el Constructor de Copia

    // Inicializacion de la Lista Nueva
    Elementos     = nullptr;   // Se Inicia sin Arreglo
    Capacidad     = 0;         // Se Inicia sin Espacios
    Tamano_Actual = 0;         // Se Inicia sin Elementos

    // Copia de los Elementos de la Otra Lista
    if (Otra.Tamano_Actual > 0){                                                  // Si la Otra Lista Tiene Datos
        Elementos = new T[Otra.Tamano_Actual];                                    // Se Reserva un Arreglo del Tamano Exacto
        Recursos::reservar(static_cast<long>(Otra.Tamano_Actual) * sizeof(T));   // Se Registra la Memoria Reservada
        Capacidad = Otra.Tamano_Actual;                                           // Se Guarda la Nueva Capacidad

        for (int i = 0 ; i < Otra.Tamano_Actual ; i++){   // Ciclo que Copia Cada Elemento
            Elementos[i] = Otra.Elementos[i];             // Copia el Elemento en la Misma Posicion
            Recursos::contarIteracion();                  // Registra la Iteracion
        }
        Tamano_Actual = Otra.Tamano_Actual;   // Se Actualiza el Tamano Copiado
    }
}

// Destructor
template <class T>
ListaDinamica<T>::~ListaDinamica(){                                    // Se Declara el Destructor
    if (Elementos != nullptr){                                         // Si Existe un Arreglo Reservado
        delete[] Elementos;                                            // Se Libera el Arreglo del Heap
        Recursos::liberar(static_cast<long>(Capacidad) * sizeof(T));   // Se Registra la Memoria Liberada
    }
}

// Funcion que Cambia el Tamano del Arreglo Interno
template <class T>
void ListaDinamica<T>::redimensionar(int Nueva_Capacidad){   // Se Declara la Funcion de Redimension

    // Validacion de la Nueva Capacidad
    if (Nueva_Capacidad < Tamano_Actual){   // Nunca se Puede Reducir por Debajo de lo Ocupado
        Nueva_Capacidad = Tamano_Actual;    // Se Ajusta al Minimo Posible
    }
    if (Nueva_Capacidad == Capacidad){      // Si la Capacidad no Cambia
        return;                             // No Hay Nada que Hacer
    }

    // Declaracion de Variables Necesarias para la Redimension
    T* Nuevo_Arreglo = nullptr;   // Arreglo que Reemplazara al Actual

    // Reserva del Nuevo Arreglo
    if (Nueva_Capacidad > 0){                                                // Si se Necesitan Espacios
        Nuevo_Arreglo = new T[Nueva_Capacidad];                              // Se Reserva el Nuevo Arreglo
        Recursos::reservar(static_cast<long>(Nueva_Capacidad) * sizeof(T));  // Se Registra la Memoria Reservada
    }

    // Ciclo que Copia los Elementos al Nuevo Arreglo
    for (int i = 0 ; i < Tamano_Actual ; i++){   // Recorre Cada Elemento Ocupado
        Nuevo_Arreglo[i] = Elementos[i];         // Copia el Elemento (Solo un Puntero si T es Puntero)
        Recursos::contarIteracion();             // Registra la Iteracion
    }

    // Liberacion del Arreglo Anterior
    if (Elementos != nullptr){                                         // Si Habia un Arreglo Viejo
        delete[] Elementos;                                            // Se Libera el Arreglo Viejo
        Recursos::liberar(static_cast<long>(Capacidad) * sizeof(T));   // Se Registra la Memoria Liberada
    }

    // Actualizacion de los Atributos
    Elementos = Nuevo_Arreglo;     // La Lista Ahora Apunta al Nuevo Arreglo
    Capacidad = Nueva_Capacidad;   // Se Guarda la Nueva Capacidad

}

// Sobrecarga del Operador de Asignacion (Copia Profunda)
template <class T>
ListaDinamica<T>& ListaDinamica<T>::operator=(const ListaDinamica<T>& Otra){   // Se Declara el Operador =

    // Verificacion de Autoasignacion
    if (this == &Otra){   // Si se Asigna la Lista a Si Misma
        return *this;     // No se Hace Nada
    }

    // Liberacion de los Datos Actuales y Copia de los Nuevos
    vaciar();                                     // Se Libera el Arreglo Actual
    redimensionar(Otra.Tamano_Actual);            // Se Reserva el Tamano Exacto Necesario
    for (int i = 0 ; i < Otra.Tamano_Actual ; i++){   // Ciclo que Copia Cada Elemento
        Elementos[i] = Otra.Elementos[i];             // Copia el Elemento en la Misma Posicion
        Recursos::contarIteracion();                  // Registra la Iteracion
    }
    Tamano_Actual = Otra.Tamano_Actual;           // Se Actualiza el Tamano

    // Fin de la Funcion
    return *this;   // Se Retorna la Lista para Permitir Asignaciones Encadenadas

}

// Sobrecarga del Operador [] (Lectura y Escritura)
template <class T>
T& ListaDinamica<T>::operator[](int Indice){   // Se Declara el Operador []
    return Elementos[Indice];                  // Acceso Directo O(1) (Quien Llama Garantiza el Rango)
}

// Sobrecarga del Operador [] (Solo Lectura)
template <class T>
const T& ListaDinamica<T>::operator[](int Indice) const{   // Se Declara el Operador [] Constante
    return Elementos[Indice];                              // Acceso Directo O(1) (Quien Llama Garantiza el Rango)
}

// Funcion que Agrega un Elemento al Final de la Lista
template <class T>
void ListaDinamica<T>::agregar(const T& Elemento){   // Se Declara la Funcion de Agregar

    // Verificacion de Espacio Disponible
    if (Tamano_Actual == Capacidad){                       // Si el Arreglo Esta Lleno
        redimensionar(Capacidad == 0 ? 4 : Capacidad * 2);  // Se Duplica la Capacidad (o se Inicia en 4)
    }

    // Insercion del Elemento
    Elementos[Tamano_Actual] = Elemento;   // Se Guarda el Elemento en la Primera Posicion Libre
    Tamano_Actual++;                       // Se Aumenta el Tamano de la Lista

}

// Funcion que Inserta un Elemento en una Posicion Especifica
template <class T>
void ListaDinamica<T>::insertar(int Indice, const T& Elemento){   // Se Declara la Funcion de Insertar

    // Validacion del Indice Recibido
    if (Indice < 0){                  // Si el Indice es Negativo
        Indice = 0;                   // Se Inserta al Inicio
    }
    if (Indice > Tamano_Actual){      // Si el Indice Supera el Final
        Indice = Tamano_Actual;       // Se Inserta al Final
    }

    // Verificacion de Espacio Disponible
    if (Tamano_Actual == Capacidad){                       // Si el Arreglo Esta Lleno
        redimensionar(Capacidad == 0 ? 4 : Capacidad * 2);  // Se Duplica la Capacidad
    }

    // Ciclo que Corre los Elementos una Posicion a la Derecha
    for (int i = Tamano_Actual ; i > Indice ; i--){   // Recorre Desde el Final Hasta el Indice
        Elementos[i] = Elementos[i - 1];              // Mueve el Elemento una Posicion
        Recursos::contarIteracion();                  // Registra la Iteracion
    }

    // Insercion del Elemento
    Elementos[Indice] = Elemento;   // Se Guarda el Elemento en el Hueco Creado
    Tamano_Actual++;                // Se Aumenta el Tamano de la Lista

}

// Funcion que Retorna el Elemento de una Posicion con Validacion
template <class T>
T ListaDinamica<T>::obtener(int Indice) const{          // Se Declara la Funcion de Obtener
    if (Indice < 0 || Indice >= Tamano_Actual){         // Si el Indice esta Fuera de Rango
        return T();                                     // Se Retorna un Valor Vacio (nullptr si T es Puntero)
    }
    return Elementos[Indice];                           // Se Retorna el Elemento Pedido
}

// Funcion que Elimina el Elemento de una Posicion
template <class T>
void ListaDinamica<T>::eliminar(int Indice){   // Se Declara la Funcion de Eliminar

    // Validacion del Indice Recibido
    if (Indice < 0 || Indice >= Tamano_Actual){   // Si el Indice esta Fuera de Rango
        return;                                   // No se Elimina Nada
    }

    // Ciclo que Corre los Elementos una Posicion a la Izquierda
    for (int i = Indice ; i < Tamano_Actual - 1 ; i++){   // Recorre Desde el Indice Hasta el Penultimo
        Elementos[i] = Elementos[i + 1];                  // Tapa el Hueco con el Siguiente Elemento
        Recursos::contarIteracion();                      // Registra la Iteracion
    }

    // Actualizacion del Tamano
    Tamano_Actual--;   // Se Reduce el Tamano (la Memoria del Objeto la Maneja Quien lo Creo)

}

// Funcion que Busca un Elemento y Retorna su Posicion
template <class T>
int ListaDinamica<T>::buscar(const T& Elemento) const{   // Se Declara la Funcion de Busqueda

    // Ciclo de Busqueda Lineal
    for (int i = 0 ; i < Tamano_Actual ; i++){   // Recorre Cada Elemento de la Lista
        Recursos::contarIteracion();             // Registra la Iteracion
        if (Elementos[i] == Elemento){           // Si Encuentra Coincidencia Exacta
            return i;                            // Retorna la Posicion Encontrada
        }
    }

    // Fin de la Funcion
    return -1;   // Se Retorna -1 si el Elemento no Existe

}

// Funcion que Indica si un Elemento Existe en la Lista
template <class T>
bool ListaDinamica<T>::contiene(const T& Elemento) const{   // Se Declara la Funcion de Verificacion
    return buscar(Elemento) != -1;                          // Existe si la Busqueda no Retorna -1
}

// Funcion que Retorna la Cantidad de Elementos
template <class T>
int ListaDinamica<T>::obtenerTamano() const{   // Se Declara la Funcion de Consulta
    return Tamano_Actual;                      // Se Retorna el Tamano Actual
}

// Funcion que Retorna la Cantidad de Espacios Reservados
template <class T>
int ListaDinamica<T>::obtenerCapacidad() const{   // Se Declara la Funcion de Consulta
    return Capacidad;                             // Se Retorna la Capacidad Actual
}

// Funcion que Indica si la Lista esta Vacia
template <class T>
bool ListaDinamica<T>::estaVacia() const{   // Se Declara la Funcion de Consulta
    return Tamano_Actual == 0;              // Esta Vacia si no Tiene Elementos
}

// Funcion que Vacia la Lista y Libera su Arreglo
template <class T>
void ListaDinamica<T>::vaciar(){                                       // Se Declara la Funcion de Vaciar
    if (Elementos != nullptr){                                         // Si Existe un Arreglo Reservado
        delete[] Elementos;                                            // Se Libera el Arreglo del Heap
        Recursos::liberar(static_cast<long>(Capacidad) * sizeof(T));   // Se Registra la Memoria Liberada
    }
    Elementos     = nullptr;   // La Lista Queda sin Arreglo
    Capacidad     = 0;         // Sin Espacios Reservados
    Tamano_Actual = 0;         // Sin Elementos
}

// Funcion que Ajusta la Capacidad al Tamano Real (Ahorra Memoria)
template <class T>
void ListaDinamica<T>::compactar(){   // Se Declara la Funcion de Compactar
    redimensionar(Tamano_Actual);     // Se Deja Exactamente el Espacio que se Usa
}

#endif // LISTADINAMICA_H
