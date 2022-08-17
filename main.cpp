#include <array>
#include <chrono>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>
#include <spdlog/spdlog.h>
#include <thread>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

#include "camera.hpp"
#include "cube.hpp"
#include "input.hpp"
#include "screen.hpp"
#include "shader.hpp"

auto main(int /*argc*/, char* /*argv*/[]) -> int
{
    bool is_app_running = true;

    auto* screen = new Screen{1920, 1080};

    auto init_res = screen->initialize();
    if (!init_res) {
        return -1;
    }

    auto* shader = new Shader();

    bool create_program_success = shader->create_program();
    if (!create_program_success) {
        spdlog::critical("Failed to create program");
        return -1;
    }

    bool create_shaders_success = shader->create_shaders();
    if (!create_shaders_success) {
        spdlog::critical("Failed to create shaders");
        return -1;
    }

    bool compile_vertex_buffer_success = shader->compile_vertex_shader("shaders/main.vert");
    if (!compile_vertex_buffer_success) {
        spdlog::critical("Failed to compile vertex shaders");
        return -1;
    }

    bool compile_fragment_buffer_success = shader->compile_fragment_shader("shaders/main.frag");
    if (!compile_fragment_buffer_success) {
        spdlog::critical("Failed to compile fragment shaders");
        return -1;
    }

    shader->attach_shaders();

    bool link_program_success = shader->link_program();
    if (!link_program_success) {
        spdlog::critical("Failed to link the program");
        return -1;
    }

    auto* input = new Input{};
    auto* camera = new Camera(shader, input);
    camera->setup();
    Cube cube{shader};
    bool show_cube = true;
    bool animate_cube = false;

    std::array<float, 2> cube_rotation{0.0F, 0.0F};

    while (is_app_running) {

        screen->clear_screen();
        screen->start_frame();

        {
            ImGui::Begin("Configuration");
            ImGui::Text("Show the animated cube");
            ImGui::Checkbox("Show Cube", &show_cube);
            ImGui::Checkbox("Animate Cube", &animate_cube);
            ImGui::Text("Application average %.1f FPS", ImGui::GetIO().Framerate);
            ImGui::SliderFloat2("Rotation", cube_rotation.data(), 0, 2*M_PI);
            ImGui::End();
        }

        input->update();
        is_app_running = !input->is_x_clicked();

        if (!input->pressed_keys().empty()) {
            bool w_is_pressed = input->pressed_keys().count('w') > 0;

            bool lctrl_is_pressed = input->pressed_keys().count(SDLK_LCTRL) > 0;
            bool rctrl_is_pressed = input->pressed_keys().count(SDLK_RCTRL) > 0;
            bool ctrl_is_pressed = lctrl_is_pressed || rctrl_is_pressed;

            if (ctrl_is_pressed && w_is_pressed) {
                is_app_running = false;
            }
        }

        if (animate_cube) {
            cube_rotation[0] = std::fmod(cube_rotation[0] + 0.01F, 2*M_PI);
            cube_rotation[1] = std::fmod(cube_rotation[1] + 0.001F, 2*M_PI);
        }

        cube.set_rotation_about_x(cube_rotation[0]);
        cube.set_rotation_about_y(cube_rotation[1]);

        camera->update();
        if (show_cube) {
            cube.render();
        }

        screen->present();
    }

    shader->detach_shaders();
    shader->destroy_shaders();
    shader->destroy_program();
    screen->shutdown();
}
