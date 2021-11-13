//
//  main.cpp
//  GabbyOpenGL
//
//  Created by Gabriel Womark on 5/16/21.
//

#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Window.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
void draw_triangle();

/*
Vertices and indices for drawing rectangle.
Vertices defines what points to be drawn,
indices specifies what indexes of the vertex
buffer apply to which triangles
 
Also note that even though this is just a 1d array
each group of 3 points is a vertex in 3d space
 
float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
};

 
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};
 
*/

/* Draw two separate triangles with one VAO

float vertices[] = {
    -0.50f, -0.5f, 0.0f,  // bottom left of first triangle
     0.00f, -0.5f, 0.0f,  // bottom right of first triangle, bottom left of second
     0.50f, -0.5f, 0.0f,  // bottom right of second triangle
    -0.25f,  0.5f, 0.0f,  // top of first triangle
     0.25f,  0.5f, 0.0f   // top of second triangle
};

unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 4  // second triangle
};
*/

float vertices_one[] = {
    -0.50f, -0.5f, 0.0f,  // bottom left of first triangle
     0.00f, -0.5f, 0.0f,  // bottom right of first triangle, bottom left of second
    -0.25f,  0.5f, 0.0f,  // top of first triangle
};

unsigned int indices[] = {
    0, 1, 2 // first triangle
};


float vertices_two[] = {
     0.00f, -0.5f, 0.0f,  //  bottom left of second
     0.50f, -0.5f, 0.0f,  // bottom right of second triangle
     0.25f,  0.5f, 0.0f   // top of second triangle
};


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


const char* read_shader_source(std::string file, std::string* contents) {
    std::ifstream t(file);
    std::string text((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());
    *contents = text;
    return contents->c_str();
}



void check_shader_compilation(GLuint shader) {
    // Check Success
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void check_shader_program(GLuint program) {
    // Check Success
    int  success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
}


GLuint setup_vertex_shader(const char* source) {
    // Create and compile vertex shader
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &source, NULL);
    glCompileShader(vertexShader);
    check_shader_compilation(vertexShader);
    
    return vertexShader;

}

GLuint setup_fragment_shader(const char* source) {
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &source, NULL);
    glCompileShader(fragmentShader);
    check_shader_compilation(fragmentShader);
    
    return fragmentShader;
}


GLuint build_shader_program(
                            const char* vertex_sharder_source,
                            const char* fragment_shader_source) {
    GLuint vertexShader = setup_vertex_shader(vertex_sharder_source);
    GLuint fragmentShader = setup_fragment_shader(fragment_shader_source);
    
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    
    // attach shaders
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    
    // link shader program
    glLinkProgram(shaderProgram);
    check_shader_program(shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}


GLuint setup_vao(float vertices[], size_t vertices_len, unsigned int indices[], size_t indices_len) {
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    setup_vbo(VBO, vertices, vertices_len);
    setup_ebo(EBO, indices, indices_len);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    return VAO;
}

void draw_triangle(GLuint VAO, GLuint shaderProgram) {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

int main()
{
    Window window;
    std::string vertex_source = "";
    std::string frag_orange_source = "";
    std::string frag_yellow_source = "";
    const char * vertex_shader_source = read_shader_source("/Users/gabrielwomark/dev/GabbyOpenGL/shaders/vertex.txt", &vertex_source);
    const char * frag_shader_orange_source = read_shader_source("/Users/gabrielwomark/dev/GabbyOpenGL/shaders/frag_shader_orange.txt", &frag_orange_source);
    const char * frag_shader_yellow_source = read_shader_source("/Users/gabrielwomark/dev/GabbyOpenGL/shaders/frag_shader_yellow.txt", &frag_yellow_source);
    
    std::cout << vertex_source << std::endl;
    
    GLuint VAO_ONE = setup_vao(vertices_one, sizeof(vertices_one), indices, sizeof(indices));
    GLuint VAO_TWO = setup_vao(vertices_two, sizeof(vertices_two), indices, sizeof(indices));
    GLuint shaderProgram = build_shader_program(vertex_shader_source, frag_shader_orange_source);
    GLuint shaderProgramYellow = build_shader_program(vertex_shader_source, frag_shader_yellow_source);
    

    // Loop until the user closes the window
    while(!glfwWindowShouldClose(window.get_window()))
    {
        window.process_input();
        
        // Render here!
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        draw_triangle(VAO_ONE, shaderProgram);
        draw_triangle(VAO_TWO, shaderProgramYellow);
        
        // Swap front and back buffers
        glfwSwapBuffers(window.get_window());

        // Poll for and process events
        glfwPollEvents();
    }

    return 0;
}
