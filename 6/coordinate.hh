#ifndef COORDINATE_HH
#define COORDINATE_HH

#include <cstdint>

struct Coordinate {
  int x;
  int y;

  [[nodiscard]] constexpr auto operator<=>(const Coordinate&) const = default;

  [[nodiscard]] constexpr auto operator+(const Coordinate& rhs) const
      -> Coordinate {
    return {.x = x + rhs.x, .y = y + rhs.y};
  }

  void rotateCW() { *this = Coordinate{-y, x}; }

  [[nodiscard]] constexpr auto idx() const -> uint16_t {
    // NOLINTNEXTLINE
    return static_cast<uint16_t>((static_cast<uint16_t>(y) << 8) +
                                 (static_cast<uint16_t>(x) & 0xFF));  // NOLINT
  }
};

#endif  // COORDINATE_HH
