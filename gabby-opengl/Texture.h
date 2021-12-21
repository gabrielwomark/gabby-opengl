#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>
#include <glfw3.h>
#include <stb/stb_image.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
using namespace std;
typedef uint8_t u8;

class Texture {
   public:
    unsigned int handle;

    Texture(string path, GLuint color_channel_type)
        : color_channel_type{color_channel_type} {
        string asset_path = get_asset_path(path);
        unique_ptr<u8[]> pixels;
        size_t width, height;
        load_pixels(asset_path, pixels, &width, &height);
        create_from_pixels(pixels, width, height);
    }

    ~Texture(){};

    void bind_texture() {
        glActiveTexture(GL_TEXTURE0 + handle);
        glBindTexture(GL_TEXTURE_2D, handle);
    }

   private:
    GLuint color_channel_type;

    string get_asset_path(string sub_path) {
        string ROOT_DIR = "/Users/gabrielwomark/dev/GabbyOpenGL";
        return ROOT_DIR + "/" + sub_path;
    }

    void load_pixels(string path, unique_ptr<u8[]>& pixels_out,
                     size_t* width_out, size_t* height_out) {
        // load and generate texture
        int w, h, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &w, &h, &nrChannels, 0);

        if (!data) {
            cout << get_asset_path(path) << "Failed to load texture" << endl;
            exit(1);
        }

        pixels_out = make_unique<u8[]>(w * h * 4);
        memcpy(pixels_out.get(), data, w * h * 4);
        *width_out = w;
        *height_out = h;
        stbi_image_free(data);
    }

    void create_from_pixels(unique_ptr<u8[]>& pixels, size_t width,
                            size_t height) {
        // GENERATE AND ADD FIRST TEXTURE TO GL_TEXTURE0
        // takes in the number of textures and takes in an unsinged int * as an
        // array
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);
        // set the texture filtering and wrapping options on the currently bound
        // texture here we're setting the wrap option to repeat along the
        // horizontal and vertical dimensions
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                     color_channel_type, GL_UNSIGNED_BYTE, pixels.get());
        glGenerateMipmap(GL_TEXTURE_2D);
    }
};
#endif
