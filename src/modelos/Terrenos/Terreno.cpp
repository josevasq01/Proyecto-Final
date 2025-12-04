//
// Created by lvjos on 30/11/2025.
//

#include "Terreno.h"
#include "Llanura.h"
#include "Bosque.h"
#include "Montania.h"
#include "Agua.h"
#include "Pantano.h"
#include <memory>

std::unique_ptr<Terreno> crearTerreno(TipoTerreno t){
    switch(t){
        case TipoTerreno::LLANURA: return std::make_unique<Llanura>();
        case TipoTerreno::BOSQUE: return std::make_unique<Bosque>();
        case TipoTerreno::MONTANIA: return std::make_unique<Montania>();
        case TipoTerreno::AGUA: return std::make_unique<Agua>();
        case TipoTerreno::PANTANO: return std::make_unique<Pantano>();
        default: return nullptr;
    }
}