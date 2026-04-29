#ifndef PROYECTOV1_MANTENIMIENTO_H
#define PROYECTOV1_MANTENIMIENTO_H
#include "Equipo.h"
#include <string>
#include <iostream>
using namespace std;


class Mantenimiento {
public:
    virtual ~Mantenimiento() {}
    virtual void aplicar(Equipo* equipo) = 0;
    virtual string getTipo() const = 0;
};

#endif //PROYECTOV1_MANTENIMIENTO_H
