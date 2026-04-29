#include "MantenimientoPredictivo.h"

void MantenimientoPredictivo::aplicar(Equipo* equipo) {
    double nuevoEstado = equipo->getEstado() + 15.0;
    if (nuevoEstado > 100.0) nuevoEstado = 100.0;
    equipo->setEstado(nuevoEstado);
    equipo->setTiempoInactivo(0);
    equipo->limpiarIncidenciasGraves();
    equipo->registrarMantenimiento();
}
