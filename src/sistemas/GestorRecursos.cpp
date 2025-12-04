//
// Created by lvjos on 30/11/2025.
//

#include "GestorRecursos.h"
#include "MapaMundo.h"
#include "../modelos/Unidades/Unidad.h"
#include "../core/Contexto.h"

void GestorRecursos::applyProduction(Contexto& ctx){
    ctx.log("Produccion aplicada.");
}

void GestorRecursos::applyMaintenance(Contexto& ctx){
    int totalUnits = 0;
    for(int y=0;y<ctx.mapa->height();++y) for(int x=0;x<ctx.mapa->width();++x){
        auto& c = ctx.mapa->at(x,y);
        if(c.unidad && c.unidad->owner()==1) totalUnits++;
    }
    if(ctx.recursos.comida >= totalUnits){
        ctx.recursos.comida -= totalUnits;
        ctx.log("Mantenimiento de unidades: -" + std::to_string(totalUnits) + " comida");
    } else {
        ctx.log("No hay comida suficiente: morale y penalizaciones (pendiente)");
    }
}