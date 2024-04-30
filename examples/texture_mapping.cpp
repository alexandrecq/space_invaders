#include <GL/glut.h>

GLuint textureID; // Texture ID for the texture atlas

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    
    // Load texture atlas
    // You need to implement the function to load your texture atlas
    // For simplicity, let's assume you have a function loadTextureAtlas()
    textureID = loadTextureAtlas("texture_atlas.png"); // Load texture atlas image
}

void drawSprite(float x, float y, float width, float height, float texCoordX, float texCoordY, float texWidth, float texHeight) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glBegin(GL_QUADS);
    glTexCoord2f(texCoordX, texCoordY);
    glVertex2f(x, y);
    
    glTexCoord2f(texCoordX + texWidth, texCoordY);
    glVertex2f(x + width, y);
    
    glTexCoord2f(texCoordX + texWidth, texCoordY + texHeight);
    glVertex2f(x + width, y + height);
    
    glTexCoord2f(texCoordX, texCoordY + texHeight);
    glVertex2f(x, y + height);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw a sprite at (100, 100) with a size of 50x50 pixels
    // Assume the sprite's texture coordinates in the atlas are (0, 0) to (0.1, 0.1)
    drawSprite(100, 100, 50, 50, 0.0, 0.0, 0.1, 0.1);
    
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Texture Mapping Example");
    
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}
