#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
