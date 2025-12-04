//
// Created by lvjos on 30/11/2025.
//

#include "Mago.h"
#include "../../core/Contexto.h"

Mago::Mago(int owner, const Coordenada& pos): Unidad(owner,pos){ m_hp = 2; }

bool Mago::mover(const Coordenada& destino, Contexto& ctx){
    int dx = abs(destino.x - m_pos.x), dy = abs(destino.y - m_pos.y);
    if(dx + dy != 1){ ctx.log("Mago: movimiento no valido"); return false; }
    set_posicion(destino);
    ctx.log("Mago movido");
    return true;
}

bool Mago::atacar(Unidad& objetivo, Contexto& ctx){
    objetivo.damage(2);
    ctx.log("Mago ataco; dano 2");
    return true;
}

std::string Mago::tipo() const { return "Mago"; }