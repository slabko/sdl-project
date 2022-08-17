#include "input.hpp"
#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>

#include "imgui_impl_sdl.h"

auto Input::is_x_clicked() const -> bool
{
    return is_x_clicked_;
}

auto Input::is_mouse_button_down() const -> bool
{
    return is_mouse_button_down_;
}

auto Input::mouse_motion_x() const -> int
{
    return mouse_motion_x_;
}

auto Input::mouse_motion_y() const -> int
{
    return mouse_motion_y_;
}

auto Input::mouse_pos_x() const -> int
{
    return mouse_pos_x_;
}

auto Input::mouse_pos_y() const -> int
{
    return mouse_pos_y_;
}

auto Input::pressed_keys() const -> std::unordered_set<SDL_Keycode> const&
{
    return pressed_keys_;
}

auto Input::update() -> void
{
    SDL_Event e;

    mouse_motion_x_ = 0;
    mouse_motion_y_ = 0;

    while (SDL_PollEvent(&e)) {
        ImGui_ImplSDL2_ProcessEvent(&e);
        switch (e.type) {
        case SDL_KEYDOWN:
            // spdlog::info("a key is pressed: {}", e.key.keysym.sym);
            pressed_keys_.insert(e.key.keysym.sym);
            break;
        case SDL_KEYUP:
            // spdlog::info("a key is released: {}", e.key.keysym.sym);
            pressed_keys_.erase(e.key.keysym.sym);
            break;
        case SDL_QUIT:
            spdlog::info("SDL_QUIT event");
            is_x_clicked_ = true;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_LEFT) {
                is_mouse_button_down_ = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (e.button.button == SDL_BUTTON_LEFT) {
                is_mouse_button_down_ = false;
            }
            break;
        case SDL_MOUSEMOTION:
            mouse_motion_x_ = e.motion.xrel;
            mouse_motion_y_ = e.motion.yrel;
            mouse_pos_x_ = e.motion.x;
            mouse_pos_y_ = e.motion.y;
            break;
        }
    }
}
