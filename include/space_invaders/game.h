#pragma once

#include "space_invaders/interface.h"


class Game {
public:
	Game();
	void run();

private:
	Interface m_interface;
};


