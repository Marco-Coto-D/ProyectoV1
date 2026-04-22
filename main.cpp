#include <iostream>
#include <ctime>
#include "Sistema.h"
using namespace std;

int main() {
    srand(time(nullptr));
    Sistema sistema;
    sistema.simular();
    return 0;
}