//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_CABALLERO_H
#define PROYECTO_FINAL_CABALLERO_H

#include "Unidad.h"

class Caballero : public Unidad {
public:
    Caballero(int owner, const Coordenada& pos);
    bool mover(const Coordenada& destino, Contexto& ctx) override;
    bool atacar(Unidad& objetivo, Contexto& ctx) override;
    std::string tipo() const override;
};

#endif //PROYECTO_FINAL_CABALLERO_H