#include "Sistema.h"
#include "IncidenciaAlta.h"
#include "IncidenciaMedia.h"
#include "IncidenciaBaja.h"
#include "Excepciones.h"
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;

// ----- QuickSort para ordenar equipos de mayor a menor prioridad -----

void quickSort(vector<Equipo>& lista, int izq, int der) {
    if (izq >= der) return;
    double medio = lista[der].getPrioridad();
    int menor = izq - 1;
    for (int i = izq; i < der; i++) {
        if (lista[i].getPrioridad() >= medio) {
            menor++;
            lista[menor].swap(lista[i]);
        }
    }
    lista[menor + 1].swap(lista[der]);
    int p = menor + 1;
    quickSort(lista, izq, p - 1);
    quickSort(lista, p + 1, der);
}

// ----- Funciones para leer el archivo equipos.txt -----

// Retorna el valor despues del "=" en un campo del tipo "clave=valor"
string getValor(string campo) {
    for (int i = 0; i < (int)campo.size(); i++) {
        if (campo[i] == '=') {
            return campo.substr(i + 1);
        }
    }
    return "";
}

void Sistema::guardarEnLog(string texto) {
    if (logDiario.is_open()) logDiario << texto;
}

// ----- Constructor: carga los equipos e incidencias desde el archivo -----

Sistema::Sistema() {
    diaActual = 0;
    totalCorrectivos = 0;
    totalPreventivos = 0;
    totalPredictivos = 0;
    totalIncidenciasGeneradas = 0;
    sumaRiesgoGlobal = 0.0;

    abrirArchivo();
    construirIndice();
}

void Sistema::abrirArchivo() {
    ifstream archivo("equipos.txt");
    if (!archivo.is_open()) archivo.open("../equipos.txt");
    if (!archivo.is_open()) throw ArchivoInvalidoException("equipos.txt no encontrado");

    string linea;
    int numLinea = 0;
    while (getline(archivo, linea)) {
        numLinea++;
        if (linea.empty()) continue;
        separarObjetos(linea, numLinea);
    }
    archivo.close();
    cout << equipos.size() << " equipos cargados." << endl;
}

void Sistema::separarObjetos(string linea, int numLinea) {
    vector<string> campos;
    int inicio = 0;
    for (int i = 0; i <= (int)linea.size(); i++) {
        if (i == (int)linea.size() || linea[i] == ';') {
            string c = linea.substr(inicio, i - inicio);
            int ini = 0;
            while (ini < (int)c.size() && c[ini] == ' ') ini++;
            int fin = (int)c.size() - 1;
            while (fin > ini && c[fin] == ' ') fin--;
            campos.push_back(c.substr(ini, fin - ini + 1));
            inicio = i + 1;
        }
    }

    string primero = campos.empty() ? "" : campos[0];

    if (primero == "INC") {
        string idEquipo  = campos.size() > 1 ? campos[1] : "";
        string severidad = getValor(campos.size() > 2 ? campos[2] : "");
        string diaTexto  = getValor(campos.size() > 3 ? campos[3] : "");

        if (idEquipo.empty() || severidad.empty() || diaTexto.empty()) {
            throw FormatoInvalidoException("linea " + to_string(numLinea) + " INC incompleta: " + linea);
        }

        int dia = 0;
        try {
            dia = stoi(diaTexto);
        } catch (...) {
            throw FormatoInvalidoException("linea " + to_string(numLinea) + " dia no es numero: " + diaTexto);
        }

        IncidenciaPendiente p;
        p.idEquipo      = idEquipo;
        p.severidad     = severidad;
        p.diaActivacion = dia;
        incidenciasPendientes.push_back(p);

    } else {
        string id          = primero;
        string campoCrit   = campos.size() > 1 ? campos[1] : "";
        string campoEstado = campos.size() > 2 ? campos[2] : "";

        if (id.empty() || campoCrit.empty() || campoEstado.empty()) {
            throw FormatoInvalidoException("linea " + to_string(numLinea) + " equipo incompleto: " + linea);
        }

        int    criticidad = 0;
        double estado     = 0;
        try {
            criticidad = stoi(getValor(campoCrit));
            estado     = stod(getValor(campoEstado));
        } catch (...) {
            throw FormatoInvalidoException("linea " + to_string(numLinea) + " valor no numerico: " + linea);
        }

        Equipo eq(id, id, criticidad);
        eq.setEstado(estado);
        equipos.push_back(eq);
    }
}

