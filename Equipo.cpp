#include "Equipo.h"
#include "Incidencia.h"
#include <iostream>
using namespace std;

Equipo::Equipo(string nombre, string ID, int criticidad) {
    this->nombre = nombre;
    this->ID = ID;
    this->estado = 100;
    this->criticidad = criticidad;
    this->tiempoInactivo = 0;
    this->prioridad = 0;
    this->contadorMantenimientos = 0;
}

Equipo::~Equipo() {
    limpiarIncidencias();
}

// Intercambia todos los campos con otro equipo campo por campo.
// El swap del vector transfiere los punteros sin copiarlos.
void Equipo::swap(Equipo& other) {
    string tmpNombre = this->nombre;
    this->nombre = other.nombre;
    other.nombre = tmpNombre;

    string tmpID = this->ID;
    this->ID = other.ID;
    other.ID = tmpID;

    double tmpEstado = this->estado;
    this->estado = other.estado;
    other.estado = tmpEstado;

    int tmpCrit = this->criticidad;
    this->criticidad = other.criticidad;
    other.criticidad = tmpCrit;

    int tmpTI = this->tiempoInactivo;
    this->tiempoInactivo = other.tiempoInactivo;
    other.tiempoInactivo = tmpTI;

    double tmpPrior = this->prioridad;
    this->prioridad = other.prioridad;
    other.prioridad = tmpPrior;

    int tmpCont = this->contadorMantenimientos;
    this->contadorMantenimientos = other.contadorMantenimientos;
    other.contadorMantenimientos = tmpCont;

    this->incidencias.swap(other.incidencias);
}

string Equipo::getID() const {
    return this->ID;
}
double Equipo::getEstado() {
    return this->estado;
}
int Equipo::getCriticidad() {
    return this->criticidad;
}
int Equipo::getTiempoInactivo() {
    return this->tiempoInactivo;
}
double Equipo::getPrioridad() {
    return this->prioridad;
}
int Equipo::getContadorMantenimientos() {
    return this->contadorMantenimientos;
}

const vector<Incidencia*>& Equipo::getIncidencias() {
    return this->incidencias;
}

void Equipo::setEstado(double estado) {
    this->estado = estado;
}
void Equipo::setCriticidad(int criticidad) {
    this->criticidad = criticidad;
}
void Equipo::setTiempoInactivo(int tiempoInactivo) {
    this->tiempoInactivo = tiempoInactivo;
}
void Equipo::setPrioridad(double prioridad) {
    this->prioridad = prioridad;
}

void Equipo::degradar() {
    double desgaste = rand() % 5 + 1;
    estado -= desgaste;
    if (estado < 0) estado = 0;
    tiempoInactivo++;
}


double Equipo::calcularPrioridad() {
    double incActivas = (double)incidencias.size();
    return prioridad = (criticidad * 0.5) + (incActivas * 0.3) + (tiempoInactivo * 0.2);
}

void Equipo::agregarIncidencia(Incidencia* inc) {
    inc->setEquipo(this);
    incidencias.push_back(inc);
}

int Equipo::gravedadIncidencias() {
    int total = 0;
    for (size_t i = 0; i < incidencias.size(); i++) {
        total += incidencias[i]->getGravedad();
    }
    return total;
}

void Equipo::limpiarIncidencias() {
    for (size_t i = 0; i < incidencias.size(); i++) {
        delete incidencias[i];
    }
    incidencias.clear();
}

void Equipo::limpiarIncidenciasGraves() {
    vector<Incidencia*> restantes;
    for (size_t i = 0; i < incidencias.size(); i++) {
        if (incidencias[i]->getGravedad() >= 3) {
            delete incidencias[i];
        } else {
            restantes.push_back(incidencias[i]);
        }
    }
    incidencias = restantes;
}

void Equipo::registrarMantenimiento() {
    contadorMantenimientos++;
}

void Equipo::mostrar() {
    cout << this->nombre << endl;
    cout << "ID: " << this->ID << endl;
    cout << "El estado actual del equipo es: " << this->estado << endl;
    cout << "Su ultimo uso fue hace: " << this->tiempoInactivo << " dias." << endl;
}

