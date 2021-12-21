#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>
#include <string>

#include "vbo.h"

class VAO {
   public:
    VAO() { glGenVertexArrays(1, &handle); }
    ~VAO() { glDeleteVertexArrays(1, &handle); }
    void bind() { glBindVertexArray(handle); }
    void attr(std::string name, VBO& vbo, GLuint index, GLint size, GLenum type,
              GLsizei stride, size_t offset) {
        this->bind();
        vbo.bind();
        glVertexAttribPointer(index, size, type, GL_FALSE, stride,
                              (void*)offset);
        glEnableVertexAttribArray(index);
    }

   private:
    GLuint handle;
};
#endif
