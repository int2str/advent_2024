#ifndef DAY_6_MAP_HH
#define DAY_6_MAP_HH

#include "utils/coordinate.hh"

namespace Day6 {

struct Map {
  Coordinate size{};
  Coordinate guard{};
  CoordinateSet blocked{};  // NOLINT

  static constexpr auto start_direction = Coordinate{0, -1};

  // NOLINTNEXTLINE
  void operator()(std::size_t x, std::size_t y, char chr) {
    const auto pos =
        Coordinate{.x = static_cast<int>(x), .y = static_cast<int>(y)};
    size.x = std::max(size.x, pos.x + 1);
    size.y = std::max(size.y, pos.y + 1);

    if (chr == '^') guard = pos;
    if (chr == '#') blocked.insert(pos);
  }
};

}  // namespace Day6

#endif  // DAY_6_MAP_HH
