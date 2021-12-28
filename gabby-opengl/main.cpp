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
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "World.h"
#include "glm/glm/fwd.hpp"
#include "math.h"

// GL libs
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

// third party libs
#include <stb/stb_image.h>

// My libs
#include "Block.h"
#include "BlockAtlas.h"
#include "Camera.h"
#include "Shader.h"
#include "State.h"
#include "Texture.h"
#include "Window.h"
#include "util.h"
#include "vao.h"
#include "vbo.h"

struct State state;

Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));
float lastX = float(SRC_WIDTH) / 2;
float lastY = float(SRC_HEIGHT) / 2;
bool firstMouse;

enum ImageFormat { JPG, PNG };

float triangle_vertices[]{
    -0.5f, -0.5f, -0.5f,  // 0
    -0.5f, -0.5f, 0.5f,   // 1
    -0.5f, 0.5f,  -0.5f,  // 2
    -0.5f, 0.5f,  0.5f,   // 3
    0.5f,  -0.5f, -0.5f,  // 4
    0.5f,  -0.5f, 0.5f,   // 5
    0.5f,  0.5f,  -0.5f,  // 6
    0.5f,  0.5f,  0.5f,   // 7
};

float texture_vertices[]{};

float triangle_indices[]{
    0, 4, 6, 6, 2, 0,  // NORTH
    1, 5, 7, 7, 3, 1,  // SOUTH
    3, 2, 0, 0, 1, 3,  // WEST
    7, 6, 4, 4, 5, 7,  // EAST
    0, 4, 5, 5, 1, 0,  // BOTTOM

};

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
    if (glfwGetKey(w, GLFW_KEY_P) == GLFW_PRESS) {
        window.enable_cursor();
    }
}

#define BLOCK_ATLAS_FRAMES = 16

// struct Blockatlas {
//     Blockatlas() {
//         texture = std::make_unique<Texture>("images/terrain.png", GL_RGBA);
//     }
//
//     std::unique_ptr<Texture> texture;
// };

int main() {
    state.window = &window;
    state.window->disable_cursor();
    state.window->set_mouse_callback(mouse_callback);

    World world;

    // enable depth testing so things render properly
    glEnable(GL_DEPTH_TEST);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(state.window->get_window())) {
        state.window->tick();

        process_input(*(state.window));

        // Render here!
        glClearColor(0.49f, 0.75f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.set_view_projection_matrix(world.chunk_shader);
        world.render();
        world.update();
        //    Swap front and back buffers
        glfwSwapBuffers(state.window->get_window());

        // Poll for and process events
        glfwPollEvents();
    }

    return 0;
}
