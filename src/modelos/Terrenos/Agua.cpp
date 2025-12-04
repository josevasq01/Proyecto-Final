//
// Created by lvjos on 30/11/2025.
//

#include "Agua.h"
#include "../unidades/Unidad.h"
std::string Agua::codigo() const { return "AG"; }
int Agua::costo_movimiento(const Unidad&) const { return 99; } // impasable para terrestres
int Agua::bono_defensa(const Unidad&) const { return 0; }