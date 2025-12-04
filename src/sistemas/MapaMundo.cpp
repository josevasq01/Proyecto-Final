//
// Created by lvjos on 30/11/2025.
//

#include "MapaMundo.h"
#include "../modelos/terrenos/Llanura.h"
#include "../modelos/terrenos/Bosque.h"
#include "../modelos/terrenos/Montania.h"
#include "../modelos/terrenos/Agua.h"
#include "../modelos/terrenos/Pantano.h"
#include "../modelos/unidades/Soldado.h"
#include "../modelos/unidades/Arquero.h"
#include "../modelos/unidades/Caballero.h"
#include "../modelos/unidades/Mago.h"
#include "../modelos/unidades/Ingeniero.h"
#include "../modelos/edificios/Cuartel.h"
#include "../modelos/edificios/Granja.h"
#include "../modelos/edificios/Torre.h"
#include "../modelos/edificios/Forja.h"
#include "../core/Contexto.h"
#include <iostream>
#include <iomanip>
#include <random>
#include <sstream>
#include <queue>
#include <set>

Celda::Celda(): terreno(nullptr), unidad(nullptr), edificio(nullptr), propietario(0) {}

MapaMundo::MapaMundo(int width, int height): w(width), h(height){
    grid.resize(h, std::vector<Celda>(w));
    fillDefaultTerrain();
}

Coordenada MapaMundo::buscarTerrenoAptoCercano(Coordenada start){
    std::queue<Coordenada> q;
    std::set<std::pair<int,int>> vis;

    auto add = [&](int x,int y){
        if(x<0 || y<0 || x>=w || y>=h) return;
        if(vis.count({x,y})) return;
        vis.insert({x,y});
        q.push(Coordenada(x,y));
    };

    add(start.x, start.y);

    while(!q.empty()){
        Coordenada c = q.front(); q.pop();
        Celda &cell = grid[c.y][c.x];

        // Debe ser LL + estar libre
        if(cell.terreno->codigo() == "LL" &&
           !cell.unidad &&
           !cell.edificio)
        {
            return c;
        }

        add(c.x+1,c.y);
        add(c.x-1,c.y);
        add(c.x,  c.y+1);
        add(c.x,  c.y-1);
    }

    return start;
}

void MapaMundo::fillDefaultTerrain(){
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, 4);

    for(int y=0;y<h;++y){
        for(int x=0;x<w;++x){

            int r = dist(rng);
            switch(r){
                case 0: grid[y][x].terreno = std::make_shared<Llanura>(); break;
                case 1: grid[y][x].terreno = std::make_shared<Bosque>(); break;
                case 2: grid[y][x].terreno = std::make_shared<Montania>(); break;
                case 3: grid[y][x].terreno = std::make_shared<Agua>(); break;
                case 4: grid[y][x].terreno = std::make_shared<Pantano>(); break;
            }
        }
    }
}

int MapaMundo::width() const { return w; }
int MapaMundo::height() const { return h; }

const Celda& MapaMundo::at(int x,int y) const { return grid[y][x]; }
Celda& MapaMundo::at(int x,int y) { return grid[y][x]; }

void MapaMundo::printCompact() const {
    // legacy simple view (kept)
    for(int y=0;y<h;++y){
        for(int x=0;x<w;++x){
            const Celda& c = grid[y][x];
            char ch='.';
            if(c.edificio) ch = 'B';
            else if(c.unidad) ch = (c.unidad->owner()==1 ? 'S' : 's');
            else if(c.propietario==1) ch = 'j';
            else if(c.propietario==2) ch = 'g';
            std::cout << "[" << ch << "]";
        }
        std::cout << "\n";
    }
}

static std::string unitAbbrev(const std::string& tipo){
    if(tipo == "Soldado") return "So";
    if(tipo == "Arquero") return "Ar";
    if(tipo == "Caballero") return "Ca";
    if(tipo == "Mago") return "Ma";
    if(tipo == "Ingeniero") return "In";
    // fallback: take first letter
    return tipo.empty() ? "U" : std::string(1, toupper(tipo[0]));
}

static std::string edificioAbbrev(const std::string& tipo){
    if(tipo == "Cuartel") return "Cu";
    if(tipo == "Granja") return "Gr";
    if(tipo == "Torre") return "To";
    if(tipo == "Forja")  return "Fo";
    return tipo.empty() ? "E" : tipo.substr(0,2);
}

std::string MapaMundo::cellCompact(int x,int y) const {
    // returns e.g. "J1S/LL" or "../LL" or "NeCu/LL"
    if(x<0 || y<0 || x>=w || y>=h) return "  ../..  ";
    const Celda& c = grid[y][x];
    std::ostringstream ss;
    // prefix: unit or building or neutral
    if(c.unidad){
        std::string t = c.unidad->tipo(); // e.g. "Soldado"
        std::string ab = unitAbbrev(t);
        ss << (c.unidad->owner()==1 ? "J1" : "J2") << ab;
    } else if(c.edificio){
        std::string t = c.edificio->tipo(); // e.g. "Cuartel"
        std::string ab = edificioAbbrev(t);
        int owner = c.edificio->owner();
        if(owner==0) ss << "Ne" << ab;
        else ss << (owner==1 ? "J1" : "J2") << ab;
    } else {
        ss << "  ..";
    }
    ss << "/" ;
    if(c.terreno) ss << c.terreno->codigo();
    else ss << "??";
    return ss.str();
}

