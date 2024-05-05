
#include "space_invaders/entity.h"
#include "space_invaders/interface.h"


Entity::Entity(Array2f widthHeight, Array2f position, Array3f color) :
    m_position(position), m_widthHeight(widthHeight), m_color(color) {}

void Entity::draw(Interface* interface) const {
    if (!m_active) return;
    // printf("m_position: (%.2f, %.2f)\n", m_position.x(), m_position.y());
    // printf("m_widthHeight: (%.2f, %.2f)\n", m_widthHeight.x(), m_widthHeight.y());
    auto bottomLeft = m_position - m_widthHeight / 2;
    interface->drawRectangle(bottomLeft, m_widthHeight, m_color);
}


Projectile::Projectile(Array2f widthHeight, Array2f position, Array3f color, float vertStepSize) :
    Entity(widthHeight, position, color), m_vertStepSize(vertStepSize) {}

void Projectile::update(int ticks)  {
    if (m_targets.size()) {
        // printf("proj update: %p, %lu\n", m_targets.get(), m_targets->size());
        //check collisions
    }
    if (ticks - m_lastStepTick > m_stepEveryTicks) {
        m_position.y() += m_vertStepSize;
        m_lastStepTick = ticks;
    }
}


CanFire::CanFire(bool firesUp) : m_firesUp(firesUp){
    //set default values for projectile
    Array2f projectileWidthHeight{.01, .05};
    Array2f projectilePosition{0, 0};
    Array3f projectileColor{1., 1., 1.};
    float vertStepSize((firesUp ? +1 : -1) * .01);
    m_projectile = std::make_shared<Projectile>(projectileWidthHeight, projectilePosition, projectileColor, vertStepSize);
    m_projectile->setActive(false);
}

void CanFire::fire(Array2f sourcePosition, Array2f sourceWidthHeight) {
    m_projectile->setActive(true);
    float offset = sourceWidthHeight.y() / 2 + m_projectile->getWidthHeight().y() / 2;
    m_projectile->setPosition({sourcePosition.x(), sourcePosition.y() + (m_firesUp ? +offset : -offset)});
}


Player::Player(Array2f widthHeight, Array2f position, Array3f color) :
    Entity(widthHeight, position, color), CanFire(true) {}

void Player::update(int ticks)  {
    // printf("%p %d\n", &*m_projectile, m_projectile == nullptr);
    // if (m_projectile->isActive) {
        // m_projectile->update(ticks);
    // }
}

void Player::takeStep(bool toTheRight) {
    m_position.x() += toTheRight ? +.1 : -.1;
    m_position.x() = fmin(m_position.x(),  1 - m_widthHeight.x() / 2);
    m_position.x() = fmax(m_position.x(), -1 + m_widthHeight.x() / 2);
}


Alien::Alien(Array2f widthHeight,Array2f position, Array3f color,
             Array2f stepSize, int numStepsTilReverse) :
    Entity(widthHeight, position, color), CanFire(false),
    m_stepSize(stepSize), m_numStepsTilReverse(numStepsTilReverse)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1000);
    m_gen = gen;
    m_distribution = dist;

}

void Alien::update(int ticks)  {
    const int randomNumber = m_distribution(m_gen);
    if (randomNumber <= m_fireProbability) {
        fire(m_position, m_widthHeight);
    }

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
