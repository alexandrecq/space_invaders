#include <GL/glut.h>

GLuint renderBufferID; // Render buffer ID

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    // Create and bind render buffer
    glGenRenderbuffers(1, &renderBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
    
    // Set render buffer storage
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, 800, 600); // Assuming 800x600 resolution
    
    // Unbind render buffer
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void renderToBuffer() {
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
    
    // Render your scene with coarse blocky sprites here
    // You can draw directly to the render buffer using OpenGL commands
    
    // For simplicity, let's clear the buffer with a solid color
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0); // White color
    glBegin(GL_QUADS);
    glVertex2f(-1.0, -1.0);
    glVertex2f(1.0, -1.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(-1.0, 1.0);
    glEnd();
    
    // End rendering to the render buffer
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void display() {
    // Render the scene to the render buffer
    renderToBuffer();
    
    // Draw the render buffer texture to the screen
    glClear(GL_COLOR_BUFFER_BIT);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Render Buffer Example");
    
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}

