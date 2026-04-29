#include <iostream>
#include <ctime>
#include "Sistema.h"
using namespace std;

int main() {
    srand(time(nullptr));
    try {
        Sistema sistema;
        sistema.simular();
    } catch (const exception& e) {
        cerr << "Error fatal: " << e.what() << endl;
        return 1;
    }
    return 0;
}
