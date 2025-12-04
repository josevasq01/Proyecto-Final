//
// Created by lvjos on 30/11/2025.
//

#include "Ingeniero.h"
#include "../../core/Contexto.h"
#include "../../sistemas/MapaMundo.h"

Ingeniero::Ingeniero(int owner, const Coordenada& pos): Unidad(owner,pos){ m_hp = 2; }

bool Ingeniero::mover(const Coordenada& destino, Contexto& ctx){
    int dx = abs(destino.x - m_pos.x), dy = abs(destino.y - m_pos.y);
    if(dx + dy != 1){ ctx.log("Ingeniero: movimiento no valido"); return false; }
    set_posicion(destino);
    ctx.log("Ingeniero movido");
    return true;
}

bool Ingeniero::atacar(Unidad& objetivo, Contexto& ctx){
    objetivo.damage(1);
    ctx.log("Ingeniero ataco; dano 1");
    return true;
}

std::string Ingeniero::tipo() const { return "Ingeniero"; }

bool Ingeniero::construir(const Coordenada& donde, const std::string& tipoEd, Contexto& ctx){
    return ctx.mapa->constructAt(m_owner, tipoEd, donde, ctx);
}