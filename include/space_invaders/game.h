#pragma once

#include <chrono>

#include "space_invaders/constants.h"
#include "space_invaders/entity.h"
#include "space_invaders/interface.h"


class Game {
public:
	Game();
	void run();
	void initPlayer();
	void initAlienGrid(const int numRows = GAME_NUM_ALIEN_ROWS, const int numCols = GAME_NUM_ALIEN_COLS);
	void initBarriers(const int numBarriers = GAME_NUM_BARRIERS);
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
	const long long m_tick_ms = GAME_TICK_MS;
};
