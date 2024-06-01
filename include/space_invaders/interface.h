#pragma once

#include "imgui.h"
#include <memory>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "space_invaders/common.h"
#include "space_invaders/constants.h"

class Player;

struct keyboardEvents {
	bool playerStepLeft = false;
	bool playerStepRight = false;
	bool playerFire = false;
	bool gameStart = false;
	bool gamePause = false;
	bool gameRestart = false;
};

struct Canvas {
	Canvas(const Array2f& bottomLeft, const Array2f& widthHeight) :
		m_bottomLeft(bottomLeft), m_widthHeight(widthHeight) {}
	void mapToGlobal(Array2f& widthHeight, Array2f& bottomLeft) const {
	    bottomLeft = (bottomLeft + 1) * m_widthHeight / 2  + m_bottomLeft;
	    widthHeight *= m_widthHeight / 2;
	}
	const Array2f m_bottomLeft;
	const Array2f m_widthHeight;
};

enum class OverlayType {
    Starting,
    Pause,
    GameOver
};

struct OverlayConfig {
    const char* windowTitle;
    const char* headingText;
    const char* bodyText;
};


class Interface {
public:
	Interface();
	~Interface();
	void pollKeyboardEvents(keyboardEvents& eventsOut) const;
	void startFrame();
	void renderFrame() const;
	bool isAlive() const;
	void drawRectangle(Array2f bottomLeft, Array2f widthHeight,
		    const Array4f& color = {1.f, 1.f, 1.f, 1.f}, bool onGameCanvas = false) const;
	void setPlayer(std::shared_ptr<const Player> player) { m_player = player; }
	void drawTexture(
		const GLuint textureID,
		const float& x, const float& y, const float& width, const float& height,
		const float& texCoordX = 0.f, const float& texCoordY = 0.f, const float& texWidth = 1.f, const float& texHeight = 1.f
	) const;
	void drawTexture(
		const GLuint textureID,
		Array2f bottomLeft, Array2f widthHeight, bool onGameCanvas = false
	) const;
	void displayOverlay(OverlayType type) const;
private:
	void drawDashboard() const;
	void drawBorder(const Array2f& bottomLeft, const Array2f& widthHeight, float thickness,
		const Array4f& color = {1.f, 1.f, 1.f, 1.f}
	) const;
	inline void updateWindowSize() { glfwGetWindowSize(m_window, &m_windowWidthHeight.x(), &m_windowWidthHeight.y()); }

	std::shared_ptr<const Player> m_player;
	GLFWwindow* m_window = nullptr;
	Eigen::Array2i m_windowWidthHeight;
	ImFont* m_fontBody;
	ImFont* m_fontHeading;
	const Canvas m_gameCanvas = Canvas{
		GAME_CANVAS_BOTTOM_LEFT + INTERFACE_BORDER_THICKNESS,
		GAME_CANVAS_WIDTH_HEIGHT - 2 * INTERFACE_BORDER_THICKNESS
	};
};
