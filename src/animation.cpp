#include <assert.h>

#include "space_invaders/animation.h"
#include "space_invaders/constants.h"
#include "space_invaders/load_image.h"


Animation::Animation(const vector<string> texturePaths) {
    for (auto& path : texturePaths) {
        GLuint textureID;
        bool loaded = LoadTextureFromFile((RELATIVE_PATH + path).c_str(), &textureID);
        assert(loaded);
        m_textureIDs.push_back(textureID);
        printf("loaded %d\n", textureID);
    }
}

GLuint Animation::getCurrentTexture() const {
    if (isEmpty()) return 0;
    else return m_textureIDs[0];
}
