#ifndef PROYECTOV1_MANTENIMIENTOPREDICTIVO_H
#define PROYECTOV1_MANTENIMIENTOPREDICTIVO_H
#include "Mantenimiento.h"
#include <iostream>
using namespace std;


class MantenimientoPredictivo : public Mantenimiento {
public:
    void aplicar(Equipo* equipo) override;
    string getTipo() const override { //.cpp
        return "Predictivo";
    }
};

#endif //PROYECTOV1_MANTENIMIENTOPREDICTIVO_H
