//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_IA_GUARDIAN_H
#define PROYECTO_FINAL_IA_GUARDIAN_H

#include "core/Contexto.h"

namespace IA_Guardian {
    void step(Contexto& ctx);
    bool tryBuild(Contexto& ctx, const std::string& tipoEd);
}

#endif //PROYECTO_FINAL_IA_GUARDIAN_H