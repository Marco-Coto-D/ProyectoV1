#ifndef PROYECTOV1_EQUIPO_H
#define PROYECTOV1_EQUIPO_H
#include <iostream>
#include <vector>
using namespace std;

class Incidencia;

class Equipo {
private:
    string nombre;
    string ID;
    double estado;
    int criticidad;
    int tiempoInactivo;
    double prioridad;
    int contadorMantenimientos;
    vector<Incidencia*> incidencias;
public:
    Equipo(string nombre, string ID, int criticidad);
    ~Equipo();
    void swap(Equipo& other);
    string getID() const;
    double getEstado();
    int getCriticidad();
    int getTiempoInactivo();
    double getPrioridad();
    int getContadorMantenimientos();
    const vector<Incidencia*>& getIncidencias();
    void setEstado(double estado);
    void setCriticidad(int criticidad);
    void setTiempoInactivo(int tiempoInactivo);
    void setPrioridad(double prioridad);
    void degradar();
    double calcularPrioridad();
    void agregarIncidencia(Incidencia* inc);
    int gravedadIncidencias();
    void limpiarIncidencias();
    void limpiarIncidenciasGraves();
    void registrarMantenimiento();
    void mostrar();
};

#endif //PROYECTOV1_EQUIPO_H
