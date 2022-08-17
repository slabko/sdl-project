#ifndef PLAYGROUND_CUBE_HPP
#define PLAYGROUND_CUBE_HPP

#include <array>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "buffer.hpp"
#include "texture.hpp"
#include "shader.hpp"

class Cube {
public:
    explicit Cube(Shader *shader);

    Cube(Cube const&) = delete;
    Cube(Cube&&) = delete;
    auto operator=(Cube const&) -> Cube& = delete;
    auto operator=(Cube&&) -> Cube& = delete;
    virtual ~Cube();

    void render();

    auto get_rotation_about_x() -> float {
        return rotation_about_x_;
    }

    auto get_rotation_about_y() -> float {
        return rotation_about_y_;
    }

    void set_rotation_about_x(float value) {
        rotation_about_x_ = value;
    }

    void set_rotation_about_y(float value) {
        rotation_about_y_ = value;
    }

private:
    Shader* shader_;
    Buffer buffer_;
    Texture texture_{"textures/crate.png"};

    glm::mat4 model_{1.0F};

    float rotation_about_x_{};
    float rotation_about_y_{};
};

#endif // PLAYGROUND_CUBE_HPP
