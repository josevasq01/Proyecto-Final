//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_EDIFICIOS_H
#define PROYECTO_FINAL_EDIFICIOS_H

#include "core/Coordenadas.h"
#include <string>

class Contexto;

class Edificio {
public:
    Edificio(int owner, const Coordenada& pos);
    virtual ~Edificio() = default;
    int owner() const;
    Coordenada posicion() const;
    virtual void efecto_turno(Contexto& ctx) = 0;
    virtual std::string tipo() const = 0;
protected:
    int m_owner;
    Coordenada m_pos;
};

#endif //PROYECTO_FINAL_EDIFICIOS_H