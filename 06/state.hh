#ifndef DAY_6_STATE_HH
#define DAY_6_STATE_HH

#include <cstdint>

#include "map.hh"
#include "utils/coordinate.hh"
#include "utils/coordinate_set.hh"

namespace Day6 {

enum class Mode : uint8_t { Tracing, Probing, Done };

struct State {
  Map map;
  Mode mode = Mode::Tracing;

  Utils::Coordinate guard_at{};
  Utils::Coordinate guard_direction{};

  Utils::CoordinateSet visited{};
  Utils::CoordinateSet candidates{};
  Utils::CoordinateSet::Iterator next_candidate{};

  Utils::Coordinate candidate_at{};

  size_t candidates_attempted{};
  size_t obstruction_positions{};
  size_t travelled{};

  void resetGuard() {
    guard_at        = map.guard;
    guard_direction = Map::start_direction;
    visited         = {};
    travelled       = {};
  }

  void switchToProbing() {
    candidates = visited;
    candidates.erase(map.guard);
    next_candidate = candidates.begin();

    mode                  = Mode::Probing;
    candidates_attempted  = 1;
    obstruction_positions = {};
    candidate_at          = *next_candidate;
    map.blocked.insert(candidate_at);
    resetGuard();
  }

  void nextCandidate() {
    map.blocked.erase(candidate_at);
    if (++next_candidate != candidates.end()) {
      ++candidates_attempted;
      candidate_at = *next_candidate;
      map.blocked.insert(candidate_at);
      resetGuard();
    }
  };

  [[nodiscard]] constexpr auto noMoreCandidates() const -> bool {
    return next_candidate == candidates.end();
  }
};

}  // namespace Day6

#endif  // DAY_6_STATE_HH
