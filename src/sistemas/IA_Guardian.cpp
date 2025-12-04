//
// Created by lvjos on 30/11/2025.
//

#include "IA_Guardian.h"
#include "sistemas/MapaMundo.h"
#include "core/Contexto.h"
#include "core/Coordenadas.h"
#include "modelos/Edificios/Edificio.h"
#include "modelos/Terrenos/Terreno.h"
#include "modelos/Unidades/Unidad.h"
#include <queue>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <functional>
#include <numeric>

namespace {

// configuración de la IA
static const int MAX_ACTIONS_PER_STEP = 2; // actions por ciclo (ajusta si quieres)
static const double CHANCE_RECRUIT_ARCHER = 0.40; // probabilidad de arquero vs soldado
static std::mt19937 rng((unsigned) std::random_device{}());

// util: distancia Manhattan
static int manhattan(const Coordenada &a, const Coordenada &b){
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

// util: busca todas las coordenadas de unidades del owner
static std::vector<Coordenada> findUnits(Contexto &ctx, int owner){
    std::vector<Coordenada> out;
    for(int y=0;y<ctx.mapa->height();++y)
        for(int x=0;x<ctx.mapa->width();++x){
            auto &c = ctx.mapa->at(x,y);
            if(c.unidad && c.unidad->owner() == owner) out.push_back(Coordenada(x,y));
        }
    return out;
}

// util: busca edificios del owner
static std::vector<Coordenada> findBuildings(Contexto &ctx, int owner){
    std::vector<Coordenada> out;
    for(int y=0;y<ctx.mapa->height();++y)
        for(int x=0;x<ctx.mapa->width();++x){
            auto &c = ctx.mapa->at(x,y);
            if(c.edificio && c.edificio->owner() == owner) out.push_back(Coordenada(x,y));
        }
    return out;
}

// BFS para encontrar la ruta mínima (devuelve primer paso hacia objetivo). Retorna true si encontró.
// start = origen, targetCond = función que indica si esa casilla es objetivo.
// nextStep será la coordenada de un paso (adyacente) desde start hacia la meta.
static bool find_step_towards(Contexto &ctx, const Coordenada &start,
                              const std::function<bool(const Celda& , const Coordenada&)>& targetCond,
                              Coordenada &nextStep)
{
    int W = ctx.mapa->width();
    int H = ctx.mapa->height();
    std::queue<Coordenada> q;
    std::vector<std::vector<bool>> vis(H, std::vector<bool>(W,false));
    std::vector<std::vector<Coordenada>> parent(H, std::vector<Coordenada>(W, Coordenada(-1,-1)));

    auto push = [&](int x,int y){
        if(x<0||y<0||x>=W||y>=H) return;
        if(vis[y][x]) return;
        vis[y][x] = true;
        q.push(Coordenada(x,y));
    };

    push(start.x, start.y);
    bool found = false;
    Coordenada foundAt(-1,-1);

    while(!q.empty() && !found){
        Coordenada c = q.front(); q.pop();
        const Celda &cell = ctx.mapa->at(c.x, c.y);

        if(targetCond(cell, c)){
            found = true;
            foundAt = c;
            break;
        }

        // vecinos 4-dir
        const int dx[4] = {1,-1,0,0};
        const int dy[4] = {0,0,1,-1};
        for(int i=0;i<4;++i){
            int nx = c.x + dx[i];
            int ny = c.y + dy[i];
            if(nx<0||ny<0||nx>=W||ny>=H) continue;
            if(vis[ny][nx]) continue;
            vis[ny][nx] = true;
            parent[ny][nx] = c;
            q.push(Coordenada(nx,ny));
        }
    }

    if(!found) return false;

    // reconstruir camino: desde foundAt hasta start, tomar el segundo nodo (primer paso)
    Coordenada cur = foundAt;
    Coordenada prev = cur;
    while(!(cur.x == start.x && cur.y == start.y)){
        prev = cur;
        cur = parent[cur.y][cur.x];
        if(cur.x == -1 && cur.y == -1) break; // safety
    }
    // prev es la casilla inmediata después de start en la ruta
    nextStep = prev;
    // si prev == start (ocurre cuando start es objetivo) -> nextStep igual start -> devolver false (no moverse)
    if(nextStep.x == start.x && nextStep.y == start.y) return false;
    return true;
}

// chequea si una casilla es "apta para construir" (simple): terreno LL o BO o PA (ajustable)
static bool terrenoAptoParaEdificio(const Celda &c){
    if(!c.terreno) return false;
    std::string code = c.terreno->codigo();
    return (code == "LL" || code == "BO" || code == "PA");
}

} // namespace (helpers privados)

namespace IA_Guardian {

