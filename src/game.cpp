#include "space_invaders/game.h"
#include "Eigen/src/Core/Matrix.h"

Game::Game() {
    m_start_time = std::chrono::steady_clock::now();
    m_tick_ms = 1;

    m_entities[0] = std::make_shared<Player>(&m_interface);
    Vector2f pos = {-1, 1};
    m_entities[1] = std::make_shared<Alien>(&m_interface, pos);

}

void Game::run() {
    while (m_interface.isAlive()) {
        std::this_thread::sleep_for(std::chrono::milliseconds((long long)m_tick_ms));
        auto elapsed = std::chrono::steady_clock::now() - m_start_time;
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
        printf("Elapsed: %.2ld\n", elapsed_ms.count());


        m_interface.startFrame();

        // m_interface.drawRectangle({pos, 0}, {.1, .1}, {1, 1, 1});
        // long int update_interval = 1000;
        // float pos = ((float)elapsed_ms.count() / 5000) - 1;

        for (auto entity : m_entities) {
            if (entity) {
                entity->update(elapsed_ms.count());
                entity->draw();
            }
        }
        m_interface.renderFrame();
    }
}
