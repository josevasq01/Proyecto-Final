//
// Created by lvjos on 30/11/2025.
//

#include "Juego.h"
#include <fstream>

int main(){
    Juego j;
    j.run();
    std::ofstream clean("log.txt", std::ios::trunc);
    clean.close();
    return 0;
}
