#pragma once

#include <string>
#include <vector>

#include <GL/gl.h>

using std::string;
using std::vector;


class Animation {
public:
	Animation() = default;
	Animation(const vector<string> texturePaths, int updateEveryTicks = 1000, bool isLoop = true);
	void updateTexture(int ticks);
	void reset() { m_currentIndex = 0; }
	GLuint getCurrentTexture() const;
	bool isEmpty() const { return m_textureIDs.empty(); }
	bool isDone() const;
private:
	void incrementTexture();

	vector<GLuint> m_textureIDs;
	int m_currentIndex{};
	bool m_isLoop;
	int m_updateEveryTicks;
	int m_lastUpdateTick = 0;
};
