//
// Created by marco on 4/15/2026.
//

#include "IncidenciaBaja.h"
#include <iostream>
using namespace std;

IncidenciaBaja::IncidenciaBaja(){
}

int IncidenciaBaja::getGravedad() const{
    return 1;
}

string IncidenciaBaja::getTipo() const{
    return "Baja";
}
