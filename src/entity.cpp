#include "space_invaders/entity.h"
#include "space_invaders/interface.h"
#include <memory>


Entity::Entity(Array2f widthHeight, Array2f position, Array3f color) :
    m_position(position), m_widthHeight(widthHeight), m_color(color) {}

void Entity::draw(Interface* interface) const {
    // printf("m_position: (%.2f, %.2f)\n", m_position.x(), m_position.y());
    // printf("m_widthHeight: (%.2f, %.2f)\n", m_widthHeight.x(), m_widthHeight.y());
    auto bottomLeft = m_position - m_widthHeight / 2;
    interface->drawRectangle(bottomLeft, m_widthHeight, m_color);
}

Array2f Entity::getPosition() const {
    return m_position;
}

void Entity::setPosition(Array2f position) {
    m_position = position;
}

float Entity::getHeight() {
    return m_widthHeight.y();
}


Projectile::Projectile(Array2f widthHeight, Array2f position, Array3f color) :
    Entity(widthHeight, position, color) {}

void Projectile::update(int ticks)  {}

CanFire::CanFire(bool firesUp) : m_firesUp(firesUp){
    //set default values for projectile
    Array2f widthHeight{.01, .1};
    Array2f position{0, 0};
    Array3f color{1., 1., 1.};
    m_projectile = std::make_unique<Projectile>(widthHeight, position, color);
    // printf("CanFire constructor: %p, %d\n", &*m_projectile, m_projectile == nullptr);
}

void CanFire::fire(Array2f startPosition) {
    m_projectile->isActive = true;
    float offset = m_projectile->getHeight() / 2;
    m_projectile->setPosition(startPosition + (m_firesUp ? +offset : -offset));
}

Player::Player(Array2f widthHeight, Array2f position, Array3f color) :
    Entity(widthHeight, position, color), CanFire(true) {}

void Player::update(int ticks)  {
    // printf("%p %d\n", &*m_projectile, m_projectile == nullptr);
    if (m_projectile->isActive) {
        // m_projectile->update(ticks);
    }
}

void Player::takeStep(bool toTheRight) {
    m_position.x() += toTheRight ? +.1 : -.1;
    m_position.x() = fmin(m_position.x(),  1 - m_widthHeight.x() / 2);
    m_position.x() = fmax(m_position.x(), -1 + m_widthHeight.x() / 2);
}


Alien::Alien(Array2f widthHeight,Array2f position, Array3f color,
             Array2f stepSize, int numStepsTilReverse) :
    Entity(widthHeight, position, color),
    m_stepSize(stepSize), m_numStepsTilReverse(numStepsTilReverse)
{}

void Alien::update(int ticks)  {
    // printf("ticks: %d, pos: (%.2f, %.2f)\n", ticks, m_position.x(), m_position.y());
    if (ticks - m_lastStepTick > m_stepEveryTicks) {
        if (m_stepsTaken == m_numStepsTilReverse) {
            m_position.y() += m_stepSize.y();
            m_stepSize.x() *= -1;
            m_stepsTaken = 0;
        } else {
            m_position.x() += m_stepSize.x();
            m_stepsTaken += 1;
        }
        m_lastStepTick = ticks;
    }
}

Barrier::Barrier(Array2f widthHeight, Array2f position, Array3f color) :
    Entity(widthHeight, position, color) {}
void Barrier::update(int ticks)  {}
