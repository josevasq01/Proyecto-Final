//
// Created by lvjos on 30/11/2025.
//

#include "Soldado.h"
#include "../../core/Contexto.h"

Soldado::Soldado(int owner, const Coordenada& pos) : Unidad(owner, pos) { m_hp = 3; }

bool Soldado::mover(const Coordenada& destino, Contexto& ctx){
    int dx = abs(destino.x - m_pos.x), dy = abs(destino.y - m_pos.y);
    if(dx + dy != 1){ ctx.log("Soldado: movimiento no ortogonal de 1"); return false; }
    set_posicion(destino);
    ctx.log("Soldado movido a (" + std::to_string(destino.x) + "," + std::to_string(destino.y) + ")");
    return true;
}

bool Soldado::atacar(Unidad& objetivo, Contexto& ctx){
    objetivo.damage(2);
    ctx.log("Soldado ataco objetivo; dano 2");
    if(objetivo.hp() <= 0) ctx.log("Objetivo eliminado");
    return true;
}

std::string Soldado::tipo() const { return "Soldado"; }