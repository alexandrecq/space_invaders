#pragma once

#include <memory>
#include <stdio.h>

#include <Eigen/Dense>

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "space_invaders/entity.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

class Interface {
public:
	Interface();
	~Interface();
	void startFrame() const;
	void renderFrame() const;
	bool isAlive() const;
	void drawRectangle(Eigen::Array2f bottomLeft, Eigen::Array2f widthHeight, Eigen::Array3f color = {1., 1., 1.}) const;
	bool keyboardEvent() const;
	void setPlayer(std::shared_ptr<Player> player);
private:
	// Eigen::Array2i getWindowSize
	std::shared_ptr<Player> m_player;
	GLFWwindow* m_window = nullptr;
};
