//
// Created by lvjos on 30/11/2025.
//

#include "Unidad.h"

Unidad::Unidad(int owner, const Coordenada& pos) : m_owner(owner), m_pos(pos), m_hp(1) {}
int Unidad::owner() const { return m_owner; }
Coordenada Unidad::posicion() const { return m_pos; }
void Unidad::set_posicion(const Coordenada& p){ m_pos = p; }
int Unidad::hp() const { return m_hp; }
void Unidad::damage(int d){ m_hp -= d; }