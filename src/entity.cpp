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

Alien::Alien(Interface* interface, Vector2f position) : Entity(interface)  {
    m_position = position;
}

void Alien::update(int ticks)  {
    // printf("ticks: %d, pos: (%.2f, %.2f)\n", ticks, m_position.x(), m_position.y());
    if (ticks - m_last_step > m_step_every_ticks) {
        m_position += m_step_size;
        m_last_step = ticks;
    }
}

void Alien::draw()  {
    m_interface->drawRectangle(m_position, m_width_height, m_color);
}
