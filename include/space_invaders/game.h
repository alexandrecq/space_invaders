#pragma once

#include <chrono>

#include "space_invaders/constants.h"
#include "space_invaders/entity.h"
#include "space_invaders/interface.h"


class Game {
public:
	Game();
	void run();
private:
	void initPlayer();
	void initAlienGrid(
		const int numRows = GAME_NUM_ALIEN_ROWS, const int numCols = GAME_NUM_ALIEN_COLS,
		const Array2f alienWidthHeight = ALIEN_WIDTH_HEIGHT, const Array3f alienColor = ALIEN_COLOR
	);
	void initSaucer();
	void initBarrierTiles(
		const Array2f barrierPosition, barrierAnimations& allAnimations,
		const int tileRows = BARRIER_TILE_ROWS, const int tileCols = BARRIER_TILE_COLS,
		const Array2f tileWidthHeight = TILE_WIDTH_HEIGHT, const Array3f tileColor = TILE_COLOR
	);
	void initBarriers(const int numBarriers = GAME_NUM_BARRIERS);
	void setPlayerTargets();
	void setAlienTargets();
	void loadPlayerAnimations(entityAnimations& playerAnimations);
	void loadAlienAnimations(std::array<entityAnimations, 3>& alienAnimations);
	void loadBarrierAnimations(barrierAnimations& barrierAnimations);

	Interface m_interface;
	// static constexpr int m_max_entities = 10;
	// shared_ptr<Entity> m_entities[m_num_entities];
	shared_ptr<Player> m_player;
	vector<shared_ptr<Entity>> m_entities;
	vector<shared_ptr<Alien>> m_aliens;
	vector<shared_ptr<Entity>> m_barriers;

	std::chrono::time_point<std::chrono::steady_clock> m_startTime;
	const long long m_tickMS = GAME_TICK_MS;
};
