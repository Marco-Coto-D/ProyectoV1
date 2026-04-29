//
// Created by mcoto on 4/16/2026.
//

#include "Sistema.h"
#include "IncidenciaAlta.h"
#include "IncidenciaMedia.h"
#include "IncidenciaBaja.h"
#include <fstream>
#include <iomanip>
#include <sstream>

// "cambiar pos"

static void swapEquipos(Equipo& a, Equipo& b) {
    Equipo temp = a;
    a = b;
    b = temp;
}

//acomodar de aceurdo a un pivote, izquierda los de mayor prioridad y derecha del pivote los de menor

static int particion(vector<Equipo>& v, int izq, int der) {
    double pivot = v[der].getPrioridad();
    int i = izq - 1;
    for (int j = izq; j < der; j++) {
        if (v[j].getPrioridad() >= pivot) {
            i++;
            swapEquipos(v[i], v[j]);
        }
    }
    swapEquipos(v[i + 1], v[der]);
    return i + 1;
}

static void quickSort(vector<Equipo>& v, int izq, int der) {
    if (izq >= der) return;
    int p = particion(v, izq, der);
    quickSort(v, izq, p - 1);
    quickSort(v, p + 1, der);
}




// devuelve el campo n separado por ';', sin espacios
static string getCampo(const string& linea, int n) {
    int count = 0, inicio = 0;
    for (int i = 0; i <= (int)linea.size(); i++) {
        if (i == (int)linea.size() || linea[i] == ';') {
            if (count == n) {
                string r = linea.substr(inicio, i - inicio);
                int s = 0;
                while (s < (int)r.size() && r[s] == ' ') s++;
                int e = (int)r.size() - 1;
                while (e > s && r[e] == ' ') e--;
                return r.substr(s, e - s + 1);
            }
            count++;
            inicio = i + 1;
        }
    }
    return "";
}

// devuelve el valor despues del '=' en un campo como "criticidad=9"
static string getValor(const string& campo) {
    size_t pos = campo.find('=');
    if (pos == string::npos) return "";
    return campo.substr(pos + 1);
}



Sistema::Sistema() {
    diaActual = 0;

    ifstream archivo("equipos.txt");
    if (!archivo.is_open()) {
        archivo.open("../equipos.txt");
    }
    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir equipos.txt" << endl;
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        string campo0 = getCampo(linea, 0);

        if (campo0 == "INC") {
            // INC; EQ-001; severidad=ALTA; dia=3
            string idEquipo  = getCampo(linea, 1);
            string severidad = getValor(getCampo(linea, 2));

            for (int i = 0; i < (int)equipos.size(); i++) {
                if (equipos[i].getID() == idEquipo) {
                    Incidencia* inc = nullptr;
                    if (severidad == "ALTA")       inc = new IncidenciaAlta();
                    else if (severidad == "MEDIA") inc = new IncidenciaMedia();
                    else                           inc = new IncidenciaBaja();
                    equipos[i].agregarIncidencia(inc);
                    break;
                }
            }
        } else {
            // EQ-001; criticidad=9; estado=70
            string id         = campo0;
            int    criticidad = stoi(getValor(getCampo(linea, 1)));
            double estado     = stod(getValor(getCampo(linea, 2)));

            Equipo eq(id, id, criticidad);
            eq.setEstado(estado);
            equipos.push_back(eq);
        }
    }
    archivo.close();
    cout << equipos.size() << " equipos cargados." << endl;
}

void Sistema::simular() {
    logDiario.open("simulacion_diaria.txt");
    if (!logDiario.is_open()) {
        cout << "Advertencia: no se pudo crear simulacion_diaria.txt" << endl;
    }
    for (int i = 0; i < 30; i++) {
        simularDia();
    }
    if (logDiario.is_open()) logDiario.close();
    generarReporte();
}

