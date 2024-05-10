#pragma once

#include <string>
#include <vector>

#include <GL/gl.h>

using std::string;
using std::vector;


class Animation {
public:
	Animation(const vector<string> texturePaths);
private:
	vector<GLuint> m_textureIDs;
};

