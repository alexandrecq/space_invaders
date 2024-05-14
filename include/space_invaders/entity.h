#pragma once

#include <random>
#include <memory>
#include <vector>

#include "space_invaders/aabb.h"
#include "space_invaders/animation.h"
#include "space_invaders/constants.h"

using std::vector;
using std::shared_ptr;

class Interface;

class Entity {
public:
	// Entity() = default;
	// virtual ~Entity() = default;
	Entity(Array2f widthHeight, Array2f position, Array3f color);
	Entity(Array2f widthHeight, Array2f position, Array3f color, Animation animation);
	virtual void update(int ticks) = 0;
	void draw(Interface const *interface) const;
	AABB aabb() const;
	void hit();

	void setPosition(Array2f position) { m_position = position; }
	Array2f getPosition() const { return m_position; }
	Array2f getWidthHeight() const { return m_widthHeight; }
	void setActive(bool active) { m_active = active; }
	bool isActive() const { return m_active; }
	void setDrawMe(bool drawMe) { m_drawMe = drawMe; }
	void setDefaultAnimation(const Animation& animation) {
		m_defaultAnimation = animation;
		m_currentAnimation = m_defaultAnimation;
	}
	void setDeathAnimation(const Animation& animation) { m_deathAnimation = animation; }
	void updateCurrentAnimation(int ticks);
protected:
	const Array2f m_widthHeight{.01, .01};
	Array2f m_position;
	Array3f m_color;
	Animation m_defaultAnimation;
	Animation m_deathAnimation;
	Animation m_currentAnimation;
	bool m_active = true;
	bool m_drawMe = true;
	int m_numLives = 1;
};


class Barrier : public Entity {
public:
	Barrier(Array2f widthHeight, Array2f position, Array3f color);
	void update(int ticks) override;
};


class Projectile : public Entity {
public:
	Projectile(Array2f widthHeight, Array2f position, Array3f color, float m_vertStepSize);
	void update(int ticks) override;
	void setTargets(vector<shared_ptr<Entity>> targets) { m_targets = targets; }
private:
	float m_vertStepSize;
	vector<shared_ptr<Entity>> m_targets;
};


class EntityThatFires : public Entity {
public:
	EntityThatFires(Array2f widthHeight, Array2f position, Array3f color, bool firesUp);
	shared_ptr<Projectile> getProjectile() { return m_projectile; }
	void setProjectileTargets(vector<shared_ptr<Entity>> targets) { m_projectile->setTargets(targets); }
	void fire();
protected:
	bool m_firesUp;
	shared_ptr<Projectile> m_projectile = nullptr;
};


class Player : public EntityThatFires {
public:
	Player(Array2f widthHeight, Array2f position, Array3f color);
	void update(int ticks) override;
	void takeStep(bool toTheRight);
};


class Alien : public EntityThatFires {
public:
	Alien(Array2f widthHeight, Array2f position, Array3f color,
		int numStepsTilReverse);
	void update(int ticks) override;
private:
	const int m_stepEveryTicks = ALIEN_STEP_EVERY_TICKS;
	Array2f m_stepSize = ALIEN_STEP_SIZE;
	const int m_numStepsTilReverse;
	int m_lastStepTick = 0;
	int m_stepsTaken = 0;

	std::mt19937 m_gen;
	std::uniform_int_distribution<int> m_distribution;
	const int m_fireProbability = ALIEN_FIRE_PROBABILITY_X100K;
};

