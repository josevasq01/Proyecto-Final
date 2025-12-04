//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_INGENIERO_H
#define PROYECTO_FINAL_INGENIERO_H

#include "Unidad.h"
#include <string>

class Ingeniero : public Unidad {
public:
    Ingeniero(int owner, const Coordenada& pos);
    bool mover(const Coordenada& destino, Contexto& ctx) override;
    bool atacar(Unidad& objetivo, Contexto& ctx) override;
    std::string tipo() const override;

    bool construir(const Coordenada& donde, const std::string& tipoEd, Contexto& ctx);
};

#endif //PROYECTO_FINAL_INGENIERO_H