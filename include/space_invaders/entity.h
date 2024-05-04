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

protected:
	const Array2f m_widthHeight{.01, .01};
	Array2f m_position;
	Array3f m_color;
private:
};


class Player : public Entity {
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