void Sistema::construirIndice() {
    for (int i = 0; i < (int)equipos.size(); i++) {
        equiposOrdenadosPorId.push_back(i);
    }
    ordenarPorId();
}

// ----- Ordena el indice de IDs con bubble sort -----

void Sistema::ordenarPorId() {
    int n = (int)equiposOrdenadosPorId.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (equipos[equiposOrdenadosPorId[j]].getID() > equipos[equiposOrdenadosPorId[j+1]].getID()) {
                int tmp = equiposOrdenadosPorId[j];
                equiposOrdenadosPorId[j]   = equiposOrdenadosPorId[j+1];
                equiposOrdenadosPorId[j+1] = tmp;
            }
        }
    }
}

// ----- Busqueda binaria: busca un equipo por ID en el indice ordenado -----

int Sistema::buscarEquipo(string id) {
    int izq = 0, der = (int)equiposOrdenadosPorId.size() - 1;
    while (izq <= der) {
        int med = (izq + der) / 2;
        string idMed = equipos[equiposOrdenadosPorId[med]].getID();
        if (idMed == id) return equiposOrdenadosPorId[med];
        if (idMed < id) izq = med + 1;
        else            der = med - 1;
    }
    return -1;
}

// ----- Patron Strategy: decide que tipo de mantenimiento aplicar -----

Mantenimiento* Sistema::seleccionarEstrategia(Equipo& equipo) {
    // Downcasting seguro: verifica si hay una IncidenciaAlta con paro inmediato
    const vector<Incidencia*>& lista = equipo.getIncidencias();
    for (int i = 0; i < (int)lista.size(); i++) {
        IncidenciaAlta* alta = dynamic_cast<IncidenciaAlta*>(lista[i]);
        if (alta != nullptr && alta->requiereParoInmediato()) {
            return &correctivo;
        }
    }
    if (equipo.getEstado() < 40.0 || equipo.gravedadIncidencias() >= 10) {
        return &correctivo;
    }
    if ((int)equipo.getIncidencias().size() >= 3) {
        return &predictivo;
    }
    return &preventivo;
}

// ----- Simulacion de 30 dias -----

void Sistema::simular() {
    logDiario.open("simulacion_diaria.txt");
    if (!logDiario.is_open()) {
        cout << "Advertencia: no se pudo crear simulacion_diaria.txt" << endl;
    }
    cout << fixed << setprecision(3);
    if (logDiario.is_open()) logDiario << fixed << setprecision(3);
    for (int i = 0; i < 30; i++) {
        simularDia();
        if (i < 29) {
            cout << "\nPresiona Enter para avanzar al dia " << diaActual + 1 << "...\n";
            cin.get();
        }
    }
    if (logDiario.is_open()) logDiario.close();
    generarReporte();
}

void Sistema::simularDia() {
    int dia = diaActual + 1;

    cout << "\nDia " << dia << "\n";
    guardarEnLog("\nDia " + to_string(dia) + "\n");

    // Paso 1: todos los equipos se degradan
    for (int i = 0; i < (int)equipos.size(); i++) {
        equipos[i].degradar();
    }

    // Paso 2: se activan las incidencias del archivo y se generan incidencias aleatorias
    aplicarIncidenciasPendientes();
    generarIncidencias();

    // Paso 3: se calcula la prioridad de cada equipo
    for (int i = 0; i < (int)equipos.size(); i++) {
        equipos[i].calcularPrioridad();
    }

    // Paso 4: se ordenan los equipos de mayor a menor prioridad
    ordenarEquipos();

    // Mostrar los 3 equipos mas urgentes
    int top = 3 < (int)equipos.size() ? 3 : (int)equipos.size();
    cout << "Top prioridad: ";
    guardarEnLog("Top prioridad: ");
    for (int i = 0; i < top; i++) {
        cout << equipos[i].getID() << " (" << equipos[i].getPrioridad() << ")";
        if (logDiario.is_open()) logDiario << equipos[i].getID() << " (" << equipos[i].getPrioridad() << ")";
        if (i < top - 1) { cout << ", "; guardarEnLog(", "); }
    }
    cout << "\n";
    guardarEnLog("\n");

    // Pasos 5-8: seleccion, mantenimiento, actualizacion global y registro
    aplicarMantenimientos();

    diaActual++;
}

