//
// Created by lvjos on 30/11/2025.
//

#include "Bosque.h"
#include "../unidades/Unidad.h"
std::string Bosque::codigo() const { return "BO"; }
int Bosque::costo_movimiento(const Unidad&) const { return 2; }
int Bosque::bono_defensa(const Unidad&) const { return 1; }