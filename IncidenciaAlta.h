#ifndef PROYECTOV1_INCIDENCIAALTA_H
#define PROYECTOV1_INCIDENCIAALTA_H
#include "Incidencia.h"
#include <iostream>
using namespace std;

class IncidenciaAlta : public Incidencia {
public:
    IncidenciaAlta();
    int getGravedad() const override;
    string getTipo() const override;
    bool requiereParoInmediato() const {
        return true;
    }
};

#endif //PROYECTOV1_INCIDENCIAALTA_H
