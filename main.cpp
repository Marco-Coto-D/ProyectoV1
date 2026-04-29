#include <iostream>
#include <ctime>
#include "Sistema.h"
using namespace std;

int main() {
    srand(time(nullptr));
    try { // AGREGADO (Paso 1): captura excepciones personalizadas lanzadas por Sistema
        Sistema sistema;
        sistema.simular();
    } catch (const exception& e) { // AGREGADO (Paso 1): imprime el error y termina con código 1
        cerr << "Error fatal: " << e.what() << endl;
        return 1;
    }
    return 0;
}
