//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_FORJA_H
#define PROYECTO_FINAL_FORJA_H

#include "Edificio.h"

class Forja : public Edificio {
public:
    Forja(int owner, const Coordenada& pos);
    void efecto_turno(Contexto& ctx) override;
    std::string tipo() const override;
};

#endif //PROYECTO_FINAL_FORJA_H