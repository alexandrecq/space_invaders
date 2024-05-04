#pragma once

#include "memory"

#include <Eigen/Dense>

using Eigen::Array2f;
using Eigen::Array3f;

class Interface;

class Entity {
public:
	Entity() = default;
	// virtual ~Entity() = default;
	Entity(Array2f widthHeight, Array2f position, Array3f color);
	virtual void update(int ticks) = 0;
	void draw(Interface* interface) const;

	void setPosition(Array2f position) { m_position = position; }
	Array2f getPosition() const { return m_position; }
	Array2f getWidthHeight() const { return m_widthHeight; }
	void setActive(bool active) { m_active = active; }
	bool isActive() const { return m_active; }

protected:
	const Array2f m_widthHeight{.01, .01};
	Array2f m_position;
	Array3f m_color;
private:
	bool m_active = true;
};


class Projectile : public Entity {
public:
	Projectile(Array2f widthHeight, Array2f position, Array3f color);
	void update(int ticks) override;
};

class CanFire {
public:
	CanFire(bool firesUp);
	std::shared_ptr<Projectile> getProjectile();
	void fire(Array2f sourcePosition, Array2f sourceWidthHeight);
protected:
	bool m_firesUp;
	std::shared_ptr<Projectile> m_projectile = nullptr;
};


class Player : public Entity, public CanFire {
public:
	Player(Array2f widthHeight, Array2f position, Array3f color);
	void update(int ticks) override;
	void takeStep(bool toTheRight);
private:
	int m_num_lives = 3;
};

class Alien : public Entity {
public:
	Alien(Array2f widthHeight, Array2f position, Array3f color,
		Array2f stepSize, int numStepsTilReverse);
	void update(int ticks) override;
private:
	const int m_stepEveryTicks = 1000;
	const int m_numStepsTilReverse = 3;
	int m_lastStepTick = 0;
	int m_stepsTaken = 0;
	Array2f m_stepSize{.1, -.1};
};

class Barrier : public Entity {
public:
	Barrier(Array2f widthHeight, Array2f position, Array3f color);
	void update(int ticks) override;
private:
};
