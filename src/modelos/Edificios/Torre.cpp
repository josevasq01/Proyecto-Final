//
// Created by lvjos on 30/11/2025.
//

#include "Torre.h"
#include "../../core/Contexto.h"

Torre::Torre(int owner, const Coordenada& pos): Edificio(owner,pos){}

void Torre::efecto_turno(Contexto& ctx){
    if(m_owner == 1) ctx.recursos.energia += 1;
    ctx.log("Torre efecto_turno: +1 energia para propietario " + std::to_string(m_owner));
}

std::string Torre::tipo() const { return "Torre"; }