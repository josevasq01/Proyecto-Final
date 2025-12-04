//
// Created by lvjos on 30/11/2025.
//

#include "Cuartel.h"
#include "../../core/Contexto.h"

Cuartel::Cuartel(int owner, const Coordenada& pos): Edificio(owner,pos){}

void Cuartel::efecto_turno(Contexto& ctx){
    ctx.log("Cuartel listo en (" + std::to_string(m_pos.x) + "," + std::to_string(m_pos.y) + ")");
}

std::string Cuartel::tipo() const { return "Cuartel"; }