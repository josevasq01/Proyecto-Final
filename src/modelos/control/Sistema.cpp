//
// Created by lvjos on 30/11/2025.
//

#include "Sistema.h"
#include "../../core/Contexto.h"
#include "../../sistemas/MapaMundo.h"

Sistema::Sistema(){}
void Sistema::resolver_fase(Contexto& ctx){
    ctx.log("Sistema (guardianes) ejecuta su fase.");
}
std::string Sistema::nombre() const { return "Sistema"; }