#include "Recursos.h"

// Inicializacion de los Atributos Estaticos de la Clase
long Recursos::Iteraciones      = 0;   // Arranca Sin Iteraciones
long Recursos::Memoria_Dinamica = 0;   // Arranca Sin Memoria Reservada
long Recursos::Memoria_Local    = 0;   // Arranca Sin Variables Locales Registradas
long Recursos::Usos_Ctime       = 0;   // Arranca Sin Llamadas a <ctime>
long Recursos::Usos_Aleatorio   = 0;   // Arranca Sin Numeros Aleatorios
long Recursos::Lineas_Archivo   = 0;   // Arranca Sin Lineas Procesadas

// Funcion que Suma una Iteracion al Contador Global
void Recursos::contarIteracion(){   // Se Declara la Funcion de Conteo de Iteraciones
    Iteraciones++;                  // Aumenta en Uno las Iteraciones de la Funcionalidad
}

// Funcion que Registra Memoria Pedida al Heap
void Recursos::reservar(long Bytes){   // Se Declara la Funcion de Reserva
    Memoria_Dinamica += Bytes;         // Suma los Bytes al Total Vivo en el Heap
}

// Funcion que Registra Memoria Devuelta al Heap
void Recursos::liberar(long Bytes){   // Se Declara la Funcion de Liberacion
    Memoria_Dinamica -= Bytes;        // Resta los Bytes del Total Vivo en el Heap
}

// Funcion que Registra las Variables Locales y Parametros por Valor
void Recursos::sumarMemoriaLocal(long Bytes){   // Se Declara la Funcion de Registro Local
    Memoria_Local += Bytes;                     // Suma el Tamano del Marco de la Funcion
}

// Funcion que Registra una Llamada a la Libreria <ctime>
void Recursos::contarUsoCtime(){   // Se Declara la Funcion de Registro de Tiempo
    Usos_Ctime++;                  // Aumenta en Uno las Llamadas a <ctime>
}

// Funcion que Registra un Numero Aleatorio Generado
void Recursos::contarUsoAleatorio(){   // Se Declara la Funcion de Registro Aleatorio
    Usos_Aleatorio++;                  // Aumenta en Uno los Numeros Generados
}

// Funcion que Registra una Linea Leida o Escrita en un Archivo
void Recursos::contarLineaArchivo(){   // Se Declara la Funcion de Registro de Archivos
    Lineas_Archivo++;                  // Aumenta en Uno las Lineas Procesadas
}

// Funcion que Calcula los Bytes que un String Ocupa en el Heap
long Recursos::memoriaCadena(const string& Texto){   // Se Declara la Funcion de Medicion de Cadenas

    // Declaracion de Variables Necesarias para la Medicion
    static const long Capacidad_Interna = static_cast<long>(string().capacity());   // Caracteres que Caben Dentro del Objeto (SSO)
    long Bytes_Extra = 0;                                                           // Bytes que el Texto Ocupa Fuera del Objeto

    // Verificacion del Tamano del Texto
    if (static_cast<long>(Texto.capacity()) > Capacidad_Interna){   // Si el Texto no Cabe Dentro del Objeto String
        Bytes_Extra = static_cast<long>(Texto.capacity()) + 1;      // Ocupa su Capacidad Mas el Caracter Nulo en el Heap
    }

    // Fin de la Funcion
    return Bytes_Extra;   // Se Retornan los Bytes Externos del Texto

}

// Funcion que Retorna las Iteraciones Acumuladas
long Recursos::obtenerIteraciones(){   // Se Declara la Funcion de Consulta
    return Iteraciones;                // Se Retorna el Contador de Iteraciones
}

// Funcion que Retorna los Bytes Vivos en el Heap
long Recursos::obtenerMemoriaDinamica(){   // Se Declara la Funcion de Consulta
    return Memoria_Dinamica;               // Se Retorna la Memoria Dinamica Actual
}

// Funcion que Retorna los Bytes Locales Registrados
long Recursos::obtenerMemoriaLocal(){   // Se Declara la Funcion de Consulta
    return Memoria_Local;               // Se Retorna la Memoria Local Registrada
}

// Funcion que Retorna la Memoria Total en Uso
long Recursos::obtenerMemoriaTotal(){        // Se Declara la Funcion de Consulta
    return Memoria_Dinamica + Memoria_Local;  // Se Retorna Heap + Variables Locales
}

// Funcion que Reinicia los Contadores al Iniciar una Funcionalidad
void Recursos::reiniciar(){   // Se Declara la Funcion de Reinicio

    // Reinicio de los Contadores Propios de Cada Funcionalidad
    Iteraciones    = 0;   // Las Iteraciones se Cuentan por Funcionalidad
    Memoria_Local  = 0;   // Las Variables Locales se Cuentan por Funcionalidad
    Usos_Ctime     = 0;   // Las Llamadas Externas se Cuentan por Funcionalidad
    Usos_Aleatorio = 0;   // Los Aleatorios se Cuentan por Funcionalidad
    Lineas_Archivo = 0;   // Las Lineas de Archivo se Cuentan por Funcionalidad

    // Nota: la Memoria Dinamica NO se Reinicia Porque Representa lo que Existe en el Heap

}

