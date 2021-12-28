#ifndef WORLD_H
#define WORLD_H

#include <algorithm>
#include <glm/glm/ext/vector_float3.hpp>
#include <glm/glm/gtx/string_cast.hpp>
#include <vector>

#include "Block.h"
#include "Shader.h"
#include "glm/glm/gtx/string_cast.hpp"
#include "util.h"
#include "vao.h"

extern const size_t world_zero = 64;
const size_t CHUNK_HEIGHT = 16;  // 16;
const size_t CHUNK_WIDTH = 16;   // 16;
const size_t CHUNK_DEPTH = 16;
const size_t CHUNK_VOLUME = CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT;
const size_t FACE_BYTES = 30 * sizeof(float);
const size_t FACE_VERTICES = 30;
const size_t NUM_FACES = 6;

glm::vec3 DEFAULT_POSITION = {-1, -1, -1};

std::vector<glm::vec3> dirs_to_check = {
    {0, 0, -1},  // NORTH
    {0, 0, 1},   // SOUTH
    {-1, 0, 0},  // WEST
    {1, 0, 0},   // EAST
    {0, -1, 0},  // BOTTOM
    {0, 1, 0}    // TOP
};

enum Faces { FRONT, BACK, R, L, D, U };
float vertices[] = {
    // NORTH
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // 0
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,   // 4
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,    // 6
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,    // 6
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // 2
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // 0
                                      // SOUTH
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // 1
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,    // 5
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,     // 7
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,     // 7
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,    // 3
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // 1
                                      // WEST
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // 3
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // 2
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // 0
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // 0
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // 1
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // 3
                                      // EAST
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,     // 7
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,    // 6
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,   // 4
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,   // 4
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,    // 5
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,     // 7
                                      // BOTTOM
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // 0
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,   // 4
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,    // 5
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,    // 5
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // 1
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // 0
                                      // TOP
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // 2
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,    // 6
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,     // 7
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,     // 7
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,    // 3
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // 2

};

class World {
   public:
    Block block;
    unsigned int ticks;
    VAO vao;
    VBO vbo;
    std::vector<float> chunk_vertices;
    std::vector<float>::iterator vertex_iter;
    Shader chunk_shader;
    Texture chunk_texture;

    World()
        : ticks{0},
          vbo{GL_ARRAY_BUFFER, false},
          chunk_vertices{180, 0},
          chunk_shader(get_asset_path("shaders/vertex.glsl").c_str(),
                       get_asset_path("shaders/frag_shader.glsl").c_str()),
          chunk_texture("images/terrain.png", GL_RGBA) {
        chunk_vertices.resize(CHUNK_VOLUME * 180);
        vertex_iter = chunk_vertices.begin();
        m_positions.reserve(10);
        for (size_t y = 0; y < CHUNK_HEIGHT; y++) {
            for (size_t z = 0; z < CHUNK_DEPTH; z++) {
                for (size_t x = 0; x < CHUNK_WIDTH; x++) {
                    if (y % 4 == 2) {
                        m_positions.emplace_back(DEFAULT_POSITION);
                    } else {
                        auto position = glm::vec3(x, y, z);
                        m_positions.push_back(position);
                    }
                }
            }
        }
        // set up vertex attributes, shader and texture
        // position attributes
        vao.attr("position", vbo, 0, 3, GL_FLOAT, 5 * sizeof(float), 0);
        // texture attributes
        vao.attr("texture", vbo, 1, 2, GL_FLOAT, 5 * sizeof(float),
                 3 * sizeof(float));

        chunk_shader.use();
        chunk_shader.setInt("block_texture", 0);
        chunk_texture.bind_texture();
    }

    void prepare() {}

    void update() { ++ticks; }

    void render() {
        size_t num_vertices = 0;

        for (auto position : m_positions) {
            if (position != DEFAULT_POSITION) {
                for (size_t face = 0; face < NUM_FACES; face++) {
                    glm::vec3 position_to_check =
                        position + dirs_to_check[face];
                    if (!block_in_position(position_to_check)) {
                        for (size_t face_row = 0; face_row < 6; face_row++) {
                            size_t face_offset = face_row * 5 + face * 30;
                            // x
                            std::transform(
                                &vertices[face_offset],
                                &vertices[face_offset + 1], vertex_iter,
                                [&](float x) { return x + position.x; });
                            ++vertex_iter;

                            // y
                            std::transform(
                                &vertices[face_offset + 1],
                                &vertices[face_offset + 2], vertex_iter,
                                [&](float y) { return y + position.y; });
                            ++vertex_iter;

                            // z
                            std::transform(
                                &vertices[face_offset + 2],
                                &vertices[face_offset + 3], vertex_iter,
                                [&](float z) { return z + position.z; });
                            ++vertex_iter;

                            // texture x
                            std::transform(
                                &vertices[face_offset + 3],
                                &vertices[face_offset + 4], vertex_iter,
                                [&](float tex_x) { return tex_x / 16; });
                            ++vertex_iter;

                            // texture z
                            std::transform(
                                &vertices[face_offset + 4],
                                &vertices[face_offset + 5], vertex_iter,
                                [&](float tex_y) { return tex_y / 16; });
                            ++vertex_iter;
                        }

                        // update number of vertices copied to buffer
                        num_vertices += 30;
                    }
                }
            }
        }

        chunk_shader.use();
        vao.bind();
        vbo.buffer(static_cast<void*>(chunk_vertices.data()),
                   num_vertices * sizeof(float));
        glDrawArrays(GL_TRIANGLES, 0, num_vertices / 5);
        std::fill_n(chunk_vertices.begin(), CHUNK_VOLUME * 180, 0);
        vertex_iter = chunk_vertices.begin();
    }

    bool block_in_position(glm::vec3& position_to_check) {
        if (position_to_check.x < 0 || position_to_check.y < 0 ||
            position_to_check.z < 0) {
            return false;
        }
        if (position_to_check.x >= CHUNK_WIDTH ||
            position_to_check.y >= CHUNK_HEIGHT ||
            position_to_check.z >= CHUNK_DEPTH) {
            return false;
        }
        size_t index_to_check = position_to_index(position_to_check);
        glm::vec3& position = m_positions[index_to_check];
        if (position != DEFAULT_POSITION) {
            return true;
        }
        return false;
    }

    static size_t position_to_index(glm::vec3& position) {
        return (position.y * CHUNK_DEPTH * CHUNK_WIDTH) +
               (position.z * CHUNK_WIDTH) + position.x;
    }

   private:
    std::vector<glm::vec3> m_positions;
};

#endif
