#ifndef COORDINATE_HH
#define COORDINATE_HH

#include <array>
#include <compare>  // IWYU pragma: keep

namespace Utils {

struct Coordinate {
  int x{};
  int y{};

  [[nodiscard]] constexpr auto operator<=>(const Coordinate&) const = default;

  // Math

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

  // Transform

  void rotateClockwise() { *this = Coordinate{-y, x}; }

  void flip() { *this = Coordinate{-x, -y}; }

  // Info

  [[nodiscard]] constexpr auto neighbors() const -> std::array<Coordinate, 8> {
    return {Coordinate{x - 1, y - 1},
            {x, y - 1},
            {x + 1, y - 1},
            {x - 1, y},
            {x + 1, y},
            {x - 1, y + 1},
            {x, y + 1},
            {x + 1, y + 1}};
  }

  [[nodiscard]] constexpr auto orthogonalNeighbors() const
      -> std::array<Coordinate, 4> {
    return {Coordinate{x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}};
  }

  [[nodiscard]] constexpr auto diagonalNeighbors() const
      -> std::array<Coordinate, 4> {
    return {Coordinate{x - 1, y - 1},
            {x + 1, y + 1},
            {x + 1, y - 1},
            {x - 1, y + 1}};
  }
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