// Funcion que Agrega Separadores de Miles a un Numero
string Recursos::formatearNumero(long Numero){   // Se Declara la Funcion de Formato

    // Declaracion de Variables Necesarias para el Formato
    bool   Negativo  = (Numero < 0);                         // Indica si el Numero es Negativo
    string Digitos   = to_string(Negativo ? -Numero : Numero);   // Digitos del Numero sin Signo
    string Resultado = "";                                   // Texto Final con Separadores
    int    Contador  = 0;                                    // Digitos Copiados Desde la Derecha

    // Ciclo que Recorre los Digitos de Derecha a Izquierda
    for (int i = static_cast<int>(Digitos.size()) - 1 ; i >= 0 ; i--){   // Recorre Cada Digito
        Resultado = Digitos[i] + Resultado;                              // Agrega el Digito al Inicio
        Contador++;                                                      // Cuenta el Digito Copiado
        if (Contador % 3 == 0 && i > 0){                                 // Si se Completo un Grupo de Tres
            Resultado = '.' + Resultado;                                 // Agrega el Separador de Miles
        }
    }

    // Fin de la Funcion
    return (Negativo ? "-" : "") + Resultado;   // Se Retorna el Numero Formateado

}

// Funcion que Convierte Bytes a una Unidad Legible
string Recursos::formatearBytes(long Bytes){   // Se Declara la Funcion de Conversion

    // Declaracion de Variables Necesarias para la Conversion
    long   Divisor = 1;      // Divisor de la Unidad Elegida
    string Unidad  = " B";   // Nombre de la Unidad Elegida

    // Seleccion de la Unidad Mas Adecuada
    if (Bytes >= 1024L * 1024L){   // Si Supera un Megabyte
        Divisor = 1024L * 1024L;   // Se Divide en Megabytes
        Unidad  = " MB";           // Se Usa la Unidad MB
    }
    else if (Bytes >= 1024L){      // Si Supera un Kilobyte
        Divisor = 1024L;           // Se Divide en Kilobytes
        Unidad  = " KB";           // Se Usa la Unidad KB
    }

    // Calculo de la Parte Entera y Dos Decimales
    long Entero   = Bytes / Divisor;                     // Parte Entera de la Conversion
    long Decimal  = ((Bytes % Divisor) * 100) / Divisor;   // Dos Decimales de la Conversion
    string Texto  = formatearNumero(Entero);             // Parte Entera con Separadores

    if (Divisor > 1){                                          // Si Hubo Conversion de Unidad
        Texto += string(",") + (Decimal < 10 ? "0" : "") + to_string(Decimal);   // Se Agregan los Decimales
    }

    // Fin de la Funcion
    return Texto + Unidad;   // Se Retorna el Texto con su Unidad

}

// Funcion que Imprime el Reporte de Consumo de Recursos
void Recursos::mostrarReporte(const string& Funcionalidad){   // Se Declara la Funcion de Reporte

    // Declaracion de Variables Necesarias para el Reporte
    long   Total   = Memoria_Dinamica + Memoria_Local;   // Memoria Total en Este Momento
    string Externo = "";                                 // Texto con los Componentes Externos Usados

    // Construccion del Texto de Componentes Externos
    if (Usos_Ctime > 0){                                                      // Si se Uso <ctime>
        Externo += "<ctime> x" + formatearNumero(Usos_Ctime) + "  ";          // Se Agrega su Conteo
    }
    if (Usos_Aleatorio > 0){                                                  // Si se Uso <random>
        Externo += "<random> x" + formatearNumero(Usos_Aleatorio) + "  ";     // Se Agrega su Conteo
    }
    if (Lineas_Archivo > 0){                                                  // Si se Uso <fstream>
        Externo += "<fstream> " + formatearNumero(Lineas_Archivo) + " lineas";   // Se Agrega su Conteo
    }
    if (Externo == ""){            // Si no se Uso Ningun Componente Externo
        Externo = "Ninguno";       // Se Indica Explicitamente
    }

    // Impresion del Reporte en Pantalla
    cout << endl;
    cout << "  +------------------- CONSUMO DE RECURSOS -------------------+" << endl;
    cout << "    Funcionalidad        : " << Funcionalidad << endl;
    cout << "    Iteraciones          : " << formatearNumero(Iteraciones) << endl;
    cout << "    Memoria total en uso : " << formatearNumero(Total) << " bytes (" << formatearBytes(Total) << ")" << endl;
    cout << "      - Objetos y estructuras (Heap) : " << formatearBytes(Memoria_Dinamica) << endl;
    cout << "      - Variables locales/parametros : " << formatearBytes(Memoria_Local) << endl;
    cout << "    Componentes externos : " << Externo << endl;
    cout << "  +-----------------------------------------------------------+" << endl;

}
