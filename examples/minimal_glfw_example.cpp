#include <GLFW/glfw3.h>

void drawTriangle() {
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);   // Red
    glVertex2f(-0.5f, -0.5f);
    glColor3f(0.0f, 1.0f, 0.0f);   // Green
    glVertex2f(0.5f, -0.5f);
    glColor3f(0.0f, 0.0f, 1.0f);   // Blue
    glVertex2f(0.0f, 0.5f);
    glEnd();
}

void drawSquare() {
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 0.0f);   // Yellow
    glVertex2f(-0.5f, -0.5f);
    glColor3f(0.0f, 1.0f, 1.0f);   // Cyan
    glVertex2f(-0.5f, 0.5f);
    glColor3f(1.0f, 0.0f, 1.0f);   // Magenta
    glVertex2f(0.5f, 0.5f);
    glColor3f(1.0f, 0.5f, 0.0f);   // Orange
    glVertex2f(0.5f, -0.5f);
    glEnd();
}

void renderScene(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a triangle
    drawTriangle();

    // Draw a square
    // drawSquare();

    glfwSwapBuffers(window);
}

int OpenWindow() {
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Render here
        renderScene(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
