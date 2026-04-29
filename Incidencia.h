//
// Created by marco on 4/15/2026.
//

#ifndef PROYECTOV1_INCIDENCIA_H
#define PROYECTOV1_INCIDENCIA_H
#include <iostream>
using namespace std;

class Equipo; // forward declaration — dependencia mutua con Equipo (§8.3)

class Incidencia {
protected:
    Equipo* equipoOwner; // puntero de vuelta al equipo que contiene esta incidencia
public:
    Incidencia() { equipoOwner = nullptr; }
    virtual int getGravedad() const = 0;
    virtual string getTipo() const = 0;
    virtual ~Incidencia() {}
    void setEquipo(Equipo* e) { equipoOwner = e; }
    Equipo* getEquipo() const { return equipoOwner; }
};


#endif //PROYECTOV1_INCIDENCIA_H