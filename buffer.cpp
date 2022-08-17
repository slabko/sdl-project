#include <iostream>

#include "buffer.hpp"
#include "shader.hpp"

auto Buffer::create_buffer() -> void
{
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ibo_);
    glGenVertexArrays(1, &vao_);
}

auto Buffer::alloc_vbo(GLsizeiptr buffer_size) -> void
{
    glBindVertexArray(vao_);
    {
        bind_vbo();
        glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, Buffer::MULTIPLE);
    }
    glBindVertexArray(0);
}

auto Buffer::fill_vbo(GLfloat* data, GLintptr offset, GLsizeiptr buffer_size) -> void
{
    glBindVertexArray(vao_);
    {
        bind_vbo();
        glBufferSubData(GL_ARRAY_BUFFER, offset, buffer_size, data);
    }
    glBindVertexArray(0);
}

auto Buffer::fill_vbo( GLfloat* data, GLsizeiptr buffer_size, Buffer::FillType fill_type) -> void
{
    glBindVertexArray(vao_);
    {
        bind_vbo();
        glBufferData(GL_ARRAY_BUFFER, buffer_size, data, fill_type);
    }
    glBindVertexArray(0);
}

auto Buffer::fill_ibo(GLuint* data, GLsizeiptr buffer_size, Buffer::FillType fill_type) -> void
{
    glBindVertexArray(vao_);
    {
        bind_ibo();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer_size, data, fill_type);
    }
    glBindVertexArray(0);
}

auto Buffer::link_vbo(
  std::string const& attribute,
  Buffer::ComponentType component_type,
  Buffer::DataType data_type,
  size_t stride,
  size_t offset) -> void
{

    auto attribute_id = glGetAttribLocation(shader_->shader_program_id(), attribute.c_str());
    created_attributes_.insert(attribute);

    glBindVertexArray(vao_);
    {
        bind_vbo();
        glVertexAttribPointer(attribute_id, component_type, data_type, GL_FALSE, stride, (const void*)offset);
        glEnableVertexAttribArray(attribute_id);
    }
    glBindVertexArray(0);
}

auto Buffer::render(Buffer::DrawType draw_type, GLint n_vertices) -> void
{
    glBindVertexArray(vao_);
    bind_ibo();
    glDrawElements(draw_type, n_vertices, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

auto Buffer::destroy_buffer() -> void
{
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);

    for (auto const& attribute : created_attributes_) {
        auto attribute_id = glGetAttribLocation(shader_->shader_program_id(), attribute.c_str());
        glDisableVertexAttribArray(attribute_id);
    }
}

inline auto Buffer::bind_vbo() -> void
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
}

inline auto Buffer::bind_ibo() -> void
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
}
