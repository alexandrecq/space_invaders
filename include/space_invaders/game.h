#pragma once

#include <chrono>

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
	void setAlienTargets();
private:
	Interface m_interface;
	
	// static constexpr int m_max_entities = 10;
	// shared_ptr<Entity> m_entities[m_num_entities];
	shared_ptr<Player> m_player;
	vector<shared_ptr<Entity>> m_entities;
	vector<shared_ptr<Alien>> m_aliens;
	vector<shared_ptr<Entity>> m_barriers;

	std::chrono::time_point<std::chrono::steady_clock> m_start_time;
	const long long m_tick_ms = 10;
};
