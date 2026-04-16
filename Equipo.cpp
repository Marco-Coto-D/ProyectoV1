//
// Created by marco on 4/13/2026.
//

#include "Equipo.h"
#include "Incidencia.h"

Equipo::Equipo(string nombre, string ID, int criticidad){
    this->nombre = nombre;
    this->ID = ID;
    this->estado = 100;
    this->criticidad = criticidad;
    this->tiempoInactivo = 0;
    this->prioridad = 0;
}

string Equipo::getID() {
    return this->ID;
}

double Equipo::getEstado(){
    return this->estado;
}

int Equipo::getCriticidad(){
    return this->criticidad;
}

int Equipo::getTiempoInactivo(){
    return this->tiempoInactivo;
}

double Equipo::getPrioridad(){
    return this->prioridad;
}

void Equipo::setEstado(double estado){
    this->estado = estado;
}

void Equipo::setCriticidad(int criticidad){
    this->criticidad = criticidad;
}

void Equipo::setTiempoInactivo(int tiempoInactivo){
    this->tiempoInactivo = tiempoInactivo;
}

void Equipo::setPrioridad(double prioridad){
    this->prioridad = prioridad;
}

void Equipo::degradar(){ //Desgaste el equipo aleatoriamemte diariamente
    double desgaste = rand() % 5 + 1;
    estado = estado - desgaste;
    if (estado < 0 ) {
        estado = 0;
    }
    tiempoInactivo++;
}

double Equipo::calcularPrioridad(){
    return prioridad = (criticidad * 0.5) + (gravedadIncidencias() * 0.3) + (tiempoInactivo * 0.2);
}

void Equipo::agregarIncidencia(Incidencia* inc){
    incidencias.push_back(inc);
}

int Equipo::gravedadIncidencias(){
    int total = 0;
    for(int i = 0; i < incidencias.size(); i++)
    {
        total += incidencias[i]->getGravedad();
    }
    return total;
}

void Equipo::limpiarIncidencias() {
    for (Incidencia * inc : incidencias) {
        delete inc;
    }
    incidencias.clear();
}

void Equipo::mostrar(){
    cout<<this->nombre<<endl;
    cout<<"ID: "<<this->ID<<endl;
    cout<<"El estado actual del equipo es: "<<this->estado<<endl;
    cout<<"Su ultimo uso fue hace: "<<this->tiempoInactivo<<" dias."<<endl;
}
