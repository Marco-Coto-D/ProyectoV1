#ifndef PROYECTOV1_INCIDENCIAMEDIA_H
#define PROYECTOV1_INCIDENCIAMEDIA_H
#include "Incidencia.h"
#include <iostream>
using namespace std;


class IncidenciaMedia : public Incidencia {
    public:
    IncidenciaMedia();
    int getGravedad() const override;
    string getTipo() const override;
};


#endif //PROYECTOV1_INCIDENCIAMEDIA_H
