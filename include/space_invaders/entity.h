#pragma once

#include <Eigen/Dense>

#include "space_invaders/interface.h"

class Entity {
public:
	Entity(Interface* m_interface);
	virtual ~Entity() = default;
	virtual void update(int ticks) = 0;
	virtual void draw() = 0;

protected:
	Interface *m_interface;
private:
	Eigen::Vector2f m_position;
	Eigen::Vector2f m_width_height;
	Eigen::Vector3f m_color;
};

class Player : public Entity {
public:
	Player(Interface* interface);
	Player();
	void update(int ticks) override;
	void draw() override;
private:
	Eigen::Vector3f m_color = {0, 0, 1};
	Eigen::Vector2f m_width_height = {.1, .1};
	Eigen::Vector2f m_position = {0 - m_width_height.x() / 2, -1 + m_width_height.y()};
	int m_num_lives = 3;
};

class Alien : public Entity {
public:
	Alien(Interface* interface);
	void update(int ticks) override;
	void draw() override;
private:
	Eigen::Vector3f m_color = {1, .5, 0};
	Eigen::Vector2f m_width_height = {.1, .1};
	int m_step_every_ticks = 1000;
};
