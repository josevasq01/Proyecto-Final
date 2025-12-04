//
// Created by lvjos on 30/11/2025.
//

#include "Edificio.h"
Edificio::Edificio(int owner, const Coordenada& pos): m_owner(owner), m_pos(pos){}
int Edificio::owner() const { return m_owner; }
Coordenada Edificio::posicion() const { return m_pos; }