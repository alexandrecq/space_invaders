#pragma once

#include <chrono>
#include <thread>
#include <memory>
#include <vector>

#include "space_invaders/entity.h"
#include "space_invaders/interface.h"


class Game {
public:
	Game();
	void run();
	void initAlienGrid(int num_rows = 5, int num_cols = 10);
private:
	Interface m_interface;
	// static constexpr int m_max_entities = 10;
	// std::shared_ptr<Entity> m_entities[m_num_entities];
	std::vector<std::shared_ptr<Entity>> m_entities;

	std::chrono::time_point<std::chrono::steady_clock> m_start_time;
	const long long m_tick_ms = 100;
};
