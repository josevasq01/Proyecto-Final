//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_JUEGO_H
#define PROYECTO_FINAL_JUEGO_H

#include "Contexto.h"

class Juego {
public:
    Contexto ctx;
    Juego();
    void run();
private:
    void faseJugador();
    void faseMundo();
    void evaluarCondiciones();
    void turnoInicio();
    void turnoFin();
};

#endif //PROYECTO_FINAL_JUEGO_H