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
	void initPlayer();
	void initAlienGrid(const int num_rows = 5, const int num_cols = 10);
	void initBarriers(const int numBarriers = 4);
	void setPlayerTargets();
private:
	std::shared_ptr<Interface> m_interface = nullptr;
	
	// static constexpr int m_max_entities = 10;
	// std::shared_ptr<Entity> m_entities[m_num_entities];
	std::shared_ptr<Player> m_player;
	std::vector<std::shared_ptr<Entity>> m_entities;
	std::vector<std::shared_ptr<Entity>> m_aliens;
	std::vector<std::shared_ptr<Entity>> m_barriers;

	std::chrono::time_point<std::chrono::steady_clock> m_start_time;
	const long long m_tick_ms = 10;
};
