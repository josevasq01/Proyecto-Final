//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_MAPAMUNDO_H
#define PROYECTO_FINAL_MAPAMUNDO_H

#include <vector>
#include <memory>
#include <string>
#include "core/Coordenadas.h"

class Terreno;
class Unidad;
class Edificio;
class Contexto;

struct Celda {
    std::shared_ptr<Terreno> terreno;
    std::shared_ptr<Unidad> unidad;
    std::shared_ptr<Edificio> edificio;
    int propietario; // 0 neutral, 1 jugador, 2 sistema
    Celda();
};

class MapaMundo {
public:
    MapaMundo(int width, int height);
    int width() const;
    int height() const;
    const Celda& at(int x,int y) const;
    Celda& at(int x,int y);
    void printCompact() const; // legacy, kept
    std::string cellCompact(int x,int y) const; // new: devuelve "J1S/LL" o "../LL" (sin corchetes)

    // actions
    void placeInitialDemo(Contexto& ctx);
    bool moveUnit(int owner, const Coordenada& from, const Coordenada& to, Contexto& ctx);
    bool recruitAt(int owner, const std::string& tipoUn, const Coordenada& pos, Contexto& ctx);
    bool constructAt(int owner, const std::string& tipoEd, const Coordenada& pos, Contexto& ctx);
    int countOwned(int owner) const;
    void expandirControl(int owner, const Coordenada& c);
    bool estaCercaDeEdificio(int owner, const Coordenada& pos, const std::string& tipoEd);
    Coordenada buscarTerrenoAptoCercano(Coordenada start);

private:
    int w,h;
    std::vector<std::vector<Celda>> grid;
    void fillDefaultTerrain();
};

#endif //PROYECTO_FINAL_MAPAMUNDO_H