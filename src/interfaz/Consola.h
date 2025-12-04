//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_CONSOLAIU_H
#define PROYECTO_FINAL_CONSOLAIU_H


#include "../core/Contexto.h"
#include "../core/Coordenadas.h"
#include <string>

class MapaMundo;

class Consola {
public:
    static void clear();
    static void printPanelSuperior(const Contexto& ctx);
    static void renderMapa(const MapaMundo& mapa);
    static void printMenu();
    static int readOption();
    static int readRecruitUnitMenu();
    static Coordenada readCoord(const std::string& prompt);
};

#endif //PROYECTO_FINAL_CONSOLAIU_H