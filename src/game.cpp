#include <array>
#include <memory>
#include <thread>

#include "space_invaders/constants.h"
#include "space_invaders/entity.h"
#include "space_invaders/game.h"
#include "space_invaders/interface.h"


Game::Game() {
    reset();
}

void Game::reset() {
    m_startTime = std::chrono::steady_clock::now();
    m_paused = false;
    m_gameOver = false;

    m_player = nullptr;
    m_entities.clear();
    m_aliens.clear();
    m_barriers.clear();

    initPlayer();
    initAlienGrid();
    initBarriers();
    initSaucer();

    setPlayerTargets();
    setAlienTargets();
}

void Game::initPlayer() {
    entityAnimations& playerAnimations = loadPlayerAnimations();

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
    alienAnimations& allAlienAnimations = loadAlienAnimations();

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
    entityAnimations& saucerAnimations = loadSaucerAnimations();
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

    barrierAnimations& allBarrierAnimations = loadBarrierAnimations();
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
        m_interface.startFrame();
        keyboardEvents events;
        m_interface.pollKeyboardEvents(events);
        handleKeyboardEvents(events);

        std::this_thread::sleep_for(std::chrono::milliseconds(m_tickMS));
        auto elapsed = std::chrono::steady_clock::now() - m_startTime;
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
        int gameTicks = elapsed_ms.count() / m_tickMS;

        for (auto& entity : m_entities) {
            if (!entity) continue;
            if (isRunning()) entity->update(gameTicks);
            entity->draw(&m_interface);
        }

        checkGameOver();
        displayOptionalOverlay();
        m_interface.renderFrame();
    }
}

void Game::handleKeyboardEvents(keyboardEvents& events) {
    // game events
    if (events.gameStart)
        m_started = true;
    if (events.gamePause)
        togglePause();
    if (events.gameRestart)
        reset();
    if (!isRunning()) return;

    // player events
    if (events.playerStepLeft)
        m_player->takeStep(false);
    if (events.playerStepRight)
        m_player->takeStep(true);
    if (events.playerFire)
        m_player->fire();
}

void Game::checkGameOver() {
    if (m_player->getNumLives() <= 0) {
        m_gameOver = true;
    }
    // TODO also check if aliens have reached the bottom
}

void Game::displayOptionalOverlay() const {
    if (!m_started) {
        m_interface.displayStartingOverlay();
    } else if (m_gameOver) {
        m_interface.displayGameOverOverlay();
    } else if (m_paused) {
        m_interface.displayPauseOverlay();
    }
}


entityAnimations& loadPlayerAnimations() {
    static entityAnimations playerAnimations{
        Animation(PLAYER_DEFAULT_TEXTURE_PATHS, 0, true),
        Animation(PLAYER_DEATH_TEXTURE_PATHS, GAME_DEATH_ANIMATION_TICKS)
    };
    return playerAnimations;
}

entityAnimations& loadSaucerAnimations() {
    static entityAnimations saucerAnimations{
        Animation(SAUCER_DEFAULT_TEXTURE_PATHS, 0, true),
        Animation(SAUCER_DEATH_TEXTURE_PATHS, GAME_DEATH_ANIMATION_TICKS)
    };
    return saucerAnimations;
}

alienAnimations& loadAlienAnimations() {
    static alienAnimations allAlienAnimations{
        entityAnimations{
            Animation(ALIEN_A_DEFAULT_TEXTURE_PATHS, ALIEN_STEP_EVERY_TICKS, true),
            Animation(ALIEN_A_DEATH_TEXTURE_PATHS, GAME_DEATH_ANIMATION_TICKS)
        },
        entityAnimations{
            Animation(ALIEN_B_DEFAULT_TEXTURE_PATHS, ALIEN_STEP_EVERY_TICKS, true),
            Animation(ALIEN_B_DEATH_TEXTURE_PATHS, GAME_DEATH_ANIMATION_TICKS)
        },
        entityAnimations{
            Animation(ALIEN_C_DEFAULT_TEXTURE_PATHS, ALIEN_STEP_EVERY_TICKS, true),
            Animation(ALIEN_C_DEATH_TEXTURE_PATHS, GAME_DEATH_ANIMATION_TICKS)
        }
    };
    return allAlienAnimations;
}

barrierAnimations& loadBarrierAnimations() {
    static barrierAnimations allBarrierAnimations{
        entityAnimations{Animation(BARRIER_A_DEFAULT_TEXTURE_PATHS), Animation()},
        entityAnimations{Animation(BARRIER_B_DEFAULT_TEXTURE_PATHS), Animation()},
        entityAnimations{Animation(BARRIER_C_DEFAULT_TEXTURE_PATHS), Animation()},
        entityAnimations{Animation(BARRIER_D_DEFAULT_TEXTURE_PATHS), Animation()},
        entityAnimations{Animation(BARRIER_E_DEFAULT_TEXTURE_PATHS), Animation()}
    };
    return allBarrierAnimations;
}
