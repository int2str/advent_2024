#ifndef COORDINATE_HH
#define COORDINATE_HH

#include <compare>  // IWYU pragma: keep

namespace Utils {

struct Coordinate {
  int x{};
  int y{};

  [[nodiscard]] constexpr auto operator<=>(const Coordinate&) const = default;

  constexpr auto operator+=(const Coordinate& other) -> Coordinate& {
    x += other.x;
    y += other.y;
    return *this;
  }

  constexpr auto operator-=(const Coordinate& other) -> Coordinate& {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  void rotateCW() { *this = Coordinate{-y, x}; }
};

}  // namespace Utils

[[nodiscard]] constexpr auto operator+(const Utils::Coordinate& lhs,
                                       const Utils::Coordinate& rhs)
    -> Utils::Coordinate {
  return {.x = lhs.x + rhs.x, .y = lhs.y + rhs.y};
}

[[nodiscard]] constexpr auto operator-(const Utils::Coordinate& lhs,
                                       const Utils::Coordinate& rhs)
    -> Utils::Coordinate {
  return {.x = lhs.x - rhs.x, .y = lhs.y - rhs.y};
}

#endif  // COORDINATE_HH
