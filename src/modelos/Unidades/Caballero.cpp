//
// Created by lvjos on 30/11/2025.
//

#include "Caballero.h"
#include "../../core/Contexto.h"

Caballero::Caballero(int owner, const Coordenada& pos): Unidad(owner,pos){ m_hp = 5; }

bool Caballero::mover(const Coordenada& destino, Contexto& ctx){
    int dx = abs(destino.x - m_pos.x), dy = abs(destino.y - m_pos.y);
    if(dx + dy != 1){ ctx.log("Caballero: movimiento no valido"); return false; }
    set_posicion(destino);
    ctx.log("Caballero movido");
    return true;
}

bool Caballero::atacar(Unidad& objetivo, Contexto& ctx){
    objetivo.damage(3);
    ctx.log("Caballero ataco; dano 3");
    return true;
}

std::string Caballero::tipo() const { return "Caballero"; }