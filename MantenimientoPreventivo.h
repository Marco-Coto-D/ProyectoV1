#ifndef PROYECTOV1_MANTENIMIENTOPREVENTIVO_H
#define PROYECTOV1_MANTENIMIENTOPREVENTIVO_H
#include "Mantenimiento.h"
#include <iostream>
using namespace std;

class MantenimientoPreventivo : public Mantenimiento {
public:
    void aplicar(Equipo* equipo) override;
    string getTipo() const override { //.cpp
        return "Preventivo";
    }
};

#endif //PROYECTOV1_MANTENIMIENTOPREVENTIVO_H
