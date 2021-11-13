//
//  Window.h
//  GabbyOpenGL
//
//  Created by Gabriel Womark on 5/16/21.
//

#ifndef Window_h
#define Window_h
#include <glad/glad.h>
#include <glfw3.h>

class Window {
public:
    Window();
    ~Window();
    void window_update();
    void window_destroy();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void process_input(float& alpha, float& x);
    static void key_callback(GLFWwindow* window0, GLint key, GLint scancode, GLint action, GLint mods);
    GLFWwindow* get_window();
private:
    GLFWwindow* window;
    GLuint polygon_mode;
    void toggle_polygon_mode();
    
};

#endif /* Window_h */
