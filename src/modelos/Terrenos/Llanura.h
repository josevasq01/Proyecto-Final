//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_LLANURA_H
#define PROYECTO_FINAL_LLANURA_H

#include "Terreno.h"

class Llanura : public Terreno {
public:
    int costo_movimiento(const Unidad& u) const override;
    int bono_defensa(const Unidad& u) const override;
    std::string codigo() const override;
};

#endif //PROYECTO_FINAL_LLANURA_H