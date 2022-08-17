#include <SDL2/SDL_image.h>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/io.hpp>
#include <iostream>
#include <random>

#include "cube.hpp"
#include "shader.hpp"

Cube::Cube(Shader* shader) :
  shader_{shader}, buffer_{shader}
{
    // clang-format off
    std::array<GLfloat, 288> vertices{
      //     Vertices             Colors             UV      objectId //
      -0.5F,  0.5F,  0.5F,   1.0F, 1.0F, 1.0F,   0.0F, 0.0F,   1.0F,
       0.5F,  0.5F,  0.5F,   1.0F, 1.0F, 1.0F,   1.0F, 0.0F,   1.0F,
      -0.5F, -0.5F,  0.5F,   1.0F, 1.0F, 1.0F,   0.0F, 1.0F,   1.0F,
       0.5F, -0.5F,  0.5F,   1.0F, 1.0F, 1.0F,   1.0F, 1.0F,   1.0F,
       0.5F,  0.5F,  0.5F,   1.0F, 1.0F, 1.0F,   0.0F, 0.0F,   1.0F,
       0.5F,  0.5F, -0.5F,   1.0F, 1.0F, 1.0F,   1.0F, 0.0F,   1.0F,
       0.5F, -0.5F,  0.5F,   1.0F, 1.0F, 1.0F,   0.0F, 1.0F,   1.0F,
       0.5F, -0.5F, -0.5F,   1.0F, 1.0F, 1.0F,   1.0F, 1.0F,   1.0F,
      -0.5F,  0.5F, -0.5F,   1.0F, 1.0F, 1.0F,   0.0F, 0.0F,   1.0F,
      -0.5F,  0.5F,  0.5F,   1.0F, 1.0F, 1.0F,   1.0F, 0.0F,   1.0F,
      -0.5F, -0.5F, -0.5F,   1.0F, 1.0F, 1.0F,   0.0F, 1.0F,   1.0F,
      -0.5F, -0.5F,  0.5F,   1.0F, 1.0F, 1.0F,   1.0F, 1.0F,   1.0F,
      -0.5F,  0.5F, -0.5F,   1.0F, 1.0F, 1.0F,   0.0F, 0.0F,   1.0F,
       0.5F,  0.5F, -0.5F,   1.0F, 1.0F, 1.0F,   1.0F, 0.0F,   1.0F,
      -0.5F,  0.5F,  0.5F,   1.0F, 1.0F, 1.0F,   0.0F, 1.0F,   1.0F,
       0.5F,  0.5F,  0.5F,   1.0F, 1.0F, 1.0F,   1.0F, 1.0F,   1.0F,
      -0.5F, -0.5F,  0.5F,   1.0F, 1.0F, 1.0F,   0.0F, 0.0F,   1.0F,
       0.5F, -0.5F,  0.5F,   1.0F, 1.0F, 1.0F,   1.0F, 0.0F,   1.0F,
      -0.5F, -0.5F, -0.5F,   1.0F, 1.0F, 1.0F,   0.0F, 1.0F,   1.0F,
       0.5F, -0.5F, -0.5F,   1.0F, 1.0F, 1.0F,   1.0F, 1.0F,   1.0F,
      -0.5F, -0.5F, -0.5F,   1.0F, 1.0F, 1.0F,   0.0F, 0.0F,   1.0F,
       0.5F, -0.5F, -0.5F,   1.0F, 1.0F, 1.0F,   1.0F, 0.0F,   1.0F,
      -0.5F,  0.5F, -0.5F,   1.0F, 1.0F, 1.0F,   0.0F, 1.0F,   1.0F,
       0.5F,  0.5F, -0.5F,   1.0F, 1.0F, 1.0F,   1.0F, 1.0F,   1.0F,
    };
    // clang-format on

    buffer_.create_buffer();

    // buffer_.fill_vbo(vertices.data(), sizeof(vertices), Buffer::MULTIPLE);
    buffer_.alloc_vbo(sizeof(vertices) * 10);
    buffer_.fill_vbo(vertices.data(), 0, sizeof(vertices));

    buffer_.link_vbo("vertexIn", Buffer::XYZ, Buffer::FLOAT, 9 * sizeof(float), 0);
    buffer_.link_vbo("colorIn", Buffer::RGB, Buffer::FLOAT, 9 * sizeof(float), 12);
    buffer_.link_vbo("textureIn", Buffer::UV, Buffer::FLOAT, 9 * sizeof(float), 24);
    buffer_.link_vbo("objectId", Buffer::ONE, Buffer::FLOAT, 9 * sizeof(float), 32);

    // clang-format off
    std::array<GLuint, 36> indices = {
       0,  1,  2,   1,  2,  3,
       4,  5,  6,   5,  6,  7,
       8,  9, 10,   9, 10, 11,
      12, 13, 14,  13, 14, 15,
      16, 17, 18,  17, 18, 19,
      20, 21, 22,  21, 22, 23,
    };
    // clang-format on

    buffer_.fill_ibo(indices.data(), sizeof(indices), Buffer::SINGLE);
}

auto Cube::render() -> void
{
    shader_->send_uniform_data("models[0]", glm::mat4{1.0F});

    model_ = glm::mat4{1.0F};
    model_ = glm::rotate(model_, rotation_about_y_, glm::vec3{0.0F, 1.0F, 0.0F});
    model_ = glm::rotate(model_, rotation_about_x_, glm::vec3{1.0F, 0.0F, 0.0F});
    shader_->send_uniform_data("models[1]", model_);


    texture_.bind();
    buffer_.render(Buffer::TRIANGLES, 36);
    texture_.unbind();
}

Cube::~Cube()
{
    buffer_.destroy_buffer();
}
