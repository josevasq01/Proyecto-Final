//
// Created by lvjos on 30/11/2025.
//

#include "Llanura.h"
#include "../unidades/Unidad.h"
std::string Llanura::codigo() const { return "LL"; }
int Llanura::costo_movimiento(const Unidad&) const { return 1; }
int Llanura::bono_defensa(const Unidad&) const { return 0; }