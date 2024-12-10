#ifndef COORDINATE_MAP_HH
#define COORDINATE_MAP_HH

#include <unordered_map>

#include "coordinate.hh"

namespace Utils {

template <typename T>
using CoordinateMap = std::unordered_map<Coordinate, T, CoordinateHash>;

}  // namespace Utils

#endif  // COORDINATE_MAP_HH
