#include <fstream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>

#include "shader.hpp"

auto Shader::create_program() -> bool
{
    shader_program_id_ = glCreateProgram();
    if (!shader_program_id_) {
        spdlog::error("Failed to create a shader program");
        return false;
    }
    return true;
}

auto Shader::create_shaders() -> bool
{
    vertex_shader_id_ = glCreateShader(GL_VERTEX_SHADER);
    if (!vertex_shader_id_) {
        spdlog::error("Failed to create vertex shader object");
        return false;
    }

    fragment_shader_id_ = glCreateShader(GL_FRAGMENT_SHADER);
    if (!fragment_shader_id_) {
        spdlog::error("Failed to create fragment shader object");
        return false;
    }

    return true;
}

auto Shader::compile_shaders(std::string const& filename, GLuint shader_id) -> bool
{
    std::string source_code{};
    {
        std::ifstream file(filename);
        if (!file) {
            spdlog::error("Failed to open shader file {}", filename);
            return false;
        }

        std::stringstream buffer{};
        buffer << file.rdbuf();
        source_code = buffer.str();
    }

    auto const* raw_source_code = reinterpret_cast<GLchar const*>(source_code.c_str());
    glShaderSource(shader_id, 1, &raw_source_code, nullptr);
    glCompileShader(shader_id);

    GLint error_code{};
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &error_code);

    if (error_code != GL_TRUE) {
        GLsizei buffer_size{};
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &buffer_size);
        std::vector<GLchar> buffer(buffer_size);
        glGetShaderInfoLog(shader_id, buffer_size, nullptr, buffer.data());
        spdlog::error("Failed to compile shader: {}", buffer.data());
        return false;
    }

    return true;
}

auto Shader::compile_fragment_shader(std::string const& filename) -> bool
{
    return compile_shaders(filename, fragment_shader_id_);
}
auto Shader::compile_vertex_shader(std::string const& filename) -> bool

{
    return compile_shaders(filename, vertex_shader_id_);
}

auto Shader::attach_shaders() -> void
{
    glAttachShader(shader_program_id_, vertex_shader_id_);
    glAttachShader(shader_program_id_, fragment_shader_id_);
}

auto Shader::link_program() -> bool
{
    glLinkProgram(shader_program_id_);
    glUseProgram(shader_program_id_);

    GLint error_code{};
    glGetProgramiv(shader_program_id_, GL_LINK_STATUS, &error_code);

    if (error_code != GL_TRUE) {
        GLsizei buffer_size{};
        glGetProgramiv(shader_program_id_, GL_INFO_LOG_LENGTH, &buffer_size);
        std::vector<GLchar> buffer(buffer_size);
        glGetProgramInfoLog(shader_program_id_, buffer_size, nullptr, buffer.data());
        spdlog::error("Failed to link shader: {}", buffer.data());
        return false;
    }

    return true;
}

auto Shader::detach_shaders() -> void
{
    glDetachShader(shader_program_id_, vertex_shader_id_);
    glDetachShader(shader_program_id_, fragment_shader_id_);
}

auto Shader::destroy_shaders() -> void
{
    glDeleteShader(vertex_shader_id_);
    glDeleteShader(fragment_shader_id_);
}

auto Shader::destroy_program() -> void
{
    glDeleteProgram(shader_program_id_);
}

auto Shader::get_uniform_location(std::string const& name) -> GLint
{
    GLint id = glGetUniformLocation(shader_program_id_, name.c_str());
    if (id < 0) {
        spdlog::error("Uniform name `{}` is not found", name);
    }
    return id;
}

auto Shader::send_uniform_data(std::string const& name, GLint data) -> void
{
    auto id = get_uniform_location(name);
    glUniform1ui(id, data);
}

auto Shader::send_uniform_data(std::string const& name, glm::mat4 const& data) -> void
{
    auto id = get_uniform_location(name);
    glUniformMatrix4fv(id, 1, GL_FALSE, &data[0][0]);
}

auto Shader::send_uniform_data(std::string const& name, size_t size, std::vector<glm::mat4> const& data) -> void
{
    auto id = get_uniform_location(name);
    glUniformMatrix4fv(id, size, GL_FALSE, reinterpret_cast<GLfloat const*>(data.data()));
}
