//
// Created by lvjos on 30/11/2025.
//

#include "MotorEventos.h"
#include "MapaMundo.h"
#include "modelos/Edificios/Edificio.h"

void MotorEventos::process(Contexto& ctx){
    if(ctx.turno % 5 == 0){
        ctx.log("Evento global: brisa calma (sin efecto mayor)");
    }
    for(int y=0;y<ctx.mapa->height();++y){
        for(int x=0;x<ctx.mapa->width();++x){
            auto& c = ctx.mapa->at(x,y);
            if(c.edificio) c.edificio->efecto_turno(ctx);
        }
    }
}
