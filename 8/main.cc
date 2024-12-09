//
// int2str's Advent of CodeG 2024
// https://adventofcode.com/2024/day/8
//

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <filesystem>
#include <ranges>
#include <unordered_map>

#include "utils/coordinate.hh"
#include "utils/curry.hh"
#include "utils/nm_view.hh"
#include "utils/read_file.hh"

namespace Day8 {

using LookupTable = std::unordered_map<char, CoordinateSet>;

struct Map {
  LookupTable frequencies;
  CoordinateSet antennae;
  Coordinate size;

  [[nodiscard]] constexpr auto inBounds(const Coordinate& coordinate) const
      -> bool {
    return coordinate.x >= 0 and coordinate.y >= 0 and coordinate.x < size.x and
           coordinate.y < size.y;
  };
};

[[nodiscard]] auto makeMap(const std::filesystem::path& path) -> Map {
  auto map = Map{};
  for (const auto& [y, line] : Utils::readLines(path) | std::views::enumerate) {
    map.size.y = static_cast<int>(y + 1);
    map.size.x = static_cast<int>(line.length());
    auto chars =
        line                     //
        | std::views::enumerate  //
        | std::views::filter(
              [](const auto& tuple) { return std::get<1>(tuple) != '.'; })  //
        | std::views::transform(Utils::uncurry([&](int x, auto frequency_code) {
            return std::tuple(frequency_code,
                              Coordinate{.x = x, .y = static_cast<int>(y)});
          }));
    for (const auto& [frequency_code, antenna] : chars) {
      map.frequencies[frequency_code].insert(antenna);
      map.antennae.insert(antenna);
    }
  }
  return map;
}

[[nodiscard]] auto antiNodes(const Map& map) -> size_t {
  auto anti_nodes = CoordinateSet{};

  const auto add_node = [&](const Coordinate& coordinate) {
    if (map.inBounds(coordinate)) anti_nodes.insert(coordinate);
  };

  for (const auto& [_, antennae] : map.frequencies) {
    for (auto [a, b] : Utils::nm_const_view(antennae)) {
      add_node(*a + (*a - *b));
      add_node(*b + (*b - *a));
    }
  }

  return anti_nodes.count();
}

[[nodiscard]] auto harmonicAntiNodes(const Map& map) -> size_t {
  auto anti_nodes = map.antennae;

  const auto add_node = [&](const Coordinate& coordinate) {
    if (map.inBounds(coordinate)) {
      anti_nodes.insert(coordinate);
      return true;
    }
    return false;
  };

  for (const auto& [_, antennae] : map.frequencies) {
    for (auto [a, b] : Utils::nm_const_view(antennae)) {
      const auto diff = *a - *b;
      auto nodes_plus = *a + diff;
      while (add_node(nodes_plus)) nodes_plus += diff;
      auto nodes_minus = *b - diff;
      while (add_node(nodes_minus)) nodes_minus -= diff;
    }
  }

  return anti_nodes.count();
}

}  // namespace Day8

auto main() -> int {
  const auto map = Day8::makeMap("8/input.txt");

  fmt::print("Day 8\n-----\n");
  fmt::print("Part 1 | Anti-node count    : {}\n", Day8::antiNodes(map));
  fmt::print("Part 2 | Harmonic anti-nodes: {}\n\n",
             Day8::harmonicAntiNodes(map));
}
