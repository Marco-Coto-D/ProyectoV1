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

    // AGREGADO (Paso 7): incidencias del archivo esperando su día de activación
    vector<IncidenciaPendiente> incidenciasPendientes;

    // AGREGADO (Paso 4): índice de posiciones ordenado por ID para búsqueda binaria
    vector<int> equiposOrdenadosPorId;

    // AGREGADO (Paso 8): estadísticas acumuladas del reporte final
    int totalCorrectivos;
    int totalPreventivos;
    int totalPredictivos;
    int totalIncidenciasGeneradas;
    double sumaRiesgoGlobal; // acumula el riesgo promedio diario para calcular promedio final

    // AGREGADO (Paso 4): búsqueda binaria O(log n) sobre equiposOrdenadosPorId
    int buscarEquipoBinaria(const string& id) const;

    // AGREGADO (Paso 2/3): selecciona la estrategia según el estado del equipo (usa dynamic_cast internamente)
    Mantenimiento* seleccionarEstrategia(Equipo& equipo);

    // helper: escribe en consola y en el archivo de log simultáneamente
    void escribir(const string& texto);

public:
    Sistema();
    void simular();
    void simularDia();
    void generarIncidencias();
    void aplicarIncidenciasPendientes(); // AGREGADO (Paso 7): aplica las INC cuyo día llegó
    void ordenarEquipos();
    void aplicarMantenimientos();
    void generarReporte();
};

#endif //PROYECTOV1_SISTEMA_H
