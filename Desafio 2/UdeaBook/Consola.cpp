#include "Consola.h"
#include <sstream>   // Libreria para Convertir Texto a Numero de Forma Segura

// Inicializacion del Atributo Estatico
bool Consola::Entrada_Cerrada = false;   // Al Iniciar la Entrada Esta Abierta

// Funcion que Muestra la Portada del Programa
void Consola::mostrarPortada(){   // Se Declara la Funcion de Portada
    cout << endl;
    linea('=');
    cout << endl;
    centrado("U d e A B o o k");
    centrado("Amigos para siempre");
    cout << endl;
    centrado("Informatica II - Universidad de Antioquia");
    cout << endl;
    linea('=');
}

// Funcion que Dibuja una Linea Horizontal
void Consola::linea(char Caracter){                 // Se Declara la Funcion de Linea
    cout << "  " << string(ANCHO, Caracter) << endl;   // Se Dibujan ANCHO Caracteres Seguidos
}

// Funcion que Dibuja un Titulo Enmarcado
void Consola::titulo(const string& Texto){   // Se Declara la Funcion de Titulo
    cout << endl;                            // Se Deja un Espacio Antes del Titulo
    linea('=');                              // Linea Superior
    centrado(Texto);                         // Texto Centrado
    linea('=');                              // Linea Inferior
}

// Funcion que Escribe un Texto Centrado en el Ancho de la Consola
void Consola::centrado(const string& Texto){   // Se Declara la Funcion de Centrado

    // Declaracion de Variables Necesarias para Centrar
    int Espacios = (ANCHO - static_cast<int>(Texto.size())) / 2;   // Espacios a la Izquierda

    // Correccion por si el Texto es Mas Largo que el Ancho
    if (Espacios < 0){   // Si no Cabe en el Ancho
        Espacios = 0;    // Se Escribe sin Espacios
    }

    // Impresion del Texto
    cout << "  " << string(Espacios, ' ') << Texto << endl;   // Se Escribe con el Margen Calculado

}

// Funcion que Muestra un Mensaje de Exito
void Consola::exito(const string& Texto){    // Se Declara la Funcion de Exito
    cout << "  [OK] " << Texto << endl;      // Se Escribe con su Prefijo
}

// Funcion que Muestra un Mensaje de Error
void Consola::error(const string& Texto){    // Se Declara la Funcion de Error
    cout << "  [!]  " << Texto << endl;      // Se Escribe con su Prefijo
}

// Funcion que Muestra un Mensaje Informativo
void Consola::aviso(const string& Texto){    // Se Declara la Funcion de Aviso
    cout << "  [i]  " << Texto << endl;      // Se Escribe con su Prefijo
}

// Funcion que Ajusta un Texto a un Ancho Fijo (Para Tablas)
string Consola::ajustar(const string& Texto, int Ancho){   // Se Declara la Funcion de Ajuste

    // Verificacion del Largo del Texto
    if (static_cast<int>(Texto.size()) >= Ancho){            // Si el Texto Ocupa Todo el Ancho
        return Texto.substr(0, Ancho - 1) + " ";             // Se Recorta Dejando un Espacio
    }

    // Fin de la Funcion
    return Texto + string(Ancho - Texto.size(), ' ');        // Se Rellena con Espacios a la Derecha

}

// Funcion que Quita los Espacios al Inicio y al Final de un Texto
string Consola::recortar(const string& Texto){   // Se Declara la Funcion de Recorte

    // Declaracion de Variables Necesarias para el Recorte
    int Inicio = 0;                                   // Primer Caracter Util
    int Fin    = static_cast<int>(Texto.size()) - 1;   // Ultimo Caracter Util

    // Ciclos que Saltan los Espacios de los Extremos
    while (Inicio <= Fin && (Texto[Inicio] == ' ' || Texto[Inicio] == '\t' || Texto[Inicio] == '\r')){   // Espacios al Inicio
        Inicio++;                                                                                        // Avanza el Inicio
    }
    while (Fin >= Inicio && (Texto[Fin] == ' ' || Texto[Fin] == '\t' || Texto[Fin] == '\r')){            // Espacios al Final
        Fin--;                                                                                           // Retrocede el Fin
    }

    // Fin de la Funcion
    return Texto.substr(Inicio, Fin - Inicio + 1);   // Se Retorna el Texto sin Espacios Sobrantes

}

