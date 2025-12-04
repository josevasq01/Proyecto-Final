//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_PERSISTENCIA_H
#define PROYECTO_FINAL_PERSISTENCIA_H

#include "../core/Contexto.h"
#include <string>

namespace Persistencia {
    bool save(const Contexto& ctx, const std::string& filename);
    bool load(Contexto& ctx, const std::string& filename);
}

#endif //PROYECTO_FINAL_PERSISTENCIA_H