    // implementado en .h; función pública: buscar y construir tipoEd (primera casilla válida)
    bool tryBuild(Contexto& ctx, const std::string& tipoEd){
        int owner = 2;
        int W = ctx.mapa->width();
        int H = ctx.mapa->height();

        for(int y=0; y<H; y++){
            for(int x=0; x<W; x++){
                auto &c = ctx.mapa->at(x,y);

                // solo construir si la casilla es del owner, está libre y el terreno sirve
                if(c.propietario == owner && !c.unidad && !c.edificio && terrenoAptoParaEdificio(c)){
                    if(ctx.mapa->constructAt(owner, tipoEd, Coordenada(x,y), ctx)){
                        ctx.log("IA Construyo " + tipoEd + " en ("+std::to_string(x)+","+std::to_string(y)+")");
                        return true;
                    }
                }
            }
        }
        return false;
    }

    // intenta reclutar 1 unidad (prioriza cuarteles y espacios adyacentes). Devuelve true si reclutó.
    static bool tryRecruit(Contexto& ctx){
        int owner = 2;
        int W = ctx.mapa->width();
        int H = ctx.mapa->height();
        auto buildings = findBuildings(ctx, owner);

        // shufle para variar comportamiento
        std::shuffle(buildings.begin(), buildings.end(), rng);

        for(auto &bpos : buildings){
            auto &cell = ctx.mapa->at(bpos.x, bpos.y);
            if(!cell.edificio) continue;
            std::string t = cell.edificio->tipo();

            // only consider recruiting near buildings that produce units: Cuartel => Soldiers/Caballeros, Torre => archers/magos, Forja => engineers maybe
            const int dx[8] = {1,-1,0,0,1,1,-1,-1};
            const int dy[8] = {0,0,1,-1,1,-1,1,-1};

            for(int i=0;i<8;i++){
                int nx = bpos.x + dx[i];
                int ny = bpos.y + dy[i];
                if(nx<0||ny<0||nx>=W||ny>=H) continue;
                auto &adj = ctx.mapa->at(nx, ny);
                if(adj.unidad || adj.edificio) continue;
                if(adj.propietario != owner) continue;

                // elegir tipo según edificio
                std::string tipo = "Soldado";
                if(t == "Cuartel"){
                    // soldado o caballero (baja prob caballero)
                    std::uniform_real_distribution<> d(0.0,1.0);
                    double r = d(rng);
                    if(r < 0.15) tipo = "Caballero";
                    else tipo = (d(rng) < CHANCE_RECRUIT_ARCHER ? "Arquero" : "Soldado"); // small chance de arquero near cuartel too
                } else if(t == "Torre"){
                    // torre produce arqueros o magos
                    std::uniform_real_distribution<> d(0.0,1.0);
                    tipo = (d(rng) < 0.25 ? "Mago" : "Arquero");
                } else if(t == "Forja"){
                    // forja -> ingeniero or caballero
                    std::uniform_real_distribution<> d(0.0,1.0);
                    tipo = (d(rng) < 0.3 ? "Ingeniero" : "Soldado");
                } else {
                    tipo = "Soldado";
                }

                if(ctx.mapa->recruitAt(owner, tipo, Coordenada(nx,ny), ctx)){
                    ctx.log("IA Recluto " + tipo + " en (" + std::to_string(nx) + "," + std::to_string(ny) + ")");
                    return true;
                }
            }
        }

        return false;
    }

