//
// Created by marco on 4/15/2026.
//

#ifndef PROYECTOV1_INCIDENCIA_H
#define PROYECTOV1_INCIDENCIA_H
#include <iostream>
using namespace std;

class Incidencia{
    public:
    virtual int getGravedad() const = 0;
    virtual string getTipo() const = 0;
    virtual ~Incidencia(){};
};


#endif //PROYECTOV1_INCIDENCIA_H