//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_MAGO_H
#define PROYECTO_FINAL_MAGO_H

#include "Unidad.h"

class Mago : public Unidad {
public:
    Mago(int owner, const Coordenada& pos);
    bool mover(const Coordenada& destino, Contexto& ctx) override;
    bool atacar(Unidad& objetivo, Contexto& ctx) override;
    std::string tipo() const override;
};

#endif //PROYECTO_FINAL_MAGO_H