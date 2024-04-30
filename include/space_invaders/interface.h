#pragma once

#include <stdio.h>
#include <Eigen/Dense>

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// void renderScene(GLFWwindow* window);

class Interface {
public:
	Interface();
	~Interface();
	void update();
	bool isAlive() const;
	void renderScene();
private:
	void drawRectangle(Eigen::Vector2f topLeft, Eigen::Vector2f widthHeight, Eigen::Vector3f color);
	GLFWwindow* m_window;
};
