#pragma once

#include <string>
#include <vector>

#include <GL/gl.h>

using std::string;
using std::vector;


class Animation {
public:
	Animation() = default;
	Animation(const vector<string> texturePaths);
	GLuint getCurrentTexture() const;
	bool isEmpty() const { return m_textureIDs.empty(); }
private:
	vector<GLuint> m_textureIDs;
};
