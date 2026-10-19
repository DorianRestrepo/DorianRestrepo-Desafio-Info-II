#include "UdeABookGestor.h"
#include "Usuario.h"
#include "Album.h"
#include "Foto.h"
#include "Mensaje.h"
#include "Recursos.h"
#include "Consola.h"
#include <fstream>
#include <sstream>


// Constructor con la Carpeta de Datos
UdeABookGestor::UdeABookGestor(const string& Carpeta_Datos)
    : Generador(static_cast<unsigned int>(time(nullptr))){


    Sesion_Activa      = nullptr;
    Matriz_Adyacencia  = nullptr;
    Dimension_Matriz   = 0;
    Pesos_Amigos       = nullptr;
    Comunes_Amigos     = nullptr;
    Cantidad_Pesos     = 0;
    Cantidad_Mostrados = 0;
    Cambios_Amistades  = false;
    Cambios_Mensajes   = false;
    Cambios_Likes      = false;
    this->Carpeta_Datos = "";

    // Registro de Uso de <ctime> y Asignacion de la Carpeta
    Recursos::contarUsoCtime();
    establecerCarpetaDatos(Carpeta_Datos);

}

// Destructor: Libera lo que el Gestor Reservo
UdeABookGestor::~UdeABookGestor(){
    liberarDatos();
    Recursos::liberar(Recursos::memoriaCadena(Carpeta_Datos));
}

// Funcion que Cambia la Carpeta de los Archivos de Datos
void UdeABookGestor::establecerCarpetaDatos(const string& Carpeta){

    // Descuento del Texto Anterior
    Recursos::liberar(Recursos::memoriaCadena(Carpeta_Datos));

    // Normalizacion de la Ruta
    Carpeta_Datos = Carpeta;
    for (size_t i = 0 ; i < Carpeta_Datos.size() ; i++){
        if (Carpeta_Datos[i] == '\\'){
            Carpeta_Datos[i] = '/';
        }
    }
    if (Carpeta_Datos != "" && Carpeta_Datos[Carpeta_Datos.size() - 1] != '/'){
        Carpeta_Datos += '/';
    }

    // Registro del Texto Nuevo
    Recursos::reservar(Recursos::memoriaCadena(Carpeta_Datos));

}

// Funcion que Retorna la Carpeta de Datos
const string& UdeABookGestor::obtenerCarpetaDatos() const{
    return Carpeta_Datos;
}

// Funcion que Indica si Hay Cambios por Guardar
bool UdeABookGestor::hayCambiosPendientes() const{
    return Cambios_Amistades || Cambios_Mensajes || Cambios_Likes;
}

// Funcion que Libera Todos los Datos Cargados
void UdeABookGestor::liberarDatos(){


    liberarPesos();
    Sesion_Activa = nullptr;


    for (int i = 0 ; i < Usuarios.obtenerTamano() ; i++){
        delete Usuarios[i];
        Recursos::liberar(sizeof(Usuario));
    }


    Usuarios.vaciar();
    Usuarios_Ordenados.vaciar();
    liberarMatriz();

}


// Funcion que Separa una Linea en Campos
int UdeABookGestor::dividir(const string& Linea, char Separador, string* Campos, int Maximo){

    // Declaracion de Variables Necesarias
    stringstream Flujo(Linea);   // Flujo para Recorrer la Linea
    int          Cantidad = 0;   // Campos Leidos

    // Ciclo que Lee los Campos Separados
    while (Cantidad < Maximo - 1 && getline(Flujo, Campos[Cantidad], Separador)){
        Cantidad++;
    }

    // Lectura del Ultimo Campo
    if (Cantidad == Maximo - 1 && getline(Flujo, Campos[Cantidad])){
        Cantidad++;
    }

    // Fin de la Funcion
    return Cantidad;   // Se Retorna Cuantos Campos se Leyeron

}

// Funcion que Convierte un Texto a Entero de Forma Segura
bool UdeABookGestor::aEntero(const string& Texto, int& Valor){   // Se Declara la Funcion de Conversion
    stringstream Flujo(Texto);                                   // Flujo para Convertir el Texto
    char         Sobra = ' ';                                    // Caracter que Sobra Despues del Numero
    return (Flujo >> Valor) && !(Flujo >> Sobra);                // Es Valido si es un Numero Limpio
}

// Funcion que Quita el '\r' que Dejan los Archivos Creados en Windows
void UdeABookGestor::limpiarLinea(string& Linea){                       // Se Declara la Funcion de Limpieza
    if (Linea.empty() == false && Linea[Linea.size() - 1] == '\r'){     // Si la Linea Termina en '\r'
        Linea.erase(Linea.size() - 1);                                  // Se Elimina ese Caracter
    }
}

// ============================================================================
//                        ESTRUCTURAS: MATRIZ E INDICE
// ============================================================================

// Funcion que Reserva la Matriz de Adyacencia N x N en un Solo Bloque
void UdeABookGestor::crearMatriz(int Dimension){   // Se Declara la Funcion de Creacion

    // Declaracion de Variables Necesarias
    long  Total_Celdas = static_cast<long>(Dimension) * Dimension;   // Cantidad Total de Celdas (N x N)
    bool* Bloque       = nullptr;                                    // Bloque Continuo con Todas las Celdas

    // Verificacion de la Dimension
    liberarMatriz();            // Se Libera una Matriz Anterior si Existia
    if (Dimension <= 0){        // Si no Hay Usuarios
        return;                 // No se Crea Nada
    }

    // Reserva de Memoria
    Matriz_Adyacencia = new bool*[Dimension];                          // Arreglo de Punteros a las Filas
    Recursos::reservar(static_cast<long>(Dimension) * sizeof(bool*));  // Se Registra el Arreglo de Filas
    Bloque = new bool[Total_Celdas];                                   // Un Solo new para Todas las Celdas
    Recursos::reservar(Total_Celdas * sizeof(bool));                   // Se Registra el Bloque de Celdas
    Dimension_Matriz = Dimension;                                      // Se Guarda la Dimension

    // Ciclo que Apunta Cada Fila a su Parte del Bloque
    for (int i = 0 ; i < Dimension ; i++){                             // Recorre Cada Fila
        Matriz_Adyacencia[i] = Bloque + static_cast<long>(i) * Dimension;   // La Fila i Empieza en i*N
        Recursos::contarIteracion();                                   // Registra la Iteracion
    }

    // Ciclo que Inicializa Todas las Celdas en false (Nadie es Amigo de Nadie)
    for (long k = 0 ; k < Total_Celdas ; k++){   // Recorre Cada Celda
        Bloque[k] = false;                       // Sin Amistad
        Recursos::contarIteracion();             // Registra la Iteracion
    }

}

// Funcion que Libera la Matriz de Adyacencia
void UdeABookGestor::liberarMatriz(){                                                    // Se Declara la Funcion de Liberacion
    if (Matriz_Adyacencia != nullptr){                                                   // Si Existe una Matriz
        delete[] Matriz_Adyacencia[0];                                                   // Se Libera el Bloque de Celdas
        Recursos::liberar(static_cast<long>(Dimension_Matriz) * Dimension_Matriz * sizeof(bool));   // Se Registra
        delete[] Matriz_Adyacencia;                                                      // Se Libera el Arreglo de Filas
        Recursos::liberar(static_cast<long>(Dimension_Matriz) * sizeof(bool*));          // Se Registra
    }
    Matriz_Adyacencia = nullptr;   // La Matriz Queda Vacia
    Dimension_Matriz  = 0;         // Sin Dimension
}

// Funcion que Inserta un Usuario en el Indice Alfabetico (Busqueda Binaria de la Posicion)
bool UdeABookGestor::insertarOrdenado(Usuario* Nuevo){   // Se Declara la Funcion de Insercion Ordenada

    // Declaracion de Variables Necesarias
    int Izquierda = 0;                                    // Limite Izquierdo de la Busqueda
    int Derecha   = Usuarios_Ordenados.obtenerTamano();   // Limite Derecho (Excluido)
    int Medio     = 0;                                    // Posicion Central

    // Ciclo de Busqueda Binaria de la Posicion de Insercion
    while (Izquierda < Derecha){                           // Mientras Haya Rango
        Recursos::contarIteracion();                       // Registra la Iteracion
        Medio = Izquierda + (Derecha - Izquierda) / 2;     // Centro del Rango
        if (*Usuarios_Ordenados[Medio] < *Nuevo){          // Usa la Sobrecarga < de Usuario
            Izquierda = Medio + 1;                         // El Nuevo va Despues
        }
        else {                                             // El Nuevo va Antes o Aqui
            Derecha = Medio;                               // Se Reduce el Rango
        }
    }

    // Verificacion de Nombre Repetido
    if (Izquierda < Usuarios_Ordenados.obtenerTamano() && *Usuarios_Ordenados[Izquierda] == *Nuevo){   // Usa la Sobrecarga ==
        return false;                                                                                  // El Nombre ya Existe
    }

    // Fin de la Funcion (si el Archivo Viene Ordenado, Siempre se Inserta al Final en O(1))
    Usuarios_Ordenados.insertar(Izquierda, Nuevo);   // Se Inserta en su Posicion Alfabetica
    return true;                                     // Insercion Exitosa

}

// Funcion que Busca un Usuario por Nombre (Busqueda Binaria O(log N))
Usuario* UdeABookGestor::buscarUsuario(const string& Nombre_Usuario) const{   // Se Declara la Busqueda por Nombre

    // Declaracion de Variables Necesarias
    int Izquierda = 0;                                        // Limite Izquierdo
    int Derecha   = Usuarios_Ordenados.obtenerTamano() - 1;   // Limite Derecho (Incluido)
    int Medio     = 0;                                        // Posicion Central

    // Ciclo de Busqueda Binaria
    while (Izquierda <= Derecha){                                                // Mientras Haya Rango
        Recursos::contarIteracion();                                             // Registra la Iteracion
        Medio = Izquierda + (Derecha - Izquierda) / 2;                           // Centro del Rango
        const string& Actual = Usuarios_Ordenados[Medio]->obtenerNombreUsuario();   // Nombre Central (por Referencia)
        if (Actual == Nombre_Usuario){                                           // Si Coincide
            return Usuarios_Ordenados[Medio];                                    // Se Encontro el Usuario
        }
        if (Actual < Nombre_Usuario){                                            // Si el Buscado va Despues
            Izquierda = Medio + 1;                                               // Se Busca a la Derecha
        }
        else {                                                                   // Si el Buscado va Antes
            Derecha = Medio - 1;                                                 // Se Busca a la Izquierda
        }
    }

    // Fin de la Funcion
    return nullptr;   // El Usuario no Existe

}

// Funcion que Busca un Usuario por Id (Acceso Directo O(1))
Usuario* UdeABookGestor::buscarUsuario(int Id) const{   // Se Declara la Busqueda por Id
    return Usuarios.obtener(Id);                        // Retorna nullptr si el Id no Existe
}

// Funcion que Retorna la Cantidad de Usuarios
int UdeABookGestor::cantidadUsuarios() const{   // Se Declara la Funcion de Consulta
    return Usuarios.obtenerTamano();            // Se Retorna N
}

// Funcion que Consulta si Dos Usuarios son Amigos (O(1) con la Matriz)
bool UdeABookGestor::sonAmigos(Usuario* A, Usuario* B) const{                    // Se Declara la Funcion de Consulta
    if (A == nullptr || B == nullptr || Matriz_Adyacencia == nullptr){           // Si Falta Algun Dato
        return false;                                                            // No Pueden ser Amigos
    }
    return Matriz_Adyacencia[A->obtenerId()][B->obtenerId()];                    // Lectura Directa de la Celda
}

