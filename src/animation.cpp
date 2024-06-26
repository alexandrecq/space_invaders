#include <stdexcept>
#include <assert.h>

#include "space_invaders/animation.h"
#include "space_invaders/constants.h"
#include "space_invaders/load_image.h"


std::unordered_set<string> Animation::m_loadedTextures;

Animation::Animation(const vector<string> texturePaths, int updateEveryTicks, bool isLoop) :
    m_updateEveryTicks(updateEveryTicks), m_isLoop(isLoop) {
    for (auto& path : texturePaths) {
        checkSingleTextureLoad(path);
        GLuint textureID;
        bool loaded = LoadTextureFromFile((ASSETS_RELATIVE_PATH + path).c_str(), &textureID);
        if (!loaded) throw std::runtime_error{"Could not load texture file: " + path};
        m_textureIDs.push_back(textureID);
    }
}

void Animation::checkSingleTextureLoad(const string texturePath) {
    bool found = (m_loadedTextures.find(texturePath) != m_loadedTextures.end());
    if (found) throw std::runtime_error{"Texture " + texturePath + " is already loaded\n"};
    m_loadedTextures.insert(texturePath);
}

void Animation::updateTexture(int ticks) {
    if (ticks - m_lastUpdateTick > m_updateEveryTicks) {
        incrementTexture();
        m_lastUpdateTick = ticks;
    }
}

void Animation::updateTexture() {
    incrementTexture();
}

GLuint Animation::getCurrentTexture() const {
    if (isEmpty()) return 0;
    else return m_textureIDs[m_currentIndex];
}

bool Animation::isDone() const {
    if (m_isLoop) return false;
    return (m_currentIndex == m_textureIDs.size() - 1);
}

void Animation::incrementTexture() {
    m_currentIndex++;
    if (m_isLoop) {
        m_currentIndex = m_currentIndex % m_textureIDs.size();
    }
}