// Activa las incidencias del archivo cuando llega su dia
void Sistema::aplicarIncidenciasPendientes() {
    // Reconstruir el indice porque el quickSort reordeno el vector
    for (int i = 0; i < (int)equipos.size(); i++) equiposOrdenadosPorId[i] = i;
    ordenarPorId();

    for (int i = 0; i < (int)incidenciasPendientes.size(); i++) {
        IncidenciaPendiente& p = incidenciasPendientes[i];
        if (p.diaActivacion != diaActual + 1) continue;

        int posicion = buscarEquipo(p.idEquipo);
        if (posicion == -1) {
            throw OperacionInconsistenteException("INC referencia equipo inexistente: " + p.idEquipo);
        }

        Incidencia* nueva = nullptr;
        if (p.severidad == "ALTA") {
            nueva = new IncidenciaAlta();
        } else if (p.severidad == "MEDIA") {
            nueva = new IncidenciaMedia();
        } else {
            nueva = new IncidenciaBaja();
        }
        equipos[posicion].agregarIncidencia(nueva);
        cout << "  ocurrio una INC " << nueva->getTipo() << " en " << nueva->getEquipo()->getID() << "\n";
        guardarEnLog("  ocurrio una INC " + nueva->getTipo() + " en " + nueva->getEquipo()->getID() + "\n");
    }
}

void Sistema::generarIncidencias() {
    for (int i = 0; i < (int)equipos.size(); i++) {
        if (rand() % 100 < 40) {
            int tipo = rand() % 3;
            Incidencia* nueva = nullptr;
            if (tipo == 0) {
                nueva = new IncidenciaAlta();
            } else if (tipo == 1) {
                nueva = new IncidenciaMedia();
            } else {
                nueva = new IncidenciaBaja();
            }
            equipos[i].agregarIncidencia(nueva);
            totalIncidenciasGeneradas++;
        }
    }
}

void Sistema::ordenarEquipos() {
    if ((int)equipos.size() > 1)
        quickSort(equipos, 0, (int)equipos.size() - 1);
}

void Sistema::aplicarMantenimientos() {
    int total    = (int)equipos.size();
    int atendidos = total < 3 ? total : 3;

    string lineaAsignados = "Asignados: ";
    for (int i = 0; i < atendidos; i++) {
        Mantenimiento* estrategia = seleccionarEstrategia(equipos[i]);

        if (estrategia->getTipo() == "Correctivo") {
            totalCorrectivos++;
        } else if (estrategia->getTipo() == "Preventivo") {
            totalPreventivos++;
        } else {
            totalPredictivos++;
        }
        lineaAsignados += equipos[i].getID() + " [" + estrategia->getTipo() + "]";
        if (i < atendidos - 1) lineaAsignados += ", ";
        estrategia->aplicar(&equipos[i]);
    }
    cout << lineaAsignados << "\n";
    guardarEnLog(lineaAsignados + "\n");

    // Mostrar los siguientes 5 equipos en espera
    int mostrar = (5 < total - atendidos) ? 5 : (total - atendidos);
    cout << "Pendientes: ";
    guardarEnLog("Pendientes: ");
    for (int i = atendidos; i < atendidos + mostrar; i++) {
        cout << equipos[i].getID() << " (" << equipos[i].getPrioridad() << ")";
        if (logDiario.is_open()) logDiario << equipos[i].getID() << " (" << equipos[i].getPrioridad() << ")";
        if (i < atendidos + mostrar - 1) { cout << ", "; guardarEnLog(", "); }
    }
    cout << "\n";
    guardarEnLog("\n");

    // Calcular estadisticas del dia
    int backlog             = 0;
    double sumaEstado       = 0;
    double sumaPrioridad    = 0;
    int incidenciasActivas  = 0;
    for (int i = 0; i < total; i++) {
        if (!equipos[i].getIncidencias().empty() || equipos[i].getEstado() < 50.0) {
            backlog++;
        }
        sumaPrioridad      += equipos[i].getPrioridad();
        sumaEstado         += equipos[i].getEstado();
        incidenciasActivas += (int)equipos[i].getIncidencias().size();
    }
    cout << "Backlog: " << backlog << " equipos requieren atencion\n";
    guardarEnLog("Backlog: " + to_string(backlog) + " equipos requieren atencion\n");

    double riesgoPromedio = sumaPrioridad / total;
    sumaRiesgoGlobal += riesgoPromedio;

    string nivelRiesgo;
    if (riesgoPromedio >= 8) {
        nivelRiesgo = "ALTO";
    } else if (riesgoPromedio >= 5) {
        nivelRiesgo = "MEDIO";
    } else {
        nivelRiesgo = "BAJO";
    }
    cout << "Riesgo global: " << nivelRiesgo << "\n";
    guardarEnLog("Riesgo global: " + nivelRiesgo + "\n");

    cout << "Estado promedio: " << sumaEstado / total
         << " | Prioridad promedio: " << sumaPrioridad / total
         << " | Incidencias activas: " << incidenciasActivas << "\n";
    if (logDiario.is_open())
        logDiario << "Estado promedio: " << sumaEstado / total
                  << " | Prioridad promedio: " << sumaPrioridad / total
                  << " | Incidencias activas: " << incidenciasActivas << "\n";
}

