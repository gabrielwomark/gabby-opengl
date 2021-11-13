#ifndef VAO_H
#define VAO_H
#include <glad/glad.h>
#include <glfw3.h>

class VAO {
    public:
        GLuint ID;
        GLuint vboID;
        GLuint eboID;

        VAO(float vertices [], size_t vertices_len, unsigned int indices[], size_t indices_len);
};

#endif
