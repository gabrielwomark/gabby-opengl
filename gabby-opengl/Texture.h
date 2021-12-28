#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>
#include <glfw3.h>
#include <stb/stb_image.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
typedef uint8_t u8;

class Texture {
   public:
    unsigned int handle;

    Texture(std::string path, GLuint color_channel_type)
        : color_channel_type{color_channel_type} {
        std::string asset_path = get_asset_path(path);
        std::vector<uint8_t> pixels;
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

    std::string get_asset_path(std::string sub_path) {
        std::string ROOT_DIR = "/Users/gabrielwomark/dev/GabbyOpenGL";
        return ROOT_DIR + "/" + sub_path;
    }

    void load_pixels(std::string path, std::vector<uint8_t>& pixels_out,
                     size_t* width_out, size_t* height_out) {
        // load pixel data
        int w, h, nrChannels;
        // TODO: figure out what to do about this
        // stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &w, &h, &nrChannels, 0);

        if (!data) {
            std::cout << get_asset_path(path) << "Failed to load texture"
                      << "\n";
            exit(1);
        }

        // copy function local pointer to vector owned by texture class for
        // future use
        pixels_out.reserve(w * h * 4);
        std::copy(data, data + w * h * 4, pixels_out.begin());
        *width_out = w;
        *height_out = h;
        stbi_image_free(data);
    }

    void create_from_pixels(std::vector<u8>& pixels, size_t width,
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
                     color_channel_type, GL_UNSIGNED_BYTE,
                     (static_cast<void*>(pixels.data())));
        glGenerateMipmap(GL_TEXTURE_2D);
    }
};
#endif
