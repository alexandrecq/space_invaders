#include <assert.h>

#include "space_invaders/animation.h"
#include "space_invaders/constants.h"
#include "space_invaders/load_image.h"


Animation::Animation(const vector<string> texturePaths) {
    for (auto& path : texturePaths) {
        GLuint textureID;
        bool loaded = LoadTextureFromFile((RELATIVE_TEXTURE_PATH + path).c_str(), &textureID);
        assert(loaded);
        m_textureIDs.push_back(textureID);
    }
}

void Animation::incrementTexture() {
    m_currentIndex = (m_currentIndex + 1) % m_textureIDs.size();
}

GLuint Animation::getCurrentTexture() const {
    if (isEmpty()) return 0;
    else return m_textureIDs[m_currentIndex];
}
