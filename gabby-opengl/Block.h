#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm/ext/vector_int2.hpp"
#include "vao.h"

class World;
enum BlockId { AIR, GRASS };
#define MAX_BLOCK_ID GRASS

class Block {
    bool transparent;
    bool solid;
    BlockId id;
    VAO vao;
    VBO vbo;

   public:
    Block() : id(GRASS), vbo(GL_ARRAY_BUFFER, false){};

    Block(bool transparent) : id(GRASS), vbo(GL_ARRAY_BUFFER, false){};

    glm::ivec2 (*get_texture_location)();
};

#endif
