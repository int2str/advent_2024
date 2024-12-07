#ifndef MAP_HH
#define MAP_HH

#include <bitset>
#include <filesystem>
#include <ranges>

#include "coordinate.hh"
#include "utils/read_file.hh"

// This limits us to 256x256...
using MapBits = std::bitset<65'536>;  // NOLINT

struct Map {
  Coordinate size{};
  Coordinate guard{};
  MapBits blocked{};  // NOLINT

  static constexpr auto start_direction = Coordinate{0, -1};

  [[nodiscard]] static auto fromFile(const std::filesystem::path& path) -> Map {
    auto map = Map{};
    for (const auto& [y, line] :
         Utils::readLines(path) | std::views::enumerate) {
      map.size.y = static_cast<int>(y + 1);
      for (const auto& [x, chr] : line | std::views::enumerate) {
        map.size.x = static_cast<int>(x + 1);
        const auto pos =
            Coordinate{.x = static_cast<int>(x), .y = static_cast<int>(y)};
        if (chr == '^') map.guard = pos;
        if (chr == '#') map.blocked.set(pos.idx());
      }
    }
    return map;
  }
};

#endif  // MAP_HH
