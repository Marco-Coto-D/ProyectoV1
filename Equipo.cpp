//
// Created by marco on 4/13/2026.
//

#include "Equipo.h"
#include <ctime>

Equipo::Equipo(string nombre, string ID, int criticidad, double prioridad){
    this->nombre = nombre;
    this->ID = ID;
    this->estado = 100;
    this->criticidad = criticidad;
    this->tiempoInactivo = 0;
    this->prioridad = prioridad;
}

void Equipo::degradar(){
    double desgaste = rand() % 5 + 1;
    estado = estado - desgaste;
    tiempoInactivo++;
}

double Equipo::calcularPrioridad(){

}

void Equipo::aplicarMantenimiento(){

}
