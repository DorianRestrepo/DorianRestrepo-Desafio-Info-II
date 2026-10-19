#ifndef MENSAJE_H
#define MENSAJE_H

#include <string>
#include <iostream>
#include "Fecha.h"

using namespace std;

class Usuario;


class Mensaje {

private:

    Usuario* Emisor;
    Usuario* Receptor;
    string   Contenido;
    Fecha    Fecha_Envio;
    bool     Estado_Leido;

public:

    // Constructor y Destructor
    Mensaje(Usuario* Emisor, Usuario* Receptor, const string& Contenido, const Fecha& Fecha_Envio, bool Estado_Leido);
    ~Mensaje();


    Mensaje(const Mensaje& Otro) = delete;
    Mensaje& operator=(const Mensaje& Otro) = delete;

    // Getters
    Usuario*      obtenerEmisor() const;
    Usuario*      obtenerReceptor() const;
    const string& obtenerContenido() const;
    const Fecha&  obtenerFechaEnvio() const;
    bool          estaLeido() const;


    bool marcarComoLeido();

    bool operator<(const Mensaje& Otro) const;

    long memoriaCadenas() const;

    friend ostream& operator<<(ostream& Salida, const Mensaje& Objeto);
};

#endif // MENSAJE_H
