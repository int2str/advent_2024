#ifndef COORDINATE_STEP_MAP_HH
#define COORDINATE_STEP_MAP_HH

#include <unordered_map>

#include "coordinate_map.hh"
#include "coordinate_step.hh"

namespace Utils {

struct StepHash {
  [[nodiscard]] constexpr auto operator()(const Step& step) const -> int {
    return static_cast<int>(Utils::CoordinateHash()(step.position) ^
                            Utils::CoordinateHash()(step.direction));
  }
};

template <typename T>
using StepMap = std::unordered_map<Step, T, StepHash>;

}  // namespace Utils

#endif  // COORDINATE_STEP_MAP_HH