    // intenta construir de forma "inteligente" según prioridad. Devuelve true si construyó algo.
    static bool tryBuildSmart(Contexto& ctx){
        int owner = 2;
        auto buildings = findBuildings(ctx, owner);
        int countCuartel = 0, countTorre = 0, countForja = 0;
        for(auto &b : buildings){
            auto &cell = ctx.mapa->at(b.x,b.y);
            if(cell.edificio){
                std::string t = cell.edificio->tipo();
                if(t == "Cuartel") ++countCuartel;
                if(t == "Torre") ++countTorre;
                if(t == "Forja") ++countForja;
            }
        }

        // Prioridades en orden (puedes ajustar condiciones)
        // 1) Cuartel si < 2
        if(countCuartel < 2){
            if(tryBuild(ctx, "Cuartel")) return true;
        }
        // 2) Torre si tiene al menos 1 cuartel y menos de 2 torres
        if(countCuartel >= 1 && countTorre < 2){
            if(tryBuild(ctx, "Torre")) return true;
        }
        // 3) Forja si tiene torre y no tiene forja
        if(countTorre >= 1 && countForja < 1){
            if(tryBuild(ctx, "Forja")) return true;
        }
        // 4) Granja si comida baja
        if(ctx.recursos.comida < 8){
            if(tryBuild(ctx, "Granja")) return true;
        }

        // 5) si nada, construir torre/granja en cualquier casilla controlada (fallback)
        if(tryBuild(ctx, "Torre")) return true;
        if(tryBuild(ctx, "Granja")) return true;

        return false;
    }

    // intenta mover/atacar con una unidad. Devuelve true si realizó alguna acción (mover/ataque/captura)
    static bool tryMoveAttack(Contexto& ctx){
        int owner = 2;
        int W = ctx.mapa->width();
        int H = ctx.mapa->height();

        auto units = findUnits(ctx, owner);
        if(units.empty()) return false;

        // buscar unidades enemigas para referencia
        std::vector<Coordenada> enemyUnits;
        for(int y=0;y<H;++y) for(int x=0;x<W;++x){
            auto &cc = ctx.mapa->at(x,y);
            if(cc.unidad && cc.unidad->owner() == 1) enemyUnits.push_back(Coordenada(x,y));
        }

        // ordenar unidades para que las que están más cerca actúen primero
        std::sort(units.begin(), units.end(), [&](const Coordenada &a, const Coordenada &b){
            int da = enemyUnits.empty() ? 0 : std::accumulate(enemyUnits.begin(), enemyUnits.end(), 0,
                        [&](int acc, const Coordenada &eu){ return acc + manhattan(a, eu); });
            int db = enemyUnits.empty() ? 0 : std::accumulate(enemyUnits.begin(), enemyUnits.end(), 0,
                        [&](int acc, const Coordenada &eu){ return acc + manhattan(b, eu); });
            return da < db;
        });

        for(auto &uPos : units){
            // 1) atacar unidad enemiga
            auto targetUnitCond = [&](const Celda &cell, const Coordenada &coord)->bool{
                return (cell.unidad && cell.unidad->owner() == 1);
            };
            Coordenada step;
            bool found = find_step_towards(ctx, uPos, targetUnitCond, step);
            if(found){
                auto &dest = ctx.mapa->at(step.x, step.y);
                if(dest.unidad && dest.unidad->owner() == 1){
                    // eliminar enemiga y mover
                    dest.unidad = nullptr;
                    if(ctx.mapa->moveUnit(owner, uPos, step, ctx)){
                        ctx.log("IA: Unidad ataco y tomo casilla (" + std::to_string(step.x) + "," + std::to_string(step.y) + ")");
                        return true;
                    }
                } else {
                    // mover/capturar si está vacía y no es edificio propio
                    if(!dest.unidad && !dest.edificio){
                        if(ctx.mapa->moveUnit(owner, uPos, step, ctx)){
                            ctx.mapa->at(step.x, step.y).propietario = owner;
                            ctx.log("IA: Unidad movida y capturo (" + std::to_string(step.x) + "," + std::to_string(step.y) + ")");
                            return true;
                        }
                    }
                }
            }

            // 2) acercarse a edificio enemigo
            auto targetEdCond = [&](const Celda &cell, const Coordenada &coord)->bool{
                return (cell.edificio && cell.edificio->owner() == 1);
            };
            found = find_step_towards(ctx, uPos, targetEdCond, step);
            if(found){
                auto &dest = ctx.mapa->at(step.x, step.y);
                if(!dest.unidad && !dest.edificio){
                    if(ctx.mapa->moveUnit(owner, uPos, step, ctx)){
                        ctx.mapa->at(step.x, step.y).propietario = owner;
                        ctx.log("IA: Unidad movida hacia edificio enemigo (" + std::to_string(step.x) + "," + std::to_string(step.y) + ")");
                        return true;
                    }
                }
            }

            // 3) expandir hacia neutral
            auto targetNeutral = [&](const Celda &cell, const Coordenada &coord)->bool{
                return (cell.propietario != owner && !cell.edificio);
            };
            found = find_step_towards(ctx, uPos, targetNeutral, step);
            if(found){
                auto &dest = ctx.mapa->at(step.x, step.y);
                if(!dest.unidad && !dest.edificio){
                    if(ctx.mapa->moveUnit(owner, uPos, step, ctx)){
                        ctx.mapa->at(step.x, step.y).propietario = owner;
                        ctx.log("IA: Unidad expandio a (" + std::to_string(step.x) + "," + std::to_string(step.y) + ")");
                        return true;
                    }
                }
            }
        }

        return false;
    }

