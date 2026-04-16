//
// Created by marco on 4/13/2026.
//

#ifndef PROYECTOV1_EQUIPO_H
#define PROYECTOV1_EQUIPO_H
#include <iostream>
using namespace std;
#include <vector>

class Incidencia;

class Equipo {
private:
    string nombre;
    string ID;
    double estado;
    int criticidad;
    int tiempoInactivo;
    double prioridad; //De este atributo depende cual equipo recibe mantenimiento
    vector<Incidencia*> incidencias;
public:
    Equipo(string nombre, string ID, int criticidad);
    string getID();
    double getEstado();
    int getCriticidad();
    int getTiempoInactivo();
    double getPrioridad();
    void setEstado(double estado);
    void setCriticidad(int criticidad);
    void setTiempoInactivo(int tiempoInactivo);
    void setPrioridad(double prioridad);
    void degradar(); //Metodo para degradar el equipo por dia
    double calcularPrioridad();
    void agregarIncidencia(Incidencia*);
    int gravedadIncidencias();
    void limpiarIncidencias();
    void mostrar();
};


#endif //PROYECTOV1_EQUIPO_H