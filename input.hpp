#ifndef PLAYGROUND_INPUT_HPP
#define PLAYGROUND_INPUT_HPP

#include <SDL2/SDL_keycode.h>
#include <unordered_set>

class Input {
public:
    Input() = default;
    Input(Input const&) = delete;
    Input(Input&&) = delete;
    auto operator=(Input const&) -> Input& = delete;
    auto operator=(Input&&) -> Input& = delete;
    ~Input() = default;

    auto update() -> void;

private:

    bool is_x_clicked_{};
    bool is_mouse_button_down_{};

    int mouse_motion_x_{};
    int mouse_motion_y_{};

    int mouse_pos_x_{};
    int mouse_pos_y_{};

    std::unordered_set<SDL_Keycode> pressed_keys_{};

public:
    [[nodiscard]] auto is_x_clicked() const -> bool;
    [[nodiscard]] auto is_mouse_button_down() const -> bool;

    [[nodiscard]] auto mouse_motion_x() const -> int;
    [[nodiscard]] auto mouse_motion_y() const -> int;
    [[nodiscard]] auto mouse_pos_x() const -> int;
    [[nodiscard]] auto mouse_pos_y() const -> int;

    [[nodiscard]] auto pressed_keys() const -> std::unordered_set<SDL_Keycode> const&;
};

#endif // PLAYGROUND_INPUT_HPP
