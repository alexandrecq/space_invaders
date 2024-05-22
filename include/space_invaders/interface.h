#pragma once

#include <memory>
#include <stdio.h>

#include <Eigen/Dense>

using Eigen::Array2f;
using Eigen::Array2f;

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "space_invaders/entity.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

struct Canvas {
	Canvas(const Array2f& bottomLeft, const Array2f& widthHeight) :
		m_bottomLeft(bottomLeft), m_widthHeight(widthHeight) {}
	void mapFromGlobal(Array2f& widthHeight, Array2f& bottomLeft) const {
	    bottomLeft = (bottomLeft + 1) * m_widthHeight / 2  + m_bottomLeft;
	    widthHeight *= m_widthHeight / 2;
	}
	const Array2f m_bottomLeft;
	const Array2f m_widthHeight;
};

class Interface {
public:
	Interface();
	~Interface();
	void startFrame() const;
	void renderFrame() const;
	bool isAlive() const;
	void drawRectangle(Array2f bottomLeft, Array2f widthHeight, Array3f color = {1., 1., 1.}, bool globalCanvas = false) const;
	void setPlayer(std::shared_ptr<Player> player) { m_player = player; }
	void drawTexture(
		const GLuint textureID,
		const float& x, const float& y, const float& width, const float& height,
		const float& texCoordX = 0.f, const float& texCoordY = 0.f, const float& texWidth = 1.f, const float& texHeight = 1.f
	) const;
	void drawTexture(
		const GLuint textureID,
		Array2f bottomLeft, Array2f widthHeight,
		const Array2f& texBottomLeft = {0., 0.}, const Array2f& texWidthHeight = {1., 1.}
	) const;
private:
	bool keyboardEvent() const;
	void drawOverlay() const;

	std::shared_ptr<Player> m_player;
	GLFWwindow* m_window = nullptr;
	vector<GLuint> m_textureIDs{2};
	const Canvas m_gameCanvas = Canvas(
		GAME_CANVAS_BOTTOM_LEFT + INTERFACE_FRAME_THICKNESS,
		GAME_CANVAS_WIDTH_HEIGHT - 2 * INTERFACE_FRAME_THICKNESS
	);
};
