#ifndef PLAYGROUND_BUFFER_HPP
#define PLAYGROUND_BUFFER_HPP

#include <glad/glad.h>
#include <string>
#include <unordered_set>

#include "shader.hpp"

class Buffer {
public:
    enum ComponentType {
        ONE = 1,
        XY = 2,
        XYZ = 3,
        RGB = 3,
        RGBA = 4,
        UV = 2
    };
    enum FillType { SINGLE = GL_STATIC_DRAW,
        MULTIPLE = GL_DYNAMIC_DRAW };

    // NOLINTNEXTLINE(readability-identifier-naming)
    enum DataType { INT = GL_INT,
        FLOAT = GL_FLOAT,
        UNSIGNED_INT = GL_UNSIGNED_INT // NOLINT(readability-identifier-naming)
    };
    enum DrawType {
        POINTS = GL_POINTS,
        TRIANGLES = GL_TRIANGLES,
        LINES = GL_LINES
    };

    explicit Buffer(Shader* shader) :
      shader_{shader} {};

    auto create_buffer() -> void;

    auto alloc_vbo(GLsizeiptr buffer_size) -> void;
    auto fill_vbo(GLfloat* data, GLintptr offset, GLsizeiptr buffer_size) -> void;
    auto fill_vbo(GLfloat* data, GLsizeiptr buffer_size, FillType fill_type) -> void;
    auto fill_ibo(GLuint* data, GLsizeiptr buffer_size, FillType fill_type) -> void;

    auto link_vbo(
      std::string const& attribute,
      ComponentType component_type,
      DataType data_type,
      size_t stride,
      size_t offset) -> void;

    auto render(DrawType draw_type, GLint n_vertices) -> void;

    auto destroy_buffer() -> void;

private:
    auto bind_vbo() -> void;
    auto bind_ibo() -> void;

    Shader* shader_;

    // Vertex Array Object
    GLuint vao_{};

    // VBO - Vertex Buffer Object
    GLuint vbo_{};

    // IBO - Index Buffer Object
    GLuint ibo_{};

    std::unordered_set<std::string> created_attributes_;
};

#endif // PLAYGROUND_BUFFER_HPP
