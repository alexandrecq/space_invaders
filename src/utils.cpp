#include "space_invaders/utils.h"

const vector<string> generateTexturePaths(const std::string& baseName, int numTextures) {
    std::vector<std::string> texturePaths;
    texturePaths.reserve(numTextures);
    for (int i = 0; i < numTextures; ++i) {
        texturePaths.push_back(baseName + std::to_string(i) + ".png");
    }
    return texturePaths;
}


template class RandomNumberGenerator<int>;

template<typename T>
RandomNumberGenerator<T>::RandomNumberGenerator(T maxValue) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, maxValue);
    m_gen = gen;
    m_distribution = dist;
}

template<typename T>
void RandomNumberGenerator<T>::generateNumber(T* numberOut) {
    *numberOut = m_distribution(m_gen);
}
