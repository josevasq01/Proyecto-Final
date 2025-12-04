//
// Created by lvjos on 30/11/2025.
//

#include "Forja.h"
#include "../../core/Contexto.h"

Forja::Forja(int owner, const Coordenada& pos): Edificio(owner,pos){}

void Forja::efecto_turno(Contexto& ctx){
    if(m_owner == 1) ctx.recursos.metal += 1;
    ctx.log("Forja produjo +1 metal para propietario " + std::to_string(m_owner));
}

std::string Forja::tipo() const { return "Forja"; }