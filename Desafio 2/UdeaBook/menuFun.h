#ifndef MENUFUN_H
#define MENUFUN_H

// Modulo con las Funciones de los Menus del Programa (Consola)

// Clases Necesarias
class UdeABookGestor;   // Clase Controladora del Sistema

// Declaracion de las Funciones del Menu
bool prepararDatos(UdeABookGestor* Gestor);    // Carga los Datos (Pide Otra Carpeta si no los Encuentra)
void menuPrincipal(UdeABookGestor* Gestor);    // Menu del Usuario Autenticado
void menuAmistades(UdeABookGestor* Gestor);    // Submenu III - Administrar Amistades
void menuMensajes(UdeABookGestor* Gestor);     // Submenu VI - Administrar Mensajes

#endif // MENUFUN_H
