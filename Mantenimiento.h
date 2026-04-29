#ifndef PROYECTOV1_MANTENIMIENTO_H
#define PROYECTOV1_MANTENIMIENTO_H
#include "Equipo.h"
#include <string>


class Mantenimiento {
public:
    virtual ~Mantenimiento() = default;
    virtual void aplicar(Equipo* equipo) = 0;
    virtual std::string getTipo() const = 0;
};

#endif //PROYECTOV1_MANTENIMIENTO_H