void MapaMundo::placeInitialDemo(Contexto& ctx){
    // Asegurar terrenos válidos
    grid[0][0].terreno     = std::make_shared<Llanura>();
    grid[h-1][w-1].terreno = std::make_shared<Llanura>();

    // --- Jugador 1 ---
    Coordenada p1(0,0);
    grid[p1.y][p1.x].edificio = std::make_shared<Cuartel>(1, p1);
    expandirControl(1, p1);

    // --- Jugador 2 ---
    Coordenada p2(w-1, h-1);    // último casillero (11,11 en mapa 12x12)
    grid[p2.y][p2.x].edificio = std::make_shared<Cuartel>(2, p2);
    expandirControl(2, p2);

    ctx.log("Mundo inicial fijo colocado.");
}

bool MapaMundo::moveUnit(int owner, const Coordenada& from, const Coordenada& to, Contexto& ctx){
    if(from.x<0||from.y<0||to.x<0||to.y<0||from.x>=w||from.y>=h||to.x>=w||to.y>=h){ ctx.log("Coordenada fuera de mapa"); return false; }
    Celda& cf = grid[from.y][from.x];
    Celda& ct = grid[to.y][to.x];
    if(!cf.unidad){ ctx.log("No hay unidad en la casilla origen"); return false; }
    if(cf.unidad->owner() != owner){ ctx.log("Unidad no pertenece al jugador"); return false; }
    if(ct.unidad){ ctx.log("Casilla destino ocupada"); return false; }
    ct.unidad = cf.unidad;
    ct.unidad->set_posicion(to);
    ct.propietario = owner;
    cf.unidad = nullptr;
    ctx.log("Unidad movida");
    return true;
}

struct CosteUnidad {
    int comida;
    int metal;
    int energia;
};

static CosteUnidad costoUnidad(const std::string& tipo){
    if(tipo == "Soldado")  return {1, 1, 0};
    if(tipo == "Arquero")  return {1, 1, 1};
    if(tipo == "Caballero")return {2, 2, 1};
    if(tipo == "Mago")     return {1, 0, 3};
    if(tipo == "Ingeniero")return {0, 2, 1};

    return {999,999,999}; // tipo desconocido
}

struct CosteEdificio {
    int comida;
    int metal;
    int energia;
};

static CosteEdificio costoEdificio(const std::string& tipo){
    if(tipo == "Granja")  return {0, 1, 0};
    if(tipo == "Cuartel") return {1, 2, 0};
    if(tipo == "Torre")   return {0, 2, 1};
    if(tipo == "Forja")   return {1, 1, 2};
    return {999,999,999};
}

bool MapaMundo::recruitAt(int owner, const std::string& tipoUn, const Coordenada& pos, Contexto& ctx){
    if(pos.x<0||pos.y<0||pos.x>=w||pos.y>=h){
        ctx.log("Posicion invalida");
        return false;
    }
    Celda& c = grid[pos.y][pos.x];

    if(c.unidad || c.edificio){
        ctx.log("Casilla no libre");
        return false;
    }
    if(c.propietario != owner){
        ctx.log("No controlas esta casilla");
        return false;
    }

    // ================== COSTO =====================
    CosteUnidad cost = costoUnidad(tipoUn);

    if(owner == 1){
        if(ctx.recursos.comida < cost.comida ||
           ctx.recursos.metal  < cost.metal  ||
           ctx.recursos.energia< cost.energia)
        {
            ctx.log("Recursos insuficientes para " + tipoUn);
            return false;
        }
    }

    // ======= REQUISITOS POR UNIDAD =======
    bool tieneCuartel = false;
    bool tieneTorre = false;
    bool tieneForja = false;

    // revisar todo el mapa en busca de edificio del jugador
    for(int y=0; y<h; y++){
        for(int x=0; x<w; x++){
            Celda& ce = grid[y][x];
            if(!ce.edificio) continue;
            if(ce.edificio->owner() != owner) continue;
            std::string tipoEd = ce.edificio->tipo();
            if(tipoEd == "Cuartel") tieneCuartel = true;
            if(tipoEd == "Torre")   tieneTorre   = true;
            if(tipoEd == "Forja")   tieneForja   = true;
        }
    }

    // SOLDADO
    if(tipoUn == "Soldado"){
        if(!tieneCuartel){
            ctx.log("Requiere Cuartel");
            return false;
        }
        if(!estaCercaDeEdificio(owner, pos, "Cuartel")){
            ctx.log("Debe reclutarse adyacente a un Cuartel");
            return false;
        }
        c.unidad = std::make_shared<Soldado>(owner, pos);
    }

    // ARQUERO
    else if(tipoUn == "Arquero"){
        if(!tieneTorre){
            ctx.log("Requiere Torre");
            return false;
        }
        if(!estaCercaDeEdificio(owner, pos, "Torre")){
            ctx.log("Debe reclutarse adyacente a una Torre");
            return false;
        }
        c.unidad = std::make_shared<Arquero>(owner, pos);
    }

    // CABALLERO
    else if(tipoUn == "Caballero"){
        if(!tieneCuartel){
            ctx.log("Requiere Cuartel");
            return false;
        }
        if(!estaCercaDeEdificio(owner, pos, "Cuartel")){
            ctx.log("Debe reclutarse adyacente a un Cuartel");
            return false;
        }
        c.unidad = std::make_shared<Caballero>(owner, pos);
    }

    // MAGO
    else if(tipoUn == "Mago"){
        if(!tieneTorre){
            ctx.log("Requiere Torre");
            return false;
        }
        if(!estaCercaDeEdificio(owner, pos, "Torre")){
            ctx.log("Debe reclutarse adyacente a una Torre");
            return false;
        }
        c.unidad = std::make_shared<Mago>(owner, pos);
    }

    // INGENIERO
    else if(tipoUn == "Ingeniero"){
        if(!tieneForja){
            ctx.log("Requiere Forja");
            return false;
        }
        if(!estaCercaDeEdificio(owner, pos, "Forja")){
            ctx.log("Debe reclutarse adyacente a una Forja");
            return false;
        }
        c.unidad = std::make_shared<Ingeniero>(owner, pos);
    }

    ctx.log(tipoUn + " reclutado en (" +
            std::to_string(pos.x) + "," +
            std::to_string(pos.y) + ")");

    // PAGAR EL COSTO
    if(owner == 1){
        ctx.recursos.comida  -= cost.comida;
        ctx.recursos.metal   -= cost.metal;
        ctx.recursos.energia -= cost.energia;
    }

    return true;
}

