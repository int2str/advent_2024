#ifndef DAY_14_ROBOTS_HH
#define DAY_14_ROBOTS_HH

#include <filesystem>
#include <vector>

#include "utils/coordinate.hh"

namespace Day14 {

struct Robot {
  Utils::Coordinate position{};
  Utils::Coordinate velocity{};

  void move(Utils::Coordinate max) {
    position += velocity;
    if (position.y < 0) position.y = max.y + position.y;
    if (position.y >= max.y) position.y = position.y - max.y;
    if (position.x < 0) position.x = max.x + position.x;
    if (position.x >= max.x) position.x = position.x - max.x;
  }
};

using Robots = std::vector<Robot>;

[[nodiscard]] auto robotsFromFile(const std::filesystem::path& path) -> Robots;

[[nodiscard]] auto distanceBetween(const Robots& robots) -> double;

[[nodiscard]] auto distanceFrom(const Robots& robots,
                                Utils::Coordinate where) -> double;

}  // namespace Day14

#endif  // DAY_14_ROBOTS_HH
