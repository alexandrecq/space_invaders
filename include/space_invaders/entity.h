#pragma once

#include <random>
#include <memory>

#include <Eigen/Dense>
#include <vector>

using Eigen::Array2f;
using Eigen::Array3f;
using std::vector;
using std::shared_ptr;

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
	Projectile(Array2f widthHeight, Array2f position, Array3f color, float m_vertStepSize);
	void update(int ticks) override;
	void setTargets(vector<shared_ptr<Entity>> targets) { m_targets = targets; }
private:
	const int m_stepEveryTicks = 1;
	float m_vertStepSize;
	int m_lastStepTick = 0;
	vector<shared_ptr<Entity>> m_targets;
};

class CanFire {
public:
	CanFire(bool firesUp);
	shared_ptr<Projectile> getProjectile() { return m_projectile; }
	void setProjectileTargets(vector<shared_ptr<Entity>> targets) { m_projectile->setTargets(targets); }
	void fire(Array2f sourcePosition, Array2f sourceWidthHeight);
protected:
	bool m_firesUp;
	shared_ptr<Projectile> m_projectile = nullptr;
};


class Player : public Entity, public CanFire {
public:
	Player(Array2f widthHeight, Array2f position, Array3f color);
	void update(int ticks) override;
	void takeStep(bool toTheRight);
private:
	int m_num_lives = 3;
};

class Alien : public Entity , public CanFire {
public:
	Alien(Array2f widthHeight, Array2f position, Array3f color,
		Array2f stepSize, int numStepsTilReverse);
	void update(int ticks) override;
private:
	const int m_stepEveryTicks = 1000;
	Array2f m_stepSize{.1, -.1};
	int m_lastStepTick = 0;

	const int m_numStepsTilReverse = 3;
	int m_stepsTaken = 0;

	std::mt19937 m_gen;
	std::uniform_int_distribution<int> m_distribution;
	const int m_fireProbability = 1;
};

class Barrier : public Entity {
public:
	Barrier(Array2f widthHeight, Array2f position, Array3f color);
	void update(int ticks) override;
private:
};
