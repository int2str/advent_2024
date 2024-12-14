#include "robots.hh"

#include <filesystem>
#include <ranges>
#include <string>
#include <vector>

#include "utils/coordinate.hh"
#include "utils/nm_view.hh"
#include "utils/read_file.hh"
#include "utils/split.hh"

namespace Day14::Internal {

[[nodiscard]] auto filterLine(const std::string& line) -> std::string {
  const auto no_pv = [](char ch) {
    return ch != 'p' and ch != 'v' and ch != '=';
  };
  const auto space_comma = [](char ch) { return ch == ' ' ? ',' : ch; };
  return line                                  //
         | std::views::filter(no_pv)           //
         | std::views::transform(space_comma)  //
         | std::ranges::to<std::string>();
}

[[nodiscard]] auto makeRobot(const std::string& filtered) -> Robots {
  const auto make_coordinate = [](const auto& chunk) {
    return Utils::Coordinate{*chunk.begin(), *std::next(chunk.begin())};
  };
  const auto make_robot = [](auto position, auto velocity) {
    return Robot{position, velocity};
  };
  return Utils::split<int>(filtered, ",")          //
         | std::views::chunk(2)                    //
         | std::views::transform(make_coordinate)  //
         | std::views::pairwise_transform(make_robot) |
         std::ranges::to<std::vector>();
}

}  // namespace Day14::Internal

namespace Day14 {

auto robotsFromFile(const std::filesystem::path& path) -> Robots {
  return Utils::readLines(path)                         //
         | std::views::transform(Internal::filterLine)  //
         | std::views::transform(Internal::makeRobot)   //
         | std::views::join                             //
         | std::ranges::to<std::vector>();
}

auto distanceBetween(const Robots& robots) -> double {
  if (robots.empty()) return 0;
  auto distance = double{};
  for (const auto [first, second] : Utils::nm_const_view(robots))
    distance += first->position.distanceFrom(second->position);
  return distance / static_cast<double>(robots.size() * robots.size());
}

auto distanceFrom(const Robots& robots, Utils::Coordinate where) -> double {
  if (robots.empty()) return 0;
  auto distance = double{};
  for (const auto& robot : robots) {
    distance += robot.position.distanceFrom(where);
  }
  return distance / static_cast<double>(robots.size());
}

}  // namespace Day14
