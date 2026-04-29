//
// Created by marco on 4/15/2026.
//

#ifndef PROYECTOV1_INCIDENCIA_H
#define PROYECTOV1_INCIDENCIA_H
#include <iostream>
using namespace std;

class Equipo;

class Incidencia {
protected:
    Equipo* equipoOwner;
public:
    Incidencia() { equipoOwner = nullptr; }
    virtual int getGravedad() const = 0;
    virtual string getTipo() const = 0;
    virtual ~Incidencia() {}
    void setEquipo(Equipo* e) { equipoOwner = e; } //.cpp
    Equipo* getEquipo() const { return equipoOwner; } //.cpp
};


#endif //PROYECTOV1_INCIDENCIA_H