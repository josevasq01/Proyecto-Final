//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_JUGADOR_H
#define PROYECTO_FINAL_JUGADOR_H

#include "Controlador.h"

class Jugador : public Controlador {
public:
    Jugador();
    void resolver_fase(Contexto& ctx) override;
    std::string nombre() const override;
};

#endif //PROYECTO_FINAL_JUGADOR_H