#include "space_invaders/constants.h"
#include "space_invaders/entity.h"
#include "space_invaders/game.h"

#include <iterator>
#include <thread>


Game::Game() {
    m_start_time = std::chrono::steady_clock::now();

    initPlayer();
    initAlienGrid();
    initBarriers();

    setPlayerTargets();
    setAlienTargets();

}

void Game::initPlayer() {
    const Animation playerAnimation(PLAYER_TEXTURE_PATHS);

    m_player = std::make_shared<Player>(PLAYER_WIDTH_HEIGHT, PLAYER_START_POSITION, PLAYER_COLOR);
    m_player->setAnimation(playerAnimation);
    m_entities.push_back(m_player);
    m_interface.setPlayer(m_player);

    m_entities.push_back(m_player->getProjectile());
}

void Game::initAlienGrid(const int numRows, const int numCols) {
    const Array2f gridIncXY = 1.5 * ALIEN_WIDTH_HEIGHT;
    const Array2f stepSize = {ALIEN_WIDTH_HEIGHT.x() / 2, -ALIEN_WIDTH_HEIGHT.y() / 2};
    const Animation alienAnimationA(ALIEN_A_TEXTURE_PATHS);
    const Animation alienAnimationB(ALIEN_B_TEXTURE_PATHS);
    const Animation alienAnimationC(ALIEN_C_TEXTURE_PATHS);

    const Array2f colsRows{numCols, numRows};
    const Array2f gridOuterDims = (colsRows * ALIEN_WIDTH_HEIGHT)
        + (colsRows - 1.f) * (gridIncXY - ALIEN_WIDTH_HEIGHT);
    const float extraSpaceX = 2 - gridOuterDims.x();
    const int numStepsTilReverse = extraSpaceX / stepSize.x();
    const Array2f gridBottomLeft{-1, 1 - gridOuterDims.y()};

    Animation rowAnimation = alienAnimationA;
    for (int y = 0; y < numRows; y++) {
        if (y > 1 && y < 4) rowAnimation = alienAnimationB;
        else if (y == 4) rowAnimation = alienAnimationC;
        for (int x = 0; x < numCols; x++) {
            Array2f alienBottomLeft = gridBottomLeft + Array2f{x * gridIncXY.x(), y * gridIncXY.y()};
            Array2f alienPosition = alienBottomLeft + ALIEN_WIDTH_HEIGHT / 2;
            auto alien = std::make_shared<Alien>(
                ALIEN_WIDTH_HEIGHT,
                alienPosition,
                ALIEN_COLOR,
                numStepsTilReverse
            );
            alien->setAnimation(rowAnimation);
            m_entities.push_back(alien);
            m_entities.push_back(alien->getProjectile());
            m_aliens.push_back(alien);
        }
    }
}

void Game::initBarriers(const int numBarriers) {
    const Array2f barrierWidthHeight{.2, .2};
    const Array3f barrierColor = {.5, .4, .1};
    const float incX = 2 / ((float)numBarriers + 1);
    const float midlineY = -.7;

    for (int idx = 0; idx < numBarriers; idx++) {
        const Array2f barrierPosition{-1 + (idx + 1) * incX, midlineY};
        auto barrier = std::make_shared<Barrier>(barrierWidthHeight, barrierPosition, barrierColor);
        m_entities.push_back(barrier);
        m_barriers.push_back(barrier);
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
        std::this_thread::sleep_for(std::chrono::milliseconds(m_tick_ms));
        auto elapsed = std::chrono::steady_clock::now() - m_start_time;
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
        // printf("Elapsed: %.2ld\n", elapsed_ms.count());

        m_interface.startFrame();
        for (auto entity : m_entities) {
            if (entity) {
                entity->update(elapsed_ms.count());
                entity->draw(&m_interface);


            }
        }
        m_interface.renderFrame();
    }
}
