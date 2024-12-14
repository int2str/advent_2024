//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/14
//

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <ranges>
#include <vector>

#include "robots.hh"
#include "utils/coordinate.hh"
#include "utils/coordinate_formatter.hh"
#include "window.hh"

namespace Day14 {

void animate(std::vector<Robot> robots, Utils::Coordinate max) {
  auto steps = size_t{};

  auto window     = Window(max);
  auto min        = 99999999.9;
  auto anomaly    = robots;
  auto anomaly_at = steps;

  while (window.isOpen()) {
    window.handleEvents();

    ++steps;
    for (auto& robot : robots) robot.move(max);

    const auto dist = distanceBetween(robots);
    if (dist <= min) {
      min        = dist;
      anomaly    = robots;
      anomaly_at = steps;
      fmt::print("{:6.1F} @ {}\n", min, steps);
    }
    window.draw(anomaly, anomaly_at, min, steps);
  }
  fmt::print("Closed on step: {})\n", steps);
}

}  // namespace Day14

auto main() -> int {
  const auto robots = Day14::robotsFromFile("14/input.txt");
  animate(robots, Utils::Coordinate{101, 103});
}
