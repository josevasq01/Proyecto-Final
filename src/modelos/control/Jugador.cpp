//
// Created by lvjos on 30/11/2025.
//

#include "Jugador.h"
#include "../../core/Contexto.h"

Jugador::Jugador() {}
void Jugador::resolver_fase(Contexto& ctx){
    ctx.log("Resolver fase jugador (externalizada).");
}
std::string Jugador::nombre() const { return "Jugador"; }