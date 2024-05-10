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
    }
}
                // m_interface.drawTexture(textureID, 0.5, 0.5, .1, .1, 0., 0., 1., 1.);
