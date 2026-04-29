#ifndef PROYECTOV1_EXCEPCIONES_H
#define PROYECTOV1_EXCEPCIONES_H
#include <stdexcept>
#include <string>
#include <iostream>
using namespace std;

class ArchivoInvalidoException : public runtime_error {
public:
    explicit ArchivoInvalidoException(const string& msg)
        : runtime_error("ArchivoInvalido: " + msg) {}
};

class FormatoInvalidoException : public runtime_error {
public:
    explicit FormatoInvalidoException(const string& msg)
        : runtime_error("FormatoInvalido: " + msg) {}
};

class OperacionInconsistenteException : public runtime_error {
public:
    explicit OperacionInconsistenteException(const string& msg)
        : runtime_error("OperacionInconsistente: " + msg) {}
};

#endif //PROYECTOV1_EXCEPCIONES_H
