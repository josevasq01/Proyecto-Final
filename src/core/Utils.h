//
// Created by lvjos on 30/11/2025.
//

#ifndef PROYECTO_FINAL_UTILS_H
#define PROYECTO_FINAL_UTILS_H

#include <string>
#include <sstream>

namespace Utils {
    inline std::string to_string_pad(int v, int pad=2){
        std::ostringstream ss;
        ss.width(pad);
        ss << v;
        return ss.str();
    }
}

#endif //PROYECTO_FINAL_UTILS_H