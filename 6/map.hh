#ifndef MAP_HH
#define MAP_HH

#include <filesystem>
#include <ranges>
#include <unordered_set>

#include "coordinate.hh"
#include "utils/read_file.hh"

struct Map {
  Coordinate size{};
  Coordinate guard{};
  std::unordered_set<Coordinate, CoordinateHash> blocks{};

  static constexpr auto start_direction = Coordinate{0, -1};

  [[nodiscard]] static auto fromFile(const std::filesystem::path& path) -> Map {
    auto map = Map{};
    for (const auto& [y, line] :
         Utils::readLines(path) | std::views::enumerate) {
      map.size.y = static_cast<int>(y + 1);
      for (const auto& [x, chr] : line | std::views::enumerate) {
        map.size.x = static_cast<int>(x + 1);
        if (chr == '^')
          map.guard = {.x = static_cast<int>(x), .y = static_cast<int>(y)};
        if (chr == '#')
          map.blocks.insert({static_cast<int>(x), static_cast<int>(y)});
      }
    }
    return map;
  }
};

#endif  // MAP_HH
