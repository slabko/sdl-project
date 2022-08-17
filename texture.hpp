#ifndef PLAYGROUND_TEXTURE_HPP
#define PLAYGROUND_TEXTURE_HPP

#include <glad/glad.h>
#include <string>

class Texture {
public:
    explicit Texture(std::string const& filename);

    void bind();
    static void unbind();
    void unload();

private:
    GLuint id_{};

    auto load(std::string const& filename) -> bool;
};

#endif // PLAYGROUND_TEXTURE_HPP
