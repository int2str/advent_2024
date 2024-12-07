#ifndef COORDINATE_HH
#define COORDINATE_HH

// For std::hash():
#include <unordered_set>  // IWYU pragma: keep

struct Coordinate {
  int x;
  int y;

  [[nodiscard]] constexpr auto operator<=>(const Coordinate&) const = default;

  [[nodiscard]] constexpr auto operator+(const Coordinate& rhs) const
      -> Coordinate {
    return {.x = x + rhs.x, .y = y + rhs.y};
  }

  void rotateCW() { *this = Coordinate{-y, x}; }
};

struct CoordinateHash {
  [[nodiscard]] constexpr auto operator()(const Coordinate& coord) const
      -> int {
    return static_cast<int>(std::hash<int>()(coord.x) ^
                            std::hash<int>()(coord.y));
  }
};

#endif  // COORDINATE_HH
