//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_COORDENADAS_H
#define PROYECTO_FINAL_COORDENADAS_H

struct Coordenada {
    int x;
    int y;
    Coordenada(int x_=0, int y_=0) : x(x_), y(y_) {}
    bool operator==(const Coordenada& o) const { return x==o.x && y==o.y; }
};

#endif //PROYECTO_FINAL_COORDENADAS_H