void Sistema::simularDia() {
    int dia = diaActual + 1;

    cout << "\nDia " << dia << endl;
    if (logDiario.is_open()) logDiario << "\nDia " << dia << "\n";

    generarIncidencias();

    for (int i = 0; i < (int)equipos.size(); i++) {
        equipos[i].degradar();
        equipos[i].calcularPrioridad();
    }

    ordenarEquipos();

    // Top prioridad: los 3 equipos más urgentes
    cout << "Top prioridad: ";
    if (logDiario.is_open()) logDiario << "Top prioridad: ";
    int topN = 3 < (int)equipos.size() ? 3 : (int)equipos.size();
    for (int i = 0; i < topN; i++) {
        ostringstream oss;
        oss << fixed << setprecision(3) << equipos[i].getPrioridad();
        string entry = equipos[i].getID() + " (" + oss.str() + ")";
        cout << entry;
        if (logDiario.is_open()) logDiario << entry;
        if (i < topN - 1) {
            cout << ", ";
            if (logDiario.is_open()) logDiario << ", ";
        }
    }
    cout << endl;
    if (logDiario.is_open()) logDiario << "\n";

    aplicarMantenimientos();

    diaActual++;
}

void Sistema::generarIncidencias() {
    for (int i = 0; i < (int)equipos.size(); i++) {
        if (rand() % 100 < 40) {
            int tipo = rand() % 3;
            Incidencia* inc = nullptr;
            if (tipo == 0)      inc = new IncidenciaAlta();
            else if (tipo == 1) inc = new IncidenciaMedia();
            else                inc = new IncidenciaBaja();
            equipos[i].agregarIncidencia(inc);
        }
    }
}

void Sistema::ordenarEquipos() {
    if ((int)equipos.size() > 1)
        quickSort(equipos, 0, (int)equipos.size() - 1);
}

void Sistema::aplicarMantenimientos() {
    int total     = (int)equipos.size();
    int atendidos = total < 3 ? total : 3;

    // Asignados
    cout << "Asignados: ";
    if (logDiario.is_open()) logDiario << "Asignados: ";
    for (int i = 0; i < atendidos; i++) {
        cout << equipos[i].getID();
        if (logDiario.is_open()) logDiario << equipos[i].getID();
        if (i < atendidos - 1) {
            cout << ", ";
            if (logDiario.is_open()) logDiario << ", ";
        }
        mantenimientos.aplicar(&equipos[i]);
    }
    cout << endl;
    if (logDiario.is_open()) logDiario << "\n";

    // Backlog pendiente: equipos sin atención en este día
    int backlog = total - atendidos;
    cout << "Backlog pendiente: " << backlog << endl;
    if (logDiario.is_open()) logDiario << "Backlog pendiente: " << backlog << "\n";

    // Riesgo global basado en prioridad promedio
    double sumaPrioridad = 0;
    for (int i = 0; i < total; i++) {
        sumaPrioridad += equipos[i].getPrioridad();
    }
    double riesgoPromedio = sumaPrioridad / total;

    string nivel;
    if (riesgoPromedio >= 8)      nivel = "ALTO";
    else if (riesgoPromedio >= 5) nivel = "MEDIO";
    else                          nivel = "BAJO";

    cout << "Riesgo global: " << nivel << endl;
    if (logDiario.is_open()) logDiario << "Riesgo global: " << nivel << "\n";
}

void Sistema::generarReporte() {
    cout << "\nREPORTE FINAL - Simulacion de " << diaActual << " dias" << endl;
    cout << "========================================" << endl;

    ofstream archivo("reporte_simulacion.txt");
    if (archivo.is_open()) {
        archivo << "REPORTE FINAL - Simulacion de " << diaActual << " dias\n";
        archivo << "========================================\n";
        for (int i = 0; i < (int)equipos.size(); i++) {
            archivo << "ID: "              << equipos[i].getID()            << "\n";
            archivo << "Estado: "          << equipos[i].getEstado()         << "\n";
            archivo << "Criticidad: "      << equipos[i].getCriticidad()     << "\n";
            archivo << "Tiempo inactivo: " << equipos[i].getTiempoInactivo() << " dias\n";
            archivo << "Prioridad: "       << equipos[i].getPrioridad()      << "\n";
            archivo << "----------------------------\n";
        }
        archivo.close();
        cout << "Reporte final guardado en reporte_simulacion.txt" << endl;
    }
    cout << "Log diario guardado en simulacion_diaria.txt" << endl;
}