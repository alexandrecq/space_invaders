#pragma once

#include <array>

#include "space_invaders/aabb.h"
#include "space_invaders/animation.h"
#include "space_invaders/constants.h"

using entityAnimations = std::array<Animation, 2>;
using alienAnimations = std::array<entityAnimations, 3>;
using barrierAnimations = std::array<entityAnimations, 5>;

class Interface;

class Entity {
public:
	Entity(Array2f widthHeight, Array2f position);
	Entity(Array2f widthHeight, Array2f position, entityAnimations animations);
	virtual void update(int ticks) = 0;
	virtual int hit();
	void draw(Interface const *interface) const;
	AABB aabb() const;

	void setPosition(Array2f position) { m_position = position; }
	Array2f getPosition() const { return m_position; }
	Array2f getWidthHeight() const { return m_widthHeight; }
	void setActive(bool active) { m_active = active; }
	bool isActive() const { return m_active; }
	void setDrawMe(bool drawMe) { m_drawMe = drawMe; }
	int getNumLives() { return m_numLives; }
	Animation getDefaultAnimation() { return m_animations[ENTITY_DEFAULT_ANIMATION]; }
protected:
	void updateCurrentAnimation(int ticks);
	virtual void reset();
	virtual int getHitValue() const { return 0; }

	const Array2f m_widthHeight{.01, .01};
	Array2f m_position;
	Array4f m_color{1.f, 1.f, 1.f, 1.f};
	entityAnimations m_animations;
	int m_currentAnimation = ENTITY_DEFAULT_ANIMATION;
	bool m_active = true;
	bool m_drawMe = true;
	int m_numLives = 1;
};


class BarrierTile : public Entity {
public:
	BarrierTile(Array2f widthHeight, Array2f position, entityAnimations animations);
	void update(int ticks) override;
protected:
	int hit() override;
};


class Projectile : public Entity {
public:
	Projectile(Array2f widthHeight, Array2f position, float m_vertStepSize);
	void update(int ticks) override;
	void setTargets(vector<shared_ptr<Entity>> targets) { m_targets = targets; }
	long long const* getTotalHits() const { return &m_totalHits; }
private:
	float m_vertStepSize;
	vector<shared_ptr<Entity>> m_targets;
	long long m_totalHits = 0;
};


class EntityThatFires : public Entity {
public:
	EntityThatFires(Array2f widthHeight, Array2f position, entityAnimations animations, bool firesUp);
	shared_ptr<Projectile> getProjectile() { return m_projectile; }
	void setProjectileTargets(vector<shared_ptr<Entity>> targets) { m_projectile->setTargets(targets); }
	void fire();
protected:
	bool m_firesUp;
	shared_ptr<Projectile> m_projectile = nullptr;
};


class Player : public EntityThatFires {
public:
	Player(Array2f widthHeight, Array2f position, entityAnimations animations);
	void update(int ticks) override;
	void takeStep(bool toTheRight);
	void reset() override;
	long long const* getScore() { return m_projectile->getTotalHits(); }
};


class Alien : public EntityThatFires {
public:
	Alien(const Array2f& widthHeight, const Array2f& position, entityAnimations animations,
		const int& numStepsTilReverse, const int& hitValue,
		const int& stepEveryTicks = ALIEN_STEP_EVERY_TICKS,
		const Array2f& stepSize = ALIEN_STEP_SIZE,
		const float& fireProb = ALIEN_FIRE_PROBABILITY
	);
	void update(int ticks) override;
protected:
	virtual int getHitValue() const override  { return m_hitValue; }

	const int m_stepEveryTicks;
	Array2f m_stepSize;
	int m_lastStepTick = 0;
private:
	const int m_hitValue = 0;
	const int m_numStepsTilReverse;
	int m_stepsTaken = 0;
	RandomNumberGenerator m_rng;
	const float m_fireProbability;
};


class Saucer : public Alien {
public:
	Saucer(entityAnimations animations);
	void update(int ticks) override;
private:
	const float m_appearProbability = SAUCER_APPEAR_PROBABILITY;
	RandomNumberGenerator m_rng;
};
