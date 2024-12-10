//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/10
//

#include <fmt/core.h>

#include <algorithm>  // IWYU pragma: keep

#include "testrunner/testrunner.h"
#include "utils/coordinate.hh"
#include "utils/coordinate_map.hh"
#include "utils/read_file.hh"
#include "utils/xy_array.hh"

namespace Day10 {

class HeightMap {
 public:
  static constexpr auto MAX_WIDTH  = 256ULL;
  static constexpr auto MAX_HEIGHT = 256ULL;

  static constexpr auto PEAK      = uint8_t{9};
  static constexpr auto TRAILHEAD = uint8_t{0};

  Utils::xy_array<uint8_t, MAX_WIDTH, MAX_HEIGHT> elevation{};
  Utils::CoordinateMap<Utils::CoordinateMap<size_t>> paths_to_peaks{};

  std::size_t width{};
  std::size_t height{};

  Utils::CoordinateSet trailheads{};

  [[nodiscard]] constexpr auto inBounds(
      const Utils::Coordinate& coordinate) const -> bool {
    return coordinate.x >= 0 and coordinate.x < static_cast<int>(width) and
           coordinate.y >= 0 and coordinate.y < static_cast<int>(height);
  }

  [[nodiscard]] constexpr auto elevationAt(
      const Utils::Coordinate& coordinate) const -> uint8_t {
    if (!inBounds(coordinate)) return static_cast<uint8_t>(-1);
    return elevation[static_cast<size_t>(coordinate.x),
                     static_cast<size_t>(coordinate.y)];
  }

  // NOLINTNEXTLINE
  void operator()(std::size_t x, std::size_t y, char chr) {
    width  = std::max(width, x + 1);
    height = std::max(height, y + 1);

    const auto current_elevation = static_cast<uint8_t>(chr - '0');
    elevation[x, y]              = current_elevation;
    if (current_elevation == TRAILHEAD) {
      trailheads.insert(Utils::Coordinate{.x = static_cast<int>(x),
                                          .y = static_cast<int>(y)});
    }
  }
};

constexpr auto offsets = std::array<Utils::Coordinate, 4>{
    Utils::Coordinate{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

void markAdjacent(HeightMap& map, Utils::Coordinate position,
                  Utils::Coordinate head) {
  if (map.elevation[position] == HeightMap::PEAK) {
    ++map.paths_to_peaks[head][position];
    return;
  }

  for (const auto& offset : offsets) {
    const auto adjacent = position + offset;
    if (map.elevationAt(adjacent) == (map.elevationAt(position) + 1))
      markAdjacent(map, adjacent, head);
  }
}

[[nodiscard]] auto makeMap(const std::filesystem::path& path) -> HeightMap {
  auto map = Utils::readFileXY(path, HeightMap{});
  for (const auto head : map.trailheads) markAdjacent(map, head, head);
  return map;
}

[[nodiscard]] auto trailRatings(const HeightMap& map)
    -> std::pair<size_t, size_t> {
  auto result = std::pair<size_t, size_t>{};
  for (const auto& [head, peaks] : map.paths_to_peaks) {
    result.first += peaks.size();
    for (const auto& [peak, paths] : peaks) result.second += paths;
  }
  return result;
}

}  // namespace Day10

TEST(Day_10_Hoof_It_SAMPLE) {
  const auto [reachable_peaks, paths_to_peaks] =
      Day10::trailRatings(Day10::makeMap("10/sample.txt"));
  EXPECT_EQ(reachable_peaks, 36);
  EXPECT_EQ(paths_to_peaks, 81);
}

TEST(Day_10_Hoof_It_FINAL) {
  const auto [reachable_peaks, paths_to_peaks] =
      Day10::trailRatings(Day10::makeMap("10/input.txt"));
  EXPECT_EQ(reachable_peaks, 688);
  EXPECT_EQ(paths_to_peaks, 1459);
}
