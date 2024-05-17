#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include <GL/gl.h>

using std::string;
using std::vector;


class Animation {
public:
	Animation() = default;
	// Animation(const vector<string> texturePaths, int updateEveryTicks = 1000, bool isLoop = false);
	Animation(const vector<string> texturePaths, int updateEveryTicks = 1000, bool isLoop = true);
	void updateTexture(int ticks);
	void updateTexture();
	void reset() { m_currentIndex = 0; }
	GLuint getCurrentTexture() const;
	bool isEmpty() const { return m_textureIDs.empty(); }
	bool isDone() const;
private:
	void incrementTexture();
	void checkSingleTextureLoad(const string texturePath);

	vector<GLuint> m_textureIDs;
	int m_currentIndex{};
	bool m_isLoop;
	int m_updateEveryTicks;
	int m_lastUpdateTick = 0;
	static std::unordered_set<string> m_loadedTextures;
};
