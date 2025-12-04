//
// Created by lvjos on 30/11/2025.
//

#include "Consola.h"
#include <iostream>
#include <limits>
#include <string>
#include <cmath>
#include "../sistemas/MapaMundo.h"
#include "RenderMapa.h"
#include "modelos/Edificios/Edificio.h"
#include "modelos/Unidades/Unidad.h"

void Consola::clear(){
    for(int i=0;i<30;i++);
    std::cout << "\n";
}

static std::string objectiveText(){
    // For demo: static objective. Later can be dynamic in Contexto.
    return "Alcanzar 60% de dominio antes del turno 20";
}

static std::string alertaText(const Contexto& ctx){
    // produce a simple alert: if any guardian (owner==2) unit in NE quadrant -> alert
    int w = ctx.mapa->width();
    int h = ctx.mapa->height();
    bool guardianNE = false;
    for(int y=0;y<h/2 && !guardianNE; ++y){
        for(int x=w/2; x<w; ++x){
            auto &c = ctx.mapa->at(x,y);
            if(c.unidad && c.unidad->owner()==2){ guardianNE = true; break; }
            if(c.edificio && c.edificio->owner()==2){ guardianNE = true; break; }
        }
    }
    if(guardianNE) return "Guardianes activos en cuadrante noreste";
    return "Sin alertas importantes";
}

void Consola::printPanelSuperior(const Contexto& ctx){
    // Turno / Objetivo / Recursos / Dominio / Alerta
    std::cout << "Turno #" << ctx.turno << "\n";
    std::cout << "Objetivo: " << objectiveText() << "\n";

    std::cout << "Recursos: Comida=" << ctx.recursos.comida
              << " | Metal=" << ctx.recursos.metal
              << " | Energia=" << ctx.recursos.energia << "\n";

    int total = ctx.mapa->width() * ctx.mapa->height();
    int player = ctx.mapa->countOwned(1);
    int system = ctx.mapa->countOwned(2);
    int neutral = total - player - system;
    int pctPlayer = (total==0) ? 0 : (player * 100) / total;
    int pctSystem = (total==0) ? 0 : (system * 100) / total;
    int pctNeutral = (total==0) ? 0 : (neutral * 100) / total;

    std::cout << "Jugador: Dominio=" << pctPlayer << "% | Sistema: Dominio=" << pctSystem << "%\n";
    std::cout << "Neutrales: " << pctNeutral << "%\n";

    std::cout << "Alerta: " << alertaText(ctx) << "\n";

    /* Mostrar ultimas actualizaciones
    std::cout << "Bitacora (ultimas 5):\n";
    int start = (int)ctx.bitacora.size() - 5;
    if(start < 0) start = 0;
    for(size_t i=start;i<ctx.bitacora.size();++i) std::cout << " - " << ctx.bitacora[i] << "\n";
    std::cout << "---------------------------\n";
    */
}

void Consola::renderMapa(const MapaMundo& mapa){
    RendererMapa::renderCompact(mapa);
}

void Consola::printMenu(){
    std::cout << "Acciones:\n";
    std::cout << "1) Mover unidad\n";
    std::cout << "2) Reclutar unidad\n"; // AHORA es menú general
    std::cout << "3) Construir edificio\n"; // opcional, si ya tienes
    std::cout << "4) Pasar\n";
    std::cout << "Selecciona una opcion: ";
}

int Consola::readRecruitUnitMenu(){
    std::cout << "--- Reclutar Unidad ---\n";
    std::cout << "1) Soldado\n";
    std::cout << "2) Arquero\n";
    std::cout << "3) Caballero\n";
    std::cout << "4) Mago\n";
    std::cout << "5) Ingeniero\n";
    std::cout << "Selecciona una opcion: ";
    int o;
    std::cin >> o;
    return o;
}

int Consola::readOption(){
    int o; std::cin >> o;
    if(!std::cin) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); return -1; }
    return o;
}

Coordenada Consola::readCoord(const std::string& prompt){
    std::cout << prompt;
    int x,y; std::cin >> x >> y;
    if(!std::cin){ std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); return Coordenada(-1,-1); }
    return Coordenada(x,y);
}