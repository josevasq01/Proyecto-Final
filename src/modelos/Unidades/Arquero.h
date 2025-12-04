//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_ARQUERO_H
#define PROYECTO_FINAL_ARQUERO_H

#include "Unidad.h"

class Arquero : public Unidad {
public:
    Arquero(int owner, const Coordenada& pos);
    bool mover(const Coordenada& destino, Contexto& ctx) override;
    bool atacar(Unidad& objetivo, Contexto& ctx) override;
    std::string tipo() const override;
};

#endif //PROYECTO_FINAL_ARQUERO_H