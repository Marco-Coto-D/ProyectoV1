#ifndef PROYECTOV1_EXCEPCIONES_H
#define PROYECTOV1_EXCEPCIONES_H
#include <stdexcept>
#include <string>

// AGREGADO (Paso 1): jerarquía de excepciones propias requerida por §7 del PDF
// Cada clase representa un tipo de error distinto del sistema

class ArchivoInvalidoException : public std::runtime_error { // AGREGADO: error al abrir archivos
public:
    explicit ArchivoInvalidoException(const std::string& msg)
        : std::runtime_error("ArchivoInvalido: " + msg) {}
};

class FormatoInvalidoException : public std::runtime_error { // AGREGADO: línea mal formada en equipos.txt
public:
    explicit FormatoInvalidoException(const std::string& msg)
        : std::runtime_error("FormatoInvalido: " + msg) {}
};

class OperacionInconsistenteException : public std::runtime_error { // AGREGADO: INC referencia equipo inexistente
public:
    explicit OperacionInconsistenteException(const std::string& msg)
        : std::runtime_error("OperacionInconsistente: " + msg) {}
};

#endif //PROYECTOV1_EXCEPCIONES_H
