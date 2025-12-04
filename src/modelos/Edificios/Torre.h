//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_TORRE_H
#define PROYECTO_FINAL_TORRE_H

#include "Edificio.h"

class Torre : public Edificio {
public:
    Torre(int owner, const Coordenada& pos);
    void efecto_turno(Contexto& ctx) override;
    std::string tipo() const override;
};

#endif //PROYECTO_FINAL_TORRE_H