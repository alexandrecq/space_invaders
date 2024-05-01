#pragma once

#include <chrono>
#include <thread>
#include <memory>

#include "space_invaders/entity.h"
#include "space_invaders/interface.h"


class Game {
public:
	Game();
	void run();
private:
	Interface m_interface;
	static constexpr int m_num_entities = 1;
	std::shared_ptr<Entity> m_entities[m_num_entities];

	std::chrono::time_point<std::chrono::steady_clock> m_start_time;
	float m_tick_ms;
};