// Funcion que Cuenta los Amigos en Comun Entre Dos Usuarios
int UdeABookGestor::contarAmigosEnComun(Usuario* A, Usuario* B) const{   // Se Declara la Funcion de Conteo

    // Declaracion de Variables Necesarias
    Usuario* Menor   = A;   // Usuario con Menos Amigos (se Recorre su Lista)
    Usuario* Mayor   = B;   // Usuario con Mas Amigos (se Consulta su Fila en la Matriz)
    int      Comunes = 0;   // Amigos Compartidos

    // Seleccion de la Lista Mas Corta para Recorrer Menos
    if (A->cantidadAmigos() > B->cantidadAmigos()){   // Si A Tiene Mas Amigos que B
        Menor = B;                                    // Se Recorre la Lista de B
        Mayor = A;                                    // Se Consulta la Fila de A
    }

    // Ciclo que Revisa Cada Amigo del Menor en la Fila del Mayor
    for (int i = 0 ; i < Menor->cantidadAmigos() ; i++){                                  // Recorre los Amigos del Menor
        Recursos::contarIteracion();                                                      // Registra la Iteracion
        if (Matriz_Adyacencia[Mayor->obtenerId()][Menor->obtenerAmigo(i)->obtenerId()]){  // Si Tambien es Amigo del Mayor
            Comunes++;                                                                    // Es un Amigo en Comun
        }
    }

    // Fin de la Funcion
    return Comunes;   // Se Retorna la Cantidad de Amigos en Comun

}

// ============================================================================
//                     I - CARGA DE DATOS DESDE LOS ARCHIVOS
// ============================================================================

// Funcion Principal de Carga: Lleva Todo el Almacenamiento al Heap
bool UdeABookGestor::cargarDatos(){   // Se Declara la Funcion de Carga

    // Declaracion de Variables Necesarias para la Carga
    int Total_Amistades   = 0;   // Amistades Cargadas
    int Total_Solicitudes = 0;   // Solicitudes Cargadas
    int Total_Mensajes    = 0;   // Mensajes Cargados
    int Total_Albumes     = 0;   // Albumes Cargados
    int Total_Fotos       = 0;   // Fotos Cargadas
    int Invalidas         = 0;   // Lineas Ignoradas por Errores de Formato

    // Preparacion de la Carga
    Recursos::reiniciar();   // Se Inicia la Medicion de Esta Funcionalidad
    liberarDatos();          // Se Limpia Cualquier Carga Anterior
    Consola::aviso("Cargando datos desde la carpeta '" + Carpeta_Datos + "' ...");

    // Carga de los Usuarios (Archivo Obligatorio)
    if (cargarUsuarios() == false){   // Si no se Pudieron Cargar los Usuarios
        return false;                 // No se Puede Continuar
    }

    // Creacion de la Matriz de Adyacencia y Carga de las Relaciones
    crearMatriz(Usuarios.obtenerTamano());                         // Matriz N x N en false
    cargarAmistades(Total_Amistades, Invalidas);                   // Llena la Matriz y las Listas de Amigos
    cargarSolicitudes(Total_Solicitudes, Invalidas);               // Llena las Solicitudes Pendientes
    cargarMensajes(Total_Mensajes, Invalidas);                     // Llena los Mensajes Enviados y Recibidos
    cargarAlbumes(Total_Albumes, Total_Fotos, Invalidas);          // Llena los Albumes y sus Fotos

    // Ciclo que Compacta las Listas de Cada Usuario (Devuelve la Memoria Sobrante)
    for (int i = 0 ; i < Usuarios.obtenerTamano() ; i++){   // Recorre Cada Usuario
        Usuarios[i]->compactarListas();                      // Ajusta Todas sus Listas
        Recursos::contarIteracion();                         // Registra la Iteracion
    }
    Usuarios.compactar();             // Ajusta la Lista Principal
    Usuarios_Ordenados.compactar();   // Ajusta el Indice Alfabetico

    // Resumen de la Carga
    Consola::exito("Datos cargados correctamente.");
    cout << "        Usuarios: " << Usuarios.obtenerTamano() << "   Amistades: " << Total_Amistades
         << "   Solicitudes pendientes: " << Total_Solicitudes << endl;
    cout << "        Mensajes: " << Total_Mensajes << "   Albumes: " << Total_Albumes
         << "   Fotos: " << Total_Fotos << endl;
    if (Invalidas > 0){                                                                        // Si Hubo Lineas con Errores
        Consola::aviso(to_string(Invalidas) + " linea(s) con formato invalido fueron ignoradas.");   // Se Informa
    }

    // Medicion de Recursos
    Recursos::sumarMemoriaLocal(6 * sizeof(int));   // Seis Contadores Locales
    medirConsumoRecursos("Carga de datos");          // Se Muestran las Metricas

    // Fin de la Funcion
    return true;   // La Carga fue Exitosa

}

// Funcion que Lee el Archivo de Usuarios (el Id de Cada Usuario es su Orden en el Archivo)
bool UdeABookGestor::cargarUsuarios(){   // Se Declara la Funcion de Carga de Usuarios

    // Declaracion de Variables Necesarias
    ifstream Archivo(Carpeta_Datos + ARCHIVO_USUARIOS);   // Archivo de Usuarios
    string   Linea        = "";                           // Linea Leida
    string   Campos[7];                                   // Campos de la Linea
    int      Numero_Linea = 0;                            // Numero de la Linea (para Mensajes de Error)
    Usuario* Nuevo        = nullptr;                      // Usuario Creado

    // Verificacion de Apertura
    if (Archivo.is_open() == false){                                                 // Si el Archivo no Existe
        Consola::error("No se encontro el archivo '" + Carpeta_Datos + ARCHIVO_USUARIOS + "'.");
        return false;                                                                // No se Puede Cargar
    }

    // Ciclo de Lectura Linea por Linea
    while (getline(Archivo, Linea)){                 // Mientras Haya Lineas
        Recursos::contarIteracion();                 // Registra la Iteracion
        Recursos::contarLineaArchivo();              // Registra la Linea Leida
        Numero_Linea++;                              // Cuenta la Linea
        limpiarLinea(Linea);                         // Quita el '\r' de Windows

        if (Linea.empty() || Linea[0] == '#'){       // Si es Vacia o un Comentario
            continue;                                // Se Salta
        }

        // Validacion del Formato (un Error Aqui Desordenaria Todos los Ids, por eso Detiene la Carga)
        if (dividir(Linea, ';', Campos, 7) != 7 || Campos[0].empty() || Campos[1].empty()){   // Si Faltan Campos
            Consola::error(ARCHIVO_USUARIOS + " linea " + to_string(Numero_Linea) + ": se esperaban 7 campos.");
            liberarDatos();                                                                    // Se Deshace lo Cargado
            return false;                                                                      // Carga Fallida
        }

        // Creacion del Usuario en el Heap
        Nuevo = new Usuario(Usuarios.obtenerTamano(), Campos[0], Campos[1], Campos[2],
                            Campos[3], Campos[4], Campos[5], Campos[6]);   // Id = Posicion Actual
        Recursos::reservar(sizeof(Usuario));                                // Se Registra el Objeto

        // Registro en el Indice Alfabetico (Detecta Nombres Repetidos)
        if (insertarOrdenado(Nuevo) == false){                                                      // Si el Nombre ya Existia
            Consola::error(ARCHIVO_USUARIOS + " linea " + to_string(Numero_Linea) + ": el usuario '" + Campos[0] + "' esta repetido.");
            delete Nuevo;                                                                           // Se Libera el Repetido
            Recursos::liberar(sizeof(Usuario));                                                     // Se Registra la Liberacion
            liberarDatos();                                                                         // Se Deshace lo Cargado
            return false;                                                                           // Carga Fallida
        }
        Usuarios.agregar(Nuevo);   // Se Agrega a la Lista Principal (Posicion = Id)
    }
    Archivo.close();   // Se Cierra el Archivo

    // Verificacion de que Exista al Menos un Usuario
    if (Usuarios.estaVacia()){                                        // Si el Archivo no Tenia Usuarios
        Consola::error("El archivo de usuarios no tiene registros.");
        return false;                                                 // Carga Fallida
    }

    // Fin de la Funcion
    return true;   // Usuarios Cargados

}

// Funcion que Lee las Amistades (Formato: idA;idB)
void UdeABookGestor::cargarAmistades(int& Total, int& Invalidas){   // Se Declara la Funcion de Carga de Amistades

    // Declaracion de Variables Necesarias
    ifstream Archivo(Carpeta_Datos + ARCHIVO_AMISTADES);   // Archivo de Amistades
    string   Linea = "";                                   // Linea Leida
    string   Campos[2];                                    // Campos de la Linea
    int      A = 0, B = 0;                                 // Ids de los Dos Amigos
    int      N = Usuarios.obtenerTamano();                 // Cantidad de Usuarios

    // Verificacion de Apertura
    if (Archivo.is_open() == false){                                  // Si el Archivo no Existe
        Consola::aviso("No se encontro " + ARCHIVO_AMISTADES + "; se inicia sin amistades.");
        return;                                                       // Se Continua sin Amistades
    }

    // Ciclo de Lectura Linea por Linea
    while (getline(Archivo, Linea)){             // Mientras Haya Lineas
        Recursos::contarIteracion();             // Registra la Iteracion
        Recursos::contarLineaArchivo();          // Registra la Linea Leida
        limpiarLinea(Linea);                     // Quita el '\r' de Windows
        if (Linea.empty() || Linea[0] == '#'){   // Si es Vacia o un Comentario
            continue;                            // Se Salta
        }

        // Validacion de la Linea
        if (dividir(Linea, ';', Campos, 2) != 2 || !aEntero(Campos[0], A) || !aEntero(Campos[1], B) ||
            A < 0 || A >= N || B < 0 || B >= N || A == B || Matriz_Adyacencia[A][B]){   // Si es Invalida o Repetida
            Invalidas++;                                                                // Se Cuenta el Error
            continue;                                                                   // Se Salta
        }

        // Registro de la Amistad Mutua
        Matriz_Adyacencia[A][B] = true;                   // A es Amigo de B
        Matriz_Adyacencia[B][A] = true;                   // B es Amigo de A (Matriz Simetrica)
        Usuarios[A]->agregarAmigo(Usuarios[B]);           // Se Agrega B a la Lista de A
        Usuarios[B]->agregarAmigo(Usuarios[A]);           // Se Agrega A a la Lista de B
        Total++;                                          // Se Cuenta la Amistad
    }
    Archivo.close();   // Se Cierra el Archivo

}

// Funcion que Lee las Solicitudes Pendientes (Formato: idRemitente;idDestino)
void UdeABookGestor::cargarSolicitudes(int& Total, int& Invalidas){   // Se Declara la Funcion de Carga de Solicitudes

    // Declaracion de Variables Necesarias
    ifstream Archivo(Carpeta_Datos + ARCHIVO_SOLICITUDES);   // Archivo de Solicitudes
    string   Linea = "";                                     // Linea Leida
    string   Campos[2];                                      // Campos de la Linea
    int      R = 0, D = 0;                                   // Ids del Remitente y del Destino
    int      N = Usuarios.obtenerTamano();                   // Cantidad de Usuarios

    // Verificacion de Apertura
    if (Archivo.is_open() == false){                                    // Si el Archivo no Existe
        Consola::aviso("No se encontro " + ARCHIVO_SOLICITUDES + "; se inicia sin solicitudes.");
        return;                                                         // Se Continua sin Solicitudes
    }

    // Ciclo de Lectura Linea por Linea
    while (getline(Archivo, Linea)){             // Mientras Haya Lineas
        Recursos::contarIteracion();             // Registra la Iteracion
        Recursos::contarLineaArchivo();          // Registra la Linea Leida
        limpiarLinea(Linea);                     // Quita el '\r' de Windows
        if (Linea.empty() || Linea[0] == '#'){   // Si es Vacia o un Comentario
            continue;                            // Se Salta
        }

        // Validacion de la Linea (no se Aceptan Solicitudes Entre Amigos ni Repetidas en Ningun Sentido)
        if (dividir(Linea, ';', Campos, 2) != 2 || !aEntero(Campos[0], R) || !aEntero(Campos[1], D) ||
            R < 0 || R >= N || D < 0 || D >= N || R == D || Matriz_Adyacencia[R][D] ||
            Usuarios[D]->tieneSolicitudDe(Usuarios[R]) || Usuarios[R]->tieneSolicitudDe(Usuarios[D])){   // Si es Invalida
            Invalidas++;                                                                                 // Se Cuenta el Error
            continue;                                                                                    // Se Salta
        }

        // Registro de la Solicitud
        Usuarios[D]->recibirSolicitud(Usuarios[R]);   // La Solicitud Queda en la Lista del Destino
        Total++;                                      // Se Cuenta la Solicitud
    }
    Archivo.close();   // Se Cierra el Archivo

}

