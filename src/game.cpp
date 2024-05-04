#include "space_invaders/game.h"
#include "space_invaders/entity.h"

Game::Game() {
    m_interface = std::make_shared<Interface>();
    m_start_time = std::chrono::steady_clock::now();

    initPlayer();
    initAlienGrid();
    initBarriers();

}

void Game::initPlayer() {
    const Array2f playerWidthHeight{.1, .1};
    const Array2f playerStartPosition{0, -1 + playerWidthHeight.y() / 2};
    const Array3f playerColor{0., .1, 1.};

    auto player = std::make_shared<Player>(playerWidthHeight, playerStartPosition, playerColor);
    m_entities.push_back(player);
    m_interface->setPlayer(player);

    m_entities.push_back(player->getProjectile());
}

void Game::initAlienGrid(const int numRows, const int numCols) {
    const Array2f alienWidthHeight{.1, .1};
    const Array3f alienColor = {.5, 0, .5};
    const Array2f gridIncXY = 1.5 * alienWidthHeight;
    const Array2f stepSize = {alienWidthHeight.x() / 2, -alienWidthHeight.y() / 2};

    const Array2f colsRows{numCols, numRows};
    const Array2f gridOuterDims = (colsRows * alienWidthHeight)
        + (colsRows - 1.f) * (gridIncXY - alienWidthHeight);
    const float extraSpaceX = 2 - gridOuterDims.x();
    const int numStepsTilReverse = extraSpaceX / stepSize.x();
    const Array2f gridBottomLeft{-1, 1 - gridOuterDims.y()};

    for (int x = 0; x < numCols; x++) {
        for (int y = 0; y < numRows; y++) {
            Array2f alienBottomLeft = gridBottomLeft + Array2f{x * gridIncXY.x(), y * gridIncXY.y()};
            Array2f alienPosition = alienBottomLeft + alienWidthHeight / 2;
            auto alien = std::make_shared<Alien>(
                alienWidthHeight,
                alienPosition,
                alienColor,
                stepSize,
                numStepsTilReverse
            );
            m_entities.push_back(alien);
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
        m_entities.push_back(std::make_shared<Barrier>(barrierWidthHeight, barrierPosition, barrierColor));
    }
}

void Game::run() {
    while (m_interface->isAlive()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(m_tick_ms));
        auto elapsed = std::chrono::steady_clock::now() - m_start_time;
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
        // printf("Elapsed: %.2ld\n", elapsed_ms.count());

        m_interface->startFrame();
        for (auto entity : m_entities) {
            if (entity) {
                entity->update(elapsed_ms.count());
                entity->draw(m_interface.get());
            }
        }
        m_interface->renderFrame();
    }
}
