#ifndef PROYECTOV1_SISTEMA_H
#define PROYECTOV1_SISTEMA_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "MantenimientoCorrectivo.h"
#include "MantenimientoPreventivo.h"
#include "MantenimientoPredictivo.h"
#include "Incidencia.h"
#include "Equipo.h"
using namespace std;

struct IncidenciaPendiente {
    string idEquipo;
    string severidad;
    int diaActivacion;
};

class Sistema {
private:
    vector<Equipo> equipos;
    int diaActual;
    ofstream logDiario;


    MantenimientoCorrectivo correctivo;
    MantenimientoPreventivo preventivo;
    MantenimientoPredictivo predictivo;


    vector<IncidenciaPendiente> incidenciasPendientes;


    vector<int> equiposOrdenadosPorId;


    int totalCorrectivos;
    int totalPreventivos;
    int totalPredictivos;
    int totalIncidenciasGeneradas;
    double sumaRiesgoGlobal;

    void abrirArchivo();
    void separarObjetos(string linea, int numLinea);
    void construirIndice();
    int buscarEquipo(string id);
    void ordenarPorId();
    Mantenimiento* seleccionarEstrategia(Equipo& equipo);
    void guardarEnLog(string texto);

public:
    Sistema();
    void simular();
    void simularDia();
    void generarIncidencias();
    void aplicarIncidenciasPendientes();
    void ordenarEquipos();
    void aplicarMantenimientos();
    void generarReporte();
};

#endif //PROYECTOV1_SISTEMA_H
