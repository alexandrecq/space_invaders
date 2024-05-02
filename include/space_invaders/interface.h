#pragma once

#include <stdio.h>
#include <Eigen/Dense>

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

class Interface {
public:
	Interface();
	~Interface();
	void update();
	void startFrame();
	void renderFrame();
	bool isAlive() const;
	void renderScene(float pos);
	void drawRectangle(Eigen::Vector2f topLeft, Eigen::Vector2f widthHeight, Eigen::Vector3f color = {1., 1., 1.});
private:
	// Eigen::Vector2i getWindowSize
	GLFWwindow* m_window;
};
