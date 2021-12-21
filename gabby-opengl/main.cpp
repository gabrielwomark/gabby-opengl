//
//  main.cpp
//  GabbyOpenGL
//
//  Created by Gabriel Womark on 5/16/21.
//

// standard libs
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "math.h"

// GL libs
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

// third party libs
#include <stb/stb_image.h>

// My libs
#include "Camera.h"
#include "Shader.h"
#include "State.h"
#include "Texture.h"
#include "Window.h"
#include "vao.h"
#include "vbo.h"

struct State state;

// camera globals
const unsigned int SRC_WIDTH = 800;
const unsigned int SRC_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));
float lastX = float(SRC_WIDTH) / 2;
float lastY = float(SRC_HEIGHT) / 2;
bool firstMouse;

enum ImageFormat { JPG, PNG };

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f),
    glm::vec3(2.0f, 0.0f, 2.0f)};
/* ,   glm::vec3(2.0f, 5.0f, -15.0f),
             glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
             glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
             glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
             glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)}; */

void draw_vertices(VAO& vao, Shader shader, std::vector<Texture>& textures) {
    // bind textures
    for (int i = 0; i < textures.size(); ++i) {
        textures[i].bind_texture();
    }
    // use shaders before setting uniforms
    shader.use();

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f), SRC_WIDTH / (float)SRC_HEIGHT, 0.1f, 100.0f);
    shader.setMat4fv("view", view);
    shader.setMat4fv("projection", projection);

    // Control which texture appears more
    vao.bind();
    for (unsigned int i = 0; i < 5; i++) {
        // matrix transform
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        shader.setMat4fv("model", model);

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

std::string get_asset_path(std::string sub_path) {
    std::string ROOT_DIR = "/Users/gabrielwomark/dev/GabbyOpenGL";
    return ROOT_DIR + "/" + sub_path;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseInput(xoffset, yoffset);
}

void process_input(Window& window) {
    GLFWwindow* w = window.get_window();
    if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(w, true);
    }

    if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboardInput(FORWARD, window.deltaTime);
    }
    if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboardInput(BACKWARD, window.deltaTime);
    }
    if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboardInput(RIGHT, window.deltaTime);
    }
    if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboardInput(LEFT, window.deltaTime);
    }
    if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.ProcessKeyboardInput(UP, window.deltaTime);
    }
    if (glfwGetKey(w, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.ProcessKeyboardInput(DOWN, window.deltaTime);
    }
}

int main() {
    state.window = &window;
    state.window->disable_cursor();
    state.window->set_mouse_callback(mouse_callback);
    Shader shader = Shader(get_asset_path("shaders/vertex.glsl").c_str(),
                           get_asset_path("shaders/frag_shader.glsl").c_str());

    VAO vao;
    VBO vbo(GL_ARRAY_BUFFER, false);
    vbo.buffer(vertices, 180 * sizeof(float));

    // position attributes
    vao.attr("position", vbo, 0, 3, GL_FLOAT, 5 * sizeof(float), 0);
    // texture attributes
    vao.attr("texture", vbo, 1, 2, GL_FLOAT, 5 * sizeof(float),
             3 * sizeof(float));

    // TEXTURE STUFF
    Texture texture1 = Texture("images/container.jpg", GL_RGB);
    Texture texture2 = Texture("images/smiley.png", GL_RGBA);

    std::vector<Texture> textures = {texture1, texture2};
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
    // END TEXTURE STUFF

    // enable depth testing so things render properly
    glEnable(GL_DEPTH_TEST);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(state.window->get_window())) {
        state.window->tick();

        process_input(*(state.window));

        // Render here!
        glClearColor(0.49f, 0.75f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw_vertices(vao, shader, textures);

        // Swap front and back buffers
        glfwSwapBuffers(state.window->get_window());

        // Poll for and process events
        glfwPollEvents();
    }

    return 0;
}
