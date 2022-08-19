#include "texture.hpp"
#include "png.hpp"

Texture::Texture(std::string const& filename)
{
    load(filename);
}

auto Texture::load(std::string const& filename) -> bool
{
    auto const png = png::read_png(filename);
    auto format = (png.channels == 4) ? GL_RGBA : GL_RGB;

    glGenTextures(1, &id_);
    bind();
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, format, png.width, png.height, 0, format, GL_UNSIGNED_BYTE, png.pixels.data());
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
