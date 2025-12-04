//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_CONTROLADOR_H
#define PROYECTO_FINAL_CONTROLADOR_H

#include <string>

class Contexto;

class Controlador {
public:
    virtual ~Controlador() = default;
    virtual void resolver_fase(Contexto& ctx) = 0;
    virtual std::string nombre() const = 0;
};

#endif //PROYECTO_FINAL_CONTROLADOR_H