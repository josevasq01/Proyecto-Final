//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_TERRENO_H
#define PROYECTO_FINAL_TERRENO_H

#include <string>
class Unidad;

class Terreno {
public:
    virtual ~Terreno() = default;
    virtual int costo_movimiento(const Unidad& u) const = 0;
    virtual int bono_defensa(const Unidad& u) const = 0;
    virtual std::string codigo() const = 0;
};

enum class TipoTerreno {
    LLANURA,
    BOSQUE,
    MONTANIA,
    AGUA,
    PANTANO
};

#endif //PROYECTO_FINAL_TERRENO_H