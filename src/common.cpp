#include "space_invaders/common.h"

const vector<string> generateTexturePaths(const std::string& baseName, int numTextures) {
    vector<string> texturePaths;
    texturePaths.reserve(numTextures);
    for (int i = 0; i < numTextures; ++i) {
        texturePaths.push_back(baseName + std::to_string(i) + ".png");
    }
    return texturePaths;
}


RandomNumberGenerator::RandomNumberGenerator() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);
    m_gen = gen;
    m_distribution = dist;
}

void RandomNumberGenerator::generateNumber(float& numberOut) {
    numberOut = m_distribution(m_gen);
}
