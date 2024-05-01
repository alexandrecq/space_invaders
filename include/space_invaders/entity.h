#pragma once

#include <Eigen/Dense>

#include "Eigen/src/Core/Matrix.h"
#include "space_invaders/interface.h"

using Eigen::Vector2f;
using Eigen::Vector3f;

class Entity {
public:
	Entity(Interface* m_interface);
	virtual ~Entity() = default;
	virtual void update(int ticks) = 0;
	virtual void draw() = 0;

protected:
	Interface *m_interface;
	Vector2f m_position;
	Vector2f m_width_height;
	Vector3f m_color;
private:
};

class Player : public Entity {
public:
	Player(Interface* interface);
	Player();
	void update(int ticks) override;
	void draw() override;
private:
	Vector3f m_color = {0, 0, 1};
	Vector2f m_width_height = {.1, .1};
	Vector2f m_position = {0 - m_width_height.x() / 2, -1 + m_width_height.y()};
	int m_num_lives = 3;
};

class Alien : public Entity {
public:
	Alien(Interface* interface, Vector2f position);
	void update(int ticks) override;
	void draw() override;
private:
	Vector3f m_color = {1, .5, .5};
	Vector2f m_width_height = {.1, .1};
	int m_step_every_ticks = 1000;
	int m_last_step = 0;
	Vector2f m_step_size = {m_width_height.x() * 1.5, 0};
};
