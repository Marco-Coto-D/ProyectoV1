#include "Sistema.h"
#include "IncidenciaAlta.h"   // AGREGADO (Paso 3): necesario para dynamic_cast<IncidenciaAlta*>
#include "IncidenciaMedia.h"
#include "IncidenciaBaja.h"
#include "Excepciones.h"      // AGREGADO (Paso 1): excepciones personalizadas
#include <fstream>

// ── Formateo de doubles sin <iomanip> ni <sstream> ──────────────────────────
// MODIFICADO (Paso 9): función propia para mostrar doubles con N decimales fijos
static string formatearDouble(double valor, int decimales) {
    int multiplicador = 1;
    for (int i = 0; i < decimales; i++) multiplicador *= 10;
    long long parteEntera  = (long long)valor;
    int parteDecimal = (int)((valor - (double)parteEntera) * multiplicador + 0.5); // redondeo
    if (parteDecimal >= multiplicador) { parteEntera++; parteDecimal = 0; }         // acarreo (ej: 9.9995 → 10.000)
    string decimalesTexto = to_string(parteDecimal);
    while ((int)decimalesTexto.size() < decimales) decimalesTexto = "0" + decimalesTexto; // rellena ceros
    return to_string(parteEntera) + "." + decimalesTexto;
}

// ── QuickSort propio (algoritmo obligatorio §7) ──────────────────────────────

// MODIFICADO (Paso 6/9): usa swap campo por campo en vez de copia temporal — evita aliasing de punteros
static void intercambiar(vector<Equipo>& lista, int posA, int posB) {
    lista[posA].swap(lista[posB]); // llama al swap propio de Equipo, sin librerías externas
}

static int particion(vector<Equipo>& lista, int izquierda, int derecha) {
    double pivote = lista[derecha].getPrioridad();
    int indiceMenor = izquierda - 1;
    for (int posActual = izquierda; posActual < derecha; posActual++) {
        if (lista[posActual].getPrioridad() >= pivote) {
            indiceMenor++;
            intercambiar(lista, indiceMenor, posActual);
        }
    }
    intercambiar(lista, indiceMenor + 1, derecha);
    return indiceMenor + 1;
}

static void quickSort(vector<Equipo>& lista, int izquierda, int derecha) {
    if (izquierda >= derecha) return;
    int indicePivote = particion(lista, izquierda, derecha);
    quickSort(lista, izquierda, indicePivote - 1);
    quickSort(lista, indicePivote + 1, derecha);
}

// ── Helpers de parseo ────────────────────────────────────────────────────────

static string getCampo(const string& linea, int numeroCampo) {
    int contadorCampos = 0, posicionInicio = 0;
    for (int i = 0; i <= (int)linea.size(); i++) {
        if (i == (int)linea.size() || linea[i] == ';') {
            if (contadorCampos == numeroCampo) {
                string resultado = linea.substr(posicionInicio, i - posicionInicio);
                int posIzquierda = 0;
                while (posIzquierda < (int)resultado.size() && resultado[posIzquierda] == ' ') posIzquierda++;
                int posDerecha = (int)resultado.size() - 1;
                while (posDerecha > posIzquierda && resultado[posDerecha] == ' ') posDerecha--;
                return resultado.substr(posIzquierda, posDerecha - posIzquierda + 1);
            }
            contadorCampos++;
            posicionInicio = i + 1;
        }
    }
    return "";
}

static string getValor(const string& campo) {
    size_t posigualSign = campo.find('=');
    if (posigualSign == string::npos) return "";
    return campo.substr(posigualSign + 1);
}

// ── Helper: escribe en consola y en archivo simultáneamente ─────────────────

void Sistema::escribir(const string& texto) {
    cout << texto;
    if (logDiario.is_open()) logDiario << texto;
}

// ── Constructor ──────────────────────────────────────────────────────────────

