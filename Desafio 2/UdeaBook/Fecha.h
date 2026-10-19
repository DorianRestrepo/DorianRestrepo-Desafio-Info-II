#ifndef FECHA_H
#define FECHA_H


#include <string>
#include <ctime>
#include <iostream>

using namespace std;


class Fecha {

private:

    time_t Segundos;

    static string dosDigitos(int Numero);
    static bool   construirTiempo(int Anio, int Mes, int Dia, int Hora, int Minuto, int Segundo, time_t& Resultado);         // Valida y Convierte Componentes a time_t

public:

    // Constructores
    Fecha();
    Fecha(const Fecha& Otra);

    //sobrecarga del operador de asignacion
    Fecha& operator=(const Fecha& Otra);

    // Conversion
    static bool desdeTexto(const string& Texto, Fecha& Resultado);   // Lee "AAAA-MM-DD" o "AAAA-MM-DD HH:MM:SS"
    string aTexto() const;                                           // Retorna "AAAA-MM-DD HH:MM:SS"
    string aTextoCorto() const;                                      // Retorna "AAAA-MM-DD"

    // Funciones de Consulta
    Fecha  finDelDia() const;
    time_t obtenerSegundos() const;

    // Sobrecarga de Operadores de Comparacion
    bool operator<(const Fecha& Otra) const;
    bool operator>(const Fecha& Otra) const;
    bool operator<=(const Fecha& Otra) const;
    bool operator>=(const Fecha& Otra) const;
    bool operator==(const Fecha& Otra) const;
    bool operator!=(const Fecha& Otra) const;


    // Funcion Amiga para Imprimir la Fecha con cout
    friend ostream& operator<<(ostream& Salida, const Fecha& Objeto);
};

#endif // FECHA_H
