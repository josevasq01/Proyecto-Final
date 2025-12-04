//
// Created by lvjos on 30/11/2025.
//

#include "RenderMapa.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// prints a compact map like the PDF example:
// header row with column indices, then each row prefixed by row index and cells [ AAA/TT ]
void RendererMapa::renderCompact(const MapaMundo& mapa){
    int w = mapa.width();
    int h = mapa.height();

    // Top summary line (compact) could be printed by caller (Consola)
    // Print column indices with spacing
    std::cout << "    ";
    for(int x=0;x<w;++x){
        std::cout << std::setw(6) << x << "   ";
    }
    std::cout << "\n";

    // For each row
    for(int y=0;y<h;++y){
        // row index
        std::cout << std::setw(3) << y << " ";
        for(int x=0;x<w;++x){
            std::string code = mapa.cellCompact(x,y); // e.g. "J1S/LL"
            // print inside brackets with fixed width
            std::ostringstream cell;
            cell << "[" << std::setw(5) << std::left << code << "]";
            std::cout << cell.str();
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}