Sistema::Sistema() {
    diaActual = 0;
    totalCorrectivos = 0;        // AGREGADO (Paso 8): contadores iniciales
    totalPreventivos = 0;
    totalPredictivos = 0;
    totalIncidenciasGeneradas = 0;
    sumaRiesgoGlobal = 0.0;

    // MODIFICADO (Paso 1): lanza ArchivoInvalidoException en lugar de imprimir y continuar
    ifstream archivoEntrada("equipos.txt");
    if (!archivoEntrada.is_open()) {
        archivoEntrada.open("../equipos.txt"); // fallback para CLion (ejecuta desde cmake-build-debug)
    }
    if (!archivoEntrada.is_open()) {
        throw ArchivoInvalidoException("equipos.txt no encontrado en directorio actual ni en ../");
    }

    string linea;
    int numeroLinea = 0;
    while (getline(archivoEntrada, linea)) {
        numeroLinea++;
        if (linea.empty()) continue;

        string primerCampo = getCampo(linea, 0);

        if (primerCampo == "INC") {
            // MODIFICADO (Paso 7): las INC se guardan con su día, no se aplican de inmediato
            string idEquipo  = getCampo(linea, 1);
            string severidad = getValor(getCampo(linea, 2));
            string diaTexto  = getValor(getCampo(linea, 3));

            // MODIFICADO (Paso 1): valida que los campos no estén vacíos
            if (idEquipo.empty() || severidad.empty() || diaTexto.empty()) {
                throw FormatoInvalidoException("linea " + to_string(numeroLinea) + " INC incompleta: " + linea);
            }

            int diaActivacion = 0;
            try {
                diaActivacion = stoi(diaTexto); // MODIFICADO (Paso 1): captura stoi y relanza
            } catch (...) {
                throw FormatoInvalidoException("linea " + to_string(numeroLinea) + " dia no es numero: " + diaTexto);
            }

            // AGREGADO (Paso 7): guardar en cola pendiente, se aplica al llegar su día
            IncidenciaPendiente pendiente;
            pendiente.idEquipo      = idEquipo;
            pendiente.severidad     = severidad;
            pendiente.diaActivacion = diaActivacion;
            incidenciasPendientes.push_back(pendiente);

        } else {
            // Línea de equipo: EQ-001; criticidad=9; estado=70
            string identificador = primerCampo;
            string campoCriticidad = getCampo(linea, 1);
            string campoEstado     = getCampo(linea, 2);

            // MODIFICADO (Paso 1): valida campos antes de stoi/stod
            if (identificador.empty() || campoCriticidad.empty() || campoEstado.empty()) {
                throw FormatoInvalidoException("linea " + to_string(numeroLinea) + " equipo incompleto: " + linea);
            }

            int    criticidad = 0;
            double estado     = 0;
            try {
                criticidad = stoi(getValor(campoCriticidad));
                estado     = stod(getValor(campoEstado));
            } catch (...) {
                throw FormatoInvalidoException("linea " + to_string(numeroLinea) + " valor no numerico: " + linea);
            }

            Equipo equipo(identificador, identificador, criticidad);
            equipo.setEstado(estado);
            equipos.push_back(equipo); // incidencias están vacías aquí, la copia superficial es segura
        }
    }
    archivoEntrada.close();
    cout << equipos.size() << " equipos cargados." << endl;

    // AGREGADO (Paso 4): construir índice de posiciones para búsqueda binaria O(log n)
    // Justificación: con 100 equipos y 300 INC — búsqueda binaria ~2100 comparaciones vs ~30000 lineal
    for (int i = 0; i < (int)equipos.size(); i++) {
        equiposOrdenadosPorId.push_back(i);
    }
    // MODIFICADO (Paso 9): bubble sort propio en lugar de std::sort — sin <algorithm>
    int cantidadEquipos = (int)equiposOrdenadosPorId.size();
    for (int i = 0; i < cantidadEquipos - 1; i++) {
        for (int j = 0; j < cantidadEquipos - 1 - i; j++) {
            if (equipos[equiposOrdenadosPorId[j]].getID() > equipos[equiposOrdenadosPorId[j+1]].getID()) {
                int temporal = equiposOrdenadosPorId[j];
                equiposOrdenadosPorId[j]   = equiposOrdenadosPorId[j+1];
                equiposOrdenadosPorId[j+1] = temporal;
            }
        }
    }
}

// ── Búsqueda binaria propia O(log n) (§7) ────────────────────────────────────