// Funcion que Lee una Linea Completa del Teclado
string Consola::leerLinea(const string& Mensaje){   // Se Declara la Funcion de Lectura

    // Declaracion de Variables Necesarias para la Lectura
    string Texto = "";   // Texto Escrito por el Usuario

    // Lectura de la Linea
    cout << "  " << Mensaje;              // Se Muestra el Mensaje de Solicitud
    if (!getline(cin, Texto)){            // Si ya no Hay Entrada Disponible
        Entrada_Cerrada = true;           // Se Marca la Entrada como Cerrada
        cout << endl;                     // Se Salta de Linea
        return "0";                       // Se Retorna "0" (Opcion Universal de Volver/Salir)
    }

    // Fin de la Funcion
    return recortar(Texto);   // Se Retorna el Texto sin Espacios Sobrantes

}

// Funcion que Lee un Entero Dentro de un Rango
int Consola::leerEntero(const string& Mensaje, int Minimo, int Maximo){   // Se Declara la Funcion de Lectura de Enteros

    // Ciclo que se Repite Hasta Recibir un Numero Valido
    while (true){

        // Declaracion de Variables Necesarias para la Lectura
        string       Texto  = leerLinea(Mensaje);   // Texto Escrito por el Usuario
        stringstream Flujo(Texto);                  // Flujo para Convertir el Texto
        int          Valor  = 0;                    // Numero Convertido
        char         Sobra  = ' ';                  // Caracter que Sobra Despues del Numero

        // Verificacion de Entrada Cerrada
        if (Entrada_Cerrada){   // Si ya no Hay Entrada
            return Minimo;      // Se Retorna el Minimo (Siempre es la Opcion de Volver)
        }

        // Verificacion del Numero
        if ((Flujo >> Valor) && !(Flujo >> Sobra) && Valor >= Minimo && Valor <= Maximo){   // Si es un Entero Limpio y en Rango
            return Valor;                                                                  // Se Retorna el Numero
        }

        // Mensaje de Error
        error("Ingrese un numero entre " + to_string(Minimo) + " y " + to_string(Maximo) + ".");
    }

}

// Funcion que Lee una Opcion de una Sola Letra (en Mayuscula)
char Consola::leerOpcion(const string& Mensaje){   // Se Declara la Funcion de Lectura de Opciones

    // Declaracion de Variables Necesarias para la Lectura
    string Texto  = leerLinea(Mensaje);   // Texto Escrito por el Usuario
    char   Opcion = '\0';                 // Opcion Resultante ('\0' si Solo se Presiono ENTER)

    // Verificacion del Texto
    if (Texto.size() == 1){                     // Si se Escribio Exactamente un Caracter
        Opcion = Texto[0];                      // Se Toma el Caracter
        if (Opcion >= 'a' && Opcion <= 'z'){    // Si es una Letra Minuscula
            Opcion = Opcion - 'a' + 'A';        // Se Convierte a Mayuscula
        }
    }
    else if (Texto.size() > 1){   // Si se Escribio Mas de un Caracter
        Opcion = '?';             // Se Marca como Opcion Invalida
    }

    // Fin de la Funcion
    return Opcion;   // Se Retorna la Opcion Leida

}

// Funcion que Espera a que el Usuario Presione ENTER
void Consola::pausar(){   // Se Declara la Funcion de Pausa

    // Declaracion de Variables Necesarias
    string Basura = "";   // Texto que se Descarta

    // Espera del ENTER
    if (Entrada_Cerrada){   // Si ya no Hay Entrada
        return;             // No se Espera Nada
    }
    cout << endl << "  Presione ENTER para continuar...";   // Se Muestra la Indicacion
    if (!getline(cin, Basura)){                             // Si la Entrada se Cerro
        Entrada_Cerrada = true;                             // Se Marca como Cerrada
    }

}

// Funcion que Indica si la Entrada Estandar se Cerro
bool Consola::entradaCerrada(){   // Se Declara la Funcion de Consulta
    return Entrada_Cerrada;       // Se Retorna el Estado de la Entrada
}
