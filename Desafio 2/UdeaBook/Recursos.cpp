#include "Recursos.h"

long Recursos::Iteraciones      = 0;
long Recursos::Memoria_Dinamica = 0;
long Recursos::Memoria_Local    = 0;
long Recursos::Usos_Ctime       = 0;
long Recursos::Usos_Aleatorio   = 0;
long Recursos::Lineas_Archivo   = 0;

// Funcion que Suma una Iteracion al Contador Global
void Recursos::contarIteracion(){
    Iteraciones++;
}

// Funcion que Registra Memoria Pedida al Heap
void Recursos::reservar(long Bytes){
    Memoria_Dinamica += Bytes;
}

// Funcion que Registra Memoria Devuelta al Heap
void Recursos::liberar(long Bytes){
    Memoria_Dinamica -= Bytes;
}

// Funcion que Registra las Variables Locales y Parametros por Valor
void Recursos::sumarMemoriaLocal(long Bytes){
    Memoria_Local += Bytes;
}

// Funcion que Registra una Llamada a la Libreria <ctime>
void Recursos::contarUsoCtime(){
    Usos_Ctime++;
}

// Funcion que Registra un Numero Aleatorio Generado
void Recursos::contarUsoAleatorio(){
    Usos_Aleatorio++;
}

// Funcion que Registra una Linea Leida o Escrita en un Archivo
void Recursos::contarLineaArchivo(){
    Lineas_Archivo++;
}

// Funcion que Calcula los Bytes que un String Ocupa en el Heap
long Recursos::memoriaCadena(const string& Texto){

    // Declaracion de Variables Necesarias para la Medicion
    static const long Capacidad_Interna = static_cast<long>(string().capacity());
    long Bytes_Extra = 0;

    // Verificacion del Tamano del Texto
    if (static_cast<long>(Texto.capacity()) > Capacidad_Interna){
        Bytes_Extra = static_cast<long>(Texto.capacity()) + 1;
    }

    // Fin de la Funcion
    return Bytes_Extra;

}

// Funcion que Retorna las Iteraciones Acumuladas
long Recursos::obtenerIteraciones(){
    return Iteraciones;
}

// Funcion que Retorna los Bytes Vivos en el Heap
long Recursos::obtenerMemoriaDinamica(){
    return Memoria_Dinamica;
}

// Funcion que Retorna los Bytes Locales Registrados
long Recursos::obtenerMemoriaLocal(){
    return Memoria_Local;
}

// Funcion que Retorna la Memoria Total en Uso
long Recursos::obtenerMemoriaTotal(){
    return Memoria_Dinamica + Memoria_Local;
}

// Funcion que Reinicia los Contadores al Iniciar una Funcionalidad
void Recursos::reiniciar(){

    // Reinicio de los Contadores
    Iteraciones    = 0;
    Memoria_Local  = 0;
    Usos_Ctime     = 0;
    Usos_Aleatorio = 0;
    Lineas_Archivo = 0;

}

// Funcion que Agrega Separadores de Miles a un Numero
string Recursos::formatearNumero(long Numero){

    bool   Negativo  = (Numero < 0);
    string Digitos   = to_string(Negativo ? -Numero : Numero);
    string Resultado = "";
    int    Contador  = 0;

    // Ciclo que Recorre los Digitos de Derecha a Izquierda
    for (int i = static_cast<int>(Digitos.size()) - 1 ; i >= 0 ; i--){
        Resultado = Digitos[i] + Resultado;
        Contador++;
        if (Contador % 3 == 0 && i > 0){
            Resultado = '.' + Resultado;
        }
    }

    return (Negativo ? "-" : "") + Resultado;

}

// Funcion que Convierte Bytes a una Unidad Legible
string Recursos::formatearBytes(long Bytes){

    long   Divisor = 1;
    string Unidad  = " B";


    if (Bytes >= 1024L * 1024L){
        Divisor = 1024L * 1024L;
        Unidad  = " MB";
    }
    else if (Bytes >= 1024L){
        Divisor = 1024L;
        Unidad  = " KB";
    }

    long Entero   = Bytes / Divisor;
    long Decimal  = ((Bytes % Divisor) * 100) / Divisor;
    string Texto  = formatearNumero(Entero);

    if (Divisor > 1){
        Texto += string(",") + (Decimal < 10 ? "0" : "") + to_string(Decimal);
    }

    return Texto + Unidad;
}

// Funcion que Imprime el Reporte de Consumo de Recursos
void Recursos::mostrarReporte(const string& Funcionalidad){

    long   Total   = Memoria_Dinamica + Memoria_Local;
    string Externo = "";

    // Construccion del Texto de Componentes Externos
    if (Usos_Ctime > 0){
        Externo += "<ctime> x" + formatearNumero(Usos_Ctime) + "  ";
    }
    if (Usos_Aleatorio > 0){
        Externo += "<random> x" + formatearNumero(Usos_Aleatorio) + "  ";
    }
    if (Lineas_Archivo > 0){
        Externo += "<fstream> " + formatearNumero(Lineas_Archivo) + " lineas";
    }
    if (Externo == ""){
        Externo = "Ninguno";
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
