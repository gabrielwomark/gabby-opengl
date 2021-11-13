//
//  Window.cpp
//  GabbyOpenGL
//
//  Created by Gabriel Womark on 5/16/21.
//

#include "Window.h"
#include <iostream>
#include <stdlib.h>

Window::Window() : polygon_mode{GL_FILL} {
    if (!glfwInit()) {
        std::exit(-1);
    }
    
    // Define version and compatibility settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(-1);
    }
    
    // Mathe the window's context current
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, this->framebuffer_size_callback);

    // Initialize the OpenGL API with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::exit(-1);
    }
    
    glfwSetKeyCallback(window, this->key_callback);

}

Window::~Window() {
    glfwTerminate();
}

void Window::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina displays
    glViewport(0, 0, width, height);
}

void Window::process_input() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLFWwindow* Window::get_window() {
    return window;
}

    
void Window::key_callback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mods){
    if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
        GLint polygon_mode;
        glGetIntegerv(GL_POLYGON_MODE, &polygon_mode);
        if (polygon_mode == GL_LINE) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    }
}
