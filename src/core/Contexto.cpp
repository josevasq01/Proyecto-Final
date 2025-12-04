//
// Created by lvjos on 30/11/2025.
//

#include "Contexto.h"
#include "../sistemas/MapaMundo.h"
#include <fstream>

Contexto::Contexto(): turno(1), puntos_accion(2) {
    recursos.comida = 18;
    recursos.metal = 12;
    recursos.energia = 7;
    mapa = std::make_shared<MapaMundo>(6,6);
}

void Contexto::log(const std::string& msg){
    // 1) Guardar en archivo log.txt
    std::ofstream file("log.txt", std::ios::app);
    if(file.is_open()){
        file << msg << "\n";
    }

    bitacora.push_back(msg);
}
