//
// Created by marco on 4/15/2026.
//

#ifndef PROYECTOV1_INCIDENCIABAJA_H
#define PROYECTOV1_INCIDENCIABAJA_H
#include "Incidencia.h"
#include <iostream>
using namespace std;


class IncidenciaBaja : public Incidencia {
    public:
    IncidenciaBaja();
    int getGravedad() const override;
    string getTipo() const override;
};


#endif //PROYECTOV1_INCIDENCIABAJA_H