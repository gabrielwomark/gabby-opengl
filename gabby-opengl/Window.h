//
//  Window.h
//  GabbyOpenGL
//
//  Created by Gabriel Womark on 5/16/21.
//

#ifndef Window_h
#define Window_h
#include <glad/glad.h>
#include <stdlib.h>

#include <iostream>

#include "glfw3.h"

class Window {
   public:
    float currentFrame, deltaTime, lastFrame = 0.0f;
    float ticks = 0.0f;
    Window() : polygon_mode{GL_FILL} {
        if (!glfwInit()) {
            std::exit(-1);
        }

        // Define version and compatibility settings
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

        if (!window) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            std::exit(-1);
        }

        // Mathe the window's context current
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, this->framebuffer_size_callback);

        // Initialize the OpenGL API with GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            std::exit(-1);
        }

        this->disable_cursor();
        glfwSetKeyCallback(window, this->key_callback);
    }

    ~Window() { glfwTerminate(); }

    void disable_cursor() {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void set_mouse_callback(GLFWcursorposfun callback) {
        glfwSetCursorPosCallback(window, callback);
    }
    static void framebuffer_size_callback(GLFWwindow* window, int width,
                                          int height) {
        glViewport(0, 0, width, height);
    }

    static void key_callback(GLFWwindow* window, GLint key, GLint scancode,
                             GLint action, GLint mods) {
        if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
            GLint polygon_mode;
            glGetIntegerv(GL_POLYGON_MODE, &polygon_mode);
            if (polygon_mode == GL_LINE) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
        }
    };

    GLFWwindow* get_window() { return window; }

    void tick() {
        // calculate camera speed
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        ++ticks;
    }

   private:
    GLFWwindow* window;
    GLuint polygon_mode;
    void toggle_polygon_mode();
};

extern Window window;

Window window;
#endif /* Window_h */
