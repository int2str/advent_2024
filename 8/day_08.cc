//
// int2str's Advent of CodeG 2024
// https://adventofcode.com/2024/day/8
//

#include <filesystem>
#include <unordered_map>

#include "testrunner/testrunner.h"
#include "utils/coordinate.hh"
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

  // NOLINTNEXTLINE
  void operator()(size_t x, size_t y, char chr) {
    const auto coordinate =
        Coordinate{.x = static_cast<int>(x), .y = static_cast<int>(y)};
    size.x = std::max(size.x, coordinate.x + 1);
    size.y = std::max(size.y, coordinate.y + 1);
    if (chr != '.') {
      frequencies[chr].insert(coordinate);
      antennae.insert(coordinate);
    }
  }
};

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

TEST(Day_08_Resonant_Collinearity_SAMPLE) {
  const auto map = Utils::readFileXY("8/sample.txt", Day8::Map{});
  EXPECT_EQ(Day8::antiNodes(map), 14);
  EXPECT_EQ(Day8::harmonicAntiNodes(map), 34);
}

TEST(Day_08_Resonant_Collinearity_FINAL) {
  const auto map = Utils::readFileXY("8/input.txt", Day8::Map{});
  EXPECT_EQ(Day8::antiNodes(map), 336);
  EXPECT_EQ(Day8::harmonicAntiNodes(map), 1131);
}
