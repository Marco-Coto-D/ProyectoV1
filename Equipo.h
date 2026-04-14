//
// Created by marco on 4/13/2026.
//

#ifndef PROYECTOV1_EQUIPO_H
#define PROYECTOV1_EQUIPO_H
#include <iostream>
using namespace std;
#include <vector>


class Equipo {
private:
    string nombre;
    string ID;
    double estado;
    int criticidad;
    int tiempoInactivo;
    double prioridad; //De este atributo depende cual equipo recibe mantenimiento
    vector<*Incidencias> incidencias;
public:
    Equipo(string nombre, string ID, int criticidad, double prioridad);
    void degradar(); //Metodo para degradar el equipo por dia
    double calcularPrioridad();
    void aplicarMantenimiento();
};


#endif //PROYECTOV1_EQUIPO_H