//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_SOLDADO_H
#define PROYECTO_FINAL_SOLDADO_H

#include "Unidad.h"

class Soldado : public Unidad {
public:
    Soldado(int owner, const Coordenada& pos);
    bool mover(const Coordenada& destino, Contexto& ctx) override;
    bool atacar(Unidad& objetivo, Contexto& ctx) override;
    std::string tipo() const override;
};

#endif //PROYECTO_FINAL_SOLDADO_H