    // fallback: intenta reclutar soldados alrededor de cuarteles (último recurso)
    static void fallbackRecruit(Contexto& ctx){
        int owner = 2;
        int W = ctx.mapa->width();
        int H = ctx.mapa->height();
        auto buildings = findBuildings(ctx, owner);
        for(auto &b : buildings){
            auto &cell = ctx.mapa->at(b.x,b.y);
            if(cell.edificio && cell.edificio->tipo() == "Cuartel"){
                const int dx[4] = {1,-1,0,0};
                const int dy[4] = {0,0,1,-1};
                for(int i=0;i<4;i++){
                    int nx = b.x + dx[i];
                    int ny = b.y + dy[i];
                    if(nx<0||ny<0||nx>=W||ny>=H) continue;
                    auto &adj = ctx.mapa->at(nx, ny);
                    if(adj.unidad || adj.edificio) continue;
                    if(adj.propietario != owner) continue;
                    if(ctx.mapa->recruitAt(owner, "Soldado", Coordenada(nx,ny), ctx)){
                        ctx.log("IA: Recluto Soldado en (" + std::to_string(nx) + "," + std::to_string(ny) + ")");
                        return;
                    }
                }
            }
        }
    }

    // función principal: slot-based
    void step(Contexto& ctx){
        int acciones = 0;

        // SLOT 1: intentar reclutar (prioridad alta)
        if(acciones < MAX_ACTIONS_PER_STEP){
            if(tryRecruit(ctx)){
                acciones++;
            }
        }

        // SLOT 2: intentar construir (prioridad media)
        if(acciones < MAX_ACTIONS_PER_STEP){
            if(tryBuildSmart(ctx)){
                acciones++;
            }
        }

        // SLOT 3: intentar mover/atacar (prioridad baja)
        if(acciones < MAX_ACTIONS_PER_STEP){
            if(tryMoveAttack(ctx)){
                acciones++;
            }
        }

        // si no hizo nada, fallback: tratar de reclutar para mantener presión
        if(acciones == 0){
            // solo en turnos pares para no saturar
            if(ctx.turno % 2 == 0){
                fallbackRecruit(ctx);
            }
        }
    }

}
