//
// Created by lvjos on 30/11/2025.
//

#include "Persistencia.h"
#include <fstream>

bool Persistencia::save(const Contexto& ctx, const std::string& filename){
    std::ofstream ofs(filename);
    if(!ofs) return false;
    ofs << ctx.turno << "\n";
    ofs << ctx.recursos.comida << " " << ctx.recursos.metal << " " << ctx.recursos.energia << "\n";
    return true;
}

bool Persistencia::load(Contexto& ctx, const std::string& filename){
    std::ifstream ifs(filename);
    if(!ifs) return false;
    ifs >> ctx.turno;
    ifs >> ctx.recursos.comida >> ctx.recursos.metal >> ctx.recursos.energia;
    return true;
}