// Funcion que Lee los Mensajes (Formato: idEmisor;idReceptor;AAAA-MM-DD HH:MM:SS;leido;contenido)
void UdeABookGestor::cargarMensajes(int& Total, int& Invalidas){   // Se Declara la Funcion de Carga de Mensajes

    // Declaracion de Variables Necesarias
    ifstream Archivo(Carpeta_Datos + ARCHIVO_MENSAJES);   // Archivo de Mensajes
    string   Linea = "";                                  // Linea Leida
    string   Campos[5];                                   // Campos de la Linea
    int      E = 0, R = 0;                                // Ids del Emisor y del Receptor
    int      N = Usuarios.obtenerTamano();                // Cantidad de Usuarios
    Fecha    Fecha_Envio;                                 // Fecha Leida del Archivo
    Mensaje* Nuevo = nullptr;                             // Mensaje Creado

    // Verificacion de Apertura
    if (Archivo.is_open() == false){                                 // Si el Archivo no Existe
        Consola::aviso("No se encontro " + ARCHIVO_MENSAJES + "; se inicia sin mensajes.");
        return;                                                      // Se Continua sin Mensajes
    }

    // Ciclo de Lectura Linea por Linea
    while (getline(Archivo, Linea)){             // Mientras Haya Lineas
        Recursos::contarIteracion();             // Registra la Iteracion
        Recursos::contarLineaArchivo();          // Registra la Linea Leida
        limpiarLinea(Linea);                     // Quita el '\r' de Windows
        if (Linea.empty() || Linea[0] == '#'){   // Si es Vacia o un Comentario
            continue;                            // Se Salta
        }

        // Validacion de la Linea
        if (dividir(Linea, ';', Campos, 5) != 5 || !aEntero(Campos[0], E) || !aEntero(Campos[1], R) ||
            E < 0 || E >= N || R < 0 || R >= N || E == R || (Campos[3] != "0" && Campos[3] != "1") ||
            Campos[4].empty() || static_cast<int>(Campos[4].size()) > LIMITE_MENSAJE ||
            Fecha::desdeTexto(Campos[2], Fecha_Envio) == false){   // Si es Invalida
            Invalidas++;                                           // Se Cuenta el Error
            continue;                                              // Se Salta
        }

        // Creacion del Mensaje (Existe una Sola Vez y Ambos Usuarios lo Referencian)
        Nuevo = new Mensaje(Usuarios[E], Usuarios[R], Campos[4], Fecha_Envio, Campos[3] == "1");   // Se Crea en el Heap
        Recursos::reservar(sizeof(Mensaje));                                                      // Se Registra el Objeto
        Usuarios[E]->enviarMensaje(Nuevo);                                                        // El Emisor es su Dueno
        Usuarios[R]->recibirMensaje(Nuevo);                                                       // El Receptor lo Referencia
        Total++;                                                                                  // Se Cuenta el Mensaje
    }
    Archivo.close();   // Se Cierra el Archivo

}

// Funcion que Lee los Albumes y sus Fotos
// Formato:  A;idUsuario;nombreAlbum;privacidad
//           F;nombreFoto;fechaCaptura;tamanoBytes;idsLike(separados por coma)
void UdeABookGestor::cargarAlbumes(int& Total_Albumes, int& Total_Fotos, int& Invalidas){   // Se Declara la Funcion de Carga de Albumes

    // Declaracion de Variables Necesarias
    ifstream Archivo(Carpeta_Datos + ARCHIVO_ALBUMES);   // Archivo de Albumes
    string   Linea       = "";                           // Linea Leida
    string   Campos[5];                                  // Campos de la Linea
    string   Id_Texto    = "";                           // Texto de un Id de Like
    int      Id          = 0;                            // Id Leido
    int      Tamano      = 0;                            // Tamano de la Foto
    int      Leidos      = 0;                            // Campos Leidos de la Linea
    int      N           = Usuarios.obtenerTamano();     // Cantidad de Usuarios
    char     Privacidad  = '\0';                         // Codigo de Privacidad
    Album*   Actual      = nullptr;                      // Ultimo Album Leido (Dueno de las Fotos Siguientes)
    Foto*    Nueva       = nullptr;                      // Foto Creada

    // Verificacion de Apertura
    if (Archivo.is_open() == false){                                // Si el Archivo no Existe
        Consola::aviso("No se encontro " + ARCHIVO_ALBUMES + "; se inicia sin albumes.");
        return;                                                     // Se Continua sin Albumes
    }

    // Ciclo de Lectura Linea por Linea
    while (getline(Archivo, Linea)){             // Mientras Haya Lineas
        Recursos::contarIteracion();             // Registra la Iteracion
        Recursos::contarLineaArchivo();          // Registra la Linea Leida
        limpiarLinea(Linea);                     // Quita el '\r' de Windows
        if (Linea.empty() || Linea[0] == '#'){   // Si es Vacia o un Comentario
            continue;                            // Se Salta
        }

        // Linea de Album
        if (Linea[0] == 'A'){
            Leidos     = dividir(Linea, ';', Campos, 4);                              // A;id;nombre;privacidad
            Privacidad = (Leidos == 4) ? Album::privacidadDesdeTexto(Campos[3]) : '\0';  // Codigo de Privacidad
            if (Leidos != 4 || !aEntero(Campos[1], Id) || Id < 0 || Id >= N || Campos[2].empty() || Privacidad == '\0'){   // Si es Invalida
                Invalidas++;          // Se Cuenta el Error
                Actual = nullptr;     // Sus Fotos Tambien Seran Ignoradas
                continue;             // Se Salta
            }
            Actual = new Album(Campos[2], RUTA_BASE_FOTOS + Usuarios[Id]->obtenerNombreUsuario() + "/albums/" + Campos[2], Privacidad);   // Se Crea el Album
            Recursos::reservar(sizeof(Album));    // Se Registra el Objeto
            Usuarios[Id]->agregarAlbum(Actual);   // El Usuario es su Dueno
            Total_Albumes++;                      // Se Cuenta el Album
        }

        // Linea de Foto (Pertenece al Ultimo Album Leido)
        else if (Linea[0] == 'F'){
            Leidos = dividir(Linea, ';', Campos, 5);                                  // F;nombre;fecha;tamano;likes
            if (Actual == nullptr || Leidos < 4 || Campos[1].empty() || !aEntero(Campos[3], Tamano) || Tamano < 0){   // Si es Invalida
                Invalidas++;   // Se Cuenta el Error
                continue;      // Se Salta
            }
            Nueva = new Foto(Campos[2], Tamano, Actual->obtenerRuta() + "/" + Campos[1] + ".png");   // Se Crea la Foto
            Recursos::reservar(sizeof(Foto));   // Se Registra el Objeto
            Actual->agregarFoto(Nueva);         // El Album es su Dueno
            Total_Fotos++;                      // Se Cuenta la Foto

            // Lectura de los Likes Separados por Coma
            if (Leidos == 5){                                       // Si la Linea Trae Likes
                stringstream Flujo_Likes(Campos[4]);                // Flujo para Separar los Ids
                while (getline(Flujo_Likes, Id_Texto, ',')){        // Ciclo que Lee Cada Id
                    Recursos::contarIteracion();                    // Registra la Iteracion
                    if (aEntero(Id_Texto, Id) && Id >= 0 && Id < N){   // Si el Id es Valido
                        Nueva->darLike(Id);                         // Se Registra el Like (sin Repetir)
                    }
                }
            }
        }

        // Linea Desconocida
        else {
            Invalidas++;   // Se Cuenta el Error
        }
    }
    Archivo.close();   // Se Cierra el Archivo

}

// ============================================================================
//                I - ACTUALIZACION DEL ALMACENAMIENTO PERMANENTE
// ============================================================================

// Funcion Principal de Guardado: Solo Reescribe los Archivos que Cambiaron
bool UdeABookGestor::guardarDatos(){   // Se Declara la Funcion de Guardado

    // Declaracion de Variables Necesarias
    bool   Exito    = true;   // Indica si Todo se Guardo Bien
    string Guardado = "";     // Lista de lo que se Guardo

    // Verificacion de Cambios Pendientes
    if (hayCambiosPendientes() == false){   // Si no Hubo Cambios
        return true;                        // No se Toca el Disco
    }

    // Inicio de la Medicion
    Recursos::reiniciar();   // Se Inicia la Medicion de Esta Funcionalidad

    // Guardado de Cada Archivo Afectado
    if (Cambios_Amistades){                                         // Si Cambiaron Amistades o Solicitudes
        Exito = guardarAmistades() && Exito;                        // Se Reescribe amistades.txt
        Exito = guardarSolicitudes() && Exito;                      // Se Reescribe solicitudes.txt
        Guardado += "amistades, solicitudes ";                      // Se Anota lo Guardado
    }
    if (Cambios_Mensajes){                                          // Si Cambiaron los Mensajes
        Exito = guardarMensajes() && Exito;                         // Se Reescribe mensajes.txt
        Guardado += "mensajes ";                                    // Se Anota lo Guardado
    }
    if (Cambios_Likes){                                             // Si Cambiaron los Likes
        Exito = guardarAlbumes() && Exito;                          // Se Reescribe albumes.txt
        Guardado += "likes ";                                       // Se Anota lo Guardado
    }

    // Resultado del Guardado
    if (Exito){                                                     // Si Todo Salio Bien
        Cambios_Amistades = false;                                  // Ya no Hay Cambios Pendientes
        Cambios_Mensajes  = false;                                  // Ya no Hay Cambios Pendientes
        Cambios_Likes     = false;                                  // Ya no Hay Cambios Pendientes
        Consola::exito("Cambios guardados en '" + Carpeta_Datos + "' (" + Guardado + ").");
    }
    else {                                                          // Si Algun Archivo Fallo
        Consola::error("No se pudieron guardar todos los cambios. Revise la carpeta de datos.");
    }

    // Medicion de Recursos
    Recursos::sumarMemoriaLocal(sizeof(Exito) + sizeof(Guardado));                    // Variables Locales
    medirConsumoRecursos("Actualizacion de datos (guardado en disco)");               // Se Muestran las Metricas

    // Fin de la Funcion
    return Exito;   // Se Retorna el Resultado

}

// Funcion que Escribe las Amistades (Cada Pareja una Sola Vez)
bool UdeABookGestor::guardarAmistades(){   // Se Declara la Funcion de Guardado de Amistades

    // Declaracion de Variables Necesarias
    ofstream Archivo(Carpeta_Datos + ARCHIVO_AMISTADES);   // Archivo de Salida
    Usuario* Actual = nullptr;                             // Usuario que se Recorre
    Usuario* Amigo  = nullptr;                             // Amigo del Usuario

    // Verificacion de Apertura
    if (Archivo.is_open() == false){   // Si no se Pudo Abrir
        return false;                  // Guardado Fallido
    }

    // Encabezado Explicativo
    Archivo << "# Formato: idUsuarioA;idUsuarioB  (cada amistad se escribe una sola vez con idA < idB)\n";

    // Ciclo que Recorre las Listas de Amigos (O(Amistades) en Lugar de O(N^2) con la Matriz)
    for (int i = 0 ; i < Usuarios.obtenerTamano() ; i++){            // Recorre Cada Usuario
        Actual = Usuarios[i];                                         // Usuario Actual
        for (int j = 0 ; j < Actual->cantidadAmigos() ; j++){         // Recorre sus Amigos
            Recursos::contarIteracion();                              // Registra la Iteracion
            Amigo = Actual->obtenerAmigo(j);                          // Amigo Actual
            if (Actual->obtenerId() < Amigo->obtenerId()){            // Solo se Escribe Desde el Id Menor
                Archivo << Actual->obtenerId() << ';' << Amigo->obtenerId() << '\n';   // '\n' Evita Vaciar el Bufer en Cada Linea
                Recursos::contarLineaArchivo();                       // Registra la Linea Escrita
            }
        }
    }

    // Fin de la Funcion
    Archivo.close();   // Se Cierra el Archivo
    return true;       // Guardado Exitoso

}

