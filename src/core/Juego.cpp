//
// Created by lvjos on 30/11/2025.
//

#include "Juego.h"
#include "../interfaz/Consola.h"
#include "../sistemas/MotorEventos.h"
#include "../sistemas/GestorRecursos.h"
#include "../sistemas/IA_Guardian.h"
#include "../sistemas/MapaMundo.h"
#include <iostream>
#include <limits>

Juego::Juego(): ctx() {
    ctx.log("Juego inicializado");
}

int num_turnos = 5;

void Juego::run(){
    ctx.mapa->placeInitialDemo(ctx);

    bool running = true;
    while(running){
        std::cout << "============================================================";
        turnoInicio();
        Consola::clear();
        Consola::printPanelSuperior(ctx);
        Consola::renderMapa(*ctx.mapa);
        faseJugador();
        faseMundo();
        evaluarCondiciones();
        turnoFin();

        // Fin anticipado para pruebas
        if(ctx.turno > num_turnos)
            running = false;
    }

    // ======= MENSAJE FINAL =======
    int total = ctx.mapa->width() * ctx.mapa->height();
    int player = ctx.mapa->countOwned(1);
    int ia     = ctx.mapa->countOwned(2);

    int porcPlayer = (player * 100) / (total == 0 ? 1 : total);
    int porcIA     = (ia     * 100) / (total == 0 ? 1 : total);

    std::cout << "\n===================== FIN DE LA PARTIDA =====================\n";
    std::cout << "     Dominio jugador: " << porcPlayer << "%      |      ";
    std::cout << "Dominio IA: " << porcIA     << "%\n";

    Consola::renderMapa(*ctx.mapa);

    if(porcPlayer > porcIA)
        std::cout << "========================== VICTORIA =========================\n";
    else if(porcPlayer < porcIA)

        std::cout << "========================== DERROTA ==========================\n";
    else
        std::cout << "=========================== EMPATE ==========================\n";
}


void Juego::turnoInicio(){
    ctx.puntos_accion = 2;
    ctx.log("Inicio de turno " + std::to_string(ctx.turno));
}

void Juego::faseJugador(){
    ctx.log("Fase jugador: tienes " + std::to_string(ctx.puntos_accion) + " puntos de accion.");

    bool finished = false;

    while(ctx.puntos_accion > 0 && !finished){

        std::cout << "\nAcciones restantes: " << ctx.puntos_accion << "\n";
        Consola::printMenu();

        int opt = Consola::readOption();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // LIMPIA BUFFER

        switch(opt){
            case 1: {
                Coordenada from = Consola::readCoord("Desde (x y): ");
                Coordenada to   = Consola::readCoord("Hasta (x y): ");

                if(ctx.mapa->moveUnit(1, from, to, ctx)){
                    ctx.puntos_accion--;
                }

                break;
            }

            case 2: {
                int u = Consola::readRecruitUnitMenu();
                std::string tipo;
                switch(u){
                    case 1: tipo="Soldado"; break;
                    case 2: tipo="Arquero"; break;
                    case 3: tipo="Caballero"; break;
                    case 4: tipo="Mago"; break;
                    case 5: tipo="Ingeniero"; break;
                    default:
                        std::cout << "Opción inválida.\n";
                        continue;
                }

                Coordenada where = Consola::readCoord("Lugar (x y): ");
                if(ctx.mapa->recruitAt(1, tipo, where, ctx)){
                    ctx.puntos_accion--;
                }
                break;
            }

            case 3: {
                std::string tipoEd;
                std::cout << "Edificio (Granja/Cuartel/Torre/Forja): ";
                std::cin >> tipoEd;

                Coordenada pos = Consola::readCoord("Lugar (x y): ");
                if(ctx.mapa->constructAt(1, tipoEd, pos, ctx)){
                    ctx.puntos_accion--;
                }
                break;
            }

            case 4:
                finished = true;
                break;

            default:
                std::cout << "Opción inválida.\n";
                break;
        }

        // SIEMPRE imprimir mapa DESPUÉS de restar acción
        Consola::clear();
        //Consola::printPanelSuperior(ctx);
        Consola::renderMapa(*ctx.mapa);
    }
}


void Juego::faseMundo(){
    ctx.log("Fase mundo: ejecutando IA.");

    MotorEventos::process(ctx);

    // Guardar tamaño actual de la bitácora ANTES de la IA
    int inicioLogsIA = ctx.bitacora.size();

    // IA realiza acciones
    IA_Guardian::step(ctx);

    // Mostrar SOLO los logs generados por la IA
    std::cout << "--------- Acciones de la IA ---------\n";
    for(int i = inicioLogsIA; i < ctx.bitacora.size(); i++){
        const std::string& log = ctx.bitacora[i];
        if(log.rfind("IA", 0) == 0) {
            // Remover prefijo "IA:" o "IA "
            std::string limpio = log;
            if(limpio.rfind("IA: ", 0) == 0) limpio = limpio.substr(4);
            else if(limpio.rfind("IA ", 0) == 0) limpio = limpio.substr(3);
            std::cout << limpio << "\n";
        }
    }
    std::cout << "-------------------------------------\n";

    GestorRecursos::applyProduction(ctx);
    GestorRecursos::applyMaintenance(ctx);
}

void Juego::evaluarCondiciones(){
    int total = ctx.mapa->width() * ctx.mapa->height();
    int player = ctx.mapa->countOwned(1);
    int porcentaje = (player * 100) / (total==0?1:total);
    ctx.log("Dominio jugador = " + std::to_string(porcentaje) + "%");
}

void Juego::turnoFin(){
    ctx.log("Fin de turno " + std::to_string(ctx.turno));
    ctx.turno++;
}