#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <spdlog/spdlog.h>

#include "camera.hpp"
#include "input.hpp"
#include "shader.hpp"

Camera::Camera(Shader* shader, Input* input): shader_{shader}, input_{input} {};

auto Camera::setup() -> void
{
    auto proj = glm::mat4{1.0F};
    GLfloat field_of_view = 75.0F; // degrees
    GLfloat aspect_ratio = 1920.0F / 1080.0F;

    proj = glm::perspective(field_of_view, aspect_ratio, 0.001F, 1000.0F);

    shader_->send_uniform_data("proj", proj);
}

auto Camera::update() -> void
{
    /*
    // Disable mouse countrol
    if (input_->mouse_motion_x()) {
        // rotation_about_y_ = -(static_cast<float>(input->mouse_pos_x()) - 960.0f) * 0.0025f;
        rotation_about_y_ -= static_cast<float>(input_->mouse_motion_x()) * 0.001F;
    }

    if (input_->mouse_motion_y()) {
        // rotation_about_x_ = -(static_cast<float>(input->mouse_pos_y()) - 540.0f) * 0.0025f;
        rotation_about_x_ -= static_cast<float>(input_->mouse_motion_y()) * 0.001F;
    }
    */

    if (!input_->pressed_keys().empty()) {
        bool w_is_pressed = input_->pressed_keys().count('w') > 0;
        bool a_is_pressed = input_->pressed_keys().count('a') > 0;
        bool s_is_pressed = input_->pressed_keys().count('s') > 0;
        bool d_is_pressed = input_->pressed_keys().count('d') > 0;

        if (w_is_pressed) {
            z_pos_ += 0.01F;
        }
        if (s_is_pressed) {
            z_pos_ -= 0.01F;
        }
        if (a_is_pressed) {
            x_pos_ -= 0.01F;
        }
        if (d_is_pressed) {
            x_pos_ += 0.01F;
        }
    }

    auto view = glm::mat4{1.0F};
    view = glm::rotate(view, rotation_about_x_, glm::vec3{1.0F, 0.0F, 0.0F});
    view = glm::rotate(view, rotation_about_y_, glm::vec3{0.0F, 1.0F, 0.0F});
    view = glm::translate(view, glm::vec3{x_pos_, 0.0F, z_pos_});
    shader_->send_uniform_data("view", view);
}
