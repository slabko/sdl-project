#ifndef PLAYGROUND_SHADER_HPP
#define PLAYGROUND_SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Shader {
public:
    Shader() = default;

    auto create_program() -> bool;
    auto create_shaders() -> bool;

    auto compile_vertex_shader(std::string const& filename) -> bool;
    auto compile_fragment_shader(std::string const& filename) -> bool;
    auto attach_shaders() -> void;
    auto link_program() -> bool;

    auto detach_shaders() -> void;
    auto destroy_shaders() -> void;
    auto destroy_program() -> void;

    auto send_uniform_data(std::string const& name, GLint data) -> void;
    auto send_uniform_data(std::string const& name, glm::mat4 const& data) -> void;

    auto send_uniform_data(std::string const& name, size_t size, std::vector<glm::mat4> const& data) -> void;


    Shader(Shader const&) = delete;
    Shader(Shader&&) = delete;
    auto operator=(Shader const&) -> Shader& = delete;
    auto operator=(Shader&&) -> Shader& = delete;
    ~Shader() = default;

    auto shader_program_id() -> GLuint {
        return shader_program_id_;
    }

private:

    static auto compile_shaders(std::string const& filename, GLuint shader_id) -> bool;
    auto get_uniform_location(std::string const& name) -> GLint;

    GLuint shader_program_id_{};
    GLuint vertex_shader_id_{};
    GLuint fragment_shader_id_{};
};

#endif // PLAYGROUND_SHADER_HPP
