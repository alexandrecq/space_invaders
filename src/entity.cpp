#include "space_invaders/entity.h"
#include "space_invaders/interface.h"

Entity::Entity(Interface* interface) : m_interface(interface) {}

Player::Player(Interface* interface) : Entity(interface) {
}

void Player::update(int ticks)  {
}

void Player::draw()  {
    m_interface->drawRectangle(m_position, m_width_height, m_color);
}
