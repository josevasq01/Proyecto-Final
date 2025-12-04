//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_CONTEXTO_H
#define PROYECTO_FINAL_CONTEXTO_H

#include <vector>
#include <string>
#include <memory>

struct Recursos {
    int comida;
    int metal;
    int energia;
};

class MapaMundo; // forward

class Contexto {
public:
    int turno;
    Recursos recursos;
    std::shared_ptr<MapaMundo> mapa;
    std::vector<std::string> bitacora;
    int puntos_accion; // puntos de accion por turno para el jugador

    Contexto();
    void log(const std::string &msg);
};

#endif //PROYECTO_FINAL_CONTEXTO_H