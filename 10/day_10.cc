//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/10
//

#include <fmt/core.h>

#include <algorithm>  // IWYU pragma: keep
#include <filesystem>
#include <fstream>

#include "testrunner/testrunner.h"
#include "utils/coordinate.hh"
#include "utils/coordinate_set.hh"
#include "utils/grid.hh"

namespace Day10 {

using ElevationGrid =
    Utils::Grid<char, Utils::OutOfBoundsPolicy::Default<char{}>>;

[[nodiscard]] auto makeGrid(const std::filesystem::path& path)
    -> ElevationGrid {
  auto file = std::ifstream(path);
  return ElevationGrid::from(file);
}

[[nodiscard]] constexpr auto reachablePeaks(const ElevationGrid& grid)
    -> size_t {
  const auto is_head = [&](auto coordinate) { return grid[coordinate] == '0'; };
  const auto is_peak = [&](auto coordinate) { return grid[coordinate] == '9'; };

  const auto peaks_reached = [&](this auto self, auto from) {
    if (is_peak(from)) return std::vector<Utils::Coordinate>{from};
    return from.orthogonalNeighbors()  //
           | std::views::filter(
                 [&](auto to) { return (grid[to] - grid[from]) == 1; })  //
           | std::views::transform(self)                                 //
           | std::views::join                                            //
           | std::ranges::to<std::vector>();
  };

  const auto unique_peaks = [&](auto peaks) {
    return (peaks | std::ranges::to<Utils::CoordinateSet>()).count();
  };

  auto peaks = grid.coordinates()                      //
               | std::views::filter(is_head)           //
               | std::views::transform(peaks_reached)  //
               | std::views::transform(unique_peaks);
  return std::ranges::fold_left(peaks, 0U, std::plus{});
}

[[nodiscard]] constexpr auto trailRatings(const ElevationGrid& grid) -> size_t {
  const auto is_head = [&](auto coordinate) { return grid[coordinate] == '0'; };
  const auto is_peak = [&](auto coordinate) { return grid[coordinate] == '9'; };

  auto count_paths_to_top = [&](this auto self, auto from) {
    if (is_peak(from)) return 1;
    auto next = from.orthogonalNeighbors()  //
                | std::views::filter(
                      [&](auto to) { return (grid[to] - grid[from]) == 1; })  //
                | std::views::transform(self);
    return std::ranges::fold_left(next, 0, std::plus{});
  };

  auto paths_from_heads = grid.coordinates()             //
                          | std::views::filter(is_head)  //
                          | std::views::transform(count_paths_to_top);
  return std::ranges::fold_left(paths_from_heads, 0U, std::plus{});
}

}  // namespace Day10

TEST(Day_10_Hoof_It_SAMPLE) {
  const auto grid = Day10::makeGrid("10/sample.txt");
  EXPECT_EQ(Day10::reachablePeaks(grid), 36);
  EXPECT_EQ(Day10::trailRatings(grid), 81);
}

TEST(Day_10_Hoof_It_FINAL) {
  const auto grid = Day10::makeGrid("10/input.txt");
  EXPECT_EQ(Day10::reachablePeaks(grid), 688);
  EXPECT_EQ(Day10::trailRatings(grid), 1459);
}
