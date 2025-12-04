//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_UNIDAD_H
#define PROYECTO_FINAL_UNIDAD_H

#include "core/Coordenadas.h"
#include <string>

class Contexto;

class Unidad {
public:
    Unidad(int owner, const Coordenada& pos);
    virtual ~Unidad() = default;

    int owner() const;
    Coordenada posicion() const;
    void set_posicion(const Coordenada& p);

    virtual bool mover(const Coordenada& destino, Contexto& ctx) = 0;
    virtual bool atacar(Unidad& objetivo, Contexto& ctx) = 0;
    virtual std::string tipo() const = 0;

    int hp() const;
    void damage(int d);
protected:
    int m_owner;
    Coordenada m_pos;
    int m_hp;
};

#endif //PROYECTO_FINAL_UNIDAD_H