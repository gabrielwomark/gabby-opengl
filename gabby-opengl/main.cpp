//
//  main.cpp
//  GabbyOpenGL
//
//  Created by Gabriel Womark on 5/16/21.
//

// GL libs
#include <glad/glad.h>
#include <glfw3.h>
#include "third_party/glm/glm/glm.hpp"
#include "third_party/glm/glm/gtc/matrix_transform.hpp"
#include "third_party/glm/glm/gtc/type_ptr.hpp"

// standard libs
#include "math.h"
#include <iostream>
#include <fstream>
#include <sstream>

// third party libs
#include <stb/stb_image.h>

// My libs
#include "Window.h"
#include "Shader.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
void draw_triangle();

void setup_vbo(GLuint VBO, float vertices[], size_t vertices_len) {
    // Buffer is memory on GPU, this snippet sets up a buffer and copies
    // vertices to it
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_len, vertices, GL_STATIC_DRAW);
}

void setup_ebo(GLuint EBO, unsigned int indices[], size_t indices_len) {
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_len, indices, GL_STATIC_DRAW);
}


GLuint setup_vao_with_ebo(float* vertices, size_t vertices_len, unsigned int *indices, size_t indices_len) {
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    setup_vbo(VBO, vertices, vertices_len);
    setup_ebo(EBO, indices, indices_len);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    return VAO;
}

GLuint setup_vao(float* vertices, size_t vertices_len) {
    GLuint VAO = 0;
    GLuint VBO = 0;
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    setup_vbo(VBO, vertices, vertices_len);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    return VAO;
}
GLuint setup_vao_w_color_texture(
        float* vertices, 
        size_t vertices_len,
        unsigned int* indices,
        size_t indices_len,
        unsigned int stride, 
        unsigned int color_offset,
        unsigned int color_size,
        unsigned int texture_offset,
        unsigned texture_size) {

    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    setup_vbo(VBO, vertices, vertices_len);
    setup_ebo(EBO, indices, indices_len);
    unsigned int pos_offset = 0;
    unsigned int pos_size = 3;
    
    // position attribute
    glVertexAttribPointer(pos_offset, pos_size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    if (color_offset != 0) {
        glVertexAttribPointer(color_offset, color_size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(pos_size*sizeof(float)));
        glEnableVertexAttribArray(color_offset);
    }

    // texture attribute
    std::cout << "texture offset " << texture_offset << " texture size " << texture_size << " start position " << pos_size + color_size << std::endl; 
    if (texture_offset != 0) {
        glVertexAttribPointer(texture_offset, texture_size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)((pos_size + color_size)*sizeof(float)));
        glEnableVertexAttribArray(texture_offset);
    }
    
    return VAO;
}

void draw_triangle(GLuint VAO, Shader shader) {
    float timeValue = glfwGetTime();
    shader.use();
    shader.setFloat("ourOffset", timeValue / 10);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

void draw_vertices(GLuint VAO, Shader shader, unsigned int* textures, float t1t2alpha) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    shader.use();

    // Control which texture appears more
    shader.setFloat("alpha", t1t2alpha);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}
std::string get_asset_path(std::string sub_path) {
    std::string ROOT_DIR = "/Users/gabrielwomark/dev/GabbyOpenGL";
    return ROOT_DIR + "/" + sub_path;
}

int main()
{
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << vec.x << vec.y << vec.z << std::endl;


    Window window;
    Shader shader = Shader(
        get_asset_path("shaders/vertex.glsl").c_str(),
        get_asset_path("shaders/frag_shader.glsl").c_str()
    );
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    GLuint VAO = setup_vao_w_color_texture(vertices, sizeof(vertices), indices, sizeof(indices), 8, 1, 3, 2, 2);

    
    // TEXTURE STUFF
    unsigned int texture1;
    unsigned int texture2;

    // GENERATE AND ADD FIRST TEXTURE TO GL_TEXTURE0
    // takes in the number of textures and takes in an unsinged int * as an array
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture filtering and wrapping options on the currently bound texture
    // here we're setting the wrap option to repeat along the horizontal and vertical dimensions
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // load and generate texture
    int w, h, nrChannels;
    unsigned char* data = stbi_load(get_asset_path("images/container.jpg").c_str(), &w, &h, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // ADD SECOND TEXTURE TO GL_TEXTURE1
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(get_asset_path("images/smiley.png").c_str(), &w, &h, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    unsigned int textures[] = {texture1, texture2};
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    float t1t2alpha = 0.2;
    float x = 0.2;

    // Loop until the user closes the window
    while(!glfwWindowShouldClose(window.get_window()))
    {
        window.process_input(t1t2alpha, x);
        
        // Render here!
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        draw_vertices(VAO, shader, textures, t1t2alpha);
        
        // Swap front and back buffers
        glfwSwapBuffers(window.get_window());

        // Poll for and process events
        glfwPollEvents();
    }

    return 0;
}