// Funcion que Escribe las Solicitudes Pendientes
bool UdeABookGestor::guardarSolicitudes(){   // Se Declara la Funcion de Guardado de Solicitudes

    // Declaracion de Variables Necesarias
    ofstream Archivo(Carpeta_Datos + ARCHIVO_SOLICITUDES);   // Archivo de Salida
    Usuario* Destino = nullptr;                              // Usuario que Recibio las Solicitudes

    // Verificacion de Apertura
    if (Archivo.is_open() == false){   // Si no se Pudo Abrir
        return false;                  // Guardado Fallido
    }

    // Encabezado Explicativo
    Archivo << "# Formato: idRemitente;idDestino  (solicitudes de amistad pendientes)\n";

    // Ciclo que Recorre las Solicitudes de Cada Usuario
    for (int i = 0 ; i < Usuarios.obtenerTamano() ; i++){              // Recorre Cada Usuario
        Destino = Usuarios[i];                                          // Usuario Actual
        for (int j = 0 ; j < Destino->cantidadSolicitudes() ; j++){     // Recorre sus Solicitudes
            Recursos::contarIteracion();                                // Registra la Iteracion
            Archivo << Destino->obtenerSolicitud(j)->obtenerId() << ';' << Destino->obtenerId() << '\n';   // Remitente;Destino
            Recursos::contarLineaArchivo();                             // Registra la Linea Escrita
        }
    }

    // Fin de la Funcion
    Archivo.close();   // Se Cierra el Archivo
    return true;       // Guardado Exitoso

}

// Funcion que Escribe Todos los Mensajes
bool UdeABookGestor::guardarMensajes(){   // Se Declara la Funcion de Guardado de Mensajes

    // Declaracion de Variables Necesarias
    ofstream Archivo(Carpeta_Datos + ARCHIVO_MENSAJES);   // Archivo de Salida
    Usuario* Emisor = nullptr;                            // Usuario que Envio los Mensajes
    Mensaje* Actual = nullptr;                            // Mensaje que se Escribe

    // Verificacion de Apertura
    if (Archivo.is_open() == false){   // Si no se Pudo Abrir
        return false;                  // Guardado Fallido
    }

    // Encabezado Explicativo
    Archivo << "# Formato: idEmisor;idReceptor;AAAA-MM-DD HH:MM:SS;leido(0/1);contenido\n";

    // Ciclo que Recorre los Mensajes Enviados de Cada Usuario (Cada Mensaje Aparece una Sola Vez)
    for (int i = 0 ; i < Usuarios.obtenerTamano() ; i++){            // Recorre Cada Usuario
        Emisor = Usuarios[i];                                         // Usuario Actual
        for (int j = 0 ; j < Emisor->cantidadEnviados() ; j++){       // Recorre sus Mensajes Enviados
            Recursos::contarIteracion();                              // Registra la Iteracion
            Actual = Emisor->obtenerEnviado(j);                       // Mensaje Actual
            Archivo << Emisor->obtenerId() << ';' << Actual->obtenerReceptor()->obtenerId() << ';'
                    << Actual->obtenerFechaEnvio() << ';' << (Actual->estaLeido() ? '1' : '0') << ';'
                    << Actual->obtenerContenido() << '\n';            // Se Escribe la Linea Completa
            Recursos::contarLineaArchivo();                           // Registra la Linea Escrita
        }
    }

    // Fin de la Funcion
    Archivo.close();   // Se Cierra el Archivo
    return true;       // Guardado Exitoso

}

// Funcion que Escribe los Albumes, sus Fotos y los Likes
bool UdeABookGestor::guardarAlbumes(){   // Se Declara la Funcion de Guardado de Albumes

    // Declaracion de Variables Necesarias
    ofstream Archivo(Carpeta_Datos + ARCHIVO_ALBUMES);   // Archivo de Salida
    Usuario* Duenio = nullptr;                           // Dueno de los Albumes
    Album*   Actual = nullptr;                           // Album que se Escribe
    Foto*    Imagen = nullptr;                           // Foto que se Escribe

    // Verificacion de Apertura
    if (Archivo.is_open() == false){   // Si no se Pudo Abrir
        return false;                  // Guardado Fallido
    }

    // Encabezado Explicativo
    Archivo << "# A;idUsuario;nombreAlbum;privacidad(publico/amigos/privado)\n";
    Archivo << "# F;nombreFoto;fechaCaptura;tamanoBytes;idsQueDieronLike(separados por coma)\n";
    Archivo << "# Cada linea F pertenece al ultimo album A escrito encima de ella.\n";

    // Ciclo que Recorre Usuarios, Albumes y Fotos
    for (int i = 0 ; i < Usuarios.obtenerTamano() ; i++){                 // Recorre Cada Usuario
        Duenio = Usuarios[i];                                              // Usuario Actual
        for (int j = 0 ; j < Duenio->cantidadAlbumes() ; j++){             // Recorre sus Albumes
            Recursos::contarIteracion();                                   // Registra la Iteracion
            Actual = Duenio->obtenerAlbum(j);                              // Album Actual
            Archivo << "A;" << Duenio->obtenerId() << ';' << Actual->obtenerNombre() << ';' << Actual->privacidadTexto() << '\n';
            Recursos::contarLineaArchivo();                                // Registra la Linea Escrita

            for (int k = 0 ; k < Actual->cantidadFotos() ; k++){           // Recorre sus Fotos
                Recursos::contarIteracion();                               // Registra la Iteracion
                Imagen = Actual->obtenerFoto(k);                           // Foto Actual
                Archivo << "F;" << Imagen->obtenerNombreArchivo() << ';' << Imagen->obtenerFechaCaptura() << ';'
                        << Imagen->obtenerTamano() << ';';                 // Datos de la Foto

                for (int l = 0 ; l < Imagen->obtenerConteoLikes() ; l++){  // Recorre los Likes
                    Recursos::contarIteracion();                           // Registra la Iteracion
                    Archivo << (l > 0 ? "," : "") << Imagen->obtenerIdLike(l);   // Ids Separados por Coma
                }
                Archivo << '\n';                                           // Fin de la Linea de la Foto
                Recursos::contarLineaArchivo();                            // Registra la Linea Escrita
            }
        }
    }

    // Fin de la Funcion
    Archivo.close();   // Se Cierra el Archivo
    return true;       // Guardado Exitoso

}

// ============================================================================
//               II - INGRESO A LA PLATAFORMA Y MEJORES AMIGOS
// ============================================================================

// Funcion que Inicia Sesion con Nombre de Usuario y Contrasena
bool UdeABookGestor::iniciarSesion(const string& Nombre_Usuario, const string& Contrasena){   // Se Declara la Funcion de Ingreso

    // Declaracion de Variables Necesarias
    Usuario* Encontrado = nullptr;   // Usuario con ese Nombre
    bool     Exito      = false;     // Indica si el Ingreso fue Correcto

    // Inicio de la Medicion y Busqueda del Usuario
    Recursos::reiniciar();                         // Se Inicia la Medicion de Esta Funcionalidad
    Encontrado = buscarUsuario(Nombre_Usuario);    // Busqueda Binaria O(log N)

    // Verificacion de las Credenciales
    if (Encontrado != nullptr && Encontrado->verificarContrasena(Contrasena)){   // Si las Credenciales son Correctas
        Sesion_Activa      = Encontrado;      // Se Guarda la Sesion
        Exito              = true;            // Ingreso Correcto
        Cantidad_Mostrados = 0;               // No Hay Fotos Previas
        calcularPesosMejoresAmigos();         // Se Calcula la Prioridad de Cada Amigo
        elegirMejoresAmigos(false);           // Se Sortean las 3 Fotos Iniciales

        Consola::exito("Bienvenido de nuevo, " + Sesion_Activa->obtenerNombreUsuario() + ".");
        cout << endl << "  Tus mejores amigos de hoy:" << endl;
        imprimirMejoresAmigos();              // Se Muestran las 3 Fotos Seleccionadas
    }
    else {                                    // Si las Credenciales Fallan
        Consola::error("Usuario o contrasena incorrectos. Intente de nuevo.");
    }

    // Medicion de Recursos
    Recursos::sumarMemoriaLocal(sizeof(Encontrado) + sizeof(Exito));                       // Variables Locales
    medirConsumoRecursos(Exito ? "Ingreso a la plataforma y mejores amigos" : "Intento de ingreso fallido");

    // Fin de la Funcion
    return Exito;   // Se Retorna el Resultado

}

// Funcion que Cierra la Sesion Activa
void UdeABookGestor::cerrarSesion(){       // Se Declara la Funcion de Cierre
    if (Sesion_Activa != nullptr){         // Si Hay una Sesion
        Sesion_Activa->limpiarSugerencias();   // Se Libera la Memoria de las Sugerencias
    }
    liberarPesos();               // Se Liberan los Arreglos de Mejores Amigos
    Sesion_Activa      = nullptr; // Ya no Hay Sesion
    Cantidad_Mostrados = 0;       // Ya no Hay Fotos Mostradas
}

// Funcion que Retorna el Usuario con Sesion Activa
Usuario* UdeABookGestor::obtenerSesionActiva() const{   // Se Declara la Funcion de Consulta
    return Sesion_Activa;                               // Se Retorna el Puntero
}

// Funcion que Libera los Arreglos de Mejores Amigos
void UdeABookGestor::liberarPesos(){                                              // Se Declara la Funcion de Liberacion
    if (Pesos_Amigos != nullptr){                                                 // Si Hay Pesos Reservados
        delete[] Pesos_Amigos;                                                    // Se Liberan
        Recursos::liberar(static_cast<long>(Cantidad_Pesos) * sizeof(double));   // Se Registra
    }
    if (Comunes_Amigos != nullptr){                                               // Si Hay Conteos Reservados
        delete[] Comunes_Amigos;                                                  // Se Liberan
        Recursos::liberar(static_cast<long>(Cantidad_Pesos) * sizeof(int));       // Se Registra
    }
    Pesos_Amigos   = nullptr;   // Sin Pesos
    Comunes_Amigos = nullptr;   // Sin Conteos
    Cantidad_Pesos = 0;         // Sin Amigos Procesados
}

