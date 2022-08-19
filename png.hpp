//
// Created by slabko on 8/17/22.
//

#ifndef PLAYGROUND_PNG_HPP
#define PLAYGROUND_PNG_HPP

#include <string>
#include <vector>

namespace png {

struct PngData {
    std::vector<uint8_t> pixels{};
    int width{};
    int height{};
    int bit_depth{};
    int color_type{};
    int channels{};
};

auto read_png(std::string const& filepath) -> PngData;

} // namespace png

#endif // PLAYGROUND_PNG_HPP
