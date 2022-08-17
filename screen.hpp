#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <SDL2/SDL.h>
#include <memory>

class Screen {

public:
    explicit Screen(int height = 640, int width = 480);
    Screen(Screen const&) = delete;
    Screen(Screen&&) = delete;
    auto operator=(Screen const&) -> Screen& = delete;
    auto operator=(Screen&&) -> Screen& = delete;
    ~Screen() = default;

    auto initialize() -> bool;
    static auto start_frame() -> void;
    static auto clear_screen() -> void;
    auto present() -> void;
    auto shutdown() -> void;

    auto window() -> SDL_Window* { return window_; }
    auto context() -> SDL_GLContext { return context_; }

private:
    int height_{640};
    int width_{480};

    SDL_Window* window_{};
    SDL_GLContext context_{};
};

#endif