// Funcion que Calcula la Prioridad de Cada Amigo Segun los Amigos en Comun
void UdeABookGestor::calcularPesosMejoresAmigos(){   // Se Declara la Funcion de Calculo de Pesos

    // Declaracion de Variables Necesarias
    int Comunes = 0;   // Amigos en Comun con el Amigo Actual

    // Liberacion de Pesos Anteriores y Reserva de los Nuevos
    liberarPesos();                                            // Se Liberan los Arreglos Viejos
    Cantidad_Pesos = Sesion_Activa->cantidadAmigos();          // Un Peso por Cada Amigo
    if (Cantidad_Pesos == 0){                                  // Si no Tiene Amigos
        return;                                                // No Hay Nada que Calcular
    }
    Pesos_Amigos   = new double[Cantidad_Pesos];               // Arreglo de Pesos
    Comunes_Amigos = new int[Cantidad_Pesos];                  // Arreglo de Amigos en Comun
    Recursos::reservar(static_cast<long>(Cantidad_Pesos) * (sizeof(double) + sizeof(int)));   // Se Registran Ambos

    // Ciclo que Calcula el Peso de Cada Amigo
    for (int i = 0 ; i < Cantidad_Pesos ; i++){                                    // Recorre Cada Amigo
        Recursos::contarIteracion();                                               // Registra la Iteracion
        Comunes           = contarAmigosEnComun(Sesion_Activa, Sesion_Activa->obtenerAmigo(i));   // Amigos Compartidos
        Comunes_Amigos[i] = Comunes;                                               // Se Guarda el Conteo

        if (Comunes <= 10){                    // 10 o Menos Amigos en Comun
            Pesos_Amigos[i] = PESO_SIMPLE;     // Probabilidad Simple
        }
        else if (Comunes <= 20){               // Entre 11 y 20 Amigos en Comun
            Pesos_Amigos[i] = PESO_MEDIO;      // 2.5 Veces Mas Prioridad
        }
        else {                                 // 21 o Mas Amigos en Comun
            Pesos_Amigos[i] = PESO_ALTO;       // 3.5 Veces Mas Prioridad
        }
    }

    // Registro de Variables Locales
    Recursos::sumarMemoriaLocal(sizeof(Comunes));   // Variable Local

}

// Funcion que Sortea un Amigo Segun su Peso (Ruleta Ponderada)
// Estado: 0 = Libre, 1 = Mostrado en el Grupo Anterior, 2 = Ya Elegido en Este Grupo
int UdeABookGestor::elegirAmigoPonderado(const int* Estado, bool Bloquear_Previos){   // Se Declara la Funcion de Sorteo

    // Declaracion de Variables Necesarias
    double Total         = 0.0;   // Suma de los Pesos Disponibles
    double Acumulado     = 0.0;   // Suma Acumulada Durante el Recorrido
    double Aleatorio     = 0.0;   // Punto Sorteado en la Ruleta
    int    Ultimo_Valido = -1;    // Ultimo Amigo Disponible (Respaldo por Redondeo)

    // Ciclo que Suma los Pesos de los Amigos Disponibles
    for (int i = 0 ; i < Cantidad_Pesos ; i++){                                  // Recorre Cada Amigo
        Recursos::contarIteracion();                                             // Registra la Iteracion
        if (Estado[i] == 0 || (Estado[i] == 1 && Bloquear_Previos == false)){    // Si Puede Participar
            Total += Pesos_Amigos[i];                                            // Su Peso Entra en la Ruleta
        }
    }

    // Sorteo de un Punto Entre 0 y el Total
    uniform_real_distribution<double> Distribucion(0.0, Total);   // Distribucion Uniforme Continua
    Aleatorio = Distribucion(Generador);                          // Numero Aleatorio en [0, Total)
    Recursos::contarUsoAleatorio();                               // Se Registra el Uso de <random>

    // Ciclo que Busca en que Tramo de la Ruleta Cayo el Punto
    for (int i = 0 ; i < Cantidad_Pesos ; i++){                                  // Recorre Cada Amigo
        Recursos::contarIteracion();                                             // Registra la Iteracion
        if (Estado[i] == 0 || (Estado[i] == 1 && Bloquear_Previos == false)){    // Si Puede Participar
            Acumulado    += Pesos_Amigos[i];                                     // Avanza su Tramo
            Ultimo_Valido = i;                                                   // Se Guarda como Respaldo
            if (Aleatorio < Acumulado){                                          // Si el Punto Cayo en su Tramo
                return i;                                                        // Este Amigo es el Elegido
            }
        }
    }

    // Fin de la Funcion
    return Ultimo_Valido;   // Respaldo por Errores de Redondeo

}

// Funcion que Elige las 3 Fotos de Perfil sin Repetir y sin Patron Fijo
void UdeABookGestor::elegirMejoresAmigos(bool Alternar){   // Se Declara la Funcion de Seleccion

    // Declaracion de Variables Necesarias
    int  Cantidad          = (Cantidad_Pesos < FOTOS_MEJORES_AMIGOS) ? Cantidad_Pesos : FOTOS_MEJORES_AMIGOS;   // Fotos a Mostrar
    int* Estado            = nullptr;                                         // Estado de Cada Amigo en el Sorteo
    bool Evitar_Anteriores = Alternar && Cantidad_Pesos > FOTOS_MEJORES_AMIGOS;   // Solo si Hay Mas de 3 Amigos
    bool Bloquear          = false;                                           // Bloqueo del Sorteo Actual

    // Verificacion de Amigos
    if (Cantidad == 0){            // Si no Tiene Amigos
        Cantidad_Mostrados = 0;    // No se Muestra Nada
        return;                    // Fin
    }

    // Reserva e Inicializacion del Arreglo de Estados
    Estado = new int[Cantidad_Pesos];                                       // Un Estado por Amigo
    Recursos::reservar(static_cast<long>(Cantidad_Pesos) * sizeof(int));    // Se Registra
    for (int i = 0 ; i < Cantidad_Pesos ; i++){                             // Ciclo que Inicializa los Estados
        Estado[i] = 0;                                                      // Todos Empiezan Libres
        Recursos::contarIteracion();                                        // Registra la Iteracion
    }

    // Marcado del Grupo Anterior para que el Nuevo Grupo sea Distinto
    if (Evitar_Anteriores){                                   // Si se Esta Alternando
        for (int k = 0 ; k < Cantidad_Mostrados ; k++){       // Recorre las Fotos Anteriores
            Estado[Mostrados[k]] = 1;                         // Se Marcan como Mostradas
            Recursos::contarIteracion();                      // Registra la Iteracion
        }
    }

    // Ciclo que Sortea Cada una de las Fotos
    Cantidad_Mostrados = 0;                                                                  // Se Llena a Medida que se Sortea
    for (int k = 0 ; k < Cantidad ; k++){                                                    // Recorre Cada Posicion
        Recursos::contarIteracion();                                                         // Registra la Iteracion
        Bloquear      = Evitar_Anteriores && (Cantidad_Pesos >= 2 * FOTOS_MEJORES_AMIGOS || k == 0);   // Con 6+ Amigos el Grupo es Totalmente Nuevo; con 4-5 al Menos la Primera Cambia
        Mostrados[k]  = elegirAmigoPonderado(Estado, Bloquear);                              // Sorteo Ponderado
        if (Mostrados[k] == -1){                                                             // Proteccion: si no Quedara Ningun Candidato
            break;                                                                           // Se Muestran los que ya se Eligieron
        }
        Estado[Mostrados[k]] = 2;                                                            // No Puede Repetirse en el Grupo
        Cantidad_Mostrados++;                                                                // Una Foto Mas en el Grupo
    }

    // Liberacion del Arreglo Temporal
    delete[] Estado;                                                        // Se Libera el Arreglo
    Recursos::liberar(static_cast<long>(Cantidad_Pesos) * sizeof(int));     // Se Registra

    // Registro de Variables Locales y Parametros por Valor
    Recursos::sumarMemoriaLocal(sizeof(Alternar) + sizeof(Cantidad) + sizeof(Estado) + sizeof(Evitar_Anteriores) + sizeof(Bloquear));

}

// Funcion que Muestra las 3 Fotos de Perfil Seleccionadas
void UdeABookGestor::imprimirMejoresAmigos() const{   // Se Declara la Funcion de Impresion

    // Declaracion de Variables Necesarias
    Usuario* Amigo = nullptr;   // Amigo que se Muestra

    // Verificacion de Amigos
    if (Cantidad_Mostrados == 0){                                                      // Si no Hay Fotos
        Consola::aviso("Aun no tienes amigos. Revisa tus sugerencias de amistad (opcion 3).");
        return;                                                                        // Fin
    }

    // Ciclo que Muestra Cada Foto de Perfil
    for (int k = 0 ; k < Cantidad_Mostrados ; k++){                                    // Recorre Cada Foto
        Amigo = Sesion_Activa->obtenerAmigo(Mostrados[k]);                             // Amigo de esa Posicion
        cout << "  Foto perfil de amigo " << (k + 1) << ": " << Amigo->obtenerRutaFotoPerfil() << endl;
        cout << "                          (" << Amigo->obtenerNombreUsuario() << " - "
             << Comunes_Amigos[Mostrados[k]] << (Comunes_Amigos[Mostrados[k]] == 1 ? " amigo" : " amigos") << " en comun)" << endl;
    }

}

// Funcion de la Opcion 'N': Cambia el Grupo de 3 Fotos
void UdeABookGestor::alternarMejoresAmigos(){   // Se Declara la Funcion de Alternar

    // Inicio de la Medicion
    Recursos::reiniciar();   // Se Inicia la Medicion de Esta Funcionalidad

    // Sorteo del Nuevo Grupo
    Consola::titulo("FOTOS DE PERFIL DE TUS AMIGOS");
    elegirMejoresAmigos(true);   // Se Sortea un Grupo Distinto al Anterior
    if (Cantidad_Pesos > 0 && Cantidad_Pesos <= FOTOS_MEJORES_AMIGOS){                        // Si Tiene 3 o Menos Amigos
        Consola::aviso("Tienes " + to_string(Cantidad_Pesos) + " amigo(s): se muestran todos en otro orden.");
    }
    imprimirMejoresAmigos();     // Se Muestra el Nuevo Grupo

    // Medicion de Recursos
    medirConsumoRecursos("Alternar fotos de perfil de amigos");

}

// ============================================================================
//                     FUNCIONES AUXILIARES DE INTERFAZ
// ============================================================================

// Funcion que Muestra los Amigos de la Sesion en Tres Columnas
void UdeABookGestor::listarAmigos() const{   // Se Declara la Funcion de Listado

    // Declaracion de Variables Necesarias
    int Total = Sesion_Activa->cantidadAmigos();   // Cantidad de Amigos

    // Verificacion de Amigos
    if (Total == 0){                                // Si no Tiene Amigos
        Consola::aviso("Aun no tienes amigos.");    // Se Informa
        return;                                     // Fin
    }

    // Ciclo que Imprime los Nombres en Columnas
    cout << "  Tus amigos (" << Total << "):" << endl;
    for (int i = 0 ; i < Total ; i++){                                                       // Recorre Cada Amigo
        Recursos::contarIteracion();                                                         // Registra la Iteracion
        cout << (i % 3 == 0 ? "    " : "") << Consola::ajustar(Sesion_Activa->obtenerAmigo(i)->obtenerNombreUsuario(), 22);
        if (i % 3 == 2 || i == Total - 1){                                                   // Al Completar 3 o al Final
            cout << endl;                                                                    // Salto de Linea
        }
    }

}

// Funcion que Pide el Nombre de un Amigo y lo Valida
Usuario* UdeABookGestor::pedirAmigo(const string& Mensaje){   // Se Declara la Funcion de Solicitud de Amigo

    // Declaracion de Variables Necesarias
    string   Nombre = "";        // Nombre Escrito
    Usuario* Amigo  = nullptr;   // Usuario Encontrado

    // Ciclo que se Repite Hasta Recibir un Amigo Valido o '0'
    while (true){
        Recursos::contarIteracion();                                              // Registra la Iteracion
        Nombre = Consola::leerLinea(Mensaje + " ('?' = ver amigos, 0 = volver): ");   // Se Pide el Nombre

        if (Nombre == "0"){        // Si Quiere Volver
            return nullptr;        // No se Eligio Amigo
        }
        if (Nombre == "?"){        // Si Quiere Ver sus Amigos
            listarAmigos();        // Se Muestran
            continue;              // Se Vuelve a Preguntar
        }

        Amigo = buscarUsuario(Nombre);                                                 // Busqueda Binaria
        if (Amigo == nullptr){                                                         // Si no Existe
            Consola::error("No existe ningun usuario llamado '" + Nombre + "'.");
        }
        else if (Amigo == Sesion_Activa){                                              // Si es el Mismo Usuario
            Consola::error("Ese eres tu. Escribe el nombre de un amigo.");
        }
        else if (sonAmigos(Sesion_Activa, Amigo) == false){                            // Si no es su Amigo (O(1))
            Consola::error("'" + Nombre + "' no esta en tu lista de amigos.");
        }
        else {                                                                         // Si Todo es Correcto
            return Amigo;                                                              // Se Retorna el Amigo
        }
    }

}