// AGREGADO (Paso 4): busca un equipo por ID en el índice ordenado
int Sistema::buscarEquipoBinaria(const string& identificador) const {
    int izquierda = 0, derecha = (int)equiposOrdenadosPorId.size() - 1;
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        const string& idMedio = equipos[equiposOrdenadosPorId[medio]].getID();
        if (idMedio == identificador) return equiposOrdenadosPorId[medio]; // retorna índice en equipos[]
        if (idMedio < identificador)  izquierda = medio + 1;
        else                          derecha   = medio - 1;
    }
    return -1; // no encontrado
}

// ── Selección de estrategia (Strategy + dynamic_cast) ────────────────────────

// AGREGADO (Paso 2/3): decide qué tipo de mantenimiento aplicar según el estado del equipo
// Usa dynamic_cast para detectar IncidenciaAlta con requiereParoInmediato() (§8.2)
Mantenimiento* Sistema::seleccionarEstrategia(Equipo& equipo) {
    // AGREGADO (Paso 3): downcasting seguro — accede a comportamiento exclusivo de IncidenciaAlta
    const vector<Incidencia*>& listaIncidencias = equipo.getIncidencias();
    for (int i = 0; i < (int)listaIncidencias.size(); i++) {
        IncidenciaAlta* incidenciaAlta = dynamic_cast<IncidenciaAlta*>(listaIncidencias[i]); // retorna nullptr si no es IncidenciaAlta
        if (incidenciaAlta != nullptr && incidenciaAlta->requiereParoInmediato()) {
            return &correctivo; // paro inmediato → mantenimiento correctivo forzoso
        }
    }
    // Correctivo si el equipo está muy degradado o tiene muchas incidencias graves
    if (equipo.getEstado() < 40.0 || equipo.gravedadIncidencias() >= 10) {
        return &correctivo;
    }
    // Predictivo si tiene varias incidencias pero sin paro inmediato
    if ((int)equipo.getIncidencias().size() >= 3) {
        return &predictivo;
    }
    // Preventivo para mantenimiento de rutina
    return &preventivo;
}

// ── Simulación ───────────────────────────────────────────────────────────────

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
    int diaSimulacion = diaActual + 1;

    escribir("\nDia " + to_string(diaSimulacion) + "\n");

    aplicarIncidenciasPendientes(); // AGREGADO (Paso 7): activa las INC del archivo cuyo día llegó
    generarIncidencias();

    for (int i = 0; i < (int)equipos.size(); i++) {
        equipos[i].degradar();
        equipos[i].calcularPrioridad();
    }

    ordenarEquipos();

    // Top prioridad: los 3 equipos más urgentes
    int cantidadTop = 3 < (int)equipos.size() ? 3 : (int)equipos.size();
    string lineaTop = "Top prioridad: ";
    for (int i = 0; i < cantidadTop; i++) {
        lineaTop += equipos[i].getID() + " (" + formatearDouble(equipos[i].getPrioridad(), 3) + ")";
        if (i < cantidadTop - 1) lineaTop += ", ";
    }
    escribir(lineaTop + "\n");

    aplicarMantenimientos();

    diaActual++;
}

// AGREGADO (Paso 7): aplica las incidencias del archivo cuyo diaActivacion == día actual
void Sistema::aplicarIncidenciasPendientes() {
    // MODIFICADO (Paso 4): reconstruir índice — quickSort del día anterior reubicó los objetos
    for (int i = 0; i < (int)equipos.size(); i++) equiposOrdenadosPorId[i] = i;

    // MODIFICADO (Paso 9): bubble sort propio para reordenar el índice por ID
    int cantidadEquipos = (int)equiposOrdenadosPorId.size();
    for (int i = 0; i < cantidadEquipos - 1; i++) {
        for (int j = 0; j < cantidadEquipos - 1 - i; j++) {
            if (equipos[equiposOrdenadosPorId[j]].getID() > equipos[equiposOrdenadosPorId[j+1]].getID()) {
                int temporal = equiposOrdenadosPorId[j];
                equiposOrdenadosPorId[j]   = equiposOrdenadosPorId[j+1];
                equiposOrdenadosPorId[j+1] = temporal;
            }
        }
    }

    for (int i = 0; i < (int)incidenciasPendientes.size(); i++) {
        IncidenciaPendiente& pendiente = incidenciasPendientes[i];
        if (pendiente.diaActivacion != diaActual + 1) continue;

        // MODIFICADO (Paso 4): búsqueda binaria O(log n) en lugar de recorrido lineal O(n)
        int indice = buscarEquipoBinaria(pendiente.idEquipo);
        if (indice == -1) {
            throw OperacionInconsistenteException( // MODIFICADO (Paso 1): lanza excepción
                "INC referencia equipo inexistente: " + pendiente.idEquipo);
        }

        Incidencia* nuevaIncidencia = nullptr;
        if      (pendiente.severidad == "ALTA")  nuevaIncidencia = new IncidenciaAlta();
        else if (pendiente.severidad == "MEDIA") nuevaIncidencia = new IncidenciaMedia();
        else                                     nuevaIncidencia = new IncidenciaBaja();
        equipos[indice].agregarIncidencia(nuevaIncidencia);
    }
}

