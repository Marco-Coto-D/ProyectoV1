//
// Created by mcoto on 4/16/2026.
//

#include "Mantenimiento.h"

void Mantenimiento::aplicar(Equipo *equipo) {
    equipo->setEstado(100);
    equipo->setTiempoInactivo(0);
    equipo->limpiarIncidencias();
}