// Funcion que Valida si se Puede Enviar una Solicitud a un Usuario
bool UdeABookGestor::puedeEnviarSolicitud(Usuario* Destino, bool Mostrar_Motivo) const{   // Se Declara la Funcion de Validacion

    // Declaracion de Variables Necesarias
    string Motivo = "";   // Razon por la que no se Puede Enviar

    // Evaluacion de las Restricciones de la Guia
    if (Destino == nullptr){                                       // Si el Usuario no Existe
        Motivo = "Ese usuario no existe.";
    }
    else if (Destino == Sesion_Activa){                            // Si es el Mismo Usuario
        Motivo = "No puedes enviarte una solicitud a ti mismo.";
    }
    else if (sonAmigos(Sesion_Activa, Destino)){                   // Si ya son Amigos (O(1))
        Motivo = "Ya eres amigo de " + Destino->obtenerNombreUsuario() + ".";
    }
    else if (Destino->tieneSolicitudDe(Sesion_Activa)){            // Si ya le Envio una Solicitud
        Motivo = "Ya le enviaste una solicitud a " + Destino->obtenerNombreUsuario() + " y sigue pendiente.";
    }
    else if (Sesion_Activa->tieneSolicitudDe(Destino)){            // Si el Otro ya le Envio una Solicitud
        Motivo = Destino->obtenerNombreUsuario() + " ya te envio una solicitud. Aceptala en 'Solicitudes recibidas'.";
    }

    // Resultado de la Validacion
    if (Motivo != "" && Mostrar_Motivo){   // Si no se Puede y se Pide Explicar
        Consola::error(Motivo);            // Se Muestra el Motivo
    }

    // Fin de la Funcion
    return Motivo == "";   // Se Puede Enviar si no Hubo Ningun Motivo

}

// ============================================================================
//                       III - ADMINISTRAR AMISTADES
// ============================================================================

// Funcion a. Enviar Solicitud de Amistad
void UdeABookGestor::enviarSolicitudAmistad(){   // Se Declara la Funcion de Envio de Solicitud

    // Declaracion de Variables Necesarias
    string   Nombre  = "";        // Nombre Escrito por el Usuario
    Usuario* Destino = nullptr;   // Usuario que Recibira la Solicitud

    // Inicio de la Medicion
    Recursos::reiniciar();   // Se Inicia la Medicion de Esta Funcionalidad
    Consola::titulo("ENVIAR SOLICITUD DE AMISTAD");

    // Lectura y Validacion del Destino
    Nombre = Consola::leerLinea("Nombre de usuario (0 = volver): ");   // Se Pide el Nombre
    if (Nombre != "0" && Nombre != ""){                                // Si no Quiere Volver
        Destino = buscarUsuario(Nombre);                               // Busqueda Binaria O(log N)
        if (Destino == nullptr){                                       // Si no Existe
            Consola::error("No existe ningun usuario llamado '" + Nombre + "'.");
        }
        else if (puedeEnviarSolicitud(Destino, true)){                 // Si Cumple Todas las Reglas
            Sesion_Activa->enviarSolicitud(Destino);                   // Se Envia la Solicitud
            Cambios_Amistades = true;                                  // Hay Cambios por Guardar
            Consola::exito("Solicitud enviada a " + Destino->obtenerNombreUsuario() + ".");
        }
    }

    // Medicion de Recursos
    Recursos::sumarMemoriaLocal(sizeof(Nombre) + sizeof(Destino));   // Variables Locales
    medirConsumoRecursos("Enviar solicitud de amistad");             // Se Muestran las Metricas

}

// Funcion b. Ver, Aceptar o Rechazar las Solicitudes Recibidas
void UdeABookGestor::gestionarSolicitudesRecibidas(){   // Se Declara la Funcion de Gestion de Solicitudes

    // Declaracion de Variables Necesarias
    int      Total        = 0;         // Solicitudes Pendientes
    int      Opcion       = 0;         // Solicitud Elegida
    char     Accion       = ' ';       // Aceptar o Rechazar
    bool     Hubo_Nuevos  = false;     // Indica si se Acepto Alguna
    Usuario* Remitente    = nullptr;   // Usuario que Envio la Solicitud

    // Inicio de la Medicion
    Recursos::reiniciar();   // Se Inicia la Medicion de Esta Funcionalidad
    Consola::titulo("SOLICITUDES DE AMISTAD RECIBIDAS");

    // Ciclo que Muestra las Solicitudes Hasta que el Usuario Vuelva
    while (true){
        Recursos::contarIteracion();                     // Registra la Iteracion
        Total = Sesion_Activa->cantidadSolicitudes();    // Solicitudes Pendientes

        if (Total == 0){                                                  // Si no Hay Solicitudes
            Consola::aviso("No tienes solicitudes de amistad pendientes.");
            break;                                                        // Fin del Ciclo
        }

        // Listado de las Solicitudes
        cout << endl << "  Tienes " << Total << " solicitud(es) pendiente(s):" << endl;
        for (int i = 0 ; i < Total ; i++){                                // Recorre Cada Solicitud
            Recursos::contarIteracion();                                  // Registra la Iteracion
            Remitente = Sesion_Activa->obtenerSolicitud(i);               // Remitente de la Solicitud
            cout << "    " << Consola::ajustar(to_string(i + 1) + ".", 5)
                 << Consola::ajustar(Remitente->obtenerNombreUsuario(), 22)
                 << Consola::ajustar(Remitente->obtenerCiudad(), 16)
                 << contarAmigosEnComun(Sesion_Activa, Remitente) << " en comun" << endl;
        }

        // Eleccion de una Solicitud
        Opcion = Consola::leerEntero("Elija una solicitud (0 = volver): ", 0, Total);   // Se Pide la Solicitud
        if (Opcion == 0){   // Si Quiere Volver
            break;          // Fin del Ciclo
        }
        Remitente = Sesion_Activa->obtenerSolicitud(Opcion - 1);   // Solicitud Elegida
        Accion    = Consola::leerOpcion("[A] Aceptar   [R] Rechazar   [0] Cancelar: ");   // Se Pide la Accion

        // Ejecucion de la Accion Elegida
        if (Accion == 'A'){                                                     // Si Acepta
            if (sonAmigos(Sesion_Activa, Remitente) == false){                  // Si Aun no son Amigos
                Sesion_Activa->aceptarSolicitud(Opcion - 1);                    // Amistad Mutua en las Listas
                Matriz_Adyacencia[Sesion_Activa->obtenerId()][Remitente->obtenerId()] = true;   // Se Actualiza la Matriz
                Matriz_Adyacencia[Remitente->obtenerId()][Sesion_Activa->obtenerId()] = true;   // Matriz Simetrica
            }
            else {                                                              // Si ya Eran Amigos (Dato Inconsistente)
                Sesion_Activa->rechazarSolicitud(Opcion - 1);                   // Solo se Quita la Solicitud
            }
            Hubo_Nuevos       = true;                                           // Cambia la Lista de Amigos
            Cambios_Amistades = true;                                           // Hay Cambios por Guardar
            Consola::exito("Ahora eres amigo de " + Remitente->obtenerNombreUsuario() + ".");
        }
        else if (Accion == 'R'){                                                // Si Rechaza
            Sesion_Activa->rechazarSolicitud(Opcion - 1);                       // Se Quita la Solicitud
            Cambios_Amistades = true;                                           // Hay Cambios por Guardar
            Consola::aviso("Rechazaste la solicitud de " + Remitente->obtenerNombreUsuario() + ".");
        }
        else {                                                                  // Cualquier Otra Tecla
            Consola::aviso("Operacion cancelada.");
        }
    }

    // Recalculo de los Mejores Amigos si la Lista de Amigos Cambio
    if (Hubo_Nuevos){                     // Si se Acepto al Menos una Solicitud
        calcularPesosMejoresAmigos();     // Los Amigos en Comun Pudieron Cambiar
        Cantidad_Mostrados = 0;           // Se Descarta el Grupo Anterior
        elegirMejoresAmigos(false);       // Se Sortea un Grupo con los Datos Nuevos
    }

    // Medicion de Recursos
    Recursos::sumarMemoriaLocal(sizeof(Total) + sizeof(Opcion) + sizeof(Accion) + sizeof(Hubo_Nuevos) + sizeof(Remitente));
    medirConsumoRecursos("Aceptar solicitudes de amistad recibidas");

}

// ============================================================================
//                      IV - VISITAR PERFIL DE UN AMIGO
// ============================================================================

// Funcion que Lista los Albumes Visibles de un Amigo y Permite Abrirlos
void UdeABookGestor::visitarPerfilAmigo(){   // Se Declara la Funcion de Visita

    // Declaracion de Variables Necesarias
    Usuario* Amigo      = nullptr;   // Amigo Visitado
    int*     Visibles   = nullptr;   // Posiciones de los Albumes que se Pueden Ver
    int      Cantidad   = 0;         // Cantidad de Albumes Visibles
    int      Opcion     = 0;         // Album Elegido
    int      Reservados = 0;         // Tamano Reservado del Arreglo de Visibles
    Album*   Actual     = nullptr;   // Album que se Revisa

    // Inicio de la Medicion
    Recursos::reiniciar();   // Se Inicia la Medicion de Esta Funcionalidad
    Consola::titulo("VISITAR PERFIL DE UN AMIGO");

    // Eleccion del Amigo
    Amigo = pedirAmigo("Nombre del amigo");   // Se Pide y Valida el Amigo
    if (Amigo != nullptr){

        // Reserva del Arreglo de Albumes Visibles
        Reservados = Amigo->cantidadAlbumes();                                  // Maximo Posible
        if (Reservados > 0){                                                    // Si Tiene Albumes
            Visibles = new int[Reservados];                                     // Se Reserva el Arreglo
            Recursos::reservar(static_cast<long>(Reservados) * sizeof(int));    // Se Registra
        }

        // Ciclo que Filtra los Albumes por Privacidad ("publico" o "amigos")
        for (int i = 0 ; i < Reservados ; i++){                       // Recorre Cada Album
            Recursos::contarIteracion();                              // Registra la Iteracion
            Actual = Amigo->obtenerAlbum(i);                          // Album Actual
            if (Actual->esVisible(false, true)){                      // Visible para un Amigo
                Visibles[Cantidad] = i;                               // Se Guarda su Posicion
                Cantidad++;                                           // Se Cuenta
            }
        }

        // Ciclo del Perfil: Muestra los Albumes Hasta que el Usuario Vuelva
        while (true){
            Recursos::contarIteracion();   // Registra la Iteracion
            cout << endl;
            Consola::linea('-');
            cout << *Amigo;                // Tarjeta de Perfil (Sobrecarga <<)
            Consola::linea('-');

            if (Cantidad == 0){                                                       // Si no Hay Albumes Visibles
                Consola::aviso(Amigo->obtenerNombreUsuario() + " no tiene albumes visibles para ti.");
                break;                                                                // Fin del Ciclo
            }

            cout << "  Albumes visibles (" << Cantidad << "):" << endl;
            for (int i = 0 ; i < Cantidad ; i++){                                     // Recorre los Visibles
                Recursos::contarIteracion();                                          // Registra la Iteracion
                Actual = Amigo->obtenerAlbum(Visibles[i]);                            // Album Visible
                cout << "    " << Consola::ajustar(to_string(i + 1) + ".", 5)
                     << Consola::ajustar(Actual->obtenerNombre(), 26)
                     << Consola::ajustar(Actual->privacidadTexto(), 10)
                     << Actual->cantidadFotos() << " foto(s)" << endl;
            }

            Opcion = Consola::leerEntero("Elija un album para verlo (0 = volver): ", 0, Cantidad);   // Se Pide el Album
            if (Opcion == 0){   // Si Quiere Volver
                break;          // Fin del Ciclo
            }
            carruselFotos(Amigo->obtenerAlbum(Visibles[Opcion - 1]));   // Se Abre el Carrusel
        }
    }

    // Medicion de Recursos (Antes de Liberar para Reportar la Memoria en Uso)
    Recursos::sumarMemoriaLocal(sizeof(Amigo) + sizeof(Visibles) + sizeof(Cantidad) + sizeof(Opcion) + sizeof(Reservados) + sizeof(Actual));
    medirConsumoRecursos("Visitar perfil de un amigo");

    // Liberacion del Arreglo Temporal
    if (Visibles != nullptr){                                                // Si se Reservo el Arreglo
        delete[] Visibles;                                                   // Se Libera
        Recursos::liberar(static_cast<long>(Reservados) * sizeof(int));      // Se Registra
    }

}

