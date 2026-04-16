#ifndef PROYECTOV1_INCIDENCIAMEDIA_H
#define PROYECTOV1_INCIDENCIAMEDIA_H
#include "Incidencia.h"


class IncidenciaMedia : public Incidencia {
    public:
    IncidenciaMedia();
    int getGravedad() const override;
    string getTipo() const override;
};


#endif //PROYECTOV1_INCIDENCIAMEDIA_H
