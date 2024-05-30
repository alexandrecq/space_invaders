#pragma once

#include "space_invaders/utils.h"


//Game
static const int GAME_TICK_MS = 1;
static const int GAME_NUM_ALIEN_ROWS = 5;
static const int GAME_NUM_ALIEN_COLS = 10;
static const int GAME_NUM_BARRIERS = 4;
static const int GAME_DEATH_ANIMATION_TICKS = 50;

//Entity
static const int ENTITY_DEFAULT_ANIMATION = 0;
static const int ENTITY_DEATH_ANIMATION = 1;

// Player
static const Array2f PLAYER_WIDTH_HEIGHT = {.1, .1};
static const Array2f PLAYER_START_POSITION = {0, -1 + PLAYER_WIDTH_HEIGHT.y() / 2};
static const Array3f PLAYER_COLOR = {0., .1, 1.};
static const float PLAYER_STEP_SIZE = .05;
static const int PLAYER_NUM_LIVES = 3;
static const int PLAYER_DEATH_SLEEP_MS = 1000;

// Alien
static const Array2f ALIEN_WIDTH_HEIGHT = {.1, .1};
static const Array3f ALIEN_COLOR = {.5, 0, .5};
static const int ALIEN_STEP_EVERY_TICKS = 500;
static const Array2f ALIEN_STEP_SIZE = {.05, -.05};
static const float ALIEN_FIRE_PROBABILITY = .0005f;

// Saucer
static const Array2f SAUCER_WIDTH_HEIGHT = {.13, .05};
static const float SAUCER_POS_Y = .85;
static const int SAUCER_STEP_EVERY_TICKS = 1;
static const Array2f SAUCER_STEP_SIZE = {.005, 0.};
static const float SAUCER_APPEAR_PROBABILITY = 1.f;

// Barrier
static const Array2f TILE_WIDTH_HEIGHT = {.08, .08};
static const float BARRIER_POS_Y = -.75;
static const Array3f TILE_COLOR = {.5, .4, .1};
static const int BARRIER_NUM_SHAPES = 5;
static const int BARRIER_TILE_ROWS = 3;
static const int BARRIER_TILE_COLS = 4;
static const int BARRIER_NUM_LIVES = 5;
static const int BARRIER_FULL_TILE    = 0;
static const int BARRIER_BOTTOM_RIGHT = 1;
static const int BARRIER_BOTTOM_LEFT  = 2;
static const int BARRIER_TOP_LEFT     = 3;
static const int BARRIER_TOP_RIGHT    = 4;

//Interface
static const float INTERFACE_FRAME_THICKNESS = 0.01;
static const Array2f GAME_CANVAS_BOTTOM_LEFT = {-1, -1};
static const Array2f GAME_CANVAS_WIDTH_HEIGHT = {2, 1.8};
static const Eigen::Array4f INTERFACE_CLEAR_COLOR = {0.15f, 0.05f, 0.20f, .90f};
static const float FONT_SIZE_HEADING = 100;
static const float FONT_SIZE_BODY = 50;

// Texture paths
const string ASSETS_RELATIVE_PATH("../assets/");
const vector<string> PLAYER_DEFAULT_TEXTURE_PATHS{ "player_default_0.png" };
const vector<string> PLAYER_DEATH_TEXTURE_PATHS = generateTexturePaths("player_death_", 6);
const vector<string> ALIEN_A_DEFAULT_TEXTURE_PATHS = generateTexturePaths("alien_a_walk_", 2);
const vector<string> ALIEN_B_DEFAULT_TEXTURE_PATHS = generateTexturePaths("alien_b_walk_", 2);
const vector<string> ALIEN_C_DEFAULT_TEXTURE_PATHS = generateTexturePaths("alien_c_walk_", 2);
const vector<string> ALIEN_A_DEATH_TEXTURE_PATHS = generateTexturePaths("alien_a_death_", 9);
const vector<string> ALIEN_B_DEATH_TEXTURE_PATHS = generateTexturePaths("alien_b_death_", 9);
const vector<string> ALIEN_C_DEATH_TEXTURE_PATHS = generateTexturePaths("alien_c_death_", 9);
const vector<string> SAUCER_DEFAULT_TEXTURE_PATHS{ "saucer_default_0.png" };
const vector<string> SAUCER_DEATH_TEXTURE_PATHS = generateTexturePaths("saucer_death_", 9);
const vector<string> BARRIER_A_DEFAULT_TEXTURE_PATHS = generateTexturePaths("barrier_a_", BARRIER_NUM_LIVES);
const vector<string> BARRIER_B_DEFAULT_TEXTURE_PATHS = generateTexturePaths("barrier_b_", BARRIER_NUM_LIVES);
const vector<string> BARRIER_C_DEFAULT_TEXTURE_PATHS = generateTexturePaths("barrier_c_", BARRIER_NUM_LIVES);
const vector<string> BARRIER_D_DEFAULT_TEXTURE_PATHS = generateTexturePaths("barrier_d_", BARRIER_NUM_LIVES);
const vector<string> BARRIER_E_DEFAULT_TEXTURE_PATHS = generateTexturePaths("barrier_e_", BARRIER_NUM_LIVES);
