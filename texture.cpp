#include <SDL2/SDL_image.h>
#include <spdlog/spdlog.h>

#include "texture.hpp"

auto sdl_image_load(std::string const& filename)
{
    struct SdlSurfaceDeleter {
        void operator()(SDL_Surface* s) {
            SDL_FreeSurface(s);
        }
    };

    auto texture_data = std::unique_ptr<SDL_Surface, SdlSurfaceDeleter>(IMG_Load(filename.c_str()));
    return texture_data;
}

Texture::Texture(std::string const& filename) {
    load(filename);
}

auto Texture::load(std::string const& filename) -> bool
{
    auto texture_data = sdl_image_load(filename);
    if (!texture_data) {
        spdlog::error("failed to load texture data");
        return false;
    }
    auto width = texture_data->w;
    auto height = texture_data->h;

    auto* pixels = reinterpret_cast<Uint8*>(texture_data->pixels);

    auto depth = texture_data->format->BytesPerPixel;

    auto format = (depth == 4) ? GL_RGBA : GL_RGB;

    glGenTextures(1, &id_);
    bind();
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
    }
    unbind();

    return true;
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::unload()
{
    glDeleteTextures(1, &id_);
}
