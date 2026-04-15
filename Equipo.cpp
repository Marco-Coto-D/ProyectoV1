//
// Created by marco on 4/13/2026.
//

#include "Equipo.h"
#include <ctime>

Equipo::Equipo(string nombre, string ID, int criticidad){
    this->nombre = nombre;
    this->ID = ID;
    this->estado = 100;
    this->criticidad = criticidad;
    this->tiempoInactivo = 0;
    this->prioridad = 0;
}

void Equipo::degradar(){ //Desgaste el equipo aleatoriamemte diariamente
    double desgaste = rand() % 5 + 1;
    estado = estado - desgaste;
    tiempoInactivo++;
}

double Equipo::calcularPrioridad(){ //falta terminar la llamada de incidencias
    return prioridad = (criticidad * 0.5) + (incidencias * 0.3) + (tiempoInactivo * 0.2);
}

void Equipo::agregarIncidencia(Incidencia* inc){
    incidencias.push_back(inc);
}
