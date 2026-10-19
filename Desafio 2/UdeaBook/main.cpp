// Librerias Necesarias
#include <iostream>
#include <string>
#include "Configuracion.h"
#include "Consola.h"
#include "Recursos.h"
#include "UdeABookGestor.h"
#include "menuFun.h"

using namespace std;


int main(){

    // Declaracion de Variables Necesarias
    UdeABookGestor* Gestor     = nullptr;
    string          Nombre     = "";
    string          Contrasena = "";


    Consola::mostrarPortada();
    Gestor = new UdeABookGestor(RUTA_DATOS_DEFECTO);
    Recursos::reservar(sizeof(UdeABookGestor));

    // I - Carga de Datos (Antes de Mostrar Cualquier Menu)
    if (prepararDatos(Gestor)){


        while (Consola::entradaCerrada() == false){
            Consola::titulo("INICIO DE SESION");
            cout << "  Usuarios registrados en la plataforma: " << Gestor->cantidadUsuarios() << endl << endl;

            Nombre = Consola::leerLinea("Usuario (0 = salir del programa): ");   // Se Pide el Usuario
            if (Nombre == "0"){   // Si Quiere Salir
                break;            // Fin del Programa
            }
            if (Nombre == ""){
                Consola::error("Escriba su nombre de usuario.");
                continue;
            }
            Contrasena = Consola::leerLinea("Contrasena: ");    // Se Pide la Contrasena

            // II - Ingreso a la Plataforma
            if (Gestor->iniciarSesion(Nombre, Contrasena)){
                Consola::pausar();
                menuPrincipal(Gestor);
                Gestor->cerrarSesion();
                Consola::exito("Sesion cerrada. Hasta pronto!");
                Gestor->guardarDatos();
            }
            Consola::pausar();
        }
    }

    // Guardado Final y Liberacion de la Memoria
    Gestor->guardarDatos();
    delete Gestor;
    Recursos::liberar(sizeof(UdeABookGestor));


    cout << endl;
    Consola::linea('=');
    Consola::centrado("Gracias por usar UdeABook. Memoria final en uso: " + to_string(Recursos::obtenerMemoriaDinamica()) + " bytes");
    Consola::linea('=');


    return 0;

}
