#ifndef PLAYGROUND_CAMERA_HPP
#define PLAYGROUND_CAMERA_HPP

#include <array>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "input.hpp"

class Camera {
public:
    Camera(Shader* shader, Input* input);
    Camera(Camera const&) = delete;
    Camera(Camera&&) = delete;
    auto operator=(Camera const&) -> Camera& = delete;
    auto operator=(Camera&&) -> Camera& = delete;
    ~Camera() = default;

    auto update() -> void;
    auto setup() -> void;

private:

    Shader* shader_;
    Input* input_;

    float x_pos_{};
    float z_pos_{-10.0F};

    float rotation_about_y_{};
    float rotation_about_x_{};
};

#endif // PLAYGROUND_CAMERA_HPP
