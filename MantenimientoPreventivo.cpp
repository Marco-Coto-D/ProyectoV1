#include "MantenimientoPreventivo.h"
#include <iostream>
using namespace std;

void MantenimientoPreventivo::aplicar(Equipo* equipo) {
    double nuevoEstado = equipo->getEstado() + 30.0;
    if (nuevoEstado > 100.0) nuevoEstado = 100.0;
    equipo->setEstado(nuevoEstado);
    equipo->setTiempoInactivo(0);
    equipo->limpiarIncidencias();
    equipo->registrarMantenimiento();
}
