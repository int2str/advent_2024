#ifndef COORDINATE_MAP_HH
#define COORDINATE_MAP_HH

#include <unordered_map>

#include "coordinate.hh"

namespace Utils {

struct CoordinateHash {
  [[nodiscard]] constexpr auto operator()(const Coordinate& coord) const
      -> int {
    return static_cast<int>(std::hash<int>()(coord.x) ^
                            std::hash<int>()(coord.y));
  }
};

template <typename T>
using CoordinateMap = std::unordered_map<Coordinate, T, CoordinateHash>;

}  // namespace Utils

#endif  // COORDINATE_MAP_HH
