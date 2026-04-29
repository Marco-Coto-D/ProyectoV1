#ifndef PROYECTOV1_MANTENIMIENTOCORRECTIVO_H
#define PROYECTOV1_MANTENIMIENTOCORRECTIVO_H
#include "Mantenimiento.h"
#include <iostream>
using namespace std;


class MantenimientoCorrectivo : public Mantenimiento {
public:
    void aplicar(Equipo* equipo) override;
    string getTipo() const override{
        return "Correctivo";
    }
};

#endif //PROYECTOV1_MANTENIMIENTOCORRECTIVO_H
