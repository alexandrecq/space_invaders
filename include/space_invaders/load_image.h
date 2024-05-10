#pragma once

#include <GL/gl.h> // Will drag system OpenGL headers

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
bool LoadTextureFromFile(const char* filename, GLuint* out_texture);
