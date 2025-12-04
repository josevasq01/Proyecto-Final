//
// Created by lvjos on 30/11/2025.
//

#include "Arquero.h"
#include "../../core/Contexto.h"

Arquero::Arquero(int owner, const Coordenada& pos): Unidad(owner,pos){ m_hp = 2; }

bool Arquero::mover(const Coordenada& destino, Contexto& ctx){
    int dx = abs(destino.x - m_pos.x), dy = abs(destino.y - m_pos.y);
    if(dx + dy != 1){ ctx.log("Arquero: movimiento no valido"); return false; }
    set_posicion(destino);
    ctx.log("Arquero movido");
    return true;
}

bool Arquero::atacar(Unidad& objetivo, Contexto& ctx){
    objetivo.damage(1);
    ctx.log("Arquero ataco; dano 1");
    return true;
}

std::string Arquero::tipo() const { return "Arquero"; }