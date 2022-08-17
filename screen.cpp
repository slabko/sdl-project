// #include <GL/gl.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

#include "screen.hpp"


Screen::Screen(int height, int width) :
  height_(height), width_(width) {}

auto Screen::initialize() -> bool
{
    spdlog::info("initializing");

    auto sdl_init_res = SDL_Init(SDL_INIT_EVERYTHING);
    if (sdl_init_res == -1) {
        spdlog::critical("Error initializing SDL");
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    window_ = SDL_CreateWindow(
      "Graphics Engine",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      height_, width_,
      SDL_WINDOW_OPENGL);
    if (!window_) {
        spdlog::critical("Error creating SDL window");
        return false;
    }

    context_ = SDL_GL_CreateContext(window_);
    if (!context_) {
        spdlog::critical("Error creating OpenGL context");
        return false;
    }

    auto glad_load_res = gladLoadGL();
    if (!glad_load_res) {
        spdlog::critical("Error loading OpenGL extension");
        return false;
    }

    /* 
    auto relative_mouse_mode_res = SDL_SetRelativeMouseMode(SDL_TRUE);
    if (relative_mouse_mode_res) {
        spdlog::critical("Error setting relative mouse mode");
    }
    */

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(1.0F, 1.0F, 1.0F, 1.0F);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsLight();

    ImGui_ImplSDL2_InitForOpenGL(window_, context_);
    ImGui_ImplOpenGL3_Init("#version 460");

    return true;
}

auto Screen::start_frame() -> void
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

auto Screen::clear_screen() -> void
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

auto Screen::present() -> void
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window_);
}

auto Screen::shutdown() -> void
{
    spdlog::info("shutting down");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(context_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}