// ----- Reporte final con estadisticas de los 30 dias -----

void Sistema::generarReporte() {
    cout << "\nREPORTE FINAL - Simulacion de " << diaActual << " dias\n";
    guardarEnLog("\nREPORTE FINAL - Simulacion de " + to_string(diaActual) + " dias\n");
    cout << "========================================\n";
    guardarEnLog("========================================\n");

    int iMas   = 0;
    int iMenos = 0;
    for (int i = 1; i < (int)equipos.size(); i++) {
        if (equipos[i].getContadorMantenimientos() > equipos[iMas].getContadorMantenimientos())
            iMas = i;
        if (equipos[i].getContadorMantenimientos() < equipos[iMenos].getContadorMantenimientos())
            iMenos = i;
    }

    string resumen =
        "Mantenimientos - Correctivo: " + to_string(totalCorrectivos) +
        " | Preventivo: "               + to_string(totalPreventivos) +
        " | Predictivo: "               + to_string(totalPredictivos) + "\n" +
        "Equipo mas atendido:   "       + equipos[iMas].getID() +
        " (" + to_string(equipos[iMas].getContadorMantenimientos())   + " veces)\n" +
        "Equipo menos atendido: "       + equipos[iMenos].getID() +
        " (" + to_string(equipos[iMenos].getContadorMantenimientos()) + " veces)\n" +
        "Total incidencias generadas: " + to_string(totalIncidenciasGeneradas) + "\n";
    cout << resumen;
    cout << "Riesgo global promedio 30 dias: " << sumaRiesgoGlobal / diaActual << "\n";
    cout << "========================================\n";

    // Guardar estado final de cada equipo en archivo
    ofstream reporte("reporte_simulacion.txt");
    if (reporte.is_open()) {
        reporte << fixed << setprecision(3);
        reporte << "REPORTE FINAL - Simulacion de " << diaActual << " dias\n";
        reporte << resumen;
        reporte << "Riesgo global promedio 30 dias: " << sumaRiesgoGlobal / diaActual << "\n";
        reporte << "========================================\n";
        reporte << "\nEstado final de cada equipo:\n";
        for (int i = 0; i < (int)equipos.size(); i++) {
            equipos[i].calcularPrioridad();
            reporte << "ID: "              << equipos[i].getID()                     << "\n";
            reporte << "Estado: "          << equipos[i].getEstado()                 << "\n";
            reporte << "Criticidad: "      << equipos[i].getCriticidad()             << "\n";
            reporte << "Tiempo inactivo: " << equipos[i].getTiempoInactivo()         << " dias\n";
            reporte << "Prioridad: "       << equipos[i].getPrioridad()              << "\n";
            reporte << "Mantenimientos: "  << equipos[i].getContadorMantenimientos() << "\n";
            reporte << "----------------------------\n";
        }
        reporte.close();
        cout << "Reporte final guardado en reporte_simulacion.txt" << endl;
    }
    cout << "Log diario guardado en simulacion_diaria.txt" << endl;
}
