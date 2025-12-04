//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_SISTEMA_H
#define PROYECTO_FINAL_SISTEMA_H

#include "Controlador.h"

class Sistema : public Controlador {
public:
    Sistema();
    void resolver_fase(Contexto& ctx) override;
    std::string nombre() const override;
};

#endif //PROYECTO_FINAL_SISTEMA_H