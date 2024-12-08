#ifndef STATE_HH
#define STATE_HH

#include <cstdint>
#include <vector>

#include "map.hh"
#include "utils/coordinate.hh"

enum class Mode : uint8_t { Tracing, Probing, Done };

struct State {
  Map map;
  Mode mode = Mode::Tracing;

  Coordinate guard_at{};
  Coordinate guard_direction{};

  CoordinateSet visited{};
  CoordinateSet candidates{};
  CoordinateSet::Iterator next_candidate{};

  Coordinate candidate_at{};

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
    candidates_attempted  = {};
    obstruction_positions = {};
    nextCandidate();
  }

  void nextCandidate() {
    if (candidates_attempted++ != 0) map.blocked.erase(candidate_at);
    candidate_at = *next_candidate++;
    map.blocked.insert(candidate_at);
    resetGuard();
  };

  [[nodiscard]] constexpr auto noMoreCandidates() const -> bool {
    return next_candidate == candidates.end();
  }
};

#endif  // STATE_HH
