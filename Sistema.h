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

    // Tres estrategias de mantenimiento (patron Strategy)
    MantenimientoCorrectivo correctivo;
    MantenimientoPreventivo preventivo;
    MantenimientoPredictivo predictivo;

    // Incidencias del archivo que esperan su dia de activacion
    vector<IncidenciaPendiente> incidenciasPendientes;

    // Indice ordenado por ID para busqueda binaria
    vector<int> equiposOrdenadosPorId;

    // Estadisticas acumuladas para el reporte final
    int totalCorrectivos;
    int totalPreventivos;
    int totalPredictivos;
    int totalIncidenciasGeneradas;
    double sumaRiesgoGlobal;

    int buscarEquipo(string id);
    void ordenarIndice();
    Mantenimiento* seleccionarEstrategia(Equipo& equipo);
    void escribir(string texto);

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
