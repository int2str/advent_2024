//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/14
//

#include <algorithm>  // IWYU pragma: keep
#include <ranges>
#include <vector>

#include "robots.hh"
#include "testrunner/testrunner.h"
#include "utils/coordinate.hh"

namespace Day14 {

void stepRobots(std::vector<Robot>& robots, Utils::Coordinate max) {
  for (auto& robot : robots) robot.move(max);
}

[[nodiscard]] constexpr auto safetyFactor(std::vector<Robot> robots,
                                          Utils::Coordinate max) -> size_t {
  const auto off_center = [&](Robot robot) {
    const auto center = max / 2;
    return robot.position.x != center.x and robot.position.y != center.y;
  };
  const auto quadrant = [&](Robot robot) {
    const auto center = max / 2;
    return (robot.position.y > center.y ? 2U : 0) |
           (robot.position.x > center.x ? 1U : 0);
  };
  const auto count = [](auto acc, auto quadrant) {
    ++acc[quadrant];
    return acc;
  };

  auto quadrants = robots                            //
                   | std::views::filter(off_center)  //
                   | std::views::transform(quadrant);

  return std::ranges::fold_left(
      std::ranges::fold_left(quadrants, std::array<size_t, 4>{}, count), 1U,
      std::multiplies{});
}

[[nodiscard]] constexpr auto safetyFactorAfter(
    size_t steps, Robots robots, Utils::Coordinate max) -> size_t {
  while ((steps--) != 0U) stepRobots(robots, max);
  return safetyFactor(robots, max);
}

[[nodiscard]] auto detectAnomaly(Robots robots, Utils::Coordinate max,
                                 double threshold) {
  auto steps = size_t{};
  while (distanceFrom(robots, max / 2) > threshold) {
    stepRobots(robots, max);
    ++steps;
  }
  return steps;
}

}  // namespace Day14

TEST(Day_14_Restroom_Redoubt_SAMPLE) {
  const auto robots    = Day14::robotsFromFile("14/sample.txt");
  const auto grid_size = Utils::Coordinate{11, 7};
  EXPECT_EQ(Day14::safetyFactorAfter(100, robots, grid_size), 12);
}

TEST(Day_14_Restroom_Redoubt_FINAL) {
  const auto ANOMALY_THRESHOLD = 25.0;
  const auto robots            = Day14::robotsFromFile("14/input.txt");
  const auto grid_size         = Utils::Coordinate{101, 103};
  EXPECT_EQ(Day14::safetyFactorAfter(100, robots, grid_size), 229'868'730);
  EXPECT_EQ(Day14::detectAnomaly(robots, grid_size, ANOMALY_THRESHOLD), 7'861);
}
