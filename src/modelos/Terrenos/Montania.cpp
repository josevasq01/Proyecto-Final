//
// Created by lvjos on 30/11/2025.
//

#include "Montania.h"
#include "../unidades/Unidad.h"
std::string Montania::codigo() const { return "MO"; }
int Montania::costo_movimiento(const Unidad&) const { return 3; }
int Montania::bono_defensa(const Unidad&) const { return 2; }