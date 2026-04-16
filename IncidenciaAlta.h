#ifndef PROYECTOV1_INCIDENCIAALTA_H
#define PROYECTOV1_INCIDENCIAALTA_H
#include "Incidencia.h"


class IncidenciaAlta : public Incidencia {
    public:
    IncidenciaAlta();
    int getGravedad() const override;
    string getTipo() const override;
};


#endif //PROYECTOV1_INCIDENCIAALTA_H
