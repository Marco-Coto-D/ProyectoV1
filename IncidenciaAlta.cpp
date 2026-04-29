#include "IncidenciaAlta.h"
#include <iostream>
using namespace std;

IncidenciaAlta::IncidenciaAlta(){
}

int IncidenciaAlta::getGravedad() const{
    return 5;
}

string IncidenciaAlta::getTipo() const{
    return "Alta";
}
