#include "space_invaders/load_image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    int num_channels = 4;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, num_channels);
    if (image_data == NULL)
        return false;

    // //Print image data
    // printf("%d, %d, %d\n", image_width, image_height, num_channels);
    // int values_per_row = image_width * num_channels;
    // unsigned char* data_ptr = image_data;
    // for (int x = 0; x < image_width; x++){
    //     for (int y = 0; y < image_height; y++){
    //         printf("(");
    //         for (int c = 0; c < num_channels; c++){
    //             printf("%02x, ", *data_ptr);
    //             data_ptr++;
    //         }
    //         printf(") ");
    //     }
    //     printf("\n");
    // }

    // Create a OpenGL texture identifier
    GLuint texture_id;
    GLint internal_format = (num_channels > 3) ? GL_RGBA : GL_RGB;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, image_width, image_height, 0, internal_format, GL_UNSIGNED_BYTE, image_data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image_data);

    *out_texture = texture_id;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

bool LoadTextureFromFile(const char* filename, GLuint* out_texture) {
    int out_width, out_height;
    return LoadTextureFromFile(filename, out_texture, &out_width, &out_height);
}
