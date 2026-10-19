#include <iostream>
#include <string>
#include "menuFun.h"
#include "Consola.h"
#include "UdeABookGestor.h"
#include "Usuario.h"

using namespace std;

// Funcion que Carga los Datos y Pide Otra Carpeta si no se Encuentran
bool prepararDatos(UdeABookGestor* Gestor){

    // Declaracion de Variables Necesarias
    string Carpeta = "";

    // Ciclo que Intenta Cargar Hasta Lograrlo o Hasta que el Usuario Desista
    while (Gestor->cargarDatos() == false){
        cout << endl;
        Consola::aviso("Asegurese de que la carpeta 'datos' este junto al ejecutable o en el directorio de trabajo.");
        Carpeta = Consola::leerLinea("Ruta de la carpeta de datos (0 = salir): ");
        if (Carpeta == "0"){
            return false;
        }
        Gestor->establecerCarpetaDatos(Carpeta);
    }


    Consola::pausar();
    return true;
}

// Funcion que Muestra el Menu Principal del Usuario Autenticado
void menuPrincipal(UdeABookGestor* Gestor){


    char     Opcion  = ' ';
    Usuario* Activo  = nullptr;

    // Ciclo del Menu (Termina al Cerrar Sesion)
    do {
        Activo = Gestor->obtenerSesionActiva();

        // Encabezado con las 3 Fotos de Perfil
        cout << endl;
        Consola::linea('=');
        Gestor->imprimirMejoresAmigos();
        Consola::linea('-');
        Consola::centrado("Bienvenido a UdeABook, " + Activo->obtenerNombreUsuario());
        Consola::linea('-');

        // Opciones del Menu (con Avisos de Pendientes)
        cout << "  Menu de opciones:" << endl << endl;
        cout << "    1. Administrar amistades";
        if (Activo->cantidadSolicitudes() > 0){ cout << "      (" << Activo->cantidadSolicitudes() << " solicitud(es) pendiente(s))"; }
        cout << endl;
        cout << "    2. Visitar perfil de un amigo" << endl;
        cout << "    3. Ver sugerencias de amistad" << endl;
        cout << "    4. Administrar mensajes";
        if (Activo->obtenerNoLeidos() > 0){ cout << "        (" << Activo->obtenerNoLeidos() << " mensaje(s) nuevo(s))"; }
        cout << endl << endl;
        cout << "    N. Alternar el grupo de fotos de perfil de amigos" << endl;
        cout << "    S. Cerrar sesion" << endl;
        Consola::linea('=');

        Opcion = Consola::leerOpcion("Su opcion: ");

        // Ejecucion de la Opcion Elegida
        switch (Opcion){
        case '1':                                          // III - Administrar Amistades
            menuAmistades(Gestor);
            break;
        case '2':                                          // IV - Visitar Perfil de un Amigo
            Gestor->visitarPerfilAmigo();
            Consola::pausar();
            break;
        case '3':                                          // V - Sugerencias de Amistad
            Gestor->verSugerenciasAmistad();
            Consola::pausar();
            break;
        case '4':                                          // VI - Administrar Mensajes
            menuMensajes(Gestor);
            break;
        case 'N':                                          // Alternar las 3 Fotos de Perfil
            Gestor->alternarMejoresAmigos();
            Consola::pausar();
            break;
        case 'S':                                          // Cerrar Sesion
            break;
        default:                                           // Opcion no Reconocida
            if (Consola::entradaCerrada() == false){
                Consola::error("Opcion no valida. Elija 1, 2, 3, 4, N o S.");
            }
            break;
        }

    } while (Opcion != 'S' && Consola::entradaCerrada() == false);

}

// Funcion que Muestra el Submenu de Amistades
void menuAmistades(UdeABookGestor* Gestor){

    // Declaracion de Variables Necesarias
    char     Opcion = ' ';
    Usuario* Activo = Gestor->obtenerSesionActiva();

    // Ciclo del Submenu
    while (Consola::entradaCerrada() == false){
        Consola::titulo("ADMINISTRAR AMISTADES");
        cout << "  Tienes " << Activo->cantidadAmigos() << " amigo(s) y "
             << Activo->cantidadSolicitudes() << " solicitud(es) pendiente(s)." << endl << endl;
        cout << "    1. Enviar solicitud de amistad" << endl;
        cout << "    2. Ver y aceptar solicitudes recibidas" << endl;
        cout << "    0. Volver al menu principal" << endl << endl;

        Opcion = Consola::leerOpcion("Su opcion: ");
        if (Opcion == '0'){
            break;
        }
        else if (Opcion == '1'){
            Gestor->enviarSolicitudAmistad();
            Consola::pausar();
        }
        else if (Opcion == '2'){
            Gestor->gestionarSolicitudesRecibidas();
            Consola::pausar();
        }
        else {
            Consola::error("Opcion no valida.");
        }
    }

}

// Funcion que Muestra el Submenu de Mensajes
void menuMensajes(UdeABookGestor* Gestor){

    // Declaracion de Variables Necesarias
    char     Opcion = ' ';
    Usuario* Activo = Gestor->obtenerSesionActiva();

    // Ciclo del Submenu
    while (Consola::entradaCerrada() == false){
        Consola::titulo("ADMINISTRAR MENSAJES");
        cout << "  Recibidos: " << Activo->cantidadRecibidos() << "   Enviados: " << Activo->cantidadEnviados()
             << "   Nuevos: " << Activo->obtenerNoLeidos() << endl << endl;
        cout << "    1. Enviar mensaje a un amigo" << endl;
        cout << "    2. Leer mensajes nuevos" << endl;
        cout << "    3. Buscar mensajes por rango de fecha" << endl;
        cout << "    0. Volver al menu principal" << endl << endl;

        Opcion = Consola::leerOpcion("Su opcion: ");
        if (Opcion == '0'){
            break;
        }
        else if (Opcion == '1'){                   // a. Enviar Mensaje
            Gestor->enviarMensaje();
            Consola::pausar();
        }
        else if (Opcion == '2'){                   // b. Leer Nuevos
            Gestor->leerMensajesNuevos();
            Consola::pausar();
        }
        else if (Opcion == '3'){                   // c. Buscar por Fecha
            Gestor->buscarMensajesPorFecha();
            Consola::pausar();
        }
        else {                                     // Opcion no Reconocida
            Consola::error("Opcion no valida.");
        }
    }

}
