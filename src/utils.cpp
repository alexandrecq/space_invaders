#include "space_invaders/utils.h"

const vector<string> generateTexturePaths(const std::string& baseName, int numTextures) {
    std::vector<std::string> texturePaths;
    texturePaths.reserve(numTextures);
    for (int i = 0; i < numTextures; ++i) {
        texturePaths.push_back(baseName + std::to_string(i) + ".png");
    }
    return texturePaths;
}
