cmake_minimum_required(VERSION 3.22)

# create `compile_commands.json` for clangd and autocompletion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-march=native)
add_compile_options(-ffast-math)
add_compile_options(-Wall)

project(playground)

configure_file(textures/crate.png textures/crate.png COPYONLY)
configure_file(shaders/main.vert shaders/main.vert COPYONLY)
configure_file(shaders/main.frag shaders/main.frag COPYONLY)

find_package(spdlog REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(PNG REQUIRED)

add_executable(playground 
    main.cpp 
    screen.cpp 
    input.cpp 
    shader.cpp
    buffer.cpp
    cube.cpp
    camera.cpp
    texture.cpp
    png.cpp 

    glad/src/glad.c 
    imgui/src/imgui.cpp
    imgui/src/imgui_demo.cpp
    imgui/src/imgui_draw.cpp
    imgui/src/imgui_tables.cpp
    imgui/src/imgui_widgets.cpp
    imgui/src/imgui_impl_opengl3.cpp
    imgui/src/imgui_impl_sdl.cpp
)

# target_link_libraries(playground spdlog::spdlog OpenGL::GL glm::glm ${SDL2_LIBRARIES})
target_link_libraries(playground 
    spdlog::spdlog 
    OpenGL::GL 
    glm::glm 
    SDL2::SDL2 
    PNG::PNG 
)

target_include_directories(playground PRIVATE glad/include imgui/include)
