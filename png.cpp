//
// Created by slabko on 8/17/22.
//

#include <array>
#include <fstream>
#include <iostream>

#include <png.h>

#include "png.hpp"

/* Insparation *
 * http://www.libpng.org/pub/png/libpng-1.2.5-manual.html
 * https://www.piko3d.net/tutorials/libpng-tutorial-loading-png-files-from-streams/
 * https://jeromebelleman.gitlab.io/posts/devops/libpng/
 */

namespace png {

void read_chunk(png_structp pngptr, png_bytep data, png_size_t size)
{
    auto* source = reinterpret_cast<std::ifstream*>(png_get_io_ptr(pngptr));
    source->read(reinterpret_cast<char*>(data), static_cast<int64_t>(size));
}

// Use RAII for raw C pointers
struct PngHandle final {
    png_structp pngptr{};
    png_infop pnginfo{};

    PngHandle() = default;
    PngHandle(PngHandle const&) = delete;
    PngHandle(PngHandle&&) = delete;

    auto operator=(PngHandle&&) -> PngHandle& = delete;
    auto operator=(PngHandle const&) -> PngHandle& = delete;

    ~PngHandle() {
        png_destroy_read_struct(&pngptr, &pnginfo, nullptr);
    }

    void init() {
        pngptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!pngptr) {
            throw std::runtime_error("Failed to create a PNG read structure");
        }

        pnginfo = png_create_info_struct(pngptr);
        if (!pnginfo) {
            throw std::runtime_error("Failed to create a PNG info structure");
        }
    }
};

auto read_png(std::string const& filepath) -> PngData
{
    std::ifstream file_stream(filepath, std::ios::binary);
    if (!file_stream) {
        throw std::runtime_error("Filed to open the file");
    }

    file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    const size_t header_length = 8;
    std::array<png_byte, header_length> header{};
    file_stream.read(reinterpret_cast<char*>(header.data()), header_length);
    int signature = png_sig_cmp(header.data(), 0, header_length);
    if (signature != 0) {
        throw std::runtime_error("Not a PNG file");
    }

    PngHandle p{};
    p.init();

    if (setjmp(png_jmpbuf(p.pngptr))) { // NOLINT(cert-err52-cpp)
        // Everything is confirms to RAII, so there is no need to clean up
        throw std::runtime_error("LibPNG Error");
    }

    png_set_read_fn(p.pngptr, reinterpret_cast<png_voidp>(&file_stream), read_chunk);

    png_set_sig_bytes(p.pngptr, header_length);
    png_set_palette_to_rgb(p.pngptr);

    png_read_info(p.pngptr, p.pnginfo);

    png_uint_32 width{};
    png_uint_32 height{};
    int bit_depth{};
    int color_type{};

    png_get_IHDR(p.pngptr, p.pnginfo, &width, &height, &bit_depth, &color_type, nullptr, nullptr, nullptr);
    int channels = png_get_channels(p.pngptr, p.pnginfo);

    std::vector<png_bytep> rows(height);
    const size_t row_size = static_cast<size_t>(width) * channels; // assuming bit_depth = 8
    std::vector<uint8_t> pixels(row_size * height);

    for (size_t i = 0; i < height; ++i) {
        unsigned int offset = i * row_size;
        rows[i] = &pixels[offset];
    }

    png_read_image(p.pngptr, rows.data());

    return {
      std::move(pixels),
      static_cast<int>(width),
      static_cast<int>(height),
      bit_depth,
      color_type,
      channels
    };
}

} // namespace png