// Funcion que Muestra las Fotos de un Album en Modo Carrusel Circular
void UdeABookGestor::carruselFotos(Album* Album_Actual){   // Se Declara la Funcion de Carrusel

    // Declaracion de Variables Necesarias
    int   Total   = Album_Actual->cantidadFotos();   // Cantidad de Fotos del Album
    int   Indice  = 0;                               // Foto Actual
    int   Id      = Sesion_Activa->obtenerId();      // Id de Quien Mira (para los Likes)
    char  Opcion  = ' ';                             // Tecla Elegida
    Foto* Imagen  = nullptr;                         // Foto que se Muestra

    // Verificacion de Fotos
    if (Total == 0){                                    // Si el Album esta Vacio
        Consola::aviso("Este album no tiene fotos.");   // Se Informa
        return;                                         // Fin
    }

    // Ciclo del Carrusel (Indices Circulares con Modulo)
    while (true){
        Recursos::contarIteracion();                    // Registra la Iteracion
        Imagen = Album_Actual->obtenerFoto(Indice);     // Foto Actual

        // Visualizacion de la Foto (Solo su Ruta y Datos, Segun la Guia)
        cout << endl;
        Consola::linea('-');
        cout << "  Album: " << Album_Actual->obtenerNombre() << "   |   Foto " << (Indice + 1) << " de " << Total << endl;
        Consola::linea('-');
        cout << *Imagen;                                                                // Sobrecarga << de Foto
        cout << "    Tu like : " << (Imagen->tieneLike(Id) ? "Si" : "No") << endl;
        Consola::linea('-');
        cout << "  [S] Siguiente   [A] Anterior   [L] Dar/Quitar like   [0] Salir del album" << endl;
        Opcion = Consola::leerOpcion("Opcion (ENTER = siguiente): ");                   // Se Pide la Accion

        // Ejecucion de la Accion
        if (Opcion == 'S' || Opcion == '\0'){             // Siguiente (o Solo ENTER)
            Indice = (Indice + 1) % Total;                // Avanza y Vuelve al Inicio al Llegar al Final
        }
        else if (Opcion == 'A' || Opcion == 'P'){         // Anterior / Previa
            Indice = (Indice - 1 + Total) % Total;        // Retrocede y Salta al Final si Estaba en la Primera
        }
        else if (Opcion == 'L'){                          // Dar o Quitar Like
            if (Imagen->tieneLike(Id)){                   // Si ya Tenia Like
                Imagen->quitarLike(Id);                   // Se Quita
                Consola::aviso("Quitaste tu like de esta foto.");
            }
            else {                                        // Si no Tenia Like
                Imagen->darLike(Id);                      // Se Agrega
                Consola::exito("Le diste like a esta foto.");
            }
            Cambios_Likes = true;                         // Hay Cambios por Guardar
        }
        else if (Opcion == '0'){                          // Salir del Album
            break;                                        // Fin del Carrusel
        }
        else {                                            // Tecla no Reconocida
            Consola::error("Opcion no valida.");
        }
    }

    // Registro de Variables Locales y Parametros por Valor
    Recursos::sumarMemoriaLocal(sizeof(Album_Actual) + sizeof(Total) + sizeof(Indice) + sizeof(Id) + sizeof(Opcion) + sizeof(Imagen));

}

// ============================================================================
//                     V - SUGERENCIAS DE AMISTAD
// ============================================================================

// Funcion que Calcula las K Mejores Sugerencias (Amigos de Amigos Ordenados por Amigos en Comun)
void UdeABookGestor::calcularSugerenciasAmistad(int K, int* Comunes_Resultado){   // Se Declara la Funcion de Calculo

    // Declaracion de Variables Necesarias
    int                     N           = Dimension_Matriz;               // Cantidad de Usuarios
    int                     Id_Sesion   = Sesion_Activa->obtenerId();     // Id del Usuario Activo
    int*                    Conteo      = nullptr;                        // Amigos en Comun por Id (-1 = Descartado)
    ListaDinamica<Usuario*> Candidatos;                                   // Amigos de Amigos Encontrados
    Usuario*                Amigo       = nullptr;                        // Amigo Directo
    Usuario*                Candidato   = nullptr;                        // Amigo de un Amigo
    Usuario*                Mejor       = nullptr;                        // Mejor Candidato de la Ronda
    int                     Encontradas = 0;                              // Sugerencias Encontradas
    int                     Id_C        = 0;                              // Id del Candidato

    // Reserva e Inicializacion del Arreglo de Conteo
    Sesion_Activa->limpiarSugerencias();                         // Se Borran las Sugerencias Anteriores
    Conteo = new int[N];                                         // Un Contador por Usuario
    Recursos::reservar(static_cast<long>(N) * sizeof(int));      // Se Registra
    for (int i = 0 ; i < N ; i++){                               // Ciclo que Pone Todos en Cero
        Conteo[i] = 0;                                           // Sin Amigos en Comun
        Recursos::contarIteracion();                             // Registra la Iteracion
    }

    // Ciclo Doble: Amigos de Mis Amigos (Cada Camino Yo-Amigo-Candidato es un Amigo en Comun)
    for (int i = 0 ; i < Sesion_Activa->cantidadAmigos() ; i++){                 // Recorre Mis Amigos
        Amigo = Sesion_Activa->obtenerAmigo(i);                                  // Amigo Directo
        for (int j = 0 ; j < Amigo->cantidadAmigos() ; j++){                     // Recorre los Amigos de mi Amigo
            Recursos::contarIteracion();                                         // Registra la Iteracion
            Candidato = Amigo->obtenerAmigo(j);                                  // Amigo de mi Amigo
            Id_C      = Candidato->obtenerId();                                  // Su Id
            if (Id_C == Id_Sesion || Matriz_Adyacencia[Id_Sesion][Id_C]){        // Si Soy Yo o ya es mi Amigo (O(1))
                continue;                                                        // No es Sugerencia
            }
            if (Conteo[Id_C] == 0){                                              // Si es la Primera Vez que Aparece
                Candidatos.agregar(Candidato);                                   // Se Agrega a los Candidatos
            }
            Conteo[Id_C]++;                                                      // Un Amigo en Comun Mas
        }
    }

    // Ciclo de Seleccion: Se Toma el Mayor K Veces (K <= 20, Mas Simple que Ordenar Todo)
    while (Encontradas < K){
        Mejor = nullptr;                                                         // Aun no Hay Mejor
        for (int c = 0 ; c < Candidatos.obtenerTamano() ; c++){                  // Recorre los Candidatos
            Recursos::contarIteracion();                                         // Registra la Iteracion
            Candidato = Candidatos[c];                                           // Candidato Actual
            Id_C      = Candidato->obtenerId();                                  // Su Id
            if (Conteo[Id_C] <= 0){                                              // Si ya fue Elegido o Descartado
                continue;                                                        // Se Salta
            }
            if (Mejor == nullptr || Conteo[Id_C] > Conteo[Mejor->obtenerId()] ||
               (Conteo[Id_C] == Conteo[Mejor->obtenerId()] && Id_C < Mejor->obtenerId())){   // Mas en Comun (Empate: Orden Alfabetico)
                Mejor = Candidato;                                               // Nuevo Mejor
            }
        }
        if (Mejor == nullptr){   // Si no Quedan Candidatos
            break;               // Fin de la Seleccion
        }

        // Descarte de Quienes Tienen Solicitudes Pendientes (en Cualquier Sentido)
        if (puedeEnviarSolicitud(Mejor, false) == false){   // Si no se le Puede Enviar Solicitud
            Conteo[Mejor->obtenerId()] = -1;                // Se Descarta
            continue;                                       // Se Busca Otro
        }

        // Registro de la Sugerencia
        Sesion_Activa->agregarSugerencia(Mejor);                          // Se Guarda en el Usuario (Diagrama)
        Comunes_Resultado[Encontradas] = Conteo[Mejor->obtenerId()];      // Se Guarda su Conteo
        Conteo[Mejor->obtenerId()]     = -1;                              // Ya no Participa
        Encontradas++;                                                    // Una Sugerencia Mas
    }

    // Registro de Variables Locales y Parametros por Valor
    Recursos::sumarMemoriaLocal(sizeof(K) + sizeof(Comunes_Resultado) + sizeof(N) + sizeof(Id_Sesion) + sizeof(Conteo) +
                                sizeof(Candidatos) + sizeof(Amigo) + sizeof(Candidato) + sizeof(Mejor) + sizeof(Encontradas) + sizeof(Id_C));

    // Liberacion del Arreglo Temporal (la Lista de Candidatos se Libera Sola al Salir)
    delete[] Conteo;                                            // Se Libera el Arreglo
    Recursos::liberar(static_cast<long>(N) * sizeof(int));      // Se Registra

}

// Funcion que Muestra las Sugerencias y Permite Enviar Solicitudes
void UdeABookGestor::verSugerenciasAmistad(){   // Se Declara la Funcion de Interfaz de Sugerencias

    // Declaracion de Variables Necesarias
    int      K         = 0;         // Cantidad de Sugerencias Pedidas
    int*     Comunes   = nullptr;   // Amigos en Comun de Cada Sugerencia
    int      Mostradas = 0;         // Sugerencias Realmente Encontradas
    int      Opcion    = 0;         // Sugerencia Elegida
    Usuario* Sugerido  = nullptr;   // Usuario Sugerido

    // Inicio de la Medicion
    Recursos::reiniciar();   // Se Inicia la Medicion de Esta Funcionalidad
    Consola::titulo("SUGERENCIAS DE AMISTAD");

    // Lectura de K
    K = Consola::leerEntero("Cuantas sugerencias quiere ver? (1-20, 0 = volver): ", 0, MAX_SUGERENCIAS);   // Se Pide K
    if (K >= MIN_SUGERENCIAS){

        // Calculo de las Sugerencias
        Comunes = new int[K];                                        // Arreglo para los Conteos
        Recursos::reservar(static_cast<long>(K) * sizeof(int));      // Se Registra
        calcularSugerenciasAmistad(K, Comunes);                      // Se Calculan las K Mejores
        Mostradas = Sesion_Activa->cantidadSugerencias();            // Cuantas se Encontraron

        if (Mostradas == 0){                                                                 // Si no Hay Sugerencias
            Consola::aviso("No hay sugerencias por ahora. Agrega mas amigos para descubrir gente nueva.");
        }
        else {

            // Tabla de Sugerencias
            cout << endl << "    " << Consola::ajustar("#", 5) << Consola::ajustar("Usuario", 22)
                 << Consola::ajustar("Ciudad", 16) << "Amigos en comun" << endl;
            cout << "    " << string(58, '-') << endl;
            for (int i = 0 ; i < Mostradas ; i++){                                           // Recorre Cada Sugerencia
                Recursos::contarIteracion();                                                 // Registra la Iteracion
                Sugerido = Sesion_Activa->obtenerSugerencia(i);                              // Usuario Sugerido
                cout << "    " << Consola::ajustar(to_string(i + 1) + ".", 5)
                     << Consola::ajustar(Sugerido->obtenerNombreUsuario(), 22)
                     << Consola::ajustar(Sugerido->obtenerCiudad(), 16) << Comunes[i] << endl;
            }
            if (Mostradas < K){                                                              // Si se Encontraron Menos de K
                Consola::aviso("Solo se encontraron " + to_string(Mostradas) + " sugerencia(s).");
            }

            // Ciclo para Enviar Solicitudes a las Sugerencias
            while (true){
                Recursos::contarIteracion();                                                       // Registra la Iteracion
                Opcion = Consola::leerEntero("Numero para enviar solicitud (0 = volver): ", 0, Mostradas);   // Se Pide la Sugerencia
                if (Opcion == 0){   // Si Quiere Volver
                    break;          // Fin del Ciclo
                }
                Sugerido = Sesion_Activa->obtenerSugerencia(Opcion - 1);   // Sugerencia Elegida
                if (puedeEnviarSolicitud(Sugerido, true)){                 // Si Cumple las Reglas
                    Sesion_Activa->enviarSolicitud(Sugerido);              // Se Envia la Solicitud
                    Cambios_Amistades = true;                              // Hay Cambios por Guardar
                    Consola::exito("Solicitud enviada a " + Sugerido->obtenerNombreUsuario() + ".");
                }
            }
        }
    }

    // Medicion de Recursos (Antes de Liberar para Reportar la Memoria en Uso)
    Recursos::sumarMemoriaLocal(sizeof(K) + sizeof(Comunes) + sizeof(Mostradas) + sizeof(Opcion) + sizeof(Sugerido));
    medirConsumoRecursos("Ver sugerencias de amistad (K = " + to_string(K) + ")");

    // Liberacion del Arreglo Temporal
    if (Comunes != nullptr){                                        // Si se Reservo el Arreglo
        delete[] Comunes;                                           // Se Libera
        Recursos::liberar(static_cast<long>(K) * sizeof(int));      // Se Registra
    }

}