void Sistema::generarIncidencias() {
    for (int i = 0; i < (int)equipos.size(); i++) {
        if (rand() % 100 < 40) {
            int tipoAleatorio = rand() % 3;
            Incidencia* nuevaIncidencia = nullptr;
            if      (tipoAleatorio == 0) nuevaIncidencia = new IncidenciaAlta();
            else if (tipoAleatorio == 1) nuevaIncidencia = new IncidenciaMedia();
            else                         nuevaIncidencia = new IncidenciaBaja();
            equipos[i].agregarIncidencia(nuevaIncidencia);
            totalIncidenciasGeneradas++; // AGREGADO (Paso 8): acumula para el reporte final
        }
    }
}

void Sistema::ordenarEquipos() {
    if ((int)equipos.size() > 1)
        quickSort(equipos, 0, (int)equipos.size() - 1);
}

void Sistema::aplicarMantenimientos() {
    int totalEquipos = (int)equipos.size();
    int atendidos    = totalEquipos < 3 ? totalEquipos : 3;

    // Asignados con tipo — MODIFICADO (Paso 2): muestra la estrategia elegida por equipo
    string lineaAsignados = "Asignados: ";
    for (int i = 0; i < atendidos; i++) {
        Mantenimiento* estrategia = seleccionarEstrategia(equipos[i]); // AGREGADO (Paso 2/3)

        // AGREGADO (Paso 8): conteo por tipo para el reporte final
        if      (estrategia->getTipo() == "Correctivo")  totalCorrectivos++;
        else if (estrategia->getTipo() == "Preventivo")  totalPreventivos++;
        else                                              totalPredictivos++;

        lineaAsignados += equipos[i].getID() + " [" + estrategia->getTipo() + "]";
        if (i < atendidos - 1) lineaAsignados += ", ";
        estrategia->aplicar(&equipos[i]);
    }
    escribir(lineaAsignados + "\n");

    // AGREGADO (Paso 8): lista los siguientes 5 equipos en cola con mayor prioridad
    int equiposAMostrar = (5 < totalEquipos - atendidos) ? 5 : (totalEquipos - atendidos);
    string lineaPendientes = "Pendientes: ";
    for (int i = atendidos; i < atendidos + equiposAMostrar; i++) {
        lineaPendientes += equipos[i].getID() + " (" + formatearDouble(equipos[i].getPrioridad(), 3) + ")";
        if (i < atendidos + equiposAMostrar - 1) lineaPendientes += ", ";
    }
    escribir(lineaPendientes + "\n");

    // MODIFICADO (Paso 8): backlog real = equipos con incidencias activas o estado < 50
    int equiposEnBacklog           = 0;
    double sumaEstado              = 0;
    double sumaPrioridad           = 0;
    int totalIncidenciasActivas    = 0;
    for (int i = 0; i < totalEquipos; i++) {
        if (!equipos[i].getIncidencias().empty() || equipos[i].getEstado() < 50.0) {
            equiposEnBacklog++;
        }
        sumaPrioridad           += equipos[i].getPrioridad();
        sumaEstado              += equipos[i].getEstado();
        totalIncidenciasActivas += (int)equipos[i].getIncidencias().size();
    }
    escribir("Backlog: " + to_string(equiposEnBacklog) + " equipos requieren atencion\n");

    // Riesgo global basado en prioridad promedio
    double riesgoPromedio = sumaPrioridad / totalEquipos;
    sumaRiesgoGlobal += riesgoPromedio; // AGREGADO (Paso 8): acumula para promedio final

    string nivelRiesgo;
    if      (riesgoPromedio >= 8) nivelRiesgo = "ALTO";
    else if (riesgoPromedio >= 5) nivelRiesgo = "MEDIO";
    else                          nivelRiesgo = "BAJO";

    escribir("Riesgo global: " + nivelRiesgo + "\n");

    // AGREGADO (Paso 8): estado general del laboratorio ese día
    string estadoGeneral = "Estado promedio: " + formatearDouble(sumaEstado / totalEquipos, 1) +
                           " | Prioridad promedio: " + formatearDouble(sumaPrioridad / totalEquipos, 2) +
                           " | Incidencias activas: " + to_string(totalIncidenciasActivas) + "\n";
    escribir(estadoGeneral);
}

