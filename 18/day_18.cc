//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/18
//

#include <filesystem>
#include <ranges>

#include "testrunner/testrunner.h"
#include "utils/coordinate.hh"
#include "utils/coordinate_directions.hh"
#include "utils/coordinate_map.hh"  // IWYU pragma: keep
#include "utils/dijkstras.hh"
#include "utils/grid.hh"
#include "utils/read_file.hh"
#include "utils/split.hh"

namespace Day18 {

using Chunks = std::vector<Utils::Coordinate>;
using Edge   = Utils::WeightedEdge<int, Utils::Coordinate>;
using Grid   = Utils::Grid<char, Utils::OutOfBoundsPolicy::Default<'#'>>;

struct Map {
  Chunks chunks;
  size_t width;
};

[[nodiscard]] auto findEscapeLength(const Grid& grid) -> int {
  const auto start      = Utils::Coordinate(0, 0);
  const auto start_edge = Edge{0, start};
  const auto target     = Utils::Coordinate{static_cast<int>(grid.width()) - 1,
                                        static_cast<int>(grid.height()) - 1};

  const auto adjacent = [&](const auto& from) {
    const auto edges     = {Edge{1, from + Utils::Direction::up()},
                            Edge{1, from + Utils::Direction::down()},
                            Edge{1, from + Utils::Direction::left()},
                            Edge{1, from + Utils::Direction::right()}};
    const auto in_bounds = [&](auto to) { return grid[to.edge] != '#'; };
    return edges | std::views::filter(in_bounds) |
           std::ranges::to<std::vector>();
  };

  return Utils::dijkstra<int, Utils::Coordinate>(start_edge, target, adjacent);
}

[[nodiscard]] auto readChunks(const std::filesystem::path& path) -> Chunks {
  const auto split = [](auto line) { return Utils::split<int, 2>(line, ","); };
  const auto to_coordinate = [](auto pair) {
    return Utils::Coordinate{pair[0], pair[1]};
  };
  return Utils::readLines(path)                  //
         | std::views::transform(split)          //
         | std::views::transform(to_coordinate)  //
         | std::ranges::to<std::vector>();
}

[[nodiscard]] auto escape(const Map& map, size_t escape_at) -> int {
  auto grid = Grid{map.width, map.width};
  for (size_t i = 0; i != escape_at; ++i) grid[map.chunks[i]] = '#';
  return findEscapeLength(grid);
}

[[nodiscard]] auto trapped(const Map& map) -> Utils::Coordinate {
  auto grid = Grid{map.width, map.width};

  auto bottom = map.chunks.begin();
  auto top    = map.chunks.end() - 1;

  while (bottom < top) {
    auto mid = bottom + (top - bottom) / 2;

    grid.clear();
    for (auto it = map.chunks.begin(); it != mid; ++it) grid[*it] = '#';

    const auto distance_low = findEscapeLength(grid);
    if (distance_low == 0) {
      top = mid;
      continue;
    }

    grid[*mid] = '#';

    const auto distance_high = findEscapeLength(grid);
    if (distance_high == 0) return *mid;

    bottom = mid;
  }

  return Utils::Coordinate{};
}

}  // namespace Day18

TEST(Day_18_RAM_Run_SAMPLE) {
  const auto map = Day18::Map{Day18::readChunks("18/sample.txt"), 7U};
  EXPECT_EQ(Day18::escape(map, 12), 22);
  EXPECT_EQ(Day18::trapped(map), Utils::Coordinate(6U, 1U));
}

TEST(Day_18_RAM_Run_FINAL) {
  const auto map = Day18::Map{Day18::readChunks("18/input.txt"), 71U};
  EXPECT_EQ(Day18::escape(map, 1024), 344);
  EXPECT_EQ(Day18::trapped(map), Utils::Coordinate(46U, 18U));
}
