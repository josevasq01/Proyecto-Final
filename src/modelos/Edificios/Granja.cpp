//
// Created by lvjos on 30/11/2025.
//

#include "Granja.h"
#include "../../core/Contexto.h"

Granja::Granja(int owner, const Coordenada& pos): Edificio(owner,pos) {}

void Granja::efecto_turno(Contexto& ctx){
    if(m_owner == 1) ctx.recursos.comida += 2;
    else if(m_owner == 2) { /* sistema produce si quieres */ }
    ctx.log("Granja produjo +2 comida para propietario " + std::to_string(m_owner));
}

std::string Granja::tipo() const { return "Granja"; }