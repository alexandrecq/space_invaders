// #define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void printGLError() {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Texture Loading", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load image
    int width, height, channels;
    // unsigned char* image_data = stbi_load("../assets/zeros10x10x3.png", &width, &height, &channels, 0);
    // unsigned char* image_data = stbi_load("../assets/255.png", &width, &height, &channels, 0);
    // unsigned char* image_data = stbi_load("../assets/100x1.jpg", &width, &height, &channels, 0);
    // unsigned char* image_data = stbi_load("../assets/albert1000x.jpg", &width, &height, &channels, 0);

    // unsigned char* image_data = stbi_load("../assets/logo-64x64.png", &width, &height, &channels, 0);
    unsigned char* image_data = stbi_load("../assets/1ST_FRAME_3.png", &width, &height, &channels, 0);
    if (!image_data) {
        std::cerr << "Failed to load image" << std::endl;
        glfwTerminate();
        return -1;
    }

    // //create test checker image
    // int width = 8, height = 8, channels = 1;
    // unsigned char image_data[64];
    // for (int i = 0; i < 64; ++i)
    //     image_data[i] = ((i + (i / 8)) % 2) * 128 + 127;
 
    // //Print image data
    // // printf("%d\n", channels);
    // int values_per_row = width * channels;
    // for (int i = 0; i < width * height * channels; i++){
    //     printf("(%d, %d, %d): %d\n", i / values_per_row, i % values_per_row / channels, i % channels, *(image_data + i));
    // }

    //upload to GPU texture
    GLuint texture_id;
    GLint internal_format = (channels > 3) ? GL_RGBA : GL_RGB;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, image_data);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, internal_format, GL_UNSIGNED_BYTE, image_data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image_data);

    printf("Texture ID: %u\n", texture_id);
    printGLError();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        //clear and draw quad with texture (could be in display callback)
        glClear(GL_COLOR_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);

        glTexCoord2f(0.0f, 1.0f); glVertex2f(-.5f, -.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(.5f, -.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(.5f, .5f);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-.5f, .5f);

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwTerminate();
    return 0;
}
