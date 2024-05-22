#include "space_invaders/entity.h"
#include "space_invaders/interface.h"


static const AABB gameAABB({-1, -1}, {1, 1});

Entity::Entity(Array2f widthHeight, Array2f position, Array3f color) :
    m_position(position), m_widthHeight(widthHeight), m_color(color) {}

Entity::Entity(Array2f widthHeight, Array2f position, Array3f color, entityAnimations animations) :
    m_position(position), m_widthHeight(widthHeight), m_color(color), m_animations(animations) {}

void Entity::draw(Interface const *interface) const {
    if (!m_drawMe) return;
    auto bottomLeft = m_position - m_widthHeight / 2;

    if (m_animations[m_currentAnimation].isEmpty()) {
        interface->drawRectangle(bottomLeft, m_widthHeight, m_color);
    } else {
        interface->drawTexture(m_animations[m_currentAnimation].getCurrentTexture(), bottomLeft, m_widthHeight);
    }
}

AABB Entity::aabb() const {
    return AABB(m_position - m_widthHeight / 2, m_position + m_widthHeight / 2);
}

void Entity::hit() {
    if (!m_active) return;  // can't be hit while dying or dead
    m_numLives--;
    if (!m_animations[ENTITY_DEATH_ANIMATION].isEmpty()) {
        m_animations[ENTITY_DEATH_ANIMATION].reset();
        m_currentAnimation = ENTITY_DEATH_ANIMATION;
    }
    m_active = false;
}

void Entity::updateCurrentAnimation(int ticks) {
    if (m_currentAnimation == ENTITY_DEATH_ANIMATION && m_animations[m_currentAnimation].isDone()) {
        m_currentAnimation = ENTITY_DEFAULT_ANIMATION;
        if (m_numLives <= 0) m_drawMe = false;
        else reset();
    }
    else m_animations[m_currentAnimation].updateTexture(ticks);
}

void Entity::reset() {}


BarrierTile::BarrierTile(Array2f widthHeight, Array2f position, Array3f color, entityAnimations animations) :
    Entity(widthHeight, position, color, animations) {
    m_numLives = BARRIER_NUM_LIVES;
}

void BarrierTile::update(int ticks)  {}

void BarrierTile::hit() {
    if (!m_active) return;
    m_numLives--;
    m_animations[m_currentAnimation].updateTexture();
    if (m_animations[m_currentAnimation].isDone() || m_numLives == 0) {
        m_active = false;
        m_drawMe = false;
    }
}


Projectile::Projectile(Array2f widthHeight, Array2f position, Array3f color, float vertStepSize) :
    Entity(widthHeight, position, color), m_vertStepSize(vertStepSize) {
    m_active = false;
    m_drawMe = false;
}

void Projectile::update(int ticks)  {
    if (!m_active) return;
    if (m_targets.size()) {
        //check collision with any targets
        for (auto target : m_targets) {
            if (!target->isActive()) continue;
            if (aabb().intersects(target->aabb())) {
                m_active = false;
                m_drawMe = false;
                target->hit();
            }
        }
    }
    m_position.y() += m_vertStepSize;
    if (!aabb().intersects(gameAABB)) {
        m_active = false;
        m_drawMe = false;
    }
}


EntityThatFires::EntityThatFires(Array2f widthHeight, Array2f position, Array3f color, entityAnimations animations, bool firesUp) :
    Entity(widthHeight, position, color, animations), m_firesUp(firesUp) {
    //set default values for projectile
    const Array2f projectileWidthHeight{.01, .05};
    const Array2f projectilePosition{0, 0};
    const Array3f projectileColor{1., 1., 1.};
    const float vertStepSize((firesUp ? +1 : -1) * .02);
    m_projectile = std::make_shared<Projectile>(projectileWidthHeight, projectilePosition, projectileColor, vertStepSize);
}

void EntityThatFires::fire() {
    if (!m_active || m_projectile->isActive()) return;
    m_projectile->setActive(true);
    m_projectile->setDrawMe(true);
    float offset = m_widthHeight.y() / 2 + m_projectile->getWidthHeight().y() / 2;
    m_projectile->setPosition({m_position.x(), m_position.y() + (m_firesUp ? +offset : -offset)});
}


Player::Player(Array2f widthHeight, Array2f position, Array3f color, entityAnimations animations) :
    EntityThatFires(widthHeight, position, color, animations, true) {
    m_numLives = PLAYER_NUM_LIVES;
}

void Player::update(int ticks)  {
    updateCurrentAnimation(ticks);
}

void Player::takeStep(bool toTheRight) {
    if (!m_active) return;
    m_position.x() += toTheRight ? PLAYER_STEP_SIZE : -PLAYER_STEP_SIZE;
    m_position.x() = fmin(m_position.x(),  1 - m_widthHeight.x() / 2);
    m_position.x() = fmax(m_position.x(), -1 + m_widthHeight.x() / 2);
}

void Player::reset() {
    // std::this_thread::sleep_for(std::chrono::milliseconds(PLAYER_DEATH_SLEEP_MS));
    // m_currentAnimation = ENTITY_DEFAULT_ANIMATION;
    m_position = PLAYER_START_POSITION;
    m_active = true;
}


Alien::Alien(const Array2f& widthHeight, const Array2f& position, const Array3f& color, entityAnimations animations,
             const int& numStepsTilReverse,
             const int& stepEveryTicks, const Array2f& stepSize, const float& fireProb) :
    EntityThatFires(widthHeight, position, color, animations, false),
    m_stepEveryTicks(stepEveryTicks), m_stepSize(stepSize), m_numStepsTilReverse(numStepsTilReverse),
    m_fireProbability(fireProb)
{
    m_stepsTaken = numStepsTilReverse / 2;  //grid starts at center of screen
}

void Alien::update(int ticks)  {
    updateCurrentAnimation(ticks);
    if (!m_active) return;

    float randomNumber;
    m_rng.generateNumber(&randomNumber);
    if (randomNumber <= m_fireProbability) fire();

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


Saucer::Saucer(entityAnimations animations) :
    Alien(SAUCER_WIDTH_HEIGHT, {0, 0}, Array3f{1, 1, 1}, animations,
          0, SAUCER_STEP_EVERY_TICKS, SAUCER_STEP_SIZE, 0)
{
    m_active = false;
    m_drawMe = false;
}

void Saucer::update(int ticks)
{
    updateCurrentAnimation(ticks);
    if (m_currentAnimation == ENTITY_DEATH_ANIMATION && !m_animations[m_currentAnimation].isDone()) {
        return;
    }
    float randomNumber;
    m_rng.generateNumber(&randomNumber);
    if (!m_active && randomNumber <= m_appearProbability) {
        m_active = true;
        m_drawMe = true;
        m_numLives = 1;
        m_position = {-1 - m_widthHeight.x() / 2, SAUCER_POS_Y};
    }
    if (!m_active) return;

    if (ticks - m_lastStepTick > m_stepEveryTicks) {
        m_position.x() += m_stepSize.x();
        m_lastStepTick = ticks;
    }
    if (!aabb().intersects(gameAABB)) {
        m_active = false;
        m_drawMe = false;
    }
}
