#include "space_invaders/game.h"
#include "space_invaders/entity.h"
#include <cstdlib>

Game::Game() {
    // m_interface = std::make_shared<Interface>();
    m_start_time = std::chrono::steady_clock::now();

    // m_entities[0] = std::make_shared<Player>(&m_interface);
    auto player = std::make_shared<Player>(&m_interface);
    m_entities.push_back(player);
    m_interface.setPlayer(player);

    initAlienGrid();

    const Vector2f barrier_position{-.25, -.5};
    m_entities.push_back(std::make_shared<Barrier>(&m_interface, barrier_position));
}

void Game::initAlienGrid(int num_rows, int num_cols) {
    const float width = .1, height = .1;
    const Vector2f delta_xy = {width * 1.5, height * 1.5};
    const Vector2f step_size = {width / 2, -height / 2};

    float grid_outer_width = (num_cols * width) + (num_cols - 1) * (delta_xy.x() - width);
    float extra_space = 2 - grid_outer_width;
    int num_steps_til_reverse = extra_space / step_size.x();

    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            Vector2f position = {-1 + delta_xy.x() * j, 1 - delta_xy.y() * i};
            // printf("i: %d, j: %d, position: (%.2f, %.2f)\n", i, j, position.x(), position.y());
            auto alien = std::make_shared<Alien>(
                &m_interface,
                position,
                Vector2f{width, height},
                step_size,
                num_steps_til_reverse
            );
            m_entities.push_back(alien);
        }
    }
}

void Game::run() {
    while (m_interface.isAlive()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(m_tick_ms));
        auto elapsed = std::chrono::steady_clock::now() - m_start_time;
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
        printf("Elapsed: %.2ld\n", elapsed_ms.count());

        m_interface.startFrame();
        for (auto entity : m_entities) {
            if (entity) {
                entity->update(elapsed_ms.count());
                entity->draw();
            }
        }
        m_interface.renderFrame();
    }
}
