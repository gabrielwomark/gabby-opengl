#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>
class VBO {
   public:
    VBO(GLint type, bool dynamic) {
        glGenBuffers(1, &handle);
        this->type = type;
        this->dynamic = dynamic;
    }

    ~VBO() { glDeleteBuffers(1, &handle); }
    void bind() { glBindBuffer(type, handle); }

    void buffer(void* data, size_t size) {
        this->bind();
        glBufferData(type, size, data,
                     dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }

   private:
    GLuint handle;
    GLint type;
    bool dynamic;
};
#endif