bool MapaMundo::constructAt(int owner, const std::string& tipoEd, const Coordenada& pos, Contexto& ctx){
    if(pos.x < 0 || pos.y < 0 || pos.x >= w || pos.y >= h){
        ctx.log("Posicion invalida para construir");
        return false;
    }

    Celda& c = grid[pos.y][pos.x];

    if(c.edificio || c.unidad){
        ctx.log("Casilla ocupada, no se puede construir");
        return false;
    }

    if(c.propietario != owner){
        ctx.log("No controlas esta casilla para construir");
        return false;
    }

    // ========================
    // SOLO EL JUGADOR PAGA
    // ========================
    if(owner == 1){
        CosteEdificio cost = costoEdificio(tipoEd);

        if(ctx.recursos.comida < cost.comida ||
           ctx.recursos.metal  < cost.metal  ||
           ctx.recursos.energia< cost.energia)
        {
            ctx.log("Recursos insuficientes para construir " + tipoEd);
            return false;
        }

        // Pagar costo SOLO si owner == 1
        ctx.recursos.comida  -= cost.comida;
        ctx.recursos.metal   -= cost.metal;
        ctx.recursos.energia -= cost.energia;
    }

    // Construcción real (gratis para la IA)
    if(tipoEd == "Granja") {
        c.edificio = std::make_shared<Granja>(owner,pos);
        ctx.log("Construida Granja");
    }
    else if(tipoEd == "Cuartel"){
        c.edificio = std::make_shared<Cuartel>(owner,pos);
        ctx.log("Construido Cuartel");
    }
    else if(tipoEd == "Torre"){
        c.edificio = std::make_shared<Torre>(owner,pos);
        ctx.log("Construida Torre");
    }
    else if(tipoEd == "Forja"){
        c.edificio = std::make_shared<Forja>(owner,pos);
        ctx.log("Construida Forja");
    }
    else {
        ctx.log("Tipo de edificio desconocido");
        return false;
    }

    // Expansión de dominio
    expandirControl(owner, pos);

    return true;
}

void MapaMundo::expandirControl(int owner, const Coordenada& c){
    for(int dx=-1; dx<=1; dx++){
        for(int dy=-1; dy<=1; dy++){
            int nx = c.x + dx;
            int ny = c.y + dy;

            if(nx>=0 && ny>=0 && nx<w && ny<h){
                grid[ny][nx].propietario = owner;
            }
        }
    }
}

bool MapaMundo::estaCercaDeEdificio(int owner, const Coordenada& pos, const std::string& tipoEd){
    for(int y=0; y<h; y++){
        for(int x=0; x<w; x++){
            Celda& ce = grid[y][x];
            if(!ce.edificio) continue;
            if(ce.edificio->owner() != owner) continue;
            if(ce.edificio->tipo() != tipoEd) continue;

            int dx = abs(x - pos.x);
            int dy = abs(y - pos.y);

            //Permitir 8 direcciones alrededor
            if(dx <= 1 && dy <= 1 && !(dx == 0 && dy == 0)){
                return true;
            }
        }
    }
    return false;
}

int MapaMundo::countOwned(int owner) const {
    int cnt=0;
    for(int y=0;y<h;++y) for(int x=0;x<w;++x) if(grid[y][x].propietario==owner) cnt++;
    return cnt;
}