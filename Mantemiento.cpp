//
// Created by mcoto on 4/16/2026.
//

#include "Mantemiento.h"

void Mantemiento::aplicar(Equipo *equipo) {
    equipo->setEstado(100);
    equipo->setTiempoInactivo(0);
    equipo->limpiarIncidencias();
}