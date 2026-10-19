#include "Consola.h"
#include <sstream>


bool Consola::Entrada_Cerrada = false;

// Funcion que Muestra la Portada del Programa
void Consola::mostrarPortada(){
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
void Consola::linea(char Caracter){
    cout << "  " << string(ANCHO, Caracter) << endl;
}

// Funcion que Dibuja un Titulo Enmarcado
void Consola::titulo(const string& Texto){
    cout << endl;
    linea('=');
    centrado(Texto);
    linea('=');
}

// Funcion que Escribe un Texto Centrado en el Ancho de la Consola
void Consola::centrado(const string& Texto){


    int Espacios = (ANCHO - static_cast<int>(Texto.size())) / 2;


    if (Espacios < 0){
        Espacios = 0;
    }


    cout << "  " << string(Espacios, ' ') << Texto << endl;

}

// Funcion que Muestra un Mensaje de Exito
void Consola::exito(const string& Texto){
    cout << "  [OK] " << Texto << endl;
}

// Funcion que Muestra un Mensaje de Error
void Consola::error(const string& Texto){
    cout << "  [!]  " << Texto << endl;
}

// Funcion que Muestra un Mensaje Informativo
void Consola::aviso(const string& Texto){
    cout << "  [i]  " << Texto << endl;
}

// Funcion que Ajusta un Texto a un Ancho Fijo (Para Tablas)
string Consola::ajustar(const string& Texto, int Ancho){


    if (static_cast<int>(Texto.size()) >= Ancho){
        return Texto.substr(0, Ancho - 1) + " ";
    }

    // Fin de la Funcion
    return Texto + string(Ancho - Texto.size(), ' ');

}

// Funcion que Quita los Espacios al Inicio y al Final de un Texto
string Consola::recortar(const string& Texto){


    int Inicio = 0;
    int Fin    = static_cast<int>(Texto.size()) - 1;


    while (Inicio <= Fin && (Texto[Inicio] == ' ' || Texto[Inicio] == '\t' || Texto[Inicio] == '\r')){
        Inicio++;
    }
    while (Fin >= Inicio && (Texto[Fin] == ' ' || Texto[Fin] == '\t' || Texto[Fin] == '\r')){
        Fin--;
    }


    return Texto.substr(Inicio, Fin - Inicio + 1);

}

// Funcion que Lee una Linea Completa del Teclado
string Consola::leerLinea(const string& Mensaje){


    string Texto = "";

    // Lectura de la Linea
    cout << "  " << Mensaje;
    if (!getline(cin, Texto)){
        Entrada_Cerrada = true;
        cout << endl;
        return "0";
    }


    return recortar(Texto);

}

// Funcion que Lee un Entero Dentro de un Rango
int Consola::leerEntero(const string& Mensaje, int Minimo, int Maximo){


    while (true){


        string       Texto  = leerLinea(Mensaje);
        stringstream Flujo(Texto);
        int          Valor  = 0;
        char         Sobra  = ' ';


        if (Entrada_Cerrada){
            return Minimo;
        }

        // Verificacion del Numero
        if ((Flujo >> Valor) && !(Flujo >> Sobra) && Valor >= Minimo && Valor <= Maximo){
            return Valor;
        }

        // Mensaje de Error
        error("Ingrese un numero entre " + to_string(Minimo) + " y " + to_string(Maximo) + ".");
    }

}

// Funcion que Lee una Opcion de una Sola Letra
char Consola::leerOpcion(const string& Mensaje){


    string Texto  = leerLinea(Mensaje);
    char   Opcion = '\0';


    if (Texto.size() == 1){
        Opcion = Texto[0];
        if (Opcion >= 'a' && Opcion <= 'z'){
            Opcion = Opcion - 'a' + 'A';
        }
    }
    else if (Texto.size() > 1){
        Opcion = '?';
    }

    // Fin de la Funcion
    return Opcion;
}

// Funcion que Espera a que el Usuario Presione ENTER
void Consola::pausar(){

    string Basura = "";


    if (Entrada_Cerrada){
        return;
    }
    cout << endl << "  Presione ENTER para continuar...";
    if (!getline(cin, Basura)){
        Entrada_Cerrada = true;
    }

}

// Funcion que Indica si la Entrada Estandar se Cerro
bool Consola::entradaCerrada(){
    return Entrada_Cerrada;
}
