#include "MantenimientoCorrectivo.h"
#include <iostream>
using namespace std;

void MantenimientoCorrectivo::aplicar(Equipo* equipo) {
    equipo->setEstado(100);
    equipo->setTiempoInactivo(0);
    equipo->limpiarIncidencias();
    equipo->registrarMantenimiento();
}
