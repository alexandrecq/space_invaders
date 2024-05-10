#pragma once

#include <string>
#include <vector>
#include <Eigen/Dense>

using std::string;
using std::vector;
using Eigen::Array2f;
using Eigen::Array3f;

// Texture paths
const string RELATIVE_TEXTURE_PATH("../assets/");
const vector<string> ALIEN_A_TEXTURE_PATHS{
	"alien_a_walk_0.png",
	"alien_a_walk_1.png",
};
const vector<string> ALIEN_B_TEXTURE_PATHS{
	"alien_b_walk_0.png",
	"alien_b_walk_1.png",
};
const vector<string> ALIEN_C_TEXTURE_PATHS{
	"alien_c_walk_0.png",
	"alien_c_walk_1.png",
};
const vector<string> PLAYER_TEXTURE_PATHS{
	"player.png",
};

//Game
static const int GAME_TICK_MS = 1;
static const int GAME_NUM_ALIEN_ROWS = 5;
static const int GAME_NUM_ALIEN_COLS = 10;
static const int GAME_NUM_BARRIERS = 4;

// Player
static const Array2f PLAYER_WIDTH_HEIGHT = {.2, .1};
static const Array2f PLAYER_START_POSITION = {0, -1 + PLAYER_WIDTH_HEIGHT.y() / 2};
static const Array3f PLAYER_COLOR = {0., .1, 1.};
static const int PLAYER_NUM_LIVES = 3;

// Alien
static const Array2f ALIEN_WIDTH_HEIGHT = {.1, .1};
static const Array3f ALIEN_COLOR = {.5, 0, .5};
static const int ALIEN_STEP_EVERY_TICKS = 500;
static const Array2f ALIEN_STEP_SIZE = {.05, -.05};
static const int ALIEN_FIRE_PROBABILITY_X100K = 50;
