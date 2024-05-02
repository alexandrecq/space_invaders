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

Alien::Alien(Interface* interface) : Entity(interface)  {
    m_position = {-1, 1};
}

Alien::Alien(Interface* interface, Vector2f position, Vector2f width_height,
             Vector2f step_size, int num_steps_til_reverse) : Entity(interface)  {
    m_position = position;
    m_width_height = width_height;
    m_step_size = step_size;
    m_num_steps_til_reverse = num_steps_til_reverse;
    // printf("ticks: %d, step_size: (%.2f, %.2f)\n", ticks, m_step_size.x(), m_step_size.y());
}

void Alien::update(int ticks)  {
    // printf("ticks: %d, pos: (%.2f, %.2f)\n", ticks, m_position.x(), m_position.y());
    if (ticks - m_last_step_tick > m_step_every_ticks) {
        if (m_steps_taken == m_num_steps_til_reverse) {
            m_position.y() += m_step_size.y();
            m_step_size.x() *= -1;
            m_steps_taken = 0;
        } else {
            m_position.x() += m_step_size.x();
            m_steps_taken += 1;
        }
        m_last_step_tick = ticks;
    }
}

void Alien::draw()  {
    m_interface->drawRectangle(m_position, m_width_height, m_color);
}
