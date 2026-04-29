#include "Equipo.h"
#include "Incidencia.h"

Equipo::Equipo(string nombre, string ID, int criticidad) {
    this->nombre = nombre;
    this->ID = ID;
    this->estado = 100;
    this->criticidad = criticidad;
    this->tiempoInactivo = 0;
    this->prioridad = 0;
    this->contadorMantenimientos = 0; // AGREGADO (Paso 8): inicia en 0
}

// AGREGADO (Paso 6): destructor — libera todos los Incidencia* al destruirse el equipo
Equipo::~Equipo() {
    limpiarIncidencias();
}

// MODIFICADO (Paso 6/9): swap campo por campo usando variables temporales simples
// El swap del vector transfiere los punteros internos sin copiarlos ni duplicarlos
void Equipo::swap(Equipo& other) {
    string tmpNombre = this->nombre;      this->nombre = other.nombre;             other.nombre = tmpNombre;
    string tmpID     = this->ID;           this->ID     = other.ID;                 other.ID     = tmpID;
    double tmpEstado = this->estado;       this->estado = other.estado;             other.estado = tmpEstado;
    int tmpCrit      = this->criticidad;   this->criticidad = other.criticidad;     other.criticidad = tmpCrit;
    int tmpTI        = this->tiempoInactivo; this->tiempoInactivo = other.tiempoInactivo; other.tiempoInactivo = tmpTI;
    double tmpPrior  = this->prioridad;    this->prioridad = other.prioridad;       other.prioridad = tmpPrior;
    int tmpCont      = this->contadorMantenimientos; this->contadorMantenimientos = other.contadorMantenimientos; other.contadorMantenimientos = tmpCont;
    this->incidencias.swap(other.incidencias); // swap interno del vector — no copia punteros, solo transfiere el bloque de memoria
}

string Equipo::getID() const { return this->ID; }
double Equipo::getEstado() { return this->estado; }
int Equipo::getCriticidad() { return this->criticidad; }
int Equipo::getTiempoInactivo() { return this->tiempoInactivo; }
double Equipo::getPrioridad() { return this->prioridad; }
int Equipo::getContadorMantenimientos() { return this->contadorMantenimientos; } // AGREGADO (Paso 8)

// AGREGADO (Paso 3): expone las incidencias como referencia const para el dynamic_cast en Sistema
const vector<Incidencia*>& Equipo::getIncidencias() {
    return this->incidencias;
}

void Equipo::setEstado(double estado) { this->estado = estado; }
void Equipo::setCriticidad(int criticidad) { this->criticidad = criticidad; }
void Equipo::setTiempoInactivo(int tiempoInactivo) { this->tiempoInactivo = tiempoInactivo; }
void Equipo::setPrioridad(double prioridad) { this->prioridad = prioridad; }

void Equipo::degradar() {
    double desgaste = rand() % 5 + 1;
    estado -= desgaste;
    if (estado < 0) estado = 0;
    tiempoInactivo++;
}

// MODIFICADO (Paso 5): usa incidencias.size() como "incidencias_activas" según RF4 del PDF
double Equipo::calcularPrioridad() {
    double incActivas = (double)incidencias.size(); // RF4: incidencias_activas = conteo de incidencias activas
    return prioridad = (criticidad * 0.5) + (incActivas * 0.3) + (tiempoInactivo * 0.2);
}

void Equipo::agregarIncidencia(Incidencia* inc) {
    incidencias.push_back(inc);
}

int Equipo::gravedadIncidencias() {
    int total = 0;
    for (size_t i = 0; i < incidencias.size(); i++) { // MODIFICADO (Paso 9): size_t elimina warning -Wsign-compare
        total += incidencias[i]->getGravedad();
    }
    return total;
}

void Equipo::limpiarIncidencias() {
    for (size_t i = 0; i < incidencias.size(); i++) {
        delete incidencias[i];
    }
    incidencias.clear();
}

// AGREGADO (Paso 2): elimina solo incidencias con gravedad >= 3 (ALTA=5, MEDIA=3); conserva BAJA=1
void Equipo::limpiarIncidenciasGraves() {
    vector<Incidencia*> restantes;
    for (size_t i = 0; i < incidencias.size(); i++) {
        if (incidencias[i]->getGravedad() >= 3) {
            delete incidencias[i]; // libera las graves
        } else {
            restantes.push_back(incidencias[i]); // conserva las bajas
        }
    }
    incidencias = restantes;
}

// AGREGADO (Paso 8): registra un mantenimiento aplicado, usado por las tres estrategias
void Equipo::registrarMantenimiento() {
    contadorMantenimientos++;
}

void Equipo::mostrar() {
    cout << this->nombre << endl;
    cout << "ID: " << this->ID << endl;
    cout << "El estado actual del equipo es: " << this->estado << endl;
    cout << "Su ultimo uso fue hace: " << this->tiempoInactivo << " dias." << endl;
}
