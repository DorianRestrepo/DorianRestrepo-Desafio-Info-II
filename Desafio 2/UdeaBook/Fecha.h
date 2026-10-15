#ifndef FECHA_H
#define FECHA_H

// Librerias Necesarias
#include <string>     // Libreria para el Manejo de Cadenas de Texto
#include <ctime>      // Libreria de Tiempo (time_t, tm, time, mktime, localtime)
#include <iostream>   // Libreria para Sobrecargar el Operador <<

using namespace std;

// Clase que Representa una Fecha y Hora Usando la Libreria <ctime>
class Fecha {

private:

    // Atributo Unico: Instante Guardado como Segundos (8 Bytes)
    time_t Segundos;   // Segundos Transcurridos Desde el 1 de Enero de 1970 (Epoca Unix)

    // Funciones Auxiliares Privadas
    static string dosDigitos(int Numero);                     // Rellena con un Cero a la Izquierda
    static bool   construirTiempo(int Anio, int Mes, int Dia,
                                  int Hora, int Minuto, int Segundo,
                                  time_t& Resultado);         // Valida y Convierte Componentes a time_t

public:

    // Constructores
    Fecha();                       // Constructor por Defecto (Fecha y Hora Actual del Sistema)
    Fecha(const Fecha& Otra);      // Constructor de Copia

    // Sobrecarga del Operador de Asignacion
    Fecha& operator=(const Fecha& Otra);   // Copia el Instante de Otra Fecha

    // Conversion Desde y Hacia Texto
    static bool desdeTexto(const string& Texto, Fecha& Resultado);   // Lee "AAAA-MM-DD" o "AAAA-MM-DD HH:MM:SS"
    string aTexto() const;                                           // Retorna "AAAA-MM-DD HH:MM:SS"
    string aTextoCorto() const;                                      // Retorna "AAAA-MM-DD"

    // Funciones de Consulta
    Fecha  finDelDia() const;          // Retorna el Mismo Dia a las 23:59:59
    time_t obtenerSegundos() const;    // Retorna el Instante en Segundos

    // Sobrecarga de Operadores de Comparacion
    bool operator<(const Fecha& Otra) const;    // Menor que (Mas Antigua)
    bool operator>(const Fecha& Otra) const;    // Mayor que (Mas Reciente)
    bool operator<=(const Fecha& Otra) const;   // Menor o Igual
    bool operator>=(const Fecha& Otra) const;   // Mayor o Igual
    bool operator==(const Fecha& Otra) const;   // Igual
    bool operator!=(const Fecha& Otra) const;   // Diferente

    // Funcion Amiga para Imprimir la Fecha con cout
    friend ostream& operator<<(ostream& Salida, const Fecha& Objeto);
};

#endif // FECHA_H