// ============================================================================
//                       VI - ADMINISTRAR MENSAJES
// ============================================================================

// Funcion a. Enviar un Mensaje a un Amigo
void UdeABookGestor::enviarMensaje(){   // Se Declara la Funcion de Envio de Mensajes

    // Declaracion de Variables Necesarias
    Usuario* Amigo     = nullptr;   // Amigo que Recibira el Mensaje
    string   Contenido = "";        // Texto del Mensaje
    Mensaje* Nuevo     = nullptr;   // Mensaje Creado

    // Inicio de la Medicion
    Recursos::reiniciar();   // Se Inicia la Medicion de Esta Funcionalidad
    Consola::titulo("ENVIAR MENSAJE");

    // Eleccion del Amigo
    Amigo = pedirAmigo("Nombre del amigo");   // Se Pide y Valida el Amigo
    if (Amigo != nullptr){
        Consola::aviso("Escribe tu mensaje (maximo " + to_string(LIMITE_MENSAJE) + " caracteres) y presiona ENTER.");

        // Ciclo que se Repite Hasta Tener un Mensaje Valido o Cancelar
        while (true){
            Recursos::contarIteracion();                       // Registra la Iteracion
            Contenido = Consola::leerLinea("Mensaje (0 = cancelar): ");   // Se Pide el Texto
            if (Contenido == "0"){                             // Si Quiere Cancelar
                Consola::aviso("Envio cancelado.");
                break;                                         // Fin del Ciclo
            }
            if (Contenido.empty()){                            // Si el Mensaje esta Vacio
                Consola::error("El mensaje no puede estar vacio.");
                continue;                                      // Se Vuelve a Pedir
            }
            if (static_cast<int>(Contenido.size()) > LIMITE_MENSAJE){   // Si Supera el Limite
                Consola::error("El mensaje tiene " + to_string(Contenido.size()) + " caracteres; el maximo es " + to_string(LIMITE_MENSAJE) + ".");
                continue;                                               // Se Vuelve a Pedir
            }

            // Creacion y Entrega del Mensaje (la Fecha se Toma del Reloj con <ctime>)
            Nuevo = new Mensaje(Sesion_Activa, Amigo, Contenido, Fecha(), false);   // Se Crea en el Heap
            Recursos::reservar(sizeof(Mensaje));                                    // Se Registra el Objeto
            Sesion_Activa->enviarMensaje(Nuevo);                                    // Queda en mis Enviados
            Amigo->recibirMensaje(Nuevo);                                           // Queda en sus Recibidos
            Cambios_Mensajes = true;                                                // Hay Cambios por Guardar
            Consola::exito("Mensaje enviado a " + Amigo->obtenerNombreUsuario() + " el " + Nuevo->obtenerFechaEnvio().aTexto() + ".");
            break;                                                                  // Fin del Ciclo
        }
    }

    // Medicion de Recursos
    Recursos::sumarMemoriaLocal(sizeof(Amigo) + sizeof(Contenido) + Recursos::memoriaCadena(Contenido) + sizeof(Nuevo));
    medirConsumoRecursos("Enviar mensaje");

}

// Funcion b. Leer los Mensajes Nuevos (no Leidos) en Orden de Fecha
void UdeABookGestor::leerMensajesNuevos(){   // Se Declara la Funcion de Lectura de Mensajes

    // Declaracion de Variables Necesarias
    int      Total  = Sesion_Activa->cantidadRecibidos();   // Mensajes Recibidos
    int      Nuevos = 0;                                    // Mensajes Nuevos Mostrados
    Mensaje* Actual = nullptr;                              // Mensaje que se Revisa

    // Inicio de la Medicion
    Recursos::reiniciar();   // Se Inicia la Medicion de Esta Funcionalidad
    Consola::titulo("MENSAJES NUEVOS");

    // Ciclo que Recorre los Recibidos (ya Estan Ordenados por Fecha)
    for (int i = 0 ; i < Total ; i++){                      // Recorre Cada Mensaje Recibido
        Recursos::contarIteracion();                        // Registra la Iteracion
        Actual = Sesion_Activa->obtenerRecibido(i);         // Mensaje Actual
        if (Actual->estaLeido() == false){                  // Si no se ha Leido
            Nuevos++;                                       // Se Cuenta
            cout << endl << "  Mensaje nuevo #" << Nuevos << endl;
            cout << *Actual;                                // Sobrecarga << de Mensaje
            Actual->marcarComoLeido();                      // Se Marca como Leido
            Sesion_Activa->descontarNoLeido();              // Se Actualiza el Contador
        }
    }

    // Resultado
    if (Nuevos == 0){                                       // Si no Habia Mensajes Nuevos
        Consola::aviso("No tienes mensajes nuevos.");
    }
    else {                                                  // Si se Leyeron Mensajes
        Cambios_Mensajes = true;                            // El Estado Cambio y Debe Guardarse
        cout << endl;
        Consola::exito(to_string(Nuevos) + " mensaje(s) marcado(s) como leido(s).");
    }

    // Medicion de Recursos
    Recursos::sumarMemoriaLocal(sizeof(Total) + sizeof(Nuevos) + sizeof(Actual));   // Variables Locales
    medirConsumoRecursos("Leer mensajes nuevos");

}

// Funcion c. Buscar Mensajes Recibidos en un Rango de Fechas (Ambas Inclusive)
void UdeABookGestor::buscarMensajesPorFecha(){   // Se Declara la Funcion de Busqueda por Fecha

    // Declaracion de Variables Necesarias
    Fecha    Inicio;                                       // Fecha Inicial del Rango
    Fecha    Fin;                                          // Fecha Final del Rango
    Fecha    Fin_Dia;                                      // Fecha Final a las 23:59:59
    string   Texto       = "";                             // Texto Escrito por el Usuario
    bool     Valido      = false;                          // Indica si se Leyo un Rango Valido
    int      Total       = Sesion_Activa->cantidadRecibidos();   // Mensajes Recibidos
    int      Posicion    = 0;                              // Primer Mensaje del Rango
    int      Encontrados = 0;                              // Mensajes Dentro del Rango
    Mensaje* Actual      = nullptr;                        // Mensaje que se Revisa

    // Inicio de la Medicion
    Recursos::reiniciar();   // Se Inicia la Medicion de Esta Funcionalidad
    Consola::titulo("BUSCAR MENSAJES POR RANGO DE FECHA");

    // Ciclo de Lectura de la Fecha Inicial
    while (true){
        Recursos::contarIteracion();                                          // Registra la Iteracion
        Texto = Consola::leerLinea("Fecha inicial (AAAA-MM-DD, 0 = volver): ");
        if (Texto == "0"){ break; }                                           // Si Quiere Volver
        if (Fecha::desdeTexto(Texto, Inicio)){ Valido = true; break; }        // Si la Fecha es Valida
        Consola::error("Fecha invalida. Use el formato AAAA-MM-DD, por ejemplo 2026-08-15.");
    }

    // Ciclo de Lectura de la Fecha Final
    while (Valido){
        Recursos::contarIteracion();                                          // Registra la Iteracion
        Texto = Consola::leerLinea("Fecha final   (AAAA-MM-DD, 0 = volver): ");
        if (Texto == "0"){ Valido = false; break; }                           // Si Quiere Volver
        if (Fecha::desdeTexto(Texto, Fin) == false){                          // Si la Fecha es Invalida
            Consola::error("Fecha invalida. Use el formato AAAA-MM-DD.");
            continue;                                                         // Se Vuelve a Pedir
        }
        if (Fin < Inicio){                                                    // Si el Rango esta al Reves (Sobrecarga <)
            Consola::error("La fecha final no puede ser anterior a la inicial.");
            continue;                                                         // Se Vuelve a Pedir
        }
        break;                                                                // Rango Correcto
    }

    // Busqueda de los Mensajes del Rango
    if (Valido){
        Fin_Dia  = Fin.finDelDia();                                     // El Ultimo Dia se Incluye Completo
        Posicion = Sesion_Activa->buscarPrimerRecibidoDesde(Inicio);    // Busqueda Binaria O(log M)

        cout << endl << "  Mensajes recibidos entre " << Inicio.aTextoCorto() << " y " << Fin.aTextoCorto() << ":" << endl;

        // Ciclo que Recorre Solo los Mensajes del Rango (Termina al Pasar la Fecha Final)
        for (int i = Posicion ; i < Total ; i++){                       // Desde el Primero del Rango
            Recursos::contarIteracion();                                // Registra la Iteracion
            Actual = Sesion_Activa->obtenerRecibido(i);                 // Mensaje Actual
            if (Actual->obtenerFechaEnvio() > Fin_Dia){                 // Si ya se Paso del Rango (Sobrecarga >)
                break;                                                  // Los Siguientes Tambien Estan Fuera
            }
            cout << *Actual;                                            // Sobrecarga << de Mensaje
            Encontrados++;                                              // Se Cuenta
        }

        // Resultado
        if (Encontrados == 0){
            Consola::aviso("No hay mensajes recibidos en ese rango.");
        }
        else {
            cout << endl;
            Consola::exito("Se encontraron " + to_string(Encontrados) + " mensaje(s) en el rango.");
        }
    }

    // Medicion de Recursos
    Recursos::sumarMemoriaLocal(3 * sizeof(Fecha) + sizeof(Texto) + sizeof(Valido) + sizeof(Total) + sizeof(Posicion) + sizeof(Encontrados) + sizeof(Actual));
    medirConsumoRecursos("Buscar mensajes por rango de fecha");

}

// ============================================================================
//                     VII - MEDICION DEL CONSUMO DE RECURSOS
// ============================================================================

// Funcion que Muestra las Metricas de la Funcionalidad que Acaba de Terminar
void UdeABookGestor::medirConsumoRecursos(const string& Funcionalidad) const{   // Se Declara la Funcion de Medicion
    Recursos::mostrarReporte(Funcionalidad);                                    // Se Imprime el Reporte (Iteraciones y Memoria)
}
