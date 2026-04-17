//
// Created by mcoto on 4/16/2026.
//

#ifndef PROYECTOV1_SISTEMA_H
#define PROYECTOV1_SISTEMA_H
#include <iostream>
#include <vector>
#include "Mantenimiento.h"
#include "Incidencia.h"
#include "Equipo.h"
using namespace std;


class Sistema {
private:
    vector<Equipo> equipos;
    int diaActual;
    Mantenimiento mantenimientos;
    public:
    Sistema();
    void simular();
    void simularDia();
    void generarIncidencias();
    void ordenarEquipos();
    void aplicarMantenimientos();
    void generarReporte();
};



#endif //PROYECTOV1_SISTEMA_H
