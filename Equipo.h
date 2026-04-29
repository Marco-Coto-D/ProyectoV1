#ifndef PROYECTOV1_EQUIPO_H
#define PROYECTOV1_EQUIPO_H
#include <iostream>
#include <vector>
using namespace std;

class Incidencia; // forward declaration — dependencia mutua resuelta correctamente (§8.3)

class Equipo {
private:
    string nombre;
    string ID;
    double estado;
    int criticidad;
    int tiempoInactivo;
    double prioridad;
    int contadorMantenimientos;          // AGREGADO (Paso 8): acumula cuántas veces fue atendido en 30 días
    vector<Incidencia*> incidencias;
public:
    Equipo(string nombre, string ID, int criticidad);

    // AGREGADO (Paso 6): destructor libera todos los Incidencia* al destruirse el equipo
    ~Equipo();

    // MODIFICADO (Paso 6/9): intercambia todos los campos con otro equipo campo por campo,
    // usando el swap interno del vector para los punteros — evita copias superficiales
    void swap(Equipo& other);

    string getID() const; // const permite llamarlo desde métodos const como buscarEquipoBinaria
    double getEstado();
    int getCriticidad();
    int getTiempoInactivo();
    double getPrioridad();
    int getContadorMantenimientos();              // AGREGADO (Paso 8)
    const vector<Incidencia*>& getIncidencias(); // AGREGADO (Paso 3): necesario para dynamic_cast en Sistema

    void setEstado(double estado);
    void setCriticidad(int criticidad);
    void setTiempoInactivo(int tiempoInactivo);
    void setPrioridad(double prioridad);

    void degradar();
    double calcularPrioridad();
    void agregarIncidencia(Incidencia*);
    int gravedadIncidencias();
    void limpiarIncidencias();
    void limpiarIncidenciasGraves(); // AGREGADO (Paso 2): elimina solo las de gravedad >= 3 (ALTA y MEDIA)
    void registrarMantenimiento();   // AGREGADO (Paso 8): incrementa contadorMantenimientos
    void mostrar();
};

#endif //PROYECTOV1_EQUIPO_H
