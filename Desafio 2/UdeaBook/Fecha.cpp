#include "Fecha.h"
#include "Recursos.h"
#include <sstream>


Fecha::Fecha(){
    Segundos = time(nullptr);
    Recursos::contarUsoCtime();
}

// Constructor de Copia
Fecha::Fecha(const Fecha& Otra){
    Segundos = Otra.Segundos;
}

// Sobrecarga del Operador de Asignacion
Fecha& Fecha::operator=(const Fecha& Otra){
    Segundos = Otra.Segundos;
    return *this;
}

// Funcion que Rellena un Numero con un Cero a la Izquierda
string Fecha::dosDigitos(int Numero){
    return (Numero < 10 ? "0" : "") + to_string(Numero);
}

// Funcion que Valida los Componentes de una Fecha y los Convierte a time_t
bool Fecha::construirTiempo(int Anio, int Mes, int Dia, int Hora, int Minuto, int Segundo, time_t& Resultado){

    // Validacion Basica de Rangos
    if (Anio < 1971 || Anio > 2099)     return false;
    if (Mes < 1 || Mes > 12)            return false;
    if (Dia < 1 || Dia > 31)            return false;
    if (Hora < 0 || Hora > 23)          return false;
    if (Minuto < 0 || Minuto > 59)      return false;
    if (Segundo < 0 || Segundo > 59)    return false;


    tm Tiempo = {};

    // Asignacion de los Componentes
    Tiempo.tm_year  = Anio - 1900;
    Tiempo.tm_mon   = Mes - 1;
    Tiempo.tm_mday  = Dia;
    Tiempo.tm_hour  = Hora;
    Tiempo.tm_min   = Minuto;
    Tiempo.tm_sec   = Segundo;
    Tiempo.tm_isdst = -1;

    // Conversion a Segundos con mktime
    time_t Calculado = mktime(&Tiempo);
    Recursos::contarUsoCtime();

    // Verificacion de que la Fecha Exista Realmente
    if (Calculado == static_cast<time_t>(-1)){
        return false;
    }
    if (Tiempo.tm_mday != Dia || Tiempo.tm_mon != Mes - 1 || Tiempo.tm_year != Anio - 1900){
        return false;
    }


    Resultado = Calculado;
    return true;

}

// Funcion que Lee una Fecha Desde un Texto "AAAA-MM-DD" o "AAAA-MM-DD HH:MM:SS"
bool Fecha::desdeTexto(const string& Texto, Fecha& Resultado){


    stringstream Flujo(Texto);
    int    Anio = 0, Mes = 0, Dia = 0;
    int    Hora = 0, Minuto = 0, Segundo = 0;
    char   Guion_1 = ' ', Guion_2 = ' ';
    char   Puntos_1 = ' ', Puntos_2 = ' ';
    time_t Calculado = 0;


    Flujo >> Anio >> Guion_1 >> Mes >> Guion_2 >> Dia;
    if (Flujo.fail() || Guion_1 != '-' || Guion_2 != '-'){
        return false;
    }


    if (Flujo.eof() == false){
        Flujo >> ws;
        if (Flujo.eof() == false){
            Flujo >> Hora >> Puntos_1 >> Minuto >> Puntos_2 >> Segundo;
            if (Flujo.fail() || Puntos_1 != ':' || Puntos_2 != ':'){
                return false;
            }
            if (Flujo.eof() == false){
                Flujo >> ws;
                if (Flujo.eof() == false){
                    return false;
                }
            }
        }
    }

    // Validacion y Conversion de los Componentes
    if (construirTiempo(Anio, Mes, Dia, Hora, Minuto, Segundo, Calculado) == false){
        return false;
    }


    Resultado.Segundos = Calculado;
    return true;

}

// Funcion que Convierte la Fecha a Texto "AAAA-MM-DD HH:MM:SS"
string Fecha::aTexto() const{


    tm* Local = localtime(&Segundos);
    Recursos::contarUsoCtime();

    // Verificacion de la Conversion
    if (Local == nullptr){
        return "0000-00-00 00:00:00";
    }


    return to_string(Local->tm_year + 1900) + "-" + dosDigitos(Local->tm_mon + 1) + "-" + dosDigitos(Local->tm_mday) + " " + dosDigitos(Local->tm_hour) + ":" + dosDigitos(Local->tm_min) + ":" + dosDigitos(Local->tm_sec);
}

// Funcion que Convierte la Fecha a Texto Corto "AAAA-MM-DD"
string Fecha::aTextoCorto() const{
    return aTexto().substr(0, 10);
}

// Funcion que Retorna el Mismo Dia a las 23:59:59
Fecha Fecha::finDelDia() const{

    //
    Fecha  Resultado(*this);
    time_t Calculado = 0;
    tm*    Local     = localtime(&Segundos);
    Recursos::contarUsoCtime();


    if (Local != nullptr && construirTiempo(Local->tm_year + 1900, Local->tm_mon + 1, Local->tm_mday, 23, 59, 59, Calculado)){   // Si se Pudo Construir
        Resultado.Segundos = Calculado;                                                                                          // Se Guarda el Nuevo Instante
    }


    return Resultado;

}

// Funcion que Retorna el Instante en Segundos
time_t Fecha::obtenerSegundos() const{
    return Segundos;
}

// Sobrecarga de los Operadores de Comparacion (Comparan Enteros: Muy Rapido)
bool Fecha::operator<(const Fecha& Otra) const{    return Segundos <  Otra.Segundos; }
bool Fecha::operator>(const Fecha& Otra) const{    return Segundos >  Otra.Segundos; }
bool Fecha::operator<=(const Fecha& Otra) const{   return Segundos <= Otra.Segundos; }
bool Fecha::operator>=(const Fecha& Otra) const{   return Segundos >= Otra.Segundos; }
bool Fecha::operator==(const Fecha& Otra) const{   return Segundos == Otra.Segundos; }
bool Fecha::operator!=(const Fecha& Otra) const{   return Segundos != Otra.Segundos; }

// Funcion Amiga que Permite Imprimir una Fecha con cout
ostream& operator<<(ostream& Salida, const Fecha& Objeto){
    Salida << Objeto.aTexto();
    return Salida;
}
