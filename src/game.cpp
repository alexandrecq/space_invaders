#include "space_invaders/game.h"

Game::Game() {
    // m_interface = Interface();
    Interface interface;
    m_interface = interface;
}

void Game::run() {
    // Interface interface;
    while (m_interface.isAlive()) {
        m_interface.update();
    }
}
