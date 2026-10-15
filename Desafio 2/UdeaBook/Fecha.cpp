#include "Fecha.h"
#include "Recursos.h"   // Clase que Registra el Uso de Componentes Externos
#include <sstream>      // Libreria para Separar el Texto de la Fecha

// Constructor por Defecto: Toma la Fecha y Hora Actual del Sistema
Fecha::Fecha(){                    // Se Declara el Constructor por Defecto
    Segundos = time(nullptr);      // Se Consulta el Reloj del Sistema con <ctime>
    Recursos::contarUsoCtime();    // Se Registra el Uso del Componente Externo
}

// Constructor de Copia
Fecha::Fecha(const Fecha& Otra){   // Se Declara el Constructor de Copia
    Segundos = Otra.Segundos;      // Se Copia el Instante (Solo 8 Bytes)
}

// Sobrecarga del Operador de Asignacion
Fecha& Fecha::operator=(const Fecha& Otra){   // Se Declara el Operador =
    Segundos = Otra.Segundos;                 // Se Copia el Instante
    return *this;                             // Se Retorna el Objeto para Asignaciones Encadenadas
}

// Funcion que Rellena un Numero con un Cero a la Izquierda
string Fecha::dosDigitos(int Numero){                          // Se Declara la Funcion de Relleno
    return (Numero < 10 ? "0" : "") + to_string(Numero);       // Ej: 7 -> "07", 12 -> "12"
}

// Funcion que Valida los Componentes de una Fecha y los Convierte a time_t
bool Fecha::construirTiempo(int Anio, int Mes, int Dia, int Hora, int Minuto, int Segundo, time_t& Resultado){   // Se Declara la Funcion de Construccion

    // Validacion Basica de Rangos
    if (Anio < 1971 || Anio > 2099)     return false;   // Rango Seguro para time_t en Windows y Linux
    if (Mes < 1 || Mes > 12)            return false;   // Mes Invalido
    if (Dia < 1 || Dia > 31)            return false;   // Dia Invalido
    if (Hora < 0 || Hora > 23)          return false;   // Hora Invalida
    if (Minuto < 0 || Minuto > 59)      return false;   // Minuto Invalido
    if (Segundo < 0 || Segundo > 59)    return false;   // Segundo Invalido

    // Declaracion de la Estructura de Tiempo de la Libreria <ctime>
    tm Tiempo = {};   // Estructura Propia de <ctime> Inicializada en Ceros

    // Asignacion de los Componentes
    Tiempo.tm_year  = Anio - 1900;   // <ctime> Cuenta los Anios Desde 1900
    Tiempo.tm_mon   = Mes - 1;       // <ctime> Cuenta los Meses Desde 0
    Tiempo.tm_mday  = Dia;           // Dia del Mes
    Tiempo.tm_hour  = Hora;          // Hora del Dia
    Tiempo.tm_min   = Minuto;        // Minuto
    Tiempo.tm_sec   = Segundo;       // Segundo
    Tiempo.tm_isdst = -1;            // Que <ctime> Decida si Aplica Horario de Verano

    // Conversion a Segundos con mktime
    time_t Calculado = mktime(&Tiempo);   // Convierte y Normaliza la Fecha (Ej: 30 de Febrero -> 2 de Marzo)
    Recursos::contarUsoCtime();           // Se Registra el Uso del Componente Externo

    // Verificacion de que la Fecha Exista Realmente
    if (Calculado == static_cast<time_t>(-1)){   // Si mktime no Pudo Convertir
        return false;                            // La Fecha es Invalida
    }
    if (Tiempo.tm_mday != Dia || Tiempo.tm_mon != Mes - 1 || Tiempo.tm_year != Anio - 1900){   // Si mktime Tuvo que Corregirla
        return false;                                                                          // La Fecha no Existe (Ej: 2026-02-30)
    }

    // Fin de la Funcion
    Resultado = Calculado;   // Se Entrega el Instante Calculado
    return true;             // La Fecha es Valida

}

