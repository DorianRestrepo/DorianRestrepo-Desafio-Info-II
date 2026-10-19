#ifndef UDEABOOKGESTOR_H
#define UDEABOOKGESTOR_H

// Librerias Necesarias
#include <string>
#include <random>
#include "ListaDinamica.h"
#include "Configuracion.h"
#include "Fecha.h"

using namespace std;

class Usuario;
class Album;
class Foto;
class Mensaje;


class UdeABookGestor {

private:


    ListaDinamica<Usuario*> Usuarios;
    ListaDinamica<Usuario*> Usuarios_Ordenados;
    Usuario*                Sesion_Activa;        // Usuario que Inicio Sesion
    bool**                  Matriz_Adyacencia;
    int                     Dimension_Matriz;
    string                  Carpeta_Datos;

    // Datos de los mejores amigos de la sesion activa
    double* Pesos_Amigos;
    int*    Comunes_Amigos;
    int     Cantidad_Pesos;
    int     Mostrados[FOTOS_MEJORES_AMIGOS];
    int     Cantidad_Mostrados;

    // Generador de numeros aleatorios
    mt19937 Generador;

    // Banderas de cambios pendientes por guardar
    bool Cambios_Amistades;
    bool Cambios_Mensajes;
    bool Cambios_Likes;

    // Funciones privadas de carga
    bool cargarUsuarios();
    void cargarAmistades(int& Total, int& Invalidas);
    void cargarSolicitudes(int& Total, int& Invalidas);
    void cargarMensajes(int& Total, int& Invalidas);
    void cargarAlbumes(int& Total_Albumes, int& Total_Fotos, int& Invalidas);

    // Funciones privadas de guardado
    bool guardarAmistades();
    bool guardarSolicitudes();
    bool guardarMensajes();
    bool guardarAlbumes();

    // Funciones privadas de estructuras
    void crearMatriz(int Dimension);
    void liberarMatriz();
    bool insertarOrdenado(Usuario* Nuevo);
    void liberarDatos();
    void liberarPesos();

    // Funciones privadas de logica
    bool     sonAmigos(Usuario* A, Usuario* B) const;
    int      contarAmigosEnComun(Usuario* A, Usuario* B) const;
    void     calcularPesosMejoresAmigos();
    void     elegirMejoresAmigos(bool Alternar);
    int      elegirAmigoPonderado(const int* Estado, bool Bloquear_Previos);
    bool     puedeEnviarSolicitud(Usuario* Destino, bool Mostrar_Motivo) const;
    Usuario* pedirAmigo(const string& Mensaje);
    void     listarAmigos() const;
    void     carruselFotos(Album* Album_Actual);

    // Funciones de texto
    static int  dividir(const string& Linea, char Separador, string* Campos, int Maximo);
    static bool aEntero(const string& Texto, int& Valor);
    static void limpiarLinea(string& Linea);

public:

    // Constructor y Destructor
    UdeABookGestor(const string& Carpeta_Datos);
    ~UdeABookGestor();


    UdeABookGestor(const UdeABookGestor& Otro) = delete;
    UdeABookGestor& operator=(const UdeABookGestor& Otro) = delete;

    // Funciones de datos (I - Carga / Actualizacion)
    void          establecerCarpetaDatos(const string& Carpeta);
    const string& obtenerCarpetaDatos() const;
    bool          cargarDatos();
    bool          guardarDatos();
    bool          hayCambiosPendientes() const;

    // Funciones de sesion (II - Ingreso y Mejores Amigos)
    bool     iniciarSesion(const string& Nombre_Usuario, const string& Contrasena);
    void     cerrarSesion();
    Usuario* obtenerSesionActiva() const;
    int      cantidadUsuarios() const;
    void     alternarMejoresAmigos();
    void     imprimirMejoresAmigos() const;

    // Busquedas
    Usuario* buscarUsuario(const string& Nombre_Usuario) const;
    Usuario* buscarUsuario(int Id) const;

    // Funcionalidad III - Administrar Amistades
    void enviarSolicitudAmistad();          // Enviar solicitud
    void gestionarSolicitudesRecibidas();   // Aceptar (o Rechazar) solicitud

    // Funcionalidad IV - Visitar perfil
    void visitarPerfilAmigo();

    // Funcionalidad V - Sugerencias de Amistad
    void calcularSugerenciasAmistad(int K, int* Comunes_Resultado);
    void verSugerenciasAmistad();

    // Funcionalidad VI - Administrar mensajes
    void enviarMensaje();            // Enviar Mensaje a un Amigo
    void leerMensajesNuevos();       // leer los Mensajes no Leidos
    void buscarMensajesPorFecha();   // buscar por Rango de Fechas

    // Funcionalidad VII - Medicion de recursos
    void medirConsumoRecursos(const string& Funcionalidad) const;
};

#endif // UDEABOOKGESTOR_H