// ── Reporte final ─────────────────────────────────────────────────────────────

void Sistema::generarReporte() {
    // MODIFICADO (Paso 8): reporte final expandido con métricas acumuladas de los 30 días
    escribir("\nREPORTE FINAL - Simulacion de " + to_string(diaActual) + " dias\n");
    escribir("========================================\n");

    // Equipo más y menos atendido durante la simulación
    int indiceMasAtendido   = 0;
    int indiceMenosAtendido = 0;
    for (int i = 1; i < (int)equipos.size(); i++) {
        if (equipos[i].getContadorMantenimientos() > equipos[indiceMasAtendido].getContadorMantenimientos())
            indiceMasAtendido = i;
        if (equipos[i].getContadorMantenimientos() < equipos[indiceMenosAtendido].getContadorMantenimientos())
            indiceMenosAtendido = i;
    }

    string resumen =
        "Mantenimientos — Correctivo: " + to_string(totalCorrectivos) +
        " | Preventivo: "               + to_string(totalPreventivos) +
        " | Predictivo: "               + to_string(totalPredictivos) + "\n" +
        "Equipo mas atendido:   "       + equipos[indiceMasAtendido].getID() +
        " (" + to_string(equipos[indiceMasAtendido].getContadorMantenimientos())   + " veces)\n" +
        "Equipo menos atendido: "       + equipos[indiceMenosAtendido].getID() +
        " (" + to_string(equipos[indiceMenosAtendido].getContadorMantenimientos()) + " veces)\n" +
        "Total incidencias generadas: " + to_string(totalIncidenciasGeneradas) + "\n" +
        "Riesgo global promedio 30 dias: " + formatearDouble(sumaRiesgoGlobal / diaActual, 2) + "\n" +
        "========================================\n";
    escribir(resumen);

    // Estado final de cada equipo — solo va al archivo para no saturar la consola
    ofstream archivoReporte("reporte_simulacion.txt");
    if (archivoReporte.is_open()) {
        archivoReporte << "REPORTE FINAL - Simulacion de " << diaActual << " dias\n";
        archivoReporte << resumen;
        archivoReporte << "\nEstado final de cada equipo:\n";
        for (int i = 0; i < (int)equipos.size(); i++) {
            equipos[i].calcularPrioridad(); // MODIFICADO (Paso 11): recalcula post-mantenimiento
            archivoReporte << "ID: "               << equipos[i].getID()                     << "\n";
            archivoReporte << "Estado: "           << equipos[i].getEstado()                 << "\n";
            archivoReporte << "Criticidad: "       << equipos[i].getCriticidad()             << "\n";
            archivoReporte << "Tiempo inactivo: "  << equipos[i].getTiempoInactivo()         << " dias\n";
            archivoReporte << "Prioridad: "        << equipos[i].getPrioridad()              << "\n";
            archivoReporte << "Mantenimientos: "   << equipos[i].getContadorMantenimientos() << "\n";
            archivoReporte << "----------------------------\n";
        }
        archivoReporte.close();
        cout << "Reporte final guardado en reporte_simulacion.txt" << endl;
    }
    cout << "Log diario guardado en simulacion_diaria.txt" << endl;
}
