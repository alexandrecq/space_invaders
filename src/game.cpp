#include <array>
#include <thread>

#include "space_invaders/constants.h"
#include "space_invaders/entity.h"
#include "space_invaders/game.h"


Game::Game() {
    m_startTime = std::chrono::steady_clock::now();

    initPlayer();
    initAlienGrid();
    initBarriers();
    initSaucer();

    setPlayerTargets();
    setAlienTargets();

}

void Game::initPlayer() {
    entityAnimations playerAnimations;
    loadPlayerAnimations(playerAnimations);

    m_player = std::make_shared<Player>(PLAYER_WIDTH_HEIGHT, PLAYER_START_POSITION, playerAnimations);
    m_entities.push_back(m_player);
    m_interface.setPlayer(m_player);

    m_entities.push_back(m_player->getProjectile());
}

void Game::initAlienGrid(
    const int numRows, const int numCols, const Array2f alienWidthHeight
) {
    const Array2f gridIncXY = 1.5 * alienWidthHeight;
    const Array2f stepSize = {alienWidthHeight.x() / 2, -alienWidthHeight.y() / 2};
    std::array<entityAnimations, 3> allAlienAnimations;
    loadAlienAnimations(allAlienAnimations);

    const Array2f colsRows{numCols, numRows};
    const Array2f gridOuterDims = (colsRows * alienWidthHeight)
        + (colsRows - 1.f) * (gridIncXY - alienWidthHeight);
    const float extraSpaceX = 2 - gridOuterDims.x();
    const int numStepsTilReverse = extraSpaceX / stepSize.x();
    const Array2f gridBottomLeft{-gridOuterDims.x() / 2, 1 - gridOuterDims.y() - .2};

    entityAnimations *rowAnimation = &allAlienAnimations[0];
    int const *rowHitValue = &ALIEN_A_HIT_VALUE;
    for (int y = 0; y < numRows; y++) {
        if (y > 1 && y < 4) {
            rowAnimation = &allAlienAnimations[1];
            rowHitValue = &ALIEN_B_HIT_VALUE;
        } else if (y == 4) {
            rowAnimation = &allAlienAnimations[2];
            rowHitValue = &ALIEN_C_HIT_VALUE;
        }
        for (int x = 0; x < numCols; x++) {
            Array2f alienBottomLeft = gridBottomLeft + Array2f{x * gridIncXY.x(), y * gridIncXY.y()};
            Array2f alienPosition = alienBottomLeft + alienWidthHeight / 2;
            auto alien = std::make_shared<Alien>(
                alienWidthHeight,
                alienPosition,
                *rowAnimation,
                numStepsTilReverse,
                *rowHitValue
            );
            m_entities.push_back(alien);
            m_entities.push_back(alien->getProjectile());
            m_aliens.push_back(alien);
        }
    }
}

void Game::initSaucer() {
    entityAnimations saucerAnimations{
        Animation(SAUCER_DEFAULT_TEXTURE_PATHS, 0, true),
        Animation(SAUCER_DEATH_TEXTURE_PATHS, GAME_DEATH_ANIMATION_TICKS)
    };
    auto saucer = std::make_shared<Saucer>(saucerAnimations);
    m_entities.push_back(saucer);
    m_aliens.push_back(saucer);
}

void Game::initBarrierTiles(
        const Array2f barrierPosition, barrierAnimations& allBarrierAnimations,
        const int tileRows, const int tileCols, const Array2f tileWidthHeight
) {
    entityAnimations *tileAnimations;
    Array2f barrierBottomLeft = barrierPosition - Array2f{tileCols, tileRows} * tileWidthHeight / 2;
    Array2f tileBottomLeft, tilePosition;
    for (int y = 0; y < tileRows; y++) {
        for (int x = 0; x < tileCols; x++) {
            tileBottomLeft = barrierBottomLeft + Array2f{x, y} * tileWidthHeight;
            tilePosition = tileBottomLeft + tileWidthHeight / 2;

            if ((x == 0) && (y == tileRows - 1)) {
                tileAnimations = &allBarrierAnimations[BARRIER_BOTTOM_RIGHT];
            } else if ((x == tileCols - 1) && (y == tileRows - 1)) {
                tileAnimations = &allBarrierAnimations[BARRIER_BOTTOM_LEFT];
            } else if ((x == 1) && (y == 1)) {
                tileAnimations = &allBarrierAnimations[BARRIER_TOP_LEFT];
            } else if ((x == 2) && (y == 1)) {
                tileAnimations = &allBarrierAnimations[BARRIER_TOP_RIGHT];
            } else if ((x > 0) && (x < 3) && (y == 0)) {
                continue;
            } else {
                tileAnimations = &allBarrierAnimations[BARRIER_FULL_TILE];
            }

            auto barrierTile = std::make_shared<BarrierTile>(tileWidthHeight, tilePosition, *tileAnimations);
            m_entities.push_back(barrierTile);
            m_barriers.push_back(barrierTile);
        }
    }
}