// Funcion que Lee una Fecha Desde un Texto "AAAA-MM-DD" o "AAAA-MM-DD HH:MM:SS"
bool Fecha::desdeTexto(const string& Texto, Fecha& Resultado){   // Se Declara la Funcion de Lectura

    // Declaracion de Variables Necesarias para la Lectura
    stringstream Flujo(Texto);                    // Flujo para Separar los Numeros del Texto
    int    Anio = 0, Mes = 0, Dia = 0;            // Componentes de la Fecha
    int    Hora = 0, Minuto = 0, Segundo = 0;     // Componentes de la Hora (0 si no se Escriben)
    char   Guion_1 = ' ', Guion_2 = ' ';          // Separadores de la Fecha (Deben ser '-')
    char   Puntos_1 = ' ', Puntos_2 = ' ';        // Separadores de la Hora (Deben ser ':')
    time_t Calculado = 0;                         // Instante Resultante

    // Lectura de la Parte de la Fecha
    Flujo >> Anio >> Guion_1 >> Mes >> Guion_2 >> Dia;                // Lee AAAA-MM-DD
    if (Flujo.fail() || Guion_1 != '-' || Guion_2 != '-'){            // Si el Formato no Coincide
        return false;                                                 // El Texto no es una Fecha
    }

    // Lectura Opcional de la Parte de la Hora
    if (Flujo.eof() == false){                                        // Si Queda Texto Despues del Dia
        Flujo >> ws;                                                  // Se Saltan los Espacios
        if (Flujo.eof() == false){                                    // Si Despues de los Espacios Hay Algo
            Flujo >> Hora >> Puntos_1 >> Minuto >> Puntos_2 >> Segundo;   // Lee HH:MM:SS
            if (Flujo.fail() || Puntos_1 != ':' || Puntos_2 != ':'){      // Si el Formato no Coincide
                return false;                                             // El Texto no es Valido
            }
            if (Flujo.eof() == false){                                // Si Queda Algo Despues de la Hora
                Flujo >> ws;                                          // Se Saltan los Espacios
                if (Flujo.eof() == false){                            // Si Hay Basura al Final
                    return false;                                     // El Texto no es Valido
                }
            }
        }
    }

    // Validacion y Conversion de los Componentes
    if (construirTiempo(Anio, Mes, Dia, Hora, Minuto, Segundo, Calculado) == false){   // Si la Fecha no Existe
        return false;                                                                  // Se Rechaza
    }

    // Fin de la Funcion
    Resultado.Segundos = Calculado;   // Se Guarda el Instante en el Objeto de Salida
    return true;                      // La Lectura fue Exitosa

}

// Funcion que Convierte la Fecha a Texto "AAAA-MM-DD HH:MM:SS"
string Fecha::aTexto() const{   // Se Declara la Funcion de Conversion

    // Conversion del Instante a Componentes Locales
    tm* Local = localtime(&Segundos);   // Convierte los Segundos a Fecha Local con <ctime>
    Recursos::contarUsoCtime();         // Se Registra el Uso del Componente Externo

    // Verificacion de la Conversion
    if (Local == nullptr){             // Si la Conversion Fallo
        return "0000-00-00 00:00:00";  // Se Retorna una Fecha Vacia
    }

    // Fin de la Funcion
    return to_string(Local->tm_year + 1900) + "-" + dosDigitos(Local->tm_mon + 1) + "-" + dosDigitos(Local->tm_mday) + " " +
           dosDigitos(Local->tm_hour) + ":" + dosDigitos(Local->tm_min) + ":" + dosDigitos(Local->tm_sec);   // Se Arma el Texto

}

// Funcion que Convierte la Fecha a Texto Corto "AAAA-MM-DD"
string Fecha::aTextoCorto() const{   // Se Declara la Funcion de Conversion Corta
    return aTexto().substr(0, 10);   // Se Toman Solo los Primeros 10 Caracteres
}

// Funcion que Retorna el Mismo Dia a las 23:59:59 (Para Rangos Inclusivos)
Fecha Fecha::finDelDia() const{   // Se Declara la Funcion de Fin del Dia

    // Declaracion de Variables Necesarias
    Fecha  Resultado(*this);            // Copia de la Fecha Actual
    time_t Calculado = 0;               // Instante Resultante
    tm*    Local     = localtime(&Segundos);   // Componentes Locales de la Fecha
    Recursos::contarUsoCtime();         // Se Registra el Uso del Componente Externo

    // Construccion del Ultimo Segundo del Dia
    if (Local != nullptr && construirTiempo(Local->tm_year + 1900, Local->tm_mon + 1, Local->tm_mday, 23, 59, 59, Calculado)){   // Si se Pudo Construir
        Resultado.Segundos = Calculado;                                                                                          // Se Guarda el Nuevo Instante
    }

    // Fin de la Funcion
    return Resultado;   // Se Retorna la Fecha al Final del Dia

}

// Funcion que Retorna el Instante en Segundos
time_t Fecha::obtenerSegundos() const{   // Se Declara la Funcion de Consulta
    return Segundos;                     // Se Retornan los Segundos
}

// Sobrecarga de los Operadores de Comparacion (Comparan Enteros: Muy Rapido)
bool Fecha::operator<(const Fecha& Otra) const{    return Segundos <  Otra.Segundos; }   // Es Mas Antigua
bool Fecha::operator>(const Fecha& Otra) const{    return Segundos >  Otra.Segundos; }   // Es Mas Reciente
bool Fecha::operator<=(const Fecha& Otra) const{   return Segundos <= Otra.Segundos; }   // Es Mas Antigua o Igual
bool Fecha::operator>=(const Fecha& Otra) const{   return Segundos >= Otra.Segundos; }   // Es Mas Reciente o Igual
bool Fecha::operator==(const Fecha& Otra) const{   return Segundos == Otra.Segundos; }   // Es el Mismo Instante
bool Fecha::operator!=(const Fecha& Otra) const{   return Segundos != Otra.Segundos; }   // Es Otro Instante

// Funcion Amiga que Permite Imprimir una Fecha con cout
ostream& operator<<(ostream& Salida, const Fecha& Objeto){   // Se Declara el Operador <<
    Salida << Objeto.aTexto();                               // Se Escribe la Fecha en Formato Texto
    return Salida;                                           // Se Retorna el Flujo para Encadenar
}
