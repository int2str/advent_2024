#ifndef COORDINATE_DIRECTIONS_HH
#define COORDINATE_DIRECTIONS_HH

#include "coordinate.hh"

namespace Utils {

struct Directions {
  [[nodiscard]] static constexpr auto left() -> Coordinate {
    return {.x = -1, .y = 0};
  }

  [[nodiscard]] static constexpr auto right() -> Coordinate {
    return {.x = 1, .y = 0};
  }

  [[nodiscard]] static constexpr auto up() -> Coordinate {
    return {.x = 0, .y = -1};
  }

  [[nodiscard]] static constexpr auto down() -> Coordinate {
    return {.x = 0, .y = 1};
  }

  [[nodiscard]] static constexpr auto upLeft() -> Coordinate {
    return up() + left();
  }

  [[nodiscard]] static constexpr auto upRight() -> Coordinate {
    return up() + right();
  }

  [[nodiscard]] static constexpr auto downLeft() -> Coordinate {
    return down() + left();
  }

  [[nodiscard]] static constexpr auto downRight() -> Coordinate {
    return down() + right();
  }

  [[nodiscard]] static constexpr auto clockwise() -> std::array<Coordinate, 8> {
    return {up(),   upRight(),  right(), downRight(),
            down(), downLeft(), left(),  upLeft()};
  }
};

}  // namespace Utils

#endif  // COORDINATE_DIRECTIONS_HH