void Game::initBarriers(const int numBarriers) {
    const float barrierWidth = BARRIER_TILE_COLS * TILE_WIDTH_HEIGHT.x();
    const float gapWidth = (2.f - float(numBarriers) * barrierWidth) / ((float)numBarriers + 1);
    const float incX = barrierWidth + gapWidth;
    const float firstBarrierX = -1 + gapWidth + barrierWidth / 2.f;

    barrierAnimations allBarrierAnimations;
    loadBarrierAnimations(allBarrierAnimations);
    Array2f barrierPosition = {firstBarrierX, BARRIER_POS_Y};
    for (int idx = 0; idx < numBarriers; idx++) {
        initBarrierTiles(barrierPosition, allBarrierAnimations);
        barrierPosition.x() += incX;
    }
}

void Game::setPlayerTargets() {
    vector<shared_ptr<Entity>> playerTargets;
    std::copy(m_aliens.begin(), m_aliens.end(), std::back_inserter(playerTargets));
    std::copy(m_barriers.begin(), m_barriers.end(), std::back_inserter(playerTargets));
    m_player->setProjectileTargets(playerTargets);
}

void Game::setAlienTargets() {
    vector<shared_ptr<Entity>> alienTargets{m_player};
    std::copy(m_barriers.begin(), m_barriers.end(), std::back_inserter(alienTargets));
    for (auto alien : m_aliens) {
        alien->setProjectileTargets(alienTargets);
    }
}

void Game::run() {
    while (m_interface.isAlive()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(m_tickMS));
        auto elapsed = std::chrono::steady_clock::now() - m_startTime;
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
        // printf("Elapsed: %.2ld\n", elapsed_ms.count());

        m_interface.startFrame();
        for (auto& entity : m_entities) {
            if (entity) {
                entity->update(elapsed_ms.count());
                entity->draw(&m_interface);
            }
        }
        m_interface.renderFrame();
    }
}

void Game::loadPlayerAnimations(entityAnimations& playerAnimations) {
    playerAnimations[0] = Animation(PLAYER_DEFAULT_TEXTURE_PATHS, 0, true);
    playerAnimations[1] = Animation(PLAYER_DEATH_TEXTURE_PATHS, GAME_DEATH_ANIMATION_TICKS);
}

void Game::loadAlienAnimations(std::array<entityAnimations, 3>& alienAnimations) {
    alienAnimations[0][0] = Animation(ALIEN_A_DEFAULT_TEXTURE_PATHS, ALIEN_STEP_EVERY_TICKS, true);
    alienAnimations[1][0] = Animation(ALIEN_B_DEFAULT_TEXTURE_PATHS, ALIEN_STEP_EVERY_TICKS, true);
    alienAnimations[2][0] = Animation(ALIEN_C_DEFAULT_TEXTURE_PATHS, ALIEN_STEP_EVERY_TICKS, true);
    alienAnimations[0][1] = Animation(ALIEN_A_DEATH_TEXTURE_PATHS, GAME_DEATH_ANIMATION_TICKS);
    alienAnimations[1][1] = Animation(ALIEN_B_DEATH_TEXTURE_PATHS, GAME_DEATH_ANIMATION_TICKS);
    alienAnimations[2][1] = Animation(ALIEN_C_DEATH_TEXTURE_PATHS, GAME_DEATH_ANIMATION_TICKS);
}

void Game::loadBarrierAnimations(barrierAnimations& barrierAnimations) {
    barrierAnimations[0] = {Animation(BARRIER_A_DEFAULT_TEXTURE_PATHS), Animation()};
    barrierAnimations[1] = {Animation(BARRIER_B_DEFAULT_TEXTURE_PATHS), Animation()};
    barrierAnimations[2] = {Animation(BARRIER_C_DEFAULT_TEXTURE_PATHS), Animation()};
    barrierAnimations[3] = {Animation(BARRIER_D_DEFAULT_TEXTURE_PATHS), Animation()};
    barrierAnimations[4] = {Animation(BARRIER_E_DEFAULT_TEXTURE_PATHS), Animation()};
}
