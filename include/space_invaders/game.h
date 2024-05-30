#pragma once

#include <chrono>

#include "space_invaders/constants.h"
#include "space_invaders/entity.h"

class Interface;

class Game {
public:
	Game();
	void run();
	void togglePause() { m_paused = !m_paused; }
	void reset();
private:
	void initPlayer();
	void initAlienGrid(
		const int numRows = GAME_NUM_ALIEN_ROWS, const int numCols = GAME_NUM_ALIEN_COLS,
		const Array2f alienWidthHeight = ALIEN_WIDTH_HEIGHT
	);
	void initSaucer();
	void initBarrierTiles(
		const Array2f barrierPosition, barrierAnimations& allAnimations,
		const int tileRows = BARRIER_TILE_ROWS, const int tileCols = BARRIER_TILE_COLS,
		const Array2f tileWidthHeight = TILE_WIDTH_HEIGHT
	);
	void initBarriers(const int numBarriers = GAME_NUM_BARRIERS);
	void setPlayerTargets();
	void setAlienTargets();

	shared_ptr<Interface> m_interface;
	shared_ptr<Player> m_player;
	vector<shared_ptr<Entity>> m_entities;
	vector<shared_ptr<Alien>> m_aliens;
	vector<shared_ptr<Entity>> m_barriers;
	bool m_paused = false;
	bool m_gameOver = false;

	std::chrono::time_point<std::chrono::steady_clock> m_startTime;
	const int m_tickMS = GAME_TICK_MS;
};

entityAnimations& loadPlayerAnimations();
entityAnimations& loadSaucerAnimations();
alienAnimations& loadAlienAnimations();
barrierAnimations& loadBarrierAnimations();
