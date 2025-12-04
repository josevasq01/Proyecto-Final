//
// Created by lvjos on 30/11/2025.
//

#include "Pantano.h"
#include "../unidades/Unidad.h"
std::string Pantano::codigo() const { return "PA"; }
int Pantano::costo_movimiento(const Unidad&) const { return 2; }
int Pantano::bono_defensa(const Unidad&) const